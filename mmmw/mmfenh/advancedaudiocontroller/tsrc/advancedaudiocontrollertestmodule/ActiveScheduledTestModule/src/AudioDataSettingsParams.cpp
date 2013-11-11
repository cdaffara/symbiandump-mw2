/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Active Scheduler Test Module
*
*/


#include "AudioDataSettingsParams.h"

CAudioDataSettingsParams* CAudioDataSettingsParams::NewLC(TInt aAction, TMdaAudioDataSettings *aSettings)
	{
	CAudioDataSettingsParams *__self = new (ELeave) CAudioDataSettingsParams(aAction, aSettings);
	CleanupStack::PushL(__self);
	return __self;
	}

CAudioDataSettingsParams* CAudioDataSettingsParams::NewL(TInt aAction, TMdaAudioDataSettings *aSettings)
	{
	CAudioDataSettingsParams *__self = CAudioDataSettingsParams::NewLC(aAction, aSettings);
	CleanupStack::Pop(__self);
	return __self;
	}

CAudioDataSettingsParams::CAudioDataSettingsParams(TInt aAction, TMdaAudioDataSettings *aSettings) : CParameters(aAction) , iSettings(aSettings) {}

CAudioDataSettingsParams::~CAudioDataSettingsParams()
	{
	}

TMdaAudioDataSettings *CAudioDataSettingsParams::GetSettings()
	{
	return iSettings;
	}
