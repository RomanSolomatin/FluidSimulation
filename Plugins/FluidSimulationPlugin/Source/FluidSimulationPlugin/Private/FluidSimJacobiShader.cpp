#include "FluidSimulationPluginPrivatePCH.h"

FFluidSimJacobiShader::FFluidSimJacobiShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer)
{
	ObstaclesIn.Bind(Initializer.ParameterMap, TEXT("ObstaclesIn"));
	DivergenceIn.Bind(Initializer.ParameterMap, TEXT("DivergenceIn"));
	PressureIn.Bind(Initializer.ParameterMap, TEXT("PressureIn"));
	PressureOut.Bind(Initializer.ParameterMap, TEXT("PressureOut"));
}

void FFluidSimJacobiShader::SetSurfaces(
	FRHICommandList& RHICmdList,
	FShaderResourceViewRHIRef ObstaclesInSRV,
	FShaderResourceViewRHIRef DivergenceInSRV,
	FShaderResourceViewRHIRef PressureInSRV,
	FUnorderedAccessViewRHIRef PressureOutUAV
	)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), ObstaclesInSRV);
	if (DivergenceIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, DivergenceIn.GetBaseIndex(), DivergenceInSRV);
	if (PressureIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, PressureIn.GetBaseIndex(), PressureInSRV);
	if (PressureOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, PressureOut.GetBaseIndex(), PressureOutUAV);
}

void FFluidSimJacobiShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters)
{
	FFluidSimulationShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderConstantParameters>(), ConstantParametersBuffer);
	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimJacobiShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (DivergenceIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, DivergenceIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (PressureIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, PressureIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (PressureOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, PressureOut.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimJacobiShader, TEXT("FluidSimJacobi"), TEXT("JacobiComputeShader"), SF_Compute);