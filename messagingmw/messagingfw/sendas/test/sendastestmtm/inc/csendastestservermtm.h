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

#ifndef __CSENDASTESTSERVERMTM_H__
#define __CSENDASTESTSERVERMTM_H__

#include <mtsr.h>

#include "tsendastestmtmprogress.h"

class CSendAsTestServerMtm : public CBaseServerMtm
	{
public:
	IMPORT_C CSendAsTestServerMtm* NewL(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry);
	virtual ~CSendAsTestServerMtm();
private:	// methods from CBaseServerMtm
	virtual void CopyToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	virtual void CopyFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	virtual void CopyWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	virtual void DeleteAllL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	virtual void CreateL(TMsvEntry aNewEntry, TRequestStatus& aStatus);
	virtual void ChangeL(TMsvEntry aNewEntry, TRequestStatus& aStatus);
	virtual void StartCommandL(CMsvEntrySelection& aSelection, TInt aCommand, const TDesC8& aParameter, TRequestStatus& aStatus);
	virtual TBool CommandExpected();
	virtual const TDesC8& Progress();
	virtual void MoveToLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	virtual void MoveFromLocalL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	virtual void MoveWithinServiceL(const CMsvEntrySelection& aSelection,TMsvId aDestination, TRequestStatus& aStatus);
	virtual void DoRunL();
	virtual void DoComplete(TInt aError);
private:	// methods from CActive
	virtual void DoCancel();	
private:
	CSendAsTestServerMtm(CRegisteredMtmDll& aRegisteredMtmDll, CMsvServerEntry* aServerEntry);
	void ConstructL();
	
	void SendMessagesL(const CMsvEntrySelection& aSelection, TRequestStatus& aStatus);
	void DoInitialiseSendL();
	void DoSendMessageL();
	void DoMoveToSentFolderL();
	void CompleteSelf();
	void HandleSendErrorL(TInt aSendingState);
private:
	RTimer						iTimer;
	CMsvEntrySelection* 		iSelection;
	TRequestStatus*				iRequestStatus;
	TMsvEntry 					iEntry;
	TMsvId						iCurrentMessage;
	TSendAsTestMtmProgress		iProgress;
	TSendAsTestMtmProgressBuf	iProgressBuf;
	};

#endif	// __CSENDASTESTSERVERMTM_H__
