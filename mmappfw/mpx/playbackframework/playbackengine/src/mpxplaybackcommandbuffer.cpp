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
* Description:  Buffer commands
*
*/


#include"mpxplaybackcommandbuffer.h"
#include "mpxlog.h"

// ============================== MEMBER FUNCTIONS ============================


// ----------------------------------------------------------------------------
// Two-phased constructor
// ----------------------------------------------------------------------------
//
CMPXPlaybackCmdBuffer* CMPXPlaybackCmdBuffer::NewL(
    MMPXPlaybackCmdBufferObserver& aObserver)
    {
    CMPXPlaybackCmdBuffer* h=new(ELeave)CMPXPlaybackCmdBuffer(aObserver);
    CleanupStack::PushL(h);
    h->ConstructL();
    CleanupStack::Pop(h);
    return h;
    }

// ----------------------------------------------------------------------------
// C++ constructor can NOT contain any code that might leave.
// ----------------------------------------------------------------------------
//
CMPXPlaybackCmdBuffer::CMPXPlaybackCmdBuffer(
    MMPXPlaybackCmdBufferObserver& aObserver)
:   CTimer(EPriorityStandard),
    iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }


// ----------------------------------------------------------------------------
// Is this a command controlled by command buffer
// ----------------------------------------------------------------------------
//
TBool CMPXPlaybackCmdBuffer::CommandForBuffering(TMPXPlaybackCommand aCmd) const
    {
    switch(aCmd)
        {
        case EPbCmdPlay:
        case EPbCmdStop:
        case EPbCmdPause:
        case EPbCmdPlayPause:
        case EPbCmdNext:
        case EPbCmdPrevious:
            return ETrue;
        default:
            return EFalse;
        }
    }

// ----------------------------------------------------------------------------
// Handle time out event
// ----------------------------------------------------------------------------
//
void CMPXPlaybackCmdBuffer::RunL()
    {
    MPX_DEBUG1("CMPXPlaybackCmdBuffer::RunL() entering");

    if (!iEventProcessing && IsCommandBuffered())
    //
    // Client is not currently processing any events, and there are new
    // events to deliver
    //
        {
        if (iEvent.iNavKeyBuffered)
            {
            iObserver.HandleCmdBufferSkipEventL( EPbsSkipEventEnd ); // notify end skip event
            }
        iIsCommandBuffered=EFalse;
        iEventProcessing=ETrue; // Handled by client
        iObserver.HandleCmdBufferEventL(iEvent);
        }

    MPX_DEBUG1("CMPXPlaybackCmdBuffer::RunL() exiting");
    }

