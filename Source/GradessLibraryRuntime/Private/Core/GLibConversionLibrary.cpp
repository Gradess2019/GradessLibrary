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
void UGLibConversionLibrary::ConvertArray(
	const TArray<From*>& SourceArray,
	TArray<To*>& OutArray,
	const bool bExcludeNullptr
)
{
	OutArray.Reset(SourceArray.Num());

	for (const auto Element : SourceArray)
	{
		const auto ElementToAdd = Cast<To>(Element);

		if (bExcludeNullptr && !ElementToAdd) { continue; }
		OutArray.Add(ElementToAdd);
	}
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
