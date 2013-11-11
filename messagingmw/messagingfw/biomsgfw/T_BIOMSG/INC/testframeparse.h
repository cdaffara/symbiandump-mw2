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

#ifndef __TESTFRAMEPARSE_H
#define __TESTFRAMEPARSE_H

#include "testframeutils.h"

#include <badesca.h>

class CBaseCommandParser : public CBase
{
public:
	virtual ~CBaseCommandParser();
	void ParseL(const TDesC& aCommand, TTestDebugInfo aDebugInfo);
	TBool CanParse(const TDesC& aCommand) const;
	TDesC& ErrorL();

protected:
	virtual void ProcessL() = 0;
	virtual void ConstructL();
	inline CBaseCommandParser() {}
	void AddCommandL(const TDesC& aCommand);
	void CheckNumberOfArgumentsL(TInt aNumberOfArguments);
	void SetErrorL(const TDesC& aError);

protected:
	HBufC*			iCurrentCommand;
	CDesCArrayFlat* iArgumentList;
	TTestDebugInfo	iDebugInfo;
	HBufC*			iLastError;

private:
	CDesCArrayFlat* iSupportedCommands;
};

class CBaseSectionParser : public CBase
{
public:
	void ParseL();
	virtual ~CBaseSectionParser();

protected:
	virtual void ConstructL(const TDesC& aSectionName);
	virtual void LogComment(const TDesC& aComment) = 0;
	inline CBaseSectionParser(CTestScript& aScript) : iScript(aScript) {}
	void AddCommandParserL(CBaseCommandParser* aParser);

protected:
	CTestScript& iScript;
	CTestSection* iSection;
	CArrayPtrFlat<CBaseCommandParser>* iCommandParsers;

private:
	void SetSectionL(const TDesC& aSectionName);
};

#endif
