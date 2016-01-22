#pragma once

#include "GlobalShader.h"
#include "FluidSimulationShader.h"

class FFluidSimImpulseShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimImpulseShader, Global);

public:
	FFluidSimImpulseShader() {}
	explicit FFluidSimImpulseShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);
	
	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ImpulseIn;
		Ar << ImpulseOut;
		return bShaderHasOutdatedParameters;
	}

	void SetSurfaces(
		FRHICommandList& RHICmdList,
		FShaderResourceViewRHIRef ImpulseInSRV,
		FUnorderedAccessViewRHIRef ImpulseOutUAV
		);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter ImpulseIn;
	FShaderResourceParameter ImpulseOut;
};