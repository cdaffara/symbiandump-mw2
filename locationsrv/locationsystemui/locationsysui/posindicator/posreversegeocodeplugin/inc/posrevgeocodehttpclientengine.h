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
* Description: Declaration of HTTP client engine class.
*
*/

#ifndef POSREVGEOCODEHTTPCLIENTENGINE_H
#define POSREVGEOCODEHTTPCLIENTENGINE_H

#include <http/mhttptransactioncallback.h>
#include <http/mhttpauthenticationcallback.h>
#include <es_sock.h>
#include <cmmanager.h>
#include <comms-infras/cs_mobility_apiext.h>



class RHTTPSession;
class RHTTPTransaction;

const TInt KDefaultBufferSize = 256;

/*
 *  Enumration for the different states of HTTP GET request.
 */
enum THttpStatus
    {
    EHttpSessionError =0,
    EHttpExitingApp,
    EHttpConnecting,
    EHttpTxCancelled,
    EHttpHdrReceived,
    EHttpBytesReceieved,
    EHttpBodyReceieved,
    EHttpTxCompleted,
    EHttpTxSuccess,
    EHttpTxFailed,
    EHttpConnectionFailure,
    EHttpUnknownEvent,
    EHttpMhfRunError,
    EHttpAuthNote,
    EHttpAuthFailed,
    EHttpAuthRequired,
    };

enum TMobileRoamingStatus
    {
    EMobileNotRegistered = 0x00,
    EMobileRegHomeNetwork, // home network
    EMobileNationalRoaming,
    EMobileInternationalRoaming
    };

/**
 * MClientObserver
 * CPosRevGeoCodeHTTPClientEngine passes events and responses body data with this interface.
 * An instance of this class must be provided for construction of CPosRevGeoCodeHTTPClientEngine.
 */
class MPosRevGeoCodeHTTPClientObserver
    {
    public:
    /*
    * ClientEvent()
    *
    * Called when event occurs in CClientEngine.
    *
    * @params aEvent Status of the event.
    */
    virtual void ClientEvent( const THttpStatus& aEvent ) = 0;
    
    /*
    * ClientBodyReceived()
    *
    * Called when a part of the HTTP body is received.
    *
    * @param aBodyData Part of the body data received. (e.g. part of
    *         the received HTML page)
    */
    virtual void ClientBodyReceived( const TDesC8& aBodyData ) = 0;
    };


/*
 * This class provides interface to HTTP client APIs
 */
class CPosRevGeoCodeHTTPClientEngine: public CActive,
                                      public MHTTPTransactionCallback,
                                      public MMobilityProtocolResp
    {
    /**
     * Internal Engine state 
     */
    enum TEngineState
        {
        EIdle = 0,
        EGet
        };
public:
    /**
     * Factory method which creates instance of CPosRevGeoCodeHTTPClientEngine object.
     */
    static CPosRevGeoCodeHTTPClientEngine* NewL( MPosRevGeoCodeHTTPClientObserver& iObserver );

    /**
     * Destructor
     */
    ~CPosRevGeoCodeHTTPClientEngine();

    /**
     * Starts a new HTTP GET transaction.
     *
     * @param aUri URI to get request. (e.g. http://host.org")
     */
    void IssueHTTPGetL( const TDesC8& aUri );

    /**
     * Closes currently running transaction and frees resources related to it.
     */
    void CancelTransaction();
    
    /**
     * Closes the connection
     */
    void CloseConnection();
    
private:
    /*
     * Symbian 2nd phase constructor
     */
    void ConstructL();

    /*
     * Constructor
     */
    CPosRevGeoCodeHTTPClientEngine( MPosRevGeoCodeHTTPClientObserver& iObserver );

    /**
     * Sets header value of an HTTP request.
     */
    void SetHeaderL( RHTTPHeaders aHeaders, TInt aHdrField,
                     const TDesC8& aHdrValue );

    /**
     * Sets up the connection
     */
    void SetupConnectionL();

private: //From MHTTPSessionEventCallback

    void MHFRunL( RHTTPTransaction aTransaction, const THTTPEvent& aEvent );

    TInt MHFRunError( TInt aError, RHTTPTransaction aTransaction, const THTTPEvent& aEvent );

private:// From MMobilityProtocolResp
    void PreferredCarrierAvailable( TAccessPointInfo aOldAPInfo,
                                    TAccessPointInfo aNewAPInfo,
                                    TBool aIsUpgrade,
                                    TBool aIsSeamless );
    
    void NewCarrierActive( TAccessPointInfo aNewAPInfo, TBool aIsSeamless );
    
    void Error( TInt aError );

private: // from CActive
    void RunL();
    void DoCancel();
    TInt RunError(TInt aError);

private:
    void DoHTTPGetL();
    
private: // Data members
    RSocketServ                 iSocketServ;
    RConnection                 iConnection;
    TUint32                     iSelectedIap;

    RHTTPSession                iSession;
    RHTTPTransaction            iTransaction;

    MPosRevGeoCodeHTTPClientObserver&            iObserver;  // Used for passing body data and events to UI
    TBool                       iRunning;   // ETrue, if transaction running
    TBool                       iConnectionSetupDone;
    
    TInt                        iPrevProfileId;
    
    CActiveCommsMobilityApiExt* iMobility;
    TBool                       iTransactionOpen;
    TEngineState                 iEngineState;
    HBufC8*                     iUri;
    RCmManager                  iCmManager;
    };
#endif /* POSREVGEOCODEHTTPCLIENTENGINE_H */
