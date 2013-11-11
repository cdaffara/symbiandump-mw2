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

//_LIT(KTextDoingStuff,"Doing some stuff");
//_LIT(KTextFinished,"All finished");

//Hard coded SL Text Msg elements
//for 1st SL test Msg 
_LIT8(KSLHref, "http://www.xyz.com/ppaid/123/abc.wml");
_LIT8(KSLAction, "execute-high");

//for 2nd SL test Msg-
_LIT8(KSLHref_1, "http://www.xyz.com/ppaid/100/aaa.wml");

/** set the test name for this test case
	@param void
	@return string
		returns test name
*/
const TDesC& CWapPushSLMessageTest::TestName()
	{
	_LIT(KTextSLMessageTest,"Service Loading");
	return KTextSLMessageTest;
	}


/** specify the number of test messages to use */
void CWapPushSLMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 3;
	}


/** create test messages to use where aTestCase represents the
	message index for multiple messages.
	Tests: 0- Complete SL Push Msg 
		   1- SL Push Msg with attribute 'action' not specified.
		   2- SL Push Msg with attribute 'href' not specified.

	@param aTestCase message index value for selecting a test message
	@return void
 */
void CWapPushSLMessageTest::PrepareTestMessageL(TInt aTestCase)
{
	// create hardcoded SL test message (Julia's code)
	const TUint8 KMessageHeader[] = 
		{
		//0xB0,  //Content type "application/vnd.wap.slc"
		0xAF,  //Content type "application/vnd.wap.sl" 
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
	
//	const TUint8 aChar[]={0x0a}; 

	iHeadersBuf.Append(KMessageHeader, sizeof(KMessageHeader));

	CWapPushMsgGen* msgGen = CWapPushMsgGen::NewL();
	CleanupStack::PushL(msgGen);
	msgGen->StartNewMsgL(CWapPushMsgGen::EServiceLoad);
		
	switch (aTestCase)
		{
	case 0 : //
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSLHref);
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSLAction);
		break;
	case 1 :// Attribute  'action' Not specified.
			// the value 'execute-low' should be used
		msgGen->AppendFieldL(CWapPushMsgGen::EHRef, KSLHref_1);
		break;
	case 2 :// Attribute  'href' Not specified.
			// the SL Content handler should leave with KErrCorrupt
		msgGen->AppendFieldL(CWapPushMsgGen::EAction, KSLAction);
		break;
//	case 3 :// only one character in Msg Body
//		iHeadersBuf.Append(aChar, sizeof(aChar));
//		break;

	default:
		// do nothing
		break;
		}

	HBufC8* msg = msgGen->CreateMsgLC();

	// copy data to iBodyBuf
	iBodyBuf.Delete(0,KBufferSize);
	iBodyBuf = msg->Des();

	CleanupStack::PopAndDestroy(2); // msg, msgGen;
}

/** confirm the message has been stored in the message store.
	Writes confirmation of number of found messages to log file
	if present.
	@param void
	@return TBool
		ETrue = message(s) found
		EFalse = no message(s) found
 */
TBool CWapPushSLMessageTest::ConfirmMessagesSavedL()
	{
	CSISLPushMsgUtils* wapPushUtils = CSISLPushMsgUtils::NewL();
	CleanupStack::PushL(wapPushUtils);
	TMsvId slMsgEntryId;
	// convert KSLHref from 8 to 16 bit for FindUrlL
	HBufC* tempHref = HBufC::NewLC(KSLHref().Length());
	tempHref->Des().Copy(KSLHref());

	slMsgEntryId = wapPushUtils->FindUrlL(*tempHref, KUidWapPushMsgSL);
	if (slMsgEntryId)
		{
		TBuf<KPushLogBuffer> buf;
		_LIT(KLogSLId,"SL messages stored:\t%d");
		buf.Format(KLogSLId,slMsgEntryId);
		WPLPrintf(buf);	
		}
	CleanupStack::PopAndDestroy(2);  // wapPushUtils, tempHref 
	return slMsgEntryId;
	}
