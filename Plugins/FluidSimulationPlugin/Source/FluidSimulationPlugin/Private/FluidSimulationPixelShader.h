#pragma once

#include "GlobalShader.h"

BEGIN_UNIFORM_BUFFER_STRUCT(FFluidSimulationPixelShaderConstantParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, Direction)
END_UNIFORM_BUFFER_STRUCT(FFluidSimulationPixelShaderConstantParameters)

BEGIN_UNIFORM_BUFFER_STRUCT(FFluidSimulationPixelShaderVariableParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, Placeholder)
END_UNIFORM_BUFFER_STRUCT(FFluidSimulationPixelShaderVariableParameters)

typedef TUniformBufferRef<FFluidSimulationPixelShaderConstantParameters> FFluidSimulationPixelShaderConstantParametersRef;
typedef TUniformBufferRef<FFluidSimulationPixelShaderVariableParameters> FFluidSimulationPixelShaderVariableParametersRef;

struct FTextureVertex
{
	FVector4 Position;
	FVector2D UV;
};

class FTextureVertexDeclaration : public FRenderResource
{
public:
	FVertexDeclarationRHIRef VertexDeclarationRHI;

	virtual void InitRHI() override
	{
		FVertexDeclarationElementList Elements;
		uint32 Stride = sizeof(FTextureVertex);
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FTextureVertex, Position), VET_Float4, 0, Stride));
		Elements.Add(FVertexElement(0, STRUCT_OFFSET(FTextureVertex, UV), VET_Float2, 1, Stride));
		VertexDeclarationRHI = RHICreateVertexDeclaration(Elements);
	}

	virtual void ReleaseRHI() override
	{
		VertexDeclarationRHI.SafeRelease();
	}
};

class FFluidSimulationVertexShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimulationVertexShader, Global);

public:
	static bool ShouldCache(EShaderPlatform Platform) { return true; }

	FFluidSimulationVertexShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer) :
		FGlobalShader(Initializer)
	{}
	FFluidSimulationVertexShader() {}
};

class FFluidSimulationPixelShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimulationPixelShader, Global);

public:

	FFluidSimulationPixelShader() {}

	explicit FFluidSimulationPixelShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);

		Ar << DensityIn;
		Ar << PixelSamplerBinding;

		return bShaderHasOutdatedParams;
	}

	void SetSurfaces(FRHICommandList& RHICmdList, FShaderResourceViewRHIRef DensityInSRV);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationPixelShaderConstantParameters& ConstantParameters, FFluidSimulationPixelShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter DensityIn;
	FShaderResourceParameter PixelSamplerBinding;
};