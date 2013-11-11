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
* Description:  Audio stream test component
*
*/

// AudioDataSettingsParams.h: interface for the CAudioDataSettingsParams class.

#ifndef __AUDIO_DATA_SETTINGS_PARAMS
#define __AUDIO_DATA_SETTINGS_PARAMS

#include "Parameters.h"
#include "mda/common/audio.h"

class CAudioDataSettingsParams : public CParameters
{
protected:
	CAudioDataSettingsParams(TInt aAction, TMdaAudioDataSettings *aSettings);
public:
	static CAudioDataSettingsParams* NewL(TInt aAction, TMdaAudioDataSettings *aSettings);
	static CAudioDataSettingsParams* NewLC(TInt aAction, TMdaAudioDataSettings *aSettings);
	virtual ~CAudioDataSettingsParams();
	TMdaAudioDataSettings *GetSettings();

private:
	TMdaAudioDataSettings *iSettings;
};

#endif	//__AUDIO_DATA_SETTINGS_PARAMS
