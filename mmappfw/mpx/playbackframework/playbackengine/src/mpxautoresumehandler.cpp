/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implemention of the auto resume handler.
*
*/


#include <e32std.h>
#include <ctsydomainpskeys.h>
#include <mpxpskeywatcher.h>
#include <mpxlog.h>
#include "mpxplaybackengine.h"
#include "mpxautoresumehandler.h"

// CONSTANTS
const TInt KMPXErrDiedTimeout = 2000000;

// Time to wait before resume after call has ended.
const TInt KMPXResumeWaitTime = 3000000; // 3.0s

// TODO: the following constants and definitions are copied from nssvascoreconstant.h, which is not
// included directly because it is an App layer API. Way to fix this is to ask Speechsrv to move the header file to MW layer.
const TUid KSINDUID = {KUidSystemCategoryValue};
const TInt ERecognitionState=0;

// Recognition state values for the P&S key ERecognitionState
enum TRecognitionStateValues
    {
    ERecognitionStarted = 0, 
    ERecognitionSpeechEnd, 
    ERecognitionSuccess, 
    ERecognitionFail
    };
// End TODO


// ================= MEMBER FUNCTIONS =======================


// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
CMPXAutoResumeHandler::CMPXAutoResumeHandler(
    CMPXPlaybackEngine& aEngine,
    TBool aMixerSupport) :
    iEngine(aEngine),
    iMixerSupport(aMixerSupport),
    iAutoResume(ETrue)
    {
    }

// -----------------------------------------------------------------------------
// Symbian OS default constructor
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::ConstructL()
    {
    // Listen to call state changes
    iStateObserver = CMPXPSKeyWatcher::NewL(KPSUidCtsyCallInformation,
                                            KCTsyCallState,this);

    // Listen to call type changes
    iTypeObserver = CMPXPSKeyWatcher::NewL(KPSUidCtsyCallInformation,
                                           KCTsyCallType,this);
    
    iVoiceCmdObserver = CMPXPSKeyWatcher::NewL( KSINDUID, ERecognitionState, this );
    
    iResumeTimer = CPeriodic::NewL(CActive::EPriorityStandard);
    }

