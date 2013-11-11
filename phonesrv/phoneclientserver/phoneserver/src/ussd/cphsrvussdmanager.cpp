/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  USSD Manager.
*
*/

// INCLUDE FILES
#include "cphsrvussdmanager.h" 
#include "phsrvutils.h" 
#include "cphsrvussdsendhandler.h" 
#include "cphsrvussdreceivehandler.h" 
#include "cphsrvussdnotifynwrelease.h" 
#include "cphsrvresourcemanager.h" 
#include "cphsrvussdreplytimer.h" 
#include "mphsrvussdmessagesentobserver.h" 
#include "mphsrvphoneinterface.h" 
#include "cphsrvussdsessioncancelwaiter.h" 

#include <apacmdln.h>
#include <apgtask.h>
#include <bautils.h>
#include <textresolver.h>
#include <charconv.h>
#include <gsmuelem.h>
#include <exterror.h>
#include <rmmcustomapi.h>

#include <w32std.h>
#include <apgcli.h>
#include <cphcltussd.h> 
#include <hbtextresolversymbian.h>
#include <phoneserver.rsg> 
#include "phsrvdebuginfo.h" 
#include <e32property.h>
#include <centralrepository.h>
#include <coreapplicationuisdomainpskeys.h>


// CONSTANTS
const TInt KPhSrvDefaultValue = 0x00000000;
// See SharedDataKeysVariant.h or NcnListInternalPSKeys.h
// const TInt KPhSrvUssdNoTone = 0; 
// const TInt KPhSrvUssdTimeOutObserverGranularity = 2;
// const TInt KPhSrvUssdSentMessageObserverGranularity = 2;
// See KUssdSecureId in phcltclientserver.h
//const TInt KPhSrvUssdAppUID = 0x10005955; 

const TInt KPhSrvUssdMessageQueryInterval = 500000; // 0.5 sec
//const TInt KPhSrvUssdNoteExitPeriod = 500000; // 0.5 sec
//
const TUint KPhSrvUssdDefaultDCS = 0x0f;                        // 00001111

const TUint KPhSrvUssdDcsAlphabetDefaultPrecededLanguage = 0x10;// 00010000
const TUint KPhSrvUssdDcsAlphabetDefaultPrecededLanguageSkipChars3 = 3;
const TUint KPhSrvUssdDcsAlphabetDefaultPrecededLanguageSkipChars2 = 2;
const TUint KPhSrvUssdDcsAlphabetDefaultPrecededLanguageStartBit = 5;

const TUint KPhSrvUssdDcsAlphabetUCS2PrecededLanguage = 0x11;   // 00010001
const TUint KPhSrvUssdDcsAlphabetUCS2PrecededLanguageSkipChars = 2;

const TUint KPhSrvUssdDcsGeneralInformationMask = 0xc0;         // 11000000
const TUint KPhSrvUssdDcsGeneralInformation = 0x40;             // 01xxxxxx
const TUint KPhSrvUssdDcsGeneralInformationCompressed = 0x20;   // xx1xxxxx
const TUint KPhSrvUssdDcsGeneralInformationSimMask = 0x13;      // 00010011
const TUint KPhSrvUssdDcsGeneralInformationSim = 0x12;          // xxx1xx10
const TUint KPhSrvUssdDcsGeneralInformationAlphabetMask = 0x0c; // 00001100
const TUint KPhSrvUssdDcsGeneralInformationAlphabet8Bit = 0x04; // xxxx01xx
const TUint KPhSrvUssdDcsGeneralInformationAlphabetUCS2 = 0x08; // xxxx10xx

const TUint KPhSrvUssdDcsMessageHandlingAlphabetMask = 0xf4;    // 11110100
const TUint KPhSrvUssdDcsMessageHandlingAlphabet8Bit = 0xf4;    // 1111x1xx
const TInt KPhrUssdNotifyArraySize = 1;

// Refers to HbPopup::NoDismiss = 0
const TInt KPhSrvUssdPopupDismissPolicy = 0; 
// The time out only for testing, from CPhSrvUssdReplyTimer.cpp
const TUint KPhSrvUssdTimeout = 300000000;

//Refers to HbMessageBox::Yes
const TUint KHbMsgBoxBtnYes = 0x00002000;
//Refers to HbMessageBox::No
const TUint KHbMsgBoxBtnNo = 0x00010000;

// Use QT style localization
_LIT(KUssdLocFilename, "ussd_");
_LIT(KCommonLocFilename, "common_");
_LIT(KLocPath, "z:\\resource\\qt\\translations");
_LIT(KUssdReply, "txt_ussd_button_reply"); // Reply
_LIT(KUssdExit, "txt_ussd_button_exit"); // Exit
_LIT(KUssdNext, "txt_ussd_button_next"); //Next
_LIT(KUssdYes, "txt_common_button_yes"); // Yes
_LIT(KUssdNo, "txt_common_button_no"); // No
_LIT(KUssdTitle, "txt_ussd_title_message"); // Message
_LIT(KUssdDone, "txt_ussd_dpopinfo_done"); // Done
_LIT(KUssdConfirm, "txt_ussd_info_there_are_still_unread_notifications");

// MACROS

#define _DPRINT_FLAGS() \
    _DDPRINT( 4, "PhSrv.FLAGS.MsgTypeReply ", iMsgTypeReply ); \
    _DDPRINT( 4, "PhSrv.FLAGS.NwReleased   ", iNetworkReleased ); \
    _DDPRINT( 4, "PhSrv.FLAGS.SendingAck   ", iSendingAck ); \
    _DDPRINT( 4, "PhSrv.FLAGS.AcksToBeSent ", iAcksToBeSent ); \
    _DDPRINT( 4, "PhSrv.FLAGS.iSendRelease ", iSendRelease ); \
    _DDPRINT( 4, "PhSrv.FLAGS.iClearArray  ", iClearArray ); \
    _DDPRINT( 4, "PhSrv.FLAGS.iLaunchGMQ   ", iLaunchGMQ ); \
    _DDPRINT( 4, "PhSrv.FLAGS.NotifyCount  ", NotifyCount() ); \
    _DDPRINT( 4, "PhSrv.FLAGS.iShowDone    ", iShowDone ); 

// ============================= LOCAL FUNCTIONS ===============================

// CLASS DECLARATION

#ifndef TPHSRVUSSDALPHABETPACKER_H
#define TPHSRVUSSDALPHABETPACKER_H

// CONSTANTS
const TUint8 KPhSrvUssdCarriageReturn = 0x0d;

/**
*  Packs and unpacks data encoded in an Ussd alphabet.
*
*  @since 1.0
*/
class TPhSrvUssdAlphabetPacker
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        *
        * @param aAlphabet The alphabet that is used.
        * @param aIsBinary Is the data binary, that will be converted.
        * @param aStartBit The start bit for conversion.
        */
        TPhSrvUssdAlphabetPacker(
            TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
            TBool aIsBinary,
            TInt aStartBit );


    public: // New functions

        /**
        * Unpacks 7-bit buffer to 8-bit buffer.
        *
        * @param aIn Descriptor where the 7-bit buffer is.
        * @param aOut 8-bit buffer to set the result.
        * @param aNumUDUnits Buffer length.
        * @return Length of the result buffer.
        */
        TInt UnpackL(
            const TDesC8& aIn,
            TDes8& aOut,
            TInt aNumUDUnits );

        /**
        * Unpacks 7-bit buffer to 8-bit and converts to unicode.
        *
        * @param aCharacterSetConverter Does the converting.
        * @param aFs File server session.
        * @param aIn Packed 7-bit buffer.
        * @param aOut Unicode buffer.
        * @param aNumUDUnits Number of charahters.
        * @return Number of used octets in 8-bit buffer.
        */
        TInt UnpackAndConvertL(
            CCnvCharacterSetConverter& aCharacterSetConverter,
            RFs& aFs,
            const TDesC8& aIn,
            TDes& aOut,
            TInt aNumUDUnits );

        /**
        * Counts the needed octet amout.
        *
        * @param aNumUDUnits Number of source characters.
        * @return Octets needed.
        */
        TInt PackedOctetsRequiredL( TInt aNumUDUnits ) const;


    private:

        // Private helper methods
        TInt ElementSizeInBitsL() const;


    private:    // Data

        // The used coding alphabet.
        TSmsDataCodingScheme::TSmsAlphabet iAlphabet;

        // Is the packed data binary.
        TBool iIsBinary;

        // The start bit for packing.
        TInt iStartBit;
    };

#endif // TPHSRVUSSDALPHABETPACKER_H


// -----------------------------------------------------------------------------
// TPhSrvUssdAlphabetPacker::TPhSrvUssdAlphabetPacker
// Constructor.
//
// -----------------------------------------------------------------------------
//
TPhSrvUssdAlphabetPacker::TPhSrvUssdAlphabetPacker(
    TSmsDataCodingScheme::TSmsAlphabet aAlphabet,
    TBool aIsBinary,
    TInt aStartBit)
    : iAlphabet( aAlphabet ),iIsBinary( aIsBinary ),iStartBit( aStartBit )
    {
    }


