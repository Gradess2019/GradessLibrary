// Copyright Gradess. All Rights Reserved.


#include "GradessLibraryRuntime/Public/Core/GLibConversionLibrary.h"
#include "UObject/UnrealType.h"
#include "Templates/SubclassOf.h"

FString UGLibConversionLibrary::ConvertInt64ToString(const int64& Integer)
{
	return FString::Printf(TEXT("%lld"), Integer);
}

TArray<FString> UGLibConversionLibrary::ConvertNameArrayToStringArray(const TArray<FName>& InArray)
{
	TArray<FString> OutArray;
	ConvertArray<FName, FString>(InArray, OutArray, [](const FName& Element) { return Element.ToString(); });

	return OutArray;
}

TArray<FText> UGLibConversionLibrary::ConvertNameArrayToTextArray(const TArray<FName>& InArray)
{
	TArray<FText> OutArray;
	ConvertArray<FName, FText>(InArray, OutArray, [](const FName& Element) { return FText::FromName(Element); });

	return OutArray;
}

TArray<float> UGLibConversionLibrary::ConvertNameArrayToFloatArray(const TArray<FName>& InArray)
{
	TArray<float> OutArray;
	ConvertArray<FName, float>(InArray, OutArray, [](const FName& Element) { return FCString::Atof(*Element.ToString()); });

	return OutArray;
}

TArray<int32> UGLibConversionLibrary::ConvertNameArrayToIntArray(const TArray<FName>& InArray)
{
	TArray<int32> OutArray;
	ConvertArray<FName, int32>(InArray, OutArray, [](const FName& Element)	{ return FCString::Atoi(*Element.ToString()); });

	return OutArray;
}

TArray<int64> UGLibConversionLibrary::ConvertNameArrayToInt64Array(const TArray<FName>& InArray)
{
	TArray<int64> OutArray;
	ConvertArray<FName, int64>(InArray, OutArray, [](const FName& Element)	{ return FCString::Atoi64(*Element.ToString());	});

	return OutArray;
}

TArray<uint8> UGLibConversionLibrary::ConvertNameArrayToByteArray(const TArray<FName>& InArray)
{
	TArray<uint8> OutArray;
	ConvertArray<FName, uint8>(InArray, OutArray, [](const FName& Element) { return static_cast<uint8>(FCString::Atoi(*Element.ToString())); });

	return OutArray;
}

TArray<bool> UGLibConversionLibrary::ConvertNameArrayToBoolArray(const TArray<FName>& InArray)
{
	TArray<bool> OutArray;
	ConvertArray<FName, bool>(InArray, OutArray, [](const FName& Element) { return Element.IsNone(); });

	return OutArray;
}

TArray<FName> UGLibConversionLibrary::ConvertStringArrayToNameArray(const TArray<FString>& InArray)
{
	TArray<FName> OutArray;
	ConvertArray<FString, FName>(InArray, OutArray, [](const FString& Element) { return FName(Element); });

	return OutArray;
}

TArray<FText> UGLibConversionLibrary::ConvertStringArrayToTextArray(const TArray<FString>& InArray)
{
	TArray<FText> OutArray;
	ConvertArray<FString, FText>(InArray, OutArray, [](const FString& Element) { return FText::FromString(Element); });

	return OutArray;
}

TArray<int32> UGLibConversionLibrary::ConvertStringArrayToIntArray(const TArray<FString>& InArray)
{
	TArray<int32> OutArray;
	ConvertArray<FString, int32>(InArray, OutArray, [](const FString& Element) { return FCString::Atoi(*Element); });

	return OutArray;
}

TArray<int64> UGLibConversionLibrary::ConvertStringArrayToInt64Array(const TArray<FString>& InArray)
{
	TArray<int64> OutArray;
	ConvertArray<FString, int64>(InArray, OutArray, [](const FString& Element) { return FCString::Atoi64(*Element); });

	return OutArray;
}

