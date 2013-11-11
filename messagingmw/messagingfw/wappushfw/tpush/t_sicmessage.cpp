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
#include <txtrich.h>
#include <msvids.h>
#include <push/csipushmsgentry.h>
#include <escapeutils.h>

//Hard coded SIC Text Msg elements
_LIT(KSICHref, "http://wap.com.com" );
_LIT(KSICHref2, "www.com.com");
_LIT(KSICHref3, "http://www.xyz.com/email/123/abc.wml");
_LIT8( KSICText, "\xE2\x82\xAC\xE4\xBD\xA0\xE5\xA5\xBD\xEF\xBC\x81\x20\xE5\xB7\xA5\xE4\xBD\x9C\xE9\xA1\xBA\xE5\x88\xA9\x20\x48\x61\x76\x65\x20\x61\x20\x6E\x69\x63\x65\x20\x64\x61\x79" );
_LIT8(KSICText2, "You have 10 new e-mail's");

// In TDateTime, month and day's offset is 0. So minus 1 from day and month for cross checking.
_LIT(KCreatedTime, "20060524:152315");
_LIT(KExpiresTime, "20080529:000000");

/** set the test name for this test case
	@param void
	@return string
		returns test name
*/
const TDesC& CWapPushSICMessageTest::TestName()
	{
	_LIT(KTextSICMessageTest,"Service Indication Content");
	return KTextSICMessageTest;
	}


/** specify the number of test messages to use */
void CWapPushSICMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 5;
	}


/** create test messages to use where aTestCase represents the
	message index for multiple messages.
	Tests: 0 & 1- Chineese character encoding
		   2- Message with Creation and Expires time

	@param aTestCase message index value for selecting a test message
	@return void
 */
