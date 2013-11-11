/*
* Copyright (c) 2007-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Encapsule the access to etelmm API in sat
*
*/


#include    <mmtsy_names.h>
#include    "csatmultimodeapi.h"
#include    "csatsactivewrapper.h"
#include    "SatLog.h"

#ifdef SAT_USE_DUMMY_TSY
    _LIT( KSatSDummyTsyModuleName, "DSAT" );
    _LIT( KSatSDummyTsyPhoneName, "DMobile" );
#endif

const TInt KLoopMaxTryouts = 5;
const TInt KLoopTimeout = 3000000;

const TUint8 KQuarterShift( 2 );
const TUint8 KHighNibbleShift( 4 );
const TUint8 KDcsCharacterSet7Bit( 0x00 );
const TUint8 KDcsCharacterSet8Bit( 0x01 );
const TUint8 KDcsCharacterSet16Bit( 0x02 );
const TUint8 KDcsCharacterSet7Bit2( 0x00 );
const TUint8 KDcsCharacterSet16Bit2( 0x01 );

// ================= MEMBER FUNCTIONS ==========================================

// -----------------------------------------------------------------------------
// Class constructor.
// -----------------------------------------------------------------------------
//
CSatMultiModeApi::CSatMultiModeApi()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::CSatMultiModeApi calling-exiting" )
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CSatMultiModeApi::~CSatMultiModeApi()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::~CSatMultiModeApi calling" )

    if ( iCallOpened )
        {
        iCall.Close();
        }
    
    if ( iLineOpened )
        {
        iLine.Close();
        }
    
    if ( iUssdOpened )
        {
        iUssd.Close();
        }
        
    iCustomPhone.Close();
    
    iPhone.Close();

#ifdef SAT_USE_DUMMY_TSY
    iDummyPhone.Close();
#endif
    iTelServer.Close();
            
    delete iWrapper;
    iWrapper = NULL;

    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::~CSatMultiModeApi calling" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::NewL
// -----------------------------------------------------------------------------
//
CSatMultiModeApi* CSatMultiModeApi::NewL()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::NewL calling" )

    CSatMultiModeApi* self = new( ELeave )CSatMultiModeApi();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::NewL calling" )
    return self;
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::Phone
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RMobilePhone* CSatMultiModeApi::Phone()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::Phone calling-exiting" )
    return &iPhone;
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::Phone
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RMobilePhone* CSatMultiModeApi::DummyPhone()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::Phone calling-exiting" )
#ifdef SAT_USE_DUMMY_TSY
    return &iDummyPhone;
#else
    return NULL;
