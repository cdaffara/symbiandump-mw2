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
#include <mccpdtmfobserver.h>
#include "ccprefdtmfprovider.h"
#include "ccprefconvergedcallprovider.h"



// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefDTMFProvider::CCPRefDTMFProvider( const CCPRefConvergedCallProvider& aProvider ) : 
    iProvider(const_cast<CCPRefConvergedCallProvider&>(aProvider)),
    iObserver(NULL)
    {

    }

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefDTMFProvider::~CCPRefDTMFProvider()
    {
    delete iStringNotSent;
    }


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CCPRefDTMFProvider* CCPRefDTMFProvider::NewL( const CCPRefConvergedCallProvider& aProvider )
    {
    CCPRefDTMFProvider* self = new( ELeave ) CCPRefDTMFProvider(aProvider);

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// -----------------------------------------------------------------------------
// Allocate memory for member iStringNotSent
// -----------------------------------------------------------------------------
//
void CCPRefDTMFProvider::InitializeStringL( const TDesC& aStringNotSent )
    {
    if ( NULL == iStringNotSent )
    	{
	 	iStringNotSent = aStringNotSent.AllocL();
    	}
    }

// -----------------------------------------------------------------------------
// Release the memory allocated for member iStringNotSent
// -----------------------------------------------------------------------------
//
void CCPRefDTMFProvider::ClearDtmfString()
	{/* Pointers are NULL-ed because this method is called outside
    	the destructor */
	delete iStringNotSent;
	iStringNotSent = NULL;
	}    
	
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
void CCPRefDTMFProvider::ConstructL()
    {
    }



// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefDTMFProvider::CancelDtmfStringSending()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPDTMFProvider::CancelDtmfStringSending"),value ) )
    	{
    	return value;
    	}
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//

        
TInt CCPRefDTMFProvider::StartDtmfTone( const TChar aTone )
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPDTMFProvider::StartDtmfTone"),value ) )
    	{
    	return value;
    	}

	RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::StartDtmfTone") );
    
    iObserver->HandleDTMFEvent( MCCPDTMFObserver::ECCPDtmfManualStart, KErrNone, aTone );
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefDTMFProvider::StopDtmfTone()
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPDTMFProvider::StopDtmfTone"),value ) )
    	{
    	return value;
    	}

    RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::StopDtmfTone") );
    
    iObserver->HandleDTMFEvent( MCCPDTMFObserver::ECCPDtmfManualStop, KErrNone, NULL );
    
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefDTMFProvider::SendDtmfToneString( const TDesC& aString )
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPDTMFProvider::SendDtmfToneString"),value ) )
    	{
    	return value;
    	}

    RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::SendDtmfToneString") );
    
    TLex16 lex( aString );
    TBool exit = EFalse; //if pause found, exit function
	
	/* Make a copy of the aString to be called iStringNotSent */
	
	TRAPD(err, InitializeStringL(aString));
	   	
   	_LIT(KWaitChar, "w"); //this is the reference to the possible wait character 'w'
   	TBufC<1> waitTone(KWaitChar); //for comparing purposes
   	
   	//TPtrC aCharPtr = lex.NextToken(); //get rest of the tokens
   
	if ( err == KErrNone )
		{
		/* loop all the tones and send those until pause or end of the string */
   		for( TInt i = 0; i < aString.Length() && exit==EFalse; i++ )
   			{
   			TChar charTone = lex.Get(); //get the token for sending purposes
   			TPtrC toneCharPtr((TText*)&charTone);
    	
   			/* check if tone is waitcharacter 'w' */
   			if ( waitTone.CompareF(toneCharPtr) != 0 ) //send the tone other than wait tone 
   				{
   				RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::SendDtmfToneString:Tone sent") );
   				iObserver->HandleDTMFEvent( MCCPDTMFObserver::ECCPDtmfSequenceStart, KErrNone, charTone );
   				iObserver->HandleDTMFEvent( MCCPDTMFObserver::ECCPDtmfSequenceStop, KErrNone, charTone );
   				}
   			else /* Wait tone found, exit loop*/
   				{
   				/* Update or the member iString (rip the sent tones) */
   				TPtr pointer = iStringNotSent->Des(); //Create a pointer to modify
   				pointer.Delete(0, i+1); //Delete the tones from index to the point tones sent plus pause char
   				
   				exit = ETrue;
   				
   				RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::SendDtmfToneString:Pause") );
   				iObserver->HandleDTMFEvent( MCCPDTMFObserver::ECCPDtmfStopInDtmfString, KErrNone, NULL );
   				}
   			}
		}
   	else //Out of Mem
		{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::SendDtmfToneString:Out of Mem") );
		ClearDtmfString();// Destroys successfully allocated member data
		}

	/* check if sending is complete */
	if (EFalse == exit)
		{
		RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::SendDtmfToneString:DONE") );
   		iObserver->HandleDTMFEvent( MCCPDTMFObserver::ECCPDtmfStringSendingCompleted, KErrNone, NULL );
   		ClearDtmfString();
		}

   	return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CCPRefDTMFProvider::ContinueDtmfStringSending( const TBool aContinue )
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPDTMFProvider::ContinueDtmfStringSending"),value ) )
    	{
    	return value;
    	}

    if (aContinue)
    	{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::ContinueDtmfStringSending:Continue") );
    	
    	if ( iStringNotSent == NULL )
    		{
    		return KErrNotFound;
    		}
    	else 
    		{
    		const TPtrC restTones( iStringNotSent->Des() );
    		SendDtmfToneString( restTones );
    		}
    		
    	}
    else
    	{
    	RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::ContinueDtmfStringSending:Cancel") );
    	
    	// Check if there is anything to cancel
    	if ( iStringNotSent == NULL )
    		{
    		return KErrNotFound;
    		}
    	else 
    		{
    		iObserver->HandleDTMFEvent( MCCPDTMFObserver::ECCPDtmfSequenceAbort, KErrNone, NULL );
    		ClearDtmfString();
    		}
    	}
    return KErrNone;
    }
    
// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
void CCPRefDTMFProvider::AddObserverL( const MCCPDTMFObserver& aObserver )
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPDTMFProvider::AddObserverL"),value ) )
    	{
	   	RDebug::Print(_L("[TESTPLUGIN] CCPRefDTMFProvider::AddObserverL, was configured to LEAVE=%d"), value );
    	User::Leave(value);
    	}
    
    // only one observer used at a time, replaces current one
    iObserver = &aObserver;
    }

// -----------------------------------------------------------------------------
// Add observer.
// -----------------------------------------------------------------------------
//
TInt CCPRefDTMFProvider::RemoveObserver(const MCCPDTMFObserver& aObserver )
    {
    TInt value;
    if( iProvider.ConfigurationHandler().IsActionConfigured(_L8("MCCPDTMFProvider::RemoveObserver"),value ) )
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

