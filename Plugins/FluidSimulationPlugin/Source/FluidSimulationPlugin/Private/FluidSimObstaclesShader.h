#pragma once

#include "GlobalShader.h"
#include "FluidSimulationShader.h"

class FFluidSimObstaclesShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimObstaclesShader, Global);

public:
	FFluidSimObstaclesShader() {}
	explicit FFluidSimObstaclesShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);
	
	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ObstaclesOut;
		return bShaderHasOutdatedParameters;
	}

	void SetSurfaces(
		FRHICommandList& RHICmdList,
		FUnorderedAccessViewRHIRef ObstaclesOutUAV
		);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter ObstaclesOut;
};