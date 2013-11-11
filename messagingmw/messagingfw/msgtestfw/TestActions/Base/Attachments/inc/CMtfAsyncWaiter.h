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

#ifndef __CMTFASYNCWAITER_H__
#define __CMTFASYNCWAITER_H__

#include <e32base.h>

class CMtfAsyncWaiter : public CActive
	{
public:
	static CMtfAsyncWaiter* NewL();
	~CMtfAsyncWaiter();
	
	void StartAndWait();
	TInt Result() const;
	
private:
	CMtfAsyncWaiter();
	
	// from CActive
	virtual void RunL();
	virtual void DoCancel();
	
private:
	TInt iError;
	};

#endif // __CMTFASYNCWAITER_H__
