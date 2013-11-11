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
* Description:  Implementation of the ClientStreamControl class.
*
*/


#include <mmf/common/mmfdrmcustomcommands.h>
#include <StreamControlEvents.h>
#include <DataBufferSource.h>
#include <EffectControl.h>
#include <SinkControl.h>
#include <SourceControl.h>
#include <FileSource.h>
#include <DescriptorSource.h>
#include <SCCustomInterfaceUIDs.h>

#include "ClientStreamControl.h"
#include "EventNotifier.h"
#include "EventBase.h"
#include "ErrorCode.h"
#include "StateChangedEvent.h"
#include "CMCustomCommand.h"
#include "SourceBase.h"
#include "SinkBase.h"
#include "tracemacros.h"

#define RETURN_IF_ERROR(x) if(x != KErrNone) return x

#ifdef __WINDOWS_MEDIA                
#ifndef RD_PD_FOR_AUDIO_CONTENT_VIA_HELIX_ENGINE
_LIT8(KWMAMimeType,"audio/x-ms-wma");
#endif
#endif

const TInt KMaxMimeLength = 256;

using namespace multimedia;

CStreamControl::CStreamControl() :
    iStreamState(EStreamClosed), iStreamControlCustomCommands(iController)
    {
    // No impl yet
    iError = KErrNone;
    }

CStreamControl::~CStreamControl()
    {
    EMC_TRACE1(_L("CStreamControl::~CStreamControl"));
    // Don't have to send events to observers.
    Close();
    delete iCMCustomCommand;
    delete iEventNotifier;
    delete iEventNotifierForEffects;
    delete iControllerEventMonitor;
    delete iFindAndOpenController;
    // Signal associated effects that this instance is closed
    TInt index(0);
    CEffectControlBase* effectBase(NULL);
    for (; index < iAssociatedEffects.Count(); index++)
        {
        effectBase = iAssociatedEffects[index];
        effectBase->StreamControlDeleted( *this);
        }
    iAssociatedEffects.Close();
    REComSession::FinalClose();
    }

TInt CStreamControl::PostConstructor()
    {
    TInt status(KErrNone);
    if (!iControllerEventMonitor)
        {
        TRAP(status, iControllerEventMonitor = CMMFControllerEventMonitor::NewL(*this, iController));
        }
    RETURN_IF_ERROR(status);

    if (!iFindAndOpenController)
        {
        TRAP(status, iFindAndOpenController = CMMFFindAndOpenController::NewL(*this));
        }
    RETURN_IF_ERROR(status);

    if (!iEventNotifier)
        {
        TRAP(status, iEventNotifier = CEventNotifier::NewL());
        }
    if ( !iEventNotifierForEffects)
        {
        TRAP(status, iEventNotifierForEffects = CEventNotifier::NewL());
        }

    if (!iCMCustomCommand)
        {
        iCMCustomCommand = new CMCustomCommand(iController);
        if ( !iCMCustomCommand)
            status = KErrNoMemory;
        }
    return status;
    }

TInt CStreamControl::AddObserver(MControlObserver& aObserver)
    {
    return iEventNotifier->AddObserver(aObserver);
    }

TInt CStreamControl::RemoveObserver(MControlObserver& aObserver)
    {
    return iEventNotifier->RemoveObserver(aObserver);
    }

TUid CStreamControl::Type()
    {
    return KStreamControl;
    }

TControlType CStreamControl::ControlType()
    {
    return EStreamControl;
    }

TInt CStreamControl::AddSource(MSourceControl& aSource)
    {
    TInt status(KErrNotReady);
    switch (iStreamState)
        {
        case EStreamClosed:
            status = KErrAlreadyExists;
            if ( !iSourceControl)
                {
                iSourceControl = &aSource;
                status = KErrNone;
                }
            break;
        default:
            break;
        };
    return status;
    }

TInt CStreamControl::RemoveSource(MSourceControl& aSource)
    {
    TInt status(KErrLocked);
    switch (iStreamState)
        {
        case EStreamClosed:
            status = KErrBadHandle;
            if (iSourceControl == &aSource)
                {
                iSourceControl = NULL;
                status = KErrNone;
                }
            break;
        default:
            break;
        };
    return status;
    }

