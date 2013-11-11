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

#include "csendastestuimtm.h"

_LIT(KSendAsTestUiMtmResourceFile, "\\resource\\messaging\\sendastestuimtm.rss");

EXPORT_C CSendAsTestUiMtm* CSendAsTestUiMtm::NewL(CBaseMtm& aBaseMtm, CRegisteredMtmDll& aRegisteredMtmDll)
	{
	CSendAsTestUiMtm* self = new (ELeave) CSendAsTestUiMtm(aBaseMtm, aRegisteredMtmDll);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}
	
CSendAsTestUiMtm::~CSendAsTestUiMtm()
	{
	}

CSendAsTestUiMtm::CSendAsTestUiMtm(CBaseMtm& aBaseMtm, CRegisteredMtmDll& aRegisteredMtmDll)
: CBaseMtmUi(aBaseMtm, aRegisteredMtmDll)
	{
	}
	
void CSendAsTestUiMtm::ConstructL()
	{
    CBaseMtmUi::ConstructL();
	}

/*
 *	Methods from CBaseMtmUi
 */
 
CMsvOperation* CSendAsTestUiMtm::OpenL(TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::CloseL(TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::EditL(TRequestStatus& aStatus)
	{
	TMsvEntry entry = BaseMtm().Entry().Entry();
	entry.SetMtmData3(1234567890); // Show we've been called by touching the TMsvEntry.
	BaseMtm().Entry().ChangeL(entry);
	return CMsvCompletedOperation::NewL(Session(), Type(), KNullDesC8, BaseMtm().Entry().OwningService(), aStatus, entry.iError);
	}
	
CMsvOperation* CSendAsTestUiMtm::ViewL(TRequestStatus& /*aStatus*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::OpenL(TRequestStatus& /*aStatus*/, const CMsvEntrySelection& /*aSelection*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::CloseL(TRequestStatus& /*aStatus*/, const CMsvEntrySelection& /*aSelection*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::EditL(TRequestStatus& /*aStatus*/, const CMsvEntrySelection& /*aSelection*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::ViewL(TRequestStatus& /*aStatus*/, const CMsvEntrySelection& /*aSelection*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::CancelL(TRequestStatus& /*aStatus*/, const CMsvEntrySelection& /*aSelection*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}

CMsvOperation* CSendAsTestUiMtm::ReplyL(TMsvId /*aDestination*/, TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::ForwardL(TMsvId /*aDestination*/, TMsvPartList /*aPartlist*/, TRequestStatus& /*aCompletionStatus*/)
	{
	User::Leave(KErrNotSupported);
	return NULL;
	}
	
CMsvOperation* CSendAsTestUiMtm::ConfirmSendL(TRequestStatus& aStatus, const CMsvEntrySelection& /*aSelection*/, const TSecurityInfo& /*aClientInfo*/)
	{
	// Use the error value of the context entry to indicate whether the send is
	// confirmed (iError == KErrNone) or not (iError != KErrNone).
	TMsvEntry entry = BaseMtm().Entry().Entry();
	return CMsvCompletedOperation::NewL(Session(), Type(), KNullDesC8, BaseMtm().Entry().OwningService(), aStatus, entry.iError);
	}
	
void CSendAsTestUiMtm::GetResourceFileName(TFileName& aFileName) const
	{
	aFileName = KSendAsTestUiMtmResourceFile();
	}