// -----------------------------------------------------------------------------
// Constructs a new entry with given values.
// -----------------------------------------------------------------------------
//
CMPXAutoResumeHandler* CMPXAutoResumeHandler::NewL(
                                         CMPXPlaybackEngine& aEngine,
                                         TBool aMixerSupport)
    {
    CMPXAutoResumeHandler* self =
        new (ELeave) CMPXAutoResumeHandler(aEngine, aMixerSupport);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::CMPXAutoResumeHandler()
// Destructor
// -----------------------------------------------------------------------------
//
CMPXAutoResumeHandler::~CMPXAutoResumeHandler()
    {
    delete iVoiceCmdObserver;
    delete iStateObserver;
    delete iTypeObserver;
    if ( iResumeTimer )
        {
        CancelResumeTimer();
        delete iResumeTimer;
        }
    }

// -----------------------------------------------------------------------------
// Open file completed
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::HandleOpenFileComplete()
    {
    MPX_FUNC("CMPXAutoResumeHandler::HandleOpenFileComplete()");
    iPausedForCall = EFalse;
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::MPlayerStateChanged
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::HandlePlaybackStateChange(TMPXPlaybackState aState)
    {
    MPX_DEBUG2("CMPXAutoResumeHandler::HandlePlaybackStateChange(%d) entering", aState);

    // Any state change means that possible waiting resume is not to be done
    // anymore.
    CancelResumeTimer();

    if (aState != EPbStatePaused)
        {
        iPausedForCall = EFalse;
        }
    MPX_DEBUG1("CMPXAutoResumeHandler::HandlePlaybackStateChange() exiting");
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::MPlayerPlayComplete
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::HandlePlaybackComplete(TInt aError)
    {
    MPX_DEBUG2("CMPXAutoResumeHandler::HandlePlaybackComplete(%d) entering", aError);
    iPausedForCall = EFalse;
    if ( KErrDied == aError ||
         KErrAccessDenied == aError || 
         KErrInUse == aError )
        {
        iKErrDiedTime.HomeTime();

        TInt callType = EPSCTsyCallTypeNone;
        TInt callState = EPSCTsyCallStateNone;
        TInt voiceCmdState(0);
        
        if (!iTypeObserver->GetValue(callType) &&
            !iStateObserver->GetValue(callState))
            {
            if ((callState == EPSCTsyCallStateRinging && iMixerSupport) ||
                 ShouldPause())
                {
                // Getting a play complete with KErrDied here
                // means Audio Policy terminated our playback,
                // due to phone call being connected. Enable
                // autoresume.
                iPausedForCall = ETrue;
                }
            }
        
        if ( !iPausedForCall && !iVoiceCmdObserver->GetValue( voiceCmdState ) ) // key exist if voice commanding is in progress
            {
            // Paused due voice command activity
            iPausedForVoiceCmd = ETrue;
            }
        }
    
    MPX_DEBUG3("CMPXAutoResumeHandler::HandlePlaybackComplete() exiting: iPausedForCall=%d, iPausedForVoiceCmd=%d",
               iPausedForCall, iPausedForVoiceCmd);
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::CancelResumeTimer
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::CancelResumeTimer()
    {
    if ( iResumeTimer )
        {
        iResumeTimer->Cancel();
        }
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::HandlePSEvent
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::HandlePSEvent(TUid aUid, TInt /*aKey*/)
    {
    MPX_FUNC("CMPXAutoResumeHandler::HandlePSEvent()");
    
    if ( aUid == KSINDUID )
        {
        DoHandleVoiceCmdChange();
        }
    else
        {
        TRAP_IGNORE(DoHandleStateChangeL());
        }
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::DoHandleStateChangeL
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::DoHandleStateChangeL()
    {
    MPX_FUNC("CMPXAutoResumeHandler::DoHandleStateChangeL()");
    MPX_DEBUG2("CMPXAutoResumeHandler::DoHandleStateChangeL(): iPausedForCall = %d", iPausedForCall);
    MPX_DEBUG2("CMPXAutoResumeHandler::DoHandleStateChangeL(): engineState = %d", iEngine.State());
    // if autoresume is disabled, do nothing
    if ( !iAutoResume )
        {
        return;
        }
    
    TBool shouldPause = ShouldPause();
    if (shouldPause &&
        !iPausedForCall &&
        iEngine.State() == EPbStatePlaying)
        {
        iEngine.HandleCommandL(EPbCmdPause);
        iPausedForCall = ETrue;
        }
    else if ( shouldPause &&
            !iPausedForCall &&
            ( iEngine.State() == EPbStateSeekingForward ||
            iEngine.State() == EPbStateSeekingBackward ) )
        {
        iEngine.HandleCommandL( EPbCmdStopSeeking );
        if ( iEngine.State() == EPbStatePlaying )
            {
            iEngine.HandleCommandL( EPbCmdPause );
            iPausedForCall = ETrue;
            }
        }
    else if(!shouldPause &&
            iPausedForCall &&
            iEngine.State() == EPbStatePaused)
        {
        MPX_DEBUG1("CMPXAutoResumeHandler::DoHandleStateChangeL(): starting resume timer");
        if ( iResumeTimer->IsActive() )
            iResumeTimer->Cancel();
        iResumeTimer->Start(
            KMPXResumeWaitTime,
            KMPXResumeWaitTime,
            TCallBack(ResumeTimerCallback, this) );
        iPausedForCall = EFalse;
        }
    else if ( shouldPause &&
             iEngine.State() == EPbStatePaused &&
             !iPausedForCall &&
             iKErrDiedTime.Int64())
        {
        // Check if we recently got a playcomplete with KErrDied,
        // it was most likely caused by an active call
        TTime now;
        now.HomeTime();
        TInt64 deltaTime = now.MicroSecondsFrom(iKErrDiedTime).Int64();
        if ( deltaTime > 0 &&
             deltaTime < KMPXErrDiedTimeout)
            {
            iResumeTimer->Cancel();
            iPausedForCall = ETrue;
            }
        }
    
    if ( shouldPause && iVoiceCmdResumeOngoing )
        {
        // Resume timer has been started after a voice command, cancel it now
        // so that playback is not resumed while calling
        if ( iResumeTimer->IsActive() )
            {
            iResumeTimer->Cancel();
            }
        iVoiceCmdResumeOngoing = EFalse;
        iPausedForCall = ETrue; // resume playback once call has been ended
        }
    
    MPX_DEBUG2("CMPXAutoResumeHandler::DoHandleStateChangeL(): iPausedForCall = %d", iPausedForCall);
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::ShouldPause
// -----------------------------------------------------------------------------
//
TBool CMPXAutoResumeHandler::ShouldPause()
    {
    MPX_DEBUG1("CMPXAutoResumeHandler::ShouldPause() entering");
    TBool ret = EFalse;

    if ( !IsPlaybackRemote() )
        {
        TInt callType;
        TInt callState;
        iTypeObserver->GetValue(callType);
        iStateObserver->GetValue(callState);
        MPX_DEBUG3("CMPXAutoResumeHandler::ShouldPause(): type = %d, state = %d", callType, callState);

        if (callType == EPSCTsyCallTypeCSVoice ||
            callType == EPSCTsyCallTypeH324Multimedia ||
            callType == EPSCTsyCallTypeVoIP ||
            callType == EPSCTsyCallTypeUninitialized)
            {
            switch (callState)
                {
                case EPSCTsyCallStateAnswering:
                case EPSCTsyCallStateAlerting:
                case EPSCTsyCallStateConnected:
                case EPSCTsyCallStateDialling:
                case EPSCTsyCallStateHold:
                case EPSCTsyCallStateDisconnecting:
                    {
                    ret = ETrue;
                    break;
                    }
                case EPSCTsyCallStateRinging:
                    {
                    if (iPausedForCall)
                       {
                       ret = ETrue;
                       }
                    else
                        {
                        // Pause playback if we cannot mix music playback
                        // with ringing tone.
                        ret = !iMixerSupport;
                        }
                    break;
                    }
                default:
                    {
                    // Default is no pause
                    break;
                    }
                }
            }
        }
    MPX_DEBUG2("CMPXAutoResumeHandler::ShouldPause() exiting: %d", ret);
    return ret;
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::HandleResumeTimerCallback
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::HandleResumeTimerCallback()
    {
    MPX_FUNC("CMPXAutoResumeHandler::HandleResumeTimerCallback() entering");

    iVoiceCmdResumeOngoing = EFalse;
    
    CancelResumeTimer();
    TRAP_IGNORE( iEngine.HandleCommandL( EPbCmdPlayWithFadeIn ));
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::ResumeTimerCallbackL
// -----------------------------------------------------------------------------
//
TInt CMPXAutoResumeHandler::ResumeTimerCallback(TAny* aPtr)
    {
    MPX_FUNC("CMPXAutoResumeHandler::ResumeTimerCallback()");

    CMPXAutoResumeHandler* ptr =
        static_cast<CMPXAutoResumeHandler*>(aPtr);
    ptr->HandleResumeTimerCallback();

    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::IsPlaybackRemote
// -----------------------------------------------------------------------------
//
TBool CMPXAutoResumeHandler::IsPlaybackRemote()
    {
    MPX_DEBUG1("CMPXAutoResumeHandler::IsPlaybackRemote() entering");
    TBool isRemote = EFalse;

    if ( iEngine.State() == EPbStatePlaying )
        {
        TMPXPlaybackPlayerType type( EPbLocal );
        TUid uid;
        TInt index;
        TPtrC subPlayerName( KNullDesC );
        iEngine.PluginHandler()->GetSelection( type, uid,
                                               index, subPlayerName );

        if ( type != EPbLocal )
            {
            isRemote = ETrue;
            }
        }

    MPX_DEBUG2("CMPXAutoResumeHandler::IsPlaybackRemote() exiting: %d", isRemote);
    return isRemote;
    }

// -----------------------------------------------------------------------------
// Set autoresume value
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::SetAutoResume(TBool aAutoResume)
    {
    MPX_DEBUG2("CMPXAutoResumeHandler::SetAutoResume(): AutoResume = %d", aAutoResume);
    iAutoResume = aAutoResume;
    }

// -----------------------------------------------------------------------------
// CMPXAutoResumeHandler::DoHandleVoiceCmdChange
// -----------------------------------------------------------------------------
//
void CMPXAutoResumeHandler::DoHandleVoiceCmdChange()
    {
    MPX_FUNC("CMPXAutoResumeHandler::DoHandleVoiceCmdChange()");
    
    TInt voiceCmdState( 0 );
    TInt err( iVoiceCmdObserver->GetValue( voiceCmdState ) );
    
    MPX_DEBUG4("CMPXAutoResumeHandler::DoHandleVoiceCmdChange(): iPausedForVoiceCmd = %d, err=%d, state=%d", 
                    iPausedForVoiceCmd, err, voiceCmdState);
    
    if ( iPausedForVoiceCmd && !iPausedForCall )
        {
        if ( err == KErrNotFound ) // voice command has been finished once the P&S key is deleted 
            {
            if ( iResumeTimer->IsActive() )
                  iResumeTimer->Cancel();
            
            iResumeTimer->Start( KMPXResumeWaitTime, KMPXResumeWaitTime, TCallBack(ResumeTimerCallback, this) );
            
            iPausedForVoiceCmd = EFalse;
            
            iVoiceCmdResumeOngoing = ETrue; // flag for cancelling resume timer due to a call
            }
        }
    
    if ( iPausedForCall ) // ensure that not interfering with call handling in any circumstances
        {
        iPausedForVoiceCmd = EFalse;
        }
    }

//  End of File
