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
#include <push/csipushmsgentry.h>
#include <push/pushmessage.h>
#include <ineturilist.h>
#include <ineturilistdef.h>

#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"

#include "httptestutils.h"
#if !defined(__TESTSCRIPTS_H__)
#include "TestScripts.h"
#endif


_LIT(KWapIniFolder, 	"wapini");
_LIT(KWapIni, 			"wap.ini");

_LIT(KSiId,			"SI-Id");
_LIT(KSICreated,	"SI-Created");
_LIT(KSIExpires,	"SI-Expires");

CWapPushSIMessageTest::CWapPushSIMessageTest(TInt aIndex, CHTTPTestUtils* aTestUtils):CWapPushMessageTest(aIndex, aTestUtils)
	{
	}
	
CWapPushSIMessageTest::~CWapPushSIMessageTest()
	{
	delete iSiId;
	delete iSiCreatedDate;
	delete iSiExpiresDate;
	}

const TDesC& CWapPushSIMessageTest::TestName()
	{
	_LIT(KTextSIMessageTest, "Service Indication");
	return KTextSIMessageTest;
	}

void CWapPushSIMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 1;
	}

TDesC8& CWapPushSIMessageTest::GetSiId() const
	{
	return (*(iSiId));		
	}
	
TDesC8& CWapPushSIMessageTest::GetSICreatedDate() const
	{
	return(*(iSiCreatedDate));
	}
	
TDesC8& CWapPushSIMessageTest::GetSIExpiresDate() const
	{
	return(*(iSiExpiresDate));	
	}

TDesC8& CWapPushSIMessageTest::Uri() const
	{
	return (*(iUri));
	}
	
void CWapPushSIMessageTest::ReadAndSetSIParamsFromIniFileL()
	{
	// Delete previously read buffers.
	delete iSiId;
	delete iSiCreatedDate;
	delete iSiExpiresDate;

	
	CScriptFile* iniFile = NULL;
	iniFile = CScriptFile::NewL(*iTestUtils, KWapIniFolder(), KWapIni());
	CleanupStack::PushL(iniFile); 
		
	TInt itemFieldCount= iniFile->Section(iSectionIndex).Items().Count();
	
	TBuf8<200> itemFieldValue8;
	
	for (TInt fctr=0; fctr < itemFieldCount ; ++fctr)
		{
		TPtrC itemFieldPtr(iniFile->Section(iSectionIndex).Item(fctr).Item());
		itemFieldValue8.Copy(iniFile->Section(iSectionIndex).Item(fctr).Value());
		
		if(itemFieldPtr.Compare(KSiId) == 0)
			{
			iSiId = itemFieldValue8.AllocL();
			RemoveQuotes(iSiId);
			}
		else if(itemFieldPtr.Compare(KSICreated) == 0)
			{
			iSiCreatedDate = itemFieldValue8.AllocL();
			RemoveQuotes(iSiCreatedDate);
			}
		else if(itemFieldPtr.Compare(KSIExpires) == 0)
			{
			iSiExpiresDate = itemFieldValue8.AllocL();
			RemoveQuotes(iSiExpiresDate);
			}
		}
	CleanupStack::PopAndDestroy(iniFile);	
	}

void CWapPushSIMessageTest::PrepareTestMessageL(TInt aTestCase)
	{
	ReadAndSetParamsFromIniFileL();
	ReadAndSetSIParamsFromIniFileL();
	
	CWapPushMsgGen* msgGen = CWapPushMsgGen::NewL();
	CleanupStack::PushL(msgGen);

	msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);	
	
	if(iHRef)
		{
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, GetHRef());
		}
	if(iAction)
		{
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, GetAction());
		}
	if(iSiId)
		{
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, GetSiId());
		}
	if(iSiCreatedDate)
		{
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, GetSICreatedDate());
		}
	if(iSiExpiresDate)
		{
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationExpires, GetSIExpiresDate());
		}

	const TUint8 KMessageHeader2[] = 
		{
		0xAD,
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
	
	switch (aTestCase)
		{
	case 0 : 
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("You have 4 new e-mails"));		
		break;
	
	default: 
		// do nothing
		break;
		}

	HBufC8* msg = msgGen->CreateMsgLC();
	iBodyBuf = msg->Des(); 

	CleanupStack::PopAndDestroy(2); //msgGen, msg
	}

TBool CWapPushSIMessageTest::ConfirmMessagesSavedL()
	{
		TInt returnVal = 0;
		const TDesC& siId16 = _L("http://www.xyz.com/ppaid/123/abc.wml"); 
		
		CSISLPushMsgUtils *wapPushUtils = CSISLPushMsgUtils::NewL();
		CleanupStack::PushL(wapPushUtils);
		
		CMsvEntrySelection* matchingIdList;
		matchingIdList = wapPushUtils->FindSiIdLC(siId16);
		
		TMsvId matchingSiMsgEntryId; 
		
		if(matchingIdList->Count())
			{
			TInt matchingListCount = matchingIdList->Count();
			matchingSiMsgEntryId = matchingIdList->At(matchingListCount-1);
		
			HBufC8* header = iHeadersBuf.AllocLC();
			HBufC8* body = iBodyBuf.AllocLC();
			
			CPushMessage* pushMsg = CPushMessage::NewL(header, body);
			CleanupStack::Pop(2); // body, header
			CleanupStack::PushL(pushMsg);
							
			TBool isInt;
			TPtrC8 appURI;
			TInt appID=0;
			pushMsg->GetAppIdL(appURI, appID, isInt);
			CSIPushMsgEntry* siEntry=NULL;
			if (isInt)
				{
				siEntry = CSIPushMsgEntry::NewL(appID);
				}
			else
				{
				siEntry = CSIPushMsgEntry::NewL(appURI);
				}
			CleanupStack::PushL(siEntry);
			
			siEntry->RetrieveL(wapPushUtils->Session(), matchingSiMsgEntryId);
			
			TInt trusted = siEntry->Trusted();
			TPtrC8 ptr(siEntry->MsgOriginUri());
			
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
				if((iUri->Compare(siEntry->MsgOriginUri()) == 0) && (err == KErrNone) && (listType == InetUriList::EWhiteList))
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
				if((iUri->Compare(siEntry->MsgOriginUri()) == 0) && (err == InetUriList::KErrUriNotFound))
					{
					returnVal = KErrNone;
					}
				else
					{
					returnVal = KErrGeneral;	
					}
				}
			
			wapPushUtils->DeleteEntryNowL(matchingSiMsgEntryId);
			CleanupStack::PopAndDestroy(siEntry);
			CleanupStack::PopAndDestroy(pushMsg);
		
			}
		CleanupStack::PopAndDestroy(matchingIdList);
		
		CleanupStack::PopAndDestroy(wapPushUtils);
			
	return returnVal;
	}	


