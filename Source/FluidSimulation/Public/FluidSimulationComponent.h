// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "FluidSimulationShaderInstance.h"
#include "FluidSimulationPixelShaderInstance.h"
#include "FluidSimulationComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FLUIDSIMULATION_API UFluidSimulationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFluidSimulationComponent();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called when the game exits
	virtual void BeginDestroy() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		float Dissipation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		float Decay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		float TimeStepModifier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		float TimeStep;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		float VorticityStrength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		FVector Point;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		FVector VelocityAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		FVector DensityAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		uint8 JacobiIterations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UMaterialInterface* FluidSimulationMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UTextureRenderTarget2D* VelocityIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UTextureRenderTarget2D* VelocityOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UTextureRenderTarget2D* DensityIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UTextureRenderTarget2D* DensityOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UTextureRenderTarget2D* PressureIn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UTextureRenderTarget2D* PressureOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UTextureRenderTarget2D* Divergence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShaderDemo)
		UTextureRenderTarget2D* VorticityOut;

private:
	FFluidSimulationPixelShaderInstance* PixelShading1;
	FFluidSimulationPixelShaderInstance* PixelShading2;
	FFluidSimulationPixelShaderInstance* PixelShading3;
	FFluidSimulationPixelShaderInstance* PixelShading4;
	FFluidSimulationPixelShaderInstance* PixelShading5;
	FFluidSimulationPixelShaderInstance* PixelShading6;
	FFluidSimulationPixelShaderInstance* PixelShading7;
	FFluidSimulationPixelShaderInstance* PixelShading8;
	FFluidSimulationShaderInstance* ComputeShading;
};
