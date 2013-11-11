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


#if !defined(AFX_RecordBitRate_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_)
#define AFX_RecordBitRate_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"
// CONSTANTS
// AMR Bit Rate constant
const TUint KAmrBitRate4750		= 4750;
const TUint KAmrBitRate5150		= 5150;
const TUint KAmrBitRate5900		= 5900;
const TUint KAmrBitRate6700		= 6700;
const TUint KAmrBitRate7400		= 7400;
const TUint KAmrBitRate7950		= 7950;
const TUint KAmrBitRate10200	= 10200;
const TUint KAmrBitRate12200	= 12200;

class CRecordBitRate  : CRecordingBase
{
public:
	CRecordBitRate(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordBitRate();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);
private:

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TTimeIntervalMicroSeconds iRecTime;

//	TBool isBitRateTest;

	TInt iCurrBitRate;
//	TInt iOrigBitRate;

};

#endif // !defined(AFX_RecordBitRate_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_)
