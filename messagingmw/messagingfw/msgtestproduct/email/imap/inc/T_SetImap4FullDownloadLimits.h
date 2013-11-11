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
* This is the header file for CT_MsgSetImap4FullDownloadLimits
* 
*
*/




#ifndef __T_SET_IMAP4_FULL_DOWNLOAD_LIMITS_H__
#define __T_SET_IMAP4_FULL_DOWNLOAD_LIMITS_H__


/* User Includes */
#include "T_MsgSyncStepIMAP.h"



/*  Literal defined for the teststep name */
_LIT(KSetImap4FullDownloadLimits,"SetImap4FullDownloadLimits");


/* Implements a test step to set the full download limits for Imap */
class CT_MsgSetImap4FullDownloadLimits : public CT_MsgSyncStepIMAP
	{
public :
	CT_MsgSetImap4FullDownloadLimits(CT_MsgSharedDataImap& aSharedDataIMAP);

	~CT_MsgSetImap4FullDownloadLimits();

	/* CTestStep implementation */
	virtual TVerdict doTestStepL();
	};
#endif /*__T_SET_IMAP4_FULL_DOWNLOAD_LIMITS_H__*/
