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
* Description:    SIMPLE Engine client base class
*
*/




#ifndef simpleclient_H
#define simpleclient_H

#include <e32std.h>
#include "simplecommon.h"
#include "msimpleconnection.h"
#include "msimpleowncallback.h"
#include "msimpleenginerequest.h"

// FORWARD DECLARATION
class CSimpleEngineRequest;
class MSimpleWatcherObserver;
class MSimpleConnectionObserver;

/**
 *  CSimpleClient
 *
 *  SIMPLE Engine client base class.
 *
 *  @lib simpleengine
 *  @since S60 v3.2
 */

class CSimpleClient : public CBase, public MSimpleOwnCallback
    {

public:

    /**
     * Destructor.
     */
    virtual ~CSimpleClient();


// from base class MSimpleOwnCallback


    /**
     * Complete the open client request
     * @since S60 3.2
     * @param aOpId operation id
     * @param aStatus result status
     * @param aReq request
     */
    void virtual Complete( TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq ) = 0;

protected:

    void IncreaseOpId( );

    /**
     * Delete pending requests
     */
    void DeleteRequests();

    /**
     * Search a specifi request
     * @param aOpID operation if to be matched
     * @return request or NULL if a request not found
     */
    CSimpleEngineRequest* SearchRequests( TInt aOpId );

    /**
     * Two-phase constructor
     */
    void BaseConstructL( );

    /**
     * constructor
     * @param aConn connection
     */
    CSimpleClient(
        MSimpleConnection& aConn );

    /**
     * Send a request to engine DLL
     * @param aReq request
     */
    void SendReqL( MSimpleEngineRequest& aReq );

    /**
     * Reason code converter
     * @param aR request that contains a reason code from NOTIFY
     * @return reason code
     */
    TInt ResponseReason(
        MSimpleEngineRequest& aR );

    /**
     * SIP Status accessor
     * Get the status of the last completed request.
     * @return SIP status, 0 if not available
     */
    TUint DoSIPStatus( );

    /**
     * SIP retry-fater value accessor
     * Get the status of the last completed request.
     * @return SIP retry-after value, 0 if not available
     */
    TUint DoRetryAfter( );

    /**
     * SIP Status getter
     * Get the SIP status of the specified request.
     * @param aOpId operation id
     */
    void GetSIPStatus( TInt aOpId );

protected: // data

    /**
     * SIMPLE engine connection
     */
    MSimpleConnection& iConn;

    /**
     * next operation id
     */
    TInt iOpId;

    /**
     * SIP status of last operation completed
     */
    TUint iSipStatus;
    
    /**
     * SIP retry-after header
     */
    TInt iRetryAfterTime;    

    /**
     * Buffer for client requests
     */
    TDblQue<CSimpleEngineRequest> iRequestList;

    /**
     * Last request sent
     */
    MSimpleEngineRequest::TSimpleRequest iRequest;

    /**
     * Pointer to stack variable to detect the deletion of the heap
     * instance. This case takes place if a client
     * calls destructor in callback method.
     * Own.
     */
    TBool*                  iDestroyedPtr;

private:    // data

    /**
     * Lower limit of opid
     */
    TInt iLimitLowerValue;

    /**
     * Upper limit of opid
     */
    TInt iLimitUpperValue;

    };

#endif

// End of File
