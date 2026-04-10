// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "MyPlayerController.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

#include "MyActor.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box")); // 컴포넌트 생성
	RootComponent = Box; // RootComponent 지정
	Box->SetBoxExtent(FVector(30.0f,32.0f,32.0f));

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);// Box에 붙임

	static ConstructorHelpers::FObjectFinder<UStaticMesh> 
		SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));

	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}

	PropellerLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropellerLeft"));
	PropellerLeft->SetupAttachment(Body);

	PropellerRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PropellerRight"));
	PropellerRight->SetupAttachment(Body);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);
	Arrow->SetRelativeLocation(FVector(200.f, 0, 0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));

	if (SM_Propeller.Succeeded())
	{
		PropellerLeft->SetStaticMesh(SM_Propeller.Object);
		PropellerRight->SetStaticMesh(SM_Propeller.Object);
	}

	PropellerLeft->SetRelativeLocation(FVector(40.000000f, -20.000000f, 0.000000f));
	PropellerRight->SetRelativeLocation(FVector(40.000000f, 20.000000f, 0.000000f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->SocketOffset = FVector(0, 0, 40.0f);
	SpringArm->TargetArmLength = 120.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));
	Movement->MaxSpeed = 500.0f;
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	
	FLatentActionInfo F;
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(),BoostValue);

	RotatePropeller(PropellerLeft, PropellerRotationSpeed);
	RotatePropeller(PropellerRight, PropellerRotationSpeed);
}

void AMyPawn::RotatePropeller(USceneComponent* Where, float Speed)
{
	Where->AddLocalRotation(FRotator(0, 0, Speed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}



// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, & AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyPawn::Fire);
	PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Pressed, this, &AMyPawn::Boost);
	PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Released, this, &AMyPawn::UnBoost);
	

}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(60 * Value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),0,0));
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0,0,60 * Value * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
}

void AMyPawn::Fire()
{
	GetWorld()->SpawnActor<AMyActor>(AMyActor::StaticClass(),
		Arrow->GetComponentTransform());
}

void AMyPawn::Boost()
{
	BoostValue = 1.0f;
}

void AMyPawn::UnBoost()
{
	BoostValue = 0.5f;
}
