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
* Description:  Interface for Dtmf handling
*
*/


#ifndef MCCEDTMFINTERFACE_H
#define MCCEDTMFINTERFACE_H

#include "mccedtmfobserver.h"

/**
 *  Interface for Dtmf handling
 *
 *  
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCEDtmfInterface
    {
public:
    /**
    * Stops the transmission of a single DTMF tone, which had previously been
    * started by use of the StartDTMFTone function member, across a connected 
    * and active voice call. 
    *
    * @since S60 3.2
    * @param none
    * @return KErrNone if successful
    * @return KErrNotReady not ready to perform the requested action.
    * @return KErrNotSupported Current plug-in holding the call does not support dtmf functionality.
    */
    virtual TInt StopDtmfTone() = 0;

    /**
    * Starts the transmission of a single DTMF tone across a connected and 
    * active voice call. 
    *
    * @since S60 3.2
    * @param aTone The DTMF tone to be send
    * @return KErrNone if successful
    * @return KErrNotReady not ready to perform the requested action.
    * @return KErrNotSupported Current plug-in holding the call does not support dtmf functionality.
    */
    virtual TInt StartDtmfTone( const TChar aTone ) = 0;
      
    /**
    * Starts sending of DTMF tones string. 
    *
    * @since S60 3.2
    * @param aTones The DTMF tones string to be send
    * @return KErrNone if successful
    * @return KErrNotReady not ready to perform the requested action.
    * @return KErrNotSupported Current plug-in holding the call does not support dtmf functionality.
    */
    virtual TInt SendDtmfToneString( const TDesC& aTones ) = 0;
      
    /**
    * Continue or cancels the sending of a DTMF string when it was
    * stopped by the use of 'w' char in the string.
    *
    * @since S60 3.2
    * @param aContinue ETrue if sending of the DTMF string should continue,
    * EFalse if the rest of the DTMF string is to be discarded
    * @return KErrNone if successful
    * @return KErrNotReady not ready to perform the requested action.
    * @return KErrNotSupported Current plug-in holding the call does not support dtmf functionality.
    */
    virtual TInt ContinueStringSending( const TBool aContinue ) = 0;
     
    /**
    * Cancels the ongoing DTMF tones string sending
    *
    * @since S60 3.2
    * @param none
    * @return none. TODO return KErrNotReady, NotFound or such?
    * @return KErrNotSupported Current plug-in holding the call does not support dtmf functionality.
    */
    virtual void CancelDtmfStringSending() = 0;
    
    /**
    * Sets the observer for DTMF events. Given when dtmf interface 
    * is first requested. Client is not required to call this.
    * TODO: remove this method and used Addobserver instead!
    * @since S60 3.2
    * @param aObserver Reference to observer interface
    * @param none
    */
    virtual void SetObserver( const MCCEDtmfObserver& aObserver ) = 0;
    
    /**
    * Adds a new observer for MCCEDtmfInterface object. 
    * @param aObserver Observer
    * @return none
    * @leave system error if observer adding fails
    */
    virtual void AddObserverL( const MCCEDtmfObserver& aObserver ) = 0;

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer
    * @return KErrNone if removed succesfully. KErrNotFound if observer was not found.
    */
    virtual TInt RemoveObserver( const MCCEDtmfObserver& aObserver ) = 0;
    };

#endif // MCCEDTMFINTERFACE_H
