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
* Description:  This file contains the header file of the RCbsMcnSession class.
*
*                Represents a MCN session to CBS Server. 
*                A client can register to the session to be informed of 
*                arrival of a new MCN message. Registering object must
*                implement interface MCbsMcnObserver.
*   
*                This session class is responsible for opening the MCN session
*                and querying the current cell information message from CbsServer.
*                Other MCN requests are processed by the subsession class 
*                RMcnSubSession.
*
*/



#ifndef RCBSMCNSESSION_H
#define RCBSMCNSESSION_H

//  INCLUDES

#include <e32std.h>
#include <e32base.h>    // CArrayPtrFlat
#include <rmcnsubsession.h>

//  FORWARD DECLARATIONS

class MCbsMcnObserver;
class CCbsMcnListener;
class CMcnTopicArray;

//  CLASS DECLARATION

/**
*   This class represents a MCN session to the CBS server.
*/
class RCbsMcnSession 
    : public RSessionBase
    {
public:             // New functions
    /**
    *   Constructor
    */
    RCbsMcnSession();

    /**
    *   Destructor
    */
    ~RCbsMcnSession();

    /**
    *   Retrieves the current MCN message from CBS server.
    *   An arriving MCN message always overwrites the previous one in CBS
    *   server, no buffering is done.
    *
    *   @param  aMcnMessage         Returns the current MCN message.
    *   @return                     Result code from server.
    */
    TInt GetCellInfo( TDes& aMcnMessage ) const;

    /**
    *   Retrieves the current info message from CBS server.
    *   
    *   @param  aMessage        Returns the current info message from the
    *                           specified topic.
    *   @param  aTopicNumber    Number of the topic that the msg belongs to.
    *   @return                 Result code from server.
    */
    TInt GetInfoMessage( TDes& aMessage, const TInt aTopicNumber ) const;

    /**
    *   Request next subscribed MCN message to be delivered to this session.
    *   Passed to the subsession object.
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
    *   Cancels an outstanding request. Passed to the subsession object.
    *
    *   @return                     Result code from server.
    */
    TInt ReadMcnMessageCancel();

    /**
    *   Subscribes topics given in aArray. Passed to the subsession object.
    *
    *   For leave reasons, see RMcnSubSession.
    *
    *   @param  aArray              Subscribed topics.
    */
    void RegisterL( const CMcnTopicArray& aArray );

    /**
    *   Clears all topic subscriptions of this session in CbsServer.
    *
    *   Leaves if server reports an error.
    */
    void ClearSubscriptionsL();

public:             // From RSessionBase
    /**
    *   Establishes a connection with CbsServer.
    *
    *   @return                     Result code indicating the success of 
    *                               operation.
    */
    TInt Connect();

    /**
    *   Closes the connection with CbsServer.
    */
    void Close();

    /**
    *   Returns version information of this dll.
    */
    TVersion Version() const;

private:            // Prohibited operators and methods
    /// Assignment operator.
    RCbsMcnSession& operator=( const RCbsMcnSession& );

    /// Copy constructor.
    RCbsMcnSession( const RCbsMcnSession& );

private:            // Data
    /// Represents MCN subsession
    RMcnSubSession iSubSession;

    /// ETrue, if a connection has been established. 
    TBool iConnected;

    };

#endif      // __RCBSMCNSESSION_H
            
// End of File


