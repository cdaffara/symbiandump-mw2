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

#include "SENDAS2MTM.H"


TInt CSendMtm::QueryCapability(TUid aCapability, TInt& /*aResponse*/)
	{
	TInt ret;
	switch (aCapability.iUid)
		{
		// supported
		case KUidMtmQueryCanReceiveMsgValue:
		case KUidMtmQuerySupportAttachmentsValue:
			ret=KErrNone;
			break;
		// not supported
		case KUidMtmQuerySendAsMessageSendSupportValue:	// can't send with sendas2
		case KUidMtmQuerySupportsRecipientTypeValue:
		case KUidMtmQueryCanSendMsgValue:
		case KUidMtmQueryOffLineAllowedValue:
		default:
			ret=KErrNotSupported;
			break;
		}
	return ret;
	}


void CSendMtm::SetSubjectL(const TDesC& /*aSubject*/)
	{
	}

TUint CSendMtm::ValidateMessage(TMsvPartList /*aPartList*/)
	{
	return 0;
	}

void CSendMtm::AddAddresseeL(TMsvRecipientType /*aType*/, const TDesC& /*aRealAddress*/)
	{
	User::Leave(KErrNotSupported);	
	}
	
void CSendMtm::AddAddresseeL(TMsvRecipientType /*aType*/, const TDesC& /*aRealAddress*/, const TDesC& /*aAlias*/)
	{
	User::Leave(KErrNotSupported);	
	}
