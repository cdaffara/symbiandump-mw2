/*
* Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CCbsReceiverHelper class
*                member functions.
*
*                CCbsReceiverHelper gets the messages from the receiver and
*                sends them to the database. All received messages are
*                sent to CCbsReceiverHelper instance by CCbsRecEtel.
*                This class makes subscription and existence checks
*                to these messages and also implements topic detection feature.
*
*/


// INCLUDE FILES
#include <barsc.h>              // Resource access
#include <barsread.h>           // Resource access

#include <cbsserver.rsg>

#include "CbsCommon.h"
#include "CbsUtils.h"
#include "CbsServerPanic.h"
#include "CCbsServer.h"
#include "CCbsReceiverHelper.h"
#include "CCbsDbImp.H"
#include "CCbsDbImpSettings.H"
#include "CCbsDbImpTopicList.h"
#include "CCbsDbImpTopicMessages.h"
#include "CCbsDbImpTopicCollection.h"
#include "CCbsRecMessage.h"
// <-- QT PHONE START -->
//#ifndef SYMBIAN_ENABLE_SPLIT_HEADERS
//#include <viewcli.h>            // View server access
//#else
//#include <viewclipartner.h>
//#endif
//#include <AknNotifyStd.h>       // ECellBroadcastNotification
//#include <AknSoftNotifier.h>    // Soft Notification API
// <-- QT PHONE END -->
#include <e32property.h>
#include <coreapplicationuisdomainpskeys.h>
// <-- QT PHONE START -->
//#include <NcnListDomainPSKeys.h> 
// <-- QT PHONE END -->

#include <data_caging_path_literals.hrh>
#include "CbsLogger.h"

#include <centralrepository.h>  // for local variation
#include "cbsinternalcrkeys.h"  // for local variation
#include "cbsvariant.hrh"       // for local variation

// CONSTANTS

// <-- QT PHONE START -->
// UID of CBS UI application
//#define KUidCbsUiappDef 0x101F4CD3
//const TUid KUidCbsUiappApp = { KUidCbsUiappDef };
// <-- QT PHONE END -->

// <-- QT PHONE START -->
//const TInt KCbsImmediateMessageIdInt = 313;
//const TUid KCbsImmediateMessageId = { KCbsImmediateMessageIdInt };
// <-- QT PHONE END -->

// <-- QT PHONE START -->
//const TInt KCbsMessageTone  = 2; // See SharedDataKeysVariant.h or NcnListInternalPSKeys.h
// <-- QT PHONE END -->
// DATA TYPES
// CbsUi application view ID's
enum TCbsUiViewIds
    {
    ECbsUiTopicViewId = 1,
    ECbsUiTopicListViewId,
    ECbsUiListAppColumnViewId,
    ECbsUiMsgViewId,            //message view id
    ECbsUiAddFromIndexViewId,
    ECbsUiSettingsViewId
    };

// These values specify a range of accepted topic number values (inclusive).
const TUint KMinTopicNumber = 1;     // 000 is not accepted.
const TUint KMaxTopicNumber = 999;

// LOCAL FUNCTION PROTOTYPES
LOCAL_C void ParseMessageFormatL(
    TLex& aLex, CCbsDbImpTopicCollection& aCollection );
LOCAL_C void ParseIndexElementIntroL(
    TLex& aLex, CCbsDbImpTopicCollection& aCollection );
LOCAL_C void ParseServiceIntroL(
    TLex& aLex, CCbsDbImpTopicCollection& aCollection );
LOCAL_C void ParseMessageIdL(
    TLex& aLex, TUint& aMessageId );
LOCAL_C void ParseSubIndexIntroL(
    TLex& aLex );
LOCAL_C void ParseSubIndexIdL(
    TLex& aLex );
LOCAL_C void ParseNameCharactersCrLfL(
    TLex& aLex, TPtrC& aName );

