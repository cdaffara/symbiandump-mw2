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
* Description:  Handles extension commands
*
*/


#ifndef CCCEEXTENSION_H
#define CCCEEXTENSION_H

#include <e32base.h>

#include "mccpextensionobserver.h"
#include "mcceextensioninterface.h"

class CCCEPluginManager;
class CCCECallContainer;
class MCCPExtensionProvider;
class MCCEExtensionObserver;
class CConvergedCallProvider;

/**
 *  CCE Dtmf
 *
 *
 *
 *  @lib cce.dll
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS(CCCEExtension): public CBase,
                             public MCCPExtensionObserver,
                             public MCCEExtensionInterface
    {
public:
    static CCCEExtension* NewL( CCCEPluginManager& aPluginManager );


    ~CCCEExtension();
   
    /**
    * Add observer to receive events 
    * @since S60 3.2
    * @param aObserver reference to observer
    * @return none
    */
    void AddObserverL( const MCCEExtensionObserver& aObserver );
  
    /**
    * Remove observer observer  
    * @since S60 3.2
    * @param aObserver reference to observer
    * @return System wide error code
    */
    TInt RemoveObserver( const MCCEExtensionObserver& aObserver );
    
public:

    // from base class MCCEExtensionInterface

    /** @see MCCEExtensionInterface::RequestCommand */
    TInt RequestCommand( TUint32 aServiceID, 
                        TInt aRequestId,
                        TDesC8& aData );

    /** @see MCCEExtensionInterface::CancelRequest */
    TInt CancelRequest( TUint32 aServiceID, 
                        TInt aRequestId );
  
    //from MCCPExtensionObserver

    /** @see MCCEExtensionObserver::HandleExtensionEvents */
    void HandleExtensionEvents( TUint32 aServiceId,
                                TInt aEvent,
                                TInt aStatus );

private:

    void ConstructL();

    /**
    *
    */
    CCCEExtension( CCCEPluginManager& aPluginManager );
    
    /**
    * Get plug-in pointer that relates to service-id.
    */
    CConvergedCallProvider* GetExtIfPlugin( TUint32 aServiceid );
    
    /**
    * Plug-in manager.
    */
    CCCEPluginManager& iPluginManager;

    /**
    * Extension observer 
    */
    MCCEExtensionObserver* iMCCEExtensionObserver;
    };

#endif // CCCEEXTENSION_H