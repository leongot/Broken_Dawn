#include "ItemDefinition.h"

UItemDefinition::UItemDefinition()
{

}

UItemDefinition* UItemDefinition::CreateItemCopy() const
{
	UItemDefinition* Copy = NewObject<UItemDefinition>(StaticClass());

	Copy->ID	    = ID;
	Copy->ItemType  = ItemType;
	Copy->TextData  = TextData;
	Copy->AssetData = AssetData;
	Copy->Quantity  = Quantity;

	return Copy;
}

FORCEINLINE void UItemDefinition::SetQuantity(const int32 NewQuantity)
{
	if (NewQuantity != Quantity)
		Quantity = FMath::Clamp(NewQuantity, 0, NumericData.bIsStackable ? NumericData.MaxStackSize : 1);
}
