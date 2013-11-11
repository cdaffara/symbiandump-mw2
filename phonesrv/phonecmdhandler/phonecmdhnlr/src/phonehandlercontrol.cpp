/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Receives call handling related key presses from accessories and 
*                executes them. 
*
*/



// INCLUDE FILES
#include "phonehandlercontrol.h" 
#include "phonehandlerservice.h" 
#include "phonehandleranswercall.h" 
#include "phonehandlerendcall.h" 
#include "phonehandlerdialcall.h" 
#include "phonehandlervoicedial.h" 
#include "phonehandlerredial.h" 
#include "phonehandlermultipartycall.h" 
#include "phonehandlerdtmf.h" 
#include "phonehandleractive.h" 
#include "phonehandlerresponse.h" 
#include "phonehandlercallstate.h" 
#include "phonehandlerdebug.h" 
#include <remconinterfaceselector.h>
#include <RemConCallHandlingTarget.h> 
#include <ctsydomainpskeys.h>

#if 0
#include <voiceuidomainpskeys.h> 
#endif

#include <connect/sbdefs.h>

// EXTERNAL DATA STRUCTURES

// EXTERNAL FUNCTION PROTOTYPES  

// CONSTANTS

// MACROS

// LOCAL CONSTANTS AND MACROS

// MODULE DATA STRUCTURES

// LOCAL FUNCTION PROTOTYPES
const TInt KMultipartyCallMinParam = 1;
const TInt KMultipartyCallMaxParam = 2;

// FORWARD DECLARATIONS

// ============================= LOCAL FUNCTIONS ===============================


// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::CPhoneHandlerControl
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPhoneHandlerControl::CPhoneHandlerControl()
    : iPrevState( EPSCTsyCallStateNone )
    {
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::ConstructL( CRemConInterfaceSelector* aIfSelector )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::ConstructL() start" );
        
    if( !aIfSelector )
        {
        COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::ConstructL() Create connection to RemCon." );
            
        // Target connection to RemCon FW hasn't been done.
        iInterfaceSelector = CRemConInterfaceSelector::NewL();
        iTarget = CRemConCallHandlingTarget::NewL( *iInterfaceSelector, *this );
        iInterfaceSelector->OpenTargetL();
        }
    else
        {
        COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::ConstructL() Don't create connection to RemCon." );
                
        // Connection to RemCon FW as target has already been done in a process.
        iTarget = CRemConCallHandlingTarget::NewL( *aIfSelector, *this );
        }
        
    iResponse = CPhoneHandlerResponse::NewL( *this );
    iCallStateObserver = CPhoneHandlerCallState::NewL( *this );
            
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::ConstructL() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPhoneHandlerControl* CPhoneHandlerControl::NewL( 
                                    CRemConInterfaceSelector* aIfSelector )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NewL() start" );
    
    CPhoneHandlerControl* self = new ( ELeave ) CPhoneHandlerControl();
    
    CleanupStack::PushL( self );
    self->ConstructL( aIfSelector );
    CleanupStack::Pop( self );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NewL() end" );
    
    return self;
    }

