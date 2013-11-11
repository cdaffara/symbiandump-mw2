/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the header file of the RMcnSubSession class.
*
*                Client-side MCN Subsession. Responsible for specifying topics
*                to route and posting a routing request at CbsServer.
*
*/


#ifndef RMCNSUBSESSION_H
#define RMCNSUBSESSION_H

// INCLUDES
#include "cbsmcncommon.h"

//  FORWARD DECLARATIONS
class CMcnTopicArray;
class RCbsMcnSession;

//  CLASS DECLARATION

/**
*   This class represents a MCN session with CBS server.
*/
class RMcnSubSession 
    : public RSubSessionBase
    {
public:             // New functions
    /**
    *   Constructor.
    */
    RMcnSubSession();

    /**
    *   Destructor.
    */
    ~RMcnSubSession();

    /**
    *   Creates a MCN subsession to CbsServer. 
    *
    *   @param  aMcnSession    Open MCN session
    *   @return                Result code
    */
    TInt Open( RCbsMcnSession& aMcnSession );
    
    /**
    *   Closes the subsession.
    *
    *   @return  Result code
    */
    TInt Close();
    
    /**
    *   Request next subscribed MCN message to be delivered to this session.
    *
    *   The delivered message will be copied into aBuffer.
    *
    *   @param  aStatus             Request status, indicates the result of 
    *                               the operation.
    *   @param  aMcnPckg            MCN message package, 
    *                               includes msg and parameters
    */    
    void ReadMcnMessage( TRequestStatus& aStatus, TPckg<TCbsMcnMessage>& aMcnPckg );     

    /**
    *   Cancels an outstanding request.
    *
    *   @return   Result code from server.
    */
    TInt ReadMcnMessageCancel();

    /**
    *   Subscribes topics given in aArray.
    *
    *   Leave reasons:
    *   KErrNoMemory    Client- or server-side out-of-memory.
    *   Others          Leave occured while accessing EPOC Telephony Sever.
    *                   In this case, the leave reason from EPOC Telephony Sever
    *                   is returned here.
    *
    *   @param  aArray   Subscribed topics.
    */
    void RegisterL( const CMcnTopicArray& aArray );

    /**
    *   Clears topic subscriptions of this session in CbsServer
    *   
    *   Leaves if server reports any error.
    */  
    void ClearSubscriptionsL();

private:            // Prohibited operators and methods
    /// Assignment operator.
    RMcnSubSession& operator=( const RMcnSubSession& );
    
    /// Copy constructor.
    RMcnSubSession( const RMcnSubSession& );

private:            // Data
    /// ETrue, if a connection has been established. 
    TBool iConnected;
    };

#endif      // RMCNSubSession_H
            
// End of File


