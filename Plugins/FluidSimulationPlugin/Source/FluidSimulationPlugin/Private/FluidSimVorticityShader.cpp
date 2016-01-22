#include "FluidSimulationPluginPrivatePCH.h"

FFluidSimVorticityShader::FFluidSimVorticityShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer)
{
	VelocityIn.Bind(Initializer.ParameterMap, TEXT("VelocityIn"));
	VorticityOut.Bind(Initializer.ParameterMap, TEXT("VorticityOut"));
}

void FFluidSimVorticityShader::SetSurfaces(
	FRHICommandList& RHICmdList,
	FShaderResourceViewRHIRef VelocityInSRV,
	FUnorderedAccessViewRHIRef VorticityOutUAV
	)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), VelocityInSRV);
	if (VorticityOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, VorticityOut.GetBaseIndex(), VorticityOutUAV);
}

void FFluidSimVorticityShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters)
{
	FFluidSimulationShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderConstantParameters>(), ConstantParametersBuffer);
	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimVorticityShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (VorticityOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, VorticityOut.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimVorticityShader, TEXT("FluidSimVorticity"), TEXT("VorticityComputeShader"), SF_Compute);