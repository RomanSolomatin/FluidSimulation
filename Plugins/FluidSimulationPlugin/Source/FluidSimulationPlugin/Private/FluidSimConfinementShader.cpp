#include "FluidSimulationPluginPrivatePCH.h"
#include "ShaderParameterUtils.h"

FFluidSimConfinementShader::FFluidSimConfinementShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
: FGlobalShader(Initializer)
{
	ObstaclesIn.Bind(Initializer.ParameterMap, TEXT("ObstaclesIn"));
	VorticityIn.Bind(Initializer.ParameterMap, TEXT("VorticityIn"));
	VelocityIn.Bind(Initializer.ParameterMap, TEXT("VelocityIn"));
	VelocityOut.Bind(Initializer.ParameterMap, TEXT("VelocityOut"));
}

void FFluidSimConfinementShader::SetSurfaces(
	FRHICommandList& RHICmdList,
	FShaderResourceViewRHIRef ObstaclesInSRV,
	FShaderResourceViewRHIRef VorticityInSRV,
	FShaderResourceViewRHIRef VelocityInSRV,
	FUnorderedAccessViewRHIRef VelocityOutUAV
	)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), ObstaclesInSRV);
	if (VorticityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VorticityIn.GetBaseIndex(), VorticityInSRV);
	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), VelocityInSRV);
	if (VelocityIn.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, VelocityOut.GetBaseIndex(), VelocityOutUAV);
}

void FFluidSimConfinementShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters)
{
	FFluidSimulationShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderConstantParameters>(), ConstantParametersBuffer);
	SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimConfinementShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (VorticityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VorticityIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (VelocityOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, VelocityOut.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimConfinementShader, TEXT("FluidSimConfinement"), TEXT("ConfinementComputeShader"), SF_Compute);