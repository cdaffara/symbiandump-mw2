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

#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"
#include <push/cwappushmsgutils.h>

/** set the test name.
	@param void
	@return string
		name for test returned
*/
const TDesC& CWapPushMMMessageTest::TestName()
{
	_LIT(KTextMMMessageTest, "Multi-part Mixed Message Test");
	return KTextMMMessageTest;
}


/** set the number of test messages to pass to wap stack */
void CWapPushMMMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 1;
	}


/** create test messages to use.
	@param TInt
		number of the test message to use
	@return void
 */
void CWapPushMMMessageTest::PrepareTestMessageL(TInt /*aTestCase*/)
{
	const TUint8 KMessageHeaderMultipart[]=
		{
		0xA3,  //Content type "application/vnd.wap.multipart.mixed"
		0x94,  // Date
		0x04,  // length of field
		0x35,  // Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0xAF,  //X-Wap-Application-ID
		0x82  // Short int - 0x02 represent x-wap-application:push.ua
		};

	iHeadersBuf.Copy(KMessageHeaderMultipart, sizeof(KMessageHeaderMultipart));	

	iBodyBuf.Delete(0,KBufferSize);
	
	const TUint8 KMessageBodyMultipart[]=
		{
		0x02,	// number of parts
		0x0A,	// part 1 header size
		0x1D,	// part 1 body size
		0xB0,	// content type application/vnd.wap.slc ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11,
		0x01,	// Body - slc (www.schmookie.com)
		0x06,
		0x6A,
		0x00,
		0x85,
		0x09,
		0x03,
		0x6D,
		0x6D,
		0x6D,
		0x2E,
		0x73,
		0x63,
		0x68,
		0x6D,
		0x6F,
		0x6F,
		0x6B,
		0x69,
		0x65,
		0x2E,
		0x63,
		0x6F,
		0x64,
		0x65,
		0x2F,
		0x00,
		0x06,
		0x01,
		0x0A,	// part 2 header size
		0x1D,	// part 2 body size
		0xB0,	// content type application/vnd.wap.slc ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11,
		0x01,	// Body - slc (www.schmookie.com)
		0x06,
		0x6A,
		0x00,
		0x85,
		0x09,
		0x03,
		0x6D,
		0x6D,
		0x6D,
		0x2E,
		0x73,
		0x63,
		0x68,
		0x6D,
		0x6F,
		0x6F,
		0x6B,
		0x69,
		0x65,
		0x2E,
		0x63,
		0x6F,
		0x64,
		0x65,
		0x2F,
		0x00,
		0x06,
		0x01,
	};

	iBodyBuf.Append(KMessageBodyMultipart, sizeof(KMessageBodyMultipart));

}

/** confirm the message has been stored in the message store.
	@param void
	@return TBool
		ETrue = message found
		EFalse = message not found
*/
TBool CWapPushMMMessageTest::ConfirmMessagesSavedL()
	{
	// TODO - Quite a bit of work is required here. Creating the WapPushUtils
	// can generate a -15 (KErrServerTerminated) from the message server. So
	// care has to be taken. Suggest creating the Push Utils in the base class
	// and not deleting till the test is done. Also, the other thing that could
	// be changed is that the base class sets the priority of the active object
	// down to Idle when in state EFinished. Suggest renaming EFinished to
	// ETestsDone and adding EConfirmMessageSaved and EComplete states.
	
	//	CWapPushMsgUtils* wapPushUtils= CWapPushMsgUtils::NewL();
	//	CleanupStack::PushL(wapPushUtils);
	//	TO DO: check multiparts in store by looking up..???
	//	TMsvId msgEntryId = wapPushUtils->FindUrlL(KSISiId(), KUidWapPushMsgSI);
	
	//	CleanupStack::PopAndDestroy();  // wapPushUtils
	return EFalse/*msgEntryId*/;
	}


/** set the test name.
	@param void
	@return string
		name for test returned
*/
const TDesC& CWapPushMAMessageTest::TestName()
{
	_LIT(KTextMAMessageTest, "Multi-part Alternative Message Test");
	return KTextMAMessageTest;
}


/** set the number of test messages to pass to wap stack */
void CWapPushMAMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 1;
	}


/** create test messages to use.
	@param TInt
		number of the test message to use
	@return void
 */
void CWapPushMAMessageTest::PrepareTestMessageL(TInt /*aTestCase*/)
	{
	const TUint8 KMessageHeaderMultipart[]=
		{
		0xA6,  //Content type "application/vnd.wap.multipart.alternative"
		0x94,  // Date
		0x04,  // length of field
		0x35,  // Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0xAF,  //X-Wap-Application-ID
		0x82  // Short int - 0x02 represent x-wap-application:push.ua
		};

	
	iHeadersBuf.Copy(KMessageHeaderMultipart, sizeof(KMessageHeaderMultipart));	
	iBodyBuf.Delete(0,KBufferSize);
	
	const TUint8 KMessageBodyMultipart[]=
		{
		0x02,	// number of parts
		0x0A,	// part 1 header size
		0x1D,	// part 1 body size
		0xB0,	// content type application/vnd.wap.slc ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11,
		0x01,	// Body - slc (www.schmookie.com)
		0x06,
		0x6A,
		0x00,
		0x85,
		0x09,
		0x03,
		0x6D,
		0x6D,
		0x6D,
		0x2E,
		0x73,
		0x63,
		0x68,
		0x6D,
		0x6F,
		0x6F,
		0x6B,
		0x69,
		0x65,
		0x2E,
		0x63,
		0x6F,
		0x64,
		0x65,
		0x2F,
		0x00,
		0x06,
		0x01,
		0x0A,	// part 2 header size
		0x1D,	// part 2 body size
		0xB0,	// content type application/vnd.wap.slc ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11,
		0x01,	// Body - slc (www.schmookie.com)
		0x06,
		0x6A,
		0x00,
		0x85,
		0x09,
		0x03,
		0x6D,
		0x6D,
		0x6D,
		0x2E,
		0x73,
		0x63,
		0x68,
		0x6D,
		0x6F,
		0x6F,
		0x6B,
		0x69,
		0x65,
		0x2E,
		0x63,
		0x6F,
		0x64,
		0x65,
		0x2F,
		0x00,
		0x06,
		0x01,
	};

	iBodyBuf.Append(KMessageBodyMultipart, sizeof(KMessageBodyMultipart));

}

