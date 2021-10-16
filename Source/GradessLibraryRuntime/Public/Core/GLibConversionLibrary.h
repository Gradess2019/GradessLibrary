// Copyright Gradess. All Rights Reserved.

#pragma once

#include <functional>
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GLibConversionLibrary.generated.h"

/**
 * Library that is used for converting arrays to different array type, int64 to string, etc.
 */
UCLASS()
class GRADESSLIBRARYRUNTIME_API UGLibConversionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * @brief Converts integer to string
	 * @param Integer integer to convert
	 * @return converted integer to string
	 */
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToString (int64)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static FString ConvertToString(const int64& Integer);

	/**
	* @brief Converts array of names to array of strings
	* @param Names array of names
	* @return converted array 
	* @return converted array 
	*/
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToString (name array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<FString> ConvertNameArrayToStringArray(
		UPARAM(ref) TArray<FName>& Names
	);

	/**
	* @brief Converts array of names to array of texts
	* @param Names array of names
	* @return converted array 
	*/
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToText (name array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<FText> ConvertNameArrayToTextArray(
		const TArray<FName>& Names
	);

	/**
	* @brief Converts array of strings to array of names
	* @param Strings array of strings
	* @return converted array 
	*/
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToName (string array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<FName> ConvertStringArrayToNameArray(
		UPARAM(ref) const TArray<FString>& Strings
	);

	/**
	 * @brief Converts array of strings to array of texts
	 * @param Strings array of strings
	 * @return converted array 
	 */
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToText (string array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<FText> ConvertStringArrayToTextArray(
		UPARAM(ref) const TArray<FString>& Strings
	);

	/**
	 * @brief Converts array of strings to array of integers
	 * @param Strings array of strings
	 * @return converted array 
	 */
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToInt (string array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<int32> ConvertStringArrayToIntArray(
		UPARAM(ref) const TArray<FString>& Strings
	);

	/**
	 * @brief Converts array of strings to array of 64-bit integers
	 * @param Strings array of strings
	 * @return converted array 
	 */
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToInt64 (string array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<int64> ConvertStringArrayToInt64Array(
		UPARAM(ref) const TArray<FString>& Strings
	);

	/**
	 * @brief Converts array of strings to array of floats
	 * @param Strings array of strings
	 * @return converted array 
	 */
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToFloat (string array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<float> ConvertStringArrayToFloatArray(
		UPARAM(ref) const TArray<FString>& Strings
	);

	/**
	 * @brief Converts array of texts to array of strings
	 * @param Texts array of texts
	 * @return converted array 
	 */
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToName (text array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<FName> ConvertTextArrayToNameArray(
		UPARAM(ref) const TArray<FText>& Texts
	);

	/**
	 * @brief Converts array of texts to array of strings
	 * @param Texts array of texts
	 * @return converted array 
	 */
	UFUNCTION(
		BlueprintPure,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "ToString (text array)",
			CompactNodeTitle = "->",
			BlueprintAutocast
		)
	)
	static TArray<FString> ConvertTextArrayToStringArray(
		UPARAM(ref) const TArray<FText>& Texts
	);

	/**
	* @brief Cast array of objects from one type to another
	* @param Objects object array to cast
	* @param ObjectClass to which class cast
	* @param bExcludeNullptr if true, exclude all unsuccessful casted object, false otherwise
	* @return casted array 
	*/
	UFUNCTION(
		BlueprintPure,
		CustomThunk,
		Category="GLib|Conversions",
		meta = (
			DisplayName = "Convert Object Array",
			DeterminesOutputType = "ObjectClass",
			CustomStructureParam = "Objects",
			BlueprintInternalUseOnly
		)
	)
	static TArray<UObject*> ConvertObjectArray_Internal(
		const TArray<UObject*>& Objects,
		TSubclassOf<UObject> ObjectClass,
		const bool bExcludeNullptr
	);

	template <typename From, typename To>
	static void ConvertArray(
		const TArray<From>& SourceArray,
		TArray<To>& ConvertedArray,
		std::function<To(const From&)> ConversionFunction
	);

private:
	DECLARE_FUNCTION(execConvertObjectArray_Internal);
};