// ----------------------------------------------------------------------------
// Handle leave in command handling
// ----------------------------------------------------------------------------
//
TInt CMPXPlaybackCmdBuffer::RunError(TInt /*aError*/)
    {
    MPX_DEBUG1("==>CMPXPlaybackCmdBuffer::RunL()");
    ClearCommands();
    iEventProcessing=EFalse;
    MPX_DEBUG1("<==CMPXPlaybackCmdBuffer::RunL()");
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Buffer a playback/skip command, along with the current state and position in
// the track. Update playback state/position internally. When time-out, restore
// state/position to playback engine.
// ----------------------------------------------------------------------------
//
void CMPXPlaybackCmdBuffer::BufferCommandL(TMPXPlaybackCommand aCmd,
                                           TMPXPlaybackState aState,
                                           TInt aPos)
    {
    MPX_DEBUG4("CMPXPlaybackCmdBuffer::BufferCommandL(%d, %d, %d) entering", aCmd, aState, aPos);

    if (!CommandForBuffering(aCmd) ||
        (aCmd==EPbCmdPlay && aState==EPbStatePlaying) ||
        (aCmd==EPbCmdPause && aState==EPbStatePaused))
        {
        // This command does not change any state, so make sure
        // to negate any previous commands
        ClearCommands();
        return;
        }

    TBool saveIsCmdBuffered = iIsCommandBuffered;
    iBufferTime = KPbMediaKeyIntervalMicroSeconds;
    const TInt KPbMediaKeyNoIntervalMicroSeconds=10;  // 0.00001s
    const TInt KPbMediaKeySkipIntervalMicroSeconds=200000;  // 0.2s or 200ms
    const TInt KPbMediaKeyPauseIntervalMicroSeconds=100000;  // 0.1s or 100ms
    Cancel(); // Stop timer and wait again for any other events

    if (!iIsCommandBuffered)
        {
        iIsCommandBuffered=ETrue;
        iEvent.iTrackOffset=0;
        iEvent.iState=aState;
        iEvent.iNavKeyBuffered=EFalse;
        }

    if (!iEvent.iNavKeyBuffered)
        {
        iEvent.iPos = aPos;
        }

    switch(aCmd)
        {
        case EPbCmdNext:
            {
            iEvent.iTrackOffset++;
            iEvent.iNavKeyBuffered++;
            TBool savedStatus( iEventProcessing );
            iEventProcessing = ETrue;
            iObserver.HandleCmdBufferSkipEventL( EPbsSkipEventNext ); // notify skip event
            iEventProcessing = savedStatus;
			if ( !saveIsCmdBuffered )
				{
				iBufferTime = KPbMediaKeySkipIntervalMicroSeconds; // 0.2s or 200ms
				}
            break;
            }
        case EPbCmdPrevious:
            {
            if (!iEvent.iNavKeyBuffered &&
                (aState==EPbStatePlaying || aState==EPbStatePaused) &&
                 iEvent.iPos > KPbReplayThresholdMiliSeconds)
                {
                MPX_DEBUG1("CMPXPlaybackCmdBuffer::BufferCommandL() Setting position 0");
                aPos=0;
                }
            else
                {
                TBool savedStatus( iEventProcessing );
                iEventProcessing = ETrue;
                iObserver.HandleCmdBufferSkipEventL( EPbsSkipEventPrevious ); // notify skip event
                iEventProcessing = savedStatus;
                iEvent.iTrackOffset--;
                }
            iEvent.iNavKeyBuffered++;
            break;
            }
        case EPbCmdPlay:
            iEvent.iState = EPbStatePlaying;
            if ( !saveIsCmdBuffered )
                {
                iBufferTime = KPbMediaKeyNoIntervalMicroSeconds;
                }
            break;
        case EPbCmdPause: // From play to pause
            if (EPbStatePlaying == iEvent.iState)
                {
                iEvent.iState = EPbStatePaused;
				if ( !saveIsCmdBuffered )
					{
					iBufferTime = KPbMediaKeyPauseIntervalMicroSeconds;
					}
                }
             break;
        case EPbCmdPlayPause:
            {
            if ( EPbStatePlaying == iEvent.iState )
                {
                iEvent.iState = EPbStatePaused;
				if ( !saveIsCmdBuffered )
					{
					iBufferTime = KPbMediaKeyPauseIntervalMicroSeconds;
					}
				break;
                }
            else
                {
                iEvent.iState = EPbStatePlaying;
                if ( !saveIsCmdBuffered )
                    {
                    iBufferTime = KPbMediaKeyNoIntervalMicroSeconds;
                    }
                }
            break;
            }
        case EPbCmdStop:
            iEvent.iState = EPbStateStopped;
            break;
        default:
            break;
        }
    // maintain pos unless track has changed
    iEvent.iPos=iEvent.iTrackOffset ? 0 : aPos;
    //
    if (!iEventProcessing) // If client isn't processing event
        {
        After(iBufferTime);
        }

    MPX_DEBUG1("CMPXPlaybackCmdBuffer::BufferCommandL() exiting");
    }

// ----------------------------------------------------------------------------
// Called by client when finished processing commands in buffer
// ----------------------------------------------------------------------------
//
void CMPXPlaybackCmdBuffer::CompleteCommand()
    {
    MPX_DEBUG1("CMPXPlaybackCmdBuffer::CompleteCommand() entering");

    iEventProcessing=EFalse; // Client finished processing event
    if (IsCommandBuffered() && !IsActive())
        {
        After(iBufferTime);
        }

    MPX_DEBUG1("CMPXPlaybackCmdBuffer::CompleteCommand() exiting");
    }

// ----------------------------------------------------------------------------
// Check if any commands buffered
// ----------------------------------------------------------------------------
//
TBool CMPXPlaybackCmdBuffer::IsCommandBuffered() const
    {
    return iIsCommandBuffered;
    }

// ----------------------------------------------------------------------------
// Clear all commands
// ----------------------------------------------------------------------------
//
void CMPXPlaybackCmdBuffer::ClearCommands()
    {
    iIsCommandBuffered=EFalse;
    Cancel();
    }



