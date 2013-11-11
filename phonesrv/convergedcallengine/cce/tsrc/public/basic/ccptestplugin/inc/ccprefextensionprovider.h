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
* Description:  CCP test plug-in implementation.
*
*/


#ifndef MCCPREFEXTENSIONPROVIDER_H
#define MCCPREFEXTENSIONPROVIDER_H

//  INCLUDES
#include <e32base.h>
#include <mccpextensionprovider.h>
#include <mccpextensionobserver.h>

class CCPRefConvergedCallProvider;

/**
* Plug-in extension feature provider.
*/
class CCPRefExtensionProvider : public CBase,
                                public MCCPExtensionProvider

	{
public:
 
    /*
    * @param aProvider provider that owns this object.
    * @return created CCPRefExtensionProvider object
    */
    static CCPRefExtensionProvider* NewL( const CCPRefConvergedCallProvider& aProvider);

    /*
    * dtor
    */
    ~CCPRefExtensionProvider();    

    /**
    * Asynchronous  DoSometing() method
    * @since S60 3.2
    * @param aRequest Id of request
    * @param aData This object cant be larger than 256 bytes
    */
    void RequestCommand( TInt aRequest,
                         TDesC8& aData );
    /**
    * Cancels asynchronous request
    * @since S60 3.2
    * @param aReqToCancel Id of request to cancel
    */
    void CancelRequest( TInt aReqToCancel );

    /**
    * Add an observer for extension related events.
    * Plug-in dependent feature if duplicates or more than one observers 
    * are allowed or not. Currently CCE will set only one observer.
    * @since S60 v3.2
    * @param aObserver Observer
    * @return none
    * @leave system error if observer adding fails
    */
    void AddObserverL( const MCCPExtensionObserver& aObserver );

    /**
    * Remove an observer.
    * @since S60 v3.2
    * @param none
    * @param aObserver Observer
    * @return KErrNone if removed succesfully. KErrNotFound if observer was not found.
    * Any other system error depending on the error.
    */
    TInt RemoveObserver( const MCCPExtensionObserver& aObserver );

private:
    /**
    * constructor.
    */
    CCPRefExtensionProvider( const CCPRefConvergedCallProvider& aProvider );

    /**
    * By default Symbian 2nd phase constructor is private.
    */
    void ConstructL();

private:
    CCPRefConvergedCallProvider& iProvider;
    mutable MCCPExtensionObserver* iObserver;
	};


#endif //MCCPextensionPROVIDER_H

