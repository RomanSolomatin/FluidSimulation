#pragma once

#include "GlobalShader.h"

BEGIN_UNIFORM_BUFFER_STRUCT(FFluidSimulationShaderConstantParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, TimeStep)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, TimeStepModifier)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, VorticityStrength)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, Dissipation)		// Found in AdvectComputeShader
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, Decay)				// Found in AdvectComputeShader
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, Point)			// Found in ImpulseComputeShader
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, Radius)				// Found in ImpulseComputeShader
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(FVector, Amount)			// Found in ImpulseComputeShader
END_UNIFORM_BUFFER_STRUCT(FFluidSimulationShaderConstantParameters)

BEGIN_UNIFORM_BUFFER_STRUCT(FFluidSimulationShaderVariableParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, TotalTimeElapsedSeconds)
END_UNIFORM_BUFFER_STRUCT(FFluidSimulationShaderVariableParameters)

typedef TUniformBufferRef<FFluidSimulationShaderConstantParameters> FFluidSimulationShaderConstantParametersRef; 
typedef TUniformBufferRef<FFluidSimulationShaderVariableParameters> FFluidSimulationShaderVariableParametersRef;