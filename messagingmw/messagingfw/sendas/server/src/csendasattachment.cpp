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
//

#include "csendasattachment.h"

#include <mtclbase.h>

#include "msendasattachmentobserver.h"
#include "tsendasserverpanic.h"

CSendAsAttachment* CSendAsAttachment::NewL(MSendAsAttachmentObserver& aObserver, CBaseMtm& aClientMtm)
	{
	CSendAsAttachment* self = new(ELeave) CSendAsAttachment(aObserver, aClientMtm);
	return self;
	}
	
CSendAsAttachment::~CSendAsAttachment()
	{
	// do not notify observer during deletion.
	iObserver = NULL;
	Cancel();
	}
	
CSendAsAttachment::CSendAsAttachment(MSendAsAttachmentObserver& aObserver, CBaseMtm& aClientMtm) 
: CActive(CActive::EPriorityStandard), iObserver(&aObserver), iClientMtm(aClientMtm)
	{
	CActiveScheduler::Add(this);
	}

void CSendAsAttachment::AddExistingFileAttachmentL(RFile& aAttachment)
	{
	AddExistingFileAttachmentWithMimeTypeL(aAttachment, KNullDesC8, 0);
	}

void CSendAsAttachment::AddExistingFileAttachmentWithMimeTypeL(RFile& aAttachment, const TDesC8& aMimeType, TUint aCharset)
	{
	__ASSERT_ALWAYS( iState == EIdle, TSendAsServerPanic::Panic(TSendAsServerPanic::ESendAsAttachmentBadState) );
		
	iClientMtm.AddAttachmentL(aAttachment, aMimeType, aCharset, iStatus);
	SetActive();

	iState = EAddingAttachment;
	}

void CSendAsAttachment::AddFileLinkAttachmentL(const TDesC& aFileName)
	{
	AddFileLinkAttachmentWithMimeTypeL(aFileName, KNullDesC8, 0);
	}

void CSendAsAttachment::AddFileLinkAttachmentWithMimeTypeL(const TDesC& aFileName, const TDesC8& aMimeType, TUint aCharset)
	{
	__ASSERT_ALWAYS( iState == EIdle, TSendAsServerPanic::Panic(TSendAsServerPanic::ESendAsAttachmentBadState) );
	
	iClientMtm.AddLinkedAttachmentL(aFileName, aMimeType, aCharset, iStatus);
	SetActive();

	iState = EAddingAttachment;
	}

void CSendAsAttachment::CreateNewFileAttachmentL(RFile& aAttachment, const TDesC& aFileName)
	{
	CreateNewFileAttachmentWithMimeTypeL(aAttachment, aFileName, KNullDesC8, 0);
	}

void CSendAsAttachment::CreateNewFileAttachmentWithMimeTypeL(RFile& aAttachment, const TDesC& aFileName, const TDesC8& aMimeType, TUint aCharset)
	{
	__ASSERT_ALWAYS( iState == EIdle, TSendAsServerPanic::Panic(TSendAsServerPanic::ESendAsAttachmentBadState) );

	iClientMtm.CreateAttachmentL(aFileName, aAttachment, aMimeType, aCharset, iStatus);
	SetActive();

	iState = ECreatingAttachment;
	}
	
void CSendAsAttachment::CompleteL(TInt aError)
	{
	if( iObserver != NULL )
		{
		// notify observer that the attachment has completed.
		// transference of the attachment file to the client on create
		// attachment completes the RMessage2.
		iObserver->AttachmentCompleteL(aError, (iState != ECreatingAttachment));
		}	
	}
	
/*
 *	Methods from CActive
 */

void CSendAsAttachment::RunL()
	{
	User::LeaveIfError(iStatus.Int());
	
	switch( iState )
		{
	case ECreatingAttachment:
		// drop through to next case...
	case EAddingAttachment:
		{
		CompleteL(KErrNone);
		} break;
	case EIdle:
	case EAttachmentComplete:
	default:
		User::Invariant();
		break;
		}
	}

void CSendAsAttachment::DoCancel()
	{
	iClientMtm.CancelAttachmentOperation();
	TRAP_IGNORE(CompleteL(KErrCancel));
	}
	
TInt CSendAsAttachment::RunError(TInt aError)
	{
	TInt error = KErrNone;
	TRAP(error,CompleteL(aError));
	return error;
	}
