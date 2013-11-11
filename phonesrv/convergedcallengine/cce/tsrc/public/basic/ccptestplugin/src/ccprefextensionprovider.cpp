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



//  INCLUDES
#include <e32base.h>
#include <e32debug.h>
#include "ccprefextensionprovider.h"
#include "ccprefconvergedcallprovider.h"
#include "ccetestconfigurations.h"

#include "configurationhandler.h"


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefExtensionProvider ::CCPRefExtensionProvider( const CCPRefConvergedCallProvider& aProvider ) :
    iProvider((CCPRefConvergedCallProvider&)aProvider),
    iObserver(NULL)
    {
    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefExtensionProvider::~CCPRefExtensionProvider ()
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefExtensionProvider* 
CCPRefExtensionProvider::NewL( const CCPRefConvergedCallProvider& aProvider )
    {
    CCPRefExtensionProvider* self = new( ELeave ) CCPRefExtensionProvider(aProvider);

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefExtensionProvider::ConstructL()
    {
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefExtensionProvider::RequestCommand( TInt aRequest,
                                              TDesC8& aData )
    {
    RDebug::Print(_L("[TESTPLUGIN] CCPRefExtensionProvider::RequestCommand") );
    RDebug::Print(_L("[TESTPLUGIN] aRequest: %d"), aRequest);
    
	CConfigurationHandler& cfh = iProvider.ConfigurationHandler();
	
	if( cfh.IsConfigRequest(aData) )
		{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefExtensionProvider::RequestCommand - request was a configuration action. %S"), &aData );
		if( !cfh.SetConfigData(aData) )
			{
			if( iObserver )
				{
				// TODO service id value 
				iObserver->HandleExtensionEvents(iProvider.ServiceId(), aRequest, KErrArgument );
				}
			}
		}

    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefExtensionProvider::CancelRequest( TInt /*aReqToCancel*/ )
    {
    }
    
// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
void CCPRefExtensionProvider::AddObserverL( const MCCPExtensionObserver& aObserver )
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPExtensionProvider::AddObserverL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefExtensionProvider::AddObserverL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}
    
    // only one observer used at a time, replaces current one
    iObserver = const_cast<MCCPExtensionObserver*>(&aObserver);
    }

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
TInt CCPRefExtensionProvider::RemoveObserver( const MCCPExtensionObserver& aObserver )
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPExtensionProvider::RemoveObserver"),value ) )
    	{
    	return value;
    	}
    
    if( iObserver!=&aObserver )
        {
        return KErrNotFound;
        }

    // only one observer used at a time
    iObserver = NULL;

    return KErrNone;
    }


