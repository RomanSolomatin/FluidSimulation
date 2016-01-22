#pragma once

#include "GlobalShader.h"
#include "FluidSimulationShader.h"

class FFluidSimConfinementShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimConfinementShader, Global);

public:
	FFluidSimConfinementShader() {}
	explicit FFluidSimConfinementShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);
	
	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ObstaclesIn;
		Ar << VorticityIn;
		Ar << VelocityIn;
		Ar << VelocityOut;
		return bShaderHasOutdatedParameters;
	}

	void SetSurfaces(
		FRHICommandList& RHICmdList,
		FShaderResourceViewRHIRef ObstaclesInSRV,
		FShaderResourceViewRHIRef VorticityInSRV,
		FShaderResourceViewRHIRef VelocityInSRV,
		FUnorderedAccessViewRHIRef VelocityOutUAV
		);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter ObstaclesIn;
	FShaderResourceParameter VorticityIn;
	FShaderResourceParameter VelocityIn;
	FShaderResourceParameter VelocityOut;
};