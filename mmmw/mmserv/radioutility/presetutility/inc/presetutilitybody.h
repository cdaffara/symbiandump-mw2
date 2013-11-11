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
* Description:  Preset Utility Body class.
*
*
*/

#ifndef C_PRESETUTILITYBODY_H
#define C_PRESETUTILITYBODY_H

#include <presetutility.h>

class MPresetUtilityObserver;
class CCenrepHandler;

NONSHARABLE_CLASS( CPresetUtilityBody ) : public CPresetUtility
    {
public:

    /**
    * Two-phased class constructor.
    */
    IMPORT_C static CPresetUtilityBody* NewL();

    /**
    * Destructor of CPresetUtilityBody class.
    */
    IMPORT_C ~CPresetUtilityBody();

// from base class CFmRadioPresetUtility

    void SetObserver( MPresetUtilityObserver* aObserver );
    TInt MaxNumberOfPresets() const;
    TInt PresetCount() const;
    TInt FirstPreset() const;
    TInt NextPreset( TInt aFromIndex ) const;
    TInt DeletePreset( TInt aIndex );
    TInt SavePresetL( const TPreset& aPreset, TInt aIndex );
    void ReadPresetL( TInt aIndex, TPreset& aPreset );

private:

    /**
    * Default class constructor.
    */
    CPresetUtilityBody();

    /**
    * Second phase class constructor.
    */
    void ConstructL();

private:

    // Observer object
    MPresetUtilityObserver*   iObserver;

    // Central Repository interface
    CCenrepHandler*         iCenrepHandler;

    };

#endif // C_PRESETUTILITYBODY_H
