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


#if !defined(AFX_RECORDMETADATA_H__05C132D7_FAC7_4581_BCFB_295A56C7FE63__INCLUDED_)
#define AFX_RECORDMETADATA_H__05C132D7_FAC7_4581_BCFB_295A56C7FE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RecordingBase.h"

class CRecordMetaData : CRecordingBase
{
public:
	CRecordMetaData(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~CRecordMetaData();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	virtual TInt PreProcess(CStifSectionParser *aParser);

	virtual TInt PostProcess(CStifSectionParser *aParser);

protected:

	static CRecordingBase* CreateObjectL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	TInt GetMetaData(CStifSectionParser *aParser, TPtrC& name, TPtrC& value, TBool & first, TBool & noMore);

	TInt IsMetaDataContained(TPtrC& name, TPtrC& value, TBool & hasMD);
};

#endif // !defined(AFX_RECORDMETADATA_H__05C132D7_FAC7_4581_BCFB_295A56C7FE63__INCLUDED_)
