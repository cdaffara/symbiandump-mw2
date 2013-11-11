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


_LIT8 ( KXMLVersion, "<xml version=\"1.0\"?>\r\n\t");
_LIT8 ( KDocType, "<!DOCTYPE sl PUBLIC \"-//WAPFORUM//DTD SL 1.0//EN\"\r\n");

// XML document contains sl tag twice.
_LIT8 (KInvalidDTD1, "<sl href=\"http://www.xyz.com/123.wml\"> </sl> <sl> </sl>");
_LIT8 (KInvalidDTD2, "<indication href=\"http://www.xyz.com/abc.wml\"></indication>");

/** set the test name for this test case
	@param void
	@return string
		returns test name
*/
const TDesC& CInvalidWAPDTDMessages::TestName()
	{
	_LIT(KTextSICMessageTest,"Invalid WAP DTD Messages");
	return KTextSICMessageTest;
	}


/** specify the number of test messages to use */
void CInvalidWAPDTDMessages::NumberTestCasesToRun()
	{
	iNumberOfTestCases = 2;
	}


/** create test messages to use where aTestCase represents the
	message index for multiple messages.
	Tests: 0 & 1- Chineese character encoding
		   2- Message with Creation and Expires time

	@param aTestCase message index value for selecting a test message
	@return void
 */
void CInvalidWAPDTDMessages::PrepareTestMessageL(TInt aTestCase)
{
	// create hardcoded SIC test message 
	const TUint8 KMessageHeader[] = 
		{
		0xAF,	// Content type "application/vnd.wap.sl"
		0x82,	// Accept-Charset  Short int - 0x02 represent x-wap-application:push.ua
		};	
	
	iHeadersBuf.Append(KMessageHeader, sizeof(KMessageHeader));
	
	HBufC8* msg = HBufC8::NewLC ( KBufferSize );
	TPtr8 ptr8 ( msg->Des() );
	ptr8.Copy ( KXMLVersion() );
	ptr8.Append ( KDocType() );

	switch ( aTestCase )
	{
	case 0 :
	ptr8.Append ( KInvalidDTD1() );
	break;
	
	case 1:	
	ptr8.Append ( KInvalidDTD2() );
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
TBool CInvalidWAPDTDMessages::ConfirmMessagesSavedL()
	{
	return ETrue;
	}
		
TInt CInvalidWAPDTDMessages::RunError(TInt /*aError*/)
	{
	return KErrNone;
	}
