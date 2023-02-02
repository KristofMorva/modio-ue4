/*
 *  Copyright (C) 2021 mod.io Pty Ltd. <https://mod.io>
 *
 *  This file is part of the mod.io UE4 Plugin.
 *
 *  Distributed under the MIT License. (See accompanying file LICENSE or
 *   view online at <https://github.com/modio/modio-ue4/blob/main/LICENSE>)
 *
 */

#include "UI/Views/Collection/ModioCollectionTileStatus.h"
#include "Core/ModioModCollectionEntryUI.h"

void UModioCollectionTileStatus::NativeOnSetDataSource()
{
	Super::NativeOnSetDataSource();
	if (UModioModCollectionEntryUI* Data = Cast<UModioModCollectionEntryUI>(DataSource))
	{
		UModioSubsystem* Subsystem = GEngine->GetEngineSubsystem<UModioSubsystem>();
		if (Subsystem)
		{
			TMap<FModioModID, FModioModCollectionEntry> UserMods = Subsystem->QueryUserSubscriptions();
			if (UserMods.Contains(Data->Underlying.GetID()))
			{
				SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				switch (UserMods[Data->Underlying.GetID()].GetModState())
				{
					case EModioModState::InstallationPending:
					case EModioModState::UninstallPending:
					case EModioModState::UpdatePending:

						if (StatusText)
						{
							StatusText->SetText(PendingLabelText);
							SetPercent(0.f);
						}
						break;

					case EModioModState::Downloading:
					case EModioModState::Extracting:
						BeginTickIfNeeded(true);
						break;
					case EModioModState::Installed:
						if (StatusText)
						{
							SetVisibility(ESlateVisibility::Hidden);
						}
						break;
				}
			}
			else
			{
				SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}

void UModioCollectionTileStatus::SetPercent(float InPercent)
{
	if (ProgressBar)
	{
		ProgressBar->SetPercent(InPercent);
	}
}

void UModioCollectionTileStatus::UpdateProgress(const struct FModioModProgressInfo& ProgressInfo)
{
	switch (ProgressInfo.GetCurrentState())
	{
		case EModioModProgressState::Downloading:
		{
			StatusText->SetText(DownloadingLabelText);
			SetPercent((float) ProgressInfo.GetCurrentProgress(EModioModProgressState::Downloading) / (float) ProgressInfo.GetTotalProgress(EModioModProgressState::Downloading));
		}
		break;
		case EModioModProgressState::Extracting:
		{
			StatusText->SetText(ExtractingLabelText);
			SetPercent((float) ProgressInfo.GetCurrentProgress(EModioModProgressState::Extracting)/ (float) ProgressInfo.GetTotalProgress(EModioModProgressState::Extracting));
		}
		break;
	}

}

void UModioCollectionTileStatus::NativeOnModManagementEvent(FModioModManagementEvent Event)
{
	IModioUIModManagementEventReceiver::NativeOnModManagementEvent(Event);
	if (DataSource)
	{
		UModioModCollectionEntryUI* Data = Cast<UModioModCollectionEntryUI>(DataSource);
		if (Data)
		{
			if (Data->Underlying.GetID() == Event.ID)
			{
				if (Event.Status)
				{
					// Display badge for error?
				}
				else
				{
					switch (Event.Event)
					{
						case EModioModManagementEventType::Installed:
						case EModioModManagementEventType::Updated:
							// hide/collapse the widget?
							/*if (Label)
							{
								Label->SetText(SubscribedLabelText);
							}
							SetPercent(100.f);*/
							break;
					}
				}
				// We don't need to keep ticking, success or fail
				bShouldTick = false;
			}
		}
	}
}
