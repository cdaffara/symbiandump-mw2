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

#ifndef __CMSVATTACHMENTWAITER_H__
#define __CMSVATTACHMENTWAITER_H__

#include <e32base.h>

class CMsvStore;
class MMsvAttachmentManager;

/**

@internalTechnology
@released
*/
class CMsvAttachmentWaiter : public CActive
	{
public:
	static CMsvAttachmentWaiter* NewL();
	~CMsvAttachmentWaiter();
	
	void StartWaitingL(TRequestStatus& aStatus, CMsvStore* aStore, MMsvAttachmentManager* aAttachmentManager);
	
private:
	CMsvAttachmentWaiter();
	
	void Reset();
	
	// from CActive
	void RunL();
	void DoCancel();
	TInt RunError(TInt aError);
	
private:
	TRequestStatus* iReportStatus;
	CMsvStore* iStore;
	MMsvAttachmentManager* iAttachmentManager;
	};

#endif // __CMSVATTACHMENTWAITER_H__
