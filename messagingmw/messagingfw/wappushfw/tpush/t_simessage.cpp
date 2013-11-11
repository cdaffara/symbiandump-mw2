// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// si text message data

_LIT8(KSIAction,"delete");
_LIT8(KSIHref, "http://www.xyz.com/ppaid/123/abc.wml");
_LIT8(KSINewHref, "http://www.xyz.com/ppaid/123/new.wml");
_LIT8(KSIOldDate,		"2000-01-01T15:23:15Z");
_LIT8(KSeptemberDate,	"2000-09-30T00:00:00Z");
_LIT8(KOctoberDate,		"2000-10-30T00:00:00Z");
_LIT8(KSISiId, "http://www.xyz.com/ppaid/123/abc.wml");

/** set test name to 'Service Indication'
	@param void
	@return string
		string represents test name
*/
const TDesC& CWapPushSIMessageTest::TestName()
{
	_LIT(KTextSIMessageTest, "Service Indication");
	return KTextSIMessageTest;
}

/** set the number of test messages to use */
void CWapPushSIMessageTest::NumberTestCasesToRun()
{
	iNumberOfTestCases = 8;
}

/** build different test messages to test specific SI reception aspects
	in the SI content handler processing code.
	@param TInt
		the number of test messages to create and use
	@return void
*/
void CWapPushSIMessageTest::PrepareTestMessageL(TInt aTestCase)
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
	case 0 : // no action - msg stored with action = "signal-medium"
			// this initial msg will match in cases 2 and 3...
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KSeptemberDate);
		break;
	case 1 : // message expired - delete
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationExpires, KSIOldDate);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, _L8("do not store this message"));
		break;	
	case 2 : // match existing si - delete received si 
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KSIOldDate);
		break;
	case 3 : // match existing si - delete existing si
			 // deletes msg created in case 0 and will replace it
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KOctoberDate); // date > stored date
		break;
	case 4 : // match existing si and discard receved message
		     // matches msg from case 3 
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSINewHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KOctoberDate);	// date = stored date
		break;
	case 5 : // action = delete; href set but no si-id (no deletion occurs)
			 // store msg but has si-id equal to received msg href.... 
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSIAction);
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSIHref);
		break;
	case 6 : // action = delete; si-id set (delete msg)
			// any matching record will already have been deleted
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSIAction);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationID, KSISiId);
		msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationCreated, KOctoberDate);
		break;
	case 7 :
		// testing fix for INC038455 - Apostrophe char (0x27) in SI message makes CXmlLibrary leave with -13106
		PrepareApostropheTestMessageL();
		break;
	default: 
		// do nothing
		break;
	}
	msgGen->AppendFieldL(CWapPushMsgGen::EServiceIndicationText, _L8("You have 4 new e-mails"));

	HBufC8* msg = msgGen->CreateMsgLC();
	// copy data to iBodyBuf
	if (aTestCase!=7)
		{
		iBodyBuf = msg->Des(); 
		}
	CleanupStack::PopAndDestroy(2); //msgGen, msg
}

/** confirm the message has been stored.
	Writes the number of stored messages found to the log file - if present.
	@param void
	@return TBool
		returns ETrue if messages found
		returns EFalse if no messages found 
*/
TBool CWapPushSIMessageTest::ConfirmMessagesSavedL()
	{
	CSISLPushMsgUtils* wapPushUtils = CSISLPushMsgUtils::NewL();
	CleanupStack::PushL(wapPushUtils);
	
	TMsvId localFolderId;
	wapPushUtils->GetPushMsgFolderIdL(localFolderId);
	
	const TDesC& siId16 = _L("http://www.xyz.com/ppaid/123/abc.wml"); 
	CMsvEntrySelection* matchingIdList = wapPushUtils->FindSiIdLC(siId16);

	// check there are any entries - zero indicates no matches
	TInt foundCount;
	if (matchingIdList) 
		foundCount = matchingIdList->Count(); 
	else
		foundCount = 0;
	
	TBuf<KPushLogBuffer> buf;
	_LIT(KLogSICount,"SI messages stored:\t%d");
	buf.Format(KLogSICount,foundCount);
	WPLPrintf(buf);	

	CleanupStack::PopAndDestroy(2);  // wapPushUtils, matchingIdList
	return foundCount;
	}

void CWapPushSIMessageTest::PrepareApostropheTestMessageL()
	{
	const TUint8 KMessageHeader2[] = 
		{
		0xAE,  //Content type "application/vnd.wap.si" 
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

	const TUint8 KBody[] = 
		{
		0x02,
		0x05,
		0x6A,
		0x00,
		0x45,
		0xC6,
		0x0C, 
		0x03,
		0x77,
		0x61,
		0x70,
		0x2E,
		0x67,
		0x6F,
		0x6F,
		0x67,
		0x6C,
		0x65,
		0x2E,
		0x63,
		0x6F,
		0x6D,
		0x00,
		0x01,
		0x03,
		0xA0,
		0x49,
		0x74,
		0x27, //
//		0x22, //
		0x73,
		0xA0,
		0x46,
		0xA0,
		0x46,
		0xA0,
		0x46,
		0xA0,
		0x46,
		0x72,
		0x69,
		0x64,
		0x61,
		0x79,
		0x21,
		0x21,
		0x21,
		0xA0,
		0x00,
		0x01,
		0x01
		};	


	TBuf8<60> tempBuf;
	TUint8 value;
	for (TInt i = 0; i < 15; i++)
		{
		value = KMessageHeader2[i];
		tempBuf.Append(value);
		}

	iHeadersBuf.Copy(tempBuf);
	tempBuf.Copy(_L8(""));

	iBodyBuf.Delete(0,KBufferSize);
		
	for (TInt ii = 0; ii < 50; ii++)
		{
		value = KBody[ii];
		tempBuf.Append(value);
		}

	// copy data to iBodyBuf
	iBodyBuf.Copy(tempBuf); 
	tempBuf.Copy(_L8(""));
	}


