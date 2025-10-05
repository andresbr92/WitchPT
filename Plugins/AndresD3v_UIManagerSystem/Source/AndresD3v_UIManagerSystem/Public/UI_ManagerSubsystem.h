// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UI_ManagerSubsystem.generated.h"

class UUI_GameUILayer;
class UUI_GameUIContext;
class UUI_GameUIPolicy;
class UUserWidget;
/**
 * 
 */
UCLASS()
class ANDRESD3V_UIMANAGERSYSTEM_API UUI_ManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ------------------------- MAIN FUNCTIONS -------------------------
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category="UI Manager", meta=(DynamicOutputParam="ReturnValue", DeterminesOutputType="WidgetClass"))
	UUserWidget* PushContentToLayer_ForPlayer(const APlayerController* PlayerController, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Manager")
	void PushTemporalContentToLayer_ForPlayer(const APlayerController* PlayerController, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, UPARAM(meta = (AllowAbstract = false)) TSubclassOf<UUserWidget> WidgetClass, float Duration);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void PopContentFromLayer_ForPlayer(const APlayerController* PlayerController, UPARAM(meta = (Categories = "UI.Layer")) FGameplayTag LayerTag, int32 RemainNum = -1);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void FocusModal(APlayerController* PlayerController, UUserWidget* WidgetToFocus, bool bShowCursor = true, bool bUIOnlyInput = false);
	
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void FocusGame(APlayerController* PlayerController);
	

	// ------------------------- PLAYER FUNCTIONS -------------------------
	UFUNCTION(BlueprintCallable, Category="UI Manager")
	virtual void AddPlayer(ULocalPlayer* LocalPlayer);
	
	UFUNCTION(BlueprintCallable, Category="UI Manager")
	void RemovePlayer(ULocalPlayer* LocalPlayer);

	
	// --------------------------- GAME UI CONTEXT FUNCTIONS ------------------
	UFUNCTION(BlueprintCallable, Category = "Game UI Context", meta=(DefaultToSelf="PlayerController"))
	void RegisterUIContext_ForPlayer(APlayerController* PlayerController, UUI_GameUIContext* Context);

	UFUNCTION(BlueprintCallable, Category = "Game UI Context", meta=(DefaultToSelf="PlayerController"))
	void UnRegisterUIContext_ForPlayer(APlayerController* PlayerController, TSubclassOf<UUI_GameUIContext> ContextClass);
	
	UFUNCTION(BlueprintCallable, Category = "Game UI Context", meta=(DefaultToSelf="Actor"))
	void RegisterUIContext_ForActor(AActor* Actor, UUI_GameUIContext* Context);

	UFUNCTION(BlueprintCallable, Category="Game UI Context", meta=(DefaultToSelf="LocalPlayer", DeterminesOutputType="ContextClass", DynamicOutputParam="OutContext", ExpandBoolAsExecs="ReturnValue"))
	bool FindUIContext_ForPlayer(ULocalPlayer* LocalPlayer, TSubclassOf<UUI_GameUIContext> ContextClass, UUI_GameUIContext*& OutContext);

	virtual void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerDestroyed(ULocalPlayer* LocalPlayer);

	

	// ------------------------- TODO: pending implementation -------------------------
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	bool RegisterLayout(APlayerController* PlayerController, FGameplayTag LayoutTag, UUI_GameUILayer* InLayer);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	bool UnRegisterLayout(APlayerController* PlayerController, FGameplayTag LayerTag);

	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearAllLayers();
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearLayerExcept(FGameplayTag LayerTag);
	UFUNCTION(BlueprintCallable, Category = "UI Manager")
	void ClearLayer(FGameplayTag LayerTag);
private:
	
	UUserWidget* GetPrimaryLayout(const APlayerController* PlayerController) const;
	
	UPROPERTY(Transient)
	TObjectPtr<UUI_GameUIPolicy> CurrentPolicy = nullptr;
};
