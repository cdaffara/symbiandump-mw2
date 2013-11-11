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

#include "MSVOP.H"

/**
@internalComponent
@released
*/
class CMsvDeleteStoreOperation : public CMsvServerOperation
	{
public:
	CMsvDeleteStoreOperation(const RMessage2& aMessage, CMsvServer& aServer);
	~CMsvDeleteStoreOperation();
	//
	virtual const TDesC8& Progress(); // Inherited from CMsvServerOperation
	void StartL();
	//
private:
	virtual void DoCancel();   // Inherited from CActive
	virtual void RunL();	   // Inherited from CActive
	TInt RunError(TInt aError);
	void DeleteMailStoreL();
	void Completed(TInt);
	//
private:
	TPckgBuf<TMsvDeleteProgress> iProgress;
	TDriveUnit iDrive;
	CMsvServer& iServer;
	TFileName iToDelete;
	CFileMan* iFileMan;	
	};
	
