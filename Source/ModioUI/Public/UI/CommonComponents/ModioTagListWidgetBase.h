/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseWidgets/ModioListView.h"
#include "UI/BaseWidgets/ModioUserWidgetBase.h"

#include "ModioTagListWidgetBase.generated.h"

/**
* Modio UI element that to wrap a list of viewable mod tags. It can
* display, clear or select elements within its list
**/
UCLASS()
class MODIOUI_API UModioTagListWidgetBase : public UModioUserWidgetBase
{
	GENERATED_BODY()
protected:
	virtual void NativeOnSetDataSource() override;
	virtual void NativePreConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Widgets", meta = (BindWidget))
	UModioListView* TagSelectorList;
	int32 CurrentlyDisplayedTagCategoryIndex = 0;
	int32 TagCategoryCount = 0;

	virtual FReply NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent) override;

public:
	/**
	* Retrieve the list of tags in this list
	* @return 
	**/
	UFUNCTION(BlueprintCallable, Category = "ModioTagWidgetBase")
	TArray<FString> GetSelectedTags();

	/**
	 * Clear the list of tags in this list
	 **/
	UFUNCTION(BlueprintCallable, Category = "ModioTagWidgetBase")
	void ClearSelectedTags();

	/**
	 * Retrieve the list of tags in this list
	 **/
	UFUNCTION(BlueprintCallable, Category = "ModioTagWidgetBase")
	void DisplayNextTagCategory();

	/**
	 * Retrieve the list of tags in this list
	 **/
	UFUNCTION(BlueprintCallable, Category = "ModioTagWidgetBase")
	void DisplayPrevTagCategory();
};
