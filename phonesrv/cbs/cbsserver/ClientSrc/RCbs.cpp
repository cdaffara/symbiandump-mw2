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
* Description:  This module contains the implementation of RCbs class 
                 member functions.
*
*/


//  INCLUDES
#include "RCbs.h"
#include "CbsCommon.h"
#include "CbsServerConstants.h"
#include <data_caging_path_literals.hrh>

// CONSTANTS 
// Server name
_LIT( KCbsServerExe, "CbsServer.exe");

// How many microseconds to wait for server
const TInt KWaitForServerRetries = 10;
const TInt KWaitForServerTime = 500000;

// ==================== LOCAL FUNCTIONS ====================


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// RCbs::RCbs
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RCbs::RCbs()   //lint -e1926
                        //lint -e1928
    : iConnected( EFalse )    
    {
    }                   //lint +e1926
                        //lint +e1928
    
// Destructor
EXPORT_C RCbs::~RCbs() 
    {
    // Close subsession and session.
    Close();
    }

// -----------------------------------------------------------------------------
// RCbs::Connect
// Creates connection to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::Connect()
    {
    TInt result( KErrNone );

    if ( !iConnected )
        {
        // Start the server if it isn't already started
        StartServer();

        // Create a session to the server.
        result = CreateSession( KCbsServerName,
                                Version(), 
                                KCbsServerDefaultSlots );
    
        if ( result == KErrNone )
            {
            // Create a settings subsession
            result = iSettings.Open( *this );
            }                        
        if ( result == KErrNone )
            {
            // Create a topic list subsession
            result = iTopicList.Open( *this );
            }        
        if ( result == KErrNone )
            {
            // Create a topic messages subsession
            result = iTopicMessages.Open( *this );
            }                        
        if ( result == KErrNone )
            {
            // Create a topic collection subsession     
            result = iTopicCollection.Open( *this );            
            if ( result == KErrNone )
                {
                iConnected = ETrue;
                }
            else
                {
                iConnected = EFalse;
                }
            }        
        }
    // Return the result code           
    return result; 
    }

// -----------------------------------------------------------------------------
// RCbs::Close
// Closes the session to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RCbs::Close()
    {
    if ( iConnected )
        {
        // First close all subsessions.
        iTopicCollection.Close();
        iTopicMessages.Close();
        iTopicList.Close();
        iSettings.Close();

        // Then close the server session.

        // Try to tell the server we've closed the session.
        // Just ignore if that fails.
        const TIpcArgs args( TIpcArgs::ENothing );
        SendReceive( ECbsCloseSession, args );

        RHandleBase::Close();
        iConnected = EFalse;
        }
    }

// -----------------------------------------------------------------------------
// RCbs::Version
// Returns the version of CbsClient.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C TVersion RCbs::Version() const
    {
    // Create version and return it.
    return TVersion( KCbsServerVersionMajor,
                     KCbsServerVersionMinor,
                     KCbsServerVersionBuild );
    }

// Settings-related methods

// -----------------------------------------------------------------------------
// RCbs::GetReceptionStatus
// Requests the reception status from the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void RCbs::GetReceptionStatus( 
    TBool& aStatus )
    {
    iSettings.GetReceptionStatus( aStatus );
    }

// -----------------------------------------------------------------------------
// RCbs::SetReceptionStatus
// Changes the reception status to aStatus.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::SetReceptionStatus( 
    TBool aStatus )
    {    
    // Change the reception status
    return iSettings.SetReceptionStatus( aStatus );
    }

// -----------------------------------------------------------------------------
// RCbs::GetTopicDetectionStatus
// Requests the current topic detection status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void RCbs::GetTopicDetectionStatus( 
    TBool& aStatus )
    {
    iSettings.GetTopicDetectionStatus( aStatus );
    }

// -----------------------------------------------------------------------------
// RCbs::SetTopicDetectionStatus
// Changes the topic detection status to aStatus.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C TInt RCbs::SetTopicDetectionStatus( 
    TBool aStatus )
    {
    return iSettings.SetTopicDetectionStatus( aStatus );
    }

// -----------------------------------------------------------------------------
// RCbs::GetLanguages
// Requests for the current language settings from the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void RCbs::GetLanguages( 
    TCbsSettingsLanguages& aLanguages )
    {
    iSettings.GetLanguages( aLanguages );
    }
