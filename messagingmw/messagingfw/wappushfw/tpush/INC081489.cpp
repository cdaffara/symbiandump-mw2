// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include  <push/sislpushmsgutils.h>

#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"
#include <push/csipushmsgentry.h>


// si text message data

_LIT8(KSIHref1, "http://www.xyz.com/ppaid/1231/abc.wml");
_LIT8(KSIHref4, "http://www.xyz.com/ppaid/1234/abc.wml");
_LIT8(KSIHref5, "http://www.xyz.com/ppaid/1236/abc.wml");
_LIT8(KDate1,	"2006-02-22T07:58:01Z");
_LIT8(KDate2,	"2006-02-22T08:58:01Z");
_LIT8(KDate3,	"2006-02-22T09:58:01Z");
_LIT8(KDate4,	"2006-02-22T10:58:01Z");
_LIT8(KDate5,	"2006-02-22T11:58:01Z");
_LIT8(KDate6,	"2006-02-22T12:58:01Z");
_LIT8(KDate7,	"2006-02-22T13:58:01Z");
_LIT8(KDate8,	"2006-02-22T14:58:01Z");
_LIT8(KDate9,	"2006-02-22T15:58:01Z");
_LIT8(KDate10,	"2006-02-22T16:58:01Z");
_LIT8(KDate11,	"2006-02-22T17:58:01Z");
_LIT8(KSISiId1, "http://www.xyz.com/ppaid/1231/abc.wml");
_LIT8(KSISiId2, "http://www.xyz.com/ppaid/1232/abc.wml");
_LIT8(KSISiId3, "http://www.xyz.com/ppaid/1233/abc.wml");
_LIT8(KSISiId5, "http://www.xyz.com/ppaid/1235/abc.wml");

/** set test name to 'Duplicate SI messages [CINC081489]'
	@param void
	@return string
		string represents test name
*/
const TDesC& CINC081489::TestName()
{
	_LIT(KTextSIMessageTest, "Duplicate SI messages [CINC081489]");
	return KTextSIMessageTest;
}

/** set the number of test messages to use */
void CINC081489::NumberTestCasesToRun()
{
	iNumberOfTestCases = 16;
}

/** build different test messages to test specific SI reception aspects
	in the SI content handler processing code.
	@param TInt
		the number of test messages to create and use
	@return void
*/
void CINC081489::PrepareTestMessageL(TInt aTestCase)
{
	// this set of test messages adds a test message
	// to the store for the match cases, 
	// replaces the stored message and ends with no stored si message 	
	const TUint8 KMessageHeader2[] = 
		{
		//0xB0,  //Content type "application/vnd.wap.slc"
		0xAD,  //Content type "application/vnd.wap.si" 
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
	//iBodyBuf.Zero();
		
	CWapPushMsgGen* msgGen = CWapPushMsgGen::NewL();
	CleanupStack::PushL(msgGen);

	msgGen->StartNewMsgL(CWapPushMsgGen::EServiceIndication);
		
	switch (aTestCase)
		{
	// INC081489 - part 1
	// Three messages with same EHRef, EServiceIndicationID and different EServiceIndicationCreated
	case 0 :
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref1);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId1);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate1);
		break;
	case 1 :
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref1);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId1);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate2);
		break;	
	case 2 :
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref1);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId1);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate3);
		break;
	case 3 :// INC081489 - part 2
			//Three messages with same EHRef, EServiceIndicationID and EServiceIndicationCreated
	case 4 :
	case 5 :
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref1);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId2);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate1);
		break;
	case 6 : // INC081568 - part 1
			 // Same "si-id","created" and no value for the "href" attribute.
	case 7 :
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId3);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate4);
		break;
	case 8 : // INC082189
			 // Two Push SI messages,the same "href"and "created",no "si-id",
	case 9 : // 
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref4);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate5);
		break;
		
	 // INC082190: Three Push SI msgs, the same "si-id",different "created",no "href" 	
	case 10 :
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId5);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate6);
		break;
	case 11 : 
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId5);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate7);
		break;
	case 12 :
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId5);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate8);
		break;
		
	 // INC082191: Three Push SI msgs, the same "href",different "created",no "si-id" 	
	case 13 :
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref5);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate9);
		break;
	case 14 : 
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref5);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate10);
		break;
	case 15 :
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref5);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KDate11);
		break;

	default: 
		// do nothing
		break;
	}
	msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("You have 4 new e-mails"));

	HBufC8* msg = msgGen->CreateMsgLC();
	iBodyBuf = msg->Des(); 
	CleanupStack::PopAndDestroy(2); //msgGen, msg
}

