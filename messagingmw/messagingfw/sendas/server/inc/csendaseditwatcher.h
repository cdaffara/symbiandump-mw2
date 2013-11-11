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

#ifndef __CSENDASEDITWATCHER_H__
#define __CSENDASEDITWATCHER_H__

#include <e32base.h>
#include <msvapi.h>

class MSendAsSenderObserver;
class MSendAsEditObserver;
class CBaseMtm;
class CMsvOperation;
class CMsvEntrySelection;
class CSendAsEditUtils;

/**
@internalComponent
@released
*/
class CSendAsEditWatcher : public CActive
	{
public:
	static CSendAsEditWatcher* NewL(MSendAsEditObserver& aObserver, TUid aUid);
	virtual ~CSendAsEditWatcher();
	//
	void LaunchEditorL(TMsvId aEntryId);
private:
	CSendAsEditWatcher(MSendAsEditObserver& aObserver);
	void ConstructL(TUid aUid);
	//	
	void Complete(TInt aError);
	// methods from CActive
	virtual void RunL();
	virtual void DoCancel();
	virtual TInt RunError(TInt aError); 
private:
	enum TSendAsEditWatcherState
		{
		EEditWatcherIdle	  =0,
		EEditWatcherEditing,
		EEditWatcherDone
		};
private:
	MSendAsEditObserver*		iObserver;
	TSendAsEditWatcherState		iState;
	CSendAsEditUtils* 			iUtils;
	RThread						iThread;
	//
	};

#endif	// __CSENDASEDITWATCHER_H__
