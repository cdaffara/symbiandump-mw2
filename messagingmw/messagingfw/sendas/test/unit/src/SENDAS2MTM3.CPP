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
#include <mtmdef.h>


TInt CSendMtm::QueryCapability(TUid aCapability, TInt& aResponse)
	{
	TInt ret=KErrNone;
	switch (aCapability.iUid)
		{
		// supported
		case KUidMtmQuerySendAsMessageSendSupportValue:
		case KUidMtmQueryCanSendMsgValue:
		case KUidMtmQueryOffLineAllowedValue:
			break;
		// supported with responses
		case KUidMtmQueryMaxBodySizeValue:
			aResponse = 0x10000;
			break;
		case KUidMtmQuerySupportedBodyValue:
			aResponse = KMtm7BitBody|KMtm8BitBody;
			break;
		case KUidMtmQueryMaxTotalMsgSizeValue:
			aResponse = 0x40000;
			break;
		// not supported
		case KUidMtmQuerySupportsRecipientTypeValue:
		case KUidMtmQueryCanReceiveMsgValue:
		case KUidMtmQuerySupportAttachmentsValue:
		default:
			ret=KErrNotSupported;
			break;
		}
	return ret;
	}

TUint CSendMtm::ValidateMessage(TMsvPartList /*aPartList*/)
	{
	return KMsvMessagePartBody;
	}


void CSendMtm::SetSubjectL(const TDesC& /*aSubject*/)
	{
	User::Leave(KErrCancel);
	}

void CSendMtm::AddAddresseeL(TMsvRecipientType /*aType*/, const TDesC& /*aRealAddress*/)
	{
	User::Leave(KErrNotSupported);	
	}
	
void CSendMtm::AddAddresseeL(TMsvRecipientType /*aType*/, const TDesC& /*aRealAddress*/, const TDesC& /*aAlias*/)
	{
	User::Leave(KErrNotSupported);	
	}

