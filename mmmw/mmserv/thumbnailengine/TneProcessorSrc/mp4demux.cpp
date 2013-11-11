/*
* Copyright (c) 2001 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*
*/



// INCLUDE FILES
#include "videoprocessorimpl.h"
#include "statusmonitor.h"
#include "activequeue.h"
#include "dataprocessor.h"
#include "mp4demux.h"
#include "mp4parser.h"

// ASSERTIONS
#define DASSERT(x) __ASSERT_DEBUG(x, User::Panic(_L("CVideoPlayer"), EInternalAssertionFailure))

//  LOCAL HELPER MACROS
// Debug printing, define DEBUGPRINT to get output
//#define DEBUGPRINT
#ifdef _DEBUG
#include <e32svr.h>
#define PRINT(x) RDebug::Print x;
#else
#define PRINT(x)
#endif

//  LOCAL CONSTANTS
const TUint KAudioReadAheadTimeMs = 100;
//const TUint KMaxMsInQueue = 600;
const TUint KMaxMsInQueue = 300;
const TUint KMaxBytesPerRun = 4096;
const TUint KMaxBlocksInQueue = 16;


//  MEMBER FUNCTIONS


//=============================================================================

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
// ?type ?function_name( ?arg_type, ?arg_type );

// ==================== LOCAL FUNCTIONS ====================

// ================= MEMBER FUNCTIONS =======================


// ---------------------------------------------------------
// CMP4Demux::NewL
// Symbian two-phased constructor.
// ---------------------------------------------------------
//
CMP4Demux* CMP4Demux::NewL(CActiveQueue *anInputQueue,              
                           TUint aNumChannels, TOutputChannel *aOutputChannels,
                           TStreamParameters *aParameters,
                           CStatusMonitor *aStatusMonitor,
                           CMP4Parser *aParser,                  
                           TInt aPriority)
{

    CMP4Demux *self = new (ELeave) CMP4Demux(anInputQueue,              
                                             aNumChannels, 
                                             aOutputChannels,
                                             aParameters,
                                             aStatusMonitor,
                                             aParser,
                                             aPriority);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();

    return self;


}

// ---------------------------------------------------------
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------
//
CMP4Demux::CMP4Demux(CActiveQueue *anInputQueue,
                     TUint aNumChannels, TOutputChannel *aOutputChannels,
                     TStreamParameters *aParameters,
                     CStatusMonitor *aStatusMonitor,
                     CMP4Parser *aParser,                     
                     TInt aPriority)
                     : CDemultiplexer(aPriority)
    {
    // Remember the objects
    iInputQueue = anInputQueue;
    iMonitor = aStatusMonitor;
    iParser = aParser;    

    iPicturePeriodMs = aParameters->iPicturePeriodMs;
    iAudioFramesInSample = aParameters->iAudioFramesInSample;    

    // Remember the channels and mux table
    iNumOutputChannels = aNumChannels;
    iOutputChannels = aOutputChannels;
    }

// EPOC default constructor can leave.
void CMP4Demux::ConstructL()
    {
    TUint i;
    
    // Set as a reader to the input queue
    if ( iInputQueue )
        {
        iInputQueue->SetReader(this, NULL);
        iReaderSet = ETrue;
        }
    
    // Set as writer to the output queues
    for ( i = 0; i < iNumOutputChannels; i++ )
        iOutputChannels[i].iTargetQueue->SetWriter(this, NULL);
    iWriterSet = ETrue;
    
    // Add us to active scheduler
    CActiveScheduler::Add(this);

    iBytesDemuxed = 0;
    iAudioEnd = iVideoEnd = 0;
    
    // Open all channels
    iAudioChannel = 0;
    iVideoChannel = 0;
    for ( i = 0; i < iNumOutputChannels; i++ )
        {
        TOutputChannel *chan = &iOutputChannels[i];
        
        // Check the channel type
        switch ( chan->iDataType )
            {
            case EDataAudio:                
                
                if ( !iAudioChannel )
                    iAudioChannel = chan;
                break;
                
            case EDataVideo:
                
                if ( !iVideoChannel )
                    iVideoChannel = chan;
                break;

            case EDataNone:
            default:
                User::Leave(CVideoProcessorImpl::EUnsupportedFormat);
            }
        }     

    // Make us active
    SetActive();
    iStatus = KRequestPending;
    }

