#include "FluidSimulationPluginPrivatePCH.h"
#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"

FFluidSimAdvectShader::FFluidSimAdvectShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
: FGlobalShader(Initializer)
{
	ObstaclesIn.Bind(Initializer.ParameterMap, TEXT("ObstaclesIn"));
	VelocityIn.Bind(Initializer.ParameterMap, TEXT("VelocityIn"));
	AdvectionIn.Bind(Initializer.ParameterMap, TEXT("AdvectionIn"));
	AdvectionOut.Bind(Initializer.ParameterMap, TEXT("AdvectionOut"));
	AdvectionSamplerBinding.Bind(Initializer.ParameterMap, TEXT("AdvectionSampler"));
}

void FFluidSimAdvectShader::SetSurfaces(
	FRHICommandList& RHICmdList,
	FShaderResourceViewRHIRef ObstaclesInSRV,
	FShaderResourceViewRHIRef VelocityInSRV,
	FShaderResourceViewRHIRef AdvectionInSRV,
	FUnorderedAccessViewRHIRef AdvectionOutUAV
	)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), ObstaclesInSRV);
	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), VelocityInSRV);
	if (AdvectionIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, AdvectionIn.GetBaseIndex(), AdvectionInSRV);
	if (AdvectionOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, AdvectionOut.GetBaseIndex(), AdvectionOutUAV);

	if (AdvectionSamplerBinding.IsBound())
		RHICmdList.SetShaderSampler(ComputeShaderRHI, AdvectionSamplerBinding.GetBaseIndex(), TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI());
}

void FFluidSimAdvectShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationShaderConstantParameters& ConstantParameters, FFluidSimulationShaderVariableParameters& VariableParameters)
{
	FFluidSimulationShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderConstantParameters>(), ConstantParametersBuffer);
	SetUniformBufferParameter(RHICmdList, GetComputeShader(), GetUniformBufferParameter<FFluidSimulationShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimAdvectShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FComputeShaderRHIParamRef ComputeShaderRHI = GetComputeShader();

	if (ObstaclesIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, ObstaclesIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (VelocityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, VelocityIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (AdvectionIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(ComputeShaderRHI, AdvectionIn.GetBaseIndex(), FShaderResourceViewRHIRef());
	if (AdvectionOut.IsBound())
		RHICmdList.SetUAVParameter(ComputeShaderRHI, AdvectionOut.GetBaseIndex(), FUnorderedAccessViewRHIRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimAdvectShader, TEXT("FluidSimAdvect"), TEXT("AdvectComputeShader"), SF_Compute);