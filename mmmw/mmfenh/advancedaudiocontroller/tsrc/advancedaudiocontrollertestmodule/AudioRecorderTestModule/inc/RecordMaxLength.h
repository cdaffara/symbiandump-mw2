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


#if !defined(AFX_RecordSupportedDataType_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_)
#define AFX_RecordSupportedDataType_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"

class CRecordMaxLength : public CRecordingBase
{
public:
	CRecordMaxLength(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordMaxLength();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

protected:
	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TInt errorRange;

	TTimeIntervalMicroSeconds timeAvailable;

};

#endif // !defined(AFX_RecordSupportedDataType_H__73051B14_ADBF_4364_81DB_E438FDF6C607__INCLUDED_)
