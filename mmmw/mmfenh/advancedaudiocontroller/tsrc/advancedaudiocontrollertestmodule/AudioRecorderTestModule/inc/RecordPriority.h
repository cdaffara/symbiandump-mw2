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

#if !defined(AFX_RECORDPRIORITY_H__200939ED_5898_49C5_A7CA_5C1CC7E857E0__INCLUDED_)
#define AFX_RECORDPRIORITY_H__200939ED_5898_49C5_A7CA_5C1CC7E857E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"

class CPriorityParams;

class CRecordPriority : CRecordingBase
{
public:
	CRecordPriority(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordPriority();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	TInt ExecuteL(CParameters* aParams);

	void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);

private:

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	static TInt ParseTag(CStifItemParser* item, TPtrC& name, TInt& priority, TInt& finish ,TTimeIntervalMicroSeconds& timeToStart);

	TInt finishPlace;

	TInt expectedFinishPlace;

	TBool firstTime;

	TBool isInterrupted;

	CPriorityParams* priorityParams;

	TBool wasInterrupted;
};

class CTimerParams : public CParameters
{
public:
	CMediaEvent* timer;
};

class CPriorityParams : public CParameters
{
public:
	TInt* finishPlace;
	TInt* totalFiles;
	TBool* error;
};

#endif // !defined(AFX_RECORDPRIORITY_H__200939ED_5898_49C5_A7CA_5C1CC7E857E0__INCLUDED_)