// ==================== LOCAL FUNCTIONS ====================

// -----------------------------------------------------------------------------
// ParseMessageFormatL
// Parses an index message.
// Returns: None
// -----------------------------------------------------------------------------
//
LOCAL_C void ParseMessageFormatL(
    TLex& aLex,
    CCbsDbImpTopicCollection& aCollection )
    {
    TInt identitiesCount( 0 );
    while ( !aLex.Eos() )
        {
        ParseIndexElementIntroL( aLex, aCollection );
        identitiesCount++;
        }

    if ( identitiesCount == 0 )
        {
        User::Leave( KErrCorrupt );
        }
    }

// -----------------------------------------------------------------------------
// ParseIndexElementIntroL
// Parses part of an index message.
// Returns: None
// -----------------------------------------------------------------------------
//
LOCAL_C void ParseIndexElementIntroL(
    TLex& aLex,
    CCbsDbImpTopicCollection& aCollection )
    {
    TBool hasSubIndexId( EFalse );

    // subindex-id (optional for service-intro)
    if ( aLex.Peek().IsAlpha() )
        {
        hasSubIndexId = ETrue;
        ParseSubIndexIdL( aLex );
        }

    // subindex-intro | service-intro
    // We'll take a peek to discover, which one
    if ( hasSubIndexId && aLex.Peek() == EKeySpace )
        {
        ParseSubIndexIntroL( aLex );
        }
    else
        {
        ParseServiceIntroL( aLex, aCollection );
        }
    }

// -----------------------------------------------------------------------------
// ParseServiceIntroL
// Parses part of an index message.
// Returns: None
// -----------------------------------------------------------------------------
//
LOCAL_C void ParseServiceIntroL(
    TLex& aLex,
    CCbsDbImpTopicCollection& aCollection )
    {
    // We'll store the results here
    TCbsDbTopicIdentity identity;

    // message-id
    TUint messageId;
    ParseMessageIdL( aLex, messageId );
    identity.iNumber = TUint16( messageId );

    // delimeter
    TChar delimeter( aLex.Get() );
    if ( delimeter != ' ' && delimeter != '.' )
        {
        User::Leave( KErrCorrupt );
        }

    // service-name
    TPtrC serviceName;
    ParseNameCharactersCrLfL( aLex, serviceName );
    // drop crlf
    serviceName.Set( serviceName.Left( serviceName.Length()-2 ) );

    // If the delimeter is ' ', store the identity. Otherwise
    // it only refers to a subindex page, so we skip it
    if ( delimeter == ' ' )
        {
        // Copy max. KCbsDbTopicNameLength characters
        identity.iName = serviceName.Left( KCbsDbTopicNameLength );
        // Store the topic identity into the database
        aCollection.AddTopicIdentityL( identity );
        }
    }

// -----------------------------------------------------------------------------
// ParseMessageIdL
// Parses part of an index message.
// Returns: None
// -----------------------------------------------------------------------------
//
LOCAL_C void ParseMessageIdL(
    TLex& aLex,
    TUint& aMessageId )
    {
    if ( aLex.Val( aMessageId ) != KErrNone
        || aMessageId < KMinTopicNumber
        || aMessageId > KMaxTopicNumber )
        {
        User::Leave( KErrCorrupt );
        }
    }

// -----------------------------------------------------------------------------
// ParseSubIndexIntroL
// Parses part of an index message.
// subindex-intro = " " subindex-name crlf
// subindex-name  = name-character+
// Returns: None
// -----------------------------------------------------------------------------
//
LOCAL_C void ParseSubIndexIntroL(
    TLex& aLex )
    {
    if ( aLex.Get() != EKeySpace )
        {
        User::Leave( KErrCorrupt );
        }

    // subindex-name crlf
    TPtrC subIndexName;
    ParseNameCharactersCrLfL( aLex, subIndexName );
    }