// -----------------------------------------------------------------------------
// TPhSrvUssdAlphabetPacker::UnpackL
// Unpacks user data units from aIn and appends to aOut.
//
// -----------------------------------------------------------------------------
//
TInt TPhSrvUssdAlphabetPacker::UnpackL(
    const TDesC8& aIn,
    TDes8& aOut,
    TInt aNumUDUnits )
    {
    TInt length = aNumUDUnits;
    // Ensure we've got enough  output buffer
    if ( aOut.Length() + length > aOut.MaxLength() )
        {
        User::Leave(KErrCorrupt);
        }

    TInt elementSizeInBits = ElementSizeInBitsL();
    if ( elementSizeInBits == 8 ) // 8-bit data?
        {
        aOut.Append( aIn );
        }
    else if ( elementSizeInBits == 7 ) // need unpacking from 7-bit data
        {
        // Get raw pointers and do unpacking
        TUint8* outPtr = ( TUint8* )aOut.Ptr() + aOut.Length();
        const TUint8* inPtr = aIn.Ptr();

        for ( TInt i = 0; i < length; i++ )
            {
            TInt from = ( *inPtr >> iStartBit ) & 0x7F; // Take seven bits
            if ( iStartBit )
                {
                inPtr++;
                // Take more from next byte
                from |= ( *inPtr << ( 8 - iStartBit ) ) & 0x7F;
                }
            outPtr[i] = ( TUint8 ) from;
            iStartBit = ( iStartBit + 7 ) % 8; // roll: 0,1,2,...,6,7,0,1,2...
            }

        // The last character will be ignored if message ends
        // on an octet bounday with CR.
        if ( outPtr[ length-1 ] == KPhSrvUssdCarriageReturn && iStartBit == 0 )
            {
            length--;
            }
        aOut.SetLength( aOut.Length() + length );
        }
    else
        {
        __ASSERT_DEBUG(EFalse,
            PhoneServerUtils::Panic( EPhoneServerPanicModeLogicFault ) );
        }
    return length;
    }


// -----------------------------------------------------------------------------
// TPhSrvUssdAlphabetPacker::UnpackAndConvertL
// Unpacks the converts the input data, aIn, and appends to aOut
//
// -----------------------------------------------------------------------------
//
TInt TPhSrvUssdAlphabetPacker::UnpackAndConvertL(
    CCnvCharacterSetConverter& aCharacterSetConverter,
    RFs& aFs,
    const TDesC8& aIn,
    TDes& aOut,
    TInt aNumUDUnits )
    {

    // Unpack first
    HBufC8* unpackedBuffer = HBufC8::NewLC( aNumUDUnits );
    TPtr8 unpackedBufferPtr( unpackedBuffer->Des() );
    UnpackL( aIn, unpackedBufferPtr,aNumUDUnits );

    // Convert
    CSmsAlphabetConverter* converter =
        CSmsAlphabetConverter::NewLC(
            aCharacterSetConverter, aFs, iAlphabet, iIsBinary );

    TPtrC convertedPtr = converter->ConvertToNativeL( *unpackedBuffer );
    if ( convertedPtr.Length() > ( aOut.MaxLength() - aOut.Length() ) )
        {
        User::Leave( KErrCorrupt );
        }
    // Cleanup and return
    aOut.Append( convertedPtr );
    CleanupStack::PopAndDestroy( 2 );    // unpackedBuffer, converter
    return aNumUDUnits;
    }


// -----------------------------------------------------------------------------
// TPhSrvUssdAlphabetPacker::PackedOctetsRequiredL
// Returns the number of octets needed to pack the specified number of
//
// -----------------------------------------------------------------------------
//
TInt TPhSrvUssdAlphabetPacker::PackedOctetsRequiredL( TInt aNumUDUnits ) const

    {
    TInt octetsRequired = 0;
    TInt elementSizeInBits = ElementSizeInBitsL();
    if ( elementSizeInBits == 8 )
        {
        octetsRequired=aNumUDUnits;
        }
    else
        {
        octetsRequired =
            ( iStartBit+aNumUDUnits * elementSizeInBits + 7 ) / 8;  // Rounds up
        }
    return octetsRequired;
    }

// -----------------------------------------------------------------------------
// TPhSrvUssdAlphabetPacker::ElementSizeInBitsL
// Returns the size in bits of a UDL element for the alphabet.  Leaves if
// invalid data coding scheme.
// -----------------------------------------------------------------------------
//
TInt TPhSrvUssdAlphabetPacker::ElementSizeInBitsL() const
    {
    if (iIsBinary)
        return 8;
    switch ( iAlphabet )
        {
        case TSmsDataCodingScheme::ESmsAlphabet7Bit:
            {
            return 7;
            }
        case TSmsDataCodingScheme::ESmsAlphabet8Bit:
        case TSmsDataCodingScheme::ESmsAlphabetUCS2:
            {
            return 8;
            }
        default:
            {
            User::Leave(KErrGsmSMSDataCodingSchemeNotSupported);
            return 8;
            }
        }
    }



// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::CPhSrvUssdManager
//
// Constructor
//
// -----------------------------------------------------------------------------
//
CPhSrvUssdManager::CPhSrvUssdManager(
    RFs& aFsSession,
    CPhSrvResourceManager& aResourceManager
    )
    :CActive( EPriorityLow ),
     iFsSession( aFsSession ),
     iResourceManager( aResourceManager ),
     iDeviceDialog( NULL ),
     iDCS ( KPhCltDcsUnknown ),
     iReturnResultPckg ( iReturnResult ),
     iTextBuffer ( NULL ),
     iDialogType ( EInvalidType )
    {
    CActiveScheduler::Add( this );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::~CPhSrvUssdManager
//
// Destructor
//
// -----------------------------------------------------------------------------
//
CPhSrvUssdManager::~CPhSrvUssdManager()
    {
    _DPRINT( 4, "PhSrv.~CPhSrvUssdManager.start" );


    delete iUssdSendHandler;
    iUssdSendHandler = NULL;

    delete iUssdReceiveHandler;
    iUssdReceiveHandler = NULL;

    delete iUssdNotifyNWRelease;
    iUssdNotifyNWRelease = NULL;

    Cancel();

    iTimer.Close();
    
    delete iTextBuffer;
    iTextBuffer = NULL;
    
    delete iDeviceDialog;
    iDeviceDialog = NULL;

    if ( iNotifyArray )
        {
        iNotifyArray->Reset();
        }
    delete iNotifyArray;
    if ( iUssdReplyTimer )
        {
        iUssdReplyTimer->Stop();
        }
    delete iUssdReplyTimer;
    iUssdReplyTimer = NULL;

    iFsSession.Close();

    iMobileUssdMessaging.Close();
    
    delete iMoAckCallback;

    _DPRINT( 4, "PhSrv.~CPhSrvUssdManager.end" );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::ConstructL
//
// Symbian OS 2nd phase constructor
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::ConstructL( MPhSrvPhoneInterface& aPhoneInterface )
    {
    _DPRINT( 4, "PhSrv.ConstructL.Start" );
    const TBool textResolver = HbTextResolverSymbian::Init( 
        KUssdLocFilename, KLocPath );
    _DDPRINT( 4, "PhSrv.ConstructL.ussd loc:", textResolver );
    User::LeaveIfError( iTimer.CreateLocal() );

    User::LeaveIfError( iMobileUssdMessaging.Open( aPhoneInterface.PhSrvMobilePhone() ) );

    // Check support
    RMobileUssdMessaging::TMobileUssdCapsV1 caps;
    RMobileUssdMessaging::TMobileUssdCapsV1Pckg pckgCaps( caps );
    User::LeaveIfError( iMobileUssdMessaging.GetCaps( pckgCaps ) );

    if ( !( ( caps.iUssdTypes & RMobileUssdMessaging::KCapsMTUssd ) == 0 &&
        ( caps.iUssdFormat & RMobileUssdMessaging::KCapsPackedString ) == 0 ) )
        {
        _DPRINT( 4, "PhSrv.CPhSrvUssdManager::ConstructL" );
        iUssdReceiveHandler = new( ELeave ) CPhSrvUssdReceiveHandler(
            *this,
            iMobileUssdMessaging );
        iUssdReceiveHandler->ConstructL();

        iUssdNotifyNWRelease = new( ELeave )  CPhSrvUssdNotifyNWRelease(
            *this,
            iMobileUssdMessaging );

        iUssdNotifyNWRelease->ConstructL();
        }

    iPhoneInterface = &aPhoneInterface;

    SendHandlerL();
    
    iMoAckCallback = new (ELeave) CAsyncCallBack( TCallBack( MoAckCallback, this ), CActive::EPriorityIdle );

    iVariantReadOnlyValues = KPhSrvDefaultValue;

    User::LeaveIfError( GetTelephonyVariantData() );

    _DDPRINT( 4, "PhSrv.ConstructL.iSatCanceled ", iSatCanceled );
    _DDPRINT( 4, "PhSrv.ConstructL.iShowDone ", iShowDone );
    iNotifyArray = new( ELeave ) CDesCArrayFlat( KPhrUssdNotifyArraySize );
    _DPRINT( 4, "PhSrv.ConstructL.End" );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::SendHandlerL
//
// The send handler.
//
// -----------------------------------------------------------------------------
//
CPhSrvUssdSendHandler& CPhSrvUssdManager::SendHandlerL()
    {
    // If SendHandler is not created, first check that MO Ussd
    // is supported by the TSY.
    _DPRINT( 4, "PhSrv.SendHandlerL.Start" );
    if ( iUssdSendHandler == NULL )
        {
        _DPRINT( 4, "PhSrv.SendHandlerL.iUssdSendHandler.NULL" );
        RMobileUssdMessaging::TMobileUssdCapsV1 caps;
        RMobileUssdMessaging::TMobileUssdCapsV1Pckg pckgCaps( caps );
        User::LeaveIfError( iMobileUssdMessaging.GetCaps( pckgCaps ) );
        _DPRINT( 4, "PhSrv.SendHandlerL.iMobileUssdMessaging.GetCaps" );

        if ( ( caps.iUssdTypes & RMobileUssdMessaging::KCapsMOUssd ) == 0 ||
            ( caps.iUssdFormat & RMobileUssdMessaging::KCapsPackedString )
            == 0 )
            {
            _DPRINT( 4, "PhSrv.SendHandlerL.KErrNotSupported" );
            User::Leave( KErrNotSupported );
            }

        iUssdSendHandler =
            new( ELeave ) CPhSrvUssdSendHandler(
                *this,
                iMobileUssdMessaging,
                *iPhoneInterface );
        }
    _DPRINT( 4, "PhSrv.SendHandlerL.End" );
    return *iUssdSendHandler;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::SendUssdL
//
// Send the USSD message.
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::SendUssdL(
    const TDesC8& aMsgData,
    RMobileUssdMessaging::TMobileUssdAttributesV1& aMsgAttribute,
    MPhSrvUssdMessageSentObserver& aObserver )
    {
    _DPRINT( 4, "PhSrv.SendUssdL.Start ######" );
    _DPRINT_FLAGS();
    
    if ( iObserver && iNetworkReleased ) 
        {
        // Network has been released but the previous send request is still alive.
        // Cancel the pervious send operation, complete the old request with error
        // and clean up the pointer.
        _DPRINT( 4, "PhSrv.SendUssdL.Error.Complete.Existing" );
        if ( iUssdSendHandler ) 
            {
            iUssdSendHandler->Cancel();
            }
        iObserver->UssdMessageSentObserverHandleResult( KErrSessionClosed );
        iObserver = NULL;
        }
    
    if ( iObserver || iSendingAck )
        {
        _DPRINT( 4, "PhSrv.SendUssdL.KErrInUse" );
        // Other client is using the service.
        User::Leave( KErrInUse );
        }

    // Check that message type is set
    if( ( aMsgAttribute.iFlags & RMobileUssdMessaging::KUssdMessageType )
        == 0 )
        _DPRINT( 4, "PhSrv.SendUssdL.KUssdMessageType.0" );
        {
        // Mesasge type not set -> Set it.
        aMsgAttribute.iFlags |= RMobileUssdMessaging::KUssdMessageType;
        if ( NetworkWaitingForAnAnswer() )
            {
            // Network is waiting for an answer
            _DPRINT( 4, "PhSrv.SendUssdL.EUssdMOReply" );   // debug print
            aMsgAttribute.iType = RMobileUssdMessaging::EUssdMOReply;
            }
        else
            {
            _DPRINT( 4, "PhSrv.SendUssdL.EUssdMORequest" );  // debug print
            aMsgAttribute.iType = RMobileUssdMessaging::EUssdMORequest;
            }
        }
        
    RMobileUssdMessaging::TMobileUssdAttributesV1Pckg attribs( aMsgAttribute );
    iShowDone = ETrue;
    _DPRINT( 4, "PhSrv.SendUssdL.iShowDone.ETrue" );
    _DPRINT( 4, "PhSrv.SendUssdL.Send" );
    SendHandlerL().SendUssdL( aMsgData , attribs );
    iObserver = &aObserver;
    // Not closing nor closed anymore
    iNetworkReleased = EFalse;
    iSendRelease = EFalse;
    iClearArray = EFalse;
    
    // Reply timer can be killed here... Receive event might come right after this
    // which will setup the timer again if needed.
    if ( NetworkWaitingForAnAnswer() )
        {
        _DPRINT( 4, "PhSrv.SendUssdCancel.TimerStop" );   // debug print
        iUssdReplyTimer->Stop();
        }
    _DPRINT( 4, "PhSrv.SendUssdL.End" );
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::NetworkWaitingForAnAnswer
// -----------------------------------------------------------------------------
//
TBool CPhSrvUssdManager::NetworkWaitingForAnAnswer() 
    {
    return iUssdReplyTimer && iUssdReplyTimer->IsTimerActive();
    }
// -----------------------------------------------------------------------------
// CPhSrvUssdManager::SendUssdCancel
//
// Cancel the USSD sending.
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::SendUssdCancel()
    {
    _DPRINT( 4, "PhSrv.SendUssdCancel.Start #######" );

    // Ack sending should not be canceled unless it's about terminating
    // the whole session
    _DPRINT_FLAGS();
    if ( !iSendingAck || iSendRelease ) 
        {
        _DPRINT( 4, "PhSrv.SendUssdCancel.DoingCancel" );
        iUssdSendHandler->SendUssdCancel();
        }

    if ( iHavePendingSatMessagePointer )
        {
        _DPRINT( 4, "PhSrv.SendUssdCancel.SatCancel" );
        iSatCanceled = ETrue;
        }
    iObserver = NULL;
    
    SetActiveIfPendingNotificationsExist();
    _DPRINT( 4, "PhSrv.SendUssdCancel.End" );
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::SetActiveIfPendingNotificationsExist
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::SetActiveIfPendingNotificationsExist()
    {
    if ( !IsActive() && NotifyCount() > 0 )
        {
        _DPRINT( 4, "PhSrv.AcrtivateIf.NotActiveSelf" );
        iLaunchGMQ = ETrue;
        iNotifyMessage = ETrue;
        _DPRINT( 4, "PhSrv.SendUssdCancel.iNotifyMessage.ETrue" );
        iTimer.After( iStatus , KPhSrvUssdMessageQueryInterval );
        SetActive();
        _DPRINT_FLAGS();
        _DDPRINT( 4, "PhSrv.AcrtivateIf.SetActive ", iStatus.Int() );
        }
    }
// -----------------------------------------------------------------------------
// CPhSrvUssdManager::UssdNetworkObserverHandleSendEventL
//
//
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::UssdNetworkObserverHandleSendEventL( TInt aError )
    {
    // debug print
    _DDPRINT(
        4,
        "PhSrv.UssdNetworkObserverHandleSendEventL.Start: ##### ",
        aError );

    // complete SAT if needed
    if ( aError < KErrNone )
        {
 
        _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleSendEventL.CompleteSat" );
        CompleteSatL( NULL, aError );
        }

    if ( iObserver )
        {
 
        _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleSendEventL.Observer" );

        iObserver->UssdMessageSentObserverHandleResult( aError );
        iObserver = NULL;
        }
    
    iSendingAck = EFalse;
    
    AsyncProcessMoAcks();
    
    // debug print
    _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleSendEventL.End" );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::UssdAppTaskExists
//
//
//
// -----------------------------------------------------------------------------
//
TBool CPhSrvUssdManager::UssdAppTaskExistsL()
{   
    TBool ret = EFalse;
    RWsSession wsSession;
    User::LeaveIfError(wsSession.Connect());
    _DPRINT( 4, "PhSrv.UssdAppTaskExists.wsSession.Connect" );
    CleanupClosePushL(wsSession);
    TApaTaskList tasklist(wsSession);
    TApaTask task = tasklist.FindApp( KUssdSecureId );
    ret = task.Exists();

    CleanupStack::PopAndDestroy();
    return ret;
}

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::UssdNetworkObserverHandleReceivedEventL
//
//
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::UssdNetworkObserverHandleReceivedEventL(
    const TDes8& aMsgData,
    const RMobileUssdMessaging::TMobileUssdAttributesV1& aMsgAttributes,
    TInt aError )
    {
    _DDPRINT(
        4,
        "PhSrv.UssdNetworkObserverHandleReceivedEventL.Start: ### ",
        aError );
    _DPRINT_FLAGS();
    if ( aError != KErrNone )
        {
        TurnLightsOn(); //Ensure lights on
 
        _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleReceivedEventL.ShErNote" );
        ShowErrorNoteL( aError );
        }
    else if ( iSendRelease )
        {
        // Ignore if sendrelease is to be done. If the message was
        _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleReceivedEventL.SendRelease.Ignore" );
        }
    else
        {
        UssdHandleReceivedEventL( aMsgData, aMsgAttributes );
        }
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::UssdHandleReceivedEventL
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::UssdHandleReceivedEventL(
    const TDes8& aMsgData,
    const RMobileUssdMessaging::TMobileUssdAttributesV1& aMsgAttributes )
    {
    // This always is either ongoing transaction or starting a new based
    // on incoming message, mark transaction to be open.
    iNetworkReleased = EFalse;
        
    // 2. Complete Send with some positive value.
    if ( iObserver )
        {
 
        _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.Observer" );
        UssdNetworkObserverHandleSendEventL( 1 ); // some positive value
        }
    
    if ( aMsgAttributes.iFlags&RMobileUssdMessaging::KUssdMessageType &&
         aMsgAttributes.iType == RMobileUssdMessaging::EUssdMTRequest )
        {
        // 3. Stop old reply timer and start new one if needed.
        RestartReplyTimerL();
        _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleReceivedEventL.Timer.Stop" );
        _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleReceivedEventL.Timer.New" );
        }
    _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.Decode" );
    TUint8 dcs = KPhSrvUssdDefaultDCS;
    if(aMsgAttributes.iFlags & RMobileUssdMessaging::KUssdMessageDcs)
        {
        dcs = aMsgAttributes.iDcs;
        }
    iDecodedMessage.Zero();
    iReceivedMessage.Zero();
    DecodeL(aMsgData, iDecodedMessage, dcs);
    if(iDecodedMessage.Length() > 0)
        {
        iReceivedMessage = iDecodedMessage;
        }
    // 5. Show note.
    // debug print
    _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.Note" );
    if ( !( aMsgAttributes.iFlags & RMobileUssdMessaging::KUssdMessageType ) ||
            aMsgAttributes.iType != RMobileUssdMessaging::EUssdMTRequest )
        {
        if ( !ShowNotesL() )
            {
     
            _DPRINT( 4,
                "PhSrv.UssdHandleReceivedEventL.SAtReturn" );
            return;
            }
        }

    // empty string is handled as ok message
    if ( !iDecodedMessage.Length() )
        {
        TurnLightsOn(); //Ensure lights on
 
        _DPRINT( 4,
            "PhSrv.UssdHandleReceivedEventL.EmptyString" );
        
        ShowDoneNoteL();

 
        _DPRINT( 4,
            "PhSrv.UssdHandleReceivedEventL.EmptyString.OK" );
        }
    else
        {
        _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.String" );
        iNotifyMessage = ( aMsgAttributes.iType == RMobileUssdMessaging::EUssdMTNotify );
        _DDPRINT( 4, "PhSrv.UssdHandleReceivedEventL.iNotifyMessage: ", iNotifyMessage );
        _DDPRINT( 4, "PhSrv.UssdNOHREventL.iNotifyMessage: ", iNotifyMessage );
        iMsgTypeReply = ( aMsgAttributes.iType == RMobileUssdMessaging::EUssdMTReply );
        _DDPRINT( 4, "PhSrv.UssdHandleReceivedEventL.iMsgTypeReply: ", iMsgTypeReply );
        
        if ( iNotifyMessage ) 
            {
            // need to send an MO ack
            iAcksToBeSent ++;
            }
        
        if ( !iDeviceDialog  ){
            iDeviceDialog = CHbDeviceMessageBoxSymbian::NewL(
                CHbDeviceMessageBoxSymbian::EInformation );
            iDeviceDialog->SetObserver( this );
            iDeviceDialog->SetTimeout( KPhSrvUssdTimeout );
            iDeviceDialog->SetDismissPolicy ( KPhSrvUssdPopupDismissPolicy );
            iDeviceDialog->SetIconVisible(EFalse);
            
            // Show left key with empty string accoring to ui concept
            iDeviceDialog->SetButton( 
                CHbDeviceMessageBoxSymbian::EAcceptButton, EFalse );
            // Show Exit Key always
            iDeviceDialog->SetButton( 
                CHbDeviceMessageBoxSymbian::ERejectButton, ETrue );
            iDeviceDialog->SetButtonTextL(
                CHbDeviceMessageBoxSymbian::ERejectButton, 
                LoadDefaultString( KUssdExit ) );
            _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.Exit" );             
            iDialogType = EOnlyExit;
        }
      
        if ( iNotifyMessage || iMsgTypeReply )
            {
            //This is for reply message in notifyarray
            iNotifyMessage = ETrue;
            _DDPRINT( 4, "PhSrv.UssdHandleReceivedEventL.iNotifyMessage: ", iNotifyMessage );

            //Notify added to array
            iNotifyArray->AppendL( iReceivedMessage );

            _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.AppendL" );
            UpdateNotifyMessageL();

            if ( !iSendRelease && NotifyCount() <= 1 )
                {
                _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.!SendRelease.Cancel" );
                Cancel();
                }
            }
        else
            {
            // New message deletes old message, i.e. Cancel existing query.
            Cancel();
            _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.NewAnswerable" );
            }

        // Remove Reply key
        if( !( aMsgAttributes.iFlags & RMobileUssdMessaging::KUssdMessageType )
             || aMsgAttributes.iType != RMobileUssdMessaging::EUssdMTRequest )
            {
            // Remove Answer key
            iDeviceDialog->SetButton( 
                CHbDeviceMessageBoxSymbian::EAcceptButton, EFalse );
            _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.NoAnswer" ); 
            iDialogType = EOnlyExit;
            }
        // Show Reply key
        else
            {
            iDeviceDialog->SetButton( 
                CHbDeviceMessageBoxSymbian::EAcceptButton, ETrue );              
            iDeviceDialog->SetButtonTextL(
                CHbDeviceMessageBoxSymbian::EAcceptButton, 
                LoadDefaultString( KUssdReply ) ); 
            iDeviceDialog->SetButton( 
                CHbDeviceMessageBoxSymbian::ERejectButton, ETrue );              
            iDeviceDialog->SetButtonTextL(
                CHbDeviceMessageBoxSymbian::ERejectButton, 
                LoadDefaultString( KUssdExit ) );
            _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.WithAnswer" ); 
            iDialogType = EReplyExit;
            }

 
        _DPRINT( 4,
            "PhSrv.UssdHandleReceivedEventL.String.Middle" );

        // Play the USSD tone if needed. Logically should be in RunL, but here
        // to give better balancing with voice and visible message.
        // <-- QT PHONE START-->
        /*
        if ( IsTelephonyFeatureSupported( KTelephonyLVFlagUssdTone ) )
            {
            _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.PlayTone" );
            PlayUssdTone();
            }
        */
            // <-- QT PHONE END-->
        // Launch the new message query
        if ( !IsActive() )
            {
            iLaunchGMQ = ETrue;
            iTimer.After( iStatus , KPhSrvUssdMessageQueryInterval );
            SetActive();
            }
        AsyncProcessMoAcks();
        }
    _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.Editor" );
    if( aMsgAttributes.iType == RMobileUssdMessaging::EUssdMTRequest && UssdAppTaskExistsL() )
        {
        iEmptyEditor = ETrue;
        }
    _DPRINT( 4, "PhSrv.UssdHandleReceivedEventL.End" );
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::RestartReplyTimerL
//
//
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::RestartReplyTimerL()
    {
    if ( iUssdReplyTimer )
        {
        _DPRINT( 4, "PhSrv.RestartReplyTimerL.Stop" );
        iUssdReplyTimer->Stop();
        }

    if ( !iUssdReplyTimer )
        {
        _DPRINT( 4, "PhSrv.RestartReplyTimerL.New" );
        iUssdReplyTimer = CPhSrvUssdReplyTimer::NewL( *this );
        }
    
    _DPRINT( 4, "PhSrv.RestartReplyTimerL.Start" );
    iUssdReplyTimer->Start();
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::UssdNetworkObserverHandleNotifyNWReleaseL
//
//
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::UssdNetworkObserverHandleNotifyNWReleaseL(
const RMobilePhone::TMobilePhoneSendSSRequestV3 & aReturnResult,
TInt aError )
    {
    _DDPRINT( 4, "PhSrv.UssdNotifyNWRelease ##### ", aError ); // debug print
    _DPRINT_FLAGS();
    TBool showNotes = ShowNotesL();
    iNetworkReleased = ETrue;
    iSendingAck = EFalse;
    iSendRelease = EFalse;
    iAcksToBeSent = 0;
    
    if ( iHavePendingSatMessagePointer )
        {
        iReturnResult = aReturnResult;
        if ( !iSatCanceled )
            {
            CompleteSatL(&iReceivedMessage, aError );
            _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleNotifyNWReleaseL.CompleteSat" );
            }
        }
    if ( iUssdReplyTimer && iUssdReplyTimer->IsTimerActive() )
        {
        _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleNotifyNWReleaseL.Timer.Stop" );
        iUssdReplyTimer->Stop();
        Cancel();
        CheckArray();
        }
    if ( showNotes && iShowDone )
        {
        ShowDoneNoteL();
        }
    if ( iUssdSendHandler && iSendingAck ) 
        {
        // if we are sending ack, it can be canceled.
        iUssdSendHandler->Cancel();
        }
    
    iSatCanceled = EFalse;
    _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleNotifyNWReleaseL.iSatCanceled.EFalse" );
    iShowDone = EFalse;
    _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleNotifyNWReleaseL.Ending" );
    _DPRINT_FLAGS();
    _DPRINT( 4, "PhSrv.UssdNetworkObserverHandleNotifyNWReleaseL.End" );
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::ShowDoneNoteL
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::ShowDoneNoteL()
    {
    _DDPRINT( 4, "PhSrv.ShowDoneNoteL.iShowDone", iShowDone );
    // Show global confirmation note "Done"
    CHbDeviceMessageBoxSymbian::InformationL(
        LoadDefaultString( KUssdDone ) );
    iShowDone = EFalse;
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::ShowErrorNoteL
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::ShowErrorNoteL( TInt aError )
    {
    _DDPRINT( 4, "PhSrv.ShowErrorNoteL.Start ", aError );
    if ( aError == KErrCancel )
        {
        _DPRINT( 4, "PhSrv.ShowErrorNoteL.Return" );
        return;
        }

    // Show specific error message
    CTextResolver* textResolver = CTextResolver::NewLC();
    HBufC* buffer = textResolver->ResolveErrorString( aError ).AllocLC();

    CHbDeviceMessageBoxSymbian::InformationL( buffer ->Des() );

    CleanupStack::PopAndDestroy( buffer ); 
    CleanupStack::PopAndDestroy( textResolver ); 
    
    
    _DPRINT( 4, "PhSrv.ShowErrorNoteL.End" );
    return;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::DecodeL
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::DecodeL(
    const TDesC8& aSrc,
    TDes& aDes ,
    TUint8 aDcs )
    {
    _DPRINT( 4, "PhSrv.DecodeL.Start" );
    aDes.Zero();
    CCnvCharacterSetConverter* charConv = CCnvCharacterSetConverter::NewLC();

    iDCS = KPhCltDcsUnknown; // default

    // Check DCS
    TSmsDataCodingScheme::TSmsAlphabet alphabet =
        TSmsDataCodingScheme::ESmsAlphabet7Bit; // default
    TUint cutChars = 0;
    TUint startBit = 0;

    // Codes from GSM 03.38
    if ( aDcs == KPhSrvUssdDcsAlphabetDefaultPrecededLanguage )
        {
        FindFirstCarriageReturnL( aSrc , cutChars , startBit );
        }
    else if ( aDcs == KPhSrvUssdDcsAlphabetUCS2PrecededLanguage )
        {
        alphabet = TSmsDataCodingScheme::ESmsAlphabetUCS2;
        cutChars = KPhSrvUssdDcsAlphabetUCS2PrecededLanguageSkipChars;
        }
    else if ( (aDcs&KPhSrvUssdDcsGeneralInformationMask)  ==
            KPhSrvUssdDcsGeneralInformation ) // General data coding information
        {
        //Is text compressed?
        if ( aDcs & KPhSrvUssdDcsGeneralInformationCompressed )
            {
            _DPRINT( 4, "PhSrv.DecodeL.KErrNotSupported" );
            User::Leave( KErrNotSupported );
            }
        // Is SIM specific message
        else if ( ( aDcs & KPhSrvUssdDcsGeneralInformationSimMask )
                   == KPhSrvUssdDcsGeneralInformationSim )
            {
            _DPRINT( 4, "PhSrv.DecodeL.KErrAbort" );
            User::Leave( KErrAbort ); // Do not show any messages
            }
        // 8 bit data?
        else if ( ( aDcs&KPhSrvUssdDcsGeneralInformationAlphabetMask ) ==
                   KPhSrvUssdDcsGeneralInformationAlphabet8Bit )
            {
            alphabet = TSmsDataCodingScheme::ESmsAlphabet8Bit;
            }
        // UCS2 bit data?
        else if ( ( aDcs&KPhSrvUssdDcsGeneralInformationAlphabetMask ) ==
                   KPhSrvUssdDcsGeneralInformationAlphabetUCS2 )
            {
            alphabet = TSmsDataCodingScheme::ESmsAlphabetUCS2;
            }
        }
    // Data coding/message handling
    else if ( ( aDcs & KPhSrvUssdDcsMessageHandlingAlphabetMask )
               == KPhSrvUssdDcsMessageHandlingAlphabet8Bit )
        {
        alphabet = TSmsDataCodingScheme::ESmsAlphabet8Bit;
        }

    TPhSrvUssdAlphabetPacker* packer =
        new ( ELeave ) TPhSrvUssdAlphabetPacker( alphabet , EFalse, startBit );
    CleanupStack::PushL( packer );

    if ( alphabet == TSmsDataCodingScheme::ESmsAlphabet7Bit )
        {
        TInt numberOfElem = ( ( aSrc.Length() - cutChars ) * 8 - startBit ) / 7;
        packer->UnpackAndConvertL(
            *charConv ,
            iFsSession ,
            aSrc.Right( aSrc.Length() - cutChars ),
            aDes ,
            numberOfElem );

        // DCS was 7-bit data.
        iDCS = KPhCltDcs7Bit;
        }
    else // ESmsAlphabet8Bit || ESmsAlphabetUCS2
        {
        CSmsAlphabetConverter* converter =
            CSmsAlphabetConverter::NewLC(
                *charConv , iFsSession,alphabet,EFalse );

        aDes = converter->ConvertToNativeL(
            aSrc.Right( aSrc.Length() - cutChars ) );
        CleanupStack::PopAndDestroy( converter );

        if( alphabet == TSmsDataCodingScheme::ESmsAlphabet8Bit )
            {
            // DCS was 8-bit data.
            iDCS = KPhCltDcs8Bit;
            }
        else if( alphabet == TSmsDataCodingScheme::ESmsAlphabetUCS2 )
            {
            // DCS was UCS2 data.
            iDCS = KPhCltDcsUcs2;
            }
        // If DCS not 8-bit or UCS2, then EPhCltDcsUnknown is returned.
        }

    //
    CleanupStack::PopAndDestroy(2); // packer, charConv
    _DPRINT( 4, "PhSrv.DecodeL.End" );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::FindFirstCarriageReturnL
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::FindFirstCarriageReturnL(
    const TDesC8& aBuffer ,
    TUint& aSkipChars ,
    TUint& aStartBit )
    {
    _DPRINT( 4, "PhSrv.FindFirstCarriageReturnL.Start" );
    if ( aBuffer.Length() < 3 )
        {
        _DPRINT( 4, "PhSrv.FindFirstCarriageReturnL.KErrUnderflow" );
        User::Leave( KErrUnderflow );
        }

    aSkipChars = 0;
    aStartBit = 0;

    // Try out two different cases:
    // 1. Find CR from thrid byte
    // 2. Find CR from second byte, starting from bit 6

    // 1.:
    if ( aBuffer[2] == KPhSrvUssdCarriageReturn )
        {
        aSkipChars = KPhSrvUssdDcsAlphabetDefaultPrecededLanguageSkipChars3;
        aStartBit = 0;
        _DPRINT( 4, "PhSrv.FindFirstCarriageReturnL.ThirdByte.Return" );
        return;
        }

    // 2.:
    // First put the pieces together and then compare
    // Take last 2 bits from the second byte:
    TUint result1 = aBuffer[1];
    result1 = result1 >> 6;

    // Take first 5 bits from the third byte:
    TUint result2 = aBuffer[2];
    result2 = result2 & 0x1f; // bits 0-4.
    result2 = result2 << 2; // move to bits 2-6.

    TUint result = result1 + result2; // 0000 00xx + 0xxx xx00
    if ( result == KPhSrvUssdCarriageReturn )
        {
        aSkipChars = KPhSrvUssdDcsAlphabetDefaultPrecededLanguageSkipChars2;
        aStartBit = KPhSrvUssdDcsAlphabetDefaultPrecededLanguageStartBit;
        _DPRINT( 4, "PhSrv.FindFirstCarriageReturnL.SecondByte.Return" );
        return;
        }

    // Is was not case 1. or 2. so we are not supporting sort of string.
    User::Leave( KErrNotSupported );
    _DPRINT( 4, "PhSrv.FindFirstCarriageReturnL.KErrNotSupported" );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::UssdReplyTimerObserverHandleExpired
//
//
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::UssdReplyTimerObserverHandleExpiredL( TInt aError )
    {
    _DDPRINT( 4, "PhSrv.ReplyTimer.Start.aError ", aError );     // debug print
    delete iUssdReplyTimer;
    iUssdReplyTimer = NULL;

    if ( aError == KErrNone &&
         IsActive() &&
         iDeviceDialog ) 
        {
        Cancel();
        // Terminates USSD session.
        _DPRINT( 4, "PhSrv.ReplyTimer.SendRelease" );     // debug print
        if ( iNotifyArray ) 
            {
            iNotifyArray->Reset();
            }
        TryCloseSession();
        }
    _DPRINT( 4, "PhSrv.ReplyTimer.End" );     // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::MessageBoxClosed
// -----------------------------------------------------------------------------
//       
void CPhSrvUssdManager::MessageBoxClosed(
    const CHbDeviceMessageBoxSymbian* /*aMessageBox*/,
    CHbDeviceMessageBoxSymbian::TButtonId aButton)
    {
    _DPRINT( 4, "PhSrv.MsgClose.Start" );
    // ussd device dialog observer callback function

    _DDPRINT( 4, "PhSrv.MsgClose.iDialogType=", iDialogType );
    _DDPRINT( 4, "PhSrv.MsgClose.aBtn=", aButton );
    
    TInt err = KErrNone;
    // Click Yes on Confirmation note (Yes, No) 
    if ( EYesNo == iDialogType && 
          ( CHbDeviceMessageBoxSymbian::EAcceptButton == aButton ) )
        {
        _DPRINT( 4, "PhSrv.MsgClose.SK.Yes" );
        iClearArray = EFalse;
        iNotifyArray->Reset();
        TryCloseSession();
        }
    // Click "No" on Confirmation note (Yes, No) 
    else if ( EYesNo == iDialogType && 
              ( CHbDeviceMessageBoxSymbian::ERejectButton == aButton ) )
        {
        _DPRINT( 4, "PhSrv.MsgClose.SK.No" );
        iClearArray = EFalse;
        iNotifyMessage = ETrue; // for removing the yes/no query
        CheckArray();
        TryCloseSession();
        }
    // Click "Next" on Notification note (Next, Exit) 
    else if ( ENextExit == iDialogType && 
               ( CHbDeviceMessageBoxSymbian::EAcceptButton == aButton ) )
        {
        _DPRINT( 4, "PhSrv.MsgClose.SK.Next" ); 
        CheckArray();
        TryCloseSession();
        }
    // Click "Exit" on Notification note (Next, Exit or only Exit) 
    else if ( (EOnlyExit == iDialogType 
            || ENextExit == iDialogType 
            || EReplyExit == iDialogType ) && 
               ( CHbDeviceMessageBoxSymbian::ERejectButton == aButton ) )
        {
        TRAP( err, ClearArrayL() );
        _DDPRINT( 4, "PhSrv.MsgClose.SK.Clear.%d", err );
        TryCloseSession();
        _DPRINT( 4, "PhSrv.MsgClose.SK.Exit" ); 
        }
    // Click "Reply" on Message note (Reply, Exit) 
    else if ( EReplyExit == iDialogType && 
               ( CHbDeviceMessageBoxSymbian::EAcceptButton == aButton ) )
        {
        // Answer
        iStartEditor = ETrue;
        iShowDone = EFalse;
        // Start the USSD editor now.
        TRAP( err, RequestStartEditingL() );
        _DDPRINT( 4, "PhSrv.MsgClose.RequestStartEditingL.%d", err );
        }    
    else 
        {
        _DPRINT( 4, "PhSrv.MsgClose.SK.Default" ); 
        }
  
    _DPRINT( 4, "PhSrv.MsgClose.End" ); 
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::RunL
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::RunL()
    {
    _DPRINT( 4, "PhSrv.RunL.Start" );    

    ProcessMoAcksL();
    
    if ( iLaunchGMQ )
        {
        LaunchGlobalMessageQueryL();
        // Do not launch Global Message Query next time.
        iLaunchGMQ = EFalse;
        _DPRINT( 4, "PhSrv.iLaunchGMQ.EFalse" );
        }
    else
        {
        iStartEditor = EFalse;
        // update device dialog
        _DPRINT( 4, "PhSrv.RunL.End" );     // debug print
        }
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::LaunchGlobalMessageQueryL
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::LaunchGlobalMessageQueryL()
    {
    _DPRINT( 4, "PhSrv.LGMQ.start" );
    _DPRINT_FLAGS();

    if ( iNotifyMessage )
        {
        _DDPRINT( 4, "PhSrv.LGMQ.NotifyMessage", iNotifyMessage );
        iNotifyMessage = ETrue;
        TInt count = NotifyCount();
        //check softkey in avkon.rss 
        if ( count > 1 )
            {
            // Next, Exit
            iDeviceDialog->SetButton(
                CHbDeviceMessageBoxSymbian::EAcceptButton, ETrue );            
            iDeviceDialog->SetButtonTextL(
                CHbDeviceMessageBoxSymbian::EAcceptButton, 
                LoadDefaultString( KUssdNext ) );
            iDeviceDialog->SetButton(
                CHbDeviceMessageBoxSymbian::ERejectButton, ETrue );    
            iDeviceDialog->SetButtonTextL(
                CHbDeviceMessageBoxSymbian::ERejectButton, 
                LoadDefaultString( KUssdExit ) );  
            _DPRINT( 4, "PhSrv.LGMQ.Next&Exit" );
            iDialogType = ENextExit;
            }
        else
            {
            // Only Exit
            iDeviceDialog->SetButton(
                CHbDeviceMessageBoxSymbian::EAcceptButton, EFalse ); 
            iDeviceDialog->SetButton(
                CHbDeviceMessageBoxSymbian::ERejectButton, ETrue );               
            iDeviceDialog->SetButtonTextL(
                CHbDeviceMessageBoxSymbian::ERejectButton, 
                LoadDefaultString( KUssdExit ) );   
            _DPRINT( 4, "PhSrv.LGMQ.onlyExit" );
            iDialogType = EOnlyExit;
            }
        if ( iClearArray )
            {
            HbTextResolverSymbian::Init( KCommonLocFilename, KLocPath );
            _DPRINT( 4, "PhSrv.LGMQ.use common loc file" );
            // Yes, No
            iDeviceDialog->SetStandardButtons(KHbMsgBoxBtnYes
                |KHbMsgBoxBtnNo);
            iDeviceDialog->SetButtonTextL(
                CHbDeviceMessageBoxSymbian::EAcceptButton, 
                LoadDefaultString( KUssdYes ) );
            iDeviceDialog->SetButtonTextL(
                CHbDeviceMessageBoxSymbian::ERejectButton, 
                LoadDefaultString( KUssdNo ) );  
            _DPRINT( 4, "PhSrv.LGMQ.Yes&No" );
            HbTextResolverSymbian::Init( KUssdLocFilename, KLocPath );
            _DPRINT( 4, "PhSrv.LGMQ.back up to use ussd loc file" );
            iDialogType = EYesNo;
            }
        iReceivedMessage.Zero();
        iReceivedMessage = (*iNotifyArray)[0];
        }
    TurnLightsOn(); //Ensure lights on

    _DPRINT( 4, "PhSrv.LGMQ.ShMsgQuery" ); // debug print
    // Launch Global MessageQuery as requested.
    // Dialog not support header text, this code is only 
    // for testing, not final solution.
    TInt receiveLength = iReceivedMessage.Length();
    _DDPRINT( 4, "PhSrv.LGMQ.ShMsgQuery.MsgLength", receiveLength ); 
    
    TInt titleLength = LoadDefaultString( KUssdTitle ).Length();
    _DDPRINT( 4, "PhSrv.LGMQ.ShMsgQuery.TilteLength", titleLength );
    
    TInt length = receiveLength + titleLength;
    _DDPRINT( 4, "PhSrv.LGMQ.ShMsgQuery.TotalLength", length );    
        
    HBufC* titleAndText = HBufC::NewLC( length );
    titleAndText->Des().Append( LoadDefaultString( KUssdTitle ) );
    titleAndText->Des().Append( iReceivedMessage );
    iDeviceDialog->SetTextL( titleAndText->Des() );
    CleanupStack::Pop( titleAndText );
    
    iDeviceDialog->Close();
    iDeviceDialog->ShowL();
    iShowDone = EFalse;
   
    // Do nothing else in RunL this time.
    _DPRINT( 4, "PhSrv.LGMQ.ShMsgQuery.ret" ); // debug print
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::ProcessMoAcksL()
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::ProcessMoAcksL()
    {
    //Acknowledgment is sent if notify message
    if ( iNotifyMessage && iAcksToBeSent )
        {
        _DPRINT( 4, "PhSrv.LGMQ.SendMoAcknowledgementL, start" );
        _DPRINT_FLAGS();
        if ( iUssdSendHandler && !iUssdSendHandler->IsActive() &&
             !iNetworkReleased && !iSendRelease && !iSendingAck )
            {
            _DPRINT( 4, "PhSrv.LGMQ.MoAckNeededToBeSent" );
            SendMoAcknowledgementL();
            iAcksToBeSent--;
            iSendingAck = ETrue;
            }
        _DPRINT( 4, "PhSrv.LGMQ.SendMoAcknowledgementL, end" );
        }
    }
 
// -----------------------------------------------------------------------------
// CPhSrvUssdManager::AsyncProcessMoAcks()
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::AsyncProcessMoAcks()
    {
    if ( iMoAckCallback ) 
        {
        iMoAckCallback->CallBack();
        _DPRINT( 4, "PhSrv.LGMQ.AsyncProcessMoAcksL, queued" );
        }
    }
// -----------------------------------------------------------------------------
// CPhSrvUssdManager::MoAckCallback()
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdManager::MoAckCallback( TAny *aUssdManager )
    {
    _DPRINT( 4, "PhSrv.MoAckCallback.start" );
    if ( aUssdManager )
        {
        CPhSrvUssdManager* manager = static_cast<CPhSrvUssdManager*>( aUssdManager );
        TRAP_IGNORE( manager->ProcessMoAcksL() );
        }
    _DPRINT( 4, "PhSrv.MoAckCallback.end" );
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::TryCloseSession()
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::TryCloseSession() 
    {
    // Dismiss, cancel the session.
    _DPRINT( 4, "PhSrv.CheckAndCloseSession.close" );  // debug print
       
    iShowDone = EFalse;
    iEmptyEditor = EFalse;
    
    // If the notification array is empty, close the session.
    if ( NotifyCount() == 0 && !iNetworkReleased )
        {
        CloseSession();
        }
    }
// -----------------------------------------------------------------------------
// CPhSrvUssdManager::CloseSession()
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::CloseSession()
    {
    iSendRelease = ETrue;
    iClearArray = EFalse;
    iAcksToBeSent = 0;
    _DPRINT( 4, "PhSrv.CheckAndCloseSession.canceling" ); 
    SendUssdCancel();
    Cancel();
        
    // Terminates USSD session.
    if ( !iNetworkReleased ) 
        {
        iUssdSendHandler->SendReleaseSession();
        }
    if ( iUssdReplyTimer )
        {
        _DPRINT( 4, "PhSrv.CheckAndCloseSession.Timer.Stop" );
        iUssdReplyTimer->Stop();
        }
    }
 
// -----------------------------------------------------------------------------
// CPhSrvUssdManager::CheckArray()
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::CheckArray()
    {
    _DPRINT( 4, "PhSrv.CheckArray.Start" );     // debug print
    if (iNotifyArray  && NotifyCount() > 0)
        {
        if  ( !iNotifyMessage && !iClearArray )
            {
            iLaunchGMQ = ETrue;
            iNotifyMessage = ETrue;
            _DPRINT( 4, "PhSrv.CheckArray.iNotifyMessage.ETrue" );
            iTimer.After( iStatus , KPhSrvUssdMessageQueryInterval );
            _DDPRINT( 4, "PhSrv.CheckArray.SetActive.NoNotifyMessage ", iStatus.Int() );
            SetActive();
            }
        else if( NotifyCount() > 1 )
            {
            ( iNotifyArray )->Delete( 0 );
            ( iNotifyArray )->Compress();
            iLaunchGMQ = ETrue;
            iNotifyMessage = ETrue;
            _DPRINT( 4, "PhSrv.CheckArray.iNotifyMessage.ETrue" );
            iTimer.After( iStatus , KPhSrvUssdMessageQueryInterval );
            _DDPRINT( 4, "PhSrv.CheckArray.SetActive.NotifyCount>1 ", iStatus.Int() );
            SetActive();
            }
        else
            {
            iNotifyArray->Reset();
            iNotifyMessage = EFalse;
            _DPRINT( 4, "PhSrv.CheckArray.iNotifyMessage.EFalse" );
            }
        }
    _DPRINT_FLAGS();
    _DPRINT( 4, "PhSrv.CheckArray.End" );     // debug print
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::ClearArray()
// -----------------------------------------------------------------------------
//
  void CPhSrvUssdManager::ClearArrayL()
    {
    _DPRINT( 4, "PhSrv.ClearArrayL.Start" );     // debug print
    if (iNotifyArray && NotifyCount() > 0)
        {
        if (iNotifyMessage && NotifyCount()== 1 )
            {
            iNotifyArray->Reset();
            }
        else
            {
            iReceivedMessage.Zero();
            iReceivedMessage.Append( 
                LoadDefaultString( KUssdConfirm ) );
            iNotifyArray->InsertL( 0, iReceivedMessage );
            iLaunchGMQ = ETrue;
            iNotifyMessage = ETrue;
            _DPRINT( 4, "PhSrv.ClearArrayL.iNotifyMessage.ETrue" );
            iClearArray = ETrue;
            iTimer.After( iStatus , KPhSrvUssdMessageQueryInterval );
            _DDPRINT( 4, "PhSrv.ClearArrayL.iTimer ", iStatus.Int() );
            SetActive();
            }
        }
    _DPRINT_FLAGS();
    _DPRINT( 4, "PhSrv.ClearArrayL.End" );     // debug print
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::NotifyCount()
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdManager:: NotifyCount()
{
    TInt count = iNotifyArray->Count();
    _DDPRINT( 4, "PhSrv.NotifyCount:", count );     // debug print
    return count;
}

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::UpdateNotifyMessageL()
// -----------------------------------------------------------------------------
//
  void CPhSrvUssdManager:: UpdateNotifyMessageL()
    {
    _DDPRINT( 4, "PhSrv.UpdateNotifyMessageL.Start, clear: ", iClearArray );     // debug print

    // Show left softkey - "Next"
    if (NotifyCount() > 1 && !iClearArray )
        {
        _DPRINT( 4, "PhSrv.UpdateNotifyMessageL" );     // debug print
        iDeviceDialog->SetButton( 
            CHbDeviceMessageBoxSymbian::EAcceptButton, ETrue ); 
        iDeviceDialog->SetButtonTextL(
            CHbDeviceMessageBoxSymbian::EAcceptButton, 
            LoadDefaultString( KUssdNext ) );
        iDialogType = ENextExit;
        }
    // Remove left softkey
    else
        {
        iDeviceDialog->SetButton( 
                    CHbDeviceMessageBoxSymbian::EAcceptButton, EFalse );         
        }
    iDeviceDialog->UpdateL();        
    
    _DPRINT( 4, "PhSrv.UpdateNotifyMessageL.End" );     // debug print
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::DoCancel
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::DoCancel()
    {
    _DPRINT( 4, "PhSrv.DoCancel.Start" ); // debug print
    iTimer.Cancel();
    iLaunchGMQ = EFalse;
    if ( iDeviceDialog )
        {
        _DPRINT( 4, "PhSrv.DoCancel" ); // debug print
        iDeviceDialog->Close();
        delete iDeviceDialog;
        iDeviceDialog = NULL;
        }
    _DPRINT( 4, "PhSrv.DoCancel.End" ); // debug print
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::RunError
//
// Because the RunL can leave, this function exists.
// In case of error, just returning KErrNone is OK.
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdManager::RunError( TInt /*aError*/ )
    {
    _DPRINT( 4, "PhSrv.RunError.Start" );     // debug print
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::RequestStartEditingL
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::RequestStartEditingL()
    {
    _DPRINT( 4, "PhSrv.UssdM.RequestStartEditingL.start" ); // debug print

    RWsSession wsSession;
    User::LeaveIfError( wsSession.Connect() );
    _DPRINT( 4, "PhSrv.RequestStartEditingL.wsSession.connect" );     // debug print
    CleanupClosePushL( wsSession );

    // Find the task with name
    TApaTaskList tasklist( wsSession );
    TApaTask task = tasklist.FindApp( KUssdSecureId );

    // If task exists, bring it to foreground
    if ( task.Exists() )
        {
        _DPRINT( 4, "PhSrv.UssdM.RequestStartEditingL.task.BringToForeground" );
        task.BringToForeground();
        }
    else
        {
        // Start new app
        RApaLsSession apaLsSession;
        User::LeaveIfError( apaLsSession.Connect() );
        _DPRINT( 4, "PhSrv.RequestStartEditingL.apaLsSession.connect" );     // debug print
        CleanupClosePushL( apaLsSession );

        TApaAppInfo appInfo;
        
        TInt err = apaLsSession.GetAppInfo( appInfo, KUssdSecureId );
        _DDPRINT( 4, "PhSrv.RequestStartEditingL.GetAppInfo ", err );     // debug print
        
        if ( err == KErrNone )
            {
        #ifndef SYMBIAN_SUPPORT_UI_FRAMEWORKS_V1
            CApaCommandLine* apaCommandLine = CApaCommandLine::NewLC();
            apaCommandLine->SetExecutableNameL( appInfo.iFullName );
        #else // SYMBIAN_SUPPORT_UI_FRAMEWORKS_V1
            CApaCommandLine* apaCommandLine =
                CApaCommandLine::NewLC( appInfo.iFullName );
        #endif // SYMBIAN_SUPPORT_UI_FRAMEWORKS_V1

            TThreadId id( static_cast<TInt64>( 0 ) );
            err = apaLsSession.StartApp( *apaCommandLine, id );
            _DDPRINT( 4, "PhSrv.RequestStartEditingL.ThreadId ", id ); 
            CleanupStack::PopAndDestroy( apaCommandLine );
            }
        CleanupStack::PopAndDestroy( &apaLsSession ); // apaLsSession
        
        // bring the ussd editor to foreground, only for testing
        TApaTaskList tasklist( wsSession );
        TApaTask task = tasklist.FindApp( KUssdSecureId );
        if ( task.Exists() )
            {
            _DPRINT( 4, "PhSrv.UssdM.RequestStartEditingL.task.BringToForeground" );
            task.BringToForeground();
            }
        // bring the ussd editor to foreground, only for testing
        }
    CleanupStack::PopAndDestroy(); // wsSession

    _DPRINT( 4, "PhSrv.UssdM.RequestStartEditingL.end" ); // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::InformUssdApplicationStarting
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::InformUssdApplicationStarting()
    {
    _DPRINT( 4, "PhSrv.UssdM.InfUssdAppStarting.start" ); // debug print
    if ( iUssdReplyTimer )
        {
        _DPRINT( 4, "PhSrv.InformUssdApplicationStarting.iUssdReplyTimer" );
        if ( iUssdReplyTimer->IsTimerActive() )
            {
            _DPRINT( 4, "PhSrv.InformUssdApplicationStarting.iUssdReplyTimer.active" );
            iUssdReplyTimer->Pause();
            }
        }
    _DPRINT( 4, "PhSrv.UssdM.InfUssdAppStarting.end" ); // debug print
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::InformUssdApplicationTerminatingL
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::InformUssdApplicationTerminatingL(
    const RMessage2& aMessage )
    {
    _DPRINT( 4, "PhSrv.UssdM.InfUssdAppTerminatingL.start" ); // debug print
    if ( iUssdReplyTimer )
        {
        if ( iUssdReplyTimer->IsTimerActive() )
            {
     
            _DPRINT( 4, "PhSrv.UssdM.InfUssdAppTerminatingL.timer" );

            // Read the information what is the reason
            // for application termination.
            TPhCltUssdAppExitReason exitReason = EPhCltExitReasonUnknown;
            TPckg< TPhCltUssdAppExitReason > exitReasonPckg( exitReason );
            aMessage.ReadL(
                0,
                exitReasonPckg );

     
            _DPRINT( 4, "PhSrv.UssdM.InfUssdAppTerminatingL.timer2" );

            // If reason was the completion of send operation, the USSD
            // session is not canceled, otherwise it is canceled.
            if ( exitReason != EPhCltSendCompleted )
                {
         
                _DPRINT( 4, "PhSrv.UssdM.InfUssdAppTerminatingL.SendRelease" );
                CloseSession();
                }
            }
        }
    _DPRINT( 4, "PhSrv.UssdM.InfUssdAppTerminatingL.end" ); // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::InformUssdApplicationToForeground
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdManager::InformUssdApplicationToForeground()
    {
    _DPRINT( 4, "PhSrv.UssdM.InfUssdAppToForeground.start" ); // debug print

    // Do the same actions as in InformUssdApplicationStarting,
    // that is why it is called.
    InformUssdApplicationStarting();

    if ( iEmptyEditor )
        {
        _DPRINT( 4, "PhSrv.UssdM.InfUssdAppToForeground.empty" ); // debug print
        iEmptyEditor = EFalse;
        return 1; // some positive number
        }
    _DPRINT( 4, "PhSrv.UssdM.InfUssdAppToForeground.end" ); // debug print
    return KErrNone;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::InformUssdApplicationToBackground
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::InformUssdApplicationToBackground()
    {
    _DPRINT( 4, "PhSrv.UssdM.InfUssdAppToBackground.start" ); // debug print
    if ( iUssdReplyTimer )
        {
        _DPRINT( 4, "PhSrv.UssdM.InfUssdAppToBackground.timer" ); // debug print
        if ( iUssdReplyTimer->IsTimerActive() )
            {
            _DPRINT( 4, "PhSrv.UssdM.InfUssdAppToBackground.timer.Active" ); // debug print
            iUssdReplyTimer->Continue();
            }
        }
    _DPRINT( 4, "PhSrv.UssdM.InfUssdAppToBackground.end" ); // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::InformStartSAT
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::InformStartSAT( const RMessage2& aSatMessage )
    {
    _DPRINT( 4, "PhSrv.InformStartSAT.Start" );    // debug print

    if ( iHavePendingSatMessagePointer )
        {
        _DPRINT( 4, "PhSrv.InformStartSAT.Complete.InUse" ); // debug print
        aSatMessage.Complete( KErrInUse );
        }
    else
        {
        _DPRINT( 4, "PhSrv.InformStartSAT.Set" );    

        // There was not pending SAT message
        iHavePendingSatMessagePointer = ETrue;
        _DDPRINT( 4, "PhSrv.UssdM.InformStartSAT.PendingSatPointer ", iHavePendingSatMessagePointer );
        iPendingSatMessagePointer = aSatMessage;
        }

    _DPRINT( 4, "PhSrv.InformStartSAT.End" );   // debug print
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::InformStopSAT
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::InformStopSAT()
    {
    _DPRINT( 4, "PhSrv.InformStopSAT.Start" );        

    // Do the actions only if there is pending SAT message.
    if ( iHavePendingSatMessagePointer )
        {
        _DPRINT( 4, "PhSrv.InformStopSAT.Complete.Cancel" ); // debug print
        if ( !iPendingSatMessagePointer.IsNull() )
            {
            iPendingSatMessagePointer.Complete( KErrCancel );
            _DPRINT( 4, "PhSrv.InformStopSAT.Complete.KErrCancel" );
            }
        iHavePendingSatMessagePointer = EFalse;
        }
    _DPRINT( 4, "PhSrv.InformStopSAT.End" );          
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::ShowNotes
// -----------------------------------------------------------------------------
//


TBool CPhSrvUssdManager::ShowNotesL()
{
_DPRINT( 4, "PhSrv.ShowNotesL.Start" );

if ( !iHavePendingSatMessagePointer )
    {
    if ( iSatCanceled )
        {
        _DPRINT( 4, "PhSrv.ShowNotesL, EFalse" );  // debug print
        return EFalse;
        }
    else
        {
        _DPRINT( 4, "PhSrv.ShowNotesL, ETrue" );  // debug print
        return ETrue;
        }
    }
// Read the information whether the notes are shown or not.
    TUint notesShown = 0;
    TPckg< TUint > noShPckg( notesShown );
    iPendingSatMessagePointer.ReadL(
        0,
        noShPckg );

    TBool showNotes = EFalse;
    _DPRINT( 4, "PhSrv.ShowNotes Sat= EFalse" );    // debug print
    if ( notesShown == KPhCltShowNotes )
        {
        _DPRINT( 4, "PhSrv.ShowNotes Sat= ETrue" ); // debug print
        showNotes = ETrue;
        }
    _DPRINT( 4, "PhSrv.ShowNotesL.End" );
    return showNotes;
}

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::CompleteSatL
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::CompleteSatL(
    TDesC* aReceiveString,
    TInt aError )
    {
    _DPRINT( 4, "PhSrv.CompleteSatL.Start" );         
    if ( aReceiveString )
        {
        if ( aReceiveString->Length() )
            {
            _DPRINT( 4, "PhSrv.CompleteSatL.recString.>0" ); // debug print
            // copy the received string to client side.
            if ( iPendingSatMessagePointer.Int1() < aReceiveString->Length() )
                {
         
                _DPRINT( 4, "PhSrv.CompleteSatL.recString.LengthError" );
                if ( !iPendingSatMessagePointer.IsNull() )
                    {
                    iPendingSatMessagePointer.Complete( KErrOverflow );
                    _DPRINT( 4, "PhSrv.CompleteSatLComplete.KErrOverFlow" );
                    }
                iHavePendingSatMessagePointer = EFalse;
                }
            else
                {
            _DPRINT( 4, "PhSrv.CompleteSatL.recString.Write" );  // debug print
            iPendingSatMessagePointer.WriteL(
                2,
                *aReceiveString );

            _DPRINT( 4, "PhSrv.CompleteSatL.recString.Write2" ); // debug print
            TPckg< TUint > dcsPackage( iDCS );
            iPendingSatMessagePointer.WriteL(
                0,
                dcsPackage );
            }
        }
        }
    _DPRINT( 4, "PhSrv.CompleteSatL.Middle" );    
    if ( !iPendingSatMessagePointer.IsNull() )
        {
        if ( aReceiveString && !iSatCanceled )
            {
            _DDPRINT( 4, "PhSrv.CompleteSatL.Complete.", iReturnResult.iOpCode  );   // debug print
            iPendingSatMessagePointer.Complete( iReturnResult.iOpCode ); // Some positive value
            }
        else
            {
            _DPRINT( 4, "PhSrv.CompleteSatL.Complete.aError" ); // debug print
            iPendingSatMessagePointer.Complete( aError );
            }
        }

    iHavePendingSatMessagePointer = EFalse;

    _DPRINT( 4, "PhSrv.CompleteSatL.End" );    
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::IsTelephonyFeatureSupported
//
// -----------------------------------------------------------------------------
//
TBool CPhSrvUssdManager::IsTelephonyFeatureSupported(
    const TInt aFeatureId )
    {
    return ( aFeatureId & iVariantReadOnlyValues );
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::PlayUssdTone
//
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdManager::PlayUssdTone()
    {
    _DPRINT( 4, "PhSrv.UssdM.PlayTone.start" );    

    TInt err = KErrNone;
// <-- QT PHONE  START-->
//    RProperty::Define( KPSUidNcnList, KNcnPlayAlertTone, RProperty::EInt, ECapability_None , ECapabilityWriteDeviceData );
//    RProperty::Set( KPSUidNcnList, KNcnPlayAlertTone, KPhSrvUssdTone );
// <-- QT PHONE END-->

    // debug print
    _DDPRINT(
        4,
        "PhSrv.UssdM.PlayTone.end",
        err );
    return err;
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::GetTelephonyVariantData
//
// -----------------------------------------------------------------------------
//
TInt CPhSrvUssdManager::GetTelephonyVariantData()
    {
    _DPRINT( 4, "PhSrv.UssdM.GetTelephonyVariantData.Start" );
    TInt err = KErrNone;
// <-- QT PHONE START-->
/*
    // Variation data should be unchangable during run-time,
    // therefore, if once succesfully read, later reads are
    // not allowed.
    if ( iVariantReadOnlyValues == KPhSrvDefaultValue )
        {
        CRepository* cenRepSession = NULL;
        TRAP ( err ,
               cenRepSession = CRepository::NewL( KCRUidTelVariation ) );
        if ( err == KErrNone )
            {
            err = cenRepSession->Get( KTelVariationFlags,
                                   iVariantReadOnlyValues );
            }
        delete cenRepSession;
        }

    _DDPRINT( 4, "PhSrv.UssdM.variant", iVariantReadOnlyValues ); // debug print
    _DPRINT( 4, "PhSrv.UssdM.GetTelephonyVariantData.End" );
    */
// <-- QT PHONE END-->
    return err;
    }


// -----------------------------------------------------------------------------
// CPhSrvUssdManager::SendMoAcknowledgementL
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::SendMoAcknowledgementL()
    {
    _DPRINT( 4, "PhSrv.SendMoAckL.Start" );    

    // Acknowledge MT USSD message.
    RMobileUssdMessaging::TMobileUssdAttributesV1 msgAttribs;
    msgAttribs.iFlags =
        RMobileUssdMessaging::KUssdMessageType +
        RMobileUssdMessaging::KUssdDataFormat  +
        RMobileUssdMessaging::KUssdMessageDcs;

    msgAttribs.iType   = RMobileUssdMessaging::EUssdMOAcknowledgement;
    _DDPRINT( 4, "PhSrv.SendMoAckL.msgAttribs.iType ", msgAttribs.iType );
    msgAttribs.iFormat = RMobileUssdMessaging::EFormatUnspecified;
    _DDPRINT( 4, "PhSrv.SendMoAckL.msgAttribs.iFormat ", msgAttribs.iFormat );
    msgAttribs.iDcs    = KPhSrvUssdDefaultDCS;

    RMobileUssdMessaging::TMobileUssdAttributesV1Pckg attribs = msgAttribs;
    SendHandlerL().SendUssdL( KNullDesC8() , attribs );

    _DPRINT( 4, "PhSrv.SendMoAckL.End" );    
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::TurnLightsOn
//
// -----------------------------------------------------------------------------
//
void CPhSrvUssdManager::TurnLightsOn()
    {
    _DPRINT( 4, "PhSrv.TurnLightsOn.Start" );    


     // Change the bit on and off. SysAp will detect that
     // the lights should be switched on for the specified time.
     //
     RProperty::Set(KPSUidCoreApplicationUIs, KLightsControl, ELightsOn);
     TInt err = RProperty::Set(KPSUidCoreApplicationUIs, KLightsControl, ELightsOff);

     if ( err != KErrNone )
         {
         _DDPRINT( 4,"PhSrv.TurnLightsOn.Error: ",err );// debug print
         }

    _DPRINT( 4, "PhSrv.TurnLightsOn.End" );    
    }

// -----------------------------------------------------------------------------
// CPhSrvUssdManager::LoadDefaultString
// -----------------------------------------------------------------------------
//
const TPtrC CPhSrvUssdManager::LoadDefaultString( const TDesC& aText )
    {
    _DPRINT( 4, "PhSrv.LoadDefaultString.Start" );
    
    if ( iTextBuffer )
        {
        delete iTextBuffer;
        iTextBuffer = NULL;
        _DPRINT( 4, "PhSrv.LoadDefaultString.Clear" );
        }
    TInt err = KErrNone;
    TPtrC ptr( aText );
    if ( ptr.Length() )
        {
        TRAP( err, iTextBuffer = HbTextResolverSymbian::LoadL( ptr ) );
        _DDPRINT( 4, "PhSrv.LoadDefaultString.LoadL.%d", err );
        if ( iTextBuffer )
            {
            ptr.Set( iTextBuffer->Des() );   
            _DPRINT( 4, "PhSrv.LoadDefaultString.Set" );
            }
        }    
    _DPRINT( 4, "PhSrv.LoadDefaultString.End" );
    return ptr;
    }

// End of File
