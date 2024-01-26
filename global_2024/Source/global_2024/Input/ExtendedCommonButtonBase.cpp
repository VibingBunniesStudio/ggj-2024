// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/ExtendedCommonButtonBase.h"

FReply UExtendedCommonButtonBase::NativeOnFocusReceived(const FGeometry& InGeometry, const FFocusEvent& InFocusEvent)
{
	FReply Reply = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);

	TSharedPtr<SWidget> SafeWidget = GetCachedWidget();
	
	if (ULocalPlayer* LocalPlayer = GetOwningPlayer()->GetLocalPlayer())
	{
		TOptional<int32> UserIndex = FSlateApplication::Get().GetUserIndexForController(LocalPlayer->GetControllerId());
		if (UserIndex.IsSet())
		{
			FReply& DelayedSlateOperations = LocalPlayer->GetSlateOperations();
			DelayedSlateOperations.CancelFocusRequest();
			/*if (FSlateApplication::Get().SetUserFocus(UserIndex.GetValue(), SafeWidget))
			{
				DelayedSlateOperations.CancelFocusRequest();
			}
			else
			{
				DelayedSlateOperations.SetUserFocus(SafeWidget.ToSharedRef());
			}*/

		}
	}
	return Reply;
}
