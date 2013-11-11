/**
* Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/



/**
 @file
*/
#ifndef CMTFTESTACTION_POP3_TOP_BASE_H_INCLUDED
#define CMTFTESTACTION_POP3_TOP_BASE_H_INCLUDED

#include <msvapi.h>
#include <popcmtm.h>

#include "CMtfTestAction.h"

typedef TPckgBuf<TImPop3PopulateOptions>	TPop3MailInfoBuf ;

/*
Base class encapsulating the actual POP3 top operation
*/

class CMtfTestActionPop3TopBase : public CMtfTestAction
	{
	public:
		virtual ~CMtfTestActionPop3TopBase(); 
		virtual void ExecuteActionL() = 0;
		void RunL();
		void DoCancel();
	
	protected:
		void DoInvokeAsyncFunctionL( TPop3Cmds aCommand );

		CMsvOperation* iOperation;	
		CMtfTestActionPop3TopBase(CMtfTestCase& aTestCase);
		TImPop3PopulateOptions iMailInfo;
	};

#endif // ! defined CMTFTESTACTION_POP3_TOP_BASE_H_INCLUDED
