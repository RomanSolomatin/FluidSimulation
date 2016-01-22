#pragma once

#include "GlobalShader.h"
#include "FluidSimulationShader.h"

class FFluidSimDivergenceShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimDivergenceShader, Global);

public:
	FFluidSimDivergenceShader() {}
	explicit FFluidSimDivergenceShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);
	
	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ObstaclesIn;
		Ar << VelocityIn;
		Ar << DivergenceOut;
		return bShaderHasOutdatedParameters;
	}

	void SetSurfaces(
		FRHICommandList& RHICmdList,
		FShaderResourceViewRHIRef ObstaclcsInSRV,
		FShaderResourceViewRHIRef VelocityInSRV,
		FUnorderedAccessViewRHIRef DivergenceOutUAV
		);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter ObstaclesIn;
	FShaderResourceParameter VelocityIn;
	FShaderResourceParameter DivergenceOut;
};