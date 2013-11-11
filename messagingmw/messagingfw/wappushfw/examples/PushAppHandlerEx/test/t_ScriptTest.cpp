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
#include "WapPushMsgGen.h"
#include "WPushUtils.h"
#include "testscripts.h"

// Location of INI file that defines the test cases to be run
_LIT(KAcceptanceTestCompName, "acceptance");
_LIT(KAcceptanceTestScriptName, "pushtestcases.ini");


// Labels for section names and resources in the INI file
_LIT(KSectionMMSheader, "MMSheader");
_LIT(KSectionMMSbody, "MMSbody");

CWapScriptTest::~CWapScriptTest()
	{
	delete iIniFile;
	}

const TDesC& CWapScriptTest::TestName()
	{
	_LIT(KTextNotifMessageTest,"Script Test");
	return KTextNotifMessageTest;
	}

/** specify the number of test messages to use */
void CWapScriptTest::NumberTestCasesToRunL()
	{
	_LIT(KErrScriptObject, "CScriptFile Object not created succesfully");
	TRAPD(err, iIniFile = CScriptFile::NewL(KAcceptanceTestCompName));
	if (err != KErrNone)
		WPLPrintf(KErrScriptObject);

	// Read from the script
	_LIT(KFileNotFound, "Script File Not Found");
	TInt error = iIniFile->ReadScriptL(KAcceptanceTestScriptName);

	if (error != KErrNone)
		WPLPrintf(KFileNotFound);

 	// Get the number of TestCases from section[0]
	_LIT(KNumOfTestCasesItem, "numOfTestCases");
	TInt testCases = iIniFile->Section(0).ItemValue(KNumOfTestCasesItem,0);
	iNumberOfTestCases = testCases;
	}

void CWapScriptTest::PrepareTestMessageL(TInt aTestCase) // aTestCase = 0,1,2 etc
{
	iHeadersBuf.Zero();
	iBodyBuf.Zero();

	// Position in the sections array for the testcase: aTestCase
	TInt aTestCaseSecPos = aTestCase + 1; // add 1 to get the index position of the test case section to be tested

	// MMSHEADER: Get the header name for the TestCase at position aTestCaseSecPos
	TPtrC hdrName = iIniFile->Section(aTestCaseSecPos).Item(0).Value();

	// Get the header section with the above named header 'hdrName'
 	CScriptSection* headerSection = iIniFile->Section(hdrName);

	_LIT(KHdrSectionNotFound, "The MMS Header is not found in the script file");
	if (!headerSection)
		{
		WPLPrintf(KHdrSectionNotFound);
		}
		// MMSBODY: Get the body name for the TestCase at position aTestCaseSecPos
	TPtrC bodyName = iIniFile->Section(aTestCaseSecPos).Item(1).Value();

	// Get the body section with the above named header 'body'
	_LIT(KBodySectionNotFound, "The MMS Body is not found in the script file");
	CScriptSection* bodySection = iIniFile->Section(bodyName);
	if (!bodySection)
		{
		WPLPrintf(KBodySectionNotFound);
		}

	// Append the header to iHeadersBuf
	_LIT(KHdrCodeLex, "To Initialize TLex Object");
	TLex HdrCodeLex(KHdrCodeLex);
	TInt itemCount;
	itemCount = headerSection->Items().Count();
	TInt i;
	for (i = 0; i<itemCount; ++i)
		{
		TPtrC name = headerSection->Item(i).Value();
		// Check if the contents is a string then don't convert to integer
		if(name[0] >= '0' && name[0] <='9')
			{
			HdrCodeLex.Assign(name);
			TUint nameInt;
			HdrCodeLex.Val(nameInt);
			iHeadersBuf.Append(nameInt);
			}
		else
			iHeadersBuf.Append(name);
		}

	// Append the body to iBodyBuf
	itemCount = bodySection->Items().Count();
	for (TInt ii = 0; ii<itemCount; ++ii)
		{
		TPtrC name = bodySection->Item(ii).Value();
		// Check if the contents is a string then don't convert to integer
		if(name[0] >= '0' && name[0] <='9')
			{
			HdrCodeLex.Assign(name);
			TUint nameInt;
			HdrCodeLex.Val(nameInt);
			iBodyBuf.Append(nameInt);
			}
		else
			iBodyBuf.Append(name);
		}

	}

TBool CWapScriptTest::ConfirmMessagesSavedL()
	{
	return ETrue;
	}