TInt CStreamControl::AddSink(MSinkControl& aSink)
    {
    TInt status(KErrNotReady);
    switch (iStreamState)
        {
        case EStreamClosed:
            status = KErrAlreadyExists;
            if ( !iSinkControl)
                {
                iSinkControl = &aSink;
                status = KErrNone;
                }
            break;
        default:
            break;
        };
    return status;
    }

TInt CStreamControl::RemoveSink(MSinkControl& aSink)
    {
    TInt status(KErrLocked);
    switch (iStreamState)
        {
        case EStreamClosed:
            status = KErrBadHandle;
            if (iSinkControl == &aSink)
                {
                iSinkControl = NULL;
                status = KErrNone;
                }
            break;
        default:
            break;
        };
    return status;
    }

TInt CStreamControl::RemoveEffect(MEffectControl& aEffect)
    {
    TInt status(KErrNotReady);
    // Get the observer from effect
    if(&aEffect)
       {
       CEffectControlBase* effectBase =
            dynamic_cast<CEffectControlBase*>(&aEffect);

    // Remove the effect from the array
    TInt index(KErrNotFound);
    if(effectBase)
       {
       index = iAssociatedEffects.Find(effectBase);
        
    if (index != KErrNotFound)
        {
        MControlObserver* observer;
        status = effectBase->GetControlObserver(observer);
        RETURN_IF_ERROR( status );

        status = iEventNotifierForEffects->RemoveObserver( *observer);
        RETURN_IF_ERROR( status );

        iAssociatedEffects.Remove(index);
        status = effectBase->RemovedFromStreamControl(*this);
        RETURN_IF_ERROR( status );
         }
        }
       }
    return status;
        
    }

TInt CStreamControl::AddEffect(MEffectControl& aEffect)
    {
    TInt status(KErrNotReady);
    
    // Get the observer from effect
    CEffectControlBase* effectBase =
            dynamic_cast<CEffectControlBase*>(&aEffect);

    MControlObserver* observer;
    if (effectBase)
        {
        status = effectBase->GetControlObserver(observer);
        }
    RETURN_IF_ERROR( status );

    if (iEventNotifierForEffects)
        {
        status = iEventNotifierForEffects->AddObserver( *observer);
        }
    RETURN_IF_ERROR( status );

    // Add effect to array to notify Effects if user destroys Stream Control
    // before removing all Effect Controls from this instance.
    status = iAssociatedEffects.Append(effectBase);
    RETURN_IF_ERROR( status );

    status = effectBase->AddedToStreamControl( *this, *iCMCustomCommand,
            aEffect);

    // If there was any error adding, undo operations and send error code to user.
    if (status != KErrNone)
        {
        // Remove the effect from the array
        TInt index(KErrNotFound);
        index = iAssociatedEffects.Find(effectBase);
        if (index != KErrNotFound)
            {
            iAssociatedEffects.Remove(index);
            }
        iEventNotifierForEffects->RemoveObserver( *observer);
        }

    return status;

    }

// Sets the current position microseconds
TInt CStreamControl::SetPosition(TInt64& aPos)
    {
    TInt status(KErrNone);
    TTimeIntervalMicroSeconds timePos(aPos);
    status = iController.SetPosition(timePos);
    return status;
    }

TInt CStreamControl::SetPriority(TInt aPriority, TInt aPreference)
    {
    TInt status(KErrNone);
    iPrioritySettings.iPriority = aPriority;
    iPrioritySettings.iPref = TMdaPriorityPreference(aPreference);

    if (iStreamState != EStreamClosed)
        {
        status = iController.SetPrioritySettings(iPrioritySettings);
        }
    return status;
    }

TInt CStreamControl::GetDuration(TInt64& aDuration)
    {
    TInt status(KErrUnknown);
    TTimeIntervalMicroSeconds duration;
    status = iController.GetDuration(duration);
    if (status == KErrNone)
        {
        aDuration = duration.Int64();
        }
    return status;
    }

TInt CStreamControl::GetPosition(TInt64& aPos)
    {
    TInt status(KErrUnknown);
    TTimeIntervalMicroSeconds posInTime;
    status = iController.GetPosition(posInTime);
    if (status == KErrNone)
        {
        aPos = posInTime.Int64();
        }
    return status;
    }

TInt CStreamControl::GetSeekingSupport(TBool& aFlag)
    {
    TInt status(KErrNone);
    status = iStreamControlCustomCommands.GetSeekingSupported(aFlag);
    return status;
    }