// -----------------------------------------------------------------------------
// ParseSubIndexIdL
// Parses part of an index message.
// subindex-id = subindex-character+
// Returns: None
// -----------------------------------------------------------------------------
//
LOCAL_C void ParseSubIndexIdL(
    TLex& aLex )
    {
    // check that there is at least one subindex-character
    if ( !aLex.Peek().IsAlpha() )
        {
        User::Leave( KErrCorrupt );
        }

    // subindex-id
    while ( aLex.Peek().IsAlpha() )
        {
        aLex.Inc();
        }
    }

// -----------------------------------------------------------------------------
// ParseNameCharactersCrLfL
// Parses part of an index message.
// name-character+ crlf
// Returns: None
// -----------------------------------------------------------------------------
//
LOCAL_C void ParseNameCharactersCrLfL(
    TLex& aLex,
    TPtrC& aName )
    {
    aLex.Mark();

    // check that we have at least one name-character
    TChar currCharacter( aLex.Get() );
    if( currCharacter == EKeyLineFeed
        || currCharacter == EKeyEnter
        || currCharacter == 0 )
        {
        User::Leave( KErrCorrupt );
        }

    // scan until LF or EOS
    while( currCharacter != EKeyLineFeed && currCharacter != 0 )
        {
        currCharacter = aLex.Get();
        }

    // CR-LF is ok, EOS is not
    if( currCharacter == EKeyLineFeed )
        {
        aName.Set( aLex.MarkedToken() );
        }
    else
        {
        User::Leave( KErrCorrupt );
        }
    }

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::CCbsReceiverHelper
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsReceiverHelper::CCbsReceiverHelper(
    CCbsDbImp& aDatabase )
    : iDatabase( aDatabase )
    {
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::ConstructL()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsReceiverHelper::ConstructL()");

    // <-- QT PHONE START -->
    /*
    iVwsSession = CVwsSessionWrapper::NewL();
    */
    // <-- QT PHONE END-->

    // Array for SIM Topic numbers. This way we know which topics
    // to delete also from the SIM card (when deleting all topics).
    iSimTopics = new ( ELeave ) CArrayFixFlat<TInt>( 1 );
    
    // Fetch local variation bits from CenRep    
    CRepository* repository = CRepository::NewL( KCRUidCbsVariation );    
    TInt err = repository->Get( KCbsVariationFlags, iLVBits );
    if ( err )
        {
        iLVBits = 0;
        }  
    CBSLOGSTRING2("CBSSERVER: CCbsRecEtel::ConstructL(): CenRep error: %d", err );
    delete repository;

    CBSLOGSTRING("CBSSERVER: <<< CCbsReceiverHelper::ConstructL()");
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CCbsReceiverHelper* CCbsReceiverHelper::NewL(
    CCbsDbImp& aDatabase )
    {
    // Normal two phase construction.
    CCbsReceiverHelper* self = new ( ELeave ) CCbsReceiverHelper( aDatabase );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// Destructor
CCbsReceiverHelper::~CCbsReceiverHelper()
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsReceiverHelper::~CCbsReceiverHelper()");

    delete iSimTopics;
    // <-- QT PHONE START -->
    /*
    delete iVwsSession;
    */
    // <-- QT PHONE END-->

    CBSLOGSTRING("CBSSERVER: <<< CCbsReceiverHelper::~CCbsReceiverHelper()");
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::CCbsReceiverHelper
// Processes a message received by the receiver.
//
// This function is called when a message is received.
//
// Assumptions;
// 1.  The message doesn't have it's permanent or read
//     flags raised - method leaves with KErrNotSupported
//     if this is the case.
// 2.  If an index message (root or subindex) is passed as
//     a parameter, the language indication prefix, if any,
//     has been removed before HandleReceivedMessageL() is called.
//
// Assumption (2) holds because language indications are removed
// by Receiver module in an instance of CCbsRecDecoder.
// Message may not be accepted if it is somehow invalid
// or the reception has been disabled.
// This function also handles the detection of new topics
// and the processing of an index message.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::HandleReceivedMessageL(
    CCbsMessage& aMessage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsReceiverHelper::HandleReceivedMessageL()");

    if ( aMessage.IsIndexMessage() )
        {
        CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): Index msg.");

        // It is assumed that a language indication prefix,
        // if any, has already been removed from the message.
        aMessage.RemoveIndexHeaderL();

        TBool isChildSubindex( aMessage.IsChildSubindex() );

        // Parse the index message
        HandleIndexMessageL( aMessage.Contents(), isChildSubindex );

        // Child subindex messages won't be displayed to the user.
        if ( isChildSubindex )
            {
            return;
            }
        }

    TUint16 topicNumber( aMessage.TopicNumber() );
    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): topic number: %d.", topicNumber );

    TCbsDbTopic topic;
    iDatabase.TopicListL().FindTopicByNumberL( topicNumber, topic );
    CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): iDatabase.TopicListL().FindTopicByNumberL(..) called OK." );

    // If subscribed, handle the message
    if ( topic.iSubscribed )
        {
        CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): Topic subscribed, processing." );

        // Create and initialize message
        TCbsDbMessage message;

        TTime now;
        now.UniversalTime();
        message.iDateTime = now.Int64();
        message.iKey = aMessage.Key();
        message.iLanguage = aMessage.Language();
        message.iLength = aMessage.Contents().Length();
        message.iPermanent = EFalse;
        message.iRead = EFalse;
        TPtrC ptr( aMessage.Contents() );

        iDatabase.TopicMessagesL().AddMessageL( topicNumber, message, ptr );
        CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): Msg added OK." );

        if ( aMessage.RequiresImmediateDisplay() )
            {
            CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): Calling LaunchMessageImmediateDisplay()... " );

            // If the operator indicates that this message must be displayed
            // immediately, request a view switch of CBS UI application.
            LaunchMessageImmediateDisplay( message );

            CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): LaunchMessageImmediateDisplay() called OK." );
            }
        else if ( topic.iHotmarked )
            {
            // The message is of a hotmarked topic => Show a soft notification.
            CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): Calling LaunchMessageSoftNotification()... " );
            LaunchMessageSoftNotificationL( ETrue );
            CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): LaunchMessageSoftNotification() called OK." );
            }
        } // if ( topic.iSubscribed )
    else
        {
        CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::HandleReceivedMessageL(): Topic NOT subscribed, msg not processed." );
        }

    CBSLOGSTRING("CBSSERVER: <<< CCbsReceiverHelper::HandleReceivedMessageL()");
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::CheckForNewTopicL
// Checks if aMessage's topic is in the topic list.
//
// If the topic detection is enabled and the topic of this
// message is not in the topic list, the topic is added
// to the list. The method will then return ETrue.
// Otherwise EFalse is returned.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsReceiverHelper::CheckForNewTopicL(
    const CCbsMessage& aMessage )
    {
    // Check if the topic detection is on and if so, add topic if the topic
    // is a new topic.
    TBool detection;
    TBool result( EFalse );

    iDatabase.SettingsL().GetTopicDetectionStatus( detection );

    if ( detection && HandleTopicDetectedL( aMessage.TopicNumber() ) )
        {
        // If a new topic added, we do not send the message to
        // the database so return ETrue.
        result = ETrue;
        }
    else
        {
        // Topic detection disabled or topic already in the database
        result = EFalse;
        }

    return result;
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::CheckForSubscriptionAndExistenceL
// Checks if the topic of this message is listed and subscribed.
// Returns ETrue only if the message does not exist in the
// database and the message's topic is subscribed.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsReceiverHelper::CheckForSubscriptionAndExistenceL(
    const CCbsMessage& aMessage )
    {
    // Check if the message belongs to some subscribed topic.
    TCbsDbTopic topic;
    TUint16 topicNumber( 0 );

    // Index messages are stored in topic 0.
    if ( !aMessage.IsIndexMessage() )
        {
        topicNumber = aMessage.TopicNumber();
        }

    TRAPD( errorCode, iDatabase.TopicListL().FindTopicByNumberL( topicNumber, topic ) );

    if ( errorCode == KErrNotFound )
        {
        return EFalse;
        }
    User::LeaveIfError( errorCode );

    return topic.iSubscribed;
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::LanguageOfMessageSubscribedL
// Checks if aMessage's language has been subscribed by the user.
// ETrue is returned either if the language specified has been
// subscribed, message language is "Other" or the user has
// prefers to receive messages of all languages.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsReceiverHelper::LanguageOfMessageSubscribedL(
    const CCbsMessage& aMessage )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsReceiverHelper::LanguageOfMessageSubscribedL" );

    TCbsDbLanguages languages;
    iDatabase.SettingsL().GetLanguages( languages );

    if ( aMessage.Language() < 0 || aMessage.Language() >= ECbsCount )
        {
        return EFalse;
        }

    TCbsDbLanguage language( aMessage.Language() );
    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::LanguageOfMessageSubscribedL: language: %d.", language );

    TBool subscribed( languages.iLanguages[ECbsAll] ||
        languages.iLanguages[ECbsOther] && language == ECbsOther ||
        !languages.iLanguages[ECbsOther] && languages.iLanguages[language] );

    CBSLOGSTRING2("CBSSERVER: <<< CCbsReceiverHelper::LanguageOfMessageSubscribedL, returning %d.", subscribed );
    return subscribed;
    }

