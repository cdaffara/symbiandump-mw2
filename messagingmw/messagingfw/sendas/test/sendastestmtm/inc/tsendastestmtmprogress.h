// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __TSENDASTESTMTMPROGRESS_H__
#define __TSENDASTESTMTMPROGRESS_H__

#include <e32base.h>

class TSendAsTestMtmProgress 
	{
public:
	enum TSendAsTestProgressState
		{
		ESendAsTestWaiting		= 0,
		ESendAsTestInitSend,
		ESendAsTestSendNext,
		ESendAsTestSending,	
		ESendAsTestMoving	
		};
public:
	TInt iError; 
	TInt iState;
	TInt iMessagesDone;
	TInt iMessageCount; 
	};

typedef TPckgBuf<TSendAsTestMtmProgress> TSendAsTestMtmProgressBuf;

#endif	// __TSENDASTESTMTMPROGRESS_H__
