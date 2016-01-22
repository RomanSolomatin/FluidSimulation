#include "FluidSimulationPluginPrivatePCH.h"
#include "RHIStaticStates.h"

//It seems to be the convention to expose all vertex declarations as globals, and then reference them as externs in the headers where they are needed.
//It kind of makes sense since they do not contain any parameters that change and are purely used as their names suggest, as declarations :)
TGlobalResource<FTextureVertexDeclaration> GTextureVertexDeclaration;

FFluidSimulationPixelShaderInstance::FFluidSimulationPixelShaderInstance(ERHIFeatureLevel::Type ShaderFeatureLevel)
{
	FeatureLevel = ShaderFeatureLevel;

	ConstantParameters = FFluidSimulationPixelShaderConstantParameters();
	VariableParameters = FFluidSimulationPixelShaderVariableParameters();

	bMustRegenerateSRV = false;
	bIsPixelShaderExecuting = false;
	bIsUnloading = false;

	CurrentTexture = NULL;
	CurrentRenderTarget = NULL;
	DensityIn = NULL;
	DensityInSRV = NULL;
}

FFluidSimulationPixelShaderInstance::~FFluidSimulationPixelShaderInstance()
{
	bIsUnloading = true;
}

void FFluidSimulationPixelShaderInstance::ExecutePixelShader(UTextureRenderTarget2D* RenderTarget, FTexture3DRHIRef InputTexture)
{
	if (bIsUnloading || bIsPixelShaderExecuting)
	{
		return;
	}

	bIsPixelShaderExecuting = true;

	if (DensityIn != InputTexture)
	{
		bMustRegenerateSRV = true;
	}

	CurrentRenderTarget = RenderTarget;
	DensityIn = InputTexture;

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		FPixelShaderRunner,
		FFluidSimulationPixelShaderInstance*, PixelShader, this,
		{
		PixelShader->ExecutePixelShaderInternal();
		}
	);
}

void FFluidSimulationPixelShaderInstance::ExecutePixelShaderInternal()
{
	check(IsInRenderingThread());

	if (bIsUnloading)
	{
		if (NULL != DensityInSRV)
		{
			DensityInSRV.SafeRelease();
			DensityInSRV = NULL;
		}

		return;
	}

	FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();

	if (bMustRegenerateSRV)
	{
		bMustRegenerateSRV = false;

		if (NULL != DensityInSRV)
		{
			DensityInSRV.SafeRelease();
			DensityInSRV = NULL;
		}

		DensityInSRV = RHICreateShaderResourceView(DensityIn, 0);
	}

	CurrentTexture = CurrentRenderTarget->GetRenderTargetResource()->GetRenderTargetTexture();
	SetRenderTarget(RHICmdList, CurrentTexture, FTextureRHIRef());
	RHICmdList.SetBlendState(TStaticBlendState<>::GetRHI());
	RHICmdList.SetRasterizerState(TStaticRasterizerState<>::GetRHI());
	RHICmdList.SetDepthStencilState(TStaticDepthStencilState<false, CF_Always>::GetRHI());

	static FGlobalBoundShaderState BoundShaderState;
	TShaderMapRef<FFluidSimulationVertexShader> VertexShader(GetGlobalShaderMap(FeatureLevel));
	TShaderMapRef<FFluidSimulationPixelShader> PixelShader(GetGlobalShaderMap(FeatureLevel));

	SetGlobalBoundShaderState(RHICmdList, FeatureLevel, BoundShaderState, GTextureVertexDeclaration.VertexDeclarationRHI, *VertexShader, *PixelShader);

	PixelShader->SetSurfaces(RHICmdList, DensityInSRV);
	PixelShader->SetUniformBuffers(RHICmdList, ConstantParameters, VariableParameters);

	FTextureVertex Vertices[4];
	Vertices[0].Position = FVector4(-1.0f, 1.0f, 0, 1.0f);
	Vertices[1].Position = FVector4(1.0f, 1.0f, 0, 1.0f);
	Vertices[2].Position = FVector4(-1.0f, -1.0f, 0, 1.0f);
	Vertices[3].Position = FVector4(1.0f, -1.0f, 0, 1.0f);
	Vertices[0].UV = FVector2D(0, 1);
	Vertices[1].UV = FVector2D(1, 1);
	Vertices[2].UV = FVector2D(0, 0);
	Vertices[3].UV = FVector2D(1, 0);

	DrawPrimitiveUP(RHICmdList, PT_TriangleStrip, 2, Vertices, sizeof(Vertices[0]));

	PixelShader->UnbindBuffers(RHICmdList);

	bIsPixelShaderExecuting = false;
	bMustRegenerateSRV = true;
}