// -----------------------------------------------------------------------------
// RCbs::SetLanguages
// Sets a new set of languages to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::SetLanguages( 
    const TCbsSettingsLanguages& aLanguages )
    {
    return iSettings.SetLanguages( aLanguages );
    }

// -----------------------------------------------------------------------------
// RCbs::NotifySettingsChanged
// Requests the server to notify the client whenever any settings will be 
// changed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void RCbs::NotifySettingsChanged( 
    TRequestStatus& aStatus, 
    TCbsSettingsEvent& aEvent )
    {    
    iSettings.NotifySettingsChanged( aStatus, aEvent );
    }

// -----------------------------------------------------------------------------
// RCbs::NotifySettingsChangedCancel
// Cancels the request to notify the client.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void RCbs::NotifySettingsChangedCancel()
    {
    iSettings.NotifySettingsChangedCancel();
    }

// Topic Collection -related methods

// -----------------------------------------------------------------------------
// RCbs::StartCollectionBrowsing
// Resets the iterator.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void RCbs::StartCollectionBrowsing()
    {
    iTopicCollection.Start();
    }

// -----------------------------------------------------------------------------
// RCbs::HasNextCollectionTopic
// Returns ETrue, if there is a topic not accessed with NextTopic()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool RCbs::HasNextCollectionTopic()
    {
    return iTopicCollection.HasNextTopic();
    }

// -----------------------------------------------------------------------------
// RCbs::NextCollectionTopic
// Returns the next topic identity if one exists.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::NextCollectionTopic( 
    TCbsTopicInfo& aInfo )
    {
    return iTopicCollection.NextTopic( aInfo );
    }

// -----------------------------------------------------------------------------
// RCbs::GetTopicCount
// Returns the total amount of topics the topic list contains. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RCbs::GetTopicCount( 
    TInt& aCount )
    {
    iTopicList.GetTopicCount( aCount );
    }

// -----------------------------------------------------------------------------
// RCbs::GetTopic
// Returns information about a topic from the topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::GetTopic( 
    const TInt aIndex, 
    TCbsTopic& aTopic )
    {
    return iTopicList.GetTopic( aIndex, aTopic ); 
    }

// -----------------------------------------------------------------------------
// RCbs::FindTopicByNumber
// Finds the topic by the given number.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::FindTopicByNumber( 
    TCbsTopicNumber aNumber, 
    TCbsTopic& aTopic )
    {
    return iTopicList.FindTopicByNumber( aNumber, aTopic );
    }

// -----------------------------------------------------------------------------
// RCbs::DeleteTopic
// Deletes an existing topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::DeleteTopic( 
    TCbsTopicNumber aNumber )
    {
    return iTopicList.DeleteTopic( aNumber );
    }

// -----------------------------------------------------------------------------
// RCbs::DeleteAllTopics
// Delete all topics.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::DeleteAllTopics()
    {
    return iTopicList.DeleteAllTopics();
    }

// -----------------------------------------------------------------------------
// RCbs::AddTopic
// Adds a new topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::AddTopic( 
    TCbsTopic& aTopic )
    {
    return iTopicList.AddTopic( aTopic );
    }

// -----------------------------------------------------------------------------
// RCbs::ChangeTopicNameAndNumber
// Changes the name and number of the existing topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::ChangeTopicNameAndNumber( 
    TCbsTopicNumber aOldNumber,
    TCbsTopicNumber aNewNumber, 
    const TCbsTopicName& aName )
    {
    return iTopicList.ChangeTopicNameAndNumber( 
        aOldNumber, aNewNumber, aName );
    }

// -----------------------------------------------------------------------------
// RCbs::ChangeTopicSubscriptionStatus
// Changes topic subscription status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::ChangeTopicSubscriptionStatus( 
    TCbsTopicNumber aNumber,
    TBool aNewStatus )
    {
    return iTopicList.ChangeTopicSubscriptionStatus( aNumber, aNewStatus );
    }

// -----------------------------------------------------------------------------
// RCbs::ChangeTopicHotmarkStatus
// Changes topic hotmark status.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::ChangeTopicHotmarkStatus( 
    TCbsTopicNumber aNumber,
    TBool aNewStatus )
    {
    return iTopicList.ChangeTopicHotmarkStatus( aNumber, aNewStatus );
    }

