// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

#include <e32std.h>
#include <f32file.h>

#ifndef TESTFRAMEUTILS_H
#define TESTFRAMEUTILS_H

class CTestSection;
class CTestScript;

class TTestDebugInfo
{
public:
	HBufC* TestStartStringLC();
	HBufC* TestCompleteStringLC();
	HBufC* TestFailedStringL(TInt aReason);
	HBufC* TestHarnessStringLC();
	TInt LineNumber();
	HBufC* CommandNameLC();

	void SetTestHarnessName(TInt aTestHarnessPosition);

	TTestDebugInfo(CTestScript& aTestScript, TInt aSectionPosition, TInt aCommandPosition);
	TTestDebugInfo(CTestScript& aTestScript, TInt aSectionPosition, TInt aCommandPosition, TInt aLineNumber);
	TTestDebugInfo();

	inline CTestScript* ScriptFile() {return iScriptFile;}

private:
	HBufC* SectionNameLC();
	HBufC* TestHarnessNameLC();
	void StringFromErrNum(TInt aError, TDes& aDes);

private:
	CTestScript* iScriptFile;
	TInt iSectionPosition;
	TInt iCommandPosition;
	TInt iTestHarnessPosition;
	TInt iLineNumber;
};



//
//
// CTestScript
//

class CTestScript : public CBase
	{
public:
	typedef TBuf8<128> TTestScriptString;

	IMPORT_C static CTestScript* NewL(RFs& aFs);

	IMPORT_C ~CTestScript();

	IMPORT_C TBool LoadFileL(const TDesC& aFileName);
	IMPORT_C CTestSection* GetSectionL(const TDesC& aSectionName);
	IMPORT_C TBool GetLineL(TTestScriptString& aLine, TInt aPos);

private:
	void ConstructL();
	CTestScript(RFs& aFs);
	TBool GetLineL(TTestScriptString& aLine);
	TBool IsSection(const TDesC8& aLine) const;
	TBool IsSectionEnd(const TDesC8& aLine) const;
	TBool IsComment(const TDesC8& aLine) const;
	TBool GetNextCommandInSectionL(TTestScriptString& aCommand, TInt& aPosition, TInt& aLineNumber);

private:
	TBuf<512> iLastError;

	struct TSectionPosition
		{
		TInt				iFilePosition;		// Start of commands
		TInt				iSectionPosition;	// Start of header name
		TTestScriptString	iSectionName;
		TInt				iLineNumber;		// MU 20/11/00 added to aid error messages
		};

	CArrayFixFlat<TSectionPosition>* iSectionPositions;
	TInt iCurrentSection;

	RFile iFile;
	RFs& iFs;
	TBool iEndOfFile;
	};


//
//
// CTestSection
//

class CTestSection : public CBase
	{
public:
	IMPORT_C static CTestSection* NewL(TInt aSectionPosition, const TDesC& aSectionName, TInt aLineNumber);
	IMPORT_C TInt GetCurrentCommand(TDes& aCurrentCommand) const;
	IMPORT_C TBool NextCommand();
	IMPORT_C void AddCommandL(const CTestScript::TTestScriptString& aCommand, TInt aCommandPosition, TInt aLineNumber);
	IMPORT_C TInt SectionPosition() const;
	IMPORT_C TInt CurrentCommandPosition() const;
	IMPORT_C const TDesC& SectionName() const;
	IMPORT_C ~CTestSection();

private:
	CTestSection(TInt aSectionPosition, TInt aLineNumber);
	void ConstructL();

private:
	struct TCommandInfo
		{
		CTestScript::TTestScriptString iCommand;
		TInt iCommandPosition;
		TInt iLineNumber;
		};

	CArrayFixFlat<TCommandInfo>* iCommandList;
	TInt iCommandIndex;
	TInt iSectionPosition;
	TInt iLineNumber;
	TBuf<128> iSectionName;
	};



#endif
