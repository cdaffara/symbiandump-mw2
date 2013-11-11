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


#if !defined(AFX_RECORDBYSIZE_H__B51B266A_7D2B_415B_AB2F_1F64DFBBB533__INCLUDED_)
#define AFX_RECORDBYSIZE_H__B51B266A_7D2B_415B_AB2F_1F64DFBBB533__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"

class CRecordBySize : CRecordingBase
{
public:
	CRecordBySize(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordBySize();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

	TInt GetFileSize(const TFileName &aFileName, TInt & size);

	void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);

protected:

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TInt errorRange;

	TInt clipSize;

	TBool stopByMMF;
};

#endif // !defined(AFX_RECORDBYSIZE_H__B51B266A_7D2B_415B_AB2F_1F64DFBBB533__INCLUDED_)
