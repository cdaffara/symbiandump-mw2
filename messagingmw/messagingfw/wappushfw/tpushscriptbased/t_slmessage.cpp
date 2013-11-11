// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <push/sislpushmsgutils.h>
#include <push/cslpushmsgentry.h>
#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"
#include <push/pushmessage.h>

#include "httptestutils.h"
#if !defined(__TESTSCRIPTS_H__)
#include "TestScripts.h"
#endif
				
_LIT8(KSLHref, "http://www.xyz.com/ppaid/123/abc.wml");

CWapPushSLMessageTest::CWapPushSLMessageTest(TInt aIndex, CHTTPTestUtils* aTestUtils):CWapPushMessageTest(aIndex, aTestUtils)
	{
	}
	
const TDesC& CWapPushSLMessageTest::TestName()
	{
	_LIT(KTextSLMessageTest, "Service Loading");
	return KTextSLMessageTest;
	}

void CWapPushSLMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 1;
	}

TDesC8& CWapPushSLMessageTest::Uri() const
	{
	return (*(iUri));
	}
	
void CWapPushSLMessageTest::PrepareTestMessageL(TInt aTestCase)
	{
	ReadAndSetParamsFromIniFileL();
	
	CWapPushMsgGen* msgGen = CWapPushMsgGen::NewL();
	CleanupStack::PushL(msgGen);

	msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);	
	
	if(iHRef)
		{
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, GetHRef());
		}
	if(iAction)
		{
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, GetAction());
		}
	
	const TUint8 KMessageHeader2[] = 
		{
		0xAF,
		0x92,  // Date
		0x04,  // length of field
		0x39,  // Monday 15 August 2000 10:41:37 GMT
		0x90,	
		0x6a,
		0xd0,
		0x94,  // Expires 
		0x04,  // length of field
		0x39,  // Monday 15 August 2000 10:41:37 GMT
		0x9a,	
		0x6a,
		0xd0,
		0xAF,  //X-Wap-Application-ID
		0x82  // Short int - 0x02 represent x-wap-application:push.ua
		};	

	TBuf8<40> tempBuf;
	TUint8 value;
	
	for (TInt i = 0; i < 15; i++)
		{
		value = KMessageHeader2[i];
		tempBuf.Append(value);
		}

	iHeadersBuf.Copy(tempBuf);
	tempBuf.Copy(_L8(""));

	iBodyBuf.Delete(0,KBufferSize);
	HBufC8* msg = msgGen->CreateMsgLC();
	
	switch (aTestCase)
		{
	case 0 : 
		iBodyBuf = msg->Des(); 
		break;
	
	default: 
		// do nothing
		break;
		}

	CleanupStack::PopAndDestroy(2); //msgGen, msg
	}

TBool CWapPushSLMessageTest::ConfirmMessagesSavedL()
	{
	HBufC* tempHref = HBufC::NewLC(KSLHref().Length());
	tempHref->Des().Copy(KSLHref());
	
	TInt returnVal = 0;
	
	CSISLPushMsgUtils *wapPushUtils = CSISLPushMsgUtils::NewL();
	CleanupStack::PushL(wapPushUtils);
		
	TMsvId matchingSlMsgEntryId; 
	matchingSlMsgEntryId = wapPushUtils->FindUrlL(*tempHref, KUidWapPushMsgSL);
	
	if(matchingSlMsgEntryId)
		{
		HBufC8* header = iHeadersBuf.AllocLC();
		HBufC8* body = iBodyBuf.AllocLC();
		
		CPushMessage* pushMsg = CPushMessage::NewL(header, body);
		CleanupStack::Pop(2); // body, header
		CleanupStack::PushL(pushMsg);
							
		TBool isInt;
		TPtrC8 appURI;
		TInt appID=0;
		pushMsg->GetAppIdL(appURI, appID, isInt);
		CSLPushMsgEntry* slEntry=NULL;
		if (isInt)
			{
			slEntry = CSLPushMsgEntry::NewL(appID);
			}
		else
			{
			slEntry = CSLPushMsgEntry::NewL(appURI);
			}
		CleanupStack::PushL(slEntry);
			
		slEntry->RetrieveL(wapPushUtils->Session(), matchingSlMsgEntryId);
		
		TInt trusted = slEntry->Trusted();
		TPtrC8 ptr(slEntry->MsgOriginUri());
		
		RInetUriList inetUriList;
		InetUriList::TListType listType;
		TRAPD(err, inetUriList.OpenL());
		if(err == KErrNone)
			{
			TPtrC8 serverAddress(iUri->Des());
			err = inetUriList.GetListType(serverAddress, InetUriList::EWapPush, listType);
			}
		inetUriList.Close();
			if(trusted == 1)
			{
			// whitelist
			if((iUri->Compare(slEntry->MsgOriginUri()) == 0) && (err == KErrNone) && (listType == InetUriList::EWhiteList))
				{
				returnVal = KErrNone;
				}
			else
				{
				returnVal = KErrGeneral;	
				}
			}
		else if(trusted == 0)
			{
			// unknown
			if((iUri->Compare(slEntry->MsgOriginUri()) == 0) && (err == InetUriList::KErrUriNotFound))
				{
				returnVal = KErrNone;
				}
			else
				{
				returnVal = KErrGeneral;	
				}
			}
			
		wapPushUtils->DeleteEntryNowL(matchingSlMsgEntryId);
		CleanupStack::PopAndDestroy(slEntry);
		CleanupStack::PopAndDestroy(pushMsg);
		}
	
	CleanupStack::PopAndDestroy(wapPushUtils);
	CleanupStack::PopAndDestroy(tempHref);
	return returnVal;
	}