// Destructor
CMP4Demux::~CMP4Demux()
    {
    // If we are demultiplexing, stop
    if ( iDemultiplexing )
        Stop();

    // return input block
    if ( iInputBlock )
        {
        iInputQueue->ReturnBlock(iInputBlock);
        iInputBlock = 0;
        }
    
    // Remove from being a reader or a writer
    if ( iReaderSet )
        iInputQueue->RemoveReader();
    if ( iWriterSet )
        {
        for ( TUint i = 0; i < iNumOutputChannels; i++ )
            iOutputChannels[i].iTargetQueue->RemoveWriter();
        }
    
    iMonitor = 0;
    iInputQueue = 0;
    iParser = 0;
    iOutputChannels = 0;
    iVideoChannel = 0;
    iAudioChannel = 0;
        
    Cancel();
    
    }

// ---------------------------------------------------------
// CMP4Demux::Start
// Starts demuxing
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CMP4Demux::Start()
    {
    if ( iDemultiplexing )
        return;
    
    // Activate the object if we have data
    if ( (iStatus == KRequestPending) && (!iInputQueue || iInputQueue->NumDataBlocks()) )
        {
        TRequestStatus *status = &iStatus;
        User::RequestComplete(status, KErrNone);
        }
    
    iDemultiplexing = ETrue;
    iAudioEnd = iVideoEnd = iStreamEnd = 0;
    iStreamEndDemuxed = 0;
    }

// ---------------------------------------------------------
// CMP4Demux::Stop
// Stops demuxing
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CMP4Demux::Stop()
    {
    iDemultiplexing = EFalse;
    }

// ---------------------------------------------------------
// CMP4Demux::RunL
// Standard active object running method, called when new input data 
// or free output space has been signaled to be available 
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CMP4Demux::RunL()
    {
    PRINT((_L("MP4Demux::RunL() in") ));

    // If we have demuxed everything up to stream end, theres is nothing for
    // us to do
    if ( iStreamEndDemuxed )
        return;
    
    // Don't do anything if we are not demuxing
    if ( !iDemultiplexing )
        {
        SetActive();
        iStatus = KRequestPending;
        return;
        }
    
    // If we don't have a primary channel, we have no open channels and may as
    // well quit
    if ( !iAudioChannel && !iVideoChannel )
        {
        iMonitor->StreamEndReached();
        return;
        }    

    // streaming case:
    // Try to demux as long as we have a free block in the primary output queue
    // and we can find more frames 
    // If we have both video and audio, we'll check the available space only
    // in the primary audio queue, and the video queue will allocate more
    // blocks as needed. This way the audio decoder will get more data as
    // needed, no matter what the video bitrate is.            

    // in file-reading case, GetFrameInfo() checks if there's available space
    // in queues, and this info is contained in variable iGotFrame       

    // send frame(s) if:
    // a frame is available AND
    // there are free blocks in output queue AND
    // we have not demuxed too much during this run so other objects get CPU AND
    // the stream end has not been demuxed    

    iBytesDemuxed = 0;

    // NOTE: only video queue fullness checked for now
    CActiveQueue *queue = iVideoChannel->iTargetQueue;

    GetFrameInfo();

    while ( iGotFrame && ( (iInputQueue && NumFreeBlocks() > 0) || 
          ( (queue->NumDataBlocks() < KMaxBlocksInQueue) && (iBytesDemuxed < KMaxBytesPerRun) ) ) && 
          (!iStreamEndDemuxed) )
        {
        // Read & send frame(s)        
        TInt error = ReadAndSendFrames();
        
        if ( error != KErrNone )
            {
            iMonitor->Error(error);
            return;        
            }
        
        // And to try get info for new frame
        GetFrameInfo();
        }
    
    // If we have demultiplexed everything up to stream end, signal the queues
    // and don't demux any more. If we have no output channels, notify the
    // status monitor.
    if ( iStreamEnd && (!iGotFrame) )
        {
        // report stream end in streaming case
        // in file-reading case, its reported in GetFrameInfo
        if ( iNumOutputChannels )
            {
            if ( iInputQueue )
                {
                TUint i;
                for ( i = 0; i < iNumOutputChannels; i++ )
                    iOutputChannels[i].iTargetQueue->WriteStreamEnd();
                }
            }
        else
            {
            iMonitor->StreamEndReached();
            }
        iStreamEndDemuxed = ETrue;
        return;
        }
    
    // Re-activate to get signals about new blocks
    SetActive();
    iStatus = KRequestPending;

    PRINT((_L("MP4Demux::RunL() out") ));
    }


// ---------------------------------------------------------
// CMP4Demux::StreamEndReached
// Informs the object that stream end has been reached
// (when we have an input queue, not used in file-reading case)
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CMP4Demux::StreamEndReached(TAny* /*aUserPointer*/)
    {
    iStreamEnd = ETrue;
    
    // Signal ourselves if we are demultiplexing
    if ( iDemultiplexing && (iStatus == KRequestPending) )
        {
        TRequestStatus *status = &iStatus;
        User::RequestComplete(status, KErrNone);
        }
    }

