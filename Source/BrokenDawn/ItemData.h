#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

class UItemDefinition;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon     UMETA(DisplayName = "Weapon"),
	Consumable UMETA(DisplayName = "Consumable"),
};

USTRUCT(BlueprintType)
struct FItemStatisticsData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Statistics")
	float DamageValue;
};

USTRUCT(BlueprintType)
struct FItemNumericData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric")
	int32 MaxStackSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Numeric")
	bool bIsStackable;
};

USTRUCT(BlueprintType)
struct FItemTextData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text")
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text")
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text")
	FText Interaction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text")
	FText Usage;
};

USTRUCT(BlueprintType)
struct FItemAssetData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Asset")
	UTexture2D* Icon;
	
	UPROPERTY(EditAnywhere, Category = "Asset")
	UStaticMesh* Mesh;
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	TSoftObjectPtr<UItemDefinition> ItemBase;
};

