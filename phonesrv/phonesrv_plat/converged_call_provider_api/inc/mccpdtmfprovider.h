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
* Description:  CCP DTMF provider class
*
*/


#ifndef MCCPDTMFPROVIDER_H
#define MCCPDTMFPROVIDER_H

//  INCLUDES
#include <e32base.h>

class MCCPDTMFObserver;

/**
* DTMF provider.
* Used to receive errors, notifications, statuses etc
*
*  @since S60 3.2
*/
class MCCPDTMFProvider
    {
protected:
    /** 
    * Protected destructor. Object cannot be deleted from plug-in client (CCE).
    * @since S60 3.2
    */
    virtual inline ~MCCPDTMFProvider() {};
    
public:
    /**
    * Cancels asynchronous DTMF string sending.
    * @param none
    * @return KErrNone request was started successfully
    * @return KErrNotFound nothing found to cancel.
    * @since Series 60 3.2
    */
    virtual TInt CancelDtmfStringSending() = 0;

    /**
    * Starts the transmission of a single DTMF tone across a connected and active call.
    * @since S60 3.2
    * @param aTone tone to be played.
    * @return KErrNone request was started successfully
    * @return KErrArgument if the specified tone contains illegal dtmf character
    * @return in case of an error system wide error code
    */
    virtual TInt StartDtmfTone( const TChar aTone ) = 0;

    /**
    * Stops playing current DTMF tone.
    * @since S60 3.2
    * @param none
    * @return KErrNone request was started successfully
    * @return KErrNotReady not ready to perform the requested action.
    */
    virtual TInt StopDtmfTone() = 0;

    /**
    * Plays DTMF string.
    * @since S60 3.2
    * @param aString String to be played.
    * @return KErrNone request was started successfully
    * @return KErrArgument if the specified string contains illegal dtmf characters
    */
    virtual TInt SendDtmfToneString( const TDesC& aString ) = 0;

    /**
    * Continue or cancel sending DTMF string which was stopped with 'w'-character
    * in string.
    * @since S60 3.2
    * @param aContinue ETrue if sending of the DTMF string should continue,
    * EFalse if the rest of the DTMF string is to be discarded.
    * @return KErrNone request was started successfully
    * @return KErrNotFound no send existed which to continue
    */
    virtual TInt ContinueDtmfStringSending( const TBool aContinue ) = 0;
    
    /**
    * Add an observer for DTMF related events.
    * Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer to add.
    * @return none
    * @leave system error if observer adding fails
    */
    virtual void AddObserverL( const MCCPDTMFObserver& aObserver ) = 0;

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param aObserver Observer to remove.
    * @return KErrNone if removed succesfully. 
    * @return KErrNotFound if observer was not found.
    */
    virtual TInt RemoveObserver( const MCCPDTMFObserver& aObserver ) = 0;

    };


#endif // MCCPDTMFPROVIDER_H

