/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Observer interface for handling Dtmf events
*
*/


#ifndef MCCEDTMFOBSERVER_H
#define MCCEDTMFOBSERVER_H

#include <e32base.h>

/**
 *  Observer interface for handling Dtmf events
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCEDtmfObserver
    {
public:
    /** Dtmf events */
    enum TCCEDtmfEvent
        {
        ECCEDtmfUnknown = 0,
        ECCEDtmfManualStart = 5,
        ECCEDtmfManualStop = 6,
        ECCEDtmfManualAbort = 7,
        ECCEDtmfSequenceStart = 9,
        ECCEDtmfSequenceStop = 10,
        ECCEDtmfSequenceAbort = 11,
        ECCEDtmfStopInDtmfString,
        ECCEDtmfStringSendingCompleted
        };

    /**
    * All the occurred DTMF events are notified through this interface
    *
    * @since S60 3.2
    * @param aEvent Event code
    * @param aError Error code
    * @param aTone tone 
    * @return none
    */
    virtual void HandleDTMFEvent( const MCCEDtmfObserver::TCCEDtmfEvent aEvent, 
                                  const TInt aError, 
                                  const TChar aTone ) = 0;
    };

#endif // MCCEDTMFOBSERVER_H
