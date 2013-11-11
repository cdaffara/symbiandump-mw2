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
* Description:  This file contains the base class from which specific audio
*                play controllers are derived. This class encapsulates common
*                behavior for all audio play controllers.
*
*/


// INCLUDE FILES
#include "AdvancedAudioPlayController.h"
#include "AdvancedAudioResource.h"
#include "DebugMacros.h"
#include <AudioOutput.h>
#include <MetaDataFieldContainer.h>
#include <MetaDataUtility.h>
#include <mmfformatimplementationuids.hrh>
#include <mmfmeta.h>
#include <MultimediaDataSourceEvents.h>
#include <MultimediaDataSourceFactory.h>
#include <oma2dcf.h>

// KMdaRepeatForever constant is defined in this file
#include <mda/common/resource.h>
// CONSTANTS
const TInt KOneThousandMilliSecond = 1000;      // 1 sec

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::CAdvancedAudioPlayController
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CAdvancedAudioPlayController::CAdvancedAudioPlayController()
	:	iState(EStopped),
		iAudioOutput(NULL),
		iAudioResource(NULL),
		iAudioUtility(NULL),
		iDisableAutoIntent (EFalse),
    iDecoderExists(EFalse),
    iRepeatCount(-1),
    iCurrentRepeatCount(0),
    iLoopPlayEnabled(EFalse),
    iRepeatForever(EFalse),
    iTrailingSilenceMs(0),
    iSavedTimePositionInMicroSecs(0) //,
	{
    iEventsEnabled = EFalse;

    RThread().SetPriority(EPriorityRealTime);
    }

EXPORT_C void CAdvancedAudioPlayController::ConstructL()
	{
	DP1(_L("CAdvancedAudioPlayController::ConstructL this[%x]"), this);
	CAdvancedAudioController::ConstructL();

    // Construct custom command parsers
    CMMFAudioPlayDeviceCustomCommandParser* audPlayDevParser =
    	CMMFAudioPlayDeviceCustomCommandParser::NewL(*this);
    CleanupStack::PushL(audPlayDevParser);
    AddCustomCommandParserL(*audPlayDevParser);
    CleanupStack::Pop(audPlayDevParser);

    CMMFAudioPlayControllerCustomCommandParser* audPlayConParser =
    	CMMFAudioPlayControllerCustomCommandParser::NewL(*this);
    CleanupStack::PushL(audPlayConParser);
    AddCustomCommandParserL(*audPlayConParser);
    CleanupStack::Pop(audPlayConParser);

	CMMFDRMCustomCommandParser* drmParser = CMMFDRMCustomCommandParser::NewL(*this);
	CleanupStack::PushL(drmParser);
	AddCustomCommandParserL(*drmParser);
	CleanupStack::Pop(drmParser);

    CStreamControlCustomCommandParser* scCCParser = CStreamControlCustomCommandParser::NewL(*this);
    CleanupStack::PushL(scCCParser);
    AddCustomCommandParserL(*scCCParser);
    CleanupStack::Pop(scCCParser);

    // new custom command parser to implement the MapcSetRepeats custom command from the client
    CMMFAudioPlayControllerSetRepeatsCustomCommandParser* audPlayConSetRepeatsParser =
            CMMFAudioPlayControllerSetRepeatsCustomCommandParser::NewL(*this);
    CleanupStack::PushL(audPlayConSetRepeatsParser);
    AddCustomCommandParserL(*audPlayConSetRepeatsParser);
    CleanupStack::Pop(audPlayConSetRepeatsParser);
	}

