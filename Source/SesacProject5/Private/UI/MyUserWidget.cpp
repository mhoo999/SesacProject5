// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MyUserWidget.h"

#include "Components/WidgetSwitcher.h"

void UMyUserWidget::gotoLobby()
{
	widgetSwitcher->SetActiveWidgetIndex(1);
}
