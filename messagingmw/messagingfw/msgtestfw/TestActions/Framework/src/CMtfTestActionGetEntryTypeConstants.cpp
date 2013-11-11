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
// GetEntryTypeConstants
// [Action Parameters]
// MessageEntryId	<output>: Value of Message entry uid.
// FolderEntryId	<output>: Value of Folder entry uid.
// TextEntryId		<output>: Value of Text entry uid.
// HtmlEntryId		<output>: Value of Html entry uid.
// AttachmentEntryId	<output>: Value of Attachment entry uid.
// [Action Description]
// Initialises message entry type related constants.
// [APIs Used]
// none.
// __ACTION_INFO_END__
// 
//

/**
 @file
*/

#include "CMtfTestActionGetEntryTypeConstants.h"
#include "CMtfTestCase.h"
#include "CMtfTestActionParameters.h"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS  
#include "timrfc822datefield.h"
#endif

CMtfTestAction* CMtfTestActionGetEntryTypeConstants::NewL(CMtfTestCase& aTestCase,CMtfTestActionParameters* aActionParameters)
	{
	CMtfTestActionGetEntryTypeConstants* self = new (ELeave) CMtfTestActionGetEntryTypeConstants(aTestCase);
	CleanupStack::PushL(self);
	self->ConstructL(aActionParameters);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionGetEntryTypeConstants::CMtfTestActionGetEntryTypeConstants(CMtfTestCase& aTestCase)
	: CMtfSynchronousTestAction(aTestCase)
	{
	}


CMtfTestActionGetEntryTypeConstants::~CMtfTestActionGetEntryTypeConstants()
	{
	}


void CMtfTestActionGetEntryTypeConstants::ExecuteActionL()
	{
	TestCase().INFO_PRINTF2(_L("Test Action %S start..."), &KTestActionGetEntryTypeConstants);
	TUid paramMessageEntryId = KUidMsvMessageEntry;
	StoreParameterL<TUid>(TestCase(),paramMessageEntryId,ActionParameters().Parameter(0));

	TUid paramFolderEntryId = KUidMsvFolderEntry;
	StoreParameterL<TUid>(TestCase(),paramFolderEntryId,ActionParameters().Parameter(1));

	TUid paramTextEntryId = KUidMsvEmailTextEntry;
	StoreParameterL<TUid>(TestCase(),paramTextEntryId,ActionParameters().Parameter(2));

	TUid paramHtmlEntryId = KUidMsvEmailHtmlEntry;
	StoreParameterL<TUid>(TestCase(),paramHtmlEntryId,ActionParameters().Parameter(3));
	
	TUid paramAttachmentEntryId = KUidMsvAttachmentEntry;
	StoreParameterL<TUid>(TestCase(),paramAttachmentEntryId,ActionParameters().Parameter(4));
	
	TestCase().INFO_PRINTF2(_L("Test Action %S completed."), &KTestActionGetEntryTypeConstants);
	TestCase().ActionCompletedL(*this);
	}