// -----------------------------------------------------------------------------
// RCbs::NotifyOnTopicListEvent
// Requests the server to notify the client whenever an event occurs 
// that changes the information of the topics.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RCbs::NotifyOnTopicListEvent( 
    TRequestStatus& aStatus, 
    const TInt aRequested, 
    TCbsTopicListEvent& aEvent, 
    TCbsTopicNumber& aNumber )
    {
    iTopicList.NotifyOnEvent( aStatus, aRequested, aEvent, aNumber );
    }

// -----------------------------------------------------------------------------
// RCbs::NotifyOnTopicListEventCancel
// Cancels the pending notify request.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RCbs::NotifyOnTopicListEventCancel()
    {
    iTopicList.NotifyOnEventCancel();
    }

// -----------------------------------------------------------------------------
// RCbs::GetNewTopicsCount
// Returns the number of topics added since last GetNewTopicsCount()
// by the topic detection feature.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::GetNewTopicsCount( 
    TInt& aCount )
    {
    return iTopicList.GetNewTopicsCount( aCount );
    }

// -----------------------------------------------------------------------------
// RCbs::GetLatestTopicNumber
// Returns the number of the topic which was last added 
// to topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::GetLatestTopicNumber( 
    TCbsTopicNumber& aNumber )
    {
    return iTopicList.GetLatestTopicNumber( aNumber );
    }

// -----------------------------------------------------------------------------
// RCbs::GetUnreadMessageCount
// Returns the total amount of unread messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RCbs::GetUnreadMessageCount( 
    TInt& aCount )
    {
    iTopicList.GetUnreadMessageCount( aCount );
    }

// -----------------------------------------------------------------------------
// RCbs::GetHotmarkedMessageHandle
// Returns the handle to the latest hotmarked message that has been
// received after the system has started up.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void RCbs::GetHotmarkedMessageHandle( 
    TCbsMessageHandle& aMessage )
    {
    iTopicList.GetHotmarkedMessageHandle( aMessage );
    }

// -----------------------------------------------------------------------------
// RCbs::GetHotmarkedMessageHandle
// Returns the number of unread messages in hotmarked topics.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::NumberOfUnreadHotmarkedMessages() 
    {
    return iTopicList.NumberOfUnreadHotmarkedMessages();
    }

// -----------------------------------------------------------------------------
// RCbs::GetNextAndPrevTopicNumber
// Returns the numbers of topics that precede and succeed the given 
// topic in server-side topic list.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::GetNextAndPrevTopicNumber( 
        const TCbsTopicNumber& aCurrentTopicNumber,
        TCbsTopicNumber& aPrevTopicNumber,
        TCbsTopicNumber& aNextTopicNumber,
        TInt& aPosition )
    {
    return iTopicList.GetNextAndPrevTopicNumber(
        aCurrentTopicNumber,
        aPrevTopicNumber,
        aNextTopicNumber,
        aPosition);
    }


// Topic Messages-related methods

// -----------------------------------------------------------------------------
// RCbs::GetMessageCount
// Returns the total amount of messages the topic contains. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt RCbs::GetMessageCount( 
    TCbsTopicNumber aNumber,
    TInt& aCount )
    {
    return iTopicMessages.GetMessageCount( aNumber, aCount );
    }

// -----------------------------------------------------------------------------
// RCbs::GetMessage
// Returns message information. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::GetMessage( 
    TCbsTopicNumber aNumber, 
    TInt aIndex, 
    TCbsMessage& aMessage )
    {
    return iTopicMessages.GetMessage( aNumber, aIndex, aMessage );
    }

// -----------------------------------------------------------------------------
// RCbs::FindMessageByHandle
// Finds a message by given handle.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::FindMessageByHandle( 
    const TCbsMessageHandle& aHandle, 
    TCbsMessage& aMessage )
    {
    return iTopicMessages.FindMessageByHandle( aHandle, aMessage );
    }

// -----------------------------------------------------------------------------
// RCbs::GetMessageIndexByHandle
// Returns the index of a message with given handle in topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C TInt RCbs::GetMessageIndexByHandle( 
    const TCbsMessageHandle& aHandle, 
    TInt& aIndex )
    {
    return iTopicMessages.GetMessageIndexByHandle( aHandle, aIndex );
    }

// -----------------------------------------------------------------------------
// RCbs::DeleteMessage
// Deletes an existing message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::DeleteMessage( 
    const TCbsMessageHandle& aHandle )
    {
    return iTopicMessages.DeleteMessage( aHandle );
    }

