// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

/*
class MTestDebugInfo
	{
public:
	virtual HBufC* TestStartStringLC() = 0;
	virtual HBufC* TestCompleteStringLC() = 0;
	virtual HBufC* TestFailedStringLC(TInt aReason) = 0;
	};
*/

TInt ResolveFile(RFs& aFs, const TDesC& aComponent, const TDesC& aFileName, TParse& aParseOut);

//
//
// TTestDebugInfo
//

class TTestDebugInfo/* : public MTestDebugInfo*/
	{
public:
	HBufC* TestStartStringLC();
	HBufC* TestCompleteStringLC();
	HBufC* TestFailedStringLC(TInt aReason);
	HBufC* TestHarnessStringLC();
	TInt LineNumber();

	void SetTestHarnessName(TInt aTestHarnessPosition);

	TTestDebugInfo(CTestScript& aTestScript, TInt aSectionPosition, TInt aCommandPosition);
	TTestDebugInfo(CTestScript& aTestScript, TInt aSectionPosition, TInt aCommandPosition, TInt aLineNumber);
	TTestDebugInfo();

protected:
	HBufC* SectionNameLC();
	HBufC* TestHarnessNameLC();
	HBufC* CommandNameLC();

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

	static CTestScript* NewL(RFs& aFs);

	~CTestScript();

	TBool LoadFileL(const TDesC& aFileName);
	CTestSection* GetSectionL(const TDesC& aSectionName);
	TBool GetLineL(TTestScriptString& aLine, TInt aPos);

private:
	void ConstructL();
	CTestScript(RFs& aFs);
	TBool GetLineL(TTestScriptString& aLine);
	TBool IsSection(const TDesC8& aLine) const;
	TBool IsSectionEnd(const TDesC8& aLine) const;
	TBool IsComment(const TDesC8& aLine) const;
	TBool GetNextCommandInSectionL(TTestScriptString& aCommand, TInt& aLineNumber);

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
	static CTestSection* NewL(TInt aSectionPosition, const TDesC& aSectionName, TInt aLineNumber);
	TInt GetCurrentCommand(TDes& aCurrentCommand) const;
	TBool NextCommand();
	void AddCommandL(const CTestScript::TTestScriptString& aCommand, TInt aCommandPosition, TInt aLineNumber);
	TInt SectionPosition() const;
	TInt CurrentCommandPosition() const;
	const TDesC& SectionName() const;
	~CTestSection();

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
