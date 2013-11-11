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
* Description:  AudioRecorder test
*
*/


#if !defined(AFX_TESTCASEBASE_H__D4B59A8B_C8BC_4A15_A680_CA3C71B2E4A9__INCLUDED_)
#define AFX_TESTCASEBASE_H__D4B59A8B_C8BC_4A15_A680_CA3C71B2E4A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MdaAudioSampleEditor.h>
#include <e32std.h>
#include <e32base.h>
#include <StifTestInterface.h>
#include <StifLogger.h>
#include <StifParser.h>
#include <Utility.h>
#include "constants.h"
const TUid KUidInterfaceMMFController = {KMmfUidPluginInterfaceController};


class CTestCaseBase : public CBase
{
public:
	CTestCaseBase();
	virtual ~CTestCaseBase();

protected:

	static TInt ParseFileName(CStifItemParser* item, TPtrC& name, TTimeIntervalMicroSeconds& duration);

	static TInt64 ParseTime(CStifItemParser* item);

	void LogTime(TPtrC str, TTimeIntervalMicroSeconds time);

	static void LogTime(TPtrC str, TTimeIntervalMicroSeconds time, CStifLogger* aLogger);

	CStifLogger* logger;

	CTestModuleIf* console;
};

#endif // !defined(AFX_TESTCASEBASE_H__D4B59A8B_C8BC_4A15_A680_CA3C71B2E4A9__INCLUDED_)
