// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPawn.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	// CDO(Class Defalut Object) 초기화
	// 에셋 로딩을 해선 안됨
	// 변수 값을 초기화하는 영역, 에디터 기본 값
	DefaultPawnClass = AMyPawn::StaticClass(); // CDO 포인터, Meaning 클래스 이름
	PlayerControllerClass = AMyPlayerController::StaticClass();
}

