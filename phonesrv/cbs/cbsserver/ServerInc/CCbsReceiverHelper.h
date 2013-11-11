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
* Description:  This file contains the header file of the CCbsReceiverHelper class.
*
*/


#ifndef     CCBSRECEIVERHELPER_H
#define     CCBSRECEIVERHELPER_H

// INCLUDES
#include <e32base.h>
#include "CbsCommon.h"

// FORWARD DECLARATIONS
class CCbsRecMessage;
class CCbsServer;
class CCbsDbImp;
class CVwsSessionWrapper;
class CCbsMessage;


//  CLASS DECLARATION 

/**
*   CCbsReceiverHelper gets the messages from the receiver and 
*   sends them to the database. All received messages are
*   sent to CCbsReceiverHelper instance by CCbsRecEtel.
*   This class makes subscription and existence checks
*   to these messages and also implements topic detection
*   feature.
*/
class CCbsReceiverHelper : public CBase
    {
    public:     // New functions
        
        /**
        *   Creates an instance of the class.
        *
        *   @param  aDatabase           Database object.
        *   @return                     New CCbsReceiverHelper instance.
        */  
         static CCbsReceiverHelper* NewL( CCbsDbImp& aDatabase );

        /**
        *   The destructor.
        */            
        ~CCbsReceiverHelper();

        /**
        *   Clears the topic detected counter.
        */    
        void ClearTopicsDetectedCounter();

        /**
        *   Returns the amount of detected topics.
        *
        *   @return                     The amount of detected topics.
        */    
        TUint TopicsDetected() const;

        /**
        *   This function is called when a message is received.
        *
        *   Assumptions;
        *   1.  The message doesn't have it's permanent or read 
        *       flags raised - method leaves with KErrNotSupported 
        *       if this is the case. 
        *   2.  If an index message (root or subindex) is passed as
        *       a parameter, the language indication prefix, if any,
        *       has been removed before HandleReceivedMessageL() is called.
        *        
        *   Assumption (2) holds because language indications are removed
        *   by Receiver module in an instance of CCbsRecDecoder.
        *
        *   Exceptions:
        *   KErrNoMemory                OOM occurred.
        *   KErrNotSupported            aMessage's flags were invalid.
        *   KErrAlreadyExists           aMessage is already stored in 
        *                               the database.
        *    
        *   @param aMessage             Interface to get the information 
        *                               about the message.
        */
        void HandleReceivedMessageL( CCbsMessage& aMessage );

        /**
        *   Checks if aMessage's topic is in the topic list.
        *   If the topic detection is enabled and the aMessage's topic
        *   is not in the list, the topic is added. The method
        *   will then return ETrue. Otherwise EFalse is returned.
        *
        *   @param  aMessage            Message page.
        *   @return                     ETrue, if topic added
        */
        TBool CheckForNewTopicL( const CCbsMessage& aMessage );

        /**
        *   Checks if aMessage's topic is subscribed and
        *   if the message already exists in the database.
        *   
        *   Returns ETrue only if the message does not exist in the
        *   database and the message's topic is subscribed.
        *
        *   @param  aMessage            Message page.
        *   @return                     ETrue, if message can be received
        */
        TBool CheckForSubscriptionAndExistenceL( const CCbsMessage& aMessage );

        /**
        *   Checks if aMessage's language has been subscribed by the user.
        *
        *   ETrue is returned either if the language specified has been 
        *   subscribed, message language is "Other" or the user has
        *   preferred to receive messages of all languages.
        *
        *   @param  aMessage            Message page
        *   @return                     ETrue, if message can be received
        */
        TBool LanguageOfMessageSubscribedL( const CCbsMessage& aMessage );

        /**
        *   Stores the CB topic (aNumber, aName) retrieved from 
        *   SIM into the DB. If a topic of the same number already exists
        *   in DB, does nothing.
        *
        *   @param aNumber              Number of the topic.
        *   @param aName                Name of the topic.        
        */
        void AddSimTopicL( const TUint aNumber, const TDesC& aName );

        /**
        *   Returns a reference to the CCbsDbImp instance.
        *
        *   @return                     Main database object.
        */
        CCbsDbImp& Database() const;
        
        /**
        *   Returns a reference to SIM Topic array
        *
        *   @return                     SIM Topic array.
        */
        CArrayFixFlat<TInt>& SimTopics() const;
        
        /**
        *   Deletes a topic from the local SIM Topic array.
        *
        *   @param aNumber              Number of the topic.        
        */
        void DeleteFromSimTopicCache( const TUint16 aNumber );
        
        /**
        *   Requests a notification to be launched when a message
		*	arrives to a hotmarked topic.
		*
		*   @param aPlayTone            Should the message alert tone be
		*                               played.
        */
        void LaunchMessageSoftNotificationL( const TBool aPlayTone );

    private:

        /**
        *   The constructor.
        *
        *   @param aDatabase            Database object.
        *   @param aServer              Server object.    
        */
        CCbsReceiverHelper( CCbsDbImp& aDatabase );

        /**
           Finalizes the construction.
        */
        void ConstructL();

        /**
        *   Handles detected topic.
        *
        *   @param  iTopicNumber        The number of the topic detected.
        *   @return                     ETrue if the detected topic is added 
        *                               to the topic list. EFalse otherwise.
        */
        TBool HandleTopicDetectedL( const TCbsDbTopicNumber& iTopicNumber );

        /**
        *   Handles index message. Parses topic identities and
        *   stores them into the database.
        *
        *   Leave reasons:
        *   KErrNoMemory                OOM occured.
        *   KErrNotSupported            Index message was of unsupported 
        *                               version.
        *   KErrCorrupt                 Parsing of the index message failed.
        *
        *   @param  aContents           Index message body.
        *   @param  aIsChildSubIndex    ETrue, if child subindex. 
        *                               EFalse, if root index.
        *   @return                     Result code.
        */
        void HandleIndexMessageL( const TDesC& aContents, 
            const TBool aIsChildSubIndex );

        /**
        *   Parses all topic identities from aText which is assumed to be
        *   an index message's content.
        *        
        *   Leave code values:
        *   KErrNoMemory                Storing of topic identities failed 
        *                               because of insufficient memory.
        *   KErrNotSupported            Index message identifies itself as 
        *                               an index of unknown type to the server.
        *   KErrCorrupt                 A problem was encountered while parsing
        *                               the index message content.
        *   KErrAlreadyExists           Index message contained two or more 
        *                               identities which shared the same topic 
        *                               number.
        *
        *   @param aText                Index message body
        */
        void ParseTopicIdentitiesL( const TDesC& aText );

        /**
        *   Requests a notification to be launched about a message that 
        *   requires immediate displaying.
        *
        *   @param  aMessage            Message to be displayed
        */
        void LaunchMessageImmediateDisplay( const TCbsDbMessage& aMessage );
        
        /**
        * Turn lights on
        */
        void TurnLightsOn();
        
        /**
        * Play Cbs tone.
        */
        void PlayCbsTone();

    private:    // Member variables

        // Reference to the database
        CCbsDbImp& iDatabase;                

        // Number of topics detected after startup.
        TUint iTopicsDetected;

        CArrayFixFlat<TInt>* iSimTopics;
        
        // View server session wrapper.
        // Used to launch CBS UI application on arrival of
        // CB messages designated "requiring immediate display".
        CVwsSessionWrapper* iVwsSession;

        // Local variation bits, fetched from CenRep
        TInt iLVBits;
    };

#endif      //  CCBSRECEIVERHELPER_H   
            
// End of File


