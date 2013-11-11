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
* Description:  This file contains the header file of the Cmcn class.
*    
*                This header file presents an API which allows a client to
*                subscribe a number of CB topics from CbsServer. The client
*                receives a notification, if a message belonging to a subscribed
*                topic is received.
*
*/



#ifndef CMCN_H
#define CMCN_H

// INCLUDES
#include "e32base.h"
#include <rcbsmcnsession.h>

// CONSTANTS
// Maximum length of a MCN message.
const TInt KCbsMcnMessageMaxLength = 93;

// DATA TYPES  
typedef TBuf< KCbsMcnMessageMaxLength > TMcnMessage;

// FORWARD DECLARATIONS
class MCbsMcnObserver;
class CCbsMcnListener;
class RCbsMcnSession;
class CMcnTopicArray;

// CLASS DECLARATION

/**
*   Client must first connect to the server and then register 
*   a MCbsMcnObserver-derived class to receive notifications
*   when MCN messages are received from the network.
*
*   Registration to CbsServer is not required to retrieve
*   the current cell info message.
*    
*   In the example below, the client has implemented the class MCbsMcnObserver
*   in class CCbsMcnObserver. First, the client establishes a session
*   with CbsServer. The subscribed topics are specified with a CMcnTopicArray
*   instance. 
*
*   Example:
*
*   // Establish the session.
*   CMcn* session = CMcn::NewL();
*   CleanupStack::PushL( session );
*
*   // Specify topics with CMcnTopicArray.
*   CMcnTopicArray* topics = CMcnTopicArray::NewL();
*   topics->AddCbTopicL( 50 );      // District info (cell info)
*   topics->AddCbTopicL( 0 );       // CB index messages
*
*   // Register. Parameter observer is an instance of CCbsMcnObserver.
*   session->RegisterL( observer, topics );
*   delete topics;
*
*   // Get current MCN state.
*   TMcnMessage mcnInfo;
*   TInt result( session->GetCellInfo( mcnInfo ) );
*   if ( result != KErrNone )
*       {
*       // mcnInfo contains the current MCN message.
*       }
*   
*   // Observer's CellInfoChanged() gets called if MCN state changes.
*   // Note that the active scheduler must be running here.
*
*   // Terminate session.
*   session->Unregister( observer );
*   CleanupStack::PopAndDestroy(); // session, gets closed when deleted
*
*/
class CMcn 
    : public CBase
    {
public:             // New functions
    /**
    *   Instantiates new CMcn object and establishes a session with
    *   CBS server. 
    *   
    *   If connecting to CBS server fails, the function leaves with 
    *   the leave code of RSessionBase::CreateSession().
    *
    *   Active scheduler has to be installed prior to calling this function.
    *
    *   @return                     A new instance of CMcn
    */
    IMPORT_C static CMcn* NewL();

    /**
    *   Destructor. Also closes CbsServer session.
    */
    IMPORT_C ~CMcn();

    /**
    *   Registers an observer to CBS server. The observer is notified when
    *   a MCN message is received from the network.
    *   The caller is also required to provide an array containing numbers
    *   of subscribed topics.
    *
    *   Parameter aArray may contain topic number entry duplicates.
    *   Topics may be in any order.
    *
    *   Leave codes indicate an error accessing EPOC Telephony Sever.
    *   In this case, the leave code is the same returned by EPOC Telephony Sever.
    *
    *   @param  aObserver           Observer implemented by the client
    *   @param  aArray              List of relayed topics
    */
    IMPORT_C void RegisterL( MCbsMcnObserver* aObserver, 
        const CMcnTopicArray& aArray );

    /**
    *   Unregisters a previously registered MCN observer clearing
    *   any topic subscriptions of this client in CBS server.
    *
    *   Panics if the observer specified was not registered.
    *
    *   @param  aObserver           Observer implemented by the client
    */
    IMPORT_C void Unregister( MCbsMcnObserver* aObserver );

    /**
    *   Retrieves the current MCN message (Cell Info, topic 050) from CBS 
    *   server. A new MCN message always overwrites the previous one in CBS
    *   server, no buffering is done. Note that this function may be
    *   only used to retrieve cell info messages.
    *
    *   Return codes:
    *   KErrNone        aMcnMessage contains the current, valid MCN message.
    *   KErrNotFound    CbsServer has not yet received a MCN message.
    *                   Content of aMcnMessage must be ignored.
    *
    *   @param  aMcnMessage         Returns the current MCN message.
    *   @return                     Result code
    */
    IMPORT_C TInt GetCellInfo( TDes& aMcnMessage ) const;

private:    // new functions
    /**
    *   Default constructor.
    */
    CMcn();

    /**
    *   2nd-phase constructor
    */
    void ConstructL();    

    /** 
    *   Relays aMcnMessage to the registered observers.
    *   Used by a friend class CCbsMcnListener instance
    *
    *   @param  aMcnMessage         Received CB message from CbsServer    
    */    
    void RelayMessage( TCbsMcnMessage& aMcnMessage );    

private:    // friend classes
    /// Allows use of RelayMessageL().
    friend class CCbsMcnListener;

private:    // prohibited functions and operators 
    /// Copy constructor.
    CMcn( const CMcn& );

    /// Assignment operator
    CMcn& operator=( const CMcn& );

private:  // Data
    /**
    *   Own: Active object responsible for responding to server-initiated
    *   notification indicating that a message has arrived from
    *   the CbsServer.
    */
    CCbsMcnListener* iMcnListener;

    /// Own: Dynamic array containing the registered observers.
    CArrayPtrFlat< MCbsMcnObserver >* iObservers;

    /// CbsServer session object.
    RCbsMcnSession iSession;

public:

    /**
    *   Retrieves the current info message (e.g. HomeZone, topic 221) from CBS 
    *   server. A new info message always overwrites the previous one in CBS
    *   server, no buffering is done. Note that this function may be
    *   only used to retrieve cell info messages.
    *
    *   Return codes:
    *   KErrNone        aInfoMessage contains the current, valid info message.
    *   KErrNotFound    CbsServer has not yet received a info message.
    *                   Content of aInfoMessage must be ignored.
    *
    *   @param  aInfoMessage    Returns the current info message.
    *   @param  aTopicNumber    Number of the topic
    *   @return                 Result code
    */
    IMPORT_C TInt GetInfoMessage( TDes& aInfoMessage, TInt aTopicNumber ) const;
    };

#endif // CMCN_H
            
// End of File


