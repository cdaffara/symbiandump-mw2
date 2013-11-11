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
// __ACTION_INFO_BEGIN__ 
// [Action Name]
// SmtpModifyAttachmentInfo
// [Action Parameters]
// Session        <input>: Reference to the session.
// MsgId		  <input>: Value of the message Id.
// AttachmentId   <input>: Attachment to add the mime headers to.
// [Action Description]
// Creates a set of test mime headers
// [APIs Used]
// CMsvSession::GetEntryL
// CImEmailMessage::AttachmentManagerL
// MMsvAttachmentManager::AttachmentCount
// MMsvAttachmentManager::ModifyAttachmentInfoL
// __ACTION_INFO_END__
// 
//


#include "CMtfTestActionSmtpModifyAttachmentInfo.h"
#include "CMtfTestCase.h"
#include "CMtfAsyncWaiter.h"
#include "CMtfTestActionParameters.h"
#include "MtfTestActionUtilsUser.h"

#include <miutset.h>
#include <mmsvattachmentmanager.h>
#include <cmsvmimeheaders.h>
#include <miutmsg.h>

_LIT8(KTxtContentDescription, "Attachment API Modify Attachment");
_LIT8(KTxtContentBase, "TestContentBase");
_LIT(KTxtContentLocation, "http://attachmentTest.AttachmentAPI/TestImage.jpg");
_LIT8(KTxtContentId, "attachmentPart@attachmentTest.AttachmentAPI");
_LIT8(KTxtContentType, "image");
_LIT8(KTxtContentSubType, "jpg");
_LIT8(KTxtContentDisposition, "Attachment");
_LIT8(KTxtParam1, "Param1");
_LIT8(KTxtParam2, "Param2");
_LIT8(KTxtParam3, "Param3");
_LIT8(KTxtParam4, "Param4");
_LIT8(KTxtParam5, "Param5");
_LIT8(KTxtParam6, "Param6");
_LIT8(KTxtParam7, "Param7");
_LIT8(KTxtParam8, "Param8");
_LIT8(KTxtParam9, "Param9");

CMtfTestAction* CMtfTestActionSmtpModifyAttachmentInfo::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionSmtpModifyAttachmentInfo* self = new(ELeave) CMtfTestActionSmtpModifyAttachmentInfo(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionSmtpModifyAttachmentInfo::CMtfTestActionSmtpModifyAttachmentInfo(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionSmtpModifyAttachmentInfo::~CMtfTestActionSmtpModifyAttachmentInfo()
	{
	}

void CMtfTestActionSmtpModifyAttachmentInfo::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionSmtpModifyAttachmentInfo);
	CMsvSession* paramSession = ObtainParameterReferenceL<CMsvSession>(TestCase(),ActionParameters().Parameter(0));
	TMsvId messageEntry = ObtainValueParameterL<TMsvId>(TestCase(),ActionParameters().Parameter(1));
	TMsvAttachmentId attachmentId = ObtainValueParameterL<TMsvAttachmentId>(TestCase(),ActionParameters().Parameter(2));

	CMsvEntry* entry = paramSession->GetEntryL(messageEntry);
	CleanupStack::PushL(entry);
	
	CImEmailMessage* emailMsg = CImEmailMessage::NewL(*entry);
	CleanupStack::PushL(emailMsg);

	MMsvAttachmentManager& manager = emailMsg->AttachmentManager();
	
	CMsvMimeHeaders* mimeHeaders = CMsvMimeHeaders::NewL();
	CleanupStack::PushL(mimeHeaders);

	mimeHeaders->SetContentDescriptionL(KTxtContentDescription());
	mimeHeaders->SetContentBaseL(KTxtContentBase());
	mimeHeaders->SetContentLocationL(KTxtContentLocation());
	mimeHeaders->SetContentIdL(KTxtContentId());
	mimeHeaders->SetContentTypeL(KTxtContentType());
	mimeHeaders->SetContentSubTypeL(KTxtContentSubType());
	mimeHeaders->SetContentDispositionL(KTxtContentDisposition());
	mimeHeaders->ContentTypeParams().AppendL(KTxtParam1());
	mimeHeaders->ContentTypeParams().AppendL(KTxtParam2());
	mimeHeaders->ContentTypeParams().AppendL(KTxtParam3());
	mimeHeaders->ContentDispositionParams().AppendL(KTxtParam4());
	mimeHeaders->ContentDispositionParams().AppendL(KTxtParam5());
	mimeHeaders->ContentDispositionParams().AppendL(KTxtParam6());
	mimeHeaders->XTypeParams().AppendL(KTxtParam7());
	mimeHeaders->XTypeParams().AppendL(KTxtParam8());
	mimeHeaders->XTypeParams().AppendL(KTxtParam9());

	CMtfAsyncWaiter* waiter = CMtfAsyncWaiter::NewL();
	CleanupStack::PushL(waiter);
	CMsvAttachment* attachment = manager.GetAttachmentInfoL(attachmentId);
	CleanupStack::PushL(attachment);
	mimeHeaders->StoreL(*attachment);
	manager.ModifyAttachmentInfoL(attachment, waiter->iStatus);
	CleanupStack::Pop(attachment);
	waiter->StartAndWait();
	User::LeaveIfError(waiter->Result());

	CleanupStack::PopAndDestroy(2, mimeHeaders); // waiter, mimeHeaders
	
	CompareMimeHeadersL(*emailMsg, attachmentId);
	
	CleanupStack::PopAndDestroy(2, entry);//emailMsg, entry

	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionSmtpModifyAttachmentInfo);
	TestCase().ActionCompletedL(*this);
	}


