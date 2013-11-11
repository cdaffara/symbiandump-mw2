/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* @file 
* This is the header file for CT_MsgStartUp
* 
*
*/





#ifndef __T_STARTUP_H__
#define __T_STARTUP_H__


/* User includes */
#include <test/testexecutestepbase.h>


/* Literals Used */
_LIT(KStartUp,"StartUp");


/* Implements a dummy test step to be called in the beginning of the script */
class CT_MsgStartUp : public CTestStep
	{
public:
	CT_MsgStartUp();

	/* CTestStep implementation */
	virtual TVerdict	doTestStepL();	
	};
#endif /* __T_STARTUP_H__ s*/
