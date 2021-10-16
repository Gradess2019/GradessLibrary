// Copyright Gradess. All Rights Reserved.


#include "GradessLibraryRuntime/Public/Core/GLibConversionLibrary.h"
#include "UObject/UnrealType.h"
#include "Templates/SubclassOf.h"

FString UGLibConversionLibrary::ConvertToString(const int64& Integer)
{
	return FString::Printf(TEXT("%lld"), Integer);
}

TArray<FString> UGLibConversionLibrary::ConvertNameArrayToStringArray(TArray<FName>& Names)
{
	TArray<FString> Strings;
	ConvertArray<FName, FString>(Names, Strings, [](const FName& Name) { return Name.ToString(); });
	
	return Strings;
}

TArray<FText> UGLibConversionLibrary::ConvertNameArrayToTextArray(const TArray<FName>& Names)
{
	TArray<FText> Texts;
	ConvertArray<FName, FText>(Names, Texts, [](const FName& name) { return FText::FromName(name); });

	return Texts;
}

TArray<FName> UGLibConversionLibrary::ConvertStringArrayToNameArray(const TArray<FString>& Strings)
{
	TArray<FName> Names;
	ConvertArray<FString, FName>(Strings, Names, [](const FString& String) { return FName(String); });

	return Names;
}

TArray<FText> UGLibConversionLibrary::ConvertStringArrayToTextArray(const TArray<FString>& Strings)
{
	TArray<FText> Texts;
	ConvertArray<FString, FText>(Strings, Texts, [](const FString& String) { return FText::FromString(String); });

	return Texts;
}

TArray<int32> UGLibConversionLibrary::ConvertStringArrayToIntArray(const TArray<FString>& Strings)
{
	TArray<int32> Integers;
	ConvertArray<FString, int32>(Strings, Integers, [](const FString& String) { return FCString::Atoi(*String); });

	return Integers;
}

TArray<int64> UGLibConversionLibrary::ConvertStringArrayToInt64Array(const TArray<FString>& Strings)
{
	TArray<int64> Integers;
	ConvertArray<FString, int64>(Strings, Integers, [](const FString& String) { return FCString::Atoi64(*String); });

	return Integers;
}

TArray<float> UGLibConversionLibrary::ConvertStringArrayToFloatArray(const TArray<FString>& Strings)
{
	TArray<float> Floats;
	ConvertArray<FString, float>(Strings, Floats, [](const FString& String) { return FCString::Atof(*String); });

	return Floats;
}

TArray<FName> UGLibConversionLibrary::ConvertTextArrayToNameArray(const TArray<FText>& Texts)
{
	TArray<FName> names;
	ConvertArray<FText, FName>(Texts, names, [](const FText& text) { return FName(text.ToString()); });

	return names;
}

TArray<FString> UGLibConversionLibrary::ConvertTextArrayToStringArray(const TArray<FText>& Texts)
{
	TArray<FString> Strings;
	ConvertArray<FText, FString>(Texts, Strings, [](const FText& text) { return text.ToString(); });

	return Strings;
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
	TArray<To>& ConvertedArray,
	std::function<To(const From&)> ConversionFunction
)
{
	ConvertedArray.Reset();
	ConvertedArray.Reserve(SourceArray.Num());
	for (auto& element : SourceArray) { ConvertedArray.Add(ConversionFunction(element)); }
}

DEFINE_FUNCTION(UGLibConversionLibrary::execConvertObjectArray_Internal)
{
	Stack.MostRecentPropertyAddress = NULL;
	Stack.MostRecentProperty = NULL;

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
	
	*(TArray<UObject*>*)RESULT_PARAM = OutArray;
	
	P_NATIVE_END;
}
