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


#if !defined(AFX_RECORDTIMEAVAILABLE_H__BA8F3C87_8354_4D28_9C9C_3711C8089406__INCLUDED_)
#define AFX_RECORDTIMEAVAILABLE_H__BA8F3C87_8354_4D28_9C9C_3711C8089406__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"

class CRecordTimeAvailable : CRecordingBase
{
public:
	CRecordTimeAvailable(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordTimeAvailable();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

protected:

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TTimeIntervalMicroSeconds errorRange;

//	TTimeIntervalMicroSeconds timeAvailable;
};

#endif // !defined(AFX_RECORDTIMEAVAILABLE_H__BA8F3C87_8354_4D28_9C9C_3711C8089406__INCLUDED_)
