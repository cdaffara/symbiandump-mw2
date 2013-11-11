// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#if !defined (__MENTACT_H__)
#define __MENTACT_H__

#if !defined (__E32BASE_H__)
#include <e32base.h>
#endif


class CMsgActive : public CActive
/**
@internalTechnology
@released
*/
	{
public:
	IMPORT_C ~CMsgActive();
protected:
	IMPORT_C CMsgActive(TInt aPriority);
	IMPORT_C void Queue(TRequestStatus& aStatus);
	IMPORT_C void Complete(TInt aStatus);
	IMPORT_C void DoCancel();
private:
	IMPORT_C void RunL();
	virtual void DoRunL()=0;
	IMPORT_C virtual void DoComplete(TInt&);
private:
	TRequestStatus* iReport;
	};

#endif 
