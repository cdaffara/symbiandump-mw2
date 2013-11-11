/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The functions in this module implements the common behavior
*                for the audio encoder base class.
*
*/



// INCLUDE FILES
#include	"AdvancedAudioEncoder.h"
#include 	<mmfdatabuffer.h>
#include    "DebugMacros.h"

// CONSTANTS
const TInt KEmptyBuffer        = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CAdvancedAudioEncoder::CAdvancedAudioEncoder(TInt aPriority)
    :   CActive(aPriority),
    	iState(EIdle),
        iSharedBuffers(NULL),
        iMMFDevSound(NULL),
        iObserver(NULL)
    {
#ifdef _DEBUG
    RDebug::Print(_L("CAdvancedAudioEncoder::CAdvancedAudioEncoder()"));
#endif
    }

// Destructor
EXPORT_C CAdvancedAudioEncoder::~CAdvancedAudioEncoder()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CAdvancedAudioEncoder::~CAdvancedAudioEncoder()"));
#endif
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::SetDevSound
// Sets the DevSound instance.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::SetDevSound(
	CMMFDevSound& aDevSound )
	{
	iMMFDevSound = &aDevSound;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::SetObserver
// Sets the observer instance.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::SetObserver(
	MAdvancedAudioEncoderObserver& aObserver )
	{
	iObserver = &aObserver;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::SetSinkBuffers
// Sets the sink buffer for encoding operations.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::SetSinkBuffers(
	RPointerArray<CMMFDataBuffer>* aBuffers )
	{
	iSharedBuffers = aBuffers;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::EmptyBuffer
// Request to fill the specified buffer with converted data.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::EmptyBuffer(
	CMMFBuffer* aBuffer)
	{
#ifdef _DEBUG
//    RDebug::Print(_L("CAdvancedAudioEncoder::EmptyBuffer"));
#endif
	iBufferToEmpty = aBuffer; // store away the buffer
	iState = EEncoding;

	iRequest = KEmptyBuffer;
	Ready(KErrNone);
    SetActive();
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::Stop
// Stops encoding process.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::Stop()
	{
	DP1(_L("CAdvancedAudioEncoder::Stop start iNextBuffer[%d]"), iNextBuffer);

    // There are 2 situations in which this function is called.
    // First when Devsound has already given a buffer to Encoder to be handled.
    // In which case, one needs to finish handling it by calling HandleEmptyBufferL().
    // Secondly, in a situation where the current shared buffer is being filled (partially full)
    // in which case, the buffer needs to be flushed
	if (iBufferToEmpty && (iState == EEncoding))
		{
	    DP1(_L("CAdvancedAudioEncoder::Stop setting last buffer[%d]"), iBufferToEmpty);
		iBufferToEmpty->SetLastBuffer(ETrue);
		TRAPD(err, HandleEmptyBufferL());
		if ( err )
			{
			DP1(_L("CAdvancedAudioEncoder::Stop err[%d]"), err);
			iState = EIdle;
			iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, err));
			}		
        }
	else if (iNextBuffer)
	    { // a record controller may call stop before buffers are initialized
        if (iNextBuffer->Status() == EBeingFilled)
            {
            DP1(_L("CAdvancedAudioEncoder::Stop setting buffer[%d] to EFull"), iNextBuffer);
            iNextBuffer->SetStatus(EFull);
            DP1(_L("CAdvancedAudioEncoder::Stop emptying buffer[%d]"), iNextBuffer);
            iObserver->EmptyBuffer(iNextBuffer);
            }
	    }
		
	iState = EIdle;
    DP0(_L("CAdvancedAudioEncoder::Stop end"));
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::Ready
// Utility function to post a request complete
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::Ready(
    const TInt aStatus)
    {
    TRequestStatus* stat = &iStatus;
    User::RequestComplete(stat, aStatus);
    }

// -----------------------------------------------------------------------------
// CDevSoundAudioInput::NextSharedBufferL
// Determine the next available shared buffer from the pool of shared buffers
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::NextSharedBufferL()
	{
	if ( (*iSharedBuffers)[iSharedBufferIndex]->Status() == EAvailable )
		{
		iNextBuffer = (*iSharedBuffers)[iSharedBufferIndex];
		iSharedBufferIndex++;
		if ( iSharedBufferIndex == iSharedBuffers->Count() )
			{
			iSharedBufferIndex = 0;
			}
		}
	else
		{
#ifdef _DEBUG
	    RDebug::Print(_L("CAdvancedAudioEncoder::NextSharedBufferL - KErrOverflow"));
#endif
		User::Leave(KErrOverflow);
		}
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::RunL
// Invoke by the active scheduler when a request completes
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::RunL()
	{
#ifdef _DEBUG
//    RDebug::Print(_L("CAdvancedAudioEncoder::RunL"));
#endif
    switch(iRequest)
		{
		case KEmptyBuffer:
			if ( iState == EEncoding )
				{
				TRAPD(err, HandleEmptyBufferL());
				if ( err )
					{
#ifdef _DEBUG
					RDebug::Print(_L("CAdvancedAudioEncoder::RunL err[%d]"), err);
#endif
					// if error is encountered, the state is reset and event is generated and the recording is stopped.
					//err = KErrDied;
					iState = EIdle;
					iObserver->SendEvent(TMMFEvent(KMMFEventCategoryPlaybackComplete, err));
					}
				}
			break;

		default:
			break;
		};
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioEncoder::DoCancel
// Cancels the current and any on going requests/tasks.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioEncoder::DoCancel()
    {
    }

// End of file