// -----------------------------------------------------------------------------
// RCbs::SaveMessage
// Saves a message (the saved message won't be deleted to make 
// room for new messages).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::SaveMessage( 
    const TCbsMessageHandle& aHandle )
    {
    return iTopicMessages.SaveMessage( aHandle );
    }

// -----------------------------------------------------------------------------
// RCbs::LockMessage
// Locks the message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::LockMessage( 
    const TCbsMessageHandle& aHandle )
    {
    return iTopicMessages.LockMessage( aHandle );   
    }

// -----------------------------------------------------------------------------
// RCbs::ReadMessage
// Sets the message as read.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::ReadMessage( 
    const TCbsMessageHandle& aHandle )
    {
    return iTopicMessages.ReadMessage( aHandle );
    }

// -----------------------------------------------------------------------------
// RCbs::GetMessageContents
// Returns the message contents.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::GetMessageContents( 
    const TCbsMessageHandle& aHandle, 
    TDes& aBuffer )
    {
    return iTopicMessages.GetMessageContents( aHandle, aBuffer );
    }

// -----------------------------------------------------------------------------
// RCbs::GetNextAndPrevMessageHandle
// Returns the handles of messages that precede and succeed the 
// given message in server-side list of topic messages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TInt RCbs::GetNextAndPrevMessageHandle(
        const TCbsMessageHandle& aCurrentMsgHandle,
        TCbsMessageHandle& aPrevMsgHandle,
        TCbsMessageHandle& aNextMsgHandle,
        TInt& aPosition )
    {
    return iTopicMessages.GetNextAndPrevMessageHandle(
        aCurrentMsgHandle,
        aPrevMsgHandle,
        aNextMsgHandle,
        aPosition );
    }

// -----------------------------------------------------------------------------
// RCbs::Connected
// Returns ETrue if CbsServer session has been established.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C TBool RCbs::Connected() const 
    {
    return iConnected;
    }

// -----------------------------------------------------------------------------
// RCbs::Shutdown
// Forces the server to shut down.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void RCbs::Shutdown() const
    {
    const TIpcArgs args( TIpcArgs::ENothing );
    SendReceive( ECbsShutdown, args );
    }

// -----------------------------------------------------------------------------
// RCbs::StartServer
// Starts the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TInt RCbs::StartServer()
    {	
    TInt errorCode( KErrNone );
    RMutex serverStartMutex;

    if ( !IsServerStarted() )
        {
        TInt mutexExists( serverStartMutex.CreateGlobal( _L("CBSSERVERMTX") ) );
        if ( mutexExists )
            {
            TInt openErr( serverStartMutex.OpenGlobal( _L("CBSSERVERMTX") ) );
            if ( openErr )
                {
                return openErr;
                }
            }
        // Mutex exists, wait until server is finished with it's tasks
        if ( mutexExists == KErrAlreadyExists )
            {
            serverStartMutex.Wait();
            }

        // Create and start a new process.
	    TBuf<1> arguments;
	    RProcess p;

	    #ifdef __WINS__
        errorCode = p.Create( KCbsServerExe, arguments );        
        #else
        // Use the correct path depending on whether using data caging or not
	    // Use path \sys\bin
	    TBuf<64> fullPath;
	    fullPath = KDC_PROGRAMS_DIR;
        fullPath.Append( KCbsServerExe );	    	  
	    errorCode = p.Create( fullPath, arguments );	    
        #endif

        if ( errorCode == KErrNone )
            {
	        p.Resume();
	        p.Close();

            TInt i( 0 );
            for ( i = 0; i < KWaitForServerRetries; i++ )
                {
                if ( IsServerStarted() )
                    {
                    break; 
                    }

                User::After( KWaitForServerTime );
                }            
            }
        // Finished with startup sequence, release the mutex
        if ( mutexExists == KErrAlreadyExists )
            {
            serverStartMutex.Signal();
            }
        serverStartMutex.Close();
        }    
    return errorCode;
    }

// -----------------------------------------------------------------------------
// RCbs::IsServerStarted
// Checks if the server is already started.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
TBool RCbs::IsServerStarted()
    {
	TFindServer findServer( KCbsServerName );
	TFullName name;
	return ( findServer.Next( name ) == KErrNone );
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File