TArray<uint8> UGLibConversionLibrary::ConvertStringArrayToByteArray(const TArray<FString>& InArray)
{
	TArray<uint8> OutArray;
	ConvertArray<FString, uint8>(InArray, OutArray, [](const FString& Element) { return static_cast<uint8>(FCString::Atoi(*Element)); });

	return OutArray;
}

TArray<bool> UGLibConversionLibrary::ConvertStringArrayToBoolArray(const TArray<FString>& InArray)
{
	TArray<bool> OutArray;
	ConvertArray<FString, bool>(InArray, OutArray, [](const FString& Element) { return Element.IsEmpty(); });

	return OutArray;
}

TArray<float> UGLibConversionLibrary::ConvertStringArrayToFloatArray(const TArray<FString>& InArray)
{
	TArray<float> OutArray;
	ConvertArray<FString, float>(InArray, OutArray, [](const FString& Element) { return FCString::Atof(*Element); });

	return OutArray;
}

TArray<FName> UGLibConversionLibrary::ConvertTextArrayToNameArray(const TArray<FText>& InArray)
{
	TArray<FName> OutArray;
	ConvertArray<FText, FName>(InArray, OutArray, [](const FText& Element) { return FName(Element.ToString()); });

	return OutArray;
}

TArray<FString> UGLibConversionLibrary::ConvertTextArrayToStringArray(const TArray<FText>& InArray)
{
	TArray<FString> OutArray;
	ConvertArray<FText, FString>(InArray, [](const FText& Element) { return Element.ToString(); });

	return OutArray;
}

TArray<float> UGLibConversionLibrary::ConvertTextArrayToFloatArray(const TArray<FText>& InArray)
{
	TArray<float> OutArray;
	ConvertArray<FText, float>(InArray, [](const FText& Element) { return FCString::Atof(*Element.ToString()); });

	return OutArray;
}

TArray<int32> UGLibConversionLibrary::ConvertTextArrayToIntArray(const TArray<FText>& InArray)
{
	TArray<int32> OutArray;
	ConvertArray<FText, int32>(InArray, [](const FText& Element) { return FCString::Atoi(*Element.ToString()); });

	return OutArray;
}

TArray<int64> UGLibConversionLibrary::ConvertTextArrayToInt64Array(const TArray<FText>& InArray)
{
	TArray<int64> OutArray;
	ConvertArray<FText, int64>(InArray, [](const FText& Element) { return FCString::Atoi64(*Element.ToString()); });

	return OutArray;
}

TArray<uint8> UGLibConversionLibrary::ConvertTextArrayToByteArray(const TArray<FText>& InArray)
{
	TArray<uint8> OutArray;
	ConvertArray<FText, uint8>(InArray, [](const FText& Element) { return static_cast<uint8>(FCString::Atoi(*Element.ToString())); });

	return OutArray;
}

TArray<bool> UGLibConversionLibrary::ConvertTextArrayToBoolArray(const TArray<FText>& InArray)
{
	TArray<bool> OutArray;
	ConvertArray<FText, bool>(InArray, [](const FText& Element) { return Element.IsEmpty(); });

	return OutArray;
}

TArray<FName> UGLibConversionLibrary::ConvertIntArrayToNameArray(const TArray<int32>& InArray)
{
	TArray<FName> OutArray;
	ConvertArray<int32, FName>(InArray, [](const int32& Element) { return FName(FString::FromInt(Element)); });

	return OutArray;
}

TArray<FString> UGLibConversionLibrary::ConvertIntArrayToStringArray(const TArray<int32>& InArray)
{
	TArray<FString> OutArray;
	ConvertArray<int32, FString>(InArray, [](const int32& Element) { return FString::FromInt(Element); });

	return OutArray;
}

TArray<FText> UGLibConversionLibrary::ConvertIntArrayToTextArray(const TArray<int32>& InArray)
{
	TArray<FText> OutArray;
	ConvertArray<int32, FText>(InArray, [](const int32& Element) { return FText::FromString(FString::FromInt(Element)); });

	return OutArray;
}

