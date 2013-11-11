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

#ifndef __T_ADDIMAP4TRANSPORTBUFFERSIZES_H__
#define __T_ADDIMAP4TRANSPORTBUFFERSIZES_H__

#include "T_MsgSyncStepIMAP.h"

_LIT(KAddImap4TransportBufferSizes,"AddImap4TransportBufferSizes");

class CT_MsgAddImap4TransportBufferSizes : public CT_MsgSyncStepIMAP
	{
public :
	CT_MsgAddImap4TransportBufferSizes(CT_MsgSharedDataImap& aSharedDataIMAP);

	~CT_MsgAddImap4TransportBufferSizes();

	/* CTestStep implementation */
	TVerdict doTestStepL();
	};

#endif //__T_ADDIMAP4TRANSPORTBUFFERSIZES_H__