// ---------------------------------------------------------
// AddSimTopicL()
//
// Adds the given topic (aNumber, aName) into the DB.
// ---------------------------------------------------------
void CCbsReceiverHelper::AddSimTopicL( 
    const TUint aNumber,
    const TDesC& aName )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsReceiverHelper::AddSimTopicL()");

    TCbsDbTopic topic;
    if ( aName == KNullDesC )       // Topic name not given, use "SIM topics".
        {
        // Establish file server session.
        RFs fs;
        User::LeaveIfError( fs.Connect() );
        CleanupClosePushL( fs );

        // Open localized resource file.
        RResourceFile resourceFile;
        CbsUtils::FindAndOpenDefaultResourceFileLC(
            fs, resourceFile ); // on CS

        // Read "SIM topic"-string.
        TResourceReader reader;
        reader.SetBuffer( resourceFile.AllocReadLC(
            R_TEXT_SIM_TOPIC ) ); // on CS

        HBufC* text = reader.ReadHBufCL();
        topic.iName = *text;
        CleanupStack::PopAndDestroy(3);  // fs, resourceFile, readerBuf

        CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::AddSimTopicL(): Topic name read from resources.");
        }
    else // Use the given topic name.
        {
        topic.iName = aName;
        }

    topic.iNumber = TUint16( aNumber );
    topic.iHotmarked = EFalse;
    topic.iProtected = EFalse;
    
    // Variated feature    
    if ( iLVBits & KCbsLVFlagTopicSubscription )
        {
        topic.iSubscribed = ETrue;
        }
    else
        {
        topic.iSubscribed = EFalse;
        }

    // Leaves, if the topic already exists, so we trap that
    // error. All other errors are passed as a normal leave.
    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::AddSimTopicL(): Topic %d from SIM to topic list...", topic.iNumber );
    // Try to add topic to topic list, topic not detected automatically
    TRAPD( err, iDatabase.TopicListL().AddTopicL( topic, EFalse ) );
    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::AddSimTopicL(): AddTopicL() TRAPped error: %d.", err );

    if( err != KErrAlreadyExists && err != KErrArgument )
        {
        CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::AddSimTopicL(): Leave if error != -11 || -6");
        User::LeaveIfError( err );
        }

    // Append to SIM Topic array
    iSimTopics->AppendL( topic.iNumber );
    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::AddSimTopicL(): Topic %d appended to array.", topic.iNumber ); 
    
    // Leave so that caller is informed e.g. if topic already exists
    User::LeaveIfError( err );
    CBSLOGSTRING("CBSSERVER: <<< CCbsReceiverHelper::AddSimTopicL()" );
    
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::SimTopics
// Returns topics currently added from the SIM card.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CArrayFixFlat<TInt>& CCbsReceiverHelper::SimTopics() const
    {
    return *iSimTopics;
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::DeleteFromSimTopicCache
// Deletes the topic number from local SIM Topic array.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::DeleteFromSimTopicCache( const TUint16 aNumber )
    {
    CBSLOGSTRING("CBSSERVER: >>> CCbsReceiverHelper::DeleteFromSimTopicCache()");
    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::DeleteFromSimTopicCache(): Topic number: %d", aNumber );
    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::DeleteFromSimTopicCache(): Topic count in cache (1): %d", iSimTopics->Count() );

    // Check if this topic is a SIM Topic (can be found in the array)
    TKeyArrayFix key( 0, ECmpTUint16 );
    TInt index;
    TUint16 topicNumber( aNumber );
    TInt result( iSimTopics->FindIsq( topicNumber, key, index ) );

    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::DeleteFromSimTopicCache(): FindIsq() result: %d", result );
    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::DeleteFromSimTopicCache(): FindIsq() result, position: %d", index );

    // Delete the topic from the array
    if ( result == KErrNone )
        {
        CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::DeleteFromSimTopicCache(): Topic no. %d found from cache.", aNumber );
        iSimTopics->Delete( index );
        CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::DeleteFromSimTopicCache(): Topic deleted from cache, index: %d", index );
        iSimTopics->Compress();
        CBSLOGSTRING("CBSSERVER: CCbsReceiverHelper::DeleteFromSimTopicCache(): SIM Topic array compressed.");
        }

    CBSLOGSTRING2("CBSSERVER: CCbsReceiverHelper::DeleteFromSimTopicCache(): Topic count in cache (2): %d", iSimTopics->Count() );
    CBSLOGSTRING("CBSSERVER: <<< CCbsReceiverHelper::DeleteFromSimTopicCache()");
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::Database
// Returns a reference to the CCbsDbImp instance.
// If a topic of the same number already exists
// in DB, does nothing.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CCbsDbImp& CCbsReceiverHelper::Database() const
    {
    return iDatabase;
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::TopicsDetected
// Returns the number of detected topics.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CCbsReceiverHelper::TopicsDetected() const
    {
    return iTopicsDetected;
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::ClearTopicsDetectedCounter
// Sets the counter for detected topics to zero.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::ClearTopicsDetectedCounter()
    {
    // Clear the variable indicating the amount of detected topics.
    iTopicsDetected = 0;
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::HandleTopicDetectedL
// Handles detected topic.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CCbsReceiverHelper::HandleTopicDetectedL(
    const TCbsDbTopicNumber& iTopicNumber )
    {
    // Create new topic and initialise it.
    TCbsDbTopic topic;
    topic.iNumber = iTopicNumber;
    topic.iName = KNullDesC;
    topic.iProtected = EFalse;
    topic.iSubscribed = EFalse;
    topic.iHotmarked = EFalse;

    TBool result( ETrue );

    // Try to add the topic. If succeeded,
    // increase counter iTopicsDetected.
    TRAPD( error, iDatabase.TopicListL().AddTopicL( topic, ETrue ) );

    if ( error == KErrAlreadyExists )
        {
        result = EFalse;
        }
    else if ( error != KErrNone && error != KErrAlreadyExists )
        {
        User::Leave( error );
        }
    else
        {
        // Update the detected topics counter
        iTopicsDetected++;
        }
    return result;
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::HandleIndexMessageL
// Processes an index message and builds a new topic collection
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::HandleIndexMessageL(
    const TDesC& aContents,
    const TBool aIsChildSubIndex )
    {
    // Previous topic identities are cleared if a root index message
    // is received.
    if ( !aIsChildSubIndex )
        {
        iDatabase.TopicCollectionL().Clear();
        }
    // Parses topic identities from the index message.
    // The identities are stored in the current topic collection.
    // If the index message is corrupt, it should still be stored,
    // so trap the error.
    TLex lex( aContents );
    TRAPD( error, ParseMessageFormatL( lex, iDatabase.TopicCollectionL() ) );
    switch ( error )
        {
        case KErrNone:
            // If the message all went fine, apply.
            // The topic identities are written into persistent memory.
            iDatabase.TopicCollectionL().Apply();
            break;

        case KErrCorrupt:
            // Do not react on corrupt messages.
            break;

        default:
            // All other errors will prevent saving the message.
            User::Leave( error );
            break;
        }
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::ParseTopicIdentitiesL
// Parses all topic identities from aText which is assumed to be
// an index message's content.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::ParseTopicIdentitiesL(
        const TDesC& aText )
    {
    // Create a lexer and pass it to the parser
    TLex lex( aText );

    ParseMessageFormatL( lex, iDatabase.TopicCollectionL() );
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::LaunchMessageSoftNotificationL
// Requests to launch a soft notification.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::LaunchMessageSoftNotificationL( const TBool /*aPlayTone*/ )
    {
    // <-- QT PHONE START -->
    /*
    TInt numberOfHotMsgs( 0 );
    numberOfHotMsgs = iDatabase.TopicListL().UnreadHotmarkedMessageCount();

    CAknSoftNotifier* notifier = CAknSoftNotifier::NewLC(); // on CS

    TurnLightsOn();

    if ( aPlayTone )
        {
        PlayCbsTone();
        }

    notifier->SetNotificationCountL( ECellBroadcastNotification, numberOfHotMsgs );
    CleanupStack::PopAndDestroy( notifier );
    */
    // <-- QT PHONE END-->
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::LaunchMessageImmediateDisplay
// Requests to show the message immediately.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::LaunchMessageImmediateDisplay(
    const TCbsDbMessage& /*aMessage*/ )
    {
    // <-- QT PHONE START -->
    /*
    TUid uiViewUid( TUid::Uid( ECbsUiMsgViewId ) );
    TVwsViewId id( KUidCbsUiappApp, uiViewUid );
    TPckgBuf<TCbsMessageHandle> pckg( aMessage.iHandle );

    // Ignore result value.
    iVwsSession->CreateActivateViewEvent( id, KCbsImmediateMessageId, pckg );
    */
    // <-- QT PHONE END-->
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::TurnLightsOn
// Turns lights on
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::TurnLightsOn()
    {
     // Change the bit on and off. SysAp will detect that
     // the lights should be switched on for the specified time.
     RProperty::Set(KPSUidCoreApplicationUIs, KLightsControl, ELightsOn);
     RProperty::Set(KPSUidCoreApplicationUIs, KLightsControl, ELightsOff);
    }

// -----------------------------------------------------------------------------
// CCbsReceiverHelper::PlayCbsTone
// Plays a tone
// -----------------------------------------------------------------------------
//
void CCbsReceiverHelper::PlayCbsTone()
    {
    // <-- QT PHONE START -->
    /*
    RProperty::Define( KPSUidNcnList, KNcnPlayAlertTone, RProperty::EInt,
        ECapability_None , ECapabilityWriteDeviceData );
    RProperty::Set( KPSUidNcnList, KNcnPlayAlertTone, KCbsMessageTone );
    */
    // <-- QT PHONE END-->
    }

// ================= OTHER EXPORTED FUNCTIONS ==============

//  End of File

