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

#if !defined(__MSVCOP_H__)
#define __MSVCOP_H__

#if !defined(__E32BASE_H__)
#include <e32base.h>
#endif

#if !defined(__MCLIENT_H__)
#include <msvapi.h>
#endif

#include <tmsvsystemprogress.h>

//**********************************
// CMsvEntryOperation
//**********************************
//
// The main operation used in the server
//

class CMsvEntryOperation : public CMsvOperation
/**
@internalComponent
@released
*/
	{
public:
	~CMsvEntryOperation();
	const TDesC8& ProgressL();
	const TDesC8& FinalProgress();
	//
protected:
	static CMsvEntryOperation* NewL(CMsvSession& aMsvSession, TRequestStatus& aObserverRequestStatus);
	static CMsvEntryOperation* NewLC(CMsvSession& aMsvSession, TRequestStatus& aObserverRequestStatus);
	CMsvEntryOperation(CMsvSession& aMsvSession, TRequestStatus& aObserverRequestStatus);
	void ConstructL();
	//
	void Start();
	void DoCancel();
	void RunL();
	//
	IMPORT_C TInt Extension_(TUint aExtensionId, TAny *&a0, TAny *a1);
	//
private:
	TInt DoGetSystemProgress(TMsvSystemProgress& aOutSysProg);
private:
	HBufC8* iProgress;
	TMsvSystemProgress iSystemProgress;
	TInt iErrorGetSystemProgress;
	//
friend class CMsvEntry;
friend class CMsvSession;
	};


#endif
