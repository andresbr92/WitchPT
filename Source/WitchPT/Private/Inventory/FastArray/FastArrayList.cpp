#pragma once

#include "Inventory/FastArray/FastArrayList.h"

#include "CoreMinimal.h"
#include "Inventory/WitchPTInventoryItemDefinition.h"
#include "Inventory/Fragments/WitchPTInventoryItemFragment.h"
#include "Inventory/WitchPTInventoryItemInstance.h"
#include "Inventory/WitchPTInventoryManagerComponent.h"
#include "Inventory/Fragments/WitchPTInventoryFragment_Stackable.h"


class FLifetimeProperty;
struct FReplicationFlags;
//////////////////////////////////////////////////////////////////////
// FWitchPTInventoryEntry
FString FWitchPTInventoryEntry::GetDebugString() const
{
	TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s ( x %s)"), *GetNameSafe(Instance),  *GetNameSafe(ItemDef));
}



//////////////////////////////////////////////////////////////////////
// FWitchPTInventoryList
void FWitchPTInventoryList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
	UWitchPTInventoryManagerComponent* IMC = Cast<UWitchPTInventoryManagerComponent>(OwnerComponent);
	if (!IsValid(IMC)) return;
	for (int32 Index : RemovedIndices)
	{
		IMC->OnItemRemoved.Broadcast(Entries[Index].Instance);
	}
}

void FWitchPTInventoryList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	UWitchPTInventoryManagerComponent* IMC = Cast<UWitchPTInventoryManagerComponent>(OwnerComponent);
	if (!IsValid(IMC)) return;
	for (int32 Index : AddedIndices)
	{
		IMC->OnItemAdded.Broadcast(Entries[Index].Instance);
	}
}

void FWitchPTInventoryList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
	UWitchPTInventoryManagerComponent* IMC = Cast<UWitchPTInventoryManagerComponent>(OwnerComponent);
	if (!IsValid(IMC)) return;
	
	for (int32 Index : ChangedIndices)
	{
		IMC->OnItemStackChanged.Broadcast(Entries[Index].Instance);
	}
}

UInventoryItemDefinition* FWitchPTInventoryList::AddEntry(TSubclassOf<UWitchPTInventoryItemDefinition> ItemDef)
{
	UInventoryItemDefinition* Result = nullptr;

	check(ItemDef != nullptr);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());


	FWitchPTInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UInventoryItemDefinition>(OwnerComponent->GetOwner());
	NewEntry.Instance->SetItemDef(ItemDef);
	// ------ Stack stuff
	const UWitchPTInventoryItemFragment* BaseFragment = NewEntry.Instance->FindFragmentByClass(UWitchPTInventoryFragment_Stackable::StaticClass());
	const UWitchPTInventoryFragment_Stackable* StackableFragment = Cast<UWitchPTInventoryFragment_Stackable>(BaseFragment);
	if (IsValid(StackableFragment))
	{
		NewEntry.Instance->SetTotalStackCount(StackableFragment->GetStackCount());
	}
	for (UWitchPTInventoryItemFragment* Fragment : GetDefault<UWitchPTInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}

	Result = NewEntry.Instance;
	
	
	MarkItemDirty(NewEntry);

	return Result;
}

void FWitchPTInventoryList::AddEntry(UInventoryItemDefinition* Instance)
{
	unimplemented();
}

void FWitchPTInventoryList::RemoveEntry(UInventoryItemDefinition* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FWitchPTInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}
TArray<UInventoryItemDefinition*> FWitchPTInventoryList::GetAllItems() const
{
	TArray<UInventoryItemDefinition*> Results;
	Results.Reserve(Entries.Num());
	for (const FWitchPTInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}
