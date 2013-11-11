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
* Description:  This class creates the ETEL receiver package subcomponents, handles
*                incoming CBS messages and communicates with the server interface.
*      
*
*/


// INCLUDE FILES

		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
#include <etelmm.h>
#include <mmlist.h> // CMobilePhoneBroadcastIdList
#include <mmtsy_names.h>

#include "CbsServerConstants.h"
#include "CbsServerPanic.h"
#include "CCbsDbImp.H"
#include "CCbsDbImpTopicList.h"
#include "CCbsDbImpSettings.H"
#include "CCbsRecCollector.h"
#include "CCbsRecDecoder.h"
#include "CCbsRecEtel.h"
#include "CCbsRecEtelMonitor.h"
#include "CCbsRecNetworkListener.h"
#include "CCbsRecMessage.h"
#include "CCbsReceiverHelper.h"
#include "CbsUtils.h"

#include "CCbsEtelMessaging.h"
#include "MCbsEtelMessaging.h"
#include "MCbsMcnSubscriptionsProvider.h"
#include "CCbsSetFilterSettingHandler.h"

#include "CCbsRecWcdmaMessage.h"
#include "CCbsMessageFactory.h"
#include "CCbsLivecastHandler.h"
#include "CbsLogger.h"

#include <centralrepository.h>  // for local variation
#include "cbsinternalcrkeys.h"  // for local variation
#include "cbsvariant.hrh"       // for local variation


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsRecEtel::CCbsRecEtel
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
CCbsRecEtel::CCbsRecEtel()
    : iReception( RMobileBroadcastMessaging::EBroadcastAcceptAll ), 
      iCellInfoReceived( EFalse ),
      iReceptionEnabled( EFalse ),
      iLimitedReception( EFalse ),
      iStartTime( 0 ),
      iEndTime( 0 ),
      iHomeZoneReceived( EFalse ),
      iNewSimTopicsAdded( EFalse )
    {
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::ConstructL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::ConstructL()");

    iSubscriptionProviders = new ( ELeave ) 
        CArrayFixFlat< MCbsMcnSubscriptionsProvider* >( 1 );

    iDecoder = CCbsRecDecoder::NewL();
    
    // Establish a session with the ETel server. 
    ConnectToEtelL();

    // Create a listener to monitor network state changes,
    iNetworkListener = CCbsRecNetworkListener::NewL( iGsmPhone );

    // Construct the command handlers
    iFilterSettingHandler = CCbsSetFilterSettingHandler::NewL( *this, *iMessaging );

    // Create the message factory
    iFactory = CCbsMessageFactory::NewL( *this );    
    
    // Message page collector
    iCollector = CCbsRecCollector::NewL( *iFactory );

    // LC message handler
    iLivecastHandler = CCbsLivecastHandler::NewL( *this );

    // Message monitor
    iEtelMonitor = CCbsRecEtelMonitor::NewL( *this, *iMessaging, *iFactory, *iLivecastHandler );    
    
    // Fetch local variation bits from CenRep    
    CRepository* repository = CRepository::NewL( KCRUidCbsVariation );    
    TInt err = repository->Get( KCbsVariationFlags, iLVBits );  
    if ( err )
        {
        iLVBits = 0;
        }  
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::ConstructL(): CenRep error: %d", err );   
    delete repository;

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsRecEtel* CCbsRecEtel::NewL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::NewL()");

    CCbsRecEtel* self = new ( ELeave ) CCbsRecEtel;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();

    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::NewL()");
    return self;
    }
    
