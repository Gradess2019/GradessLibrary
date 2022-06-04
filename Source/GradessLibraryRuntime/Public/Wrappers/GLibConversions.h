// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModuleDescriptor.h"
#include "Projects/GLibModuleDescriptor.h"


namespace GLibConversions
{
	template<typename Native, typename Blueprintable>
	std::enable_if_t<std::is_enum_v<Native> && std::is_enum_v<Blueprintable>, Native> ToNative(const Blueprintable Value) { return static_cast<Native>(Value); }

	template<typename Blueprintable, typename Native>
	std::enable_if_t<std::is_enum_v<Native> && std::is_enum_v<Blueprintable>, Blueprintable> ToBlueprintable(const Native Value) { return static_cast<Blueprintable>(Value); }

	template<typename Native, typename Blueprintable>
	std::enable_if_t<std::is_enum_v<Native> && std::is_enum_v<Blueprintable>, TArray<Native>> ToNative(const TArray<Blueprintable> Values)
	{
		TArray<Native> NativeValues;
		NativeValues.Reserve(Values.Num());
		for (const Blueprintable& Value : Values)
		{
			NativeValues.Add(ToNative<Native>(Value));
		}
		return NativeValues;	
	}

	template<typename Blueprintable, typename Native>
	std::enable_if_t<std::is_enum_v<Native> && std::is_enum_v<Blueprintable>, TArray<Blueprintable>> ToBlueprintable(const TArray<Native> Values)
	{
		TArray<Blueprintable> BlueprintableValues;
		BlueprintableValues.Reserve(Values.Num());
		for (const Native& Value : Values)
		{
			BlueprintableValues.Add(ToBlueprintable<Blueprintable>(Value));
		}
		return BlueprintableValues;
	}

	// TODO add OutArray param instead of returning array
	template<typename Native, typename Blueprintable>
	std::enable_if_t<std::is_convertible_v<Blueprintable, Native>, TArray<Native>> ToNative(const TArray<Blueprintable> Values)
	{
		TArray<Native> NativeValues;
		NativeValues.Reserve(Values.Num());
		for (const Blueprintable& Value : Values)
		{
			NativeValues.Add(Value);
		}
		return NativeValues;	
	}

	template<typename Blueprintable, typename Native>
	std::enable_if_t<std::is_convertible_v<Native, Blueprintable>, TArray<Blueprintable>> ToBlueprintable(const TArray<Native> Values)
	{
		TArray<Blueprintable> BlueprintableValues;
		BlueprintableValues.Reserve(Values.Num());
		for (const Native& Value : Values)
		{
			BlueprintableValues.Add(Value);
		}
		return BlueprintableValues;
	}
	
	template<typename Native, typename Blueprintable>
	Native* ToNative(Blueprintable* Value) { return Value; }

	template<typename Blueprintable, typename Native>
	Blueprintable* ToBlueprintable(Native* Value) { return Blueprintable::CreateWrapper(Value); }

	template<typename Blueprintable, typename Native>
	Blueprintable* ToBlueprintable(TSharedPtr<Native> Value) { return Blueprintable::CreateWrapper(Value); }
	
	template<typename Blueprintable, typename Native>
	Blueprintable* ToBlueprintable(TSharedRef<Native> Value) { return Blueprintable::CreateWrapper(Value); }

	template<typename Blueprintable, typename Native>
	void ToNative(const TArray<Blueprintable>& Blueprintables, TArray<Native>& Natives)
	{
		Natives.Reset(Blueprintables.Num());
		for (auto Value : Blueprintables)
		{
			Natives.Add(ToNative<Native>(Value));
		}
	}
	
	template<typename Blueprintable, typename Native>
	void ToBlueprintable(const TArray<Native*>& Natives, TArray<Blueprintable*>& Blueprintables)
	{
		Blueprintables.Reset(Natives.Num());
		for (auto Value : Natives)
		{
			Blueprintables.Add(ToBlueprintable<Blueprintable>(Value));
		}
	}
	
	template<typename Blueprintable, typename Native>
	void ToBlueprintable(const TArray<TSharedPtr<Native>>& Natives, TArray<Blueprintable*>& Blueprintables)
	{
		Blueprintables.Reset(Natives.Num());
		for (auto Value : Natives)
		{
			Blueprintables.Add(ToBlueprintable<Blueprintable>(Value));
		}
	}

	template<typename Blueprintable, typename Native>
	void ToBlueprintable(const TArray<TSharedRef<Native>>& Natives, TArray<Blueprintable*>& Blueprintables)
	{
		Blueprintables.Reset(Natives.Num());
		for (auto Value : Natives)
		{
			Blueprintables.Add(ToBlueprintable<Blueprintable>(Value));
		}
	}
	

	/** ----------------------- EHostType begin ----------------------- */

	/**
	* Converts an EGLibHostType value to a string literal
	*
	* @param Value The value to convert to a string
	* @return The string representation of this enum value
	*/
	GRADESSLIBRARYRUNTIME_API inline const TCHAR* ToString(const EGLibHostType Value) { return EHostType::ToString(ToNative<EHostType::Type>(Value)); }
	
	/** ----------------------- EHostType end ----------------------- */


	
	/** ----------------------- ELoadingPhase begin ----------------------- */

	/**
	* Returns the name of a module load phase.
	*
	* @param Value The value to convert to a string
	* @return The string representation of this enum value
	*/
	GRADESSLIBRARYRUNTIME_API inline const TCHAR* ToString(const EGLibLoadingPhase Value) { return ELoadingPhase::ToString(ToNative<ELoadingPhase::Type>(Value)); }
	
	/** ----------------------- ELoadingPhase end ----------------------- */

}

namespace GLibHostType
{
	/**
	* Converts a string to a EGLibHostType value
	*
	* @param Text The string to convert to a value
	* @return The corresponding value, or 'Max' if the string is not valid.
	*/
	GRADESSLIBRARYRUNTIME_API EGLibHostType FromString(const TCHAR* Text);
}

namespace GLibLoadingPhase
{
	/**
	* Converts a string to a ELoadingPhase::Type value
	*
	* @param Text The string to convert to a value
	* @return The corresponding value, or 'Max' if the string is not valid.
	*/
	GRADESSLIBRARYRUNTIME_API EGLibLoadingPhase FromString(const TCHAR* Text);
}