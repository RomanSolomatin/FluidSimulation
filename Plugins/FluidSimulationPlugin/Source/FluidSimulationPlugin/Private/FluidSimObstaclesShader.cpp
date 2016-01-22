#include "FluidSimulationPluginPrivatePCH.h"

FFluidSimObstaclesShader::FFluidSimObstaclesShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
: FGlobalShader(Initializer)
{
	ObstaclesOut.Bind(Initializer.ParameterMap, TEXT("ObstaclesOut"));
}

void FFluidSimObstaclesShader::SetSurfaces(
	FRHICommandList& RHICmdList,
	FUnorderedAccessViewRHIRef ObstaclesOutUAV
	)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, ObstaclesOut.GetBaseIndex(), ObstaclesOutUAV);
}

void FFluidSimObstaclesShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters)
{
	FFluidSimulationShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderConstantParameters>(), ConstantParametersBuffer);
	//SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimObstaclesShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, ObstaclesOut.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimObstaclesShader, TEXT("FluidSimObstacles"), TEXT("ObstaclesComputeShader"), SF_Compute);