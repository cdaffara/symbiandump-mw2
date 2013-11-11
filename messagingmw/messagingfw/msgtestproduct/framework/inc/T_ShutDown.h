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
* This is the header file for CT_MsgShutDown
* 
*
*/




#ifndef __T_SHUTDOWN_H__
#define __T_SHUTDOWN_H__


/* User includes */
#include <test/testexecutestepbase.h>

 /* Literals Used */
_LIT(KShutDown,"ShutDown");


/* Implements a dummy test step to be called at the end of the script */
class CT_MsgShutDown : public CTestStep
	{
public:
	CT_MsgShutDown();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();	
	};
#endif /*__T_SHUTDOWN_H__ */