/** confirm the message has been stored.
	Writes the number of stored messages found to the log file - if present.
	@param void
	@return TBool
		returns ETrue if messages found
		returns EFalse if no messages found 
*/
TBool CINC081489::ConfirmMessagesSavedL()
	{
	CSISLPushMsgUtils* wapPushUtils = CSISLPushMsgUtils::NewL();
	CleanupStack::PushL(wapPushUtils);
	
	TMsvId localFolderId;
	wapPushUtils->GetPushMsgFolderIdL(localFolderId);
	
	const TDesC& siId1 = _L("http://www.xyz.com/ppaid/1231/abc.wml");
	const TDesC& siId2 = _L("http://www.xyz.com/ppaid/1232/abc.wml");
	const TDesC& siId3 = _L("http://www.xyz.com/ppaid/1233/abc.wml");
	const TDesC& siId4 = _L("http://www.xyz.com/ppaid/1234/abc.wml");
	const TDesC& siId5 = _L("http://www.xyz.com/ppaid/1235/abc.wml");
	const TDesC& siId6 = _L("http://www.xyz.com/ppaid/1236/abc.wml");

	CMsvEntrySelection* matchingIdList = wapPushUtils->FindSiIdLC(siId1);

	// INC081489 - part 1
	// Three messages with same EHRef, EServiceIndicationID and different EServiceIndicationCreated
	TInt foundCount;
	if (matchingIdList) 
		{
		foundCount = matchingIdList->Count(); 
		}
	else
		foundCount = 0;
	
	CleanupStack::PopAndDestroy(matchingIdList
	);
	
	// INC081489 - part 2
	// Three messages with same EHRef, EServiceIndicationID and EServiceIndicationCreated
	matchingIdList = wapPushUtils->FindSiIdLC(siId2);

	if (matchingIdList) 
		{
		foundCount += matchingIdList->Count(); 
		}
	
	CleanupStack::PopAndDestroy(matchingIdList);
	
	// INC081568 - part 1
	// Same "si-id","created" and no value for the "href" attribute.
	matchingIdList = wapPushUtils->FindSiIdLC(siId3);

	if (matchingIdList) 
		{
		foundCount += matchingIdList->Count(); 
		}
	CleanupStack::PopAndDestroy(matchingIdList);
	
	//INC082189 - Two Push SI messages,the same "href"and "created",no "si-id",
	matchingIdList = wapPushUtils->FindSiIdLC(siId4);

	if (matchingIdList) 
		{
		foundCount += matchingIdList->Count(); 
		}
	CleanupStack::PopAndDestroy(matchingIdList);
	
	// INC082190: Three Push SI msgs, the same "si-id",different "created",no "href"
	matchingIdList = wapPushUtils->FindSiIdLC(siId5);

	if (matchingIdList) 
		{
		foundCount += matchingIdList->Count(); 
		}

	CleanupStack::PopAndDestroy(matchingIdList);
	
	// INC082191: Three Push SI msgs, the same "href",different "created",no "si-id"
	matchingIdList = wapPushUtils->FindSiIdLC(siId6);

	if (matchingIdList) 
		{
		foundCount += matchingIdList->Count(); 
		}
	
	TBuf<KPushLogBuffer> buf;
	_LIT(KLogSICount,"SI messages stored:\t%d");
	buf.Format(KLogSICount,foundCount);
	WPLPrintf(buf);	

	CleanupStack::PopAndDestroy(2);  // wapPushUtils, matchingIdList
	return foundCount;
	}

