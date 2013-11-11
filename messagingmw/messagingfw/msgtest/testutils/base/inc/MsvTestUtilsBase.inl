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
// TestUtils.INL
//

RTest& CTestUtils::Test() const
	{
	return iRTest;
	}

void CTestUtils::operator()(TInt aResult,TInt aLineNum)
	{
	iRTest(aResult, aLineNum);
	}

void CTestUtils::operator()(TInt aResult)
	{
	iRTest(aResult);
	}

RFs& CTestUtils::FileSession()
	{
	return iFs;
	}
