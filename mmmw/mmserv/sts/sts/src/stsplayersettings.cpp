/*
 * Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description:
 * This file provides the implementation for creating and deleting a
 * an MMF based player for playing and stopping a tone playback.
 */

//  Include Files  
#include "stsplayersettings.h"
_LIT(KDefaultFile,"z:\\data\\sounds\\digital\\clock.aac");

CStsPlayerSettings::CStsPlayerSettings() :
    iFileName(0), iNumberOfRepeats(0), iRepeatDelay(0),
            iVolumePercentage(50), iVolumeRamp(0)
    {
    TBufC<40> defaultFile(KDefaultFile); 
    iFileName = defaultFile.AllocL();
    }

CStsPlayerSettings::~CStsPlayerSettings()
    {
    delete iFileName;
    }

TPtrC CStsPlayerSettings::GetFileName()
    {
    return iFileName->Des();
    }

int CStsPlayerSettings::GetNumberOfRepeats()
    {
    return iNumberOfRepeats;
    }

TTimeIntervalMicroSeconds& CStsPlayerSettings::GetRepeatDelay()
    {
    return iRepeatDelay;
    }

int CStsPlayerSettings::GetVolumePercentage()
    {
    return iVolumePercentage;
    }

TTimeIntervalMicroSeconds& CStsPlayerSettings::GetVolumeRamp()
    {
    return iVolumeRamp;
    }

void CStsPlayerSettings::SetFileName(const TDesC& aFileName)
    {
    delete iFileName;
    iFileName = aFileName.AllocL();
    }

void CStsPlayerSettings::SetNumberOfRepeats(int aNumberOfRepeats)
    {
    iNumberOfRepeats = aNumberOfRepeats;
    }

void CStsPlayerSettings::SetRepeatDelay(
        TTimeIntervalMicroSeconds aRepeatDelay)
    {
    iRepeatDelay = aRepeatDelay;
    }

void CStsPlayerSettings::SetVolumePercentage(int aVolumePercentage)
    {
    iVolumePercentage = aVolumePercentage;
    }

void CStsPlayerSettings::SetVolumeRamp(TTimeIntervalMicroSeconds aVolumeRamp)
    {
    iVolumeRamp = aVolumeRamp;
    }
