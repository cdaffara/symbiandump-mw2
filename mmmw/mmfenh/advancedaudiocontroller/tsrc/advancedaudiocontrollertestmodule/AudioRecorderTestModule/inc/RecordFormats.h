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


#if !defined(AFX_RECORDFORMATS_H__3EDE1ACC_939F_49BE_B0B1_55C01807632D__INCLUDED_)
#define AFX_RECORDFORMATS_H__3EDE1ACC_939F_49BE_B0B1_55C01807632D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TestCaseBase.h"
#include <MdaAudioSampleEditor.h>

class CRecordFormats : public CTestCaseBase, public MMdaObjectStateChangeObserver
{
public:
	CRecordFormats(CTestModuleIf *aConsole, CStifLogger *aLogger);

	virtual ~CRecordFormats();


	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	virtual void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);

	static CRecordFormats* NewL(CTestModuleIf *aConsole, CStifLogger *aLogger,TFileName* fileName);

	CMdaAudioRecorderUtility* recorder;

	static void GetFileName(TPtrC path, TFileName* fileName);

private:
	void ConstructL(TFileName* fileName);
	TInt iCallBackErr;
};

#endif // !defined(AFX_RECORDFORMATS_H__3EDE1ACC_939F_49BE_B0B1_55C01807632D__INCLUDED_)
