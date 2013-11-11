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
* Description:   Implementation of the thumbnail processor
*                interface class
*
*/



//  EXTERNAL RESOURCES

//  Include Files
#include "TNEProcessorImpl.h"
#include "TNEProcessor.h"


// -----------------------------------------------------------------------------
// CTNEProcessor::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CTNEProcessor* CTNEProcessor::NewL()    

	{
	CTNEProcessor* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CTNEProcessor* CTNEProcessor::NewLC()
	{
	CTNEProcessor* self = new (ELeave) CTNEProcessor();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

// -----------------------------------------------------------------------------
// CTNEProcessor::CTNEProcessor()
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CTNEProcessor::CTNEProcessor()
	{
	iTNEProcessor=0;
	}

// -----------------------------------------------------------------------------
// CTNEProcessor::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CTNEProcessor::ConstructL()
	{
	iTNEProcessor = CTNEProcessorImpl::NewL();
	}

// -----------------------------------------------------------------------------
// CTNEProcessor::~CTNEProcessor
// Destructor.
// -----------------------------------------------------------------------------
//
CTNEProcessor::~CTNEProcessor()
{
	if(iTNEProcessor)
	{
		delete iTNEProcessor; 
		iTNEProcessor=0;
	}
}


// -----------------------------------------------------------------------------
// CTNEProcessor::GetVideoClipPropertiesL
// Retrieves information about the given clip
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessor::GetVideoClipPropertiesL(RFile& aFileHandle,
											  TTNEVideoFormat& aFormat,
											  TTNEVideoType& aVideoType, 
											  TSize& aResolution,
											  TInt& aVideoFrameCount)
	{	

	iTNEProcessor->GetClipPropertiesL(aFileHandle, aFormat, 
	                                  aVideoType, aResolution, aVideoFrameCount);

	return; 

	}

// -----------------------------------------------------------------------------
// CTNEProcessor::StartThumbL
// Initiates thumbnail generation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CTNEProcessor::StartThumbL(RFile& aFileHandle, TInt aIndex, TSize aResolution, 
																	TDisplayMode aDisplayMode, TBool aEnhance)
	{
	TInt fileSize;
	aFileHandle.Size(fileSize);
	iTNEProcessor->StartThumbL(aFileHandle, aIndex, aResolution, aDisplayMode, aEnhance); 
	}


// -----------------------------------------------------------------------------
// CTNEProcessor::ProcessThumbL
// Starts thumbnail image generation
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CTNEProcessor::ProcessThumbL(TRequestStatus &aStatus)
{
    iTNEProcessor->ProcessThumbL(aStatus);
}

// -----------------------------------------------------------------------------
// CTNEProcessor::FetchThumb
// Gets a pointer to completed thumbnail bitmap
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//	
void CTNEProcessor::FetchThumb(CFbsBitmap*& aThumb)
{
    iTNEProcessor->FetchThumb(aThumb);   
}
