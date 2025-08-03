// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidgetPool.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI/Widgets/WitchPTUserWidget.h"
#include "UIManagerSubsystem.generated.h"

class UWitchPT_GameUIPolicy;
class UWitchPTWidgetController;
/**
 * 
 */

class UWitchPTUILayer;
struct FGameplayTag;
class UWitchPTPrimaryLayout;

UCLASS()
class WITCHPT_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="UI Manager", meta=(DynamicOutputParam="ReturnValue", DeterminesOutputType="WidgetClass"))
	UUserWidget* PushContentToLayer_ForPlayer(const APlayerController* PlayerController, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void PopContentFromLayer_ForPlayer(const APlayerController* PlayerController, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, int32 RemainNum = -1);
	
	UFUNCTION(BlueprintCallable, Category="UI Manager")
	virtual void AddPlayer(ULocalPlayer* LocalPlayer);
	
	UFUNCTION(BlueprintCallable, Category="UI Manager")
	void RemovePlayer(ULocalPlayer* LocalPlayer);

	virtual void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);

	
	
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	bool RegisterLayout(APlayerController* PlayerController, FGameplayTag LayoutTag, UWitchPTUILayer* InLayer);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	bool UnRegisterLayout(APlayerController* PlayerController, FGameplayTag LayerTag);
	



	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearAllLayers();

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearLayerExcept(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearLayer(FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void FocusGame(APlayerController* PlayerController);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void FocusModal(APlayerController* PlayerController, UUserWidget* WidgetToFocus, bool bShowCursor = true, bool bUIOnlyInput = false);
private:
	
	UWitchPTUserWidget* GetPrimaryLayout(const APlayerController* PlayerController) const;
	
	UPROPERTY(Transient)
	TObjectPtr<UWitchPT_GameUIPolicy> CurrentPolicy = nullptr;
	
};