TInt CStreamControl::GetRandomSeekingSupport(TBool& aFlag)
    {
    TInt status(KErrNone);
    status = iStreamControlCustomCommands.GetRandomSeekingSupported(aFlag);
    return status;
    }

MStreamControl::TStreamState CStreamControl::GetState()
    {
    return iState;
    }

TAny* CStreamControl::CustomInterface(TUid aCIUid)
    {
    TAny* retVal(NULL);

    // This function should only be called after the StreamControl
    // is Initialized
    if (aCIUid == KUidInterfaceMMFDRMControl)
        {
        RMMFDRMCustomCommands* drmCustCommand = new RMMFDRMCustomCommands(iController);
        if (drmCustCommand && drmCustCommand->IsSupported())
            {
            retVal = drmCustCommand;
            }
        else
            {
            delete drmCustCommand;
            }
        }
    else
        if (aCIUid == KUidSCControllerRef)
            {
            retVal = &iController;
            EMC_TRACE2(_L("CStreamControl::CustomInterface [%x]"), retVal);

            }
        else
            if (aCIUid == KUidSCControllerUid)
                {
                retVal = &iControllerUid;
                }
    return retVal;
    }

TInt CStreamControl::Open()
    {
    EMC_TRACE1(_L("CStreamControl::Open"));
    TInt status(KErrNotReady);

    switch (iStreamState)
        {
        case EStreamClosed:
            if (iSourceControl)
                { // Launch the controller
                iStreamState = EStreamOpening;
                status = LaunchController();
                }
            break;
        default:
            break;
        };
    return status;
    }

TInt CStreamControl::Prime()
    {
    EMC_TRACE1(_L("CStreamControl::Prime"));
    TInt status(KErrNotReady);
    switch (iStreamState)
        {
        case EStreamOpened:
            iStreamState = EStreamPriming;
            status = iController.Prime();
            iPrimeController = EFalse;
            break;
        case EStreamPriming:
            status = KErrNone;
            break;
        default:
            break;
        };
    return status;
    }

TInt CStreamControl::Start()
    {
    EMC_TRACE1(_L("CStreamControl::Start"));
    TInt status(KErrNotReady);
    switch (iStreamState)
        {
        case EStreamOpened:
        case EStreamPrimed:
        case EStreamPaused:
        case EStreamPriming:
        case EStreamStartingPlaying://added for bug #ESLM-7X8A88
            iStreamState = EStreamStartingPlaying;
           // If EOF prime controller before calling Play().
            if (iPrimeController)
                {
                status = iController.Prime();
                iPrimeController = EFalse;
                }
            else
                {
                // This is the case where Play a file, EOF reached and app calls
                // Play() again.
                status = iController.Play();
                }
            break;
        default:
            break;
        };
    return status;
    }

TInt CStreamControl::Pause()
    {
    EMC_TRACE1(_L("CStreamControl::Pause"));
    TInt status(KErrNotReady);
    switch (iStreamState)
        {
        case EStreamPlaying:
        case EStreamBuffering:
        case EStreamPrimed:
        case EStreamPriming:	
            iStreamState = EStreamPausing;
            status = iController.Pause();
            break;
        case EStreamPaused:
        	status = KErrNone;
        default:
            break;
        };
    return status;
    }


TInt CStreamControl::Stop()
    {
    EMC_TRACE1(_L("CStreamControl::Stop"));
    TInt status(KErrNotReady);
    switch (iStreamState)
        {
        case EStreamOpened:
        case EStreamPrimed:
        case EStreamPlaying:
        case EStreamPaused:
            iStreamState = EStreamPriming;
            status = iController.Stop();
            break;
        default:
            break;
        };
    return status;
    }

