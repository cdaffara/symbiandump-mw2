// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __T_CHECKIMAP4ACCESSPOINT_H__
#define __T_CHECKIMAP4ACCESSPOINT_H__

#include "T_MsgSyncStepIMAP.h"

_LIT(KCheckImap4AccessPoint,"CheckImap4AccessPoint");

class CT_MsgCheckImap4AccessPoint : public CT_MsgSyncStepIMAP
	{
public :
	CT_MsgCheckImap4AccessPoint(CT_MsgSharedDataImap& aSharedDataIMAP);

	~CT_MsgCheckImap4AccessPoint();

	/* CTestStep implementation */
	TVerdict doTestStepL();
	};

#endif //__T_CHECKIMAP4ACCESSPOINT_H__