// Destructor 
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
CCbsRecEtel::~CCbsRecEtel()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::~CCbsRecEtel()");
    
    // Disable CB reception, ignore result    
    TRAP_IGNORE( SetReceptionStatusL( 
        RMobileBroadcastMessaging::EBroadcastAcceptNone ) );
    
    delete iNetworkListener;
    delete iEtelMonitor;
    delete iFilterSettingHandler;
    delete iLivecastHandler;

    if ( iMessaging )
        {
        iMessaging->Close(); // owned by CCbsEtelFactory
        }        
    delete iMessaging;
    
    delete iCollector;
    delete iDecoder;    
    delete iFactory;
    delete iSubscriptionProviders;

    iCustomPhone.Close();
    iGsmPhone.Close();
    iEtelServer.Close();
        
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::~CCbsRecEtel()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::HandleMessageReceivedL
// Handles a received CBS message page. 
//  
// The message have
// to be decoded, uncompressed and collected only if
// certain conditions hold. These are checked first.
//
// The handling sequence is as follows:    
// 1.  If the reception is off, reject this message.
// 2.  If the user has set a time limit for receiving messages,
//     check if the current time is such that the message
//     should not be received.
// 3.  Check whether message's language has been subscribed.
//     Reject the message if not. If message id a Class 0 message,
//     do not reject, since the msg should be shown.
// 4.  If the topic detection is enabled and this message
//     is of an unknown topic, add the new topic and
//     reject this message (since the new topic cannot
//     be subscribed at this point).
// 5.  If the same message exists in the database, this
//     message is ignored.
// 6.  The decoder decodes the page.
// 7.  If the message is multipaged, it is given to 
//     the message collector.
// 8.  A complete message is forwarded to CCbsReceiverHelper,
//     which either stores the message into the database
//     or handles it as an index message.
//
// This function is called from CCbsRecEtelMonitor::RunL().
//
// Note: Message reception status check may be unnecessary,
//       if the ETel Monitor instance handles the check.
//
// Note: Message subscription check is necessary here.
//       UI client's subscriptions might differ from MCN client's
//       subscriptions.
//
// Note: Language cannot be read from the header if the
//       language information is stored in the content of
//       the message. This is particulary troublesome, if 
//       the message is compressed. 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::HandleMessageReceivedL( 
    CCbsMessage* aMessage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::HandleMessageReceivedL()");
    
    CleanupStack::PushL( aMessage ); // take ownership
       
    // Decode a message. Language indication prefix, if any, is also
    // removed here. A leave occurs if the message is compressed.
    iDecoder->DecodeL( *aMessage );

    // Add network information to message.
    TInt error = AddNetworkInfo( *aMessage );

    // If the message is a cell info, extract the information.
    if ( aMessage->TopicNumber() == KCellInfoTopic )
        {
        if ( KErrNone == error )
            {
            ExtractCellInformation( *aMessage );
            }
        else
            {
            iCellInfoReceived = EFalse;
            }
        }
    else if ( aMessage->TopicNumber() == KHomeZoneTopic )
        {
        if ( KErrNone == error )
            {
            ExtractHomeZoneInformation( *aMessage );
            }
        else
            {
            iHomeZoneReceived = EFalse;
            }
        }

    // Route the message to all MCN client session-objects.
    RouteMessageL( *aMessage );

    // 1. If the reception is off, reject this message.
    // 2. If the time of the day is such that the user has decided
    //    not to receive any messages, reject this message.
    // 3. Check whether message's language has been subscribed
    // 4. Check if the topic of this message is not in the topic list
    //    and the topic detection is enabled.
    // 5. Check if the topic of this message has been subscribed.
    // 6. Check if this message already exists in the database.    
    
    // This variable must be local, since if the common reception status
    // is used, this check can mess it up.
    TBool uiReceptionEnabled( EFalse );
    iInterface->Database().SettingsL().GetReceptionStatus( uiReceptionEnabled );

    if ( !uiReceptionEnabled ||
        ( iInterface->LanguageOfMessageSubscribedL( *aMessage ) == EFalse &&
        aMessage->RequiresImmediateDisplay() == EFalse ) ||
        iInterface->CheckForNewTopicL( *aMessage ) ||
        iInterface->CheckForSubscriptionAndExistenceL( *aMessage ) == EFalse )
        {
        CBSLOGSTRING("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): Rejecting msg, PopAndDestroy().");
        CleanupStack::PopAndDestroy( aMessage );
        }  
    else
        {
        // Check if this message is multipaged and if it is, 
        // give it to the message collector. If it isn't,
        // forward the message to the receiver helper
        if ( aMessage->TotalPages() > 1 )
            {
            CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): Multipaged msg, total pages: %d.", aMessage->TotalPages() );
            
            // Ensure that the network info is available. 
            // If not, reject the message, because info is required to collect
            // pages of a multipaged message.
            if ( KErrNone != error )
                {
                CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): AddNetworkInfo error: %d, PopAndDestroy().", error );
                CleanupStack::PopAndDestroy( aMessage );
                }
            else
                {
                // Ownership of aMessage transferred to iCollector with
                // aMessage left on the cleanup stack.
                CBSLOGSTRING("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): Calling iCollector->CollectL()...");                
                CCbsMessage* completedMsg = iCollector->CollectL( aMessage, ECbsMessageTypeUnspecified );
                CBSLOGSTRING("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): iCollector->CollectL() finished OK.");
                
                if ( completedMsg )
                    {
                    // Message pages combined => Handle like a single-paged msg.
                    // Result code ignored.
                    CleanupStack::PushL( completedMsg );
                    CBSLOGSTRING("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): MULTIPAGED: Calling iInterface->HandleReceivedMessageL()...");
                    iInterface->HandleReceivedMessageL( *completedMsg );
                    CBSLOGSTRING("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): MULTIPAGED: iInterface->HandleReceivedMessageL() finished OK.");
                    CleanupStack::PopAndDestroy( completedMsg );
                    }
                }            
            }
        else
            {
            // single page    
            CBSLOGSTRING("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): SINGLEPAGED: Calling iInterface->HandleReceivedMessageL()...");
            iInterface->HandleReceivedMessageL( *aMessage );
            CBSLOGSTRING("CBSSERVER: CCbsRecEtel::HandleMessageReceivedL(): SINGLEPAGED: iInterface->HandleReceivedMessageL() finished OK.");
            CleanupStack::PopAndDestroy( aMessage );
            }
        }
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::HandleMessageReceivedL()");                                
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::AddNetworkInfo
// This method blocks if no current network info is 
// available and the call to retrieve information blocks.
//  
// Augments aMessage with current network information (i.e., PLMN, LAC
// and Cell ID). Does nothing if the network listener iListener
// is unavailable.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
TInt CCbsRecEtel::AddNetworkInfo( 
    CCbsMessage& aMessage ) const
    {
    TInt result( KErrNone );

    RMobilePhone::TMobilePhoneNetworkInfoV1 info;
    RMobilePhone::TMobilePhoneLocationAreaV1 area;

    result = iNetworkListener->GetCurrentNetworkInfo( info, area );
    
    if ( result == KErrNone )
        {
        aMessage.SetNetworkInfo( info, area );
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::SetInterface
// Sets the interface to which all accepted messages are given.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::SetInterface( 
    CCbsReceiverHelper* aInterface )
    {
    __ASSERT_DEBUG( aInterface != 0, User::Panic( _L( "InterfaceNull" ), 0 ) );
    iInterface = aInterface;
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::AddSubscriptionProviderL
// MCN client sessions register to CCbsRecEtel using this function.
// 
// When topic subscriptions are gathered during execution of
// ApplyStateChangesL(), subscriptions of each registered subscription
// provider are added to the CBMI list that determines which
// topics are received.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::AddSubscriptionProviderL( 
    MCbsMcnSubscriptionsProvider* aProvider )
    {
    __TEST_INVARIANT;
    if ( !aProvider )
        {
        User::Leave( KErrArgument );
        }

    iSubscriptionProviders->AppendL( aProvider );

    __TEST_INVARIANT;
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::RemoveSubscriptionProviderL
// Removes a MCN topic subscription provider.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::RemoveSubscriptionProviderL(
    const MCbsMcnSubscriptionsProvider* aProvider )
    {
    __TEST_INVARIANT;
    if ( !aProvider )
        {
        User::Leave( KErrArgument );
        }

    TInt count( iSubscriptionProviders->Count() );
    for ( TInt i( 0 ); i < count; i++ )
        {
        if ( iSubscriptionProviders->At( i ) == aProvider )
            {
            iSubscriptionProviders->Delete( i );
            ApplyStateChangesL();
            __TEST_INVARIANT;
            return;
            }
        }

#ifdef _DEBUG
    CbsServerPanic( EMcnProviderNotFound );
#endif
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::ApplyStateChangesL
// Retrieves current subscriptions and settings from DB.
// 
// Reception should be enabled if:
// 1) User has enabled it or
// 2) There are MCN clients with outstanding message routing
// requests or
// 3) New topics were found from SIM card and they were added to Topic list
//
// If the reception is enabled, all topics and languages
// are subscribed from ETel. Filtering is done in CbsServer
// on receival of a CB message (see HandleMessageReceivedL()).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
void CCbsRecEtel::ApplyStateChangesL()
    {
	CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::ApplyStateChangesL()");

    // Determine reception status: If any MCN client requests messages or new
    // SIM Topics have just been addded, then reception should be enabled. 
    // Otherwise UI client reception setting is used.
    TUint numberOfMcnSubscriptions( NumberOfMcnSubscriptions() );
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::ApplyStateChangesL(): numberOfMcnSubscriptions: %d", numberOfMcnSubscriptions );

    TBool receptionStatus( EFalse );
    TInt errorCode( KErrNone );
    TRAP( errorCode, iInterface->Database().SettingsL().GetReceptionStatus( receptionStatus ) );
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::ApplyStateChangesL(): Database reception status: %d", receptionStatus );

    TBool receptionEnabled( EFalse );
    
    // Variated feature    
    if ( iLVBits & KCbsLVFlagTopicSubscription )
        {
        receptionEnabled = ( receptionStatus || numberOfMcnSubscriptions > 0 || iNewSimTopicsAdded );
        }
    else
        {
        receptionEnabled = ( receptionStatus || numberOfMcnSubscriptions > 0 );
        }
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::ApplyStateChangesL(): Should reception be enabled: %d", receptionEnabled );

    // Check if the reception status should be changed
    if ( receptionEnabled != iReceptionEnabled )
        {
        CBSLOGSTRING("CBSSERVER: CCbsRecEtel::ApplyStateChangesL(): Changing reception status.");

        if ( !receptionEnabled )
            {
            // Set reception off
            SetReceptionStatusL( RMobileBroadcastMessaging::EBroadcastAcceptNone );
            CBSLOGSTRING("CBSSERVER: CCbsRecEtel::ApplyStateChangesL(): Changing reception status, status set OFF.");
            }
        else
            {
            // Set reception on
            SetReceptionStatusL( RMobileBroadcastMessaging::EBroadcastAcceptAll );
            CBSLOGSTRING("CBSSERVER: CCbsRecEtel::ApplyStateChangesL(): Changing reception status, status set ON.");
            }
        }
	CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::ApplyStateChangesL()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::GetCurrentMessage
// Returns the current information message.
// The information is retrieved from the last cell information or HomeZone
// message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCbsRecEtel::GetCurrentMessage( 
    TDes& aInfoMessage,
    TInt aTopicNumber )
    {
    // Handle the cell info message (topic 50)
    if ( aTopicNumber == KCellInfoTopic && iCellInfoReceived )
        {
        RMobilePhone::TMobilePhoneNetworkInfoV1 networkInfo;
        RMobilePhone::TMobilePhoneLocationAreaV1 area;

        iNetworkListener->GetCurrentNetworkInfo( networkInfo, area );

        if ( iCellInfoLAC == area.iLocationAreaCode &&
             iCellInfoCellId == area.iCellId &&
             iCellInfoPLMN.iCountryCode == networkInfo.iCountryCode &&
             iCellInfoPLMN.iNetworkId == networkInfo.iNetworkId )
            {
            aInfoMessage.Copy( iCurrentCellInfoMessage );
            return KErrNone;
            }
        iCellInfoReceived = EFalse;
        }
    // Handle the HomeZone message (topic 221)
    else if ( aTopicNumber == KHomeZoneTopic && iHomeZoneReceived )
        {
        RMobilePhone::TMobilePhoneNetworkInfoV1 networkInfo;
        RMobilePhone::TMobilePhoneLocationAreaV1 area;

        iNetworkListener->GetCurrentNetworkInfo( networkInfo, area );

        if ( iHomeZoneLAC == area.iLocationAreaCode &&
             iHomeZoneCellId == area.iCellId &&
             iHomeZonePLMN.iCountryCode == networkInfo.iCountryCode &&
             iHomeZonePLMN.iNetworkId == networkInfo.iNetworkId )
            {
            aInfoMessage.Copy( iCurrentHomeZoneMessage );
            return KErrNone;
            }
        iHomeZoneReceived = EFalse;
        }

    return KErrNotFound;            
    }

// ---------------------------------------------------------
// LoadSimTopicsL() 
// Loads topics from SIM, if any exists. Saves them into DB.
// ---------------------------------------------------------
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
TInt CCbsRecEtel::LoadSimTopicsL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::LoadSimTopicsL()");
    
    TInt result( iCustomPhone.StartSimCbTopicBrowsing() );
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::LoadSimTopicsL(): StartSimCbTopicBrowsing() returned: %d.", result);

    if ( result != KErrNone )
        {
        CBSLOGSTRING2("CBSSERVER: <<< CCbsRecEtel::LoadSimTopicsL(), returning %d.", result);
        return result;
        }

    // Retrieve topics one at time from SIM and store them into DB.
    RMmCustomAPI::TSimCbTopic topic;
    TInt error( KErrNone );
    while ( ( error = iCustomPhone.GetNextSimCbTopic( topic ) ) == KErrNone )
        {
        CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::LoadSimTopicsL(): GetNextSimCbTopic() returned KErrNone. (%d)", error );
        TRAPD( result, iInterface->AddSimTopicL( topic.iNumber, topic.iName ) );
        
        // Indicate that at least one new SIM Topic was added to Topic list
        if ( !result )
            {
            // Variated feature
            if ( iLVBits & KCbsLVFlagTopicSubscription )
                {
                iNewSimTopicsAdded = ETrue;
                CBSLOGSTRING("CBSSERVER: CCbsRecEtel::LoadSimTopicsL(), iNewSimTopicsAdded = ETRUE" );
                }            
            }
        }
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::LoadSimTopicsL(), GetNextSimCbTopic error: %d.", error);
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::LoadSimTopicsL(), AddSimTopicL result: %d.", result);
    
    // To prevent ARMV5 compiler warning
    if ( error )
        {
        error = KErrNone;    
        }        

	CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::LoadSimTopicsL(), returning KErrNone.");
    return KErrNone;
    }

// ---------------------------------------------------------
// DeleteSimTopicL
// Delete the SIM topic. If the SIM topic does not exist, 
// ignore, because that's what we actually want.
// ---------------------------------------------------------
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
void CCbsRecEtel::DeleteSimTopicL( const TUint16 aNumber, const TBool aDeleteAll ) 
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::DeleteSimTopicL()");
    
    TInt number( aNumber );

    // All other errors than KErrNotFound are thrown.
    TInt ret( iCustomPhone.DeleteSimCbTopic( number ) );
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::DeleteSimTopicL(): DeleteSimCbTopic() returned: %d.", ret);
    
#ifdef __WINS__
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::DeleteSimTopicL(): DeleteSimCbTopic() returned: %d", ret );
#else
    if ( ret != KErrNotFound )
        {
        CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::DeleteSimTopicL(): DeleteSimCbTopic() returned: %d", ret );
        User::LeaveIfError( ret );
        }

#endif
    // Delete one by one from cache only if we are deleting just one topic from the SIM card
    if ( !aDeleteAll )
        {
        // Delete from local array    
        iInterface->DeleteFromSimTopicCache( aNumber );
        }    
        
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::DeleteSimTopicL()");
    }

