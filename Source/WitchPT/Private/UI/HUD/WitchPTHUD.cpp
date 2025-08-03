// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/WitchPTHUD.h"

#include "MVVMGameSubsystem.h"
#include "Blueprint/UserWidgetPool.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Item/CauldronAltar.h"
#include "Player/WitchPTPlayerController.h"
#include "UI/ViewModels/MVVM_OverlayViewModel.h"
#include "UI/WidgetControllers/CauldronWidgetController.h"
#include "UI/WidgetControllers/InventoryWidgetController.h"
#include "UI/WidgetControllers/OverlayWidgetController.h"
#include "UI/WidgetControllers/QuickBarWidgetController.h"
#include "UI/WidgetControllers/RitualWidgetController.h"
#include "UI/Widgets/QuickBarUserWidget.h"
#include "UI/Widgets/WitchPTPrimaryLayout.h"
#include "UI/Widgets/WitchPTUserWidget.h"
#include "UI/Widgets/Inventory/InventoryUserWidget.h"
#include "UI/Widgets/Inventory/RitualUserWidget.h"

void AWitchPTHUD::BeginPlay()
{
	Super::BeginPlay();

}
void AWitchPTHUD::InitializeViewModels(APlayerController *PC, APlayerState *PS, UAbilitySystemComponent *ASC, UAttributeSet *AS)
{
	for (auto& ViewModelClass : ViewModelClasses)
	{
		UWitchPT_MVVMViewModelBase* ViewModelInstance = NewObject<UWitchPT_MVVMViewModelBase>(this, ViewModelClass.Key);
		ViewModelInstance->SetViewModelParams(FViewModelParams(PC, PS, ASC, AS));
		ViewModelInstance->BindCallbacksToDependencies();
		ViewModelInstance->BroadcastInitialValues();

		if (UMVVMGameSubsystem* MVVMSubsystem = GetGameInstance()->GetSubsystem<UMVVMGameSubsystem>())
		{
			if (UMVVMViewModelCollectionObject* MVVM_Collection = MVVMSubsystem->GetViewModelCollection())
			{
				FMVVMViewModelContext MVVM_Context(ViewModelClass.Key, ViewModelClass.Value);
				MVVM_Collection->AddViewModelInstance(MVVM_Context, ViewModelInstance);
			}
		}
	}
}



