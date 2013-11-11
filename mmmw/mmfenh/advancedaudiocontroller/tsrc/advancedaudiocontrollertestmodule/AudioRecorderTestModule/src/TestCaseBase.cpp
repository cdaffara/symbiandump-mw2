/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  AudioRecorder Test
*
*/


#include "TestCaseBase.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTestCaseBase::CTestCaseBase()
{

}

CTestCaseBase::~CTestCaseBase()
{

}

TInt CTestCaseBase::ParseFileName(CStifItemParser* item, TPtrC& name, TTimeIntervalMicroSeconds& duration)
{
	TInt sec=0, mil=0, mic=0;

	if (item->GetString(KEmptyString,name) != KErrNone) return KErrSyntax;
	item->GetInt(name,sec);
	item->GetNextInt(mil);
	item->GetNextInt(mic);

	duration = TInt64((TInt32)(sec*1000000 + mil*1000 + mic));

	return KErrNone;
}

TInt64 CTestCaseBase::ParseTime(CStifItemParser* item)
{
	TInt sec=0, mil=0, mic=0;

	item->GetInt(KEmptyString,sec);
	item->GetNextInt(mil);
	item->GetNextInt(mic);

	return TInt64((TInt32)(sec*1000000 + mil*1000 + mic));
}

void CTestCaseBase::LogTime(TPtrC str, TTimeIntervalMicroSeconds time)
{
	if (logger)
	{
		if (time.Int64() < TInt64(KMaxInt32))
			logger->Log(_L("%S %d"),&str,time.Int64());
		else
			logger->Log(_L("%S High=%u Low=%u"),&str,I64HIGH(time.Int64()),I64LOW(time.Int64()));
	}
}

void CTestCaseBase::LogTime(TPtrC str, TTimeIntervalMicroSeconds time, CStifLogger* aLogger)
{
	if (aLogger)
	{
		if (time.Int64() < TInt64(KMaxInt32))
			aLogger->Log(_L("%S %d"),&str,time.Int64());
		else
			aLogger->Log(_L("%S High=%u Low=%u"),&str,I64HIGH(time.Int64()),I64LOW(time.Int64()));
	}
}

