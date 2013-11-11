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

#ifndef __CSENDASACTIVECONTAINER_H__
#define __CSENDASACTIVECONTAINER_H__

#include <e32base.h>

#include "msendassenderobserver.h"
#include "msendaseditobserver.h"

// forward declarations
class CSendAsSender;
class CSendAsEditWatcher;
class CSendAsServer;

/**
A single instance of this class is owned by the sendas server and is used to
observe background sendas operations, e.g. background sends and launched editors. 

Once an ownership of an operation has been moved to this class, the requesting
subsession may be closed.

When all outstanding tasks are completed, the container notifies the server,
which can then shut down if it also has no open sessions.

@internalComponent
@released
*/
class CSendAsActiveContainer : public CActive, public MSendAsSenderObserver, public MSendAsEditObserver
	{
public:
	static CSendAsActiveContainer* NewL(CSendAsServer& aServer);
	virtual ~CSendAsActiveContainer();
	//
	void AddSenderL(CSendAsSender& aSender);
	void AddEditWatcherL(CSendAsEditWatcher& aEditorWatcher);
	TBool IsEmpty() const;
	void PurgeInactive();
private:	
	CSendAsActiveContainer(CSendAsServer& aServer);
	void ConstructL();
	// from CActive
	virtual void RunL();
	virtual void DoCancel();
	// from MSendAsSenderObserver
	virtual void SenderComplete(TInt aError);
	// from MSendAsEditObserver
	virtual void EditComplete(TInt aError);
private:
	CSendAsServer& 						iServer;
	RPointerArray<CActive> 				iActiveList;
	};
	
#endif	// __CSENDASACTIVECONTAINER_H__
