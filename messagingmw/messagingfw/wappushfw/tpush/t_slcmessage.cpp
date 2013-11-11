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


//Hard coded SLC Text Msg elements
//for 1st SL test Msg 
_LIT8(KSLCHref, "rtsp://media.example.com/123");

/** set the test name for this test case
	@param void
	@return string
		returns test name
*/
const TDesC& CWapPushSLCMessageTest::TestName()
	{
	_LIT(KTextSLCMessageTest,"Service Loading Content");
	return KTextSLCMessageTest;
	}


/** specify the number of test messages to use */
void CWapPushSLCMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 1;
	}


/** create test messages to use where aTestCase represents the
	message index for multiple messages.
	Tests: 0- Complete SL Push Msg 
		   1- SL Push Msg with attribute 'action' not specified.
		   2- SL Push Msg with attribute 'href' not specified.

	@param aTestCase message index value for selecting a test message
	@return void
 */
void CWapPushSLCMessageTest::PrepareTestMessageL(TInt aTestCase)
{
	// create hardcoded SLC test message 
	const TUint8 KMessageHeader[] = 
		{
		0xB0  //Content type "application/vnd.wap.slc"
		};	
	
	iHeadersBuf.Append(KMessageHeader, sizeof(KMessageHeader));

	HBufC8* msg = NULL;
	switch ( aTestCase )
	{
	case 0 :
	_LIT8 ( KMessage1, "\x02\x06\x6A\x00\x85\x08\x03\x72\x74\x73\x70\x3A\x2F\x2F\x6D\x65\x64\x69\x61\x2E\x65\x78\x61\x6D\x70\x6C\x65\x2E\x63\x6F\x6D\x2F\x31\x32\x33\x00\x06\x01" );
	msg = KMessage1().AllocLC ();
	break;	
	}

	// copy data to iBodyBuf
	iBodyBuf.Delete(0,KBufferSize);
	iBodyBuf = msg->Des();

	CleanupStack::PopAndDestroy(1); // msg
}

/** confirm the message has been stored in the message store.
	Writes confirmation of number of found messages to log file
	if present.
	@param void
	@return TBool
		ETrue = message(s) found
		EFalse = no message(s) found
 */
TBool CWapPushSLCMessageTest::ConfirmMessagesSavedL()
	{
	CSISLPushMsgUtils* wapPushUtils = CSISLPushMsgUtils::NewL();
	CleanupStack::PushL(wapPushUtils);
	TMsvId slMsgEntryId;
	// convert KSLHref from 8 to 16 bit for FindUrlL
	HBufC* tempHref = HBufC::NewLC(KSLCHref().Length());
	tempHref->Des().Copy(KSLCHref());

	slMsgEntryId = wapPushUtils->FindUrlL(*tempHref, KUidWapPushMsgSL);
	if (slMsgEntryId)
		{
		TBuf<KPushLogBuffer> buf;
		_LIT(KLogSLId,"SLC messages stored:\t%d");
		buf.Format(KLogSLId,slMsgEntryId);
		WPLPrintf(buf);	
		}
	CleanupStack::PopAndDestroy(2);  // wapPushUtils, tempHref 
	return slMsgEntryId;
	}
