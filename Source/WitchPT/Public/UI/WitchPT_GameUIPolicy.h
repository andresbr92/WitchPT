// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/UIManagerSubsystem.h"
#include "UObject/Object.h"
#include "WitchPT_GameUIPolicy.generated.h"

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
	TObjectPtr<class UWitchPTPrimaryLayout> RootLayout = nullptr;
	/**
	 * Indicates if the layout is added to the viewport.
	 */
	UPROPERTY(Transient)
	bool bIsAddedToViewport = false;
	// TODO: Nice to have.
	/**
	 * Base class for UI context data shared across multiple UI elements.
	 * @details Allows subclassing to add custom data for UI interactions.
	 */
	// UPROPERTY(Transient)
	// TArray<TObjectPtr<WitchPT_GameUIContext>> Contexts;

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
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, class UWitchPTPrimaryLayout* InRootLayout, bool bInIsAddedToViewport)
		: LocalPlayer(InLocalPlayer), RootLayout(InRootLayout), bIsAddedToViewport(bInIsAddedToViewport) {}

	/**
	 * Equality operator to compare with a local player.
	 * @param OtherLocalPlayer The local player to compare with.
	 * @return True if the local players match, false otherwise.
	 */
	bool operator==(const ULocalPlayer* OtherLocalPlayer) const { return LocalPlayer == OtherLocalPlayer; }

};

class UWitchPTPrimaryLayout;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, Within = UIManagerSubsystem)
class WITCHPT_API UWitchPT_GameUIPolicy : public UObject
{
	GENERATED_BODY()
public:
	/* Retrieves the root layout for the specified local player.
	 * @param LocalPlayer The local player for whom to retrieve the root layout.
	 * @return The root layout for the specified local player.
	 */
	UWitchPTPrimaryLayout* GetRootLayout(const ULocalPlayer* LocalPlayer) const;

	UPROPERTY(EditDefaultsOnly, Category = "Layout")
	TSoftClassPtr<UWitchPTPrimaryLayout> PrimaryLayoutClass;

protected:
	void CreateLayoutWidget(ULocalPlayer* LocalPlayer);
	void AddLayoutToViewport(ULocalPlayer* LocalPlayer, UWitchPTPrimaryLayout* Layout);
	void RemoveLayoutFromViewport(ULocalPlayer* LocalPlayer, UWitchPTPrimaryLayout* Layout);
	
	virtual void NotifyPlayerAdded(ULocalPlayer* LocalPlayer);
	virtual void NotifyPlayerRemoved(ULocalPlayer* LocalPlayer);

	
	UUIManagerSubsystem* GetOwningSubsystem() const;
	virtual UWorld* GetWorld() const override;

private:
	
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;
	friend class UUIManagerSubsystem;
};