void CWapPushSICMessageTest::PrepareTestMessageL(TInt aTestCase)
{
	// create hardcoded SIC test message 
	const TUint8 KMessageHeader[] = 
		{
		0x03,	// Content type text/plain
		0xAE,	// Content type "application/vnd.wap.sic"
		0x81,	// Accept-Charset  Short int - 0x02 represent x-wap-application:push.ua
		0xEA	// UTF-8 
		};	
	
	iHeadersBuf.Append(KMessageHeader, sizeof(KMessageHeader));
	
	HBufC8* msg = NULL;
	switch ( aTestCase )
	{
	case 0 :
	_LIT8 ( KMessage1, "\x02\x05\x6A\x00\x45\xC6\x0C\x03\x77\x61\x70\x2E\x63\x6F\x6D\x2E\x63\x6F\x6D\x00\x07\x01\x03\xE2\x82\xAC\xE4\xBD\xA0\xE5\xA5\xBD\xEF\xBC\x81\x20\xE5\xB7\xA5\xE4\xBD\x9C\xE9\xA1\xBA\xE5\x88\xA9\x20\x48\x61\x76\x65\x20\x61\x20\x6E\x69\x63\x65\x20\x64\x61\x79\x00\x01\x01" );
	msg = KMessage1().AllocLC ();
	break;
	
	case 1:	
	_LIT8 ( KMessage2, "\x02\x05\x6A\x00\x45\xC6\x0B\x03\x77\x77\x77\x2E\x63\x6F\x6D\x2E\x63\x6F\x6D\x00\x01\x03\xE2\x82\xAC\xE4\xBD\xA0\xE5\xA5\xBD\xEF\xBC\x81\x20\xE5\xB7\xA5\xE4\xBD\x9C\xE9\xA1\xBA\xE5\x88\xA9\x20\x48\x61\x76\x65\x20\x61\x20\x6E\x69\x63\x65\x20\x64\x61\x79\x00\x01\x01");
	msg = KMessage2().AllocLC ();
	break;
	
	case 2:
	/** Original message in XML format 	
	<?xml version="1.0"?>
	<!DOCTYPE si PUBLIC "-//WAPFORUM//DTD SI 1.0//EN"
	"http://www.wapforum.org/DTD/si.dtd">
	<si>
		<indication href=”http://www.xyz.com/email/123/abc.wml”
		created=”2006-06-25T15.23.15Z”
		si-expires=”2008-06-30T00.00.00Z”>
		You have 4 new e-mail's
		</indication>
	</si>
	*/
	_LIT8 ( KMessage3, "\x00\x05\x6A\x00\x45\xC6\x0D\x03\x78\x79\x7A\x00\x85\x03\x65\x6D\x61\x69\x6C\x2F\x31\x32\x33\x2F\x61\x62\x63\x2E\x77\x6D\x6C\x00\x0A\xC3\x07\x20\x06\x06\x25\x15\x23\x15\x10\xC3\x04\x20\x08\x06\x30\x01\x03\x59\x6F\x75\x20\x68\x61\x76\x65\x20\x31\x30\x20\x6E\x65\x77\x20\x65\x2D\x6D\x61\x69\x6C\x27\x73\x00\x01\x01" );
	msg = KMessage3().AllocLC ();
	break;

	case 3:
	/** Invalid Date
	 */
	_LIT8 ( KMessage4, "\x00\x05\x6A\x00\x45\xC6\x0D\x03\x78\x79\x7A\x00\x85\x03\x65\x6D\x61\x69\x6C\x2F\x31\x32\x33\x2F\x61\x62\x63\x2E\x77\x6D\x6C\x00\x0A\xC3\x07\x20\x08\x02\x30\x15\x23\x15\x10\xC3\x04\x20\x08\x06\x30\x01\x03\x59\x6F\x75\x20\x68\x61\x76\x65\x20\x31\x30\x20\x6E\x65\x77\x20\x65\x2D\x6D\x61\x69\x6C\x27\x73\x00\x01\x01" );
	msg = KMessage4().AllocLC ();
	break;
	
	case 4:
	/** Invalid Time
	 */
	_LIT8 ( KMessage5, "\x00\x05\x6A\x00\x45\xC6\x0D\x03\x78\x79\x7A\x00\x85\x03\x65\x6D\x61\x69\x6C\x2F\x31\x32\x33\x2F\x61\x62\x63\x2E\x77\x6D\x6C\x00\x0A\xC3\x07\x20\x06\x02\x28\x15\x60\x15\x10\xC3\x04\x20\x08\x06\x30\x01\x03\x59\x6F\x75\x20\x68\x61\x76\x65\x20\x31\x30\x20\x6E\x65\x77\x20\x65\x2D\x6D\x61\x69\x6C\x27\x73\x00\x01\x01" );
	msg = KMessage5().AllocLC ();
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
TBool CWapPushSICMessageTest::ConfirmMessagesSavedL()
	{
	TestMessageL ( KSICHref(), KSICText() );
	TestMessageL ( KSICHref2(), KSICText() );
	return TestMessageL ( KSICHref3(), KSICText2() );
	}

TBool CWapPushSICMessageTest::TestMessageL ( const TDesC& aUri, const TDesC8& aText )
	{
	CSISLPushMsgUtils* wapPushUtils = CSISLPushMsgUtils::NewL();
	CleanupStack::PushL(wapPushUtils);
	TMsvId slMsgEntryId;

	HBufC* tempHref = aUri.AllocLC();

	slMsgEntryId = wapPushUtils->FindUrlL(*tempHref, KUidWapPushMsgSI);
	if (slMsgEntryId)
		{
		TBuf<KPushLogBuffer> buf;
		_LIT(KLogSLId,"SIC messages stored:\t%d");
		buf.Format(KLogSLId,slMsgEntryId);
		WPLPrintf(buf);	

 		CSIPushMsgEntry* siLoad = CSIPushMsgEntry::NewL();
 		CleanupStack::PushL(siLoad);  
	    siLoad->RetrieveL(wapPushUtils->Session(), slMsgEntryId);
	    const TDesC& text = siLoad->Text();	    		
		HBufC* des16 = EscapeUtils::ConvertToUnicodeFromUtf8L ( aText );
		CleanupStack::PushL ( des16 );
		if ( des16->CompareF ( text ) )
			{
			User::Leave ( KErrCorrupt );
			}
		
		// Check for Created time & expiration time. ( Only for the KSICHref3 )		
		if ( KSICHref3().CompareF ( siLoad->Url() ) == 0 )
			{
			const TTime& createdTime = siLoad->Created();
			TTime checkCreatedTime;
			checkCreatedTime.Set ( KCreatedTime() );						
			if ( createdTime != checkCreatedTime )
				{
				User::Leave ( KErrCorrupt );
				}
			
			// Check for expiration time
			const TTime& expiresTime = siLoad->Expires ();
			TTime checkExpiresTime;
			checkExpiresTime.Set ( KExpiresTime() );
			if ( expiresTime != checkExpiresTime )
				{
				User::Leave ( KErrCorrupt );
				}
			}
		
	    CleanupStack::PopAndDestroy(2); // des16, siLoad
		}
	CleanupStack::PopAndDestroy(2);  // wapPushUtils, tempHref 
	return slMsgEntryId;	
	}
