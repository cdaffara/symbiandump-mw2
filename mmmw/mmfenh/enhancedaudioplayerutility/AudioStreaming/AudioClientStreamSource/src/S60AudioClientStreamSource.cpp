/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  S60 Audio Stream Source implementation
*
*/


#include "S60AudioClientStreamSource.h"
#include "S60SourceEventDispatcher.h"
#include "WriteRequest.h"

// CONSTANTS
const TInt KMinBufferSize = 5120;
_LIT8(KAMRMimetype,"audio/amr");
_LIT8(KAMRWBMimetype,"audio/amr-wb");

// -----------------------------------------------------------------------------
// CS60AudioStreamSource::~CS60AudioStreamSource
// -----------------------------------------------------------------------------
//
CS60AudioStreamSource::~CS60AudioStreamSource()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::~CS60AudioStreamSource [%x]"),iDispatcher);
#endif

	delete iDispatcher;
	iDispatcher = NULL;
	delete iMimeType;
	iMimeType = NULL;
	iControllers.Close();
	iWriteRequests.ResetAndDestroy();

	}

// -----------------------------------------------------------------------------
// CS60AudioStreamSource::CS60AudioStreamSource
// -----------------------------------------------------------------------------
//
CS60AudioStreamSource::CS60AudioStreamSource()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::CS60AudioStreamSource"));
#endif
	}

// -----------------------------------------------------------------------------
// CS60AudioStreamSource::NewL
// -----------------------------------------------------------------------------
//
EXPORT_C CS60AudioStreamSource* CS60AudioStreamSource::NewL(
	MS60AudioStreamSourceObserver& aObserver,
	const TDesC8& aMimeType )
	{

#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::NewL"));
#endif

	CS60AudioStreamSource* self = new(ELeave) CS60AudioStreamSource();
	CleanupStack::PushL(self);
	self->ConstructL(aObserver, aMimeType);
	CleanupStack::Pop(self);
	return self;
	}

// -----------------------------------------------------------------------------
// CS60AudioStreamSource::ConstructL
// -----------------------------------------------------------------------------
//
void CS60AudioStreamSource::ConstructL(
	MS60AudioStreamSourceObserver& aObserver,
	const TDesC8& aMimeType )
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::ConstructL "));
#endif
	TInt err = KErrNone;

	iDispatcher = CS60SourceEventDispatcher::NewL(aObserver);


	if(!aMimeType.Compare(KAMRMimetype) || !aMimeType.Compare(KAMRWBMimetype))
		iMimeType = aMimeType.Alloc();
	else
		User::Leave(KErrNotSupported);
	//TRAP(err, FindControllersL());
	User::LeaveIfError(err);
	}

// -----------------------------------------------------------------------------
// CS60AudioStreamSource::FindControllersL
// -----------------------------------------------------------------------------
//
void CS60AudioStreamSource::FindControllersL()
	{
#if _DEBUG
  RDebug::Print(_L("CS60AudioStreamSource::FindControllersL"));
#endif
	// Retrieve a list of possible controllers from ECOM
	// If we don't have a match, leave with unsupported

	iControllers.ResetAndDestroy();

	CMMFControllerPluginSelectionParameters* cSelect = CMMFControllerPluginSelectionParameters::NewLC();
	CMMFFormatSelectionParameters* fSelect = CMMFFormatSelectionParameters::NewLC();

	RArray<TUid> mediaIds;
	CleanupClosePushL(mediaIds);


	User::LeaveIfError(mediaIds.Append(KUidMediaTypeAudio));

	cSelect->SetMediaIdsL(mediaIds, CMMFPluginSelectionParameters::EAllowOnlySuppliedMediaIds);


 	if (*iMimeType != KNullDesC8)
		{
		fSelect->SetMatchToMimeTypeL(*iMimeType);//We match to mime type
		}
	else
		{
		User::Leave(KErrNotSupported);
		}

	cSelect->SetRequiredPlayFormatSupportL(*fSelect);

	__UHEAP_MARK;
	RMMFControllerImplInfoArray siControllers;
	CleanupClosePushL(siControllers);

	cSelect->ListImplementationsL(siControllers);

	if ( siControllers.Count() == 0 )
		{
		User::Leave(KErrNotSupported);
		}
	siControllers.ResetAndDestroy();
	CleanupStack::PopAndDestroy();
	__UHEAP_MARKEND;

	CleanupStack::PopAndDestroy();
	CleanupStack::PopAndDestroy(fSelect);
	CleanupStack::PopAndDestroy(cSelect);
	}


EXPORT_C TInt CS60AudioStreamSource::WriteAudioData(
	CClientAudioBuffer& aBuffer )
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::WriteAudioData"));
#endif

	TInt index;

	 if ( GetFreeRequestSlot(index) )
		{
		iWriteRequests[index]->WriteAudioData(aBuffer);
		}
	else
		{
			CWriteRequest* request = CWriteRequest::NewL(iSourceHandle, iController, *iDispatcher);
			iWriteRequests.Append(request);
			request->WriteAudioData(aBuffer);
		}

		return KErrNone;
	}
// -----------------------------------------------------------------------------
// CS60AudioStreamSource::NextAudioBuffer
// -----------------------------------------------------------------------------
//
TBool CS60AudioStreamSource::GetFreeRequestSlot(TInt& index)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::NextAudioBuffer"));
#endif

	for ( TInt i=0; i < iWriteRequests.Count(); i++)
		{
		if ( iWriteRequests[i]->IsFree() )
			{
			index = i;
			return ETrue;
			}
		}

	return EFalse;
	}


// -----------------------------------------------------------------------------
// CS60AudioStreamSource::SetSize
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioStreamSource::SetSize(
	TUint aSize)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::SetSize"));
#endif
	iSize = aSize;
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CS60AudioStreamSource::MinimumBufferSize
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CS60AudioStreamSource::GetMinimumBufferSize()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::MinimumBufferSize"));
#endif
	return KMinBufferSize;
	}

// -----------------------------------------------------------------------------
// CS60AudioStreamSource::GetMimeTypeL
// -----------------------------------------------------------------------------
//
EXPORT_C void CS60AudioStreamSource::GetMimeType(TDes8& aMimeType)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::GetMimeType"));
#endif
	aMimeType.Copy(*iMimeType);
	}

EXPORT_C TUint CS60AudioStreamSource::GetSize()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::GetSize"));
#endif
	return iSize;
	}

EXPORT_C TInt64 CS60AudioStreamSource::GetPosition()
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::GetPosition"));
#endif
	return iPosition;
	}

EXPORT_C TInt CS60AudioStreamSource::SetPosition(
	TInt64 aPosition)
	{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::SetPosition"));
#endif
	iPosition = aPosition;
	return KErrNone;
	}

TBool CS60AudioStreamSource::IsSeekingSupported()
{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::IsSeekingSupported"));
#endif
	return EFalse;
}

TBool CS60AudioStreamSource::IsRandomSeekingSupported()
{
#ifdef _DEBUG
    RDebug::Print(_L("CS60AudioStreamSource::IsRandomSeekingSupported"));
#endif
	return EFalse;
}
