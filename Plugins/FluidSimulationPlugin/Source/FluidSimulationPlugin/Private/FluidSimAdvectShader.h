#pragma once

#include "GlobalShader.h"
#include "FluidSimulationShader.h"

class FFluidSimAdvectShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FFluidSimAdvectShader, Global);

public:
	FFluidSimAdvectShader() {}
	explicit FFluidSimAdvectShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);
	
	static bool ShouldCache(EShaderPlatform Platform) { return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); }

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParameters = FGlobalShader::Serialize(Ar);
		Ar << ObstaclesIn;
		Ar << VelocityIn;
		Ar << AdvectionIn;
		Ar << AdvectionOut;
		Ar << AdvectionSamplerBinding;
		return bShaderHasOutdatedParameters;
	}

	void SetSurfaces(
		FRHICommandList& RHICmdList,
		FShaderResourceViewRHIRef ObstaclesInSRV,
		FShaderResourceViewRHIRef VelocityInSRV,
		FShaderResourceViewRHIRef AdvectionInSRV,
		FUnorderedAccessViewRHIRef AdvectionOutUAV
		);
	void SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters);
	void UnbindBuffers(FRHICommandList& RHICmdList);

private:
	FShaderResourceParameter ObstaclesIn;
	FShaderResourceParameter VelocityIn;
	FShaderResourceParameter AdvectionIn;
	FShaderResourceParameter AdvectionOut;
	FShaderResourceParameter AdvectionSamplerBinding;
};