TArray<float> UGLibConversionLibrary::ConvertIntArrayToFloatArray(const TArray<int32>& InArray)
{
	TArray<float> OutArray;
	ConvertArray<int32, float>(InArray, [](const int32& Element) { return static_cast<float>(Element); });

	return OutArray;
}

TArray<int64> UGLibConversionLibrary::ConvertIntArrayToInt64Array(const TArray<int32>& InArray)
{
	TArray<int64> OutArray;
	ConvertArray<int32, int64>(InArray, [](const int32& Element) { return static_cast<int64>(Element); });

	return OutArray;
}

TArray<uint8> UGLibConversionLibrary::ConvertIntArrayToByteArray(const TArray<int32>& InArray)
{
	TArray<uint8> OutArray;
	ConvertArray<int32, uint8>(InArray, [](const int32& Element) { return static_cast<uint8>(Element); });

	return OutArray;
}

TArray<bool> UGLibConversionLibrary::ConvertIntArrayToBoolArray(const TArray<int32>& InArray)
{
	TArray<bool> OutArray;
	ConvertArray<int32, bool>(InArray, [](const int32& Element) { return Element != 0; });

	return OutArray;
}

TArray<FName> UGLibConversionLibrary::ConvertInt64ArrayToNameArray(const TArray<int64>& InArray)
{
	TArray<FName> OutArray;
	ConvertArray<int64, FName>(InArray, [](const int64& Element) { return FName(FString::Printf(TEXT("%lld"), Element)); });

	return OutArray;
}

TArray<FString> UGLibConversionLibrary::ConvertInt64ArrayToStringArray(const TArray<int64>& InArray)
{
	TArray<FString> OutArray;
	ConvertArray<int64, FString>(InArray, [](const int64& Element) { return FString::Printf(TEXT("%lld"), Element); });

	return OutArray;
}

TArray<FText> UGLibConversionLibrary::ConvertInt64ArrayToTextArray(const TArray<int64>& InArray)
{
	TArray<FText> OutArray;
	ConvertArray<int64, FText>(InArray, [](const int64& Element) { return FText::FromString(FString::Printf(TEXT("%lld"), Element)); });

	return OutArray;
}

TArray<float> UGLibConversionLibrary::ConvertInt64ArrayToFloatArray(const TArray<int64>& InArray)
{
	TArray<float> OutArray;
	ConvertArray<int64, float>(InArray, [](const int64& Element) { return static_cast<int64>(Element); });

	return OutArray;
}

TArray<int32> UGLibConversionLibrary::ConvertInt64ArrayToIntArray(const TArray<int64>& InArray)
{
	TArray<int32> OutArray;
	ConvertArray<int64, int32>(InArray, [](const int64& Element) { return static_cast<int32>(Element); });

	return OutArray;
}

TArray<uint8> UGLibConversionLibrary::ConvertInt64ArrayToByteArray(const TArray<int64>& InArray)
{
	TArray<uint8> OutArray;
	ConvertArray<int64, uint8>(InArray, [](const int64& Element) { return static_cast<uint8>(Element); });

	return OutArray;
}

TArray<bool> UGLibConversionLibrary::ConvertInt64ArrayToBoolArray(const TArray<int64>& InArray)
{
	TArray<bool> OutArray;
	ConvertArray<int64, bool>(InArray, [](const int64& Element) { return Element != 0; });

	return OutArray;
}

TArray<FName> UGLibConversionLibrary::ConvertFloatArrayToNameArray(const TArray<float>& InArray)
{
	TArray<FName> OutArray;
	ConvertArray<float, FName>(InArray, [](const float& Element) { return FName(FString::SanitizeFloat(Element)); });

	return OutArray;
}

TArray<FString> UGLibConversionLibrary::ConvertFloatArrayToStringArray(const TArray<float>& InArray)
{
	TArray<FString> OutArray;
	ConvertArray<float, FString>(InArray, [](const float& Element) { return FString::SanitizeFloat(Element); });

	return OutArray;
}

