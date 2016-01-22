#pragma once

#include "GlobalShader.h"
#include "FluidSimulationShader.h"

class FFluidSimSubGradientShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimSubGradientShader, Global);

public:
	FFluidSimSubGradientShader() {}
	explicit FFluidSimSubGradientShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ObstaclesIn;
		Ar << PressureIn;
		Ar << VelocityIn;
		Ar << VelocityOut;
		return bShaderHasOutdatedParameters;
	}

	void SetSurfaces(
		FRHICommandList& RHICmdList,
		FShaderResourceViewRHIRef ObstaclesInSRV,
		FShaderResourceViewRHIRef PressureInSRV,
		FShaderResourceViewRHIRef VelocityInSRV,
		FUnorderedAccessViewRHIRef VelocityOutUAV
		);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter ObstaclesIn;
	FShaderResourceParameter PressureIn;
	FShaderResourceParameter VelocityIn;
	FShaderResourceParameter VelocityOut;
};