// Fill out your copyright notice in the Description page of Project Settings.

#include "FluidSimulation.h"
#include "FluidSimulationComponent.h"

// Sets default values for this component's properties
UFluidSimulationComponent::UFluidSimulationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UFluidSimulationComponent::BeginPlay()
{
	Super::BeginPlay();

	ComputeShading = new FFluidSimulationShaderInstance(64, 64, 64, GetWorld()->Scene->GetFeatureLevel());
	PixelShading1 = new FFluidSimulationPixelShaderInstance(GetWorld()->Scene->GetFeatureLevel());
	PixelShading2 = new FFluidSimulationPixelShaderInstance(GetWorld()->Scene->GetFeatureLevel());
	PixelShading3 = new FFluidSimulationPixelShaderInstance(GetWorld()->Scene->GetFeatureLevel());
	PixelShading4 = new FFluidSimulationPixelShaderInstance(GetWorld()->Scene->GetFeatureLevel());
	PixelShading5 = new FFluidSimulationPixelShaderInstance(GetWorld()->Scene->GetFeatureLevel());
	PixelShading6 = new FFluidSimulationPixelShaderInstance(GetWorld()->Scene->GetFeatureLevel());
	PixelShading7 = new FFluidSimulationPixelShaderInstance(GetWorld()->Scene->GetFeatureLevel());
	PixelShading8 = new FFluidSimulationPixelShaderInstance(GetWorld()->Scene->GetFeatureLevel());
}

void UFluidSimulationComponent::BeginDestroy()
{
	Super::BeginDestroy();

	if (ComputeShading != NULL)
	{
		delete ComputeShading;
	}

	if (PixelShading1 != NULL)
	{
		delete PixelShading1;
	}
}

// Called every frame
void UFluidSimulationComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (ComputeShading != NULL && PixelShading1 != NULL)
	{
		ComputeShading->ExecuteShader();
		FTexture3DRHIRef InputTexture = ComputeShading->GetDensityIn();
		PixelShading1->ExecutePixelShader(DensityIn, InputTexture);

		InputTexture = ComputeShading->GetDensityOut();
		PixelShading2->ExecutePixelShader(DensityOut, InputTexture);

		InputTexture = ComputeShading->GetVelocityIn();
		PixelShading3->ExecutePixelShader(VelocityIn, InputTexture);

		InputTexture = ComputeShading->GetVelocityOut();
		PixelShading4->ExecutePixelShader(VelocityOut, InputTexture);

		InputTexture = ComputeShading->GetPressureIn();
		PixelShading5->ExecutePixelShader(PressureIn, InputTexture);

		InputTexture = ComputeShading->GetPressureOut();
		PixelShading6->ExecutePixelShader(PressureOut, InputTexture);

		InputTexture = ComputeShading->GetDivergence();
		PixelShading7->ExecutePixelShader(Divergence, InputTexture);

		InputTexture = ComputeShading->GetVorticityOut();
		PixelShading8->ExecutePixelShader(VorticityOut, InputTexture);
	}
}