#endif
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::CustomApi
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RMmCustomAPI* CSatMultiModeApi::CustomApi()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::CustomApi calling-exiting" )
    return &iCustomPhone;
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::LowerErrorGranularity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::LowerErrorGranularity()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::LowerErrorGranularity \
        calling-exiting" )
    iTelServer.SetExtendedErrorGranularity( RTelServer::EErrorBasic );
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::RaiseErrorGranularity
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::RaiseErrorGranularity()
    {
    LOG( NORMAL, "SATENGINE: CSatMultiModeApi::RaiseErrorGranularity \
        calling-exiting" )
    iTelServer.SetExtendedErrorGranularity( RTelServer::EErrorExtended );
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::GetNetworkRegistrationStatus
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::GetNetworkRegistrationStatus( 
        TRequestStatus& aReqStatus, 
        RMobilePhone::TMobilePhoneRegistrationStatus& aStatus)
    {
    LOG( SIMPLE, 
    "SATENGINE: CSatMultiModeApi::GetNetworkRegistrationStatus calling" )

    iPhone.GetNetworkRegistrationStatus( aReqStatus, aStatus );

    LOG( SIMPLE,
     "SATENGINE: CSatMultiModeApi::GetNetworkRegistrationStatus exiting" )
    }
// -----------------------------------------------------------------------------
// CSatMultiModeApi::NotifyNetworkRegistrationStatusChange
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::NotifyNetworkRegistrationStatusChange(
        TRequestStatus& aReqStatus, 
        RMobilePhone::TMobilePhoneRegistrationStatus& aStatus)
    {    
    LOG( SIMPLE, "SATENGINE: \
    CSatMultiModeApi::NotifyNetworkRegistrationStatusChange calling" )
    
    iPhone.NotifyNetworkRegistrationStatusChange( aReqStatus, aStatus );
    
    LOG( SIMPLE, "SATENGINE: \
    CSatMultiModeApi::NotifyNetworkRegistrationStatusChange exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::GetSubscriberId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::GetSubscriberId(TRequestStatus& aReqStatus, 
        RMobilePhone::TMobilePhoneSubscriberId& aId)
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::GetSubscriberId calling" )
    iPhone.GetSubscriberId( aReqStatus, aId );
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::GetSubscriberId exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::SendDTMFTones
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::SendDTMFTones( TRequestStatus& aReqStatus, 
        const TDesC& aTones)
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::SendDTMFTones calling" )
    iPhone.SendDTMFTones( aReqStatus, aTones );
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::SendDTMFTones exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::ContinueDTMFStringSending
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatMultiModeApi::ContinueDTMFStringSending( TBool aContinue )
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::ContinueDTMFStringSending" )
    return iPhone.ContinueDTMFStringSending( aContinue );
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::DialNoFdnCheck
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::DialNoFdnCheck( TRequestStatus& aStatus,
        const TDesC8& aCallParams, const TDesC& aTelNumber)
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::DialNoFdnCheck calling" )
    TInt err = LoadMobileCall();
    LOG2( SIMPLE, "SATENGINE: CSatMultiModeApi::DialNoFdnCheck err %d", err )
    
    iCall.DialNoFdnCheck( aStatus, aCallParams, aTelNumber );
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::DialNoFdnCheck exiting" )
    }
                        
// -----------------------------------------------------------------------------
// CSatMultiModeApi::SendNetworkServiceRequestNoFdnCheck
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::SendNetworkServiceRequestNoFdnCheck( 
        TRequestStatus& aReqStatus, const TDesC& aServiceString)
                       
    {
    LOG( SIMPLE, "SATENGINE: \
            CSatMultiModeApi::SendNetworkServiceRequestNoFdnCheck calling" )
    
    iPhone.SendNetworkServiceRequestNoFdnCheck( aReqStatus, aServiceString );
    
    LOG( SIMPLE, "SATENGINE: \
            CSatMultiModeApi::SendNetworkServiceRequestNoFdnCheck exiting" )
    }
                                
// -----------------------------------------------------------------------------
// CSatMultiModeApi::SendMessageNoFdnCheck
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::SendMessageNoFdnCheck( TRequestStatus& aReqStatus,
            const TDesC8& aMsgData, const TDesC8& aMsgAttributes )
    {
    LOG( SIMPLE,
        "SATENGINE: CSatMultiModeApi::SendMessageNoFdnCheck calling" )
    
    iUssd.SendMessageNoFdnCheck( aReqStatus, aMsgData, aMsgAttributes );
    
    LOG( SIMPLE, 
        "SATENGINE: CSatMultiModeApi::SendMessageNoFdnCheck exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::CancelAsyncRequest
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::CancelAsyncRequest(TInt aReqToCancel)
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::CancelAsyncRequest calling" )
    iPhone.CancelAsyncRequest( aReqToCancel );
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::CancelAsyncRequest exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::DialCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::DialCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::DialCancel calling" )
    iCall.DialCancel();
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::DialCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::IsCallIncoming
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CSatMultiModeApi::IsCallIncoming()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::IsCallIncoming entering" )
    TInt lines( 0 );
    TBool callIncoming( EFalse );

    // Enumerate all lines in the phone
    TInt err( iPhone.EnumerateLines( lines ) );

    if ( KErrNone == err )
        {
        RPhone::TLineInfo lineInfo;
        // Check the lines' status one by one
        for( TInt i = 0; ( i < lines ) && !callIncoming; ++i )
            {
            err = iPhone.GetLineInfo( i, lineInfo ); 
            if ( KErrNone == err )
                {
                LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::IsCallIncoming \
                    line status is %d", lineInfo.iStatus )
                if ( ( RCall::EStatusDialling == lineInfo.iStatus ) ||
                     ( RCall::EStatusRinging == lineInfo.iStatus ) )
                    {
                    // There is an incoming call
                    callIncoming = ETrue;
                    }
                }
            else
                {
                // Error happened when getting line info
                // We currently do nothing but skip the error line and log it
                LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::IsCallIncoming \
                    getting line status error %d", err )
                }
            }
        }
    else
        {
        // Enumerate lines error
        LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::IsCallIncoming \
            enumerate lines error %d", err )
        }

    LOG2( SIMPLE, "SATENGINE: CSatMultiModeApi::IsCallIncoming exiting %d", 
          callIncoming )
    return callIncoming;
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::NotifyMobileCallStatusChange
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::NotifyMobileCallStatusChange ( 
        TRequestStatus& aReqStatus, RMobileCall::TMobileCallStatus& aStatus )
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::NotifyMobileCallStatusChange \
             calling" )
    iCall.NotifyMobileCallStatusChange( aReqStatus, aStatus );
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::NotifyMobileCallStatusChange \
             exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::NotifyCallStatusChangeCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::NotifyCallStatusChangeCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::DialCancel calling" )
    iCall.NotifyStatusChangeCancel();
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::DialCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::NotifyMobileCallStatusChange
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::TerminateActiveCalls(TRequestStatus& aReqStatus)
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::TerminateAllCalls calling" )

    iPhone.TerminateActiveCalls( aReqStatus );
    
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::TerminateAllCalls exiting" )
    } 

// -----------------------------------------------------------------------------
// CSatMultiModeApi::GetMobileCallInfo
// -----------------------------------------------------------------------------
//
TInt CSatMultiModeApi::GetMobileCallInfo(TDes8& aCallInfo)
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::GetMobileCallInfo calling" )
    TInt ret = iCall.GetMobileCallInfo( aCallInfo );
    LOG2( SIMPLE, "SATENGINE: CSatMultiModeApi::GetMobileCallInfo exiting %d",
            ret )
    return ret;
    }
       
// -----------------------------------------------------------------------------
// CSatMultiModeApi::GetMobileCallInfo
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::DialEmergencyCall(TRequestStatus& aReqStatus, 
        const TDesC& aNumber)
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::DialEmergencyCall calling" )
    TInt err = LoadMobileCall();
    LOG2( SIMPLE, "SATENGINE: CSatMultiModeApi::DialEmergencyCall err %d", err )
    iCall.DialEmergencyCall( aReqStatus, aNumber );
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::DialEmergencyCall exiting" )
    }