// ---------------------------------------------------------
// CMP4Demux::DoCancel
// Standard active object cancellation method
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CMP4Demux::DoCancel()
    {
    // Cancel our internal request
    if ( iStatus == KRequestPending )
        {
        TRequestStatus *status = &iStatus;
        User::RequestComplete(status, KErrCancel);
        }
    }

// ---------------------------------------------------------
// CMP4Demux::GetFrameInfo
// Gets information regarding the next frame. In file-reading
// case, also sets the type of next frame to be read. 
// (other items were commented in a header).
// ---------------------------------------------------------
//

void CMP4Demux::GetFrameInfo()
    {     
    
    if ( iGotFrame )
        return;    
    
    if ( !iInputQueue )
        {
        // file-reading case: set frame type according to 
        // queue fullness 
        
        SetFrameType();
        if ( iFrameType == EDataNone )            
            return;

        }
    
    TBool frameAvailable = EFalse;
    // check if parser has info & data for next frame available
    TInt error = iParser->GetNextFrameInformation((CMP4Parser::TFrameType&)iFrameType, 
        iFrameLen, frameAvailable);
    
    if ( error != KErrNone )
        {
        if ( error != CParser::EParserEndOfStream )
        {            
            iMonitor->Error(error);
        }
#ifdef _DEBUG
        else
            DASSERT( iStreamEnd );
#endif
        return;
        }                
    
    if ( iInputQueue ) 
        {                             
        
        // Read data from input queue until we know the frame type and length
        // and have data for it available        
        while ( !frameAvailable )
            {
            // Get a new input block with data
            while ( !iInputBlock )
                {
                if ( (iInputBlock = iInputQueue->ReadBlock()) == NULL )
                    return;                    
                
                // Return empty blocks immediately
                if ( iInputBlock->Length() == 0 ) 
                    {
                    iInputQueue->ReturnBlock(iInputBlock);
                    iInputBlock = 0;
                    }
                }      
            
            // give input block to parser
            error = iParser->WriteDataBlock(*iInputBlock);
            if ( error != KErrNone )
                {
                iMonitor->Error(error);
                return;
                }
            
            // Return our current input block 
            iInputQueue->ReturnBlock(iInputBlock);
            iInputBlock = 0;                                             
            
            // check if parser has info & data for next frame available
            error = iParser->GetNextFrameInformation((CMP4Parser::TFrameType&)iFrameType, 
                iFrameLen, frameAvailable);
            
            if ( error != KErrNone ) 
                {
                iMonitor->Error(error);                
                return;
                }
            }
        }
    else {
        while ( !frameAvailable )
        {
            if ( iFrameType == EDataAudio )
            {
                iAudioEnd = ETrue;
                iAudioChannel->iTargetQueue->WriteStreamEnd();
                PRINT((_L("MP4Demux, audio ended\n") ));
            }
            else
            {
                iVideoEnd = ETrue;
                iVideoChannel->iTargetQueue->WriteStreamEnd();
                PRINT((_L("MP4Demux, video ended\n") ));
            }
            if ( iVideoEnd && (iAudioChannel == 0 || iAudioEnd) )
            {
                iStreamEnd = ETrue;
                return;
            }
            iFrameType = EDataNone;
            SetFrameType();
            if ( iFrameType == EDataNone )
                return;
            error = iParser->GetNextFrameInformation((CMP4Parser::TFrameType&)iFrameType, 
                iFrameLen, frameAvailable);
            if ( error != KErrNone ) 
            {
                iMonitor->Error(error);                
                return;
            }
        }
    }
           
    // at least one frame available
    iGotFrame = ETrue;    
    }

// ---------------------------------------------------------
// CMP4Demux::NumFreeBlocks
// Gets the number of free blocks in target queue
// Relevant in streaming -case
// (other items were commented in a header).
// ---------------------------------------------------------
//

TUint CMP4Demux::NumFreeBlocks()
    {
    // check if there's space available for next frame    
    
    CActiveQueue *queue = 0;
    
    // streaming case: use audio queue value for both so
    // that enough audio is always available regardless of 
    // video bitrate.

    if ( iAudioChannel )    
        queue = iAudioChannel->iTargetQueue;    
    else     
        queue = iVideoChannel->iTargetQueue;
    
    DASSERT(queue);

    return queue->NumFreeBlocks();
    
    }