void CMtfTestActionSmtpModifyAttachmentInfo::CompareMimeHeadersL(CImEmailMessage& imEmailMsg, TMsvAttachmentId aAttachId)
{
	CMsvAttachment* attachment = imEmailMsg.AttachmentManager().GetAttachmentInfoL(aAttachId);
	CleanupStack::PushL(attachment);
	
	CMsvMimeHeaders* mimeHeaders = CMsvMimeHeaders::NewL();
	CleanupStack::PushL(mimeHeaders);
	
	mimeHeaders->RestoreL(*attachment);
	if( mimeHeaders->ContentDescription().Compare(KTxtContentDescription()) != 0 )
		User::Leave(KErrNotFound);
	if( mimeHeaders->ContentBase().Compare(KTxtContentBase()) != 0 )
		User::Leave(KErrNotFound);
	if( mimeHeaders->ContentLocation().Compare(KTxtContentLocation()) != 0 )
		User::Leave(KErrNotFound);
	if( mimeHeaders->ContentId().Compare(KTxtContentId()) != 0 )
		User::Leave(KErrNotFound);
	if( mimeHeaders->ContentType().Compare(KTxtContentType()) != 0 )
		User::Leave(KErrNotFound);
	if( mimeHeaders->ContentSubType().Compare(KTxtContentSubType()) != 0 )
		User::Leave(KErrNotFound);
	if( mimeHeaders->ContentDisposition().Compare(KTxtContentDisposition()) != 0 )
		User::Leave(KErrNotFound);
	
	CDesC8Array& desArray1 = mimeHeaders->ContentTypeParams();
	if( desArray1.Count() == 3 )
		{
		if( desArray1[0].Compare(KTxtParam1()) != 0 )
			User::Leave(KErrNotFound);
		if( desArray1[1].Compare(KTxtParam2()) != 0 )
			User::Leave(KErrNotFound);
		if( desArray1[2].Compare(KTxtParam3()) != 0 )
			User::Leave(KErrNotFound);
		}
	else
		User::Leave(KErrNotFound);
	
	CDesC8Array& desArray2 = mimeHeaders->ContentDispositionParams();
	if( desArray2.Count() == 3 )
		{
		if( desArray2[0].Compare(KTxtParam4()) != 0 )
			User::Leave(KErrNotFound);
		if( desArray2[1].Compare(KTxtParam5()) != 0 )
			User::Leave(KErrNotFound);
		if( desArray2[2].Compare(KTxtParam6()) != 0 )
			User::Leave(KErrNotFound);
		}
	else
		User::Leave(KErrNotFound);

	CDesC8Array& desArray3 = mimeHeaders->XTypeParams();
	if( desArray3.Count() == 3 )
		{
		if( desArray3[0].Compare(KTxtParam7()) != 0 )
			User::Leave(KErrNotFound);
		if( desArray3[1].Compare(KTxtParam8()) != 0 )
			User::Leave(KErrNotFound);
		if( desArray3[2].Compare(KTxtParam9()) != 0 )
			User::Leave(KErrNotFound);
		}
	else
		User::Leave(KErrNotFound);
	
	CleanupStack::PopAndDestroy(2, attachment); // mimeHeaders, attachment
}