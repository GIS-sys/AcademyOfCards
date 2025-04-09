// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class ACADEMYOFCARDS_API LevelSaveInstance
{
public:
	enum DataType {
		POINTER = 0,
		COPY = 1,
		SHAREDPTR = 2
	};

private:
	TMap<FString, void*> Data;
	TMap<FString, DataType> DataTypes;

public:
	static const FString DEFAULT_NAME;
	LevelSaveInstance() = default;
	~LevelSaveInstance() = default;

	template <typename T>
	LevelSaveInstance(const T& X) {
		SetCopy(DEFAULT_NAME, X);
	}

	template <typename T>
	void Set(const FString& Name, T* X) {
		Data.Add(Name, X);
		DataTypes.Add(Name, DataType::POINTER);
	}
	template <typename T>
	void SetCopy(const FString& Name, const T& X) {
		T* XCopy = new T(X);
		Set<T>(Name, XCopy);
		DataTypes.Add(Name, DataType::COPY);
	}
	/*template <typename T>
	void SetSharedPtr(const FString& Name, T* X) {
		T* XShared = new TSharedPtr(X);
		Set<T>(Name, XShared);
		DataTypes.Add(Name, DataType::SHAREDPTR);
	}*/

	void Remove(const FString& Name) {
		Data.Remove(Name);
		DataTypes.Remove(Name);
	}

	template <typename T>
	T* Get(const FString& Name) {
		void** Raw = Data.Find(Name);
		if (!Raw) return nullptr;
		return static_cast<T*>(*Raw);
	}
	template <typename T>
	T GetAsCopy(const FString& Name) {
		return T(*Get<T>(Name));
	}
	/*template <typename T>
	TSharedPtr<T>* GetSharedPtr(const FString& Name) {
		return Get<TSharedPtr<T>>(Name);
	}*/
	
	template <typename T>
	T ExtractAsCopy(const FString& Name) {
		T X = GetAsCopy(Name);
		Remove(Name);
		return X;
	}

	bool IsDataCopy(const FString& Name) const { return *DataTypes.Find(Name) == DataType::COPY; }
	bool IsDataPointer(const FString& Name) const { return *DataTypes.Find(Name) == DataType::POINTER; }
	bool IsDataSharedPtr(const FString& Name) const { return *DataTypes.Find(Name) == DataType::SHAREDPTR; }
};
