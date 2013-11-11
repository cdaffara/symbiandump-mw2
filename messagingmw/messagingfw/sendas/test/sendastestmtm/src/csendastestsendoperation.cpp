// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// csendastestsendoperation.cpp
//
#include "csendastestsendoperation.h"

#include <mtclbase.h>

#include "tsendastestmtmprogress.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "msvconsts.h"
#endif

CSendAsTestSendOperation* CSendAsTestSendOperation::NewL(
								CMsvSession&				aMsvSession,
								const CMsvEntrySelection&	aSelection, 
								const TDesC8& 				aParameter, 
								TRequestStatus& 			aRequestStatus)
	{
	CSendAsTestSendOperation* self = new (ELeave) CSendAsTestSendOperation(aMsvSession, aRequestStatus);
	CleanupStack::PushL(self);
	self->ConstructL(aSelection, aParameter);
	CleanupStack::Pop(self);
	return self;	
	}
	
CSendAsTestSendOperation::~CSendAsTestSendOperation()
	{
	}

CSendAsTestSendOperation::CSendAsTestSendOperation(CMsvSession& aMsvSession, TRequestStatus& aRequestStatus)
: CMsvSendOperation(aMsvSession, aRequestStatus)
	{
	}
	
void CSendAsTestSendOperation::ConstructL(const CMsvEntrySelection& aSelection, const TDesC8& aParameter)
	{
	CMsvOperation* operation = iMsvSession.TransferCommandL(aSelection, KMTMStandardFunctionsSendMessage, aParameter, iStatus);
	// Ownership of operation passed to base class.
	Start(operation);
	}

/*
 *	Methods from CMsvSendOperation
 */
 
const TDesC8& CSendAsTestSendOperation::TranslateProgress(const TDesC8& aProgress)
	{
	TSendAsTestMtmProgressBuf progress;
	progress.Copy(aProgress);
	TSendAsTestMtmProgress pr = progress();

	// Default progress
	iProgress().iProgressMax = 0;
	iProgress().iProgress = 0;
	
	// Convert the sendas test MTM progress into the standard send progress.
	if( progress().iError != KErrNone )
		{
		iProgress().iState = ESendStateFailed;
		}
	else if( progress().iMessageCount == progress().iMessagesDone )
		{
		iProgress().iState = ESendStateDone;
		}
	else
		{
		switch( progress().iState )
			{
		case TSendAsTestMtmProgress::ESendAsTestInitSend:
			{
			iProgress().iState = ESendStateInPreparation;
			} break;
		case TSendAsTestMtmProgress::ESendAsTestSendNext:
		case TSendAsTestMtmProgress::ESendAsTestSending:
		case TSendAsTestMtmProgress::ESendAsTestMoving:
			{
			iProgress().iState = ESendStateSending;
			} break;
		case TSendAsTestMtmProgress::ESendAsTestWaiting:
		default:
			iProgress().iState = ESendStateWaitingToSend;	
			break;			
			}	
		}
	// Set the remaing progress info.
	iProgress().iError			= progress().iError;
	iProgress().iProgressMax	= progress().iMessageCount;
	iProgress().iProgress		= progress().iMessagesDone;

	return iProgress;
	}
