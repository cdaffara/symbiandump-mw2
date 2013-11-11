// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CSENDASEIKEDITUTILS_H__
#define __CSENDASEIKEDITUTILS_H__

#include <csendaseditutils.h>

/**
@internalComponent
@released
*/
class CSendAsEikEditUtils : public CSendAsEditUtils
	{
public:
	static CSendAsEikEditUtils* NewL();
	virtual ~CSendAsEikEditUtils();

	// methods from CSendAsEditUtils
	virtual void LaunchEditorL(TMsvId aId, TRequestStatus& aStatus);
	virtual void LaunchEditorAndWaitL(TMsvId aId);
	virtual void Cancel();
	
private:
	CSendAsEikEditUtils();
	RThread iThread;
	};

#endif	// __CSENDASEIKEDITUTILS_H__