TInt CStreamControl::Close()
    {
    EMC_TRACE1(_L("CStreamControl::Close"));
    TInt status(KErrNotReady);

    // Disconnect All Effects. This makes sure that Effects
    // delete their MessageHandlers before the Controller is Closed.
    // This is done to Support AudioEffects in EMC.
    // This is done because if Controller tries to delete MessageHandlers
    // after deleting DevSound in the Destructor, there is a NULL pointer
    // Exception. Untill that problem is fixed in DevSound this is the fix.

    TInt count = iAssociatedEffects.Count();
    for (TInt i = 0; i < count; i++)
        {
        iAssociatedEffects[i]->Disconnect();
        }

    if (iControllerEventMonitor)
        {
        iControllerEventMonitor->Cancel();
        }

    iController.Stop();
    iController.Close();

    if (iFindAndOpenController)
        {
        iFindAndOpenController->Close();
        }

    // Signal the source that server side source is unloaded
    if (iSourceControl)
        {
        CSourceBase* sourcebase = dynamic_cast<CSourceBase*>(iSourceControl);
        if (sourcebase)
            sourcebase->ServerSourceDeleted();
        else
            return status;
        }

    SetStreamState(EStreamClosed, KErrNone);
    status = KErrNone;
    return status;
    }

void CStreamControl::HandleEvent(const TMMFEvent& aEvent)
    {
    EMC_TRACE3(_L("CStreamControl::HandleEvent:Evt[%d]Err[%d]"), aEvent.iErrorCode, aEvent.iEventType);

    if (aEvent.iEventType == KStreamControlEventStateChangedStopped)
        {
        iPrimeController = ETrue;
        SetStreamState(EStreamOpened, aEvent.iErrorCode);
        // If playback is stopped due to EOF, set flag to prime controller when
        // client calls Play() to play the same file/clip.

        }
    else
        if (aEvent.iEventType == KStreamControlEventStateChangedPrimed)
            {
            if (iStreamState == EStreamStartingPlaying)
                {
                iController.Play();
                }
            else
                {
                SetStreamState(EStreamPrimed, KErrNone);
                }
            }
        else
            if (aEvent.iEventType == KStreamControlEventStateChangedPlaying)
                {
                SetStreamState(EStreamPlaying, KErrNone);
                }
            else
                if (aEvent.iEventType
                        == KStreamControlEventStateChangedPaused)
                    {
                    SetStreamState(EStreamPaused, KErrNone);
                    }
                else
                    if (aEvent.iEventType
                            == KStreamControlEventStateChangedAutoPaused)
                        {
                        SetStreamState(EStreamBuffering, KErrNone);
                        }
                    else
                        if (aEvent.iEventType
                                == KStreamControlEventDurationChanged)
                            {
                            SendEventToClient(
                                    MStreamControlObserver::KDurationChangedEvent,
                                    NULL);
                            }
                        else
                            if (aEvent.iEventType
                                    == KStreamControlEventSeekingSupportChanged)
                                {
                                SendEventToClient(
                                        MStreamControlObserver::KSeekingSupportChangedEvent,
                                        NULL);
                                }
                            else
                                if (aEvent.iEventType
                                        == KStreamControlEventRandomSeekingSupportChanged)
                                    {
                                    SendEventToClient(
                                            MStreamControlObserver::KRandomSeekingSupportChangedEvent,
                                            NULL);
                                    }
                                else
                                    if (aEvent.iEventType
                                            == KMMFEventCategoryPlaybackComplete)
                                        { // KMMFEventCategoryPlaybackComplete will no longer be needed once
                                        // controller start sending state changed events.
                                        SetStreamState(EStreamOpened, KErrEof);
                                        iPrimeController = ETrue;
                                        }
    }

