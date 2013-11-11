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

#include "pushtests.h"
#include "dummywapstack.h"
#include "wappushmsggen.h"

#include "httptestutils.h"
#if !defined(__TESTSCRIPTS_H__)
#include "TestScripts.h"
#endif


_LIT(KWapIniFolder, 	"wapini");
_LIT(KWapIni, 			"wap.ini");

_LIT(KServerAddr,	"ServerAddr");
_LIT(KHRef,			"HRef");
_LIT(KAction,		"Action");

const TUint KDoubleQuote ('"');

CWapPushMessageTest::CWapPushMessageTest(TInt aIndex, CHTTPTestUtils* aTestUtils)
	{
	iSectionIndex = aIndex;
	iTestUtils = aTestUtils;
	}
	
CWapPushMessageTest::~CWapPushMessageTest()
	{
	delete iUri;
	delete iHRef;
	delete iAction;
	}
	
void CWapPushMessageTest::RemoveQuotes(HBufC8* aHBufPtr) 
	{
	if ((( *aHBufPtr )[0]) == KDoubleQuote)
		{
		( aHBufPtr->Des() ).Delete(0,1);
		}	
		
	TInt lastChar = ((aHBufPtr->Des()).Length());
	--lastChar;
	
	if (( *aHBufPtr )[lastChar] == KDoubleQuote)
		{
		( aHBufPtr->Des() ).Delete(lastChar,1);
		}
		
	}	
	
TDesC8& CWapPushMessageTest::GetHRef() const
	{
	return (*(iHRef));	
	}
	
TDesC8& CWapPushMessageTest::GetAction() const
	{
	return(*(iAction));	
	}
	
void CWapPushMessageTest::ReadAndSetParamsFromIniFileL()
	{
	// Delete previously read buffers.
	delete iUri;
	delete iHRef;
	delete iAction;
			
	CScriptFile* iniFile = NULL;
	iniFile = CScriptFile::NewL(*iTestUtils, KWapIniFolder(), KWapIni());
	CleanupStack::PushL(iniFile); 
		
	TInt itemFieldCount= iniFile->Section(iSectionIndex).Items().Count();
	
	TBuf8<200> itemFieldValue8;
	
	for (TInt fctr=0; fctr < itemFieldCount ; ++fctr)
		{
		TPtrC itemFieldPtr(iniFile->Section(iSectionIndex).Item(fctr).Item());
		itemFieldValue8.Copy(iniFile->Section(iSectionIndex).Item(fctr).Value());
		
		if(itemFieldPtr.Compare(KServerAddr) == 0)
			{
			iUri = itemFieldValue8.AllocL();
			RemoveQuotes(iUri);
			}
		else if(itemFieldPtr.Compare(KHRef) == 0)
			{
			iHRef = itemFieldValue8.AllocL();
			RemoveQuotes(iHRef);	
			}
		else if(itemFieldPtr.Compare(KAction) == 0)
			{
			iAction = itemFieldValue8.AllocL();
			RemoveQuotes(iAction);
			}
		}
	CleanupStack::PopAndDestroy(iniFile);	
	}





	
	



