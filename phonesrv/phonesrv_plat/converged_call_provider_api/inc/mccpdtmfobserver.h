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
* Description:  CCP DTMF observer. 
*
*/


#ifndef MCCPDTMFOBSERVER_H
#define MCCPDTMFOBSERVER_H

#include <e32base.h>

/**
* CCP DTMF related events observer class.
* This class is used for notifying DTMF specific events.
*
* @since S60 3.2
*/
class MCCPDTMFObserver
    {
public:

    /** DTMF Events */
    enum TCCPDtmfEvent
        {
        /** Unknown */
        ECCPDtmfUnknown = 0,
        /** DTMF sending started manually */
        ECCPDtmfManualStart = 5,
        /** DTMF sending stopped manually */
        ECCPDtmfManualStop = 6,
        /** DTMF sending aborted manually */
        ECCPDtmfManualAbort = 7,
        /** Automatic DTMF sending initialized */
        ECCPDtmfSequenceStart = 9,
        /** Automatic DTMF sending stopped */
        ECCPDtmfSequenceStop = 10,
        /** Automatic DTMF sending aborted */
        ECCPDtmfSequenceAbort = 11,
        /** There was stop mark in DTMF string */
        ECCPDtmfStopInDtmfString,
        /** DTMF sending completed succesfully */
        ECCPDtmfStringSendingCompleted
        };


protected:
    /** 
    * Protects the observer being deleted through the observer interface. 
    * @since S60 3.2
    */
    virtual inline ~MCCPDTMFObserver() {};
    
public:

    /**
    * HandleDTMFEvents.
    * @since S60 3.2
    * @param aEvent Event type
    * @param aError Error code
    * @param aTone Character
    * @return none
    */
    virtual void HandleDTMFEvent( const MCCPDTMFObserver::TCCPDtmfEvent aEvent,
                                  const TInt aError,
                                  const TChar aTone ) const = 0;
    };

#endif // MCCPDTMFOBSERVER_H

// End of File