// ---------------------------------------------------------
// CMP4Demux::SetFrameType
// Sets the type of next frame to be read
// Relevant in file-reading case
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CMP4Demux::SetFrameType()
    {   
    
    TUint audioDataBlocks = 0;
    TUint audioInQueue = 0;
    TUint videoDataBlocks = iVideoChannel->iTargetQueue->NumDataBlocks();    
    TUint videoInQueue = videoDataBlocks * iPicturePeriodMs;    

    DASSERT( iFrameType == EDataNone );

    if ( iAudioChannel )
    {
        audioDataBlocks = iAudioChannel->iTargetQueue->NumDataBlocks();
        audioInQueue = audioDataBlocks * 20 * iAudioFramesInSample;
    }

    if ( iAudioChannel == 0 || iAudioEnd )
    {
        iFrameType = EDataVideo;
    }        

    else if ( iVideoEnd )
    {    
        iFrameType = EDataAudio;        
    }

    else 
    {
        if ( audioInQueue > videoInQueue + KAudioReadAheadTimeMs )
            iFrameType = EDataVideo;
        else 
            iFrameType = EDataAudio;            
    }
    
    //if ( ( iFrameType == EDataVideo && videoInQueue >= KMaxMsInQueue ) || 
    //     ( iFrameType == EDataAudio && audioInQueue >= KMaxMsInQueue + KAudioReadAheadTimeMs ) )
    //    iFrameType = EDataNone;          
    
    }

// ---------------------------------------------------------
// CMP4Demux::ReadVideoFrames
// Read video frames to video queue
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CMP4Demux::ReadVideoFrames(TInt aCount)
{
    
    while (aCount--)
    {
        iFrameType = EDataVideo;

        TBool frameAvailable = 0;
        TInt error = iParser->GetNextFrameInformation((CMP4Parser::TFrameType&)iFrameType, 
            iFrameLen, frameAvailable);

        if (error !=KErrNone)
            return error;

        DASSERT(frameAvailable);

        iGotFrame = ETrue;
        error = ReadAndSendFrames();
        if (error !=KErrNone)
            return error;
    }
    return KErrNone;

}

// ---------------------------------------------------------
// CMP4Demux::StreamEndReached
// Reads the next frame(s) from parser and writes
// them to the target queue
// (other items were commented in a header).
// ---------------------------------------------------------
//

TInt CMP4Demux::ReadAndSendFrames()
    {    

    DASSERT( iGotFrame );

    // Find the correct channel. If there is no channel open for this
    // type of frames, we'll simply ignore it
    TOutputChannel *chan = 0;
    TUint i;
    for ( i = 0; i < iNumOutputChannels; i++ )
        {
        if ( iOutputChannels[i].iDataType == iFrameType )
            chan = &iOutputChannels[i];
        }    
    
    if ( chan )
        {
        // OK, we have a target channel. Get a block from its queue
        
        TPtr8 *block = 0;                       
        
        // NOTE: if output block does not need to be saved in any case, make it a local variable

        //PRINT((_L("framelen = %d, bytesdemuxed = %d\n"), iFrameLen, iBytesDemuxed));

        TUint blockLen = iFrameLen;
        TPtr8 readDes(0,0);        
        TInt error;

        if ( iFrameType == EDataVideo ) 
        {
            // make room for timestamp
            blockLen += 4;
        }              
        


        TRAP( error, (block = chan->iTargetQueue->GetFreeBlockL(blockLen)) );
        if ( error != KErrNone )
            return error;        

        if ( iFrameType == EDataVideo ) 
        {
            TUint8 *p = (TUint8 *)(block->Ptr()) + 4;
            readDes.Set( p, 0, TInt(iFrameLen) );
        }
        else
        {
            readDes.Set( *block );
        }
                
        TUint32 numReadFrames = 0;
        TUint32 timeStamp;

        // read frame(s) from parser
        error = iParser->ReadFrames(readDes, CMP4Parser::TFrameType(iFrameType), 
            numReadFrames, timeStamp);
   
        if ( error != KErrNone )
            return error;

        DASSERT( numReadFrames > 0 );   
        
        if ( iFrameType == EDataAudio )
        {
            block->SetLength(readDes.Length());            
        }
        else
        {            
            block->SetLength(readDes.Length() + 4);

            // put timestamp in the output block before the actual frame data                       
            TUint* d = (TUint *)(block->Ptr());            
            Mem::Copy(d, &timeStamp, 4);
        }

        iBytesDemuxed += TUint( readDes.Length() );
        
        // Send the block
        chan->iTargetQueue->WriteBlock(block);        
        iFrameLen = 0;
        iFrameType = EDataNone;
        iGotFrame = EFalse;
        }
    else
        {     
        PRINT((_L("Unknown channel\n")));
        }    
    
    return KErrNone;
    }

