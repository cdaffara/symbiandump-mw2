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


#if !defined(AFX_RECORDCLIP_H__94544E89_CF02_421D_B28B_2AC0D5D6E6E6__INCLUDED_)
#define AFX_RECORDCLIP_H__94544E89_CF02_421D_B28B_2AC0D5D6E6E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"

class CRecordClip : CRecordingBase
{
public:
	CRecordClip(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordClip();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

protected:

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TTimeIntervalMicroSeconds errRange;
};


#endif // !defined(AFX_RECORDCLIP_H__94544E89_CF02_421D_B28B_2AC0D5D6E6E6__INCLUDED_)
