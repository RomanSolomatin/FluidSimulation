#pragma once

#include "../Private/FluidSimulationPixelShader.h"

class FLUIDSIMULATIONPLUGIN_API FFluidSimulationPixelShaderInstance
{
public:
	FFluidSimulationPixelShaderInstance(ERHIFeatureLevel::Type ShaderFeatureLevel);
	~FFluidSimulationPixelShaderInstance();

	void ExecutePixelShader(UTextureRenderTarget2D* RenderTarget, FTexture3DRHIRef InputTexture);
	void ExecutePixelShaderInternal();

private:
	bool bIsPixelShaderExecuting;
	bool bMustRegenerateSRV;
	bool bIsUnloading;

	FFluidSimulationPixelShaderConstantParameters ConstantParameters;
	FFluidSimulationPixelShaderVariableParameters VariableParameters;
	ERHIFeatureLevel::Type FeatureLevel;

	FTexture2DRHIRef CurrentTexture;
	FTexture3DRHIRef DensityIn;
	UTextureRenderTarget2D* CurrentRenderTarget;

	FShaderResourceViewRHIRef DensityInSRV;
};