void CStreamControl::MfaocComplete(TInt& aError,
        RMMFController* /*aController*/, TUid aControllerUid,
        TMMFMessageDestination* aSourceHandle, TMMFMessageDestination* /*aSinkHandle*/)
    {
    EMC_TRACE3(_L("CStreamControl::MfaocComplete:Err[%d]aSourceHandle[0x%x]"), aError, aSourceHandle);
    if (aError == KErrNone)
        {
        iControllerLoaded = ETrue;
        // Configure the controller to send state change events.
        TInt error = iStreamControlCustomCommands.EnableEvents(ETrue);
        if (error != KErrNone)
            {
            // We should error out and not let apps to continue. This player
            // only works with controllers that send state events.

            // The same sequence occurs in Close() also. May be move this a
            // seperate function and call function here...

            // Signal the source that server side source is unloaded
        if (iSourceControl)
            {
            CSourceBase* sourcebase =
                    dynamic_cast<CSourceBase*>(iSourceControl);
             if (sourcebase)
                 sourcebase->ServerSourceDeleted();
            }
            if (iControllerEventMonitor)
                {
                iControllerEventMonitor->Cancel();
                }

            iController.Stop();
            iController.Close();

            if (iFindAndOpenController)
                {
                iFindAndOpenController->Close();
                }
            // If the stream is already in CLOSED state
            // send an error event.
            if (iState == CLOSED)
                {
                /*EError*/
                EMC_TRACE1(_L("CStreamControl::MfaocComplete:ERROR[KErrNotSupported]"));
                }
            else
                {
                SetStreamState(EStreamClosed, KErrNotSupported);
                }
            }
        else
            {

            if (!iControllerEventMonitor->IsActive())
                {
                iControllerEventMonitor->Start();
                }

            // Save the uid of controller loaded.
            iControllerUid = aControllerUid;
            // Signal the source that server side source is loaded
            if (iSourceControl)
                {
                CSourceBase* sourcebase =
                    dynamic_cast<CSourceBase*>(iSourceControl);
               if (sourcebase)
                   sourcebase->ServerSourceCreated( *iCMCustomCommand,
                    *aSourceHandle);
              else
                 {
                 SetStreamState(EStreamClosed, aError);
                 }
                }
            if (iStreamState == EStreamOpening)
                {
                iPrimeController = ETrue;
                SetStreamState(EStreamOpened, KErrNone);
                //FIX ME we need to do this even when the controller is closed
                CErrorCode* eventforEffects = new CErrorCode( KErrNone );
                SendEventToEffectControls(
                        CEffectControlBase::ECIBuilderCreated,
                        eventforEffects);
                }
            }
        }
    else
        {
        SetStreamState(EStreamClosed, aError);
        }
    }

void CStreamControl::ConfigureControllerLoader(TUid aMediaType,
        CMMFFindAndOpenController::TControllerMode aMode)
    {
    iFindAndOpenController->Configure(aMediaType, iPrioritySettings);
    iFindAndOpenController->ConfigureController(iController,
            *iControllerEventMonitor, aMode);
    }

TInt CStreamControl::LaunchController()
    {
    TInt status(KErrNotReady);
    // KUidMediaTypeAudio need to be part of source
    ConfigureControllerLoader(KUidMediaTypeAudio,
            CMMFFindAndOpenController::EPlayback);
    // if Streaming case

    if (!iSourceControl || !iSinkControl)
        return KErrNotReady;
   CSourceBase *sourceBase=(dynamic_cast<CSourceBase*>(iSourceControl));
   TUid sourceUID ;

   if (sourceBase)
      {
       sourceUID = sourceBase->GetSourceUid();
      }
   else
       {
       return KErrNotReady;
       }

   CSinkBase *sinkBase=(dynamic_cast<CSinkBase*>(iSinkControl));
   TUid sinkUID;

   if(sinkBase)
      sinkUID = sinkBase ->GetSinkUid();
   else
      return KErrNotReady;

    if (iSourceControl->Type() == KDataBufferSourceControl)
        {
        MDataBufferSource* streamingSource =
                static_cast<MDataBufferSource*>(iSourceControl);
        iFindAndOpenController->ConfigureSourceSink(
                CMMFFindAndOpenController::TSourceSink( /*TUid::Uid(0x10207AF3)*/sourceUID),
                CMMFFindAndOpenController::TSourceSink( /*KUidMmfAudioOutput*/sinkUID) );
        TBuf8<KMaxMimeLength> mimeType;
        status = streamingSource->GetMimeType(mimeType);
        if (status == KErrNone)
            {

            // This Flag is defined so that if the Helix Controller Supports
            // the playback of Local Media for WMA, then the HTTP Streaming still
            // goes through the Old WMA Controller( AdvancedAudioController)                
            // We are launching the Old WMA Controller using the UID.

#ifdef __WINDOWS_MEDIA                
#ifndef RD_PD_FOR_AUDIO_CONTENT_VIA_HELIX_ENGINE
            if(!mimeType.Compare(KWMAMimeType()))
                {
                iFindAndOpenController->OpenByControllerUid(TUid::Uid(0x10207A9B),KNullUid);
                }
            else
#endif
#endif
                {
                iFindAndOpenController->OpenByMimeType(mimeType);
                }
            }
        }

    if (iSourceControl->Type() == KFileSourceControl)
        {
        MFileSource* fileSource = static_cast<MFileSource*>(iSourceControl);

        HBufC* fileName = HBufC::NewLC(KMaxPath);
        TPtr fileNamePtr = fileName->Des();

        TInt err = fileSource->GetFileName(fileNamePtr);

        HBufC8* fileName1 = HBufC8::NewLC(KMaxPath);
        TPtr8 fileNamePtr1 = fileName1->Des();
        fileNamePtr1.Copy(fileNamePtr);

        HBufC8* mimeType = HBufC8::NewLC(KMaxMimeLength);
        TPtr8 mimeTypePtr = mimeType->Des();

        status = iSourceControl->GetMimeType(mimeTypePtr);

        iFindAndOpenController->ConfigureSourceSink(
                CMMFFindAndOpenController::TSourceSink(sourceUID,
                        fileNamePtr1),
                CMMFFindAndOpenController::TSourceSink(sinkUID));

        TMMFileSource source(fileNamePtr,
                ContentAccess::KDefaultContentObject, ContentAccess::EPlay);
        iFindAndOpenController->OpenByFileSource(source);

        CleanupStack::PopAndDestroy(mimeType); // mimeType
        CleanupStack::PopAndDestroy(fileName1); // fileName
        CleanupStack::PopAndDestroy(fileName); // fileName
        }

    if (iSourceControl->Type() == KDescriptorSourceControl)
        {
        MDescriptorSource* descriptorSource =
                static_cast<MDescriptorSource*>(iSourceControl);
        iFindAndOpenController->ConfigureSourceSink(
                CMMFFindAndOpenController::TSourceSink( /*TUid::Uid(0x10207AF3)*/sourceUID),
                CMMFFindAndOpenController::TSourceSink( /*KUidMmfAudioOutput*/sinkUID) );
        TBuf8<KMaxMimeLength> mimeType;
        status = descriptorSource->GetMimeType(mimeType);
        if (status == KErrNone)
            {
            // This Flag is defined so that if the Helix Controller Supports
            // the playback of Local Media for WMA, then the HTTP Streaming still
            // goes through the Old WMA Controller( AdvancedAudioController)                
            // We are launching the Old WMA Controller using the UID.

            iFindAndOpenController->OpenByMimeType(mimeType);
            }
        }
    return status;
    }

