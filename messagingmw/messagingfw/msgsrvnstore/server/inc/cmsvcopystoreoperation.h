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

#include <f32file.h>

/**
@internalComponent
@released
*/
class CMsvCopyStoreOperation : public CMsvServerOperation, public MFileManObserver
	{
public:
	static CMsvCopyStoreOperation* NewL(const RMessage2& aMessage, CMsvServer& aServer);
	~CMsvCopyStoreOperation();
	//
	const virtual TDesC8& Progress(); // Inherited from CMsvServerOperation
	void StartL();
	//
private:
	CMsvCopyStoreOperation(const RMessage2& aMessage, CMsvServer& aServer);
	void DoCancel(); // Inherited from CActive
	void RunL();	 // Inherited from CActive
	TInt RunError(TInt aError);
	
	void InitCopyStoreL();
	void CopySourceL();
	void FinishCopyStoreL();
	void LockMailStoreL();
	void UnlockMailStore();
	void CompleteSelf();
	void Completed(TInt aError);
	void CheckDiskSpaceL();
	MFileManObserver::TControl NotifyFileManOperation();
	MFileManObserver::TControl NotifyFileManEnded();
private:
	TPckgBuf<TMsvCopyProgress> iProgress;
	TDriveUnit iDrive;
	TInt iPos;
	CMsvServer& iServer;
	TFileName iDest;
	TFileName iIndex;
	TFileName iSrc;
	CDir* 	  iDir;
	CFileMan*	  iFileMan;
	TBool 	iCopyCancel;
	};
