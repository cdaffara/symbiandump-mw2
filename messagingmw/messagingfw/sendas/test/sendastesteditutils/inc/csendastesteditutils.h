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

#ifndef __CSENDASTESTEDITUTILS_H__
#define __CSENDASTESTEDITUTILS_H__

#include <csendaseditutils.h>
#include <msvapi.h>

// forward declarations
class CSendAsTestEditTimer;

class CSendAsTestEditUtils : public CSendAsEditUtils
	{
public:
	static CSendAsTestEditUtils* NewL();
	virtual ~CSendAsTestEditUtils();

	// methods from CSendAsEditUtils
	virtual void LaunchEditorL(TMsvId aId, TRequestStatus& aStatus);
	virtual void LaunchEditorAndWaitL(TMsvId aId);
	virtual void Cancel();
	
	// callback for timer
	void TimerDone();

private:
	CSendAsTestEditUtils();
	void ConstructL();

	CSendAsTestEditTimer*	iEditTimer;
	TRequestStatus* 		iUserStatus;
	};

//**********************************
// CDummyObserver
//**********************************
class CMsvEntrySelection;

class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};
	
//**********************************


#endif	// __CSENDASTESTEDITUTILS_H__