/** confirm the message has been stored in the message store.
	@param void
	@return TBool
		ETrue = message found
		EFalse = message not found
*/
TBool CWapPushMAMessageTest::ConfirmMessagesSavedL()
	{
	// TODO - Quite a bit of work is required here. Creating the WapPushUtils
	// can generate a -15 (KErrServerTerminated) from the message server. So
	// care has to be taken. Suggest creating the Push Utils in the base class
	// and not deleting till the test is done. Also, the other thing that could
	// be changed is that the base class sets the priority of the active object
	// down to Idle when in state EFinished. Suggest renaming EFinished to
	// ETestsDone and adding EConfirmMessageSaved and EComplete states.
	
	//	CWapPushMsgUtils* wapPushUtils= CWapPushMsgUtils::NewL();
	//	CleanupStack::PushL(wapPushUtils);
	//	TO DO: check multiparts in store by looking up..???
	//	TMsvId msgEntryId = wapPushUtils->FindUrlL(KSISiId(), KUidWapPushMsgSI);
	
	//	CleanupStack::PopAndDestroy();  // wapPushUtils
	return EFalse/*msgEntryId*/;
	}

/** set the test name.
	@param void
	@return string
		name for test returned
*/
const TDesC& CWapPushMRMessageTest::TestName()
{
	_LIT(KTextMRMessageTest, "Multi-part Related Message Test");
	return KTextMRMessageTest;
}


/** set the number of test messages to pass to wap stack */
void CWapPushMRMessageTest::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 1;
	}


/** create test messages to use.
	@param TInt
		number of the test message to use
	@return void
 */
void CWapPushMRMessageTest::PrepareTestMessageL(TInt /*aTestCase*/)
{
	const TUint8 KMessageHeaderMultipart[]=
		{
		0xB3,  //Content type "application/vnd.wap.multipart.related"
		0x94,  // Date
		0x04,  // length of field
		0x35,  // Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0xAF,  //X-Wap-Application-ID
		0x82  // Short int - 0x02 represent x-wap-application:push.ua
		};


	iHeadersBuf.Copy(KMessageHeaderMultipart, sizeof(KMessageHeaderMultipart));	
	iBodyBuf.Delete(0,KBufferSize);
	
	const TUint8 KMessageBodyMultipart[]=
		{
		0x02,	// number of parts
		0x0A,	// part 1 header size
		0x1D,	// part 1 body size
		0xB0,	// content type application/vnd.wap.slc ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11,
		0x01,	// Body - slc (www.schmookie.com)
		0x06,
		0x6A,
		0x00,
		0x85,
		0x09,
		0x03,
		0x6D,
		0x6D,
		0x6D,
		0x2E,
		0x73,
		0x63,
		0x68,
		0x6D,
		0x6F,
		0x6F,
		0x6B,
		0x69,
		0x65,
		0x2E,
		0x63,
		0x6F,
		0x64,
		0x65,
		0x2F,
		0x00,
		0x06,
		0x01,
		0x0A,	// part 2 header size
		0x1D,	// part 2 body size
		0xB0,	// content type application/vnd.wap.slc ... the headers:
		0x94,	// Date
		0x04,	// length of field
		0x35,	// Thur 23 Apr 1998 13:41:37 GMT
		0x3F,	
		0x45,
		0x11,
		0x3F,	
		0x45,
		0x11,
		0x01,	// Body - slc (www.schmookie.com)
		0x06,
		0x6A,
		0x00,
		0x85,
		0x09,
		0x03,
		0x6D,
		0x6D,
		0x6D,
		0x2E,
		0x73,
		0x63,
		0x68,
		0x6D,
		0x6F,
		0x6F,
		0x6B,
		0x69,
		0x65,
		0x2E,
		0x63,
		0x6F,
		0x64,
		0x65,
		0x2F,
		0x00,
		0x06,
		0x01,
	};

	iBodyBuf.Append(KMessageBodyMultipart, sizeof(KMessageBodyMultipart));

}

/** confirm the message has been stored in the message store.
	@param void
	@return TBool
		ETrue = message found
		EFalse = message not found
*/
TBool CWapPushMRMessageTest::ConfirmMessagesSavedL()
	{
	// TODO - Quite a bit of work is required here. Creating the WapPushUtils
	// can generate a -15 (KErrServerTerminated) from the message server. So
	// care has to be taken. Suggest creating the Push Utils in the base class
	// and not deleting till the test is done. Also, the other thing that could
	// be changed is that the base class sets the priority of the active object
	// down to Idle when in state EFinished. Suggest renaming EFinished to
	// ETestsDone and adding EConfirmMessageSaved and EComplete states.
	
	//	CWapPushMsgUtils* wapPushUtils= CWapPushMsgUtils::NewL();
	//	CleanupStack::PushL(wapPushUtils);
	//	TO DO: check multiparts in store by looking up..???
	//	TMsvId msgEntryId = wapPushUtils->FindUrlL(KSISiId(), KUidWapPushMsgSI);
	
	//	CleanupStack::PopAndDestroy();  // wapPushUtils
	return EFalse/*msgEntryId*/;
	}
