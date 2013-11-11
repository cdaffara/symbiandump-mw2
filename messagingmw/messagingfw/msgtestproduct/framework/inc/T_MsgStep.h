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
* This contains the header file for CT_MsgStep
* 
*
*/




#ifndef __T_MSGSTEP_H__
#define __T_MSGSTEP_H__


/* User includes */
#include <test/testexecutestepbase.h>
#include "T_MsgSharedDataBase.h"


/* 
 * Implements a test step which contains the pointer to the shared data object
 * Other asynchronous and synchronous test steps can be derived from it.
 */
class CT_MsgStep : public CTestStep
	{
protected:
	CT_MsgStep();
	};

#endif /* __T_MSGSTEP_H__ */
