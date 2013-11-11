/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of HTTP client engine class.
*
*/

//Symbian headers
#include "posrevgeocodehttpclientengine.h"
#include "posrevgeocodelogger.h"

#include <http.h>
#include <CommDbConnPref.h>
#include <connpref.h>
#include <extendedconnpref.h>
#include <cmgenconnsettings.h>

// Used user agent for requests
_LIT8(KUserAgent, "ReverseGeoCode 1.0");

// This client accepts all content types.
_LIT8(KAccept, "*/*");

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::NewL()
// ----------------------------------------------------------------------------
CPosRevGeoCodeHTTPClientEngine* CPosRevGeoCodeHTTPClientEngine::NewL( 
                                    MPosRevGeoCodeHTTPClientObserver& aObserver )
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::NewL");
    CPosRevGeoCodeHTTPClientEngine* self = new ( ELeave ) 
                                    CPosRevGeoCodeHTTPClientEngine( aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::CPosRevGeoCodeHTTPClientEngine()
// ----------------------------------------------------------------------------
CPosRevGeoCodeHTTPClientEngine::CPosRevGeoCodeHTTPClientEngine(
                                              MPosRevGeoCodeHTTPClientObserver& aObserver ): 
                                              CActive( CActive::EPriorityStandard ),
                                              iObserver( aObserver ),
                                              iConnectionSetupDone( EFalse ),
                                              iPrevProfileId( -1 ),
                                              iMobility(NULL),
                                              iTransactionOpen( EFalse ),
                                              iUri(NULL)
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::CPosRevGeoCodeHTTPClientEngine");
    CActiveScheduler::Add(this);
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::~CPosRevGeoCodeHTTPClientEngine()
// ----------------------------------------------------------------------------
CPosRevGeoCodeHTTPClientEngine::~CPosRevGeoCodeHTTPClientEngine()
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::~CPosRevGeoCodeHTTPClientEngine");
    Cancel();
    
    if( iMobility )
        {
		iMobility->Cancel();
        delete iMobility;
        }

    if ( iTransactionOpen )
        {
        iTransaction.Close();
        }

    
    iSession.Close();
    iConnection.Close();
    iSocketServ.Close();

    delete iUri;

    iCmManager.Close();
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::ConstructL()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::ConstructL()
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::ConstructL");
    iCmManager.OpenL();
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::CloseConnection()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::CloseConnection()
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::CloseConnection");
    if ( iTransactionOpen )
        {
        iTransaction.Close();
        iTransactionOpen = EFalse;
        }
    
    if ( iMobility )
        {
        iMobility->Cancel();
        delete iMobility;
        iMobility = NULL;
        }

    iSession.Close();
    iConnection.Close();
    iSocketServ.Close();  
    iConnectionSetupDone = EFalse;
    }


// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::SetupConnectionL()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::SetupConnectionL()
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::SetupConnectionL");
    if ( iConnectionSetupDone )
        {
        // Connection setup is done
        User::Leave(KErrAlreadyExists);
        }
 
    
    // Open HTTP Session
    iSession.OpenL();
    
    User::LeaveIfError(iSocketServ.Connect());
    User::LeaveIfError(iConnection.Open(iSocketServ));
    
    TConnPrefList prefList;
    TExtendedConnPref prefs;
    prefs.SetSnapPurpose( CMManager::ESnapPurposeInternet );
    prefs.SetForcedRoaming( EFalse );
    prefs.SetBearerSet( TExtendedConnPref::EExtendedConnBearerCellular | 
                        TExtendedConnPref::EExtendedConnBearerWLAN );
    prefList.AppendL( &prefs );
    
    iConnection.Start( prefList,iStatus );
    SetActive();
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::SetHeaderL()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::SetHeaderL( RHTTPHeaders aHeaders, TInt aHdrField, const TDesC8& aHdrValue )
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::SetHeaderL");
    RStringF valStr = iSession.StringPool().OpenFStringL( aHdrValue );
    CleanupClosePushL( valStr );
    THTTPHdrVal val(valStr);
    aHeaders.SetFieldL( iSession.StringPool().StringF( aHdrField, RHTTPSession::GetTable()), val);
    CleanupStack::PopAndDestroy();  // valStr
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::IssueHTTPGetL()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::IssueHTTPGetL( const TDesC8& aUri )
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::IssueHTTPGetL");
    if ( IsActive() )
        {
        return;
        }
    
    iEngineState = EGet;
    
    delete iUri;
    iUri = NULL;
    iUri = aUri.AllocL();
    // Create HTTP connection
    TRAPD( err, SetupConnectionL() );
    //If the Err is KErrNone, It will lead to RunL and
    //hence jump to the DoHTTPGetL() from there.
    
    if( err == KErrAlreadyExists )
        {
        DoHTTPGetL();
        }
    else if( err != KErrNone )
        {
        //If Setup Connection fails
        iObserver.ClientEvent( EHttpConnectionFailure );
        User::Leave(err);
        }
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::DoHTTPGetL()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::DoHTTPGetL()
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::DoHTTPGetL");
    // Parse string to URI (as defined in RFC2396)
    TUriParser8 uri;
    uri.Parse( *iUri );
    
    // Get request method string for HTTP GET
    RStringF method = iSession.StringPool().StringF( HTTP::EGET,RHTTPSession::GetTable() );
    CleanupClosePushL( method );
    // Open transaction with previous method and parsed uri. This class will
    // receive transaction events in MHFRunL and MHFRunError.
    iTransaction = iSession.OpenTransactionL( uri, *this, method );
    iTransactionOpen = ETrue;
    
    // Set headers for request; user agent and accepted content type
    RHTTPHeaders hdr = iTransaction.Request().GetHeaderCollection();
    SetHeaderL( hdr, HTTP::EUserAgent, KUserAgent );
    SetHeaderL( hdr, HTTP::EAccept, KAccept );
    
    // Submit the transaction. After this the framework will give transaction
    // events via MHFRunL and MHFRunError.
    iTransaction.SubmitL();
    CleanupStack::PopAndDestroy( 1 ); // method
    iObserver.ClientEvent( EHttpConnecting );
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::CancelTransaction()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::CancelTransaction()
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::CancelTransaction");
    iEngineState = EIdle;
    delete iUri; 
    iUri = NULL;
    
    // Close() also cancels transaction (Cancel() can also be used but
    // resources allocated by transaction must be still freed with Close())
    if( iTransactionOpen )
        {
        iTransaction.Close();
        iTransactionOpen = EFalse;

        iObserver.ClientEvent( EHttpTxCancelled );
        }
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::MHFRunL()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent& aEvent )
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::MHFRunL");
    switch ( aEvent.iStatus )
        {
        case THTTPEvent::EGotResponseHeaders:
            {
            // HTTP response headers have been received. Use
            // aTransaction.Response() to get the response. However, it's not
            // necessary to do anything with the response when this event occurs.
            iObserver.ClientEvent( EHttpHdrReceived );
            break;
            }
        case THTTPEvent::EGotResponseBodyData:
            {
            // Part (or all) of response's body data received. Use
            // aTransaction.Response().Body()->GetNextDataPart() to get the actual
            // body data.
        
            // Get the body data supplier
            MHTTPDataSupplier* body = aTransaction.Response().Body();
            TPtrC8 dataChunk;
        
            // GetNextDataPart() returns ETrue, if the received part is the last
            // one.
            TBool isLast = body->GetNextDataPart(dataChunk);
            iObserver.ClientBodyReceived(dataChunk);
        
            iObserver.ClientEvent( EHttpBytesReceieved );

            // NOTE: isLast may not be ETrue even if last data part received.
            // (e.g. multipart response without content length field)
            // Use EResponseComplete to reliably determine when body is completely
            // received.
            if( isLast )
                {
                iObserver.ClientEvent( EHttpBodyReceieved );
                }
            // Always remember to release the body data.
            body->ReleaseData();
            break;
            }
        case THTTPEvent::EResponseComplete:
            {
            // Indicates that header & body of response is completely received.
            // No further action here needed.

            iObserver.ClientEvent( EHttpTxCompleted );
            break;
            }
        case THTTPEvent::ESucceeded:
            {
            // Indicates that transaction succeeded.
            iObserver.ClientEvent( EHttpTxSuccess );
            // Transaction can be closed now. It's not needed anymore.
            aTransaction.Close();
            iTransactionOpen = EFalse;
            break;
            }
        case THTTPEvent::EFailed:
            {
            // Transaction completed with failure.
            iObserver.ClientEvent( EHttpTxFailed );
            aTransaction.Close();
            iTransactionOpen = EFalse;
            break;
            }
        default:
            // There are more events in THTTPEvent, but they are not usually
            // needed. However, event status smaller than zero should be handled
            // correctly since it's error.
            {
            if ( aEvent.iStatus < 0 )
                {
                iObserver.ClientEvent( EHttpConnectionFailure );
                // Close the transaction on errors
                aTransaction.Close();
                iTransactionOpen = EFalse;
                }
                break;
            }
        }
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::MHFRunError()
// ----------------------------------------------------------------------------
TInt CPosRevGeoCodeHTTPClientEngine::MHFRunError( TInt /*aError*/, 
                                                  RHTTPTransaction /*aTransaction*/,
                                                  const THTTPEvent& /*aEvent*/ )
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::MHFRunError");
    // Just notify about the error and return KErrNone.
    CloseConnection();
    iObserver.ClientEvent(EHttpMhfRunError);
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::PreferredCarrierAvailable()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::PreferredCarrierAvailable( 
                                               TAccessPointInfo /*aOldAPInfo*/,
                                               TAccessPointInfo /*aNewAPInfo*/,
                                               TBool /*aIsUpgrade*/,
                                               TBool aIsSeamless )
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::PreferredCarrierAvailable");
    if( !aIsSeamless && iMobility)
        {
        iMobility->MigrateToPreferredCarrier();
        }
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::NewCarrierActive()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::NewCarrierActive( TAccessPointInfo /*aNewAPInfo*/,
                                                      TBool aIsSeamless )
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::NewCarrierActive");
    if( !aIsSeamless && iMobility)
        {
        iMobility->NewCarrierAccepted();
        }
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::Error()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::Error(TInt /*aError*/)
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::Error");
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::DoCancel()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::DoCancel()
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::DoCancel");
    iConnection.Stop();
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::DoCancel()
// ----------------------------------------------------------------------------
TInt CPosRevGeoCodeHTTPClientEngine::RunError(TInt /*aError*/)
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::RunError");
    // Just notify about the error and return KErrNone.
    CloseConnection();
    iObserver.ClientEvent( EHttpTxFailed );
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// CPosRevGeoCodeHTTPClientEngine::RunL()
// ----------------------------------------------------------------------------
void CPosRevGeoCodeHTTPClientEngine::RunL()
    {
    FUNC("CPosRevGeoCodeHTTPClientEngine::RunL");
    TInt statusCode = iStatus.Int();
    
    if ( statusCode == KErrNone )
        {
        // Connection done ok
        iConnectionSetupDone = ETrue;
        
        RStringPool strPool = iSession.StringPool();
        CleanupClosePushL( strPool );
        // Remove first session properties just in case.
        RHTTPConnectionInfo connInfo = iSession.ConnectionInfo();
        
        // Clear RConnection and Socket Server instances
        connInfo.RemoveProperty(strPool.StringF(HTTP::EHttpSocketServ,RHTTPSession::GetTable()));
        connInfo.RemoveProperty(strPool.StringF(HTTP::EHttpSocketConnection,RHTTPSession::GetTable()));
        
        // Clear the proxy settings
        connInfo.RemoveProperty(strPool.StringF(HTTP::EProxyUsage,RHTTPSession::GetTable()));
        connInfo.RemoveProperty(strPool.StringF(HTTP::EProxyAddress,RHTTPSession::GetTable()));
        
        // RConnection and Socket Server
        connInfo.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketServ, 
                                        RHTTPSession::GetTable()), 
                                THTTPHdrVal (iSocketServ.Handle()) );
        
        TInt connPtr1 = REINTERPRET_CAST(TInt, &iConnection);
        connInfo.SetPropertyL ( strPool.StringF(HTTP::EHttpSocketConnection, 
                                RHTTPSession::GetTable() ), THTTPHdrVal (connPtr1) );    

        // Register for mobility API
        delete iMobility;
        iMobility = NULL;
        iMobility = CActiveCommsMobilityApiExt::NewL( iConnection, *this );
        CleanupStack::Pop( &strPool ); // strPool
        
        // Start selected HTTP action
        switch( iEngineState )
            {
            case EIdle:
                {
                //
                CancelTransaction();
                break;
                }
            case EGet:
               {
               DoHTTPGetL();
               break;
               }
            };
        }
    else
        {
        //handle error
        if ( statusCode == KErrPermissionDenied )
            {
            iObserver.ClientEvent( EHttpAuthFailed );
            }
        else
            {
            //Throw some general Transaction falure error!
            iObserver.ClientEvent( EHttpTxFailed );
            }
        CloseConnection();
        }
    }

// End of file
