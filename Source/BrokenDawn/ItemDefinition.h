#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.h"
#include "ItemDefinition.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BROKENDAWN_API UItemDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UItemDefinition();

	UFUNCTION(Category = "Item")
	UItemDefinition* CreateItemCopy() const;

	UFUNCTION(Category = "Item")
	FORCEINLINE void SetQuantity(const int32 NewQuantity);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemStatisticsData StatisticsData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemNumericData NumericData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemTextData TextData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	FItemAssetData AssetData;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data", meta = (UIMin = 1, UIMax = 32))
	int32 Quantity;

protected:
	FORCEINLINE bool operator ==(const FName& OtherID) const { return ID == OtherID; }

};
