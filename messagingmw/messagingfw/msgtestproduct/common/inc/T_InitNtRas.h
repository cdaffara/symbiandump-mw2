/**
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
* This contains the header file for InitNtRas
* 
*
*/



/**
 @file 
*/


#ifndef __T_INIT_NTRAS_H__
#define __T_INIT_NTRAS_H__


/** User includes */
#include "T_MsgSharedDataCommon.h"
#include "T_MsgSyncStepCommon.h"


/* Literals Used */
_LIT(KInitNtRas,"InitNtRas");


/* Implements a test step to initialise the Ras connection */
class CT_MsgInitNtRas : public CT_MsgSyncStepCommon
	{
public:
	CT_MsgInitNtRas(CT_MsgSharedDataCommon& aSharedDataCommon); 

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();	
	};
#endif /* __T_INIT_NTRAS_H__ */
