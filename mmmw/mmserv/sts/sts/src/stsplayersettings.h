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
 * This file defines the interface for creating, playing,
 * stopping, and deleting an MMF player for a tone type.
 */
#ifndef STSPLAYERSETTINGS_H_
#define STSPLAYERSETTINGS_H_

#include <e32std.h>
#include "stssettingsmanager.h"

class CStsPlayerSettings : public CStsSettingsManager::MPlayerSettings
    {
public:
    CStsPlayerSettings();
    virtual ~CStsPlayerSettings();

    void SetFileName(const TDesC& aFileName);
    void SetNumberOfRepeats(int aNumberOfRepeats);
    void SetRepeatDelay(TTimeIntervalMicroSeconds aRepeatDelay);
    void SetVolumePercentage(int aVolumePercentage);
    void SetVolumeRamp(TTimeIntervalMicroSeconds aVolumeRamp);

    // inherited from CStsSettingsManager::MPlayerSettings
    virtual TPtrC GetFileName();
    virtual int GetNumberOfRepeats();
    virtual TTimeIntervalMicroSeconds& GetRepeatDelay();
    virtual int GetVolumePercentage();
    virtual TTimeIntervalMicroSeconds& GetVolumeRamp();

private:
    HBufC* iFileName;
    int iNumberOfRepeats;
    TTimeIntervalMicroSeconds iRepeatDelay;
    int iVolumePercentage;
    TTimeIntervalMicroSeconds iVolumeRamp;
    };

#endif //STSPLAYERSETTINGS_H_
