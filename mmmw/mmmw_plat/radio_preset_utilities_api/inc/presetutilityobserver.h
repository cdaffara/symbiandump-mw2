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
* Description:  This class provides the interface class to be implemented by object
*               receiving events from preset control.
*
*
*/

#ifndef MPRESETUTILITYOBSERVER_H_
#define MPRESETUTILITYOBSERVER_H_

/**
 *  This class provides the interface class to be implemented by object receiving events
 *  from preset control.
 */
NONSHARABLE_CLASS( MPresetUtilityObserver )
    {
public:

    /**  Preset event */
    enum TPresetChangeEvent
        {
        EPresetAdded,
        EPresetDeleted,
        EPresetUpdated
        };

    /**
     * Called when a preset changes.
     *
     * NOTE: EPresetDeleted with aIndex == -1 (or < 0)=, indicates that all presets have been deleted.
     *
     * @since S60 3.2
     * @param aChange Change event type
     * @param aIndex Index to the preset that has changed. aIndex == -1 means all presets.
     */
    virtual void PresetChanged( TPresetChangeEvent aChange, TInt aIndex ) = 0;

    };

#endif // MPRESETUTILITYOBSERVER_H_