// Destructor
EXPORT_C CAdvancedAudioPlayController::~CAdvancedAudioPlayController()
    {
    DP1(_L("CAdvancedAudioPlayController::~CAdvancedAudioPlayController start this[%x]"), this);
    DP4(_L("CAdvancedAudioPlayController::~CAdvancedAudioPlayController iState[%d] iWait[%d] iBlockSetPos[%d] iSharedBuffers.Count[%d]"),
            iState, iWait, iBlockSetPos, iSharedBuffers.Count());
	RThread().SetPriority(EPriorityNormal);

    if (iMetaDataEntries.Count())
    	{
        iMetaDataEntries.ResetAndDestroy();
		iMetaDataEntries.Close();
		}
		
    if (iTrailingSilenceTimer)
        {
        delete iTrailingSilenceTimer;
        iTrailingSilenceTimer = NULL;
        }

	delete iDataSourceAdapter;
	delete iWait;
	delete iBlockSetPos;
	delete iBlockPrime;
      
	iSharedBuffers.ResetAndDestroy();
	iSharedBuffers.Close();
    DP0(_L("CAdvancedAudioPlayController::~CAdvancedAudioPlayController end"));
    }

    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::DoReadHeaderL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::DoReadHeaderL(CMMFDataBuffer* /*aBuffer*/)
    {
    // do nothing
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::DoSetPositionL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::DoSetPositionL(const TTimeIntervalMicroSeconds& aTimePos)
    {
    DP0(_L("CAdvancedAudioPlayController::DoSetPositionL start -----------"));
    TInt err = KErrNone;
    TInt foundPosition;
    TInt foundTimeMs;
    TBool seekFwd = EFalse;
	TUint timeMs = aTimePos.Int64()/1000;
    DP2(_L("CAdvancedAudioPlayController::DoSetPositionL timeMs[%u] pre[%d]"), timeMs, (iPreSeekTime.Int64()/1000));
    // Fix for the error ou1cimx1#151598 - ESLM-7T5GJH
    // unregisters any old position entries from the FrameTable before setting any new play seek positions
    iAudioUtility->SeekToTimeMs(KMaxTUint);

	if (aTimePos < 0)
		{
		timeMs = 0;
		}
		
    // get current position in time
    // if we need to play to seek forward (not in table), the time will be advanced immediately
    // if we pause before we get there, pause will set pos to that time - which is what we want.
    // but we need to remember that we are not there yet so we can still seek forward. So we use pre-seek time.
    if (!iPlaySeeking)
        {
        iPreSeekTime = PositionL();
	    DP1(_L("CAdvancedAudioPlayController::DoSetPositionL save pre-seek time[%d]"), iPreSeekTime.Int64()/1000);
        }
    if (aTimePos > iPreSeekTime)
    	{
    	seekFwd = ETrue;
    	}
    DP1(_L("CAdvancedAudioPlayController::DoSetPositionL seekFwd[%d]"), seekFwd);

   	TUint const seekTime = timeMs;
    TUint position = 0;
    TBool posFoundInTable = EFalse;
    
    // look in current buffers first
    DP0(_L("CAdvancedAudioPlayController::DoSetPositionL not time seekable"));
    err = SetPositionInSharedBuffers(timeMs, foundPosition, foundTimeMs);
    if (!iSourceIsTimeSeekable && !iSourceIsPosSeekable)
        { 
    	if (err == KErrNone)
	        { // found exact position
		    DP0(_L("CAdvancedAudioPlayController::DoSetPositionL found exact position"));
    	    iCurrentPosition = foundPosition; // absolute position into file or content
        	iTimePositionInMicroSecs = foundTimeMs;
        	iTimePositionInMicroSecs *=1000;
            DP1(_L("CAdvancedAudioPlayController::DoSetPositionL iTimePositionInMicroSecs[%d]"),foundTimeMs);
			iAudioUtility->SetSourceReference(iTimePositionInMicroSecs/1000, iCurrentPosition-iHeaderOffset-iSyncOffset);
		    RefillPreceedingBuffersL();
    		DP0(_L("CAdvancedAudioPlayController::DoSetPositionL end -----------"));
    	    return;
        	}
	    // err means it just found something close
        ASSERT(err != KErrGeneral);
        if (err == KErrGeneral)
            { // KErrGeneral means it did not find anything and we have to stop.
    		DP0(_L("CAdvancedAudioPlayController::DoSetPositionL abort -------"));
    		TMMFEvent event(KMMFEventCategoryPlaybackComplete,KErrGeneral);
    		SendEvent(event);
            return;
            }
        // source is not seekable so use buffered data
	    // use closest time and position found above
		DP0(_L("CAdvancedAudioPlayController::DoSetPositionL using closest position"));
    	iCurrentPosition = foundPosition;
        iTimePositionInMicroSecs = foundTimeMs;
        iTimePositionInMicroSecs *=1000;
        DP1(_L("CAdvancedAudioPlayController::DoSetPositionL iTimePositionInMicroSecs[%d]"),foundTimeMs);
		iAudioUtility->SetSourceReference(iTimePositionInMicroSecs/1000, iCurrentPosition-iHeaderOffset-iSyncOffset);
		RefillPreceedingBuffersL();
        }
    if (iSourceIsTimeSeekable)
	  	{ // time seekable source won't have accurate table positions
	    DP0(_L("CAdvancedAudioPlayController::DoSetPositionL is time seekable"));
        iPreSeekTime = 0;
		iDataSourceAdapter->SourceStopL(); // clear the buffers in the source before seeking and priming it
		iDataSourceAdapter->SourcePrimeL();
	  	err = iDataSourceAdapter->SeekToTime(seekTime, timeMs);
	    iAudioUtility->ResetTable();
		iDataSourceAdapter->SourcePlayL();
		iTimePositionInMicroSecs = timeMs;
		iTimePositionInMicroSecs *=1000;
        DP1(_L("CAdvancedAudioPlayController::DoSetPositionL iTimePositionInMicroSecs[%d]"),timeMs);
		iSourceReadPosition = 0; // we don't know the position - source doesn't give us that
	    // iCurrentPosition = iSourceReadPosition;
		iCurrentPosition = KMaxTUint;
		iAudioUtility->SetSourceReference(iTimePositionInMicroSecs/1000, 0);
        TRAP(err, RefillSharedBuffersL());
    	}
    else if (iSourceIsPosSeekable)
    	{
	    DP0(_L("CAdvancedAudioPlayController::DoSetPositionL is position seekable"));
	    TInt err = iAudioUtility->FindFramePosFromTime(timeMs, position);
	    if (err == KErrNone)
    		{
   			DP0(_L("CAdvancedAudioPlayController::DoSetPositionL pos found in table"));
    		posFoundInTable = ETrue;
    		}
	    TBool tryForAccuracy = EFalse;
		if (posFoundInTable)
			{ // seek in source is taken care of below
			// The position was found in the table, and assuming it was found in the low res table
			// lets try to seek fwd to the exact position.
			// The reason for this is that the record utility does a stop play instead of a pause play
			// The inaccuracy in the resume position is then visible, using the low res table.
			tryForAccuracy = ETrue;
			}
		else
			{ // then we must be going forward or just starting
   			DP0(_L("CAdvancedAudioPlayController::DoSetPositionL pos not found in table"));
   			err = iAudioUtility->LastFramePos(position);
   			if (err) // use KErrDoesNotExist or something
   				{ // do this a little better, maybe return something different from LastFramePos
   				position = 0;
   				timeMs = 0;
   				}
   			else
	   			{
			    err = iAudioUtility->FindFrameTimeFromPos(timeMs, position);
   				}	
			}
		// seek in source		
		DP0(_L("CAdvancedAudioPlayController::DoSetPositionL seek in the source"));
	    iSourceReadPosition = position+iHeaderOffset+iSyncOffset;
		iDataSourceAdapter->SourceStopL(); // clear the buffers in the source before seeking and priming it
		iDataSourceAdapter->SourcePrimeL();
	    iDataSourceAdapter->SeekToPosition(iSourceReadPosition);
	    iAudioUtility->ResetTable();
		iDataSourceAdapter->SourcePlayL();
	    iCurrentPosition = iSourceReadPosition;
	    iTimePositionInMicroSecs = timeMs;
	    iTimePositionInMicroSecs *=1000; // bh 21Mar08 // need this time to correlate to the position found
        DP1(_L("CAdvancedAudioPlayController::DoSetPositionL iTimePositionInMicroSecs[%d]"),timeMs);
		iAudioUtility->SetSourceReference(iTimePositionInMicroSecs/1000, iCurrentPosition-iHeaderOffset-iSyncOffset);
        TRAP(err, RefillSharedBuffersL());
		DP1(_L("CAdvancedAudioPlayController::DoSetPositionL RefillSharedBuffersL[%d]"),err);
        if (seekFwd && (!posFoundInTable || tryForAccuracy))
           	{
			// seeking forward past entries available in table
			// will scan buffers for frame lengths without rendering 
			// which will add table entries and notify when pos is reached
   			DP0(_L("CAdvancedAudioPlayController::DoSetPositionL do non-render seek fwd"));
           	SeekToTimeMsL(seekTime); // this will set iTimePositionInMicroSecs to the seek fwd time
           	}
       	else
       	    {
            iPreSeekTime = 0;
       	    }
    	}
    else
        { // continue from closest position and render once the time is reached
		DP0(_L("CAdvancedAudioPlayController::DoSetPositionL src not seekable continue"));
		if (seekFwd)
			{
			DP0(_L("CAdvancedAudioPlayController::DoSetPositionL src not seekable do non-render seek fwd"));
	       	SeekToTimeMsL(seekTime);
			}
        }
    DP0(_L("CAdvancedAudioPlayController::DoSetPositionL end -----------"));
    }
    
void CAdvancedAudioPlayController::SeekToTimeMsL(TUint aTimeMs)
	{ // used when seeking forward without rendering
	DP1(_L("CAdvancedAudioPlayController::SeekToTimeMs[%d]"),aTimeMs);

	iAudioUtility->SeekToTimeMs(aTimeMs);
	iPlaySeeking = ETrue;
	// ok to set the time forward because during this seek, devsound won't be getting
	// any data - so it's samples played would stay 0 and not keep incrementing if position is called
    iTimePositionInMicroSecs = aTimeMs;
    iTimePositionInMicroSecs *=1000;
    DP1(_L("CAdvancedAudioPlayController::SeekToTimeMs iTimePositionInMicroSecs[%d]"),iTimePositionInMicroSecs);
    if (iState == EPaused)
        {
        PlayForPauseSeekL();
        }
    else if (iState == EInitialized)
    	{
		PlayForInitPositionL();
    	}
	}
    
void CAdvancedAudioPlayController::GoToInitPositionL()
	{
    DP1(_L("CAdvancedAudioPlayController::GoToInitPositionL[%d]"),iInitPosition.Int64());
	if (iInitPosition >= 0)
		{ // -1 is inactive, >=0 will seek
		TTimeIntervalMicroSeconds position = iInitPosition;
	    DP0(_L("CAdvancedAudioPlayController::GoToInitPositionL clearing iInitPosition and seeking"));
		iInitPosition = -1;		
		DoSetPositionL(position);
		}
    // Register for the Playwindow end position, if PlayWindow is created
    if (iPlayWindowEndPosition > 0)
        {
        iAudioUtility->SetPlayWindowEndTimeMs(iPlayWindowEndPosition.Int64()/1000);
        }
	}
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::RefillSharedBuffersL
// -----------------------------------------------------------------------------
//
void CAdvancedAudioPlayController::RefillSharedBuffersL()
    {
	DP0(_L("CAdvancedAudioPlayController::RefillSharedBuffersL"));
    TInt i;
    ClearSharedBuffersL();
    for (i=0; i < iSharedBufferMaxNum; i++)
        {
        FillSharedBufferL(iSharedBuffers[i]);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::ReinitSharedBuffersL
// -----------------------------------------------------------------------------
//
void CAdvancedAudioPlayController::ClearSharedBuffersL()
    {
	DP0(_L("CAdvancedAudioPlayController::ClearSharedBuffersL"));
    for (TInt i=0; i < iSharedBufferMaxNum; i++)
        {
    	static_cast<CMMFDataBuffer*>(iSharedBuffers[i])->Data().SetLength(0);
    	iSharedBuffers[i]->SetLastBuffer(EFalse);
    	iSharedBuffers[i]->SetStatus(EAvailable);
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::InitSharedBuffersL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::InitSharedBuffersL()
    {
	DP0(_L("CAdvancedAudioPlayController::InitSharedBuffersL"));
    TInt i;
	ResetSharedBuffersL(iSharedBufferMaxNum, iSharedBufferMaxSize);
    for (i=0; i < iSharedBufferMaxNum; i++)
        {
        FillSharedBufferL(iSharedBuffers[i]);
        }
    }

EXPORT_C void CAdvancedAudioPlayController::RefillPreceedingBuffersL()
	{
	DP0(_L("CAdvancedAudioPlayController::RefillPreceedingBuffersL"));
	TInt curIndex = iSharedBufferIndex;
	TInt curPos = iSharedBuffers[curIndex]->FrameNumber();
	TInt maxNum = iSharedBufferMaxNum;
	// buffers are filled in increasing order
	TInt index = iSharedBufferIndex;
	TInt pos;
	TInt stat;
	for (TInt i=0; i< maxNum-1; i++)
		{
		index++;
		if (index >= maxNum)
			{
			index = 0;
			}
        stat = iSharedBuffers[index]->Status();
		pos = iSharedBuffers[index]->FrameNumber();
		DP4(_L("CAdvancedAudioPlayController::RefillPreceedingBuffersL index[%d] stat[%d] pos[%d] bufpos[%d]"), 
				index, stat, pos, iSharedBuffers[index]->Position());
		if ((pos < curPos) && (stat == EFull))
			{
			DP1(_L("CAdvancedAudioPlayController::RefillPreceedingBuffersL index[%d]"),index);
	        FillSharedBufferL(iSharedBuffers[index]);
			}
		}
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::SourceSinkStopL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::SourceSinkStopL()
    {
	iDataSourceAdapter->SourceStopL();

    if (iDataSink->DataSinkType() == KUidMmfFileSink)
        {
        iDataSink->SinkStopL();
        }
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::FillSharedBufferL
// Read the next block of data from source into the given buffer.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::FillSharedBufferL(
    CMMFBuffer* aBuffer)
    {
    DP2(_L("CAdvancedAudioPlayController::FillSharedBufferL ptr[%x], iState[%d]"),
            static_cast<CMMFDataBuffer*>(aBuffer)->Data().Ptr(), iState);

    aBuffer->SetLastBuffer(EFalse);
    aBuffer->SetStatus(EBeingFilled);
	aBuffer->SetPosition(0);
	static_cast<CMMFDataBuffer*>(aBuffer)->Data().SetLength(0);
			
	TMediaId mediaId(KUidMediaTypeAudio);
	iDataSourceAdapter->FillBufferL(aBuffer, this, mediaId);
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::FindBufferFromPos
// -----------------------------------------------------------------------------
// aPos is relative to source position in that it includes any header offsets
// Will return KErrNotFound if the position is not found in the buffers.
// Will look only in full buffers.
EXPORT_C TInt CAdvancedAudioPlayController::FindBufferFromPos(TUint aPos)
	{
    DP1(_L("CAdvancedAudioPlayController::FindBufferFromPos, aPos[%d]"), aPos);

    TInt retVal = KErrNotFound;
    
    for (TInt i = 0; i < iSharedBufferMaxNum; i++)
        {
        DP5(_L("CAdvancedAudioPlayController::FindBufferFromPos iSharedBuffers[%d], ptr=%x, FrameNumber[%d], Status[%d], LastBuffer[%d]"), i, iSharedBuffers[i]->Data().Ptr(), iSharedBuffers[i]->FrameNumber(), iSharedBuffers[i]->Status(), iSharedBuffers[i]->LastBuffer());
        TUint framenumber = iSharedBuffers[i]->FrameNumber();
        TUint buffersize = iSharedBuffers[i]->BufferSize();
        DP6(_L("CAdvancedAudioPlayController::FindBufferFromPos iSharedBuffers[%d], ptr=%x, FrameNumber[%d], Status[%d], LastBuffer[%d], BufferSize[%d]"), i, iSharedBuffers[i]->Data().Ptr(), framenumber, iSharedBuffers[i]->Status(), iSharedBuffers[i]->LastBuffer(), buffersize);
        if ((aPos >= framenumber) && (aPos < (framenumber+buffersize)) &&
//        if ((aPos >= framenumber) && (aPos < (framenumber+iSharedBufferMaxSize)) &&
        	(iSharedBuffers[i]->Status() == EFull))
            {
            iSharedBuffers[i]->SetPosition(aPos-framenumber);
            DP1(_L("CAdvancedAudioPlayController::FindBufferFromPos setting position to [%d]"), iSharedBuffers[i]->Position());
			retVal = i;
			break;
            }
        }
    DP1(_L("CAdvancedAudioPlayController::FindBufferFromPos retVal[%d]"), retVal);
	return retVal;
	}	

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::SetPositionInSharedBuffers
// -----------------------------------------------------------------------------
//
/*
This function will look in current buffers that aren't being filled and set the
buffer and position in it to the corresponding current time.
An error will be returned if the exact position was not located in a full buffer.
A closest available will be calculated. 
The buffers past the set will be sent to be filled.
If a buffer is full, it can be used.
If it is being filled, it can not be used here.
If a buffer is other than full or being filled, it should be sent to be filled
in the correct sequence after the buffer position is determined.
*/
EXPORT_C TInt CAdvancedAudioPlayController::SetPositionInSharedBuffers(TUint aTimeMs, TInt& aFoundPosition, TInt& aFoundTimeMs)
    {
    TInt err = KErrNone;
    TUint position;
    TUint timeMs;

    DP1(_L("CAdvancedAudioPlayController::SetPositionInSharedBuffersL -- start this[%x]"), this);
    
    for (TInt i = 0; i < iSharedBufferMaxNum; i++)
        {
        DP6(_L("CAdvancedAudioPlayController::SetPositionInSharedBuffersL iSharedBuffers[%d] ptr[%x] FrameNumber[%d] Status[%d] LastBuffer[%d] bufpos[%d]"),
         	i, iSharedBuffers[i]->Data().Ptr(), iSharedBuffers[i]->FrameNumber(), iSharedBuffers[i]->Status(), iSharedBuffers[i]->LastBuffer(), iSharedBuffers[i]->Position());
        
        if (iSharedBuffers[i]->Status() != EBeingFilled)
            { // reset the positions on the buffers that the source does not have
            iSharedBuffers[i]->SetPosition(0);
            }
        
        if (iSharedBuffers[i]->Status() == EBeingEmptied)
        	{
	        iSharedBuffers[i]->SetStatus(EFull);
        	}
        }
    timeMs = aTimeMs;
    // get the closest position from the table for the given time
    err = iAudioUtility->FindFramePosFromTime(timeMs, position); // err indicates if position found, or just what was available
	// see if the buffer with this position exists
    iSharedBufferIndex = FindBufferFromPos(position + iHeaderOffset + iSyncOffset);
	TUint framenumber = 0;
		
	if (iSharedBufferIndex >= 0)
	    {
	    }
	else
	    {
        DP0(_L("CAdvancedAudioPlayController::SetPositionInSharedBuffersL, buffer not found"));
        err = KErrNotFound;
        // find a frame in current buffers
        TInt minindex = 0;
        TInt tempframenum = -1;

        for (TInt j = 0; j < iSharedBufferMaxNum; j++)
            {
            if (iSharedBuffers[j]->Status() == EFull)
            	{
	            framenumber = iSharedBuffers[j]->FrameNumber();
            	}

            if (((framenumber < tempframenum) || (tempframenum == -1)) && 
                (iSharedBuffers[j]->Status() == EFull))
                {
                tempframenum = framenumber;
                minindex = j;
                }
            }
		if (tempframenum == -1)
			{
			return KErrNotFound;
			}
	    iSharedBufferIndex = minindex;
	    framenumber = iSharedBuffers[iSharedBufferIndex]->FrameNumber();
	    // find a frame start
	    if (framenumber < (iHeaderOffset+iSyncOffset))
	        { // special case for buffer that still has header info
	        position = 0;
	        timeMs = 0;
	        }
	    else
	    	{
		    position = framenumber-iHeaderOffset-iSyncOffset;
		    TInt err1 = iAudioUtility->FindFrameTimeFromPos(timeMs, position);
//#ifdef _DEBUG
		    if (err1 != KErrNone)
		    	{ // we have to find a location in the table for the buffers we have 
		    	// amr may scan whole file and buffers won't contain start data
		    	//   but it would only happen for seekable source
	    	    DP0(_L("CAdvancedAudioPlayController::SetPositionInSharedBuffersL, position in buffers not found -- aborting"));
                return KErrGeneral;
//	    	    ASSERT(err1 == KErrNone);
	    		}
//#endif	    	
	    	}
        }
           
    aFoundPosition = position + iHeaderOffset + iSyncOffset;
    aFoundTimeMs = timeMs;
    framenumber = iSharedBuffers[iSharedBufferIndex]->FrameNumber();
    iSharedBuffers[iSharedBufferIndex]->SetPosition(aFoundPosition-framenumber);
    // after this send buffers to be filled that need to be filled to maintain correct sequence.
    DP2(_L("CAdvancedAudioPlayController::SetPositionInSharedBuffersL, iSharedBufferIndex[%d] pos[%d]-- end"), 
    		iSharedBufferIndex, aFoundPosition-framenumber);
    return err;    
    }


// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::UpdateDuration
//
// Possible parameters are:
// -1 = just update duration
//  0 = triggers duration changed immediately
//  a number = will trigger duration changed event if duration has changed this amount given
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::UpdateDuration(TInt aLimitInMilliSecond)
    { // updates iDuration
    DP2(_L("CAdvancedAudioPlayController::UpdateDuration, iEventsEnabled[%d], aLimitInMilliSecond[%d]"), iEventsEnabled, aLimitInMilliSecond);
    
    if (iDurationFrozen)
        {
        return KErrNone;
        }
        
    if (aLimitInMilliSecond < -1)
        {
        return KErrArgument;
        }
	
	if (!iAudioUtility)
		{
		return KErrNone;
		}
		
    TInt64 duration = iAudioUtility->Duration(); // calculates new duration
    if ((aLimitInMilliSecond == -1) || !iEventsEnabled)
    	{
        iDuration = duration;
    	}
    else if ((iDuration != duration) && (Abs(duration-iDuration) >= (aLimitInMilliSecond*1000)))
        {
        iDuration = duration;
        SendEventToClient(TMMFEvent(KStreamControlEventDurationChanged, KErrNone));
        }
    DP1(_L("CAdvancedAudioPlayController::UpdateDuration, iDuration[%d]"), iDuration);
    return KErrNone;
    }

EXPORT_C TInt CAdvancedAudioPlayController::UpdateBitRate()
    {
    DP0(_L("CAdvancedAudioPlayController::UpdateBitRate"));
    if (iBitRateFrozen)
        {
	    DP0(_L("CAdvancedAudioPlayController::UpdateBitRate frozen"));
        return KErrNone;        
        }
    if (iAudioUtility)
        {
        iBitRate = iAudioUtility->BitRate();
	    DP1(_L("CAdvancedAudioPlayController::UpdateBitRate iBitRate[%d]"), iBitRate);
        }
    return KErrNone;        
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::HandleAutoPauseEvent
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::HandleAutoPauseEvent()
    {
    DP0(_L("CAdvancedAudioPlayController::HandleAutoPauseEvent"));
    iState = EAutoPaused;
    SendEventToClient(TMMFEvent(KStreamControlEventStateChangedAutoPaused, KErrNone));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::HandlePreemptionEvent
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::HandlePreemptionEvent(TInt aError)
    {
    DP1(_L("CAdvancedAudioPlayController::HandlePreemptionEvent this[%x]"), this);
    TInt err;

	// Stop the CActiveSchedulerWait for SetPosition if it is started during Playforseek
    if(iBlockSetPos)
        {
        if (iBlockSetPos->IsStarted())
            {
            iBlockSetPos->AsyncStop();
            }
        }
    if(iBlockPrime)
       {
       if(iBlockPrime->IsStarted())
       	   {
            iBlockPrime->AsyncStop();
           }
       }
    iRequestState = EPaused;
    TRAP(err, DoPauseL(ETrue)); // this is a preemption pause
	// In case of pre-emption we should only Pause ... but not Stop.
    SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
    SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPaused, aError));
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::HandleGeneralEvent
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::HandleGeneralEvent(const TMMFEvent& aEvent)
    {
    DP1(_L("CAdvancedAudioPlayController::HandleGeneralEvent this[%x]"), this);
    TRAP_IGNORE(DoStopL(aEvent.iErrorCode));
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::SetPlaybackWindowBoundariesL
// -----------------------------------------------------------------------------
//    
EXPORT_C void CAdvancedAudioPlayController::SetPlaybackWindowBoundariesL(const TTimeIntervalMicroSeconds& aStart, const TTimeIntervalMicroSeconds& aEnd)
    {
    DP2(_L("CAdvancedAudioPlayController::SetPlaybackWindowBoundariesL start[%d] end[%d]"), aStart.Int64(), aEnd.Int64());

    // if client calls SetPlayWindow with the same values then it can be ignored
    // as the controller may be in the middle of processing.
    if (iPlayWindowStartPosition == aStart && iPlayWindowEndPosition == aEnd)
        {
        return;
        }

	if ( iPlayWindowStartPosition < TTimeIntervalMicroSeconds(0) )
		{
		iPlayWindowStartPosition = TTimeIntervalMicroSeconds(0);
		}
	else
	    {
	    iPlayWindowStartPosition = aStart;
	    }

	if ( iPlayWindowEndPosition < TTimeIntervalMicroSeconds(0) )
		{
		iPlayWindowEndPosition = TTimeIntervalMicroSeconds(0);
		}
	else
	    {
	    iPlayWindowEndPosition = aEnd;
	    }

	// We should not set the iInitPosition here because, if ClearPlayWindow is called when iState == EPlaying || EPaused
	// then iInitPosition will be reset to zero, which should not be the case because the default iInitPosition should be -1.
	// We just have to clear the play window end postion and continue to play from the current playback position
   	// iInitPosition = iPlayWindowStartPosition;
 	
	DP2(_L("CAdvancedAudioPlayController::SetPlaybackWindowBoundariesL iPlayWindowStartPosition[%d] iPlayWindowEndPosition[%d]"), iPlayWindowStartPosition.Int64()/1000, iPlayWindowEndPosition.Int64()/1000);
	// Registers / Unregisters the PlayWindowEndPosition depending on the aEnd value, whenever the play window is set / cleared
    // For SYMBIAN ClearPlayWindow(): If the ClearPlayWindow() is called during playing,
    //                                it deletes the playwindow that has been set
    iAudioUtility->SetPlayWindowEndTimeMs(iPlayWindowEndPosition.Int64()/1000);
    if (aStart == iSavedSetPosition)
        {
        return;
        }
	switch (iState)
		{
		case EStopped:  // no need to do anything with start position since the start position is saved
			// iAudioUtility->SetPlayWindowEndTimeMs(iPlayWindowEndPosition.Int64()/1000);
			// break;
		case EInitializing: // if already primed or priming, we need to get to correct start position
		case EInitialized:
			// DoSetPositionL(iInitPosition);
			// iAudioUtility->SetPlayWindowEndTimeMs(iPlayWindowEndPosition.Int64()/1000);
            SetPositionL(iPlayWindowStartPosition);
			break;
		case EPlaying:
		case EPaused:
		case EAutoPaused:
		    // If the Playwindow start position is greater than the current position
		    // then we seek to the new position and start playing from there.
		    if ( iPlayWindowEndPosition != TTimeIntervalMicroSeconds(0) )
		        {
		        TTimeIntervalMicroSeconds currentPosition = PositionL();
                if ( currentPosition < iPlayWindowStartPosition )
                    {
                    SetPositionL(iPlayWindowStartPosition);
                    }
		        }
			break;
        default:
            Panic(EBadState);
            break;
 		}
    
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::AddDataSourceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::AddDataSourceL(
    MDataSource& aSource)
    {
    DP1(_L("CAdvancedAudioPlayController::AddDataSourceL this[%x]"), this);
    DP2(_L("CAdvancedAudioController::AddDataSourceL iSharedBufferMaxNum[%d] iSharedBufferMaxSize[%d]"), iSharedBufferMaxNum, iSharedBufferMaxSize);    
    iSourceUnreadable = EFalse;
    iEnablePrimedStateChangedEvent = EFalse;
    // source type is intended to not be needed by controllers
    // but metadata functions still use it
    iSourceType = aSource.DataSourceType();
    iBitRateFrozen = EFalse;
    iDurationFrozen = EFalse;
    if (iWait)
        {
        iWait->AsyncStop();
        }
    if (iDataSourceAdapter)
		{
        User::Leave(KErrAlreadyExists);
        }


	// set iReadHeader here in case prime is not called before set position is used.
    // reset before adding data source
    DP0(_L("CAdvancedAudioPlayController::AddDataSourceL reseting iSharedBufferCnt iReadHeader iInitPosition position vars"));
    iSharedBufferCnt = 0;
	iReadHeader = ETrue;
	iInitPosition = -1;
	ResetPositionVariables();

	DoAddDataSourceL();

	if (!iDataSourceAdapter)
		{
		iDataSourceAdapter = CDataSourceAdapter::NewL();
		}
	TRAPD(err,iDataSourceAdapter->SetDataSourceL(&aSource, this, this));
	if(err)
		{
	    DP1(_L("setdatasource left for reason %d"),err);
	    iDataSourceAdapter->ResetDataSource();
	    User::Leave(err);
		}
	
    iDataSource = &aSource; // remove this eventually when all the references are removed
	
	iSourceIsTimeSeekable = iDataSourceAdapter->IsTimeSeekable();
	iSourceIsPosSeekable = iDataSourceAdapter->IsPositonSeekable();
	
   	if (iDataSourceAdapter->IsProtectedL())
        {
		if (iDataSink && iDataSink->DataSinkType() == KUidMmfFileSink)
			{
			// Conversion is not allowed for DRM protected files
			User::Leave(KErrNotSupported);
			}
        }

    iDataSourceAdapter->SetSourcePrioritySettings(iPrioritySettings);
    
    if (iAudioUtility)
        { // 3gp would not have utility built yet
        iAudioUtility->SetObserver(*this);
        }

    // we need to block this until duration is calculated if using mmfplayutility
    iBlockDuration = EFalse;
    
    if ((!iEventsEnabled) && (!iDataSourceAdapter->OnlyHeaderPresent()))
        {
        // recorder inserts just the header into the file before recording
        // we don't want to prime in this case
        DP0(_L("CAdvancedAudioPlayController::AddDataSourceL() Prime to get duration"));
        iBlockDuration = ETrue;
        TRAPD(err,DoInitializeL());// to get data from the source to calculate bitrate and duration
        if(err)
            {
            iDataSourceAdapter->ResetDataSource();
            User::Leave(err);
            }
        }
    if ((!iEventsEnabled) && (iDataSourceAdapter->OnlyHeaderPresent()))
        {
        // this is a file being recorded.
        // the recorder might have to open this file again - and if we keep it open
        // with shared read access, he won't be able to open it for writing.
        // So we will close the file here.
        TRAPD(err,iDataSourceAdapter->SourceStopL());
        if(err)
            {
            iDataSourceAdapter->ResetDataSource();
            User::Leave(err);
            }
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::DurationL
// 
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalMicroSeconds CAdvancedAudioPlayController::DurationL() const
    {
	DP2(_L("CAdvancedAudioController::DurationL this[%x] iDuration[%d]"), this, iDuration);
    // we need to block this until duration is calculated if using mmfplayutility
    if (iBlockDuration)
        {
        iBlockDuration = EFalse;
        iWait = new (ELeave) CActiveSchedulerWait();
        DP0(_L("CAdvancedAudioController::DurationL() blocking for duration"));
        iWait->Start();
        DP0(_L("CAdvancedAudioController::DurationL() continuing"));
        delete iWait;
        iWait = NULL;
        }
    return TTimeIntervalMicroSeconds(iDuration);
    }


// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::AddDataSinkL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::AddDataSinkL(
    MDataSink& aSink)
    {
    DP1(_L("CAdvancedAudioPlayController::AddDataSinkL this[%x]"), this);

//	TThreadStackInfo info;
//	RThread().StackInfo(info);
//    DP1(_L("### Stack Base Address: [0x%x]"), info.iBase);
//    DP1(_L("### Stack End  Address: [0x%x]"), info.iLimit);
    
    if (iDataSink)
        {
        User::Leave(KErrAlreadyExists);
        }

    if (aSink.DataSinkType() == KUidMmfAudioOutput)
        {
		iDataSink = &aSink;
        }
    else if (aSink.DataSinkType() == KUidMmfFileSink)
        { // only use of source type here other than metadata functions
		if (iDataSource && iSourceType == KUidMmfFileSource)
			{
			CMMFFile* file = static_cast<CMMFFile*>(iDataSource);
			file->SourcePrimeL();
			
			if (file->IsProtectedL())
				{
				// Conversion is not allowed for DRM protected files
				User::Leave(KErrNotSupported);
				}
			}
			
        iDataSink = &aSink;
        iDataSink->SinkPrimeL();
        iDataSink->SinkThreadLogon(*this);
		iDriveNumber = iAudioUtility->GetDriveNumber(static_cast<CMMFFile*>(iDataSink)->FileDrive());
		}
    else
        {
        User::Leave(KErrNotSupported);
        }
	DoAddDataSinkL(); // iAudioOutput will be created here
	// if the source has already been added but the output has not been configured because the sink was
	// not ready, then we will configure the output here.
	if (iSinkInitDataReady)
		{
		DoInitializeSinkL(); // the decoder will be created here
		}
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::RemoveDataSourceL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::RemoveDataSourceL(
    MDataSource& aDataSource)
    {
    DP0(_L("CAdvancedAudioPlayController::RemoveDataSourceL"));

    if (!iDataSource)
		{
        User::Leave(KErrNotReady);
		}

    if (iDataSource != &aDataSource)
		{
        User::Leave(KErrArgument);
		}

    if ((iState != EStopped) && (iState != EInitialized))
		{
        User::Leave(KErrNotReady);
		}

    iDataSource->SourceStopL();     // should always stop source before logoff
	iDataSource->SourceThreadLogoff();
    
	MapcDeletePlaybackWindowL();

    if (iIsDRMProtected)
		{
		delete iDataSource;
		}
	iDataSource = NULL;
	delete iDataSourceAdapter;
	iDataSourceAdapter = NULL;
	iSinkInitDataReady = EFalse;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::RemoveDataSinkL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::RemoveDataSinkL(
    MDataSink& aDataSink)
    {
    DP0(_L("CAdvancedAudioPlayController::RemoveDataSinkL"));

    if (iDataSink != &aDataSink)
		{
        User::Leave(KErrArgument);
		}

    if ((iState != EStopped) && (iState != EInitialized))
		{
        User::Leave(KErrNotReady);
		}
		
   if (iDataSink)
   	{
       iDataSink->SinkStopL();         // should always stop source before logoff
	     iDataSink->SinkThreadLogoff();
	  }

    // dereference Decoder from Utility before deleting AudioOutput (which took ownership of decoder)
    if (iAudioUtility)
    	{
        iAudioUtility->DeReferenceDecoder();
    	}
    
    delete iAudioOutput;
	  iAudioOutput = NULL;
    iDataSink = NULL;
    iDecoderExists = EFalse;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::ResetL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::ResetL()
    {
    DP0(_L("CAdvancedAudioPlayController::ResetL"));

    RemoveDataSourceL(*iDataSource);
    
    if (iDataSink)
    	{
    		  DP0(_L("CAdvancedAudioPlayController::ResetL, calling RemoveDataSinkL"));
          RemoveDataSinkL(*iDataSink);
      }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::PrimeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::PrimeL()
    { // this is for user call only - this changes the request state 
	DP2(_L("CAdvancedAudioPlayController::PrimeL this[%x] iState[%d]"), this, iState);

	switch (iState)
		{
		case EStopped:
			iRequestState = EInitialized;
			iEnablePrimedStateChangedEvent = ETrue;
			DoInitializeL();
			break;
		case EInitializing: // we may already be initializing since we may doinitialize to get header info for mmf client-events disabled)
            iRequestState = EInitialized;
            iEnablePrimedStateChangedEvent = ETrue;
		    break;
		case EInitialized:
            iRequestState = EInitialized;
            iEnablePrimedStateChangedEvent = ETrue;
            if (AllBuffersFilled())
                {
                iEnablePrimedStateChangedEvent = EFalse;
                DP0(_L("CAdvancedAudioPlayController::PrimeL state changed primed"));
                SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPrimed, KErrNone)); // both source and sink are ready.
                }
            break;
		case EPlaying:
		case EPaused:
		case EAutoPaused:
			break;
        default:
            Panic(EBadState);
            break;
 		}
    }

void CAdvancedAudioPlayController::DoInitializeL()
	{ // this is the DoPrimeL
	DP1(_L("CAdvancedAudioPlayController::DoInitializeL this[%x]"), this);
    
	if (iSourceUnreadable)
        {
        DP0(_L("CAdvancedAudioPlayController::PrimeL KErrCorrupt"));
        User::Leave(KErrCorrupt);
        }

	if (iDataSourceAdapter)
		{
		iState = EInitializing;
		if (iAudioUtility)
			{ // 3gp won't have a utility yet
			TInt err = iAudioUtility->ResetTable();
			}	
		iDataSourceAdapter->SourcePrimeL();    // get the source ready to process data
		iDataSourceAdapter->SourcePlayL();     // get the source ready to send data
		InitSharedBuffersL();
		// The position variables are reset when primed.
		// We don't want to reset them during stop because time needs to be maintained
		// in case position is called after stopping.
		ResetPositionVariables();
		}
    }


// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::PlayL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::PlayL()
    { // this is for user call only - this changes the request state 
	DP5(_L("CAdvancedAudioPlayController::PlayL this[%x] iCurrentPosition[%d] iSourceReadPosition[%d] iState[%d] iRequestState[%d]"),
		this, iCurrentPosition, iSourceReadPosition, iState, iRequestState);
	if (iSourceUnreadable)
	    {
        SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrCorrupt));
        SendEventToClient(TMMFEvent(KStreamControlEventStateChangedStopped, KErrCorrupt));
        return;
	    }
	if (iRequestState == EPlaying)
		{
		return;
		}
	if (iPlayingForDuration || iPlayingForPauseSeek || iPlayingForInitPos)
		{ 
		// PlayForDuration is not used.
		// Internally we use DoPlayL().
		// Just save the request and return, we don't want to call doplay again here.
	 	// User has overridden reason for play - we don't want to transition back to old state now.
		// iPlayingForDuration, iPlayingForPauseSeek, and iPlayingForInitPos will 
		// be reset when the current seek is complete.
		// Already playing internally so send state change
		iRequestState = EPlaying;
	    SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPlaying, KErrNone));
		return;
		}
	switch (iState)
		{
		case EStopped:
			// don't initialize playback with DoInitialize() here
			//  because latent play calls would get through after stops - seek to eof and play
		    //  also, latent play calls would get through after pausing just after eof reached and controller stops.
		    DP1(_L("CAdvancedAudioPlayController::PlayL iState=EStopped and iRequestState[%d]"), iRequestState);
			if ((iRequestState == EInitialized) || (iRequestState == EPaused))
				{ // primed state requested, but will stop if eof
				SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrNone));
				}
			break;
		case EInitializing: // priming
			iRequestState = EPlaying;
			break;
		case EInitialized: // primed
			iRequestState = EPlaying;
	        if (iPlayWindowEndPosition > 0)
	            {
	            DP1(_L("CAdvancedAudioPlayController::PlayL iAudioUtility->SetPlayWindowEndTimeMs(%d)"), I64LOW(iPlayWindowEndPosition.Int64()/1000));
	            iAudioUtility->SetPlayWindowEndTimeMs(iPlayWindowEndPosition.Int64() / 1000);
	            }
            if (iAudioOutput->IsDSStopped())
                { // during loop play the output is not stopped unless we did a seek - seek stops the output and we need to call DoPlay
                DP0(_L("CAdvancedAudioPlayController::PlayL Calling DoPlay iState=EInitialized and iRequestState=EPlaying"));
                DoPlayL();
                }
            else
                {
                DP1(_L("CAdvancedAudioPlayController::PlayL Calling DoResume iState=EInitialized iCurrentPosition[%d]"), iCurrentPosition);
                if (IsLoopPlayEnabled())
                    {
                    DoResume(iCurrentPosition); // sends state change event if successful
                    }
                else
                    {
                    DoPlayL();
                    }
                }
			break;
		case EPlaying:
			break;
		case EPaused:
			iRequestState = EPlaying;
	        if (iPlayWindowEndPosition > 0)
	            {
	            DP1(_L("CAdvancedAudioPlayController::PlayL iAudioUtility->SetPlayWindowEndTimeMs(%d)"), I64LOW(iPlayWindowEndPosition.Int64()/1000));
	            iAudioUtility->SetPlayWindowEndTimeMs(iPlayWindowEndPosition.Int64() / 1000);
	            }
            if (iAudioOutput->IsDSStopped())
                { // during loop play the output is not stopped unless we did a seek - seek stops the output and we need to call DoPlay
                DP0(_L("CAdvancedAudioPlayController::PlayL Calling DoPlay iState=EPaused"));
                DoPlayL();
                }
            else
                {
                DP1(_L("CAdvancedAudioPlayController::PlayL Calling DoResume iState=EPaused iCurrentPosition[%d]"), iCurrentPosition);
                if (IsLoopPlayEnabled())
                    {
                    DoResume(iCurrentPosition);
                    }
                else
                    {
                    DoPlayL();
                    }
                }
			break;
		case EAutoPaused:
			break;
        default:
            Panic(EBadState);
            break;
 		}

//    SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPlaying, KErrNone));
	}


void CAdvancedAudioPlayController::DoPlayL()
	{
	DP2(_L("CAdvancedAudioPlayController::DoPlayL reqstate[%d] pfps[%d]"),iRequestState,iPlayingForPauseSeek);
    if ((iRequestState == EPlaying) || iPlayingForInitPos || iPlayingForDuration || iPlayingForPauseSeek)
        {
		if (AllBuffersFilled()) // state entry condition
			{ // DoSetPositionL will look in shared buffers first and not seek if found
			// we may consider to use the current time here and resolve current position here instead of other places in the code

			if (iCurrentPosition == KMaxTUint)
                { // in case of a time seekable source, we use KMaxTUint in the seek since we don't know source position
                DP0(_L("CAdvancedAudioPlayController::DoPlayL Resetting iCurrentPosition to zero"));
                iCurrentPosition = 0;
			    }

			iSharedBufferIndex = FindBufferFromPos(iCurrentPosition);
		
			// if there was seek past end of file, the buffers may not have any data
			// they may all be empty last buffers
			if ((iSharedBufferIndex < 0) and AllBuffersEmpty())
			    { // position in buffer not found
			    DoStopL(KErrNone);
			    return;
			    }
			if (iSharedBufferIndex < 0)
			    {
			    DoStopL(KErrGeneral);
		    	return;
			    }
		
	    	if (iDataSink->DataSinkType() == KUidMmfFileSink)
    	    	{
	    	    static_cast<CMMFClip*>(iDataSink)->SinkPrimeL();
	    	    static_cast<CMMFFile*>(iDataSink)->FileL().SetSize(0); // Reset output file
    	    	}

            // if client has called Play() again, then we need to set the repeats so that the loop play can go on.
            // this must be done only when all the repeats are done and when the play back has stopped after the repeats
            // if client issues a play after the loop play, we must retain the repeat count and start the loop play again.
            // if play was called during loop play it must be ignored and loop play should continue
            if ((!iLoopPlayEnabled) && ((iRepeatCount > 0) || (iRepeatCount == KMdaRepeatForever)))
                {
                if ((iTrailingSilenceTimer) && (iTrailingSilenceTimer->IsActive()))
                    iTrailingSilenceTimer->Cancel();
                    DoSetRepeats(iRepeatCount, TTimeIntervalMicroSeconds(iTrailingSilenceMs.Int()));
                }
			TInt errExIntent = KErrNone;
			if (!iDisableAutoIntent && (iRequestState == EPlaying))
				{
                if (iIntentStopped)
                    {
                    errExIntent = iDataSourceAdapter->EvaluateIntent(ContentAccess::EPlay);
                    if (errExIntent == KErrNone)
                        {
                        errExIntent = iDataSourceAdapter->ExecuteIntent(ContentAccess::EPlay);
                        DP2(_L("CAdvancedAudioPlayController::DoPlayL() iIntentStopped[%d] errExIntent[%d]"), iIntentStopped, errExIntent );
                        if (errExIntent == KErrNone)
                            iIntentStopped = EFalse;
                        }
                    else // if ((errExIntent == KErrKErrNoRights) || any other error)
                        {
                        DoStopL(errExIntent);
                        return;
                        }
                    }
                else
                    {
                    errExIntent = iDataSourceAdapter->EvaluateIntent((iState == EPaused) ? ContentAccess::EContinue : ContentAccess::EPlay);
                    if (errExIntent == KErrNone)
                        {
                        errExIntent = iDataSourceAdapter->ExecuteIntent((iState == EPaused) ? ContentAccess::EContinue : ContentAccess::EPlay);
                        DP2(_L("CAdvancedAudioPlayController::DoPlayL() iIntentStopped[%d] errExIntent[%d]"), iIntentStopped, errExIntent );
                        }
                    else // if ((errExIntent == KErrKErrNoRights) || any other error)
                        {
                        DoStopL(errExIntent);
                        return;
                        }
                    }
                }
			if ((errExIntent != KErrNone) && (errExIntent != KErrNotSupported))
				{
                DP1(_L("CAdvancedAudioPlayController::DoPlayL() errExIntent[%d]"), errExIntent);
                // const TInt KErrCANoRights = -17452;  from caferr.h
                // don't leave because, stopping the source above will delete this active object..
                // the active scheduler will try to call runerror on the active object, but it's gone.
                //User::Leave(errExIntent);
                return;
                }
			iState = EPlaying; // stay in initialized state so bufferfilled will continue to call doplay
			iResumePosition = -1;
			iAudioOutput->PlayL(&iSharedBuffers, iSharedBufferIndex);
            SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPlaying, KErrNone));
			}
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::PauseL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::PauseL()
    { // this is for user call only - this changes the request state 
	DP2(_L("CAdvancedAudioPlayController::PauseL this[%x] iState[%d]"), this, iState);
	if (iRequestState == EPaused)
		{
		return;
		}
	iRequestState = EPaused;

    switch (iState)
        {
        case EStopped:
        case EInitializing:
        case EInitialized:
        	break;
        case EAutoPaused:
        case EPlaying: // pause even if we are playing for seeking so state change will occur
			DoPauseL(); // the pause will set position correctly
            SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPaused, KErrNone));
        	break;
        case EPaused:
        	break;
        default:
            Panic(EBadState);
            break;
        }
//    SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPaused, KErrNone));
    }

void CAdvancedAudioPlayController::DoPauseL(TBool aPreemption)
	{
   	DP5(_L("CAdvancedAudioPlayController::DoPauseL pfsp[%d] pfip[%d] pfps[%d] pfd[%d] rqststate[%d]"),
    	    iPausingForSetPos, iPlayingForInitPos, iPlayingForPauseSeek, iPlayingForDuration, iRequestState);
//    if ((iPausingForSetPos || iPlayingForInitPos || iPlayingForPauseSeek || iPlayingForDuration) && 
//        (iRequestState != EPaused)) // seek position reached and returning to pause state automatically
   	// both user pause and internal pause come here.
   	// Internal pause may be from preemption or seek position reached.
   	/*
   	 * Pre-emption case: Pause during a pre-emption and not seeking
   	 */
    if ((iPlayingForInitPos || iPlayingForPauseSeek || iPausingForSetPos) && aPreemption)
        {// we got preempted during a seek
        // we're already seeking to a position. When we get there we'll come here again, but handle it below
        DP0(_L("CAdvancedAudioPlayController::DoPauseL got a preemption during seek"));
        
         // if we are seeking, we need to flush the devsound buffers regardless of loop play
        DP0(_L("CAdvancedAudioPlayController::DoPauseL AudioOutput->StopL()"));
        iAudioOutput->StopL();

        if (iPlayingForInitPos)
         {
           iState = EInitialized;
         }
        else
         {
           iState = EPaused;
         }
        iPlayingForPauseSeek = EFalse;
        iPlayingForInitPos = EFalse; 
        return;
        }

    /*
     * Internal Pause case: Pausing during an internal seeking and not pre-emption case.
     */
      if ((iPlayingForInitPos || iPlayingForPauseSeek || iPausingForSetPos) && !aPreemption)
        { // already seeking to position, so we don't want to set position again
    	DP0(_L("CAdvancedAudioPlayController::DoPauseL not setting pos because we are already seeking"));
        if (IsLoopPlayEnabled())
            {
            DP0(_L("CAdvancedAudioPlayController::DoPauseL AudioOutput->StopL(EFalse)"));
            // If in LoopPlay mode, DO NOT STOP the Devsound from AudioOutput
            iAudioOutput->StopL(EFalse);
            }
        else
            {
            // If in normal Play mode, Devsound can be stopped during Pause
            // StopL method takes the default value as ETrue, so we don't need to pass any value.

            // if we are seeking, we need to flush the devsound buffers regardless of loop play
            DP0(_L("CAdvancedAudioPlayController::DoPauseL AudioOutput->StopL()"));
            iAudioOutput->StopL();
            }
        if (iPlayingForInitPos)
            {
            iState = EInitialized;
            }
        else
            {
            iState = EPaused;
            }
	    iPlayingForPauseSeek = EFalse;
    	iPlayingForInitPos = EFalse;
    	// iPausingForSetPos is cleared when seekposition reached
    	// iPlayingForDuration is cleared in stop which is where seekposreached sends it
		TInt foundPosition;
		TInt foundTimeMs;
		TUint time = iTimePositionInMicroSecs/1000; // time was set at the seek so it would be correct after seek.
        DP1(_L("CAdvancedAudioPlayController::DoPauseL iTimePositionInMicroSecs[%u]"),iTimePositionInMicroSecs);
		// this set position is short version because we expect that we have the buffer available after the seek.
		// since we are not setting position as below, we can get the buffer ready for play.
	    TInt err = SetPositionInSharedBuffers(time, foundPosition, foundTimeMs);
	    iCurrentPosition = foundPosition;
	    iTimePositionInMicroSecs = foundTimeMs; // just to get exact to position
	    iTimePositionInMicroSecs *=1000;
        DP1(_L("CAdvancedAudioPlayController::DoPauseL iCurrentPosition[%d]"),iCurrentPosition);
	    DP1(_L("CAdvancedAudioPlayController::DoPauseL iTimePositionInMicroSecs[%u]"),foundTimeMs);
	    // setpositioninsharedbuffers can reposition, so we need to update the ref
		iAudioUtility->SetSourceReference(foundTimeMs, iCurrentPosition-iHeaderOffset-iSyncOffset);

	    RefillPreceedingBuffersL();
	    
        return;
        }

      /*
       * User Pause case: User Pause when TruePause is not supported by DevSound adaptation (not seeking and not pre-emption)
       */
	// if user interrupts pause, don't mess with the time since it was already set at the seek
	// and the position will end up getting set to that time below.
	if (iPlayingForInitPos || iPlayingForPauseSeek)
		{
		DP2(_L("CAdvancedAudioPlayController::DoPauseL UserPause pfip[%d] pfps[%d]"),iPlayingForInitPos,  iPlayingForPauseSeek);
	    iPlayingForPauseSeek = EFalse;
    	iPlayingForInitPos = EFalse;
		}
	else
		{
        DP1(_L("CAdvancedAudioPlayController::DoPauseL UserPause before calc iTimePositionInMicroSecs [%d]"), iTimePositionInMicroSecs);
        // during loop play devsound returns the incremented value of the position
        // it must be adjusted to the duration of the audio clip so that playback can resume from the position where it was paused
        if (IsLoopPlayEnabled())
            {
            iTimePositionInMicroSecs += (iAudioOutput->CalculateAudioOutputPositionL() - iSavedTimePositionInMicroSecs);
            }
        else
            {
            iTimePositionInMicroSecs += iAudioOutput->CalculateAudioOutputPositionL();
            }
        DP1(_L("CAdvancedAudioPlayController::DoPauseL UserPause after calc iTimePositionInMicroSecs [%d]"), iTimePositionInMicroSecs);
        }

	// in case of client pause, stop the trailing silence timer during loop play and stay in paused state.
    // controller goes to play state when client calls play
    if ((iTrailingSilenceTimer) && (iTrailingSilenceTimer->IsActive()))
        {
        DP0(_L("CAdvancedAudioPlayController::DoPauseL UserPause Cancelling the TrailingSilenceTimer for User Pause "));
        iTrailingSilenceTimer->Cancel();
        }

    TInt errExIntent = KErrNone;
    if (!iDisableAutoIntent)
        {
        errExIntent = iDataSourceAdapter->EvaluateIntent(ContentAccess::EPause);
        if (errExIntent == KErrNone)
            {    
            errExIntent = iDataSourceAdapter->ExecuteIntent(ContentAccess::EPause);
            }
        }

    if (IsLoopPlayEnabled())
        {
        DP0(_L("CAdvancedAudioPlayController::DoPauseL UserPause AudioOutput->StopL(EFalse)"));
        // if in LoopPlay mode, DO NOT STOP the Devsound during a pause operation since we resume the playback from the paused position
        iAudioOutput->StopL(EFalse);
        }
    else
        {
        // If in normal Play mode, Devsound can be stopped during Pause
        // StopL method takes the default value as ETrue, so we don't need to pass any value.
        DP0(_L("CAdvancedAudioPlayController::DoPauseL UserPause AudioOutput->StopL()"));
        iAudioOutput->StopL();
        }
	iState = EPaused;
    DoSetPositionL(TTimeIntervalMicroSeconds(iTimePositionInMicroSecs)); // if we were play seeking, this will reseek to desired position

	if ((errExIntent != KErrNone) && (errExIntent != KErrNotSupported))
		{
        // User::Leave(errExIntent);
        return;
		}
	}
	
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::StopL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::StopL()
    { // this is for user call only - this changes the request state 
	DP2(_L("CAdvancedAudioPlayController::StopL this[%x] iState[%d]"), this, iState);
	if (iRequestState == EStopped)
		{
		return;
		}
   	iRequestState = EStopped;
    switch (iState)
        {
        case EStopped:
        	break;
        case EInitializing:
        case EInitialized:
        case EPlaying:
        case EPaused:
        case EAutoPaused:
            TRAP_IGNORE(DoStopL(KErrNone));     // ignore if any leave to ensure internal state change on the next statement
    	    break;
        default:
            Panic(EBadState);
            break;
        }
    // user event    
	
	SendEventToClient(TMMFEvent(KStreamControlEventStateChangedStopped, KErrNone));
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::DoStopL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::DoStopL(TInt aError)
    {
	DP3(_L("CAdvancedAudioPlayController::DoStopL this[%x] iState[%d], aError[%d]"), this, iState, aError);
    DP0(_L("CAdvancedAudioPlayController::DoStopL reseting iInitPosition iReadHeader"));
	CleanupForStop();
	// If the playwindow is still active (iPlayWindowEndPosition > 0)
	// then set the current playback position to play window start position
	// If there is a client Stop() call or End of playback event then we need to check for the
	// playwindow start position.
	if (iPlayWindowEndPosition > 0)
	    {
	    iInitPosition = iPlayWindowStartPosition;
	    }
	else
	    {
	    iInitPosition = -1;
	    }
    if (iTrailingSilenceTimer)
        {
        delete iTrailingSilenceTimer;
        iTrailingSilenceTimer = NULL;
        }
	iReadHeader = ETrue;
    iEnablePrimedStateChangedEvent = EFalse;
    iSharedBufferCnt = 0;
    if (iAudioOutput)
		{
        // during loop play devsound returns the incremented value of the position
        // it must be adjusted to the duration of the audio clip so that playback can resume from the position where it was paused
        if (IsLoopPlayEnabled())
            {
            DP1(_L("CAdvancedAudioPlayController::DoStopL() Number of times repeated = %d"), iCurrentRepeatCount);
            iTimePositionInMicroSecs += (iAudioOutput->CalculateAudioOutputPositionL() - iSavedTimePositionInMicroSecs);
            }
        else
            {
            iTimePositionInMicroSecs += iAudioOutput->CalculateAudioOutputPositionL();
            }
        // clear the repeat flag
        ClearRepeatFlag();

	    // In order to support adaptation implementation of loop play, we cannot stop
	    // the output at the end of file.
//	    if (aError == KErrUnderflow)
//	        {
//	        TRAP_IGNORE(iAudioOutput->StopL(EFalse));
//	        }
//	    else
//	        {
            TRAP_IGNORE(iAudioOutput->StopL());
//	        }
		TRAP_IGNORE(SourceSinkStopL());
		}

    DP0(_L("CAdvancedAudioPlayController::DoStopL state changed to stop"));
    iState = EStopped;

    if (!iPlayingForInitPos) // internal play for seeking before user calls play
		{
		// we don't want an event if seeking past eof before play is called - we will get a playcomplete and stop
		// no event for a user stop which will have no error
    	if (aError == KErrUnderflow)
    		{
	    	// normal end of playback
			SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, KErrNone));
			SendEventToClient(TMMFEvent(KStreamControlEventStateChangedStopped, KErrEof));		
	    	}
    	else if (aError != KErrNone)
	    	{
	    	// processing had an error
	    	if (iSourceUnreadable)
	    	    {
	    	    // if source was not even readable, we never made it to user play
	    	    // so we don't want to send playback complete
	    	    }
	    	else
	    	    {
	    	    SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete, aError));
	    	    SendEventToClient(TMMFEvent(KStreamControlEventStateChangedStopped, aError));
	    	    }
	    	}
		}
    }
    
TInt CAdvancedAudioPlayController::CleanupForStop()
    {
    // This is used in cases that need to initial back to a stop like state without a state change 
    DP1(_L("CAdvancedAudioPlayController::CleanupForStop iState[%d]"), iState);
    TInt status = KErrNone;
    
    iPlaySeeking = EFalse;
    
    if (iWait)
        {
        iWait->AsyncStop();
        }

    if (iBlockSetPos)
        {
        if (iBlockSetPos->IsStarted())
            {
            iBlockSetPos->AsyncStop();
            }
        }
    if(iBlockPrime)
        {
        if(iBlockPrime->IsStarted())
         iBlockPrime->AsyncStop();
        }
    iPlayingForDuration = EFalse;
    iBlockDuration = EFalse;
    iPlayingForPauseSeek = EFalse;
    iPlayingForInitPos = EFalse;
    iPausingForSetPos = EFalse;
    return status;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::ResetPositionVariables()
// -----------------------------------------------------------------------------
//
// These variables are always updated together, they are reset together here.
void CAdvancedAudioPlayController::ResetPositionVariables()
	{
	iSourceReadPosition = 0;
	iCurrentPosition = 0;
	iTimePositionInMicroSecs = 0;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::PositionL()
// -----------------------------------------------------------------------------
//
EXPORT_C TTimeIntervalMicroSeconds CAdvancedAudioPlayController::PositionL() const
    {
	DP2(_L("CAdvancedAudioPlayController::PositionL, this[%x] iState[%d]"), this, iState);

	TTimeIntervalMicroSeconds positionMicroSeconds(0);

    if (iState == EPlaying || iState == EAutoPaused)
    	{
    	DP1 (_L("CAdvancedAudioPlayController::PositionL iTimePositionInMicroSecs [%d] msec"), iTimePositionInMicroSecs);
        // adjust the position here since devsound returns the incremented postion value during loopplay
        if (IsLoopPlayEnabled())
            {
            positionMicroSeconds = TTimeIntervalMicroSeconds( iAudioOutput->CalculateAudioOutputPositionL() - iSavedTimePositionInMicroSecs);
            }
        else
            {
            positionMicroSeconds = TTimeIntervalMicroSeconds(iTimePositionInMicroSecs +  iAudioOutput->CalculateAudioOutputPositionL());
            }
        }
    else
    	{
        positionMicroSeconds = TTimeIntervalMicroSeconds(iTimePositionInMicroSecs);
    	}
    	
    DP1 (_L("CAdvancedAudioPlayController::PositionL returns[%d] msec"), I64LOW(positionMicroSeconds.Int64()/1000));
	return positionMicroSeconds;
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::SetPositionL
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::SetPositionL(
    const TTimeIntervalMicroSeconds& aTimePos)
    { // this is for user call only
	DP3(_L("CAdvancedAudioPlayController::SetPositionL this[%x] [%d]msec iState[%d]"), this, I64LOW(aTimePos.Int64()/1000), iState);
    if (iSourceUnreadable)
        {
        DP0(_L("CAdvancedAudioPlayController::SetPositionL KErrCorrupt"));
        User::Leave(KErrCorrupt);
        }

    // if play window is set then validate the position with the play window boundaries
    // play window boundaries must be checked when seeking during playback in a playwindow
    TTimeIntervalMicroSeconds position = aTimePos;
    // if play window is set then the playwindowendposition is > 0
    if (iPlayWindowEndPosition > 0)
        {
        if (aTimePos < iPlayWindowStartPosition)
            {
            position = iPlayWindowStartPosition;
            }
        else if (aTimePos > iPlayWindowEndPosition)
            {
            position = iPlayWindowEndPosition;
            }
        }

	switch (iState)
		{
		case EStopped:
		case EInitializing:
            if (position != 0)
    	   		{ // if we are priming, we will already be ready to play from 0.
				     DP2(_L("CAdvancedAudioPlayController::SetPositionL, saving pos iReadHeader[%d] iState[%d]"),iReadHeader,iState);
             iInitPosition = position;
       			}
			  DP0(_L("CAdvancedAudioPlayController::SetPositionL, can ignore"));
			  DP0(_L("CAdvancedAudioController::PrimeL() blocking"));
        iBlockPrime= new (ELeave) CActiveSchedulerWait();
        iBlockPrime->Start();
        DP0(_L("CAdvancedAudioController::PrimeL() continuing"));
        delete iBlockPrime;
        iBlockPrime= NULL;
			
		case EInitialized:
		case EPaused:
		    iSavedSetPosition = position;
            DoSetPositionL(position);
   			if (iPlaySeeking)
   				{
		        DP0(_L("CAdvancedAudioController::SetPositionL() blocking"));
		        iBlockSetPos = new (ELeave) CActiveSchedulerWait();
		        iBlockSetPos->Start();
    		    DP0(_L("CAdvancedAudioController::SetPositionL() continuing"));
	    	    delete iBlockSetPos;
   		    	iBlockSetPos = NULL;
   				}
			break;
		case EAutoPaused:
		case EPlaying:
            DP0(_L("CAdvancedAudioPlayController::SetPositionL, PauseForSetPosL so we can set position"));
			PauseForSetPosL(); // will return to play state when position reached
		    iSavedSetPosition = position;
			DP0(_L("CAdvancedAudioPlayController::SetPositionL, After PauseForSetPosL, now set the desired position"));
            DoSetPositionL(position);
            if (iPlaySeeking)
                {
                DP0(_L("CAdvancedAudioController::SetPositionL() blocking"));
                iBlockSetPos = new (ELeave) CActiveSchedulerWait();
                iBlockSetPos->Start();
                DP0(_L("CAdvancedAudioController::SetPositionL() continuing"));
                delete iBlockSetPos;
                iBlockSetPos = NULL;
                }
            else
                {
                DP0(_L("CAdvancedAudioPlayController::SetPositionL Calling SeekPositionReached "));
                // comes here if seek position found in source, or starting another loop during loop play
                // during loop play we seek back to the beginning of the clip or play window start position.
                SeekPositionReached(KMaxTUint);
                }
	        break;
		}
	}

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::SetPrioritySettings
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::SetPrioritySettings(
    const TMMFPrioritySettings& aPrioritySettings)
    {
	DP0(_L("CAdvancedAudioPlayController::SetPrioritySettings"));

    CAdvancedAudioController::SetPrioritySettings(aPrioritySettings);

    if(iDataSink)
        {
        TRAPD(err, iAudioOutput->SetPrioritySettingsL(aPrioritySettings));
		err = err;
        }
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::SendEventToClient
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::SendEventToClient(const TMMFEvent& aEvent)
    {
    DP4(_L("CAdvancedAudioPlayController::SendEventToClient, this[%x] iEventsEnabled[%d], iEventType[%d], ErrorCode[%d]"), this, iEventsEnabled, aEvent.iEventType, aEvent.iErrorCode);

    // The following logic is to take care of only forward the appropiate event depends on "iEventsEanbled" flag
    // KMMFEventCategoryPlaybackComplete event is to support the Existing Symbian MMF API behavior
    // while KStreamControlEventStateChangedXXX for Streaming/PDL events, and ONLY ONE of these events should be forwarded to the client
    // Current implementation is instead of deciding what event when a SendEventToClient issued, both events would be sent, and the decision
    // on which one to be forwarded is being implemented here
    // *NOTE*:  KStreamControlEventStateChangedStopped does NOT equal to KMMFEventCategoryPlaybackComplete,
    //          while both would happends during error situation, KStreamControlEventStateChangedStopped also happens when StopL is called
    //          but KMMFEventCategoryPlaybackComplete would not
    if( (aEvent.iErrorCode == KErrSourceAdapter) || ( aEvent.iErrorCode == KErrCorrupt ) )
    	{//Stop the controller and send error since error occured in source adapter 
        iSourceUnreadable = ETrue;
    	DoStopL(KErrDied);
        }
    else
    	{
    	if (iEventsEnabled)
    	    { // request for new events
    	    if ((aEvent.iEventType == KStreamControlEventStateChangedStopped) ||
   	            (aEvent.iEventType == KStreamControlEventStateChangedPrimed)  ||
   	            (aEvent.iEventType == KStreamControlEventStateChangedPlaying) ||
   	            (aEvent.iEventType == KStreamControlEventStateChangedPaused)  ||
   	            (aEvent.iEventType == KStreamControlEventStateChangedAutoPaused))
    	        {
    	        return CAdvancedAudioController::SendEventToClient(aEvent);
    	        }
    	    else
    	        {
    	        // do not send any other events for advanced clients.	
    	        }
    	    }
    	else
    	    {
    	    if ((aEvent.iEventType == KStreamControlEventStateChangedStopped) ||
    	        (aEvent.iEventType == KStreamControlEventStateChangedPrimed)  ||
    	        (aEvent.iEventType == KStreamControlEventStateChangedPlaying) ||
    	        (aEvent.iEventType == KStreamControlEventStateChangedPaused)  ||
    	        (aEvent.iEventType == KStreamControlEventStateChangedAutoPaused))
    	        {
    	        // ignore event and do nothing except convert autopause to old event
    	        }
    	    else
    	        {
    	        return CAdvancedAudioController::SendEventToClient(aEvent);
    	        }            
    	    }
    	}   
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::BufferFilledL
// Called after the data buffer is filled.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::BufferFilledL(CMMFBuffer* aBuffer)
    {
	aBuffer->SetPosition(0);
    CMMFDataBuffer* buffer = static_cast<CMMFDataBuffer*>(aBuffer);

    DP6(_L("CAdvancedAudioPlayController::BufferFilledL[%x], readPos[%d], Length[%d], iState[%d], LB[%d], d0[%x]"),
        buffer->Data().Ptr(), iSourceReadPosition, buffer->Data().Length(), iState, aBuffer->LastBuffer(),
        buffer->Data().Ptr()[0]);
    
	if ((buffer->Data().Length() > 0) || buffer->LastBuffer())
	    {
	    aBuffer->SetStatus(EFull);
			
		// iSourceReadPosition is used to mark the buffer with the source position
		// This is stored in aBuffer->Frame(). aBuffer->Frame() will indicate
		// the source position for the starting byte position for that buffer
		aBuffer->SetFrameNumber(iSourceReadPosition);
		iSourceReadPosition += buffer->Data().Length();

        DP1(_L("CAdvancedAudioPlayController::BufferFilledL next readPos[%d]"),	iSourceReadPosition);
	    }
    else
        {
		DP0(_L("CAdvancedAudioPlayController::BufferFilledL, unexpected 0 length buffer"));
		User::Leave(KErrAbort);
        }

    if (iState != EStopped)
        {
        DoBufferFilledL(aBuffer);
        }

	TBool doState = ETrue;    
	while (doState)
		{
		DP5(_L("CAdvancedAudioPlayController::BufferFilledL, state check state[%d] reqstate[%d] pfps[%d] pfd[%d] pfip[%d]"),
				iState, iRequestState, iPlayingForPauseSeek,iPlayingForDuration,iPlayingForInitPos);
		doState = EFalse;
		switch (iState)
			{
			case EStopped:
                DP0(_L("CAdvancedAudioPlayController::BufferFilledL EStopped"));
				break;
			case EInitializing:
                DP0(_L("CAdvancedAudioPlayController::BufferFilledL EInitializing"));
				if (!iReadHeader)
					{
					// do not transition to initialized if there was a problem
				    if (iSourceUnreadable)
				        {
				        return;
				        }
					iState = EInitialized;
					if(iBlockPrime)
					    {
					    if(iBlockPrime->IsStarted())
					    	{
					      iBlockPrime->AsyncStop();
					      }
					    }
					
					// when playwindow is active for a non-seekable source during loop play
					// we must seek to the playwindow start position and then start the playback
					if (iPlayWindowStartPosition > 0) // do we need additional checks as loop play / non-seekable source ??
					    iInitPosition = iPlayWindowStartPosition;
    				GoToInitPositionL(); // see if we need to seek to somewhere
					doState = ETrue;
					}
				break;
			case EInitialized:
                DP0(_L("CAdvancedAudioPlayController::BufferFilledL EInitialized"));
                if ((iRequestState == EInitialized) && AllBuffersFilled() && iEnablePrimedStateChangedEvent)
                    { // we have read the header, user has called prime but needed to wait for priming, all the input buffers are filled
                    // the controller itself may need to prime to get duration, but we just want to send the state change in response to user prime
                    iEnablePrimedStateChangedEvent = EFalse;
                    DP0(_L("CAdvancedAudioPlayController::BufferFilledL state changed primed"));
                    SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPrimed, KErrNone)); // both source and sink are ready.
                    }
				if ((iRequestState == EPlaying)	|| iPlayingForDuration || iPlayingForPauseSeek || iPlayingForInitPos)
					{
		            if (iAudioOutput->IsDSStopped())
                        {
                        DP0(_L("CAdvancedAudioPlayController::BufferFilledL calling DoPlayL Devsound is stopped iState=EInitialized"));
                        DoPlayL();
                        }
                    else
                        {
                        // This is needed for non-seekable sources as DoRepeatL() calls DoInitialize in this case
                        // this resets the source to read from 0 and sets the iState to EInitializing.
                        // BufferFilled will not read the header again, change state to EInitialized and seek to the iInitPosition.
                        // The next BufferFilled will come here, where we will continue playback from byte position 0
                        // that is now in the buffers.
                        DP0(_L("CAdvancedAudioPlayController::BufferFilledL Resuming the playback iState=EInitialized"));
                        // we need to be able to seek fwd to playwindow start pos
                        if (IsLoopPlayEnabled())
                            {
                            DoResume(iCurrentPosition);
                            }
                        else
                            {
                            DoPlayL();
                            }

                        }
					}
				break;
			case EPlaying:
                DP0(_L("CAdvancedAudioPlayController::BufferFilledL EPlaying"));
				break;				
			case EPaused:
                DP0(_L("CAdvancedAudioPlayController::BufferFilledL EPaused"));
                // iPlayingForPauseSeek would be set if we are in pause mode and we need to seek.
                // the controller will not go to play state until all buffers are filled.
                // so we need to call DoPlay until all buffers are filled and transition to play state.
                // if PauseForSetPos, we will resume play in seekpositionreached.
                // if (((iRequestState == EPlaying) || iPlayingForPauseSeek) && (!iPausingForSetPos) ) Suk
                if ((iRequestState == EPlaying) || iPlayingForPauseSeek)
					{
		            if (iAudioOutput->IsDSStopped())
                        { // during loop play the output is not stopped unless we did a seek - seek stops the output and we need to call DoPlay
                        DP0(_L("CAdvancedAudioPlayController::BufferFilledL Calling DoPlay Devsound is stopped iState=EPaused"));
                        DoPlayL();
					    }
					else
					    {
                        DP1(_L("CAdvancedAudioPlayController::BufferFilledL Calling DoResume iState=EPaused iCurrentPosition[%d]"), iCurrentPosition);
                        if (IsLoopPlayEnabled())
                            {
                            DoResume(iCurrentPosition);
                            }
                        else
                            {
                            DP0(_L("CAdvancedAudioPlayController::BufferFilledL Calling DoPlay iState=EPaused"));
                            DoPlayL();
                            }
					    }
                    }
				break;
			case EAutoPaused:
                DP0(_L("CAdvancedAudioPlayController::BufferFilledL EAutoPaused"));
                DoResume();
				break;
    	    default:
                DP0(_L("CAdvancedAudioPlayController::BufferFilledL EBadState"));
        	    Panic(EBadState);
            	break;
	 		}
		}
    }

void CAdvancedAudioPlayController::DoResume(TInt aResumePosition) // defaults to -1
	{
    DP3(_L("CAdvancedAudioPlayController::DoResume iState[%d] iRequestState[%d] aResumePosition[%d]"), iState, iRequestState, aResumePosition);
    TInt bufferIndex = -1;
    if (aResumePosition >= 0)
        {
        iResumePosition = aResumePosition;
        DP2(_L("CAdvancedAudioPlayController::DoResume saving resume position [%d] iState[%d]"), iResumePosition, iState);
        }

		if (iCurrentPosition == KMaxTUint)
			{
            DP0(_L("CAdvancedAudioPlayController::DoResume Resetting iCurrentPosition to zero"));
			iCurrentPosition = 0;
			}

	if (AllBuffersFilled()) // state entry condition
		{
        if (iResumePosition >= 0)
            {
            bufferIndex = FindBufferFromPos(iResumePosition);
            DP1(_L("CAdvancedAudioPlayController::DoResume bufferIndex [%d]"), bufferIndex);
            iResumePosition = -1;
            DP0(_L("CAdvancedAudioPlayController::DoResume Resetting the ResumePosition"));
            // if there was seek past end of file, the buffers may not have any data
            // they may all be empty last buffers
            if ((bufferIndex < 0) && (AllBuffersEmpty()))
                { // position in buffer buffer not found
                DP0(_L("CAdvancedAudioPlayController::DoResume Position in the buffer not found "));
                DoStopL(KErrNone);
                return;
                }
            if (bufferIndex < 0)
                {
                DP0(_L("CAdvancedAudioPlayController::DoResume bufferindex < 0"));
                DoStopL(KErrGeneral);
                return;
                }
            }

        TInt errExIntent = KErrNone;
        if (!iDisableAutoIntent && (iRequestState == EPlaying))
            {
            if (iIntentStopped)
                {
                errExIntent = iDataSourceAdapter->EvaluateIntent(ContentAccess::EPlay);
                if (errExIntent == KErrNone)
                    {
                    errExIntent = iDataSourceAdapter->ExecuteIntent(ContentAccess::EPlay);
                    DP2(_L("CAdvancedAudioPlayController::DoResume() iIntentStopped[%d] errExIntent[%d]"), iIntentStopped, errExIntent );
                    if (errExIntent == KErrNone)
                        iIntentStopped = EFalse;
                    }
                else // if ((errExIntent == KErrKErrNoRights) || any other error)
                    {
                    DoStopL(errExIntent);
                    return;
                    }
                }
            else
                {
                errExIntent = iDataSourceAdapter->EvaluateIntent((iState == EPaused) ? ContentAccess::EContinue : ContentAccess::EPlay);
                if (errExIntent == KErrNone)
                    {
                    errExIntent = iDataSourceAdapter->ExecuteIntent((iState == EPaused) ? ContentAccess::EContinue : ContentAccess::EPlay);
                    DP2(_L("CAdvancedAudioPlayController::DoResume() iIntentStopped[%d] errExIntent[%d]"), iIntentStopped, errExIntent );
                    }
                else // if ((errExIntent == KErrKErrNoRights) || any other error)
                    {
                    DoStopL(errExIntent);
                    return;
                    }
                }
            }
        if ((errExIntent != KErrNone) && (errExIntent != KErrNotSupported))
            {
            DP1(_L("CAdvancedAudioPlayController::DoResume() errExIntent[%d]"), errExIntent);
            return;
            }

        DP1(_L("CAdvancedAudioPlayController::DoResume iAudioOutput->Resume with bufferIndex [%d]"), bufferIndex);
		iState = EPlaying;
		// iRequestState should not be changed since it represents user request.
        iAudioOutput->Resume(bufferIndex);

        if ((!iRepeatForever) && (iCurrentRepeatCount == iRepeatCount))
            { // this is the last play of the repeats so tell the AudioOutput to set the lastbuffer to True
            DP1(_L("CAdvancedAudioPlayController::DoResume iCurrentRepeatCount[%d]"), iCurrentRepeatCount);
            iAudioOutput->UnSetLastBuffer(EFalse);
            }
        SendEventToClient(TMMFEvent(KStreamControlEventStateChangedPlaying, KErrNone));
		}
	}


TBool CAdvancedAudioPlayController::AllBuffersFilled()
	{
    DP0(_L("CAdvancedAudioPlayController::AllBuffersFilled"));
	TBool allFilled = ETrue;
	for (TInt i = 0; i < iSharedBufferMaxNum; i++)
		{
		if (iSharedBuffers[i]->LastBuffer() && iSharedBuffers[i]->Status() == EFull)
		    { // if one buffer is filled and marked last buffer
			allFilled = ETrue;
			break;
			}
	    else if (iSharedBuffers[i]->Status() == EBeingFilled)
	    	{
			allFilled = EFalse;
	    	}
		}
    DP1(_L("CAdvancedAudioPlayController::AllBuffersFilled [%d]"),allFilled);
	return allFilled;
	}

TBool CAdvancedAudioPlayController::AllBuffersEmpty()
	{
    DP0(_L("CAdvancedAudioPlayController::AllBuffersEmpty"));
	TBool allEmpty = ETrue;
	for (TInt i = 0; i < iSharedBufferMaxNum; i++)
		{
        DP3(_L("CAdvancedAudioPlayController::AllBuffersEmpty indx[%d] len[%d] stat[%d]"), i, iSharedBuffers[i]->BufferSize(), iSharedBuffers[i]->Status());
		if ((iSharedBuffers[i]->BufferSize() != 0) && 
		    (iSharedBuffers[i]->Status() == EFull))
		    {
			allEmpty = EFalse;
			break;
		    }
		}
    DP1(_L("CAdvancedAudioPlayController::AllBuffersEmpty [%d]"), allEmpty);
	return allEmpty;
	}


void CAdvancedAudioPlayController::DoBufferFilledL(CMMFBuffer* aBuffer)
	{
    DP0(_L("CAdvancedAudioPlayController::DoBufferFilledL"));

    CMMFDataBuffer* buffer = static_cast<CMMFDataBuffer*>(aBuffer);
	if (iReadHeader)
	    {
	    TRAPD(err, DoReadHeaderL(buffer));
	    DP2(_L("CAdvancedAudioPlayController::DoBufferFilledL header is read iHeaderOffset[%d] iSyncOffset[%d]"), iHeaderOffset, iSyncOffset);
	    DP1(_L("CAdvancedAudioPlayController::DoBufferFilledL DoReadHeaderL err[%d]"), err);
			
    	if (((err == KErrNotReady) || (err == KErrNotFound) || (err == KErrCompletion && iSourceIsPosSeekable)) && !aBuffer->LastBuffer())
			{
            // this shared buffer does not contain any part of the first frame
		    DP0(_L("CAdvancedAudioPlayController::DoBufferFilledL iInitPosition = 0 to seek back to start when done"));
			iInitPosition = 0; // if we are going to throw away these buffers, we need to indicate we need to seek back to the start
			RefillBuffer(aBuffer);
			}
		else
			{
			if ( err == KErrCompletion )
				{
				if(((iSharedBufferCnt+1)  < iSharedBufferMaxNum) && !aBuffer->LastBuffer())
					{
					iSharedBufferCnt++;	
					return;	
					}
				else
					{
					err = KErrNone;	
					}
				}
				
			iReadHeader = EFalse;
			if (err != KErrNone)		    
				{
				if (((err == KErrNotReady) || (err == KErrNotFound)) && (aBuffer->LastBuffer()))
					{
					// we went through the whole file without finding the header
					// so the source is corrupted
				    DP0(_L("CAdvancedAudioPlayController::DoBufferFilledL KErrCorrupt"));
					err = KErrCorrupt;
					iSourceUnreadable = ETrue;
					}
				TRAP_IGNORE(DoStopL(err));
				}
			else
				{
		        if (iAudioUtility)
		            { // 3gp won't have a utility until it reads the header info
        		    TInt sourceSize = iDataSourceAdapter->SourceSize();
        		    if (sourceSize < 0)
        		        {
        		        sourceSize = 0;
        		        }
		           	TRAP_IGNORE(iAudioUtility->SetClipSizeL(sourceSize));
        		    }

        	    iCurrentPosition = iHeaderOffset+iSyncOffset;
                DP3(_L("CAdvancedAudioPlayController::DoBufferFilledL header read curpos[%d] hdroff[%d] syncoff[%d]"),
                        iCurrentPosition, iHeaderOffset, iSyncOffset);
        	    iTimePositionInMicroSecs = 0;
        	    if (iAudioUtility)
        	        {
					iAudioUtility->SetSourceReference(0,0);
        	        }

				UpdateDuration();
				UpdateBitRate();
				// we need to unblock any blocked duration call and also disable it from blocking
				// since we now have duration and bitrate.
				iBlockDuration = EFalse;
				if (iWait)
				    {
    				iWait->AsyncStop(); // unblock Duration() now that duration is calculated
					}
				
				// Sink may not be provided yet. Client might add source first.
				// Or a client trying to get duration may not have provided a sink.
				// Without a sink, we'll just return here, but know that we have read enough
				// source data to configure the sink once it is added.
				// But if the sink is available here, we'll initialize below.
				iSinkInitDataReady = ETrue; 
				// Primed state change event would be sent when both source and sink are ready.
				if (!iAudioOutput)
					{
//					iBlockDuration = EFalse;
//					if (iWait)
//					    {
// 	    				iWait->AsyncStop(); // unblock Duration() now that duration is calculated
//						}
					return;
					}
				DoInitializeSinkL(); // the decoder will be created here
				}
			}    		    
		}
    }

EXPORT_C void CAdvancedAudioPlayController::DoInitializeSinkL()
	{
	DP0(_L("CAdvancedAudioPlayController::DoInitializeSinkL"));
	iSinkInitDataReady = EFalse;
	
	  if (!iAudioOutput)
	        {
	        User::Leave(KErrNotReady);
	        }	
	//both source and sink have been added
     if(iDataSourceAdapter)     
         {
         iAudioOutput->SetDataSourceAdapter(iDataSourceAdapter);
         }
	
	if (!iDecoderExists)
	    {
	    // move this common code to here from doadddatasink's	
	    CAdvancedAudioDecoder* decoder = BuildDecoderL();
	    iDecoderExists = ETrue;
	    // AudioOutput takes ownership of decoder object
	    iAudioOutput->SetDecoder(decoder);
	    iAudioUtility->SetDecoder(*decoder);
	    iAudioUtility->SetObserver(*this);
	    decoder->SetDecoderUtilityObserver(*iAudioUtility);

	    // Leave if HWCodec and Conversion tried
	    if(decoder->IsHwAccelerated() && iDataSink->DataSinkType() == KUidMmfFileSink)
	        {
	        User::Leave(KErrNotSupported);
	        }
	    }
	
	
	
    // Read the default codec configuration parameters from resource file
	RArray<TInt>& codecConfigData = const_cast<RArray<TInt>&>(iAudioResource->CodecConfigParametersL());
	// Override default values with values found from header, if available
	 GetCodecConfigData(codecConfigData);
   iAudioOutput->ConfigureL(iSampleRate, iSinkNumChannels, iDataType, codecConfigData);
   DP0(_L("CAdvancedAudioPlayController::DoInitializeSinkL, output configured"));
   iAudioOutput->PrimeL();
   DP0(_L("CAdvancedAudioPlayController::DoInitializeSinkL, output primed"));
   
   	// we would use this code when we have a NULL sink
	
	/*	if (iDuration > 0)
		{
		DP0(_L("CAdvancedAudioPlayController::BufferFilledL, unblocking duration"));
		iBlockDuration = EFalse;
		}
	if (iWait)
	    {
	    if (iDuration > 0)
	        {
            DP0(_L("CAdvancedAudioPlayController::DoBufferFilledL() unblocking"));
			iWait->AsyncStop(); // unblock DoAddDataSource now that duration is calculated
	        }
	    else
	        {
	        PlayForDurationL(); // this should only be used if we are given a NULL sink
	        }                  // because policy will get involved with an audio output sink
	    }                      // We can skip using this even for NULL sink if we just continue to
*/
	}                          // stay in read header mode and calulate bitrate on more buffers
    
void CAdvancedAudioPlayController::PlayForDurationL()
    {
    DP0(_L("CAdvancedAudioPlayController::PlayForDurationL()"));
    iPlayingForDuration = ETrue;
    DoSetPositionL(1000000); // 1 sec
    DoPlayL();
    }
    
void CAdvancedAudioPlayController::PlayForInitPositionL()
    {
    DP0(_L("CAdvancedAudioPlayController::PlayForInitPositionL()"));
    iPlayingForInitPos = ETrue;
    DoPlayL();
    }

void CAdvancedAudioPlayController::PlayForPauseSeekL()
	{
    DP1(_L("CAdvancedAudioPlayController::PlayForPauseSeekL() pfsp[%d]"), iPausingForSetPos);
    if (!iPausingForSetPos)
    	{ // used to return the state to pause
	    iPlayingForPauseSeek = ETrue;
    	}
    DoPlayL();
	}

void CAdvancedAudioPlayController::PauseForSetPosL()
	{
    DP0(_L("CAdvancedAudioPlayController::PauseForSetPosL()"));
    iPausingForSetPos = ETrue; // so that pause won't set position too.
    DoPauseL(); // stops the output for reposition - it also repositions at pause point using dosetposition
	}

TInt CAdvancedAudioPlayController::DoSetRepeats(TInt aRepeatNumberOfTimes, const TTimeIntervalMicroSeconds& aTrailingSilence)
    {
    DP2(_L("CAdvancedAudioPlayController::SetRepeats RepeatCount [%d] TrailingSilencePeriod [%d]"), aRepeatNumberOfTimes, I64LOW(aTrailingSilence.Int64()/1000));
    if ((aRepeatNumberOfTimes != KMdaRepeatForever) && (aRepeatNumberOfTimes <= 0))
        {
        return KErrArgument;
        }
    if (aTrailingSilence < 0)
        {
        return KErrArgument;
        }

    if (aRepeatNumberOfTimes == KMdaRepeatForever) // -2
        {
        iRepeatForever = ETrue;
        }
    else
        {
        iRepeatForever = EFalse;
        }
    iRepeatCount = aRepeatNumberOfTimes;
    iLoopPlayEnabled = ETrue;
    if (aTrailingSilence.Int64() > KMaxTInt)
        {
        iTrailingSilenceMs = TTimeIntervalMicroSeconds32(KMaxTInt);
        }
    else
        {
        iTrailingSilenceMs = TTimeIntervalMicroSeconds32(aTrailingSilence.Int64());
        }
    if (iAudioOutput)
        {
        iAudioOutput->IsLoopPlayEnabled(iLoopPlayEnabled);
        iAudioOutput->UnSetLastBuffer(ETrue);
        }
    if (iTrailingSilenceTimer)
        {
        delete iTrailingSilenceTimer;
        iTrailingSilenceTimer = NULL;
        }
    iTrailingSilenceTimer = CTrailingSilenceTimer::NewL(*this);
    // if MapcSetRepeats is called twice or more from the client side,
    // then the loop play re-starts and the latest repeat count is considered for
    // loop play operation. Reset the counter and the samplesplayed value
    iCurrentRepeatCount = 0;
    iSavedTimePositionInMicroSecs = 0;
    return KErrNone;
    }

void CAdvancedAudioPlayController::ClearRepeatFlag()
    {
    DP0(_L("CAdvancedAudioPlayController::ClearRepeatFlag"));
    iLoopPlayEnabled = EFalse;
    if (iAudioOutput)
        {
        iAudioOutput->IsLoopPlayEnabled(iLoopPlayEnabled);
        iAudioOutput->UnSetLastBuffer(EFalse);
        }
    }

EXPORT_C void CAdvancedAudioPlayController::LastBufferSent()
    { // this is the callback we will get during loop play, instead of playcomplete/dostop
    DP2(_L("CAdvancedAudioPlayController::LastBufferSent Begin iRequestedState [%d] iState [%d]"), iRequestState, iState);
    if (!iDisableAutoIntent)
        {
        TInt errExIntent = iDataSourceAdapter->ExecuteIntent(ContentAccess::EStop);
        DP1(_L("CAdvancedAudioPlayController::LastBufferSent Calling ExecuteIntent Stop on DataSource [%d]"), errExIntent);
        if (errExIntent == KErrNone)
            {
            iIntentStopped = ETrue;
            }
        }
    if ((iRequestState == EPlaying) && (iState == EPlaying) && (IsLoopPlayEnabled())) // evaluate use of iState
        {
        // start timer and let it's RunL call repeat
        if (iTrailingSilenceTimer)
            {
            DP1(_L("CAdvancedAudioPlayController::LastBufferSent Waiting for the iTrailingSilenceTimer for [%d] ms"), I64LOW(iTrailingSilenceMs.Int()/1000));
            iTrailingSilenceTimer->After(iTrailingSilenceMs);
            }
        // save the current position, this will be used to calculate the position in loop play when
        // client calls for PositionL() or Pause() operations
        // TODO:  need to check this position when loop play is going on in a play window
        // iSavedTimePositionInMicroSecs = iAudioOutput->CalculateAudioOutputPositionL();

        /* if ((!iRepeatForever) && (iCurrentRepeatCount < iRepeatCount))
            {
            iCurrentRepeatCount++;
            }
        DP1(_L("CAdvancedAudioPlayController::LastBufferSent() Number of times played till now = %d"), iCurrentRepeatCount);
        */
        }
    //else if ((iState == EPaused) && 
    /*
     * This is needed for TruePause in a LoopPlay.
     * When User Pauses after LastBufferSent and cancels the TrailingSilenceTimer, UserPlay should automatically kickback the DoRepeats 
     */
    /*#ifdef    __TRUEPAUSE_SUPPORT__
      if (IsLoopPlayEnabled())
        { // repeat enabled and last buffer sent but userwe're paused
        DP0(_L("CAdvancedAudioPlayController::LastBufferSent iNeedToResumeForRepeat ETrue"));
        iNeedToResumeForRepeat = ETrue; // always set in case user pauses - only to be used when resuming from DoPlayL
        }
      #endif
     */
    DP0(_L("CAdvancedAudioPlayController::LastBufferSent End"));
    }

void CAdvancedAudioPlayController::DoRepeatL()
    {
    DP0(_L("CAdvancedAudioPlayController::DoRepeatL Begin"));
    // save the current position, this will be used to calculate the position in loop play when
    // client calls for PositionL() or Pause() operations
    // TODO:  need to check this position when loop play is going on in a play window
    iSavedTimePositionInMicroSecs = iAudioOutput->CalculateAudioOutputPositionL();
    DP1(_L("CAdvancedAudioPlayController::DoRepeatL iSavedTimePositionInMicroSecs[%u]"), iSavedTimePositionInMicroSecs);
    
    if ((!iRepeatForever) && (iCurrentRepeatCount < iRepeatCount))
        {
        iCurrentRepeatCount++;
        }
    DP1(_L("CAdvancedAudioPlayController::DoRepeatL Number of times played till now = %d"), iCurrentRepeatCount);
    
    if (iSourceIsTimeSeekable || iSourceIsPosSeekable)
        { // For seekable source
        // if there is a playwindow set then use that, otherwise go back to 0 time
        // if ((iPlayWindowStartPosition != 0) || (iPlayWindowEndPosition != 0))
        // SetPlayWindow(iPlayWindowStartPosition, iPlayWindowEndPosition);
        if (iPlayWindowStartPosition > 0)
            {
            DP1(_L("CAdvancedAudioPlayController::DoRepeatL SetPositionL[%d]ms"), I64LOW(iPlayWindowStartPosition.Int64()/1000));
            SetPositionL(iPlayWindowStartPosition);
            }
        else
            {
            DP0(_L("CAdvancedAudioPlayController::DoRepeatL SetPositionL(0)"));
            SetPositionL(0);
            }
        // Register for PlayWindow end position as the FrameTable has set its playwindowendpostime to zero
        if (iPlayWindowEndPosition > 0)
            {
            DP1(_L("CAdvancedAudioPlayController::DoRepeatL iAudioUtility->SetPlayWindowEndTimeMs(%d)"), I64LOW(iPlayWindowEndPosition.Int64()/1000));
            iAudioUtility->SetPlayWindowEndTimeMs(iPlayWindowEndPosition.Int64() / 1000);
            }
        }
    else
        { // For non-seekable source
        // Stop and start playback
        DP0(_L("CAdvancedAudioPlayController::DoRepeatL Non-Seekable source."));
        iAudioOutput->StopL(EFalse);
        iDataSourceAdapter->SourceStopL(); // clear the buffers in the source before seeking and priming it
        DoInitializeL();
        // set the read header flag to true so that the header is read before the playback is resumed
        // and the current position is adjusted to be placed after the header.
        iReadHeader = ETrue;
        }
    DP0(_L("CAdvancedAudioPlayController::DoRepeatL End") );
    }

EXPORT_C void CAdvancedAudioPlayController::TrailingSilenceTimerComplete()
    {
    DP0(_L("CAdvancedAudioPlayController::TrailingSilenceTimerComplete "));
   
    TRAPD(err,DoRepeatL()); //Fix for Bug ECWG-84WE9J
    DP1(_L("CAdvancedAudioPlayController::TrailingSilenceTimerComplete: DoRepeatL [%d]"), err);
    if(err)
        {
         SendEventToClient(TMMFEvent(KMMFEventCategoryPlaybackComplete,err));
        }
    }

EXPORT_C TInt CAdvancedAudioPlayController::GetCodecConfigData(RArray<TInt>& aCodecConfigData)
    {
    TInt stat = KErrNone;
    iAudioUtility->SetCodecConfigData(aCodecConfigData);
	iSinkNumChannels = iAudioUtility->ChannelsOut();
    return stat;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::Refillbuffer
//
// Called after the data buffer is filled. Update the number of bytes read
// and the current read position for the next read operation.
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CAdvancedAudioPlayController::RefillBuffer(CMMFBuffer* refillBuffer)
    {
    DP1(_L("CAdvancedAudioPlayController::RefillBuffer: [%x]"),
          static_cast<CMMFDataBuffer*>(refillBuffer)->Data().Ptr() );

    // prevent further request        - is this really neccessary
    TBool lastBufferSet = EFalse;
	
    for (TInt i = 0; i < iSharedBuffers.Count(); i++)
        {
        if (iSharedBuffers[i]->LastBuffer() && 
            (iSharedBuffers[i]->Status() != EBeingFilled)) // need to make sure it is not still held by the source
            {
            lastBufferSet = ETrue;
            break;
            }
        }

    if (lastBufferSet)
        {
        // invalidate this buffer
        DP1(_L("CAdvancedAudioPlayController::RefillBuffer: marking[%x] EUnavailable and clearing its LB"),
                static_cast<CMMFDataBuffer*>(refillBuffer)->Data().Ptr() );
        refillBuffer->SetLastBuffer(EFalse);
        refillBuffer->SetStatus(EUnAvailable);
        
        return KErrCancel;
        }

    TRAPD(err, FillSharedBufferL(refillBuffer)); // Trap if DRM read fails

    if (err != KErrNone)
        {
        DP1(_L("Error filling shared buffer!!! %d"),err);

        iState = EPlaying;          // Set here to get StopL to shutdown completely
        TRAP_IGNORE(DoStopL(err));  // If stop leaves, then just return err
        }

    return err;
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::PlaybackComplete
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::PlaybackComplete()
    { // comes here for KErrUnderflow - no error
    DP3(_L("CAdvancedAudioPlayController::PlaybackComplete() reqstate[%d] state[%d] playseek[%d]"), iRequestState, iState, iPlaySeeking);
    // Keep in mind that seek past end of file would end up here instead of SeekPositionReached()
    if (iPlaySeeking)
        {
        // Seeking past end of file.
        // Here we should be in play state and iPlaySeeking (playing for seeking purpose)
        // Take care of this with the SeekPositionReached method, but don't adjust time.
        SeekPositionReached(KMaxTUint);
        }
    else
        { // normal play to end of file
        DP0(_L("CAdvancedAudioPlayController::PlaybackComplete() stopping"));
        TRAP_IGNORE(DoStopL(KErrUnderflow));
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::SendEvent
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::SendEvent(
    const TMMFEvent& aEvent)
    {
    DP2(_L("CAdvancedAudioPlayController::SendEvent[%d] this[%x]"), aEvent.iErrorCode, this);

	if (aEvent.iErrorCode == KErrBufferNotReady)
		{
        HandleAutoPauseEvent();
		}
    // MMFDevSound throws the following error codes incase of any preemption events
    // (DevSound instance has been thrown-off or initial request has been rejected)
    else if ((aEvent.iErrorCode == KErrAccessDenied) || (aEvent.iErrorCode == KErrInUse) || (aEvent.iErrorCode == KErrDied) )
    	{
    	// this might be a DevSound Preemption
    	// KErrDied is too generic because it is being returned in many situations
        HandlePreemptionEvent(aEvent.iErrorCode);
    	}
	else
		{
	    HandleGeneralEvent(aEvent);
        }
    }
    
// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::BitRateChanged
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::BitRateChanged()
    {
    DP0(_L("CAdvancedAudioPlayController::BitRateChanged"));
    iDataSourceAdapter->Event(KMultimediaDataSourceEventBitRateChanged);
    UpdateDuration(KOneThousandMilliSecond);
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::BufferFilled
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::BufferFilled(CMMFBuffer* aBuffer)
    {
	DP0(_L("CAdvancedAudioPlayController::BufferFilled"));
    TRAPD(err, BufferFilledL(aBuffer));
    if (err != KErrNone)
        {
        DP1(_L("CAdvancedAudioPlayController::BufferFilled, BufferFilledL err[%d]"), err);
        TRAP_IGNORE(DoStopL(err));
        }
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::Event
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::Event(TUid aEvent)
    {
    if (iEventsEnabled)
        {
        if (aEvent == KMultimediaDataSourceObserverEventSourceSizeChanged)
            {
            TInt sourceSize = iDataSourceAdapter->SourceSize();
            if (sourceSize < 0)
                {
                sourceSize = 0;
                }
            if (iAudioUtility)
            	{
        	TRAP_IGNORE(iAudioUtility->SetClipSizeL(sourceSize));
            	}
            UpdateDuration(0);
            }
        if (aEvent == KMultimediaDataSourceEventRandomSeekingSupportChanged)
            {
			iSourceIsTimeSeekable = iDataSourceAdapter->IsTimeSeekable();
			iSourceIsPosSeekable = iDataSourceAdapter->IsPositonSeekable();
            }
        }
    }
    
EXPORT_C void CAdvancedAudioPlayController::SeekPositionReached(TUint aTimeMs)    
    {// this can only be called from play state - we have reached this seek time
    // update the time played and transition back to correct state
	DP1(_L("CAdvancedAudioPlayController::SeekPositionReached[%d]"), aTimeMs);
	if (aTimeMs < KMaxTUint)
	    { // no update if seek past end of file - this comes from playbackcomplete()
	    iTimePositionInMicroSecs = aTimeMs; // this is set before the seek - may not need to do this here
	    iTimePositionInMicroSecs *=1000;
        DP1(_L("CAdvancedAudioPlayController::SeekPositionReached iTimePositionInMicroSecs[%d]"),aTimeMs);
	    }
    
	iPlaySeeking = EFalse;
    if (iPlayingForDuration)
        {
        UpdateDuration();
		if (iRequestState == EPlaying)
			{ // since we are already playing for the seek and the requested is to play, we can just keep playing
	    	// rendering will be enabled when position reached so we don't have to call play here - we're already playing
			DP0(_L("CAdvancedAudioPlayController::SeekPositionReached iPlayingForDuration - not stopping"));
			iPlayingForDuration = EFalse;
			}
		else
			{ // normally this would need to go back to stop state.
			DP0(_L("CAdvancedAudioPlayController::SeekPositionReached iPlayingForDuration - stopping"));
	        TRAP_IGNORE(DoStopL(KErrNone));
			}
        }
    else if (iPlayingForInitPos)
        {
		if (iRequestState == EPlaying)
			{ // since we are already playing for the seek and the requested is to play, we can just keep playing
	    	// rendering will be enabled when position reached so we don't have to call play here - we're already playing
			DP0(_L("CAdvancedAudioPlayController::SeekPositionReached iPlayingForInitPos - not pausing"));
			iPlayingForInitPos = EFalse;
			}
		else
			{ // normally this would need to go back to pause state.
			DP0(_L("CAdvancedAudioPlayController::SeekPositionReached iPlayingForInitPos - pausing"));
	        TRAP_IGNORE(DoPauseL());
			}
        }
    else if (iPausingForSetPos)
    	{ // seek was initiated during play but paused in order to seek and are now returning to play
    	// rendering will be enabled when position reached so we don't have to call play here - we're already playing
		DP0(_L("CAdvancedAudioPlayController::SeekPositionReached iPausingForSetPos - already playing - rendering should be enabled"));
    	iPausingForSetPos = EFalse;
        if (iState == EPaused)
            {
            if (iAudioOutput->IsDSStopped())
                { // during loop play the output is not stopped unless we did a seek - seek stops the output and we need to call DoPlay
                DP0(_L("CAdvancedAudioPlayController::SeekPositionReached Calling DoPlay iState=EPaused"));
                DoPlayL();
                }
            else
                {
                DP1(_L("CAdvancedAudioPlayController::SeekPositionReached Calling DoResume iState=EPaused iCurrentPosition[%d]"), iCurrentPosition);
                if (IsLoopPlayEnabled())
                    {
                    DoResume(iCurrentPosition);
                    }
                else
                    {
                    DoPlayL();
                    }
                }
            }
    	}
    else if (iPlayingForPauseSeek)
        { // seek was initiated during a pause
		if (iRequestState == EPlaying)
			{ // since we are already playing for the seek and the requested is to play, we can just keep playing
	    	// rendering will be enabled when position reached so we don't have to call play here - we're already playing
			DP0(_L("CAdvancedAudioPlayController::SeekPositionReached iPlayingForPauseSeek - not pausing"));
			iPlayingForPauseSeek = EFalse;
			}
		else
			{ // normally this would need to go back to pause state.
			DP0(_L("CAdvancedAudioPlayController::SeekPositionReached iPlayingForPauseSeek - pausing"));
	        TRAP_IGNORE(DoPauseL());
			}
        }
	if (iBlockSetPos)
		{
		DP0(_L("CAdvancedAudioPlayController::SeekPositionReached() unblocking"));
		if (iBlockSetPos->IsStarted())
		    {
		    iBlockSetPos->AsyncStop();
		    }
		}
    }

EXPORT_C void CAdvancedAudioPlayController::PlayWindowEndPositionReached()    
    {
    }
    
TBool CAdvancedAudioPlayController::IsLoopPlayEnabled() const
    {
    if ((iRepeatForever) || (iCurrentRepeatCount <= iRepeatCount))
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// --------------------------------------------------------------------------------------------------------------
// CTrailingSilenceTimer class definitions
// --------------------------------------------------------------------------------------------------------------
CTrailingSilenceTimer* CTrailingSilenceTimer::NewL(MTrailingSilenceObserver &aobserver)
    {
    DP0(_L("CTrailingSilenceTimer::NewL Begin"));
    CTrailingSilenceTimer* self=new(ELeave) CTrailingSilenceTimer(aobserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    DP0(_L("CTrailingSilenceTimer::NewL End"));
    return self;
    }

CTrailingSilenceTimer::CTrailingSilenceTimer(MTrailingSilenceObserver &aobserver)
    :
    CTimer(CActive::EPriorityStandard),
    iObserver(&aobserver)
    {
    }

void CTrailingSilenceTimer::ConstructL()
    {
    DP0(_L("CTrailingSilenceTimer::ConstructL Begin"));
    // Base class second-phase construction.
    CTimer::ConstructL();
    // Add to the ActiveScheduler
    CActiveScheduler::Add(this);
    DP0(_L("CTrailingSilenceTimer::ConstructL End"));
    }

CTrailingSilenceTimer::~CTrailingSilenceTimer()
    {
    DP0(_L("CTrailingSilenceTimer::~CTrailingSilenceTimer Begin"));
    Cancel();
    DP0(_L("CTrailingSilenceTimer::~CTrailingSilenceTimer End"));
    }

void CTrailingSilenceTimer::DoCancel()
    {
    DP0(_L("CTrailingSilenceTimer::DoCancel Begin"));
    // Call the default DoCancel method
    CTimer::DoCancel();
    DP0(_L("CTrailingSilenceTimer::DoCancel End"));
    }

void CTrailingSilenceTimer::RunL()
    {
    DP0(_L("CTrailingSilenceTimer::RunL"));
    iObserver->TrailingSilenceTimerComplete();
    }

void CTrailingSilenceTimer::RunError()
    {
    DP0(_L("CTrailingSilenceTimer::RunError"));
    // Handle error here tbd
    }
// --------------------------------------------------------------------------------------------------------------

// End of file
