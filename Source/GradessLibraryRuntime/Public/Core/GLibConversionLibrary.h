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
	static FString ConvertInt64ToString(const int64& Integer);

	/**
	 * @brief Converts array of names to array of strings
	 * @param InArray array of names
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
		UPARAM(DisplayName = "Array") const TArray<FName>& InArray
	);

	/**
	 * @brief Converts array of names to array of texts
	 * @param InArray array of names
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
		UPARAM(DisplayName = "Array") const TArray<FName>& InArray
	);

	/**
	* @brief Converts array of names to array of floats
	* @param InArray array of names
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
	static TArray<float> ConvertNameArrayToFloatArray(
		UPARAM(DisplayName = "Array") const TArray<FName>& InArray
	);

	/**
	 * @brief Converts array of names to array of integers
	 * @param InArray array of names
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
	static TArray<int32> ConvertNameArrayToIntArray(
		UPARAM(DisplayName = "Array") const TArray<FName>& InArray
	);

	/**
	 * @brief Converts array of names to array of 64-bit integers
	 * @param InArray array of names
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
	static TArray<int64> ConvertNameArrayToInt64Array(
		UPARAM(DisplayName = "Array") const TArray<FName>& InArray
	);

	/**
	 * @brief Converts array of names to array of bytes
	 * @param InArray array of names
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
	static TArray<uint8> ConvertNameArrayToByteArray(
		UPARAM(DisplayName = "Array") const TArray<FName>& InArray
	);

	/**
	 * @brief Converts array of names to array of booleans
	 * @param InArray array of names
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
	static TArray<bool> ConvertNameArrayToBoolArray(
		UPARAM(DisplayName = "Array") const TArray<FName>& InArray
	);

	/**
	 * @brief Converts array of strings to array of names
	 * @param InArray array of strings
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
		UPARAM(DisplayName = "Array") const TArray<FString>& InArray
	);

	/**
	 * @brief Converts array of strings to array of texts
	 * @param InArray array of strings
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
		UPARAM(DisplayName = "Array") const TArray<FString>& InArray
	);

	/**
	 * @brief Converts array of strings to array of floats
	 * @param InArray array of strings
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
	static TArray<float> ConvertStringArrayToFloatArray(
		UPARAM(DisplayName = "Array") const TArray<FString>& InArray
	);

	/**
	 * @brief Converts array of strings to array of integers
	 * @param InArray array of strings
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
		UPARAM(DisplayName = "Array") const TArray<FString>& InArray
	);

	/**
	 * @brief Converts array of strings to array of 64-bit integers
	 * @param InArray array of strings
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
		UPARAM(DisplayName = "Array") const TArray<FString>& InArray
	);

	/**
	 * @brief Converts array of strings to array of bytes
	 * @param InArray array of strings
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
	static TArray<uint8> ConvertStringArrayToByteArray(
		UPARAM(DisplayName = "Array") const TArray<FString>& InArray
	);

	/**
	 * @brief Converts array of strings to array of booleans
	 * @param InArray array of strings
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
	static TArray<bool> ConvertStringArrayToBoolArray(
		UPARAM(DisplayName = "Array") const TArray<FString>& InArray
	);

	/**
	 * @brief Converts array of texts to array of strings
	 * @param InArray array of texts
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
		UPARAM(DisplayName = "Array") const TArray<FText>& InArray
	);

	/**
	 * @brief Converts array of texts to array of strings
	 * @param InArray array of texts
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
		UPARAM(DisplayName = "Array") const TArray<FText>& InArray
	);

	/**
	 * @brief Converts array of texts to array of floats
	 * @param InArray array of texts
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
	static TArray<float> ConvertTextArrayToFloatArray(
		UPARAM(DisplayName = "Array") const TArray<FText>& InArray
	);

	/**
	 * @brief Converts array of texts to array of integers
	 * @param InArray array of texts
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
	static TArray<int32> ConvertTextArrayToIntArray(
		UPARAM(DisplayName = "Array") const TArray<FText>& InArray
	);

	/**
	 * @brief Converts array of texts to array of 64-bit integers
	 * @param InArray array of texts
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
	static TArray<int64> ConvertTextArrayToInt64Array(
		UPARAM(DisplayName = "Array") const TArray<FText>& InArray
	);

	/**
	 * @brief Converts array of texts to array of bytes
	 * @param InArray array of texts
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
	static TArray<uint8> ConvertTextArrayToByteArray(
		UPARAM(DisplayName = "Array") const TArray<FText>& InArray
	);

	/**
	 * @brief Converts array of texts to array of booleans
	 * @param InArray array of texts
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
	static TArray<bool> ConvertTextArrayToBoolArray(
		UPARAM(DisplayName = "Array") const TArray<FText>& InArray
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

	/**
	 * @brief Converts array elements from one type to another
	 * @tparam From current array element type
	 * @tparam To target array element type
	 * @param SourceArray array to convert
	 * @param OutArray converted array
	 * @param ConversionFunction function to convert element from "A" to "B"
	 */
	template <typename From, typename To>
	static void ConvertArray(
		const TArray<From>& SourceArray,
		TArray<To>& OutArray,
		std::function<To(const From&)> ConversionFunction
	);

	/**
	 * @brief Converts array elements from one type to another
	 * @tparam From current array element type
	 * @tparam To target array element type
	 * @param SourceArray array to convert
	 * @param ConversionFunction function to convert element from "A" to "B"
	 * @return converted array
	 */
	template <typename From, typename To>
	static TArray<To> ConvertArray(
		const TArray<From>& SourceArray,
		std::function<To(const From&)> ConversionFunction
	);

	/**
	 * @brief Converts array elements from one type to another
	 * @tparam From current array element type
	 * @tparam To target array element type
	 * @param SourceArray array to convert
	 * @param OutArray converted array
	 * @param bExcludeNullptr if true, exclude all unsuccessful casted object, false otherwise
	 */
	template <typename From, typename To>
	static void ConvertArray(
		const TArray<From*>& SourceArray,
		TArray<To*>& OutArray,
		const bool bExcludeNullptr = false
	);

private:
	DECLARE_FUNCTION(execConvertObjectArray_Internal);
};
