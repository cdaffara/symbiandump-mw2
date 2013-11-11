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

const TUid KUidRenderingUid={0x10001000};

TInt CSendMtm::QueryCapability(TUid aCapability, TInt& aResponse)
	{
	TInt ret=KErrNone;
	switch (aCapability.iUid)
		{
		// supported
		case KUidMtmQuerySendAsMessageSendSupportValue:		
		case KUidMtmQuerySupportsRecipientTypeValue:
		case KUidMtmQueryCanSendMsgValue:
		case KUidMtmQueryOffLineAllowedValue:
		case KUidMtmQuerySupportAttachmentsValue:
			break;
		// supported with responses
		case KUidMtmQueryMaxBodySizeValue:
			aResponse = 0x1200;
			break;
		case KUidMtmQuerySupportedBodyValue:
			aResponse = KMtm7BitBody|KMtm8BitBody;
			break;
		case KUidMtmQueryMaxTotalMsgSizeValue:
			aResponse = 0x1200;
			break;
		case KUidMtmQuerySendAsRequiresRenderedImageValue:
			aResponse = ETrue;
			break;
		case KUidMtmQuerySendAsRenderingUidValue:
			aResponse = KUidRenderingUid.iUid;
			break;
		// not supported
		
		case KUidMtmQueryCanReceiveMsgValue:
		default:
			ret=KErrNotSupported;
			break;
		}
	return ret;
	}

TUint CSendMtm::ValidateMessage(TMsvPartList aPartList)
	{
	return aPartList;
	}

void CSendMtm::SetSubjectL(const TDesC& /*aSubject*/)
	{
	User::Leave(KErrNoMemory);
	}

void CSendMtm::AddAddresseeL(TMsvRecipientType /*aType*/, const TDesC& aRealAddress)
	{
	AddAddresseeL(aRealAddress);
	}
	
void CSendMtm::AddAddresseeL(TMsvRecipientType /*aType*/, const TDesC& aRealAddress, const TDesC& aAlias)
	{
	AddAddresseeL(aRealAddress, aAlias);
	}


	
