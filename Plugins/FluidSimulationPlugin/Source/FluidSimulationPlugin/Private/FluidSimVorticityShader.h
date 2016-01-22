#pragma once

#include "GlobalShader.h"
#include "FluidSimulationShader.h"

class FFluidSimVorticityShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimVorticityShader, Global);

public:
	FFluidSimVorticityShader() {}
	explicit FFluidSimVorticityShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << VelocityIn;
		Ar << VorticityOut;
		return bShaderHasOutdatedParameters;
	}

	void SetSurfaces(
		FRHICommandList& RHICmdList,
		FShaderResourceViewRHIRef VelocityInSRV,
		FUnorderedAccessViewRHIRef VorticityOutUAV
		);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter VelocityIn;
	FShaderResourceParameter VorticityOut;
};