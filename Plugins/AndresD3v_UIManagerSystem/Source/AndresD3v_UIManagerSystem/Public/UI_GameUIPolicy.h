// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UI_ManagerSubsystem.h"
#include "UI_GameUIPolicy.generated.h"

class UUI_GameUILayout;
class UUI_GameUIContext;

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
	/**
	 * The local player associated with the layout.
	 */
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;
	
	/**
	 * The root layout for the local player.
	 */
	UPROPERTY(Transient)
	TObjectPtr<class UUI_GameUILayout> RootLayout = nullptr;
	
	/**
	 * Indicates if the layout is added to the viewport.
	 */
	UPROPERTY(Transient)
	bool bIsAddedToViewport = false;
	
	/**
	 * Base class for UI context data shared across multiple UI elements.
	 * @details Allows subclassing to add custom data for UI interactions.
	 */
	UPROPERTY(Transient)
	TArray<TObjectPtr<UUI_GameUIContext>> Contexts;

	/**
	 * Default constructor.
	 */
	FRootViewportLayoutInfo(){}
	/**
	 * Constructor with parameters.
	 * @param InLocalPlayer The local player associated with the layout.
	 * @param InRootLayout The root layout for the local player.
	 * @param bInIsAddedToViewport Indicates if the layout is added to the viewport.
	 */
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, class UUI_GameUILayout* InRootLayout, bool bInIsAddedToViewport)
		: LocalPlayer(InLocalPlayer), RootLayout(InRootLayout), bIsAddedToViewport(bInIsAddedToViewport) {}

	/**
	 * Equality operator to compare with a local player.
	 * @param OtherLocalPlayer The local player to compare with.
	 * @return True if the local players match, false otherwise.
	 */
	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }

};


/**
 * 
 */
UCLASS(Abstract, Blueprintable, Within = UI_ManagerSubsystem)
class ANDRESD3V_UIMANAGERSYSTEM_API UUI_GameUIPolicy : public UObject
{
	GENERATED_BODY()
public:
	/* Retrieves the root layout for the specified local player.
	 * @param LocalPlayer The local player for whom to retrieve the root layout.
	 * @return The root layout for the specified local player.
	 */
	UUI_GameUILayout* GetRootLayout(const ULocalPlayer* LocalPlayer) const;

	UPROPERTY(EditDefaultsOnly, Category = "Layout")
	TSoftClassPtr<UUI_GameUILayout> PrimaryLayoutClass;

protected:
	// --------------------------- LAYOUT MANAGEMENT --------------------------
	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);
	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UUI_GameUILayout* Layout);
	void RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UUI_GameUILayout* Layout);
	
	// --------------------------- PLAYER FUNCTIONS ---------------------------
	virtual void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);

	// --------------------------- GAME UI CONTEXT FUNCTIONS ------------------
	virtual void AddContext(const ULocalPlayer* LocalPlayer, UUI_GameUIContext* NewContext);
	virtual void RemoveContext(const ULocalPlayer* LocalPlayer, TSubclassOf<UUI_GameUIContext> ContextClass);
	virtual UUI_GameUIContext* GetContext(const ULocalPlayer* LocalPlayer, TSubclassOf<UUI_GameUIContext> ContextClass);

	
	
	UUI_ManagerSubsystem* GetOwningSubsystem() const;
	virtual UWorld* GetWorld() const override;

private:
	
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;

	
	friend class UUI_ManagerSubsystem;
};