void CStreamControl::SetStreamState(TStreamControlState aState, TInt aReason)
    {
    EMC_TRACE4(_L("CStreamControl::SetStreamState:aState[%d]aReason[%d]iState[%d]"), \
            aState, aReason, iState);
    TStreamState prevState = iState;
    TInt prevError = iError;
    iError = aReason;
    switch (aState)
        {
        case EStreamClosed:
            iStreamState = EStreamClosed;
            iState = CLOSED;
            break;
        case EStreamOpened:
            iStreamState = EStreamOpened;
            iState = INITIALIZED;
            break;
        case EStreamPrimed:
            iStreamState = EStreamPrimed;
            iState = PRIMED;
            break;
        case EStreamPlaying:
            iStreamState = EStreamPlaying;
            iState = EXECUTING;
            break;
        case EStreamPaused:
            iStreamState = EStreamPaused;
            iState = PAUSED;
            break;
        case EStreamBuffering:
            iStreamState = EStreamBuffering;
            iState = BUFFERING;
            break;
        default:
            break;
        };
    // If there is a change in state notify client
    if (prevState != iState || iError != prevError)
        {
        CStateChangedEvent* event(NULL);
        //EMC_TRACE3(_L("CStreamControl::SetStreamState Before iState [%d] prevState [%d]"),
        //        iState,prevState);
        //EMC_TRACE3(_L("CStreamControl::SetStreamState Before iError [%d] prevError [%d]"), iError,prevError);
        event = new CStateChangedEvent( iState, aReason );
        //EMC_TRACE3(_L("CStreamControl::SetStreamState After iState [%d] iReason [%d]"), event->GetState() ,event->GetErrorCode());
        SendEventToClient(MStreamControlObserver::KStateChangedEvent, event);
        }
    }

void CStreamControl::SendEventToClient(TUint aEvent, CEventBase* aEventObject)
    {
    iEventNotifier->Event(this, aEvent, aEventObject);
    }

void CStreamControl::SendEventToEffectControls(TUint aEvent,
        CEventBase* aEventObject)
    {
    iEventNotifierForEffects->Event( this, aEvent, aEventObject);
    }
// End of file
