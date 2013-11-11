/*
 * Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
 * Description: Telephony Multimedia Service
 *
 */

#ifndef TMSGLOBALEFFECTSSETTINGS_H
#define TMSGLOBALEFFECTSSETTINGS_H

//  INCLUDES
#include <e32property.h>
#include <sounddevice.h>
#include <centralrepository.h>

namespace TMS {

// CLASS DECLARATION
class TMSGlobalEffectsSettings
    {
public:
    /**
     * Two-phased constructor.
     */
    static TMSGlobalEffectsSettings* NewL();

    /**
     * Destructor.
     */
    virtual ~TMSGlobalEffectsSettings();

public:
    /**
     * Store the current loud speaker volume.
     * @param aVolume Volume
     */
    void SetLoudSpkrVolume(TInt aVolume);

    /**
     * Store the current earpiece volume.
     * @param aVolume Volume
     */
    void SetEarPieceVolume(TInt aVolume);


    /**
     * Retrieve the current loud speaker volume.
     * @param aVolume Volume
     */
    void GetLoudSpkrVolume(TInt& aVolume);

    /**
     * Retrieve the current earpiece volume.
     * @param aVolume Volume
     */
    void GetEarPieceVolume(TInt& aVolume);

    /**
     * Returns the current volume.
     * @return Current volume
     */
    TInt MaxVolume() const;

    /**
     * Store the current gain.
     * @param aVolume Volume
     * @param aPublish ETrue to publish the status to clients
     */
    void SetGain(TInt aGain);

    /**
     * Returns the current gain.
     * @return Current volume
     */
    TInt Gain() const;

    /**
     * Returns the max gain.
     * @return Current volume
     */
    TInt MaxGain() const;

    /**
     * Resets volume to default if level is 0 when new call starts.
     * @return
     */
    void ResetDefaultVolume();

private:

    /**
     * C++ default constructor.
     */
    TMSGlobalEffectsSettings();

    /**
     * By default Symbian 2nd phase constructor is private.
     */
    void ConstructL();

private:
    TInt iGain;
    TInt iMaxVolume;
    TInt iMaxGain;
    TInt iEarVolume;
    TInt iLoudSpkrVolume;

    CRepository* iRepository;
    };

} //namespace TMS

#endif // TMSGLOBALEFFECTSSETTINGS_H

// End of File