// -----------------------------------------------------------------------------
// Check validity of a given Data Coding Scheme (Dcs).
// For the details how the DCS is validated please check TS_123038
// -----------------------------------------------------------------------------
//
TBool CSatMultiModeApi::IsValidUssdDcs( const TUint8 aDcs )
    {
    LOG2( SIMPLE, 
    "SATENGINE: CSatMultiModeApi::IsValidUssdDcs calling dcs = %x", aDcs)

    TBool isDcsValid( EFalse );
                                                               //      76543210
    TUint8 codingGroup  = ( aDcs & 0xF0 ) >> KHighNibbleShift; // bits XXXX____
    TUint8 characterSet = ( aDcs & 0x0C ) >> KQuarterShift;    // bits ____XX__
    TUint8 lowQuartet   = ( aDcs & 0x0F );                     // bits ____XXXX

    LOG2( SIMPLE, 
    "SATENGINE: CSatMultiModeApi::IsValidUssdDcs codingGroup: %x", codingGroup)
    LOG2( SIMPLE, 
    "SATENGINE: CSatMultiModeApi::IsValidUssdDcs charSet: %x", characterSet)
    LOG2( SIMPLE, 
        "SATENGINE: CSatMultiModeApi::IsValidUssdDcs lowQuartet: %x", lowQuartet)

    switch ( codingGroup )
        {
        case 0x00:
        case 0x02:
        case 0x03:
        case 0x0F:
            {
            isDcsValid = ETrue;
            break;
            }

        case 0x01:
            {
            if ( ( KDcsCharacterSet7Bit2 == lowQuartet ) ||
                 ( KDcsCharacterSet16Bit2 == lowQuartet ) )
                {
                isDcsValid = ETrue;
                }
            break;
            }

        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x09:
            {
            if ( ( KDcsCharacterSet7Bit == characterSet ) ||
                 ( KDcsCharacterSet8Bit == characterSet ) ||
                 ( KDcsCharacterSet16Bit == characterSet ) )
                {
                isDcsValid = ETrue;
                }
            break;
            }

        default:
            {
            LOG2( SIMPLE, "SATENGINE: CSatMultiModeApi::IsValidUssdDcs \
                    Reserved Dcs found: %x", aDcs )
            }
        }

    LOG2(
        SIMPLE,
        "SATENGINE: CSatMultiModeApi::IsValidUssdDcs exiting, valid = %d",
        isDcsValid )
    return isDcsValid;
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::ConstructL
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::ConstructL()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::ConstructL calling" )

    // Connect to ETel Server    
    ConnectETelServerL();
#if !defined ( __WINSCW__ )
    //On the emulator the load will leave. We can not use the functionaly
    //on enmulator
    LoadUssdMessagingL();
#endif

    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::ConstructL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::ConnectETelServerL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::ConnectETelServerL()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::ConnectETelServerL calling" )

    TInt numberOfTries( 1 );        // Indicates loop tryouts
    TBool loopSuccess( EFalse );    // Loop stopper, if this is ETrue
    TInt error = KErrNone;          // Error that is Leave'd

    if ( !iWrapper )
        {
        LOG( NORMAL, "SATENGINE: CSatMultiModeApi::ConnectETelServerL \
        iWrapper false" )
        iWrapper = new ( ELeave ) CSatSActiveWrapper();
        }

    // First loop is for connecting to RTelServer. Loop is done until
    // connection returns KErrNone or when max loop try outs has been 
    // reached
    while ( !loopSuccess && numberOfTries <= KLoopMaxTryouts )
        {
        error = iTelServer.Connect();
        if ( KErrNone == error )
            {
            LOG( NORMAL, "SATENGINE: \
            CSatMultiModeApi::ConnectETelServerL KErrNone == error" )
            loopSuccess = ETrue;
            }
        else
            {
            numberOfTries++;
            iWrapper->After( KLoopTimeout );
            }
        }

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::ConnectETelServerL \
          numberOfTries: %i", numberOfTries )

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::ConnectETelServerL \
          error: %i", error )

    // Check the error status
    User::LeaveIfError( error );

    // Now load phone module
    LoadPhoneModuleL( iPhone, KMmTsyModuleName, KMmTsyPhoneName );

#ifdef SAT_USE_DUMMY_TSY
    // Also load dummy tsy
    LoadPhoneModuleL( iDummyPhone, KSatSDummyTsyModuleName, KSatSDummyTsyPhoneName );
#endif // SAT_USE_DUMMY_TSY

    User::LeaveIfError( iCustomPhone.Open( iPhone ) );

    // No need for wrapper anymore
    delete iWrapper;
    iWrapper = NULL;

    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::ConnectETelServerL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::LoadPhoneModuleL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatMultiModeApi::LoadPhoneModuleL( RMobilePhone& aPhone,
        const TDesC& aModuleName, const TDesC& aPhoneName )
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL calling" )

    TInt numberOfTries( 1 );        // Indicates loop tryouts
    TBool loopSuccess( EFalse );    // Loop stopper, if this is ETrue
    TInt error = KErrNone;          // Error that is Leave'd

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL \
          ModuleName: %S", &aModuleName )

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL \
          PhoneName: %S", &aPhoneName )

    // We wait here until the phone module gets loaded.
    // Load the correct phone module depending on the TSY being used.
    while ( !loopSuccess && numberOfTries <= KLoopMaxTryouts )
        {
        error = iTelServer.LoadPhoneModule( aModuleName );
        if ( KErrNone == error )
            {
            loopSuccess = ETrue;
            }
        else
            {
            numberOfTries++;
            iWrapper->After( KLoopTimeout );
            }
        }

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL \
          numberOfTries: %i", numberOfTries )

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL \
          error: %i", error )

    // Check the error status
    User::LeaveIfError( error );

    // This function retrieves the total number of phones supported by all
    // the currently loaded ETel (TSY) modules.
    TInt phoneCount( 0 );
    User::LeaveIfError( iTelServer.EnumeratePhones( phoneCount ) );

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL \
          phoneCount: %i", phoneCount )

    // This function retrieves information associated with the specified phone
    RTelServer::TPhoneInfo phoneInfo;
    while ( phoneCount-- )
        {
        User::LeaveIfError( iTelServer.GetPhoneInfo( phoneCount,
            phoneInfo ) );

        // Check that do we have the right phone
        if ( phoneInfo.iName == aPhoneName )
            {
            LOG( NORMAL, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL \
            phoneInfo.iName == aPhoneName" )
            phoneCount = 0;
            }
        }

    if ( phoneInfo.iName != aPhoneName )
        {
        // Did not found correct phone info -> Leave
        LOG( SIMPLE, 
            "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL Not found" )
        User::Leave( KErrNotFound );
        }

    // Reset the counter and loop status for the next loop.
    numberOfTries = 1;
    loopSuccess = EFalse;

    // We wait here until the phone gets opened.
    // Open the correct phone depending on the TSY being used.
    while ( !loopSuccess && ( numberOfTries <= KLoopMaxTryouts ) )
        {
        error = aPhone.Open( iTelServer, aPhoneName );

        if ( KErrNone == error )
            {
            LOG( NORMAL, "SATENGINE: \
            CSatMultiModeApi::LoadPhoneModuleL KErrNone == error" )
            loopSuccess = ETrue;
            }
        else
            {
            numberOfTries++;
            iWrapper->After( KLoopTimeout );
            }
        }
    if ( !loopSuccess )
        {
        LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL\
                      phone open failed" );
        }
    
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::LoadPhoneModuleL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatMultiModeApi::LoadMobileCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatMultiModeApi::LoadMobileCall()
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::LoadMobileCallL calling" )

    if ( iCallOpened )
        {
        iCall.Close();
        }
    
    if ( iLineOpened )
        {
        iLine.Close();
        }
    
    TInt numberOfTries( 1 );        // Indicates loop tryouts
    TBool loopSuccess( EFalse );    // Loop stopper, if this is ETrue
    TInt error = KErrNone;          // Error that is Leave'd

    while ( !loopSuccess && numberOfTries <= KLoopMaxTryouts )
        {
        error = iLine.Open( iPhone, KMmTsyVoice1LineName );
        if ( KErrNone == error )
            {
            loopSuccess = ETrue;
            iLineOpened = ETrue;
            }
        else
            {
            numberOfTries++;
            iWrapper->After( KLoopTimeout );
            }
        }

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadMobileCallL \
          iLine numberOfTries: %i", numberOfTries )

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadMobileCallL \
          iLine error: %i", error )
    
    numberOfTries = 1;
    loopSuccess =  EFalse;

    while ( iLineOpened &&!loopSuccess && numberOfTries <= KLoopMaxTryouts )
        {
        error = iCall.OpenNewCall( iLine );
        if ( KErrNone == error )
            {
            loopSuccess = ETrue;
            iCallOpened = ETrue;
            }
        else
            {
            numberOfTries++;
            iWrapper->After( KLoopTimeout );
            }
        }

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadMobileCallL \
          iCall numberOfTries: %i", numberOfTries )

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadMobileCallL \
          iCall error: %i", error )

    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::LoadMobileCallL exiting" )
    return error;
    }
    
// -----------------------------------------------------------------------------
// CSatMultiModeApi::LoadMobileCallL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CSatMultiModeApi::LoadUssdMessagingL()
    
    {
    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::LoadUssdMessagingL calling" )

    TInt numberOfTries( 1 );        // Indicates loop tryouts
    TBool loopSuccess( EFalse );    // Loop stopper, if this is ETrue
    TInt error = KErrNone;          // Error that is Leave'd

    while ( !loopSuccess && numberOfTries <= KLoopMaxTryouts )
        {
        error = iUssd.Open( iPhone );
        if ( KErrNone == error )
            {
            loopSuccess = ETrue;
            iUssdOpened = ETrue;
            }
        else
            {
            numberOfTries++;
            iWrapper->After( KLoopTimeout );
            }
        }

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadUssdMessagingL \
          iUssd numberOfTries: %i", numberOfTries )

    LOG2( NORMAL, "SATENGINE: CSatMultiModeApi::LoadUssdMessagingL \
          iUssd error: %i", error )
          
    User::LeaveIfError( error );

    LOG( SIMPLE, "SATENGINE: CSatMultiModeApi::LoadUssdMessagingL exiting" )
    }


//End of file
