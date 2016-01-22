#include "FluidSimulationPluginPrivatePCH.h"

FFluidSimDivergenceShader::FFluidSimDivergenceShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
: FGlobalShader(Initializer)
{
	ObstaclesIn.Bind(Initializer.ParameterMap, TEXT("ObstaclesIn"));
	VelocityIn.Bind(Initializer.ParameterMap, TEXT("VelocityIn"));
	DivergenceOut.Bind(Initializer.ParameterMap, TEXT("DivergenceOut"));
}

void FFluidSimDivergenceShader::SetSurfaces(
	FRHICommandList& RHICmdList,
	FShaderResourceViewRHIRef ObstaclesInSRV,
	FShaderResourceViewRHIRef VelocityInSRV,
	FUnorderedAccessViewRHIRef DivergenceOutUAV
	)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), ObstaclesInSRV);
	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), VelocityInSRV);
	if (DivergenceOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, DivergenceOut.GetBaseIndex(), DivergenceOutUAV);
}

void FFluidSimDivergenceShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters)
{
	FFluidSimulationShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderConstantParameters>(), ConstantParametersBuffer);
	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimDivergenceShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (DivergenceOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, DivergenceOut.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimDivergenceShader, TEXT("FluidSimDivergence"), TEXT("DivergenceComputeShader"), SF_Compute);