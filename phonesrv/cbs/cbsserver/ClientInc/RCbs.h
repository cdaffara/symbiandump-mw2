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
* Description:  This file contains the header file of the RCbs class.
*    
*                The client of the CBS Server is required to import this header file
*                into project. The connection to the server is made with Connect()
*                 and closed with Close().
*
*                RCbs provides the interface to CBS clients for accessing the    
*                CBS server. It uses subsession classes RCbsSettings, RCbsTopicList,
*                RCbsTopicMessages and RCbsTopicCollection to actually carry out
*                the required ITC between client and server threads.
*
*
*/



#ifndef     RCBS_H
#define     RCBS_H

// INCLUDES
#include <e32base.h>
#include "CbsCommon.h"
#include "RCbsSettings.h"
#include "RCbsTopicCollection.h"
#include "RCbsTopicList.h"
#include "RCbsTopicMessages.h"

//  CLASS DECLARATION 

/**
*   This represents the client-side session to the cbs server.
*
*   Every client must have an instance of the class to be able to communicate
*   with the server.
*
*   Note that both client and server must use the same versions of the API.
*   Otherwise the server will terminate the client process.
*
*   - The server does not buffer events. This must be taken into
*     account when using notifications.
*   - There can only be one pending notification request per subsession object.
*     The behaviour is undefined, if you try to make several for only one 
*      subsession.
*/
class RCbs 
        : public RSessionBase
    {
    public:     // New functions
        /**
        *   Constructor.
        */
        IMPORT_C RCbs();

        /**
        *   Destructor.
        */
        IMPORT_C ~RCbs();

        /**
        *   Creates connection to the server.
        *
        *   Note that the method must be called before calling any other 
        *   methods. The method returns an error code and, therefore, 
        *   the caller is responsible of checking that everything went just 
        *   fine.
        *
        *   @return                 Error code.
        */
        IMPORT_C TInt Connect();

        /**
        *   Closes the session to the server.
        */
        IMPORT_C void Close();

        /**
        *   Returns the version of CbsClient.
        *
        *   CbsServer and CbsClient must be of same version.
        *
        *   @return                 Returns the version of CbsClient.
        */
        IMPORT_C TVersion Version() const;

        // === Settings-related methods

        /**
        *   Returns the reception status in aStatus, which is ETrue if the reception is
        *   on. Otherwise it is EFalse.
        *
        *   @param  aStatus     The method returns the reception status in this parameter.
        */
        IMPORT_C void GetReceptionStatus( TBool& aStatus );

        /**
        *   Changes the reception status to aStatus.
        *
        *   @param  aStatus     It contains the new reception status.
        *   @return             KErrNone if succeeded.
        */
        IMPORT_C TInt SetReceptionStatus( TBool aStatus );

        /**
        *   Returns the topic detection status in aStatus, which is ETrue if the detection
        *   is on. Otherwise it is EFalse.
        *
        *   @param  aStatus     The method returns the topic detection status in this parameter.
        */
        IMPORT_C void GetTopicDetectionStatus( TBool& aStatus );
        
        /**
        *   Changes the topic detection status to aStatus.
        *
        *   @param  aStatus     It contains the new topic detection status.
        *   @return             KErrNone if succeeded.
        */
        IMPORT_C TInt SetTopicDetectionStatus( TBool aStatus );

        /**
        *   Returns the preferred languages in aLanguages.
        *
        *   @param aLanguages   The method returns the languages in this parameter.
        */
        IMPORT_C void GetLanguages( TCbsSettingsLanguages& aLanguages );

        /**
        *   Changes the preferred languages to aLanguages.
        *
        *   @param aLanguages   It contains the new preferred languages.
        *   @return             KErrNone if succeeded.
        */
        IMPORT_C TInt SetLanguages( const TCbsSettingsLanguages& aLanguages );

        /**
        *   Requests the server to notify the client whenever any settings will be 
        *   changed.
        *
        *   Note that for each subsession only one this kind of request can be pending. Each
        *   client is responsible of assuring this.
        *
        *   @param  aStatus     It is the variable that the server will modify 
        *                       whenever an event occurs.
        *   @param  aEvent      The server will store the type of occurred event 
        *                       to this variable.
        */
        IMPORT_C void NotifySettingsChanged( TRequestStatus& aStatus, 
            TCbsSettingsEvent& aEvent );

        /**
        *   Cancels the request to notify the client.
        */
        IMPORT_C void NotifySettingsChangedCancel();

        //  === Topic Collection-related methods
        
        /**
        *   Resets the iterator. Must be called prior any call to HasNextTopic() 
        *   or NextTopic()!
        */
        IMPORT_C void StartCollectionBrowsing();

        /**
        *   Returns ETrue, if there is a topic not accessed with NextTopic()
        *    
        *   @return     ETrue, if there is a topic. EFalse if the end of the collection
        *               has been reached.
        */
        IMPORT_C TBool HasNextCollectionTopic();                            

        /**
        *   Returns the next topic in the topic collection skipping all topics with 
        *   a topic number matching a topic already listed in the current topic list.
        *   This function will return KErrNotFound if the collection is tried to 
        *   access beyond the end of the list. Otherwise the error code will be 
        *   the same returned by GetTopicInfo().
        *
        *   @param aInfo        The topic information will be stored here.
        *   @return             The error code. KErrNotFound if there are no topics left.
        */
        IMPORT_C TInt NextCollectionTopic( TCbsTopicInfo& aInfo );

        //  === Topic list-related methods

        /**
        *   Returns the total amount of topics the topic list contains.
        *
        *   @param aCount       It will contain the total amount of topics.
        */
        IMPORT_C void GetTopicCount( TInt& aCount );

        /**
        *   Returns information about a topic from the topic list.
        *
        *   Return values:
        *   KErrArgument        Topic was not found.
        *    
        *   Rest of return values indicate a file error.
        *
        *   @param aIndex       It is the index to the topic.
        *   @param aTopic       It will contain the topic information.
        *   @return             Error code.
        */
        IMPORT_C TInt GetTopic( const TInt aIndex, TCbsTopic& aTopic );

        /**
        *   Finds the topic by the given number.
        *
        *   Return values:
        *   KErrNone            Topic returned in parameter aTopic.
        *   KErrNotFound        Topic was not found.
        *
        *   @param aNumber      Number of the topic
        *   @param aTopic       Return: contains the topic information
        *   @return             Error code
        */
        IMPORT_C TInt FindTopicByNumber( TCbsTopicNumber aNumber,
            TCbsTopic& aTopic );

        /** 
        *   Deletes an existing topic.
        *
        *   @param  aNumber         Number of the topic to be deleted
        *   @return                 Error code
        */
        IMPORT_C TInt DeleteTopic( TCbsTopicNumber aNumber );

        /**
        *   Delete all topics.
        *
        *   @return             Error code.
        */
        IMPORT_C TInt DeleteAllTopics();

        /**
        *   Adds a new topic. The handle assigned to the topic will be returned
        *   in aTopic.
        *
        *   Return values:
        *   KErrNone            Topic was successfully added.
        *   KErrAlreadyExists   A topic of the same number already exists in DB
        *   KErrArgument        Topic number given was not in a proper range.
        *   KErrDiskFull        Topic not added - FFS out of space
        *
        *   Note that the number of the new topic must be unused.
        *
        *   @param aTopic       Contains the information of the new topic.
        *                       On return, aTopic also contains the topic 
        *                       handle.
        *   @return             Error code.
        */
        IMPORT_C TInt AddTopic( TCbsTopic& aTopic );

        /**
        *   Changes the name and number of the existing topic.
        *
        *   Note that the changing fails in case there is another topic with
        *   the new topic number. It also fails if the topic is protected.
        *
        *   Return values:
        *   KErrNone            Topic name and number successfully changed.
        *   KErrDiskFull        Topic information not changed - FFS out of space
        *
        *   @param  aOldNumber      Number of the topic to be changed
        *   @param  aNewNumber      Number to be given for the topic
        *   @param  aName           Name to be given for the topic
        *   @return                 Error code
        */  
        IMPORT_C TInt ChangeTopicNameAndNumber( 
            TCbsTopicNumber aOldNumber,
            TCbsTopicNumber aNewNumber, 
            const TCbsTopicName& aName );

        /**
        *   Changes topic subscription status.
        *
        *   @param  aNumber         Number of the topic
        *   @param  aNewStatus      New subscription status
        *   @return                 Error code
        */
        IMPORT_C TInt ChangeTopicSubscriptionStatus( 
            TCbsTopicNumber aNumber, TBool aNewStatus );

        /**
        *   Changes topic hotmark status.
        *
        *   @param  aNumber         Number of the topic
        *   @param  aNewStatus      New hotmark status
        *   @return                 Error code
        */
        IMPORT_C TInt ChangeTopicHotmarkStatus( TCbsTopicNumber aNumber,
            TBool aNewStatus );

        /**
        *   Requests the server to notify the client whenever an event occurs 
        *   that changes the information of the topics. 
        *   
        *   Note that the client may select what kind of events it is 
        *   interested in. Note also that there can be at most one pending
        *   request per instance of the class.
        *
        *   @param aStatus      The variable that the server will modify
        *                       whenever an event occurs.
        *   @param aRequested   Events the client is interested in
        *   @param aEvent       Indicates the variable that will contain the
        *                       type of event that occured.
        *   @param aNumber      Indicates the variable that will contain the
        *                       topic number that was changed in event.
        */
        IMPORT_C void NotifyOnTopicListEvent( 
            TRequestStatus& aStatus, 
            const TInt aRequested, 
            TCbsTopicListEvent& aEvent, 
            TCbsTopicNumber& aNumber );

        /**
        *   Cancels the pending notify request.
        */
        IMPORT_C void NotifyOnTopicListEventCancel();

        /**
        *   Returns the number of topics added since last GetNewTopicsCount()
        *   by the topic detection feature.
        *
        *   @param aCount       It will contain the amount of new topics.
        *   @return TInt Result code.
        */
        IMPORT_C TInt GetNewTopicsCount( TInt& aCount );

        /**
        *   Returns the number of the topic which was last added 
        *   to topic list.
        *
        *   Note: if a topic list cache is maintained by the client
        *   (as CBS UI application does), this function has to 
        *   be called BEFORE calling GetTopicCount() and GetTopic() 
        *   to make sure that no topic is added in between 
        *   GetTopic() and GetLatestTopicHandle() calls. If this
        *   happens, GetLatestTopicHandle() will return a handle
        *   to a topic that is not cached client-side.            
        *        
        *   Return codes:
        *   KErrNone        aNumber is a valid topic number.
        *   KErrNotFound    No topic added since server start, 
        *                   aNumber is not valid.
        *
        *   @param  aNumber         Returns: number of the topic last added
        *   @return                 Result code
        */
        IMPORT_C TInt GetLatestTopicNumber( TCbsTopicNumber& aNumber );

        /**
        *   Returns the total amount of unread messages.
        *
        *   @param  aCount          Return: number of unread messages
        */
        IMPORT_C void GetUnreadMessageCount( TInt& aCount );

        /**
        *   Returns the handle to the latest hotmarked message that has been
        *   received after the system has started up.
        *   
        *   @param  aMessage        Handle to the message
        */
        IMPORT_C void GetHotmarkedMessageHandle( TCbsMessageHandle& aMessage );

        /**
        *   Returns the number of unread messages in hotmarked topics.
        *
        *   This function is to used by the client when deciding whether
        *   the message or topic list view should be opened to display
        *   a hotmarked message(s).
        *
        *   @return                 Number of unread hotmarked messages
        */
        IMPORT_C TInt NumberOfUnreadHotmarkedMessages();

        /**
        *   Returns the numbers of topics that precede and succeed the given 
        *   topic in server-side topic list.
        *
        *   If the given topic is the first topic in list, aPosition has 
        *   ECbsHead bit up. If the given topic is the last topic in list,
        *   aPosition has ECbsTail bit up.
        *    
        *   Return code values:
        *   KErrNone		aPrevTopicNumber, aNextTopicNumber and aPosition 
        *		            contain valid values.
        *   KErrNotFound	aCurrentTopicNumber specified a topic that was not
        *                   on topic list.
        *
        *   @param  aCurrentTopicNumber	    Number that specifies the topic 
        *                                   whose surroundings are returned
        *   @param  aPrevTopicNumber		Returns: number of topic preceding 
        *                                   the given topic
        *   @param  aNextTopicNumber		Returns: number of topic succeeding
        *                                   the given topic
        *   @param  aPosition			    Returns: position of current 
        *                                   topic in list.
        *   @return 				        Result code
        */
        IMPORT_C TInt GetNextAndPrevTopicNumber( 
	        const TCbsTopicNumber& aCurrentTopicNumber,
	        TCbsTopicNumber& aPrevTopicNumber,
	        TCbsTopicNumber& aNextTopicNumber,
	        TInt& aPosition );

        // === Topic Messages-related methods
        
        /**
        *   Returns the total amount of messages the topic contains.    
        *
        *   Return codes:
        *   KErrNotFound            Invalid handle.
        *   KErrNone                aCount contains the number of messages 
        *                           in topic
        *
        *   @param  aNumber         Number of the topic.
        *   @param  aCount          Number of messages in given topic.
        *   @return                 Result code
        */
        IMPORT_C TInt GetMessageCount( TCbsTopicNumber aNumber,
            TInt& aCount );
        
        /**
        *   Returns message information.
        *
        *   Return codes:
        *   KErrNotFound        Topic or message not found.
        *   KErrNone            aMessage contains the message information.
        *
        *   @param  aNumber     Number of the topic
        *   @param  aIndex      Index to the message in topic.
        *   @param  aMessage    Returns: the message information
        *   @return             Error code
        */
        IMPORT_C TInt GetMessage( TCbsTopicNumber aNumber, TInt aIndex,
            TCbsMessage& aMessage );

        /**
        *   Finds a message by given handle.
        *
        *   @param  aHandle     Handle to the message.
        *   @param  aMessage    Return: contains the message information.
        *   @return             Error code.
        */
        IMPORT_C TInt FindMessageByHandle( 
            const TCbsMessageHandle& aHandle, 
            TCbsMessage& aMessage );

        /**
        *   Returns the index of a message with given handle in topic.
        *
        *   Result code KErrNotFound indicates that no message was found with
        *   the given handle.
        *
        *   @param  aHandle     Handle of the message
        *   @param  aIndex      Return: index of the message in message topic
        *   @return             Result code
        */
        IMPORT_C TInt GetMessageIndexByHandle( 
            const TCbsMessageHandle& aHandle, TInt& aIndex );

        /**
        *   Deletes an existing message.
        *
        *   Note that it does not care a lot about the status of the message.
        *   
        *   Please check also the description of LockMessage().
        *
        *   @param  aHandle     It is handle to the message.
        *   @return             Error code.
        */
        IMPORT_C TInt DeleteMessage( const TCbsMessageHandle& aHandle );
        
        /**
        *   Saves a message (the saved message won't be deleted to make 
        *   room for new messages).
        *
        *   Return codes:
        *   KErrNone            Message is saved.
        *   KErrGeneral         Message not saved -- total maximum of saved 
        *                       messages reached.
        *   KErrNotFound        Message not saved -- no message associated
        *                       with the given handle.
        *   KErrDiskFull        Message not saved -- FFS out of space.
        *
        *   @param  aHandle     Handle to the message to be saved.
        *   @return             Return code.
        */
        IMPORT_C TInt SaveMessage( const TCbsMessageHandle& aHandle );

        /**
        *   Locks the message.
        *
        *   Note that a single topic messages subsession can have at most one locked
        *   message. 
        *
        *   Message can be unlocked by trying to lock a null message. Locked message
        *   will also be automatically unlocked when subsession is closed. If a message
        *   is locked, then it will not be deleted from the database. Thus, deleting a 
        *   message or trying to delete a topic that contains such a message will fail.
        *
        *   Locking a message does not prevent to save the message nor read the message.
        *
        *   @param  aHandle     It is handle to the message to be locked.
        *   @return             Error code.
        */
        IMPORT_C TInt LockMessage( const TCbsMessageHandle& aHandle );
        
        /**
        *   Sets the message as read.
        *
        *   @param  aHandle     It is handle to the message to be set read.
        *   @return             Error code.
        */
        IMPORT_C TInt ReadMessage( const TCbsMessageHandle& aHandle );
        
        /**
        *   Returns the message contents.
        *
        *   @param  aHandle     It is handle to the message.
        *   @param  aBuffer     It will contain the contents (as much as it fits).
        *   @return             Error code.
        */
        IMPORT_C TInt GetMessageContents( 
            const TCbsMessageHandle& aHandle, 
            TDes& aBuffer );

        /**
        *   Returns the handles of messages that precede and succeed the 
        *   given message in server-side list of topic messages.
        *
        *   Topic is resolved from the given message handle
        *
        *   If the given handle specifies the first message in topic,
        *   aPosition has ECbsHead bit up. If the given handle specifies 
        *   the last message in topic, aPosition has ECbsTail bit up.
        *    
        *    Return code values:
        *    KErrNone       aPrevMsgHandle, aNextMsgHandle and aPosition
        *                   contain valid values.
        *    KErrNotFound   aCurrentMsgHandle specified a message that was not
        *                   found.
        *
        *    @param  aCurrentMsgHandle      Handle that specifies the message 
        *                                   whose surroundings are returned
        *    @param  aPrevMsgHandle         Returns: handle of message 
        *                                   preceding the given message
        *    @param  aNextMsgHandle         Returns: handle of message 
        *                                   succeeding the given topic
        *    @param  aPosition              Returns: position of current topic 
        *                                   in list
        *    @return 				        Result code
        */
        IMPORT_C TInt GetNextAndPrevMessageHandle(
	        const TCbsMessageHandle& aCurrentMsgHandle,
	        TCbsMessageHandle& aPrevMsgHandle,
	        TCbsMessageHandle& aNextMsgHandle,
	        TInt& aPosition );

        // Other methods

        /**
        *   Returns ETrue if CbsServer session has been established.
        *
        *   @return                     ETrue, if session open.
        */
        IMPORT_C TBool Connected() const;

        /**
        *   Forces the server to shut down.
        *        
        */
        IMPORT_C void Shutdown() const;

    private:    // starting server
        /**
        *   Starts the server.
        */
        TInt StartServer();

        /**
        *   Checks if the server is already started.
        */
        TBool IsServerStarted();

    private:

        // Copy constructor
        RCbs( const RCbs& );

        // Assignment operator
        RCbs& operator=( const RCbs& );
    
    private:
        // Settings subsession
        RCbsSettings iSettings;

        // Topic list subsession
        RCbsTopicList iTopicList;

        // Topic messages subsession
        RCbsTopicMessages iTopicMessages;

        // Collection subsession
        RCbsTopicCollection iTopicCollection;

        // ETrue, if the session has been established
        TBool iConnected;
    };

#endif      //  RCbs_H   
            
// End of File


