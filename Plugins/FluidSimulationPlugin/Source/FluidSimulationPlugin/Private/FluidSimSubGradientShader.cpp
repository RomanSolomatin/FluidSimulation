#include "FluidSimulationPluginPrivatePCH.h"

FFluidSimSubGradientShader::FFluidSimSubGradientShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer)
{
	ObstaclesIn.Bind(Initializer.ParameterMap, TEXT("ObstaclesIn"));
	PressureIn.Bind(Initializer.ParameterMap, TEXT("PressureIn"));
	VelocityIn.Bind(Initializer.ParameterMap, TEXT("VelocityIn"));
	VelocityOut.Bind(Initializer.ParameterMap, TEXT("VelocityOut"));
}

void FFluidSimSubGradientShader::SetSurfaces(
	FRHICommandList& RHICmdList,
	FShaderResourceViewRHIRef ObstaclesInSRV,
	FShaderResourceViewRHIRef PressureInSRV,
	FShaderResourceViewRHIRef VelocityInSRV,
	FUnorderedAccessViewRHIRef VelocityOutUAV
	)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), ObstaclesInSRV);
	if (PressureIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, PressureIn.GetBaseIndex(), PressureInSRV);
	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), VelocityInSRV);
	if (VelocityOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, VelocityOut.GetBaseIndex(), VelocityOutUAV);
}

void FFluidSimSubGradientShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters)
{
	FFluidSimulationShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderConstantParameters>(), ConstantParametersBuffer);
	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimSubGradientShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (PressureIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, PressureIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (VelocityOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, VelocityOut.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimSubGradientShader, TEXT("FluidSimSubGradient"), TEXT("SubtractGradientComputeShader"), SF_Compute);