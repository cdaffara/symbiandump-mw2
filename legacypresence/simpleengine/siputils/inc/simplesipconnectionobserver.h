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
* Description:    Implementation of MSIPConnectionObserver.
*
*/




#ifndef CSimpleSipConnectionObserver_H
#define CSimpleSipConnectionObserver_H

//  INCLUDES
#include <e32base.h>
#include <sipconnectionobserver.h>

#include "simplecommon.h"


// FORWARD DECLARATIONS
class CSIPRegistrationBinding;
class CSIPClientTransaction;
class MSimpleSipConnCallback;
class CSIPDialogAssocBase;
class CSIPInviteDialogAssoc;
class MSimpleEngineRequest;

#ifdef _DEBUG
#ifdef __LOCAL_MODE
// INTERNAL TEST SUITE
class CSimpleLocalModeUtils;
#endif
#endif


/**
 * Implementation of MSIPConnectionObserver.
 *
 * @lib simplesiputils
 * since S60 3.2
 */
class CSimpleSipConnectionObserver: public CBase,
                                    public MSIPConnectionObserver
    {

public:

    /**
     * Two phase constructore
     * @param aCallback status observer
     */
    static CSimpleSipConnectionObserver* NewL( MSimpleSipConnCallback& aCallback );
    
    /**
     * destructor
     */
    virtual ~CSimpleSipConnectionObserver();    

public: 

    /**
     * SIP state accesor
     *
     * since S60 3.2     
     * @return SIP state
     */    
    TSimpleSipState SIPState();
            
    /**
     * SIP Profile state has changed.
     *
     * since S60 3.2      
     * @param aState indicates the current profile state
     * @param aSipError SIP error code
     */
    void ProfileStateChanged( 
        CSIPConnection::TState aState, TInt aSipError );             

// from base class MSIPConnectionObserver

    /**
     * A SIP request outside a dialog has been received from the network.
     * @param aTransaction SIP server transaction
     * The ownership is transferred.
     */
    void IncomingRequest( CSIPServerTransaction* aTransaction );

    /**
     * A SIP request within a dialog has been received from the network.
     * The client must resolve the actual dialog association to which
     * this request belongs.
     * @param aTransaction SIP server transaction
     * The ownership is transferred.
     * @param aDialog the dialog  that
     *        this transaction belongs to.
     */
    void IncomingRequest(
        CSIPServerTransaction* aTransaction,
        CSIPDialog& aDialog );

    /**
     * A SIP response received from the network.
     * @param aTransaction contains response elements.
     * The ownership is transferred.
     */
    void IncomingResponse( CSIPClientTransaction& aTransaction );

    /**
     * A SIP response that is within a dialog association or creates
     * a dialog association.
     * @param aTransaction contains response elements.
     * The ownership is transferred.
     * @param aDialogAssoc a dialog association
     */
    void IncomingResponse(
        CSIPClientTransaction& aTransaction,
        CSIPDialogAssocBase& aDialogAssoc );

    /**
     * A SIP response that is within a Invite association or creates
     * a dialog association.
     * @param aTransaction contains response elements.
     * The ownership is transferred.
     * @param aDialogAssoc a dialog association
     */
    void IncomingResponse(
        CSIPClientTransaction& aTransaction,
        CSIPInviteDialogAssoc* aDialogAssoc );

    /**
     * A SIP response creating a registration binding or an error response
     * that is related to an refreshed registration binding
     * has been received from the network.
     * @param aTransaction contains response elements.
     * The ownership is transferred.
     * @param aSIPRegistration associated registration
     */
    void IncomingResponse(
        CSIPClientTransaction& aTransaction,
        CSIPRegistrationBinding& aRegistration );

    /**
     * An asynchronous error has occured related to a periodical refresh
     * that relates to a registration.
     * @param aError error code
     * @param aSIPRegistration associated registration
     */
    void ErrorOccured(
        TInt aError,
        CSIPRegistrationBinding& aRegistration );
    /**
     * An asynchronous error has occurred in the stack related
     * to the request indicated by the given transaction.
     * @param aError error code
     * @param aTransaction the failed transaction
     * @param aRegistration the failed registration
     */
    void ErrorOccured(
        TInt aError,
        CSIPClientTransaction& aTransaction,
        CSIPRegistrationBinding& aRegistration );

    /**
     * An asynchronous error has occurred in the stack related to the
     * request indicated by the given transaction.
     * @param aError error code
     * @param aTransaction failed transaction
     */
    void ErrorOccured(
        TInt aError,
        CSIPTransactionBase& aTransaction );

    /**
     * An asynchronous error has occured related to a request within
     * an existing dialog.
     * @param aError error code
     * @param aTransaction the failed transaction
     * @param aDialogAssoc the failed dialog associoation
     */
    void ErrorOccured(
        TInt aError,
        CSIPTransactionBase& aTransaction,
        CSIPDialogAssocBase& aDialogAssoc );

    /**
     * An asynchronous error has occured related to a refresh
     * @param aError error code
     * @param aSIPRefresh original refresh object
     */
    void ErrorOccured(
        TInt aError,
        CSIPRefresh& aRefresh );

    /**
     * An asynchronous error has occured related to a periodical refresh
     * that belongs to SIP dialog association.
     * @param aError error code
     * @param aDialogAssoc SIP dialog association
     */
    void ErrorOccured(
        TInt aError,
        CSIPDialogAssocBase& aDialogAssoc );

    /**
     * SIP stack has completed UAC core INVITE transaction 64*T1 seconds
     * after the reception of the first 2xx response. No more 2xx responses
     * can be received to the issued single INVITE.
     *
     * @param aTransaction a complete UAC core INVITE transaction
     */
    void InviteCompleted( CSIPClientTransaction& aTransaction );

    /**
     * Invite was canceled with the CANCEL
     * @param aTransaction a canceled INVITE UAS transaction
     */
    void InviteCanceled( CSIPServerTransaction& aTransaction );

    /**
     * Connection state has changed.
     * @param aState indicates the current connection state
     */
    void ConnectionStateChanged( CSIPConnection::TState aState );
     

protected:

    /**
     * Two-phase constructor.
     */
    void ConstructL();

    /**
     * Default constructor
     * @param aCallback status observer
     */
    CSimpleSipConnectionObserver( MSimpleSipConnCallback& aCallback );

private:

    /**
     * Handle Server request data content
     */
    void DoHandleSrvrReqContentL(
        const CSIPMessageElements& aElems,
        MSimpleEngineRequest& aR );
    
    /**
     * A SIP request outside a dialog has been received from the network.
     *
     * since S60 5.0
     * @param aTransaction SIP server transaction
     * The ownership is transferred.
     */
    void DoIncomingRequestL( CSIPServerTransaction* aTransaction );
        
    /**
     * A SIP request within a dialog has been received from the network.
     * The client must resolve the actual dialog association to which
     * this request belongs.
     * @param aTransaction SIP server transaction
     * The ownership is transferred.
     * @param aDialog the dialog  that
     *        this transaction belongs to.
     */
    void DoIncomingRequestL(
        CSIPServerTransaction* aTransaction,
        CSIPDialog& aDialog );
        
    /**
     * A SIP response received from the network.
     * @param aTransaction contains response elements.
     * The ownership is transferred.
     */
    void DoIncomingResponse( CSIPClientTransaction& aTransaction );
    
    /**
     * A SIP response that is within a dialog association or creates
     * a dialog association.
     * @param aTransaction contains response elements.
     * The ownership is transferred.
     * @param aDialogAssoc a dialog association
     */
    void DoIncomingResponse(
        CSIPClientTransaction& aTransaction,
        CSIPDialogAssocBase& aDialogAssoc );
            
    /**
     * Convert Sip error code to the S60 error code
     * @param aSip SIP error code
     * @param aRetryAfter wheter retry-after header is available
     * @param aPublishModify wheteer publish modify request
     * @return S60 error code
     */
    TInt ConvertSipCode( TUint aSip, TBool aRetryAfter, TBool aPublishModify ); 
    
    /**
     * Convert Sip publish modify error code to the S60 error code
     * @param aSip SIP error code
     * @return S60 error code
     */
    TInt ConvertSipModifyCode( TUint aSip );    
    
    /**
     * Get retry-after SIP header value
     * @param aElems
     * @return retry-after value
     */
    TUint GetRetryAfter( const CSIPResponseElements* aElems );
    
    /**
     * Connection state has changed.
     *
     * since S60 3.2      
     * @param aState indicates the current connection state
     * @param aSipError SIP error code
     */
    void ConnectionStateChanged2( 
        CSIPConnection::TState aState, TInt aSipError );    
	                       

private: // data

    /**
     * callback observer
     */
    MSimpleSipConnCallback& iCallback;
    
    /**
     * Error code from network
     */
    TInt iError;
    
    
#ifdef _DEBUG
#ifdef __LOCAL_MODE
    /**
     * INTERNAL TEST SUITE
     * Own.
     */
    CSimpleLocalModeUtils* iLocalModeUtils;
#endif
#endif        

#ifdef _DEBUG
    friend class T_CSimpleSipConnectionObserver;
#endif   
    };

#endif
