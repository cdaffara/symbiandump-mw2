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
* This is the header file for CT_MsgSetImap4ServiceIdleParameters
* 
*
*/





#ifndef __T_MESS_SET_IMAP4_SERVICE_IDLE_PARAMETERS_H__
#define __T_MESS_SET_IMAP4_SERVICE_IDLE_PARAMETERS_H__



/* User Include*/
#include "T_MsgSyncStepIMAP.h"

/* Literal used */
_LIT(KSetImap4ServiceIdleParamters,"SetImap4ServiceIdleParamters");



/* Implements a test step to set the IMAP service parameters */
class CT_MsgSetImap4SetImap4ServiceIdleParamters : public CT_MsgSyncStepIMAP
	{
public :
	CT_MsgSetImap4SetImap4ServiceIdleParamters(CT_MsgSharedDataImap& aSharedDataIMAP);

	~CT_MsgSetImap4SetImap4ServiceIdleParamters();

	/* CTestStep implementation */
	TVerdict doTestStepL();
	};
#endif /*__T_MESS_SET_IMAP4_SERVICE_IDLE_PARAMETERS_H__ */
