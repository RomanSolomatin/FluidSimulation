#include "FluidSimulationPluginPrivatePCH.h"
#include "ShaderParameterUtils.h"

FFluidSimImpulseShader::FFluidSimImpulseShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer)
{
	ImpulseIn.Bind(Initializer.ParameterMap, TEXT("ImpulseIn"));
	ImpulseOut.Bind(Initializer.ParameterMap, TEXT("ImpulseOut"));
}

void FFluidSimImpulseShader::SetSurfaces(
	FRHICommandList& RHICmdList,
	FShaderResourceViewRHIRef ImpulseInSRV,
	FUnorderedAccessViewRHIRef ImpulseOutUAV
	)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ImpulseIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ImpulseIn.GetBaseIndex(), ImpulseInSRV);
	if (ImpulseOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, ImpulseOut.GetBaseIndex(), ImpulseOutUAV);
}

void FFluidSimImpulseShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters)
{
	FFluidSimulationShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderConstantParameters>(), ConstantParametersBuffer);
	SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimImpulseShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ImpulseIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ImpulseIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (ImpulseOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, ImpulseOut.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimImpulseShader, TEXT("FluidSimImpulse"), TEXT("ImpulseComputeShader"), SF_Compute);