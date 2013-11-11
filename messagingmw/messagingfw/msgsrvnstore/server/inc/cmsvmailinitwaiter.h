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

#ifndef __CMSVMAILINITWAITER_H__
#define __CMSVMAILINITWAITER_H__

#include <e32base.h>
#include <f32file.h>

class CMsvServer;

class CMsvMailinitWaiter : public CActive
	{
public:
	static CMsvMailinitWaiter* NewL(CMsvServer& aServer);
	void WaitFor(RProcess& aProcess, TDriveNumber aDriveNum);
	~CMsvMailinitWaiter();

private:	
	// From CActive
	virtual void DoCancel();
	virtual void RunL();
	
	CMsvMailinitWaiter(CMsvServer& aServer);
	
private:
	CMsvServer& iServer;
	RProcess iProcess;
	TDriveNumber iDriveNum;
	};

#endif
