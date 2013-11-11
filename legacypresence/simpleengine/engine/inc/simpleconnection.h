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
* Description:    SIMPLE engine core connection
*
*/




#ifndef simpleconnection_H
#define simpleconnection_H

#include <e32std.h>
#include "simplecommon.h"
#include "msimpleconnection.h"
#include "msimpleowncallback.h"

// FORWARD DECLARATION
class CSimpleSipConnection;
class CSimpleEngineRequest;
class MSimpleConnectionObserver;

/**
 *  CSimpleConnection
 *
 *  SIMPLE Engine core connection.
 *
 *  @lib simpleengine
 *  @since S60 3.2
 */
class CSimpleConnection : public CBase, public MSimpleConnection, public MSimpleOwnCallback
    {

public:

    /**
     * Two-phased constructor.
     *
     * @since S60 3.2
     * @param aObserver callback observer
     * @return CSimpleConnection
     */
    static CSimpleConnection* NewL( MSimpleConnectionObserver& aObserver );

    /**
     * Destructor.
     */
    virtual ~CSimpleConnection();

    /**
     * SIP connction accessor from siputils.
     * 
     * @since S60 3.2     
     * @return SIP utils connection
     */
    CSimpleSipConnection* Connection();


// from base class MSimpleConnection

    /**
     * Destroy this entity
     *
     * @since S60 3.2
     */
    void Close();

    /**
     * Login using default SIP settings
     *
     * @since S60 3.2
     * @return operation id
     */
    TInt LoginL();

    /**
     * Login using specified SIP settings
     *
     * @since S60 3.2
     * @param aId SIP profile id
     * @return operation id
     */
    TInt LoginL( TInt aId );

    /**
     * Getter for the connection staus.
     *
     * @since S60 3.2
     * @return connection staus
     */
    TSimpleState ConnectionStatus();
    
    /**
     * Current SIP identity accessor
     * @return SIP identity
     */
    TPtrC8 CurrentSIPIdentityL();

// from base class MSimpleOwnCallback

    /**
     * Complete the open client request
     * @since S60 3.2
     * @param aOpId operation id
     * @param aStatus result status
     */
    void Complete( TInt aOpId, TInt aStatus, MSimpleEngineRequest& aReq );

private:

    /**
     * Count new operation id
     * @since S60 3.2  
     */
    void IncreaseOpId( );

    /**
     * Delete pending requests
     * @since S60 3.2       
     */
    void DeleteRequests();

    /**
     * Two-phase constructor
     */
    void ConstructL( );

    /**
     * constructor
     */
    CSimpleConnection( MSimpleConnectionObserver& aObserver );

    /**
     * State type converter
     *
     * @since S60 3.2     
     * @param aState TSimpleSipState
     * @return MSimpleConnection::TSimpleState
     */
    MSimpleConnection::TSimpleState ConvertState( TSimpleSipState aState );

    /**
     * Set SIP Connection status
     *
     * @since S60 3.2     
     * @param aState connection state
     */
    void SetConnectionStatus( TSimpleState aState );

private:    // data

    /**
     * Engine state
     */
    TSimpleState                    iState;

    /**
     * SIMPLE engine core
     * Own.
     */
    CSimpleSipConnection*           iConnection;

    /**
     * Client API observer for events
     */
    MSimpleConnectionObserver&      iObserver;

    /**
     * next operation id
     */
    TInt                            iOpId;

    /**
     * Buffer for client requests
     */
    TDblQue<CSimpleEngineRequest>   iRequestList;

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
