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
* Description:  Class definition for the DataSourceAdapter functions.
*
*/


//  INCLUDES
#include "DataSourceAdapter.h"
#include "DebugMacros.h"
#include <MultimediaDataSourceFactory.h>
#include <mmffile.h>
#include <MultimediaDataSource.h>
#include <AudioOutputControlUtility.h>

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CDataSourceAdapter::CDataSourceAdapter
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CDataSourceAdapter::CDataSourceAdapter()
	{
    iPosSeekable = EFalse;
    iTimeSeekable = EFalse;
    iDataSource = NULL;
    iIsProtected = EFalse;
    iIsLocalPlayback = EFalse;
    }

// -----------------------------------------------------------------------------
// CDataSourceAdapter::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CDataSourceAdapter::ConstructL()
    {
	// should end up doing this for all sources
	iZeroBuffer = CMMFDataBuffer::NewL(0); // this is just for test to allow source seek
	iHdrBuffer = CMMFDataBuffer::NewL(20); // just enough to cover the header lengths create by recorder
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CDataSourceAdapter* CDataSourceAdapter::NewL()
	{
	DP0(_L("CDataSourceAdapter::NewL"));
    CDataSourceAdapter* self = new(ELeave) CDataSourceAdapter();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
EXPORT_C CDataSourceAdapter::~CDataSourceAdapter()
    {
	DP0(_L("CDataSourceAdapter::~CDataSourceAdapter"));
	TRAP_IGNORE(SourceStopL());
	delete iAsyncProxyFillBuffer;
    delete iZeroBuffer;
    delete iHdrBuffer;
    if (iMMDataSource)
    	{
		iMMDataSource->Stop();
    	iMMDataSource->Close();
	    delete iMMDataSource;	
    	}
    iAudioOutputControlUtility = NULL;
    }

EXPORT_C void CDataSourceAdapter::SetDataSourceL(MDataSource* aDataSource,
								 MMultimediaDataSourceObserver* aMMultimediaDataSourceObserver,
								 MAsyncEventHandler* aAsyncEventHandler)
	{
	DP0(_L("CDataSourceAdapter::SetDataSourceL"));
	iHeaderOnly = EFalse;
	iDataSource = aDataSource;
	iAsyncEventHandler = aAsyncEventHandler;
    iSourceType = iDataSource->DataSourceType();
	DP1(_L("CDataSourceAdapter::SetDataSourceL DataSourceType[0x%x]"), iSourceType);
		
    if ((iSourceType == KMmdsStreamingSourceUid) || (iSourceType == KMmdsProgDLSourceUid) ||
        (iSourceType == KMmdsFileSourceUid) || (iSourceType == KMmdsDescriptorSourceUid))
        {
        TInt err = CMultimediaDataSourceFactory::CreateDataSource(*iDataSource, iMMDataSource);

        User::LeaveIfError(err);

        iMMDataSource->SetObserver(*aMMultimediaDataSourceObserver);
        
        err = iMMDataSource->Open();    
        User::LeaveIfError(err);
        
        iMMDataSource->GetSeekingSupport(iPosSeekable);
        }
	else if ((iSourceType == KUidMmfFileSource) || (iSourceType == KOldProgDLSourceUid))
    	{
    	if (iSourceType == KUidMmfFileSource) 
		{
	    	iIsLocalPlayback = ETrue;
		}
    	iPosSeekable = ETrue;
    	iDataSource->SourcePrimeL();
   		iIsProtected = static_cast<CMMFFile*>(iDataSource)->IsProtectedL();
		iClip = static_cast<CMMFClip*>(iDataSource);
		iClip->SourceThreadLogon(*aAsyncEventHandler);
		iClip->ReadBufferL(iHdrBuffer,0);
		iClip->ReadBufferL(iZeroBuffer, 0); // seek back to 0 position
		const TUint8* ptr = iHdrBuffer->Data().Ptr();
    	if (iClip->Size() == 6)
    		{ // check for just the header from recorder
    		TUint32* w1p = (TUint32*)ptr;
    		TUint16* w2p = (TUint16*)(ptr+4);
    		if (((*w1p & 0x4D412123)==0x4D412123) && 
    			((*w2p & 0x0A52)==0x0A52))
    			{
	  			iHeaderOnly = ETrue;
	  			return;
    			}
    		}
		}
	else if (iSourceType == KUidMmfDescriptorSource)
		{
	    TRAP_IGNORE(iAsyncProxyFillBuffer = new(ELeave) CAsyncProxyFillBuffer(iDataSource));
		}
	}

EXPORT_C TUid CDataSourceAdapter::DataSourceType() 
	{
	DP0(_L("CDataSourceAdapter::DataSourceType"));
	if(iDataSource)
	    {
	    return iDataSource->DataSourceType();
	    }
	else
	    {
	    TUid uid={KNullUidValue};
        return uid;
	    }
	}

EXPORT_C void CDataSourceAdapter::FillBufferL( CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId)
	{
	DP0(_L("CDataSourceAdapter::FillBufferL"));
	if (iAsyncProxyFillBuffer)
		{// mmf descriptor does not have async intfc
		iAsyncProxyFillBuffer->FillBuffer(aBuffer, aConsumer, aMediaId);
		}
	else
		{
		if(iDataSource)
	        {
			iDataSource->FillBufferL(aBuffer, aConsumer, aMediaId);
			}
		}	
	}

EXPORT_C void CDataSourceAdapter::BufferEmptiedL(CMMFBuffer* aBuffer)
	{
	DP0(_L("CDataSourceAdapter::BufferEmptiedL"));
		if(iDataSource)
	        {
			iDataSource->BufferEmptiedL(aBuffer);
			}
	}

EXPORT_C TBool CDataSourceAdapter::CanCreateSourceBuffer()
	{
	DP0(_L("CDataSourceAdapter::CanCreateSourceBuffer"));
    if(iDataSource)
		{	
		return iDataSource->CanCreateSourceBuffer();
		}
	else
	{
		return EFalse;
	}
	}
	
EXPORT_C CMMFBuffer* CDataSourceAdapter::CreateSourceBufferL(TMediaId aMediaId, TBool& aReference)
	{
	DP0(_L("CDataSourceAdapter::CreateSourceBufferL"));
	if(iDataSource)
	    {	
		return iDataSource->CreateSourceBufferL(aMediaId, aReference);
		}
	else
		{
		return NULL;
		}
	}

EXPORT_C TInt CDataSourceAdapter::SourceThreadLogon(MAsyncEventHandler& aEventHandler)
	{
	DP0(_L("CDataSourceAdapter::SourceThreadLogon"));
	if(iDataSource)
	    {
	    return iDataSource->SourceThreadLogon(aEventHandler);
	    }
	else
	    {
	    return KErrGeneral;
	    }
	}

EXPORT_C void CDataSourceAdapter::SourceThreadLogoff()
	{
	DP0(_L("CDataSourceAdapter::SourceThreadLogoff"));
	if(iDataSource)
	    {
	    iDataSource->SourceThreadLogoff();
	    }
	}

EXPORT_C void CDataSourceAdapter::SourcePrimeL()
	{
	DP0(_L("CDataSourceAdapter::SourcePrimeL"));
	if(iDataSource)
	    {
	    iDataSource->SourcePrimeL();
	    }
	}
	
EXPORT_C void CDataSourceAdapter::SourceStopL()
	{
	DP0(_L("CDataSourceAdapter::SourceStopL"));
		if(iDataSource)
		{
		iDataSource->SourceStopL();
	  }
	if (iAsyncProxyFillBuffer)
		{
		iAsyncProxyFillBuffer->Cancel();
		iAsyncProxyFillBuffer->Reset();
		}
	}

EXPORT_C void CDataSourceAdapter::SourcePlayL()
	{
	DP0(_L("CDataSourceAdapter::SourcePlayL"));
	if(iDataSource)
	    {
	    iDataSource->SourcePlayL();
	    }
	}

EXPORT_C TInt CDataSourceAdapter::SeekToPosition(TUint aPosition)
	{
	DP1(_L("CDataSourceAdapter::SeekToPosition [%d]"), aPosition);
	TInt status = KErrNone;
	if (iPosSeekable)
		{
	    if (iSourceType == KUidMmfFileSource)
    		{
			TRAP(status, iClip->ReadBufferL(iZeroBuffer, aPosition)); // seek
    		}
    	else
    	    {
        	status = iMMDataSource->Seek(aPosition);
    	    }	
		}
	else
		{
		status = KErrNotSupported;
		}	
	return status;
	}
	
EXPORT_C TInt CDataSourceAdapter::SeekToTime(TUint /*aTimeMs*/)
	{
	DP0(_L("CDataSourceAdapter::SeekToTime"));
	TInt status = KErrNotSupported;
	return status;
	}

EXPORT_C TInt CDataSourceAdapter::SeekToTime(TUint /*aTimeMs*/, TUint& /*aFoundTimeMs*/)
	{
	DP0(_L("CDataSourceAdapter::SeekToTime"));
	TInt status = KErrNotSupported;
	return status;
	}
	
EXPORT_C TBool CDataSourceAdapter::IsTimeSeekable()
	{
	DP1(_L("CDataSourceAdapter::IsTimeSeekable[%d]"), iTimeSeekable);
	return iTimeSeekable;
	}
	
EXPORT_C TBool CDataSourceAdapter::IsPositonSeekable()
	{
	DP1(_L("CDataSourceAdapter::IsPositonSeekable[%d]"), iPosSeekable);
	if (iMMDataSource)
		{ // source seekability may change after file is downloaded so ask source again.
        iMMDataSource->GetSeekingSupport(iPosSeekable);
		}
	return iPosSeekable;
	}

EXPORT_C TInt CDataSourceAdapter::SourceSize()
	{
	DP0(_L("CDataSourceAdapter::SourceSize"));
	iSourceSize = KErrUnknown;
    if ((iSourceType == KUidMmfFileSource) ||(iSourceType == KUidMmfDescriptorSource))
    	{       if(iDataSource)
                 {
   		  iSourceSize  = static_cast<CMMFClip*>(iDataSource)->Size();
                 }
    	}
	else if (iMMDataSource)
        {
        TUint size;
        TInt stat = iMMDataSource->GetSize(size);
        if (stat == KErrNone)
        	{
        	iSourceSize = size;
        	}
		}
    return iSourceSize;	
	}

EXPORT_C TInt CDataSourceAdapter::IsProtectedL()
	{
	DP0(_L("CDataSourceAdapter::IsProtectedL"));
    return iIsProtected;	
	}

EXPORT_C void CDataSourceAdapter::SetSourcePrioritySettings(const TMMFPrioritySettings& aPrioritySettings)
	{
	DP0(_L("CDataSourceAdapter::SetSourcePrioritySettings"));
	if(iDataSource)
         {
	  iDataSource->SetSourcePrioritySettings(aPrioritySettings);
         }
	}

EXPORT_C TBool CDataSourceAdapter::OnlyHeaderPresent()
	{
	DP1(_L("CDataSourceAdapter::OnlyHeaderPresent [%d]"), iHeaderOnly);
	return iHeaderOnly;	
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::ExecuteIntent
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDataSourceAdapter::ExecuteIntent(ContentAccess::TIntent aIntent)
	{
	DP1(_L("CDataSourceAdapter::ExecuteIntent [%d]"), aIntent);
	TInt status = KErrNone;
	
	if (iMMDataSource)
		{
	    status = iMMDataSource->ExecuteIntent(aIntent);
		}
	else if (iSourceType == KUidMmfFileSource)
		{
   		CMMFFile* file = static_cast<CMMFFile*>(iDataSource);
   		status = file->ExecuteIntent(aIntent);
		}	
	return status;	
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::SetAgentProperty
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDataSourceAdapter::SetAgentProperty(ContentAccess::TAgentProperty aProperty, TInt aValue)
	{
	DP2(_L("CDataSourceAdapter::SetAgentProperty prop[%d] val[%d]"), aProperty, aValue);
	TInt status = KErrNone;
	
	if (iMMDataSource)
		{
	    status = iMMDataSource->SetAgentProperty(aProperty, aValue);
		}
	else if (iSourceType == KUidMmfFileSource)
		{
		CMMFFile* file = static_cast<CMMFFile*>(iDataSource);
		status = file->SetAgentProperty(aProperty, aValue);
		}

	return status;
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::EvaluateIntent
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDataSourceAdapter::EvaluateIntent(ContentAccess::TIntent aIntent)
	{
	DP0(_L("CDataSourceAdapter::EvaluateIntent"));
	TInt status = KErrNone;
	
	if (iMMDataSource)
		{
	    status = iMMDataSource->EvaluateIntent(aIntent);
		}
	else if (iSourceType == KUidMmfFileSource)
		{
		CMMFFile* file = static_cast<CMMFFile*>(iDataSource);
		status = file->EvaluateIntent(aIntent);
		}	

	DP1(_L("CDataSourceAdapter::EvaluateIntent aIntent[%d]"), aIntent);
	return status;
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::GetInterface
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDataSourceAdapter::GetInterface(TUid aInterfaceId, TVersion& aVersion, TAny*& aInterfaceImpl)
	{
	TInt err = KErrNotSupported;
	
	if (iMMDataSource)
		{		
		err = iMMDataSource->GetInterface(aInterfaceId, aVersion, aInterfaceImpl);
		
		// This Error check is done because all sources have not Implemented this Interface
		// and the Controller will Leave if we return this Error in Prime()
		// To maintain compatibility untill all the EMC sources implement GetInterface()
		if(err == KErrNotFound)
		    return KErrNone;
		    
		if (err != KErrNone)
			return err;
		}
				
	return err;	
	}		
	
// -----------------------------------------------------------------------------
// CDataSourceAdapter::Event
// -----------------------------------------------------------------------------
//
EXPORT_C void CDataSourceAdapter::Event(TUid aEvent)
	{
	if (iMMDataSource)
		{
		iMMDataSource->Event(aEvent);
		}
	}

EXPORT_C void CDataSourceAdapter::ResetDataSource()
    {
	DP0(_L("CDataSourceAdapter::ReSetDataSource"));
    iDataSource = NULL;
    }
	
EXPORT_C TBool CDataSourceAdapter::IsLocalPlayback()
	{
	DP1(_L("CDataSourceAdapter::iIsLocalPlayback[%d]"), iIsLocalPlayback);
	return iIsLocalPlayback;
	}
// -----------------------------------------------------------------------------
// CDataSourceAdapter::CAsyncProxyFillBuffer
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// CDataSourceAdapter::CAsyncProxyFillBuffer::CAsyncProxyFillBuffer
// -----------------------------------------------------------------------------
//
CDataSourceAdapter::CAsyncProxyFillBuffer::CAsyncProxyFillBuffer(MDataSource* aDataSource) :
    CActive(EPriorityHigh),
    iDataSource(aDataSource)
	{
    DP0(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::CAsyncProxyFillBuffer"));
	CActiveScheduler::Add(this);
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::CAsyncProxyFillBuffer::~CAsyncProxyFillBuffer
// -----------------------------------------------------------------------------
//
	
CDataSourceAdapter::CAsyncProxyFillBuffer::~CAsyncProxyFillBuffer()
	{
    DP0(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::~CAsyncProxyFillBuffer"));
	Cancel();
	iQueuedAsyncBuffers.Close();
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::CAsyncProxyFillBuffer::FillBuffer
// -----------------------------------------------------------------------------
//
void CDataSourceAdapter::CAsyncProxyFillBuffer::FillBuffer(CMMFBuffer* aBuffer, MDataSink* aConsumer, TMediaId aMediaId)
	{
    DP2(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::FillBuffer buffer[%x] this=consumer[%d]"),
	    static_cast<CMMFDataBuffer*>(aBuffer)->Data().Ptr(), (aConsumer == (MDataSink*)this));
    iMediaId = aMediaId;
	if (aConsumer == (MDataSink*)this)
		{
	    iBuffer = aBuffer;
	    iStatus = KRequestPending; // service request would be made here and pending set by service provider
		SetActive();
		iRequestStatus = &iStatus;
		User::RequestComplete(iRequestStatus, KErrNone);
		}	
	else
		{
	    iConsumer = aConsumer;
	    if (IsActive()|| (iQueuedAsyncBuffers.Count() > 0))
    		{
		    DP1(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::RunL que[%x]"),
		    		static_cast<CMMFDataBuffer*>(aBuffer)->Data().Ptr());
        	iQueuedAsyncBuffers.Append(aBuffer);
	    	}
   		else
   			{
		    iBuffer = aBuffer;
		    iStatus = KRequestPending; // service request would be made here and pending set by service provider
			SetActive();
			iRequestStatus = &iStatus;
			User::RequestComplete(iRequestStatus, KErrNone);
	   		}
		}
	}
    
// -----------------------------------------------------------------------------
// CDataSourceAdapter::CAsyncProxyFillBuffer::RunL
// -----------------------------------------------------------------------------
//
void CDataSourceAdapter::CAsyncProxyFillBuffer::RunL()
	{
    DP0(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::RunL"));
  if(iDataSource)
    {
    if (iDataSource->DataSourceType() == KUidMmfDescriptorSource)
    	{ 
    	// descriptor has no way to set read position back to 0
    	// so we have to always read using position
    	// then we can adjust the read position and send the buffer to the consumer
        static_cast<CMMFFile*>(iDataSource)->ReadBufferL(iBuffer, iReadPos);
    	TInt bytesRead = static_cast<CMMFDataBuffer*>(iBuffer)->Data().Length();
    	iReadPos += bytesRead;
    	iConsumer->BufferFilledL(iBuffer);
    	}
    else
    	{
        iDataSource->FillBufferL(iBuffer,iConsumer,iMediaId);
    	}
    }
	if (iQueuedAsyncBuffers.Count() > 0)
		{
	    DP1(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::RunL que count [%d]"),iQueuedAsyncBuffers.Count());
		FillBuffer(iQueuedAsyncBuffers[0],(MDataSink*)this,iMediaId);
		iQueuedAsyncBuffers.Remove(0);
		}
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::CAsyncProxyFillBuffer::RunError
// -----------------------------------------------------------------------------
//
TInt CDataSourceAdapter::CAsyncProxyFillBuffer::RunError(TInt aError)
	{
	if (aError)
	    {
	    DP1(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::RunError, aError = %d"), aError);
	    }
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::CAsyncProxyFillBuffer::DoCancel
// -----------------------------------------------------------------------------
//
void CDataSourceAdapter::CAsyncProxyFillBuffer::DoCancel()
	{
	DP0(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::DoCancel"));
//	Reset();
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::CAsyncProxyFillBuffer::Reset
// -----------------------------------------------------------------------------
//
void CDataSourceAdapter::CAsyncProxyFillBuffer::Reset()
	{
	DP0(_L("CDataSourceAdapter::CAsyncProxyFillBuffer::Reset"));
	iQueuedAsyncBuffers.Reset();
	iReadPos = 0;
	}

// -----------------------------------------------------------------------------
// CDataSourceAdapter::SetAudioOutputControlUtil
// sets the AudioOutputControlUtitlity reference and sets Datasource to AudioOutputContolUtility
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CDataSourceAdapter::SetAudioOutputControlUtil(CAudioOutputControlUtility* aAudioOutputControlUtility)
    {
    DP0(_L("CDataSourceAdapter::SetAudioOutputControlUtil"));
	TInt status(KErrNotReady);
    iAudioOutputControlUtility = aAudioOutputControlUtility;
    
    if (iMMDataSource)
        status = iAudioOutputControlUtility->SetDataSource(iMMDataSource);   //EMC source
    else
        status = iAudioOutputControlUtility->SetDataSource(iDataSource);     //MMF source
		
	return status;
    }

// ========================== OTHER EXPORTED FUNCTIONS =========================
// End of File

