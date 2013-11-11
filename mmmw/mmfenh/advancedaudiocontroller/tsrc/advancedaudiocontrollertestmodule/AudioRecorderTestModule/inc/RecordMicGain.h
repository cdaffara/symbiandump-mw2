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


#if !defined(AFX_RECORDMICGAIN_H__08375F02_31A2_4862_BE5B_5058DA2E6D6B__INCLUDED_)
#define AFX_RECORDMICGAIN_H__08375F02_31A2_4862_BE5B_5058DA2E6D6B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"

class CRecordMicGain : CRecordingBase
{
public:
	CRecordMicGain(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordMicGain();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);
private:

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TTimeIntervalMicroSeconds recTime;

	TBool firstTime;


	TInt expectedGain;
	TInt theMaxGain;
	TInt currGain;
};

#endif // !defined(AFX_RECORDMICGAIN_H__08375F02_31A2_4862_BE5B_5058DA2E6D6B__INCLUDED_)
