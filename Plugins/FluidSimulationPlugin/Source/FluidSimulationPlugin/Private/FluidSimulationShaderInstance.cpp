#include "FluidSimulationPluginPrivatePCH.h"

#define JACOBI_ITERATIONS 50

FFluidSimulationShaderInstance::FFluidSimulationShaderInstance(int32 SizeX, int32 SizeY, int32 SizeZ, ERHIFeatureLevel::Type ShaderFeatureLevel)
{
	FeatureLevel = ShaderFeatureLevel;

	ConstantParameters = FFluidSimulationShaderConstantParameters();
	VariableParameters = FFluidSimulationShaderVariableParameters();

	bIsShaderExecuting = false;
	bIsUnloading = false;
	
	FRHIResourceCreateInfo CreateInfo;
	
	PressureIn = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_R32_UINT, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	PressureOut = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_R32_UINT, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	Divergence = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_R32_UINT, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	VelocityIn = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_A32B32G32R32F, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	VelocityOut = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_A32B32G32R32F, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	DensityIn = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_A32B32G32R32F, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	DensityOut = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_A32B32G32R32F, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	TemperatureIn = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_R32_UINT, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	TemperatureOut = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_R32_UINT, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	Obstacle = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_R32_UINT, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	VorticityIn = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_A32B32G32R32F, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);
	VorticityOut = RHICreateTexture3D(SizeX, SizeY, SizeZ, PF_A32B32G32R32F, 1, TexCreate_ShaderResource | TexCreate_UAV, CreateInfo);

	PressureInUAV = RHICreateUnorderedAccessView(PressureIn);
	PressureOutUAV = RHICreateUnorderedAccessView(PressureOut);
	DivergenceUAV = RHICreateUnorderedAccessView(Divergence);
	VelocityInUAV = RHICreateUnorderedAccessView(VelocityIn);
	VelocityOutUAV = RHICreateUnorderedAccessView(VelocityOut);
	DensityInUAV = RHICreateUnorderedAccessView(DensityIn);
	DensityOutUAV = RHICreateUnorderedAccessView(DensityOut);
	TemperatureInUAV = RHICreateUnorderedAccessView(TemperatureIn);
	TemperatureOutUAV = RHICreateUnorderedAccessView(TemperatureOut);
	ObstacleUAV = RHICreateUnorderedAccessView(Obstacle);
	VorticityInUAV = RHICreateUnorderedAccessView(VorticityIn);
	VorticityOutUAV = RHICreateUnorderedAccessView(VorticityOut);

	PressureInSRV = RHICreateShaderResourceView(PressureIn, 0);
	PressureOutSRV = RHICreateShaderResourceView(PressureOut, 0);
	DivergenceSRV = RHICreateShaderResourceView(Divergence, 0);
	VelocityInSRV = RHICreateShaderResourceView(VelocityIn, 0);
	VelocityOutSRV = RHICreateShaderResourceView(VelocityOut, 0);
	DensityInSRV = RHICreateShaderResourceView(DensityIn, 0);
	DensityOutSRV = RHICreateShaderResourceView(DensityOut, 0);
	TemperatureInSRV = RHICreateShaderResourceView(TemperatureIn, 0);
	TemperatureOutSRV = RHICreateShaderResourceView(TemperatureOut, 0);
	ObstacleSRV = RHICreateShaderResourceView(Obstacle, 0);
	VorticityInSRV = RHICreateShaderResourceView(VorticityIn, 0);
	VorticityOutSRV = RHICreateShaderResourceView(VorticityOut, 0);

	// Set up walls of simulation
	ExecuteObstaclesShader();
}

FFluidSimulationShaderInstance::~FFluidSimulationShaderInstance()
{
	bIsUnloading = true;
}

void FFluidSimulationShaderInstance::ExecuteShader()
{
	if (bIsUnloading || bIsShaderExecuting)
	{
		return;
	}

	bIsShaderExecuting = true;

	// Change variable parameters here

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		FComputeShaderRunner,
		FFluidSimulationShaderInstance*, ComputeShader, this,
		{
		ComputeShader->ExecuteShaderInternal();
		}
	);
}

