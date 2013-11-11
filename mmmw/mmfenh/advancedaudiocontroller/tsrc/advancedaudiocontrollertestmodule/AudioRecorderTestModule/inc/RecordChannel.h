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


#if !defined(AFX_RecordChannel_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_)
#define AFX_RecordChannel_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"
// CONSTANTS

class CRecordChannel  : CRecordingBase
{
public:
	CRecordChannel(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordChannel();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);
private:

	void ConstructL();

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TTimeIntervalMicroSeconds iRecTime;

//	TBool iFirstTime;
	TInt iCurrChannelNum;

};

#endif // !defined(AFX_RecordChannel_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_)
