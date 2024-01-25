// Fill out your copyright notice in the Description page of Project Settings.


#include "ComboWidget.h"

UComboWidget::UComboWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

void UComboWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UComboWidget::UpdateComboCount(int32 Value)
{
	if (TXTCombo && Value > 1)
	{
		// Turn textBox visible if already hidden
		if (TXTCombo->Visibility == ESlateVisibility::Hidden)
		{
			TXTCombo->SetVisibility(ESlateVisibility::Visible);
		}

		// Generate output like 2x Combo
		TXTCombo->SetText(FText::FromString("Current fuel: " + FString::FromInt(Value)));
	}
}

void UComboWidget::ResetCombo()
{
	if (TXTCombo)
	{
		TXTCombo->SetVisibility(ESlateVisibility::Hidden);
	}
}