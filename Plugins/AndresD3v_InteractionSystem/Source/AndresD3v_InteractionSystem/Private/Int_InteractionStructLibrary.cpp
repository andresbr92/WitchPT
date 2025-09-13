
#include "Int_InteractionStructLibrary.h"

bool operator==(const FInt_InteractionOption& Lhs, const FInt_InteractionOption& RHS)
{
	return Lhs.Definition == RHS.Definition
		&& Lhs.RequestResult == RHS.RequestResult
		&& Lhs.SlotIndex == RHS.SlotIndex
		&& Lhs.SlotState == RHS.SlotState;
}
bool operator!=(const FInt_InteractionOption& Lhs, const FInt_InteractionOption& RHS)
{
	return !(Lhs == RHS);
}
bool operator<(const FInt_InteractionOption& Lhs, const FInt_InteractionOption& RHS)
{
	return Lhs.SlotIndex < RHS.SlotIndex;
}