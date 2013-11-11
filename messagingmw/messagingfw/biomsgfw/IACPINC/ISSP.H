// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Internet Script Settings Parser
// 
//

#ifndef __ISSP_H__
#define __ISSP_H__
#include <commsdat.h>
#include <commsdattypesv1_1.h>
using namespace CommsDat;

class CScriptParser : public CBase
/**
@internalComponent
@released
*/
	{
public:
	static CScriptParser* NewLC();
	static CScriptParser* NewL();
	~CScriptParser();
	void ParseL(CParsedFieldCollection& aIacpFields);
	void ProcessL(CMsvEntry& aEntry); 

private:
	CScriptParser();
	void ConstructL();
	void ChangeLoginScriptToNewlines();

	enum TLoginScriptMode
		{
		ENewScript=1,   //Create new login script
		EAddToScript	//Add to existing login script
		};
private:
	TInt	iScriptMode; 
	HBufC*	iScriptName;
	HBufC*  iScriptType;
	TBool	iUseLoginScript;
	HBufC*	iLoginScript;

	CMDBSession*					iDbSession;
	};
#endif  
