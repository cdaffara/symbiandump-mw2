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


#if !defined(AFX_RECORDBALANCE_H__0DB26F38_987F_4224_9F9D_6389F4FC61F4__INCLUDED_)
#define AFX_RECORDBALANCE_H__0DB26F38_987F_4224_9F9D_6389F4FC61F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"

class CRecordBalance  : CRecordingBase
{
public:
	CRecordBalance(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordBalance();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);
private:

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TTimeIntervalMicroSeconds recTime;

	TBool firstTime;

	TInt currBalance;
	TInt origBalance;

};

#endif // !defined(AFX_RECORDBALANCE_H__0DB26F38_987F_4224_9F9D_6389F4FC61F4__INCLUDED_)