// ---------------------------------------------------------
// CCbsRecEtel::DeleteAllSimTopicsL
// Deletes all topics from the SIM card.
// (other items were commented in a header).
// ---------------------------------------------------------
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/

void CCbsRecEtel::DeleteAllSimTopicsL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::DeleteAllSimTopicsL()");
    
    TInt count = iInterface->SimTopics().Count();
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::DeleteAllSimTopicsL(): SIM Topic count: %d", count );    
    
    for ( TInt i( 0 ); i < count; i++ )
        {
        TInt topicNum = iInterface->SimTopics().At( i );
        CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::DeleteAllSimTopicsL(): SIM Topic number: %d", topicNum );    
        DeleteSimTopicL( topicNum, ETrue );
        }
    
    // Delete all topics from the cache
    iInterface->SimTopics().Reset();
            
    CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::DeleteAllSimTopicsL()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::ConnectToEtelL
// Establishes a connection with the EPOC Telephony Server server.
// Also opens the necessary EPOC Telephony Server resources such as RPhone and
// RAdvGsmSmsMessaging sessions.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/

		/*****************************************************
		*	Series 60 Customer / TSY
		*	Needs customer TSY implementation
		*****************************************************/
void CCbsRecEtel::ConnectToEtelL()
    {
    TInt result( KErrNone );

    // Connect to ETel
    User::LeaveIfError( iEtelServer.Connect() );

    // Load Phone TSY module
    result = iEtelServer.LoadPhoneModule( KMmTsyModuleName );
    if ( result != KErrNone )
        {
        User::Leave( result );
        }

    // This function retrieves the total number of phones supported by all 
	// the currently loaded EPOC Telephony Server (TSY) modules.
    TInt phoneCount( 0 );
    User::LeaveIfError( iEtelServer.EnumeratePhones( phoneCount ) );

    // This function retrieves information associated with the specified phone
	RTelServer::TPhoneInfo phoneInfo;	
	while ( phoneCount-- )
		{
        User::LeaveIfError( iEtelServer.GetPhoneInfo( phoneCount, 
            phoneInfo ) );

        if ( phoneInfo.iName == KMmTsyPhoneName )
            {
            phoneCount = 0;
            }
		}

    // Open GSM phone
    result = iGsmPhone.Open( iEtelServer, phoneInfo.iName );    
    if ( result != KErrNone )
        {
        User::Leave( result );
        }

    // Create an EPOC Telephony Server messaging instance
    iMessaging = CCbsEtelMessaging::NewL();

    // Open SMS Messaging
    result = iMessaging->Open( iGsmPhone );
    if ( result != KErrNone )
        {
        User::Leave( result );
        }

    // Opens a custom phone subsession by name
    result = iCustomPhone.Open( iGsmPhone );
    if ( result != KErrNone )
        {
        User::Leave( result );
        }
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::SetReceptionStatusL
// Sets the requested CB reception status aStatus to ME.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
		/*****************************************************
		*	Series 60 Customer / ETel
		*	Series 60  ETel API
		*****************************************************/
void CCbsRecEtel::SetReceptionStatusL(
    RMobileBroadcastMessaging::TMobilePhoneBroadcastFilter aSetting )
    {    
    if ( iEtelMonitor )
        {
        if ( aSetting == RMobileBroadcastMessaging::EBroadcastAcceptNone )
            {            
            iEtelMonitor->Cancel();
            }
        else
            {
            iEtelMonitor->IssueRequest();
            }
        }

    if ( iMessaging )
        {        
        iFilterSettingHandler->SetFilterSetting( aSetting );
        }

    iReceptionEnabled = ( aSetting == RMobileBroadcastMessaging::EBroadcastAcceptAll );
    
    // Variated feature
    if ( iLVBits & KCbsLVFlagTopicSubscription )
        {
        // Set reception setting on also for UI, 
        // if new topics were added from the SIM card
        if ( iNewSimTopicsAdded )
            {
            iInterface->Database().SettingsL().SetReceptionStatusL( ETrue );
            iNewSimTopicsAdded = EFalse;
            }
        }    
    }  

// -----------------------------------------------------------------------------
// CCbsRecEtel::NumberOfMcnSubscriptions
// Returns the total number of topic subscriptions made by
// MCN clients registered to the server.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CCbsRecEtel::NumberOfMcnSubscriptions() const
    {
    TUint count( 0 );
    TInt mcnClientCount( iSubscriptionProviders->Count() );
    for ( TInt i( 0 ); i < mcnClientCount; i++ )
        {
        count += iSubscriptionProviders->At( i )->NumberOfSubscriptions();
        }
    return count;
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::RouteMessageL
// Passes the given CB message to each registered subscriptions
// provider  
// a subscription provider is a MCN client wishing
// CB message routing service
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::RouteMessageL( 
    const CCbsMessage& aMessage )
    {
	CBSLOGSTRING("CBSSERVER: >>> CCbsRecEtel::RouteMessageL()");

    TInt count( iSubscriptionProviders->Count() );    
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::RouteMessageL(): Provider count: %d", count);

    for ( TInt i( 0 ); i < count; i++ )
        {
        CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::RouteMessageL(): Routing to provider: %d", i );
        iSubscriptionProviders->At( i )->RouteMessageL( aMessage );
        }

	CBSLOGSTRING("CBSSERVER: <<< CCbsRecEtel::RouteMessageL()");
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::ExtractCellInformation
// Copies Current cell information (district info, MCN)
// into receiver's cache.
// 
// This way MCN clients can be provided the current
// cell information message even without message routing service.
// 
// The given message is assumed to be a valid cell information
// message (CB message of topic 050).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::ExtractCellInformation( 
    const CCbsMessage& aMessage )
    {

    iCurrentCellInfoMessage.Copy( aMessage.Contents() );
    aMessage.GetPLMN( iCellInfoPLMN );
    iCellInfoLAC = aMessage.LAC();
    iCellInfoCellId = aMessage.CellId();

    iCellInfoReceived = ETrue;
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::ExtractHomeZoneInformation
// Copies Current HomeZone information into receiver's cache.
// 
// This way MCN clients can be provided the current HomeZone
// message even without message routing service.
// 
// The given message is assumed to be a valid HomeZone
// message (CB message of topic 221).
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::ExtractHomeZoneInformation( 
    const CCbsMessage& aMessage )
    {
    iCurrentHomeZoneMessage.Copy( aMessage.Contents() );
    aMessage.GetPLMN( iHomeZonePLMN );
    iHomeZoneLAC = aMessage.LAC();
    iHomeZoneCellId = aMessage.CellId();

    iHomeZoneReceived = ETrue;
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::Collector
// Returns the message collector reference.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsRecCollector& CCbsRecEtel::Collector()
    {
    return *iCollector;
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::Interface
// Returns the receiver helper reference.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
CCbsReceiverHelper& CCbsRecEtel::Interface() const
    {
    return *iInterface;
    }

// -----------------------------------------------------------------------------
// CCbsRecEtel::__DbgTestInvariant
// Checks that the object is in a valid state, and panics if it is not.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsRecEtel::__DbgTestInvariant() const
    {
#if defined (_DEBUG)
    if ( iSubscriptionProviders == NULL ||
        iEtelMonitor == NULL || iMessaging == NULL || iInterface == NULL ||
        iDecoder == NULL || iCollector ==NULL || iNetworkListener == NULL )
        {
        User::Invariant();
        }    
#endif // _DEBUG
    }

// ================= OTHER EXPORTED FUNCTIONS ==============
//  End of File
