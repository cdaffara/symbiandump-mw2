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
* Description:  CCP extension provider class. 
*
*/


#ifndef MCCPEXTENSIONPROVIDER_H
#define MCCPEXTENSIONPROVIDER_H

//  INCLUDES
#include <e32base.h>

class MCCPExtensionObserver;

/**
* Plug-in extension feature provider.
* E.g telephony engine might have plugin features specific to call providers functionality. 
* Plug-in specific actions can be requested via this class. 
* Asyncronous results are are returned using MCCPExtensionObserver
*
*  @since S60 3.2
*/
class MCCPExtensionProvider
    {
protected:
    /** 
    * Protected destructor. Object cannot be deleted from plug-in client (CCE).
    * @since S60 3.2
    */
    virtual inline ~MCCPExtensionProvider() {};
    
public:
    /**
    * Request a custom command from the plug-in. Result returned via MCCPExtensionObserver.
    * @since S60 3.2
    * @param aRequest Id of request. 
    * @param aData Request data.
    * @return none
    */
    virtual void RequestCommand( TInt aRequest, 
                                 TDesC8& aData ) = 0;
    /**
    * Cancels request. Result returned via MCCPExtensionObserver.
    * @since S60 3.2
    * @param aReqToCancel Id of request to cancel. 
    * @return none
    */
    virtual void CancelRequest( TInt aReqToCancel ) = 0;
    
    /**
    * Add an observer for extension related events.
    * Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer to add.
    * @return none
    * @leave system error if observer adding fails
    */
    virtual void AddObserverL( const MCCPExtensionObserver& aObserver ) = 0;

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param aObserver Observer to remove.
    * @return KErrNone if removed succesfully. 
    * @return KErrNotFound if observer was not found.
    */
    virtual TInt RemoveObserver( const MCCPExtensionObserver& aObserver ) = 0;
    
    };

#endif // MCCPEXTENSIONPROVIDER_H

