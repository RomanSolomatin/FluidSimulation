#pragma once

#include "../Private/FluidSimulationShader.h"

class FLUIDSIMULATIONPLUGIN_API FFluidSimulationShaderInstance
{
public:
	FFluidSimulationShaderInstance(int32 SizeX, int32 SizeY, int32 SizeZ, ERHIFeatureLevel::Type ShaderFeatureLevel);
	~FFluidSimulationShaderInstance();

	struct FluidSimParameters {
		float Dissipation;
		float Decay;
		float TimeStepModifier;
		float TimeStep;
		float VorticityStrength;
		FVector Point;
		float Radius;
		FVector VelocityAmount;
		FVector DensityAmount;
		uint8 JacobiIterations;
	};

	void UpdateParameters(FluidSimParameters Parameters);

	void ExecuteShader();
	void ExecuteShaderInternal();
	void ExecuteObstaclesShader();
	void ExecuteObstaclesShaderInternal();

	FTexture3DRHIRef GetDensityIn() { return DensityIn; }
	FTexture3DRHIRef GetDensityOut() { return DensityOut; }
	FTexture3DRHIRef GetPressureIn() { return PressureIn; }
	FTexture3DRHIRef GetPressureOut() { return PressureOut; }
	FTexture3DRHIRef GetVelocityIn() { return VelocityIn; }
	FTexture3DRHIRef GetVelocityOut() { return VelocityOut; }
	FTexture3DRHIRef GetVorticityOut() { return VorticityOut; }
	FTexture3DRHIRef GetDivergence() { return Divergence; }

private:
	bool bIsShaderExecuting;
	bool bIsUnloading;

	uint8 JacobiIterations;

	FVector VelocityAmount;
	FVector DensityAmount;

	FFluidSimulationShaderConstantParameters ConstantParameters;
	FFluidSimulationShaderVariableParameters VariableParameters;
	ERHIFeatureLevel::Type FeatureLevel;

	FTexture3DRHIRef PressureIn;
	FTexture3DRHIRef PressureOut;
	FTexture3DRHIRef Divergence;
	FTexture3DRHIRef VelocityIn;
	FTexture3DRHIRef VelocityOut;
	FTexture3DRHIRef DensityIn;
	FTexture3DRHIRef DensityOut;
	FTexture3DRHIRef TemperatureIn;
	FTexture3DRHIRef TemperatureOut;
	FTexture3DRHIRef Obstacle;
	FTexture3DRHIRef VorticityIn;
	FTexture3DRHIRef VorticityOut;

	FUnorderedAccessViewRHIRef PressureInUAV;
	FUnorderedAccessViewRHIRef PressureOutUAV;
	FUnorderedAccessViewRHIRef DivergenceUAV;
	FUnorderedAccessViewRHIRef VelocityInUAV;
	FUnorderedAccessViewRHIRef VelocityOutUAV;
	FUnorderedAccessViewRHIRef DensityInUAV;
	FUnorderedAccessViewRHIRef DensityOutUAV;
	FUnorderedAccessViewRHIRef TemperatureInUAV;
	FUnorderedAccessViewRHIRef TemperatureOutUAV;
	FUnorderedAccessViewRHIRef ObstacleUAV;
	FUnorderedAccessViewRHIRef VorticityInUAV;
	FUnorderedAccessViewRHIRef VorticityOutUAV;

	FShaderResourceViewRHIRef PressureInSRV;
	FShaderResourceViewRHIRef PressureOutSRV;
	FShaderResourceViewRHIRef DivergenceSRV;
	FShaderResourceViewRHIRef VelocityInSRV;
	FShaderResourceViewRHIRef VelocityOutSRV;
	FShaderResourceViewRHIRef DensityInSRV;
	FShaderResourceViewRHIRef DensityOutSRV;
	FShaderResourceViewRHIRef TemperatureInSRV;
	FShaderResourceViewRHIRef TemperatureOutSRV;
	FShaderResourceViewRHIRef ObstacleSRV;
	FShaderResourceViewRHIRef VorticityInSRV;
	FShaderResourceViewRHIRef VorticityOutSRV;
};