TArray<FText> UGLibConversionLibrary::ConvertFloatArrayToTextArray(const TArray<float>& InArray)
{
	TArray<FText> OutArray;
	ConvertArray<float, FText>(InArray, [](const float& Element) { return FText::FromString(FString::SanitizeFloat(Element)); });

	return OutArray;
}

TArray<int32> UGLibConversionLibrary::ConvertFloatArrayToIntArray(const TArray<float>& InArray)
{
	TArray<int32> OutArray;
	ConvertArray<float, int32>(InArray, [](const float& Element) { return FMath::RoundToInt(Element);; });

	return OutArray;
}

TArray<int64> UGLibConversionLibrary::ConvertFloatArrayToInt64Array(const TArray<float>& InArray)
{
	TArray<int64> OutArray;
	ConvertArray<float, int64>(InArray, [](const float& Element) { return FMath::RoundToInt(Element); });

	return OutArray;
}

TArray<uint8> UGLibConversionLibrary::ConvertFloatArrayToByteArray(const TArray<float>& InArray)
{
	TArray<uint8> OutArray;
	ConvertArray<float, uint8>(InArray, [](const float& Element) { return static_cast<float>(Element); });

	return OutArray;
}

TArray<bool> UGLibConversionLibrary::ConvertFloatArrayToBoolArray(const TArray<float>& InArray)
{
	TArray<bool> OutArray;
	ConvertArray<float, bool>(InArray, [](const float& Element) { return FMath::IsNearlyZero(Element); });

	return OutArray;
}

TArray<UObject*> UGLibConversionLibrary::ConvertObjectArray_Internal(
	const TArray<UObject*>& Objects,
	TSubclassOf<UObject> ObjectClass,
	const bool bExcludeNullptr
)
{
	check(0);
	return TArray<UObject*>();
}

template <typename From, typename To>
void UGLibConversionLibrary::ConvertArray(
	const TArray<From>& SourceArray,
	TArray<To>& OutArray,
	std::function<To(const From&)> ConversionFunction
)
{
	OutArray.Reset(SourceArray.Num());
	for (auto& Element : SourceArray) { OutArray.Add(ConversionFunction(Element)); }
}

template <typename From, typename To>
TArray<To> UGLibConversionLibrary::ConvertArray(
	const TArray<From>& SourceArray,
	std::function<To(const From&)> ConversionFunction
)
{
	TArray<To> OutArray;
	ConvertArray<From, To>(SourceArray, OutArray, ConversionFunction);

	return OutArray;
}

template <typename From, typename To>
void UGLibConversionLibrary::ConvertArray(
	const TArray<From*>& SourceArray,
	TArray<To*>& OutArray,
	const bool bExcludeNullptr
)
{
	OutArray.Reset(SourceArray.Num());

	for (const auto Element : SourceArray)
	{
		const auto ElementToAdd = Element ? Cast<To>(Element) : nullptr;

		if (bExcludeNullptr && !ElementToAdd) { continue; }
		OutArray.Add(ElementToAdd);
	}
}

DEFINE_FUNCTION(UGLibConversionLibrary::execConvertObjectArray_Internal)
{
	Stack.MostRecentPropertyAddress = nullptr;
	Stack.MostRecentProperty = nullptr;

	P_GET_TARRAY_REF(UObject*, Objects);
	P_GET_OBJECT(UClass, ObjectClass);
	P_GET_UBOOL8(bExcludeNullptr)

	P_FINISH;

	P_NATIVE_BEGIN;

		TArray<UObject*> OutArray;
		OutArray.Reserve(Objects.Num());

		for (const auto Element : Objects)
		{
			const auto ElementToAdd = Element && Element->IsA(ObjectClass) ? Element : nullptr;

			if (bExcludeNullptr && !ElementToAdd) { continue; }
			OutArray.Add(ElementToAdd);
		}

		*static_cast<TArray<UObject*>*>(RESULT_PARAM) = OutArray;

	P_NATIVE_END;
}
