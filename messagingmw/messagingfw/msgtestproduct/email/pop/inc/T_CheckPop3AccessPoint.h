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

#ifndef __T_CHECKPOP3ACCESSPOINT_H__
#define __T_CHECKPOP3ACCESSPOINT_H__

#include "T_MsgSyncStepPop.h"

_LIT(KCheckPop3AccessPoint,"CheckPop3AccessPoint");

class CT_MsgCheckPop3AccessPoint : public CT_MsgSyncStepPOP
	{
public :
	CT_MsgCheckPop3AccessPoint(CT_MsgSharedDataPop& aSharedDataPOP);

	~CT_MsgCheckPop3AccessPoint();

	/* CTestStep implementation */
	TVerdict doTestStepL();
	};

#endif //__T_CHECKPOP3ACCESSPOINT_H__
