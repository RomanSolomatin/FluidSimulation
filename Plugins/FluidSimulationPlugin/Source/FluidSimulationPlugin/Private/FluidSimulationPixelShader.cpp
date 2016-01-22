#include "FluidSimulationPluginPrivatePCH.h"
#include "ShaderParameterUtils.h"
#include "RHIStaticStates.h"

IMPLEMENT_UNIFORM_BUFFER_STRUCT(FFluidSimulationPixelShaderConstantParameters, TEXT("PSConstantParams"))
IMPLEMENT_UNIFORM_BUFFER_STRUCT(FFluidSimulationPixelShaderVariableParameters, TEXT("PSVariableParams"))

FFluidSimulationPixelShader::FFluidSimulationPixelShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
: FGlobalShader(Initializer)
{
	DensityIn.Bind(Initializer.ParameterMap, TEXT("DensityIn"));
	PixelSamplerBinding.Bind(Initializer.ParameterMap, TEXT("PixelSampler"));
}

void FFluidSimulationPixelShader::SetUniformBuffers(FRHICommandList& RHICmdList, FFluidSimulationPixelShaderConstantParameters& ConstantParameters, FFluidSimulationPixelShaderVariableParameters& VariableParameters)
{
	FFluidSimulationPixelShaderConstantParametersRef ConstantParametersBuffer;
	FFluidSimulationPixelShaderVariableParametersRef VariableParametersBuffer;

	ConstantParametersBuffer = FFluidSimulationPixelShaderConstantParametersRef::CreateUniformBufferImmediate(ConstantParameters, UniformBuffer_SingleDraw);
	VariableParametersBuffer = FFluidSimulationPixelShaderVariableParametersRef::CreateUniformBufferImmediate(VariableParameters, UniformBuffer_SingleDraw);

	SetUniformBufferParameter(RHICmdList, GetPixelShader(), GetUniformBufferParameter<FFluidSimulationPixelShaderConstantParameters>(), ConstantParametersBuffer);
	SetUniformBufferParameter(RHICmdList, GetPixelShader(), GetUniformBufferParameter<FFluidSimulationPixelShaderVariableParameters>(), VariableParametersBuffer);
}

void FFluidSimulationPixelShader::SetSurfaces(FRHICommandList& RHICmdList, FShaderResourceViewRHIRef DensityInSRV)
{
	FPixelShaderRHIParamRef PixelShaderRHI = GetPixelShader();

	if (DensityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(PixelShaderRHI, DensityIn.GetBaseIndex(), DensityInSRV);

	if (PixelSamplerBinding.IsBound())
		RHICmdList.SetShaderSampler(PixelShaderRHI, PixelSamplerBinding.GetBaseIndex(), TStaticSamplerState<SF_Bilinear, AM_Clamp, AM_Clamp, AM_Clamp>::GetRHI());
}

void FFluidSimulationPixelShader::UnbindBuffers(FRHICommandList& RHICmdList)
{
	FPixelShaderRHIParamRef PixelShaderRHI = GetPixelShader();

	if (DensityIn.IsBound())
		RHICmdList.SetShaderResourceViewParameter(PixelShaderRHI, DensityIn.GetBaseIndex(), FShaderResourceViewRHIParamRef());
}

IMPLEMENT_SHADER_TYPE(, FFluidSimulationVertexShader, TEXT("FluidSimPixelShader"), TEXT("MainVertexShader"), SF_Vertex);
IMPLEMENT_SHADER_TYPE(, FFluidSimulationPixelShader,  TEXT("FluidSimPixelShader"), TEXT("MainPixelShader"), SF_Pixel);