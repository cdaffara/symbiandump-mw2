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
* Description:  This file contains the header file of the CCbsRecEtel class
*
*/


#ifndef     CCBSRECETEL_H
#define     CCBSRECETEL_H


//  INCLUDES
#include <cmcn.h>
#include "MCbsEtelMessaging.h"

		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
#include <rmmcustomapi.h>       // SIM topics

// CONSTANTS
		/*****************************************************
		*	Series 60 Customer / TSY
		*	Needs customer TSY implementation
		*****************************************************/

// FORWARD DECLARATIONS
class CCbsEtelFactory;
class CCbsRecEtelMonitor;
class CCbsRecCollector;
class CCbsRecDecoder;
class CCbsRecNetworkListener;
class CCbsRecMessage;
class CCbsReceiverHelper;
class MCbsMcnSubscriptionsProvider;
class CCbsSetFilterSettingHandler;
class CCbsRecWcdmaMessage;
class CCbsMessage;
class CCbsMessageFactory;
class CCbsLivecastHandler;


//  CLASS DECLARATION 

/**
*   CCbsRecEtel sets up the receiver subsystem for receiving CBS messages 
*   from the ETEL server.
*
*   This class creates instances of CCbsRecEtelMonitor to communicate with 
*   ETel server, CCbsRecDecoder to decode message pages and 
*   CCbsRecCollector to combine partial messages together. CCbsRecEtel 
*   sends completed messages to the database via an interface 
*   class CCbsReceiverHelper.
*
*   An instance of CCbsRecNetworkListener is created to monitor network
*   state changes and report them to CCbsRecEtel instance.
*
*   CCbsRecEtel is also responsible for controlling the CBMI list entries 
*   in ME defining Message Identifiers and DCSes that are to be received. 
*
*   CCbsRecEtel instance does not store information on what topics and
*   languages to subscribe. Instead, on call to ApplyStateChangesL(), 
*   subscriptions and other CB settings are retrieved from the database 
*   and passed to EPOC Telephony server.
*
*   Thus, after topic subscriptions, languages or reception status have
*   been modified, a call to ApplyStateChangesL() has to be made in order
*   to actually bring the changes into effect.
*
*   ON SUBSCRIPTION OF TOPICS AND LANGUAGES
*
*   Although EPOC Telephony server can be requested to relay messages of certain topics and
*   languages, there is a couple of things to note.
*
*   1. In order to detect topics broadcasted within the current region, all
*   CB messages have to be received. This is done by subscribing all topics
*   and languages.
*
*   2. Since MCN clients may subscribe any number of topics, topic filtering 
*   has to be made to remove messages not requested by the UI client.
*
*   3. All languages have to be subscribed at all times. This is because
*   TSY limits the number of languages and a single language (e.g., English),
*   may be represented by multiple DCS values. In addition, CBS specification
*   tells that any message in unspecified language has to be received.
*   Thus the only possibility here is to ignore EPOC Telephony server provided facilities
*   to filter languages, and implement filtering in CbsServer. 
*   This is done in CCbsReceiverHelper.
*/
class CCbsRecEtel : public CBase
    {
    public:     // New functions
        
        /**
        *   Creates an instance of the class.
        *
        *   @return A new instance of CCbsRecEtel.
        */
        static CCbsRecEtel* NewL();

        /**
        *   Destructor.
        */
        ~CCbsRecEtel();

        /**
        *   Called by the network listener (instance of CCbsRecEtelMonitor)
        *   to indicate that a new message has been received and must be 
        *   handled.
        *
        *   The message page is decoded, cell information (if present) is 
        *   extracted and routed to MCN clients. Pages of multipaged messages 
        *   are then given to the collector. If the message is completed
        *   (all pages collected) or the message is singlepaged, it is
        *   given onwards to CCbsReceiverHelper.
        *
        *   Note that a multipaged message page is routed to MCN clients 
        *   before it is assembled. This is acceptable because maximum
        *   length of a MCN message is specified to be 20 characters.
        *
        *   Ownership of aMessage transferred to CCbsRecEtel.
        *
        *   @param aMessage A pointer to the message page received.
        */
        void HandleMessageReceivedL( CCbsMessage* aMessage );

        /**
        *   Augments aMessage with current network information (i.e., PLMN, LAC
        *   and Cell ID). Does nothing if the network listener iListener
        *   is unavailable.        
        *
        *   @param aMessage Message page.
        */
        TInt AddNetworkInfo( CCbsMessage& aMessage ) const;
            
        /**
        *   Sets the interface to which all accepted messages are given. 
        *   
        *   @param aInterface A pointer to the receiver interface.
        */
        void SetInterface( CCbsReceiverHelper* aInterface );

        /**
        *   Returns the current and maximum number of subscribed topics.
        *
        *   Topic restrictions are imposed by the TSY in use.
        *
        *   @param aTotal               Total amount of subscribed topics.
        *   @param aMax                 Maximum amount of subscribed topics.
        */
        void GetSubscriptionsLimitsL( TInt& aTotal, TInt& aMax );
        
        /**
        *   MCN client sessions register to CCbsRecEtel using this function.
        *
        *   When topic subscriptions are gathered during execution of
        *   ApplyStateChangesL(), subscriptions of each registered subscription
        *   provider are added to the CBMI list that determines which
        *   topics are received.
        *
        *   @param aProvider            Topic subscription providing instance
        */
        void AddSubscriptionProviderL( 
            MCbsMcnSubscriptionsProvider* aProvider );

        /**
        *   Removes a topic subscription provider.
        *
        *   @param aProvider            Topic subscription providing instance
        */
        void RemoveSubscriptionProviderL( 
            const MCbsMcnSubscriptionsProvider* aProvider );

        /**
        *   Retrieves current subscriptions, languages and settings
        *   from DB. This data is augmented by topic subscription
        *   information retrieved from MCN client(s). Resulting
        *   topic subscriptions, languages and settings will then
        *   be requested from EPOC Telephony server.
        *
        */    
        void ApplyStateChangesL();

        /**
        *   Returns the current cell information.
        *
        *   The information is retrieved from the last cell information
        *   message received.
        *
        *   Result codes:
        *   KErrNone                    Operation successful, aCellInfo 
        *                               contains a valid cell info message
        *   KErrNotFound                Cell information hasn't been received
        *                               yet - aCellInfo is invalid.
        *
        *   @param  aInfoMessage        Current info message
        *   @param  aTopicNumber        Number of the topic that 
        *                               the msg belongs to
        *   @return                     Result code
        */
        TInt GetCurrentMessage( TDes& aInfoMessage, TInt aTopicNumber );

        /**
        *   Loads CB topics from SIM and adds them on the topic list.
        *   A topic is not added on the list if the same topic
        *   already exists on the list. This identification is done
        *   by the number of the topic only. 
        *
        *   The name of an added topic will be taken from SIM.
        *   If no name has been associated with the topic, the string
        *   <qtn_cb_litext_sim_topic> will be given to the topic.
        *
        *   Result codes:
        *   KErrNotSupported            TSY does not support SIM topics.
        *
        *   @return                     Result code.
        */
        TInt LoadSimTopicsL();

        /**
        *   Deletes a CB topic matching the given topic number from SIM.
        *
        *   Leaves if the operation is unsuccessful.
        *   
        *   @param aNumber              Number of topic to be deleted from SIM.
        *   @param aDeleteAll           Are we deleting all SIM topics, ETrue/EFalse
        */
        void DeleteSimTopicL( const TUint16 aNumber, const TBool aDeleteAll );
        
        /**
        *   Deletes all topics from the SIM card.
        *
        *   Leaves if the operation is unsuccessful.
        *           
        */
        void DeleteAllSimTopicsL();

        /**
        *   Passes the given CB message to each registered subscriptions
        *   provider (a subscription provider is a MCN client wishing
        *   CB message routing service).
        *
        *   @param  aMessage                CB message
        */
        void RouteMessageL( const CCbsMessage& aMessage );

        /**
        *   Returns the message collector reference.
        *
        *   @return                     Collector reference
        */
        CCbsRecCollector& Collector();
        
        /**
        *   Returns the receiver helper reference.
        *
        *   @return                     Receiver helper reference
        */
        CCbsReceiverHelper& Interface() const;

   private:
        
        /**
        *   Constructor.
        */
        CCbsRecEtel();

        /**
        *   Finalizes the construction.
        */
        void ConstructL();

        /**
        *   Establishes a connection with the EPOC Telephony server.
        *
        *   Also opens the necessary EPOC Telephony server resources such as RMobilePhone and
        *   RMobileBroadcastMessaging sessions.
        */
        void ConnectToEtelL();
        
        /**
        *   Sets the given CB reception mode on ME.
        *
        *   @param aSetting         CB receive mode
        */
        void SetReceptionStatusL( 
            RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter aSetting );

        /**
        *   Returns the total number of topic subscriptions made by
        *   MCN clients registered to the server.
        *
        *   @return                     Number of subscriptions by
        *                               MCN clients.
        */
        TUint NumberOfMcnSubscriptions() const;

        /**
        *   Copies cell information message body into an internal
        *   cache. This way MCN clients can be provided the current
        *   cell information message even without message routing service.
        *
        *   The given message is assumed to be a valid cell information
        *   message (CB message of topic 050).
        *
        *   @param  aMessage                Cell information message
        */
        void ExtractCellInformation( const CCbsMessage& aMessage );

        /**
        *   Copies cell information message body into an internal
        *   cache. This way MCN clients can be provided the current
        *   HomeZone message even without message routing service.
        *
        *   The given message is assumed to be a valid HomeZone
        *   message (CB message of topic 221).
        *
        *   @param  aMessage                Cell information message
        */
        void ExtractHomeZoneInformation( const CCbsMessage& aMessage );

        /**
        *   Checks if the time of the day is such that the user does
        *   not want to receive any messages.
        *
        *   @param  aMessage                CB message
        *   @return                         ETrue if the message should
        *                                   be recived, otherwise EFalse.
        */
        TBool CheckTimeL( const CCbsMessage* aMessage );
                
    private:    // Data

        // Own: Collector, which is responsible for combining multipaged
        // messages.        
        CCbsRecCollector* iCollector;
        
        // Own: responsible for converting 7-bit and 8-bit representations 
        // into UCS-2.
        CCbsRecDecoder* iDecoder;

        // Current reception status. This is a cached version of 
        // a variable stored in ME/SIM.
        RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter iReception;

        // Ref: interface to the server. 
        CCbsReceiverHelper* iInterface;
        
        // Own: SMS messaging object for receiving CBS message pages 
        // from the EPOC Telephony server.
        MCbsEtelMessaging* iMessaging;

        // Own: ETel monitor receives the message pages and forwards them to
        // CCbsRecEtel.
        CCbsRecEtelMonitor* iEtelMonitor;

        // Own: Filter setting handler processes asynchronous 
        // Set Filter Setting command.
        CCbsSetFilterSettingHandler* iFilterSettingHandler;   

        // EPOC Telephony server session.
        RTelServer iEtelServer;

        // Represents the GSM phone used for messaging and network information
        // retrieving.

		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
        RMobilePhone iGsmPhone;

        // Custom API is used for SIM Topic handling.

		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
        RMmCustomAPI iCustomPhone;

        // Own: active object which listens to changes in network state
        // and reports them to CCbsRecEtel instance.
        CCbsRecNetworkListener* iNetworkListener;

        // Own: set of MCN sessions that provide their topic subscriptions and
        // get cell information routing services
        CArrayFixFlat< MCbsMcnSubscriptionsProvider* >* iSubscriptionProviders;

        // ETrue if a cell information message has been received.
        TBool iCellInfoReceived;
        
        // Current reception status. This is a cached version of 
        // a variable stored in ME.       
        TBool iReceptionEnabled;

        // Current cell information message.
        TMcnMessage iCurrentCellInfoMessage;

        // Has the user set a time to limit the reception
        TBool iLimitedReception;

        // The current time
        TTime iNow;
        TDateTime iDateTimeNow;

        // Start and end times of limited reception
        TTime iStartTime;
        TTime iEndTime;

        // Cell info cache variables
        RMobilePhone::TMobilePhoneNetworkInfoV1 iCellInfoPLMN;
        TUint iCellInfoLAC;
        TUint iCellInfoCellId;

        // ETrue if HomeZone message has been received
        TBool iHomeZoneReceived;

        // HomeZone cache variables
        RMobilePhone::TMobilePhoneNetworkInfoV1 iHomeZonePLMN;
        TUint iHomeZoneLAC;
        TUint iHomeZoneCellId;

        // Current HomeZone message
        TMcnMessage iCurrentHomeZoneMessage;

        // Message factory
        CCbsMessageFactory* iFactory;

        // LC message handler
        CCbsLivecastHandler* iLivecastHandler;

        // Local variation bits, fetched from CenRep
        TInt iLVBits;
                
        // Indication of new SIM Topics
        TBool iNewSimTopicsAdded;
        
        __DECLARE_TEST;
    };

#endif      //  CCBSRECETEL_H   
            
// End of File


