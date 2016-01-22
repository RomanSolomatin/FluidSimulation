#pragma once

#include "GlobalShader.h"
#include "FluidSimulationShader.h"

class FFluidSimJacobiShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimJacobiShader, Global);

public:
	FFluidSimJacobiShader() {}
	explicit FFluidSimJacobiShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ObstaclesIn;
		Ar << DivergenceIn;
		Ar << PressureIn;
		Ar << PressureOut;
		return bShaderHasOutdatedParameters;
	}

	void SetSurfaces(
		FRHICommandList& RHICmdList,
		FShaderResourceViewRHIRef ObstaclesInSRV,
		FShaderResourceViewRHIRef DivergenceInSRV,
		FShaderResourceViewRHIRef PressureInSRV,
		FUnorderedAccessViewRHIRef PressureOutUAV
		);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter ObstaclesIn;
	FShaderResourceParameter DivergenceIn;
	FShaderResourceParameter PressureIn;
	FShaderResourceParameter PressureOut;
};