// Destructor
CPhoneHandlerControl::~CPhoneHandlerControl()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::~CPhoneHandlerControl() start" );
    
    if( iInterfaceSelector )
        {
        delete iInterfaceSelector;
        }
    
    if( iResponse )
        {
        iResponse->Delete();
        }
        
    if( iCallStateObserver )
        {
        delete iCallStateObserver;
        }
            
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::~CPhoneHandlerControl() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::TelephoneNumber
// Provides phone number for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TPhCltTelephoneNumber& 
    CPhoneHandlerControl::TelephoneNumber() const
    {
    return iTelNumber;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::VoiceDialStatus
// Provides voice dial status for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TBool& 
    CPhoneHandlerControl::VoiceDialStatus() const
    {
    return iActivate;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::ChldCommand
// Provides CHLD command for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TInt& 
    CPhoneHandlerControl::ChldCommand() const
    {
    return iChldCommand;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::ChldCallNumber
// Provides CHLD call number for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TInt& 
    CPhoneHandlerControl::ChldCallNumber() const
    {
    return iChldCallNumber;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::Tone
// Provides DTMF tone for service.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
const TChar& 
    CPhoneHandlerControl::Tone() const
    {
    return iChar;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::CommandInitiator
// Provides API that service uses to send reponse to command.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CRemConCallHandlingTarget& 
    CPhoneHandlerControl::CommandInitiator() const
    {
    return *iTarget;
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::NotifyCallState
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::NotifyCallState( const TInt aState )
    {
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState() aState=%d", aState );
    
    if( aState == EPSCTsyCallStateConnected && iPrevState != EPSCTsyCallStateHold )
        {
        COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState(): iActiveCalls++" );
        
        iActiveCalls++;
        }
    else if( aState == EPSCTsyCallStateDisconnecting && 
            ( iPrevState == EPSCTsyCallStateConnected || iPrevState == EPSCTsyCallStateHold ))
        {
        COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState(): iActiveCalls--" );
        
        iActiveCalls--;
        }
    else if( aState == EPSCTsyCallStateNone )
        {
        COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState(): iActiveCalls = 0" );
        iActiveCalls = 0;
        }
        
    iPrevState = aState;
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::NotifyCallState() iActiveCalls=%d", iActiveCalls );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::AnswerCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::AnswerCall()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::AnswerCall() start" );
    
    StartProcessing( ERemConExtAnswerCall );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::AnswerCall() end" );
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::EndCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::EndCall()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::EndCall() start" );
    
    StartProcessing( ERemConExtEndCall );
            
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::EndCall() end" );
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::AnswerEndCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::AnswerEndCall()
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::AnswerEndCall() start" );
    
    StartProcessing( ERemConExtAnswerEnd );
           
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::AnswerEndCall() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::VoiceDial
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
#if 0 // SCB CR EHSA-7APJWF: SIND subscribes to RemCon directly 
void CPhoneHandlerControl::VoiceDial( const TBool aActivate )
    {
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::VoiceDial() aActivate=%d", aActivate );

    TInt callState( EPSCTsyCallStateUninitialized );
    iProperty.Get( KPSUidCtsyCallInformation, KCTsyCallState, callState ); 

    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() call state = %d", callState );

    if( callState == EPSCTsyCallStateUninitialized ||
        callState == EPSCTsyCallStateNone )
        {
        TInt voiceUiState( KVoiceUiIsClose );
        iProperty.Get( KPSUidVoiceUiAccMonitor, KVoiceUiOpenKey, voiceUiState ); 

        if( voiceUiState == KVoiceUiIsOpen )
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::VoiceDial() voice UI is open!" );
            iProperty.Set( KPSUidVoiceUiAccMonitor , KVoiceUiAccessoryEvent, ERemConExtVoiceDial );
            iResponse->SetResponse( ERemConExtVoiceDial, KErrNone );
            iResponse->Process();
            }
        else
            {
            iActivate = aActivate;
            StartProcessing( ERemConExtVoiceDial );
            }
        }
    else
        {
        iResponse->SetResponse( ERemConExtVoiceDial, KErrNone );
        iResponse->Process();
        }

    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::VoiceDial() end" );
    }
#else   
void CPhoneHandlerControl::VoiceDial( const TBool /*aActivate*/ )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::VoiceDial()" );

    iResponse->SetResponse( ERemConExtVoiceDial, KErrNone );
    iResponse->Process();

    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::VoiceDial() end" );
    }
#endif
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::LastNumberRedial
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::LastNumberRedial( )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::LastNumberRedial() start" );
    
    StartProcessing( ERemConExtLastNumberRedial );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::LastNumberRedial() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::DialCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::DialCall( const TDesC8& aTelNumber )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::DialCall() start" );
    
    // Check aTelNumber range    
    if( KPhCltTelephoneNumberLength < aTelNumber.Length() )
        {
        COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::DialCall() number value=%d too long", aTelNumber.Size() );
        iResponse->SetResponse( ERemConExtDialCall, KErrArgument );
        iResponse->Process();
        return; 
        }
    
    iTelNumber.Copy( aTelNumber );
                    
    StartProcessing( ERemConExtDialCall );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::DialCall() end" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::MultipartyCalling
// Method supports multiparty call handling according to BT handsfree profile 
// (HFP) 1.5. BT HFP 1.5 defines contents of aData parameter as follows: 0, 1, 
// 1<idx>, 2, 2<idx>, 3 and 4, where: 
//
// 0 = Releases all held calls or sets User Determined User Busy (UDUB) for 
// a waiting call. 
// 1 = Releases all active calls (if any exist) and accepts the other 
// (held or waiting) call. 
// 1<idx> = Releases specified active call only (<idx> = digit from 1 to 9. 
// Command is e.g. "11" ). 
// 2 = Places all active calls (if any exist) on hold and accepts the other 
// (held or waiting) call. 
// 2<idx> = Request private consultation mode with specified call (<idx> = 
// from 1 to 9. Command is e.g. "21" ). 
// (Place all calls on hold EXCEPT the call indicated by <idx>.) 
// 3 = Adds a held call to the conversation. 
// 4 = Connects the two calls and disconnects the subscriber from both calls 
// (Explicit Call Transfer). Support for this value and its associated 
// functionality is optional for the HF. 
// 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::MultipartyCalling( const TDesC8& aData )
    {
    TBool error( EFalse );
    iChldCommand = 0;
    iChldCallNumber = 0;
    TBuf8< KRemConExtParamMaxLength > buf( aData );
    buf.Trim();
    TInt length = buf.Length();
    
    // Check invalid number of characters
    if ( length < KMultipartyCallMinParam || length > KMultipartyCallMaxParam )
        {
        error = ETrue;
        }
    else
        {
        TLex8 param;
        // Set command  
        param.Assign( buf.Mid( 0, 1 ) );
        if( param.Val( iChldCommand ) != KErrNone )
            {
            error = ETrue;
            }
        else if ( length == KMultipartyCallMaxParam )
            {
            // Set call number
            param.Assign( buf.Mid(1) );
            if( param.Val( iChldCallNumber ) != KErrNone )
                {
                error = ETrue;
                }
            }
        }

    if( error )
        {
        // Invalid command 
        iResponse->SetResponse( ERemConExt3WaysCalling, KErrArgument );
        iResponse->Process();
        return; 
        }
        
    COM_TRACE_2( "[PHONECMDHANDLER] CPhoneHandlerControl::MultipartyCalling() iChldCommand=%d, iChldCallNumber=%d", iChldCommand, iChldCallNumber );
        
    StartProcessing( ERemConExt3WaysCalling );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::MultipartyCalling() end" );
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::GenerateDTMF
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::GenerateDTMF( const TChar aChar )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::GenerateDTMF() start" );
    
    iChar = aChar;
    
    StartProcessing( ERemConExtGenerateDTMF );
    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::GenerateDTMF() end" );
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::SpeedDial
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::SpeedDial( const TInt /*aIndex*/ )
    {    
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::SpeedDial() - Not Supported" );
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::StartProcessing
// Creates an object that executes call handling command received from 
// accessory.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPhoneHandlerControl::StartProcessing( 
    const TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::StartProcessing() start" );
            
    // Create a service
    MPhoneHandlerService* service = NULL;
    TInt error( KErrNone );
    TRAP( error, service = CreateServiceL( aOperation ) ); 
        
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() returned %d", error );
        
    if( error )
        {
        COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::StartProcessing(): service wasn't created succesfully." );
        
        // An error happened. Delete service, if it exists.
        if( service )
            {
            service->Delete();
            service = NULL;
            }
        
        iResponse->SetResponse( aOperation, error );
        iResponse->Process();
        return;
        }
    else
        {
        // start service
        service->Process(); 
        iSwitchCall = EFalse;
        }
            
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::StartProcessing() end" );
    }
    
// -----------------------------------------------------------------------------
// CPhoneHandlerControl::CreateServiceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
MPhoneHandlerService* CPhoneHandlerControl::
    CreateServiceL( const TRemConExtCallHandlingApiOperationId aOperation )
    {
    COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() start" );
      
    MPhoneHandlerService* pService = NULL;

    switch( aOperation )
        {
        case ERemConExtAnswerCall:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtAnswerCall command" );
            COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - iActiveCalls is %d", iActiveCalls );

            /*          
            if( iActiveCalls > 0 )
                {
                // Multiparty call
                iSwitchCall = ETrue;
                pService = CPhoneHandlerMultipartyCall::NewL( *this );
                }
            else
                {
                // non-multiparty call
                pService = CPhoneHandlerAnswerCall::NewL( *this );
                }
            */
            pService = CPhoneHandlerAnswerCall::NewL( *this );

            break;  
            }
            
        case ERemConExtEndCall:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtEndCall command" );
            
            pService = CPhoneHandlerEndCall::NewL( *this );
            
            break;  
            }
            
        case ERemConExtAnswerEnd:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtAnswerEnd command" );
                        
            TInt callState( EPSCTsyCallStateUninitialized );
            iProperty.Get( KPSUidCtsyCallInformation, KCTsyCallState, callState ); 
            
            COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() call state = %d", callState );
               
            if( callState != EPSCTsyCallStateUninitialized &&
                callState != EPSCTsyCallStateNone && 
                callState != EPSCTsyCallStateDisconnecting )
                {
                if( callState == EPSCTsyCallStateAlerting ||
                    callState == EPSCTsyCallStateDialling ||
                    callState == EPSCTsyCallStateAnswering ||
                    callState == EPSCTsyCallStateConnected ||
                    callState == EPSCTsyCallStateHold )
                    {
                    pService = CPhoneHandlerEndCall::NewL( *this, 
                                                       aOperation );
                    }
                // callState == EPSTelephonyCallStateRinging
                else
                    {
                    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() iActiveCalls = %d", iActiveCalls );
                    if( iActiveCalls > 0 )
                        {
                        // multicall case : End call that has existed longer.
                        // (From UI viewpoint ringing call is in waiting state.)
                        pService = CPhoneHandlerEndCall::NewL( *this, 
                                                       aOperation );
                        if( callState == EPSCTsyCallStateRinging )
                            {
                            // In multiparty case the telephony key is not
                            // updated. The call amount is updated manually.
                            iActiveCalls--;
                            }
                        }
                    else
                        {
                        pService = CPhoneHandlerAnswerCall::NewL( 
                                        *this, 
                                        aOperation );
                        }
                    }
                }
            else
                {
                // Send / end button was pressed when there were not 
                // any calls active. Response has to be sent back to accessory
                // at least for following reasons:
                
                // 1. RemCon FW releases message related memory only when response
                // is sent back to accessory.
                
                // 2. BT accessory key press producing ERemConExtAnswerEnd 
                // operation has some other meaning than answer/end call 
                // and it's processed by proper client. However, there 
                // might be a situation where proper client isn't active 
                // and can't process command. In any case RemCon/BT accessory
                // requires response to command. That's why PhoneCmdHandler 
                // sends reponse to command. 
                                
                iResponse->SetResponse( aOperation, KErrNone );
                pService = iResponse;
                }
            break;
            } 
            
        case ERemConExtDialCall:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtDialCall command" );
                        
            pService = CPhoneHandlerDialCall::NewL( *this );
            break;  
            }
            
        case ERemConExtVoiceDial:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtVoiceDial command" );
                        
            pService = CPhoneHandlerVoiceDial::NewL( *this );
            break;  
            }
            
        case ERemConExtLastNumberRedial:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtLastNumberRedial command" );
                        
            pService = CPhoneHandlerLastNumberRedial::NewL( *this );
            break;  
            }
            
        case ERemConExt3WaysCalling:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExt3WaysCalling command" );
                        
            pService = CPhoneHandlerMultipartyCall::NewL( *this );
            break;  
            }
            
        case ERemConExtGenerateDTMF:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() - ERemConExtGenerateDTMF command" );
                        
            pService = CPhoneHandlerDTMF::NewL( *this );
            break;  
            }
                    
        default:
            {
            COM_TRACE_( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() Unspecified state" );
            break;
            }
        };
        
    COM_TRACE_1( "[PHONECMDHANDLER] CPhoneHandlerControl::CreateServiceL() end, %d", pService );
    return pService;
    }

// -----------------------------------------------------------------------------
// CPhoneHandlerControl::SwitchCall
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPhoneHandlerControl::SwitchCall()
    {
    return iSwitchCall;
    }   
// ======================== OTHER EXPORTED FUNCTIONS ===========================

// End of File
