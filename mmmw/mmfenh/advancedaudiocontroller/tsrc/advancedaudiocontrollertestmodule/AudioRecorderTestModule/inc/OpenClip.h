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
* Description:  AudioRecorder Test
*
*/


#if !defined(AFX_OPENCLIP_H__F6C06EE1_4B03_4B62_B5DF_E6B01B9A027B__INCLUDED_)
#define AFX_OPENCLIP_H__F6C06EE1_4B03_4B62_B5DF_E6B01B9A027B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MdaAudioSampleEditor.h>
#include "TestCaseBase.h"

class COpenClip : public CTestCaseBase, public MMdaObjectStateChangeObserver
{
public:
	COpenClip(CTestModuleIf *aConsole, CStifLogger *aLogger);
	virtual ~COpenClip();

	static TInt RunTestL(CTestModuleIf *aConsole, CStifLogger *aLogger, CStifSectionParser *aParser, TInt* clipCounter);

	static COpenClip* NewL(CTestModuleIf *aConsole, CStifLogger *aLogger);

	virtual void MoscoStateChangeEvent(CBase* aObject, TInt aPreviousState, TInt aCurrentState, TInt aErrorCode);

private:
	void ConstructL();

	static TInt ParseTag(CStifItemParser* item, TInt & type, TPtrC& name, TTimeIntervalMicroSeconds& duration);

	static void OpenTarget(CMdaAudioRecorderUtility* rec, TPtrC fileName, TInt type, HBufC8* desc, TInt iapid, TDesC8& mime, CStifLogger *iLogger);

	static HBufC8* GetDescriptor8LC(const TFileName &aFileName, CStifLogger *iLogger);

	static TDesC8* ConvertTDesC(TDesC16& string);

	TInt callbackErr;
};

#endif // !defined(AFX_OPENCLIP_H__F6C06EE1_4B03_4B62_B5DF_E6B01B9A027B__INCLUDED_)
