// Copyright James Jenkins. All Rights Reserved.

#include "Helper.h"

FString UHelper::GetSessionName(const FBlueprintSessionResult& Result) {
	FString name;
    Result.OnlineResult.Session.SessionSettings.Get(FName("SESSION_NAME"), name);
	return name;
}