void FFluidSimulationShaderInstance::ExecuteObstaclesShader()
{
	if (bIsUnloading || bIsShaderExecuting)
	{
		return;
	}

	bIsShaderExecuting = true;

	// Change variable parameters here

	ENQUEUE_UNIQUE_RENDER_COMMAND_ONEPARAMETER(
		FComputeShaderRunner,
		FFluidSimulationShaderInstance*, ComputeShader, this,
		{
		ComputeShader->ExecuteObstaclesShaderInternal();
		}
	);
}

void FFluidSimulationShaderInstance::ExecuteShaderInternal()
{
	check(IsInRenderingThread());

	if (bIsUnloading)
	{
		// Should really release textures here
		return;
	}

	FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();

	// Advect temperature against velocity
	TShaderMapRef<FFluidSimAdvectShader> AdvectionShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(AdvectionShader->GetComputeShader());

	ConstantParameters.Dissipation = 0.995f;
	ConstantParameters.TimeStepModifier = 1.0f;
	ConstantParameters.Decay = 0.0f;
	ConstantParameters.VorticityStrength = 0.8f;

	AdvectionShader->SetSurfaces(RHICmdList, ObstacleSRV, VelocityInSRV, TemperatureInSRV, TemperatureOutUAV);
	AdvectionShader->SetUniformBuffers(RHICmdList, ConstantParameters, VariableParameters);
	DispatchComputeShader(RHICmdList, *AdvectionShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	AdvectionShader->UnbindBuffers(RHICmdList);
	TemperatureInUAV.Swap(TemperatureOutUAV);
	TemperatureInSRV.Swap(TemperatureOutSRV);

	// Advect density against velocity
	AdvectionShader->SetSurfaces(RHICmdList, ObstacleSRV, VelocityInSRV, DensityInSRV, DensityOutUAV);
	AdvectionShader->SetUniformBuffers(RHICmdList, ConstantParameters, VariableParameters);
	DispatchComputeShader(RHICmdList, *AdvectionShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	AdvectionShader->UnbindBuffers(RHICmdList);
	DensityInUAV.Swap(DensityOutUAV);
	DensityInSRV.Swap(DensityOutSRV);

	// Advect velocity against velocity
	AdvectionShader->SetSurfaces(RHICmdList, ObstacleSRV, VelocityInSRV, VelocityInSRV, VelocityOutUAV);
	AdvectionShader->SetUniformBuffers(RHICmdList, ConstantParameters, VariableParameters);
	DispatchComputeShader(RHICmdList, *AdvectionShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	AdvectionShader->UnbindBuffers(RHICmdList);
	VelocityInUAV.Swap(VelocityOutUAV);
	VelocityInSRV.Swap(VelocityOutSRV);

	// Add constant density into the system
	TShaderMapRef<FFluidSimImpulseShader> ImpulseShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(ImpulseShader->GetComputeShader());

	ConstantParameters.Point = FVector(32.0f, 0.0f, 32.0f);
	ConstantParameters.Amount = FVector(0.0f, 1.0f, 0.0f);
	ConstantParameters.Radius = 20.0f;
	ConstantParameters.TimeStep = 0.1f;

	ImpulseShader->SetSurfaces(RHICmdList, DensityInSRV, DensityOutUAV);
	ImpulseShader->SetUniformBuffers(RHICmdList, ConstantParameters, VariableParameters);
	DispatchComputeShader(RHICmdList, *ImpulseShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	ImpulseShader->UnbindBuffers(RHICmdList);
	DensityInUAV.Swap(DensityOutUAV);
	DensityInSRV.Swap(DensityOutSRV);

	ConstantParameters.Amount = FVector(0.0f, 1.0f, 0.0f);

	// Apply external forces to the system
	ImpulseShader->SetSurfaces(RHICmdList, VelocityInSRV, VelocityOutUAV);
	ImpulseShader->SetUniformBuffers(RHICmdList, ConstantParameters, VariableParameters);
	DispatchComputeShader(RHICmdList, *ImpulseShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	ImpulseShader->UnbindBuffers(RHICmdList);
	VelocityInUAV.Swap(VelocityOutUAV);
	VelocityInSRV.Swap(VelocityOutSRV);

	// Add vorticity to keep swirling motion of fluid
	TShaderMapRef<FFluidSimVorticityShader> VorticityShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(VorticityShader->GetComputeShader());

	VorticityShader->SetSurfaces(RHICmdList, VelocityInSRV, VorticityOutUAV);
	DispatchComputeShader(RHICmdList, *VorticityShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	VorticityShader->UnbindBuffers(RHICmdList);

	TShaderMapRef<FFluidSimConfinementShader> ConfinementShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(ConfinementShader->GetComputeShader());

	ConfinementShader->SetSurfaces(RHICmdList, ObstacleSRV, VorticityOutSRV, VelocityInSRV, VelocityOutUAV);
	ConfinementShader->SetUniformBuffers(RHICmdList, ConstantParameters, VariableParameters);
	DispatchComputeShader(RHICmdList, *ConfinementShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	ConfinementShader->UnbindBuffers(RHICmdList);
	VelocityInUAV.Swap(VelocityOutUAV);
	VelocityInSRV.Swap(VelocityOutSRV);

	// Calculate divergence
	TShaderMapRef<FFluidSimDivergenceShader> DivergenceShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(DivergenceShader->GetComputeShader());

	DivergenceShader->SetSurfaces(RHICmdList, ObstacleSRV, VelocityInSRV, DivergenceUAV);
	DivergenceShader->SetUniformBuffers(RHICmdList, ConstantParameters, VariableParameters);
	DispatchComputeShader(RHICmdList, *DivergenceShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	DivergenceShader->UnbindBuffers(RHICmdList);

	// Calculate pressure gradient using iterations of Jacobi method
	TShaderMapRef<FFluidSimJacobiShader> JacobiShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(JacobiShader->GetComputeShader());
	
	for (uint8 i = 0; i < JACOBI_ITERATIONS; ++i) {
		JacobiShader->SetSurfaces(RHICmdList, ObstacleSRV, DivergenceSRV, PressureInSRV, PressureOutUAV);
		DispatchComputeShader(RHICmdList, *JacobiShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
		JacobiShader->UnbindBuffers(RHICmdList);
		PressureInUAV.Swap(PressureOutUAV);
		PressureInSRV.Swap(PressureOutSRV);
	}

	// Subtract gradient
	TShaderMapRef<FFluidSimSubGradientShader> SubGradientShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(SubGradientShader->GetComputeShader());

	SubGradientShader->SetSurfaces(RHICmdList, ObstacleSRV, PressureInSRV, VelocityInSRV, VelocityOutUAV);
	DispatchComputeShader(RHICmdList, *SubGradientShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	SubGradientShader->UnbindBuffers(RHICmdList);
	VelocityInUAV.Swap(VelocityOutUAV);
	VelocityInSRV.Swap(VelocityOutSRV);
	
	bIsShaderExecuting = false;
}

void FFluidSimulationShaderInstance::ExecuteObstaclesShaderInternal()
{
	check(IsInRenderingThread());

	if (bIsUnloading)
	{
		if (ObstacleUAV != NULL)
		{
			ObstacleUAV.SafeRelease();
			ObstacleUAV = NULL;
		}

		return;
	}

	FRHICommandListImmediate& RHICmdList = GRHICommandList.GetImmediateCommandList();

	TShaderMapRef<FFluidSimObstaclesShader> ObstaclesShader(GetGlobalShaderMap(FeatureLevel));
	RHICmdList.SetComputeShader(ObstaclesShader->GetComputeShader());

	ObstaclesShader->SetSurfaces(RHICmdList, ObstacleUAV);
	DispatchComputeShader(RHICmdList, *ObstaclesShader, VelocityIn->GetSizeX() / 8, VelocityIn->GetSizeY() / 8, VelocityIn->GetSizeZ() / 8);
	ObstaclesShader->UnbindBuffers(RHICmdList);

	bIsShaderExecuting = false;
}