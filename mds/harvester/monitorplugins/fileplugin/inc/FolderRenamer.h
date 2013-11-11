/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:
*
*/

#ifndef FOLDERRENAMER_H_
#define FOLDERRENAMER_H_

#include <e32base.h>
#include <mdesession.h>

class CFileEventHandlerAO;
class TMdsFSPStatus;


class CRenameItem : public CBase
	{
public:
	static CRenameItem * NewL (const TDesC &aOldName, const TDesC &aNewName);

	virtual ~CRenameItem();

	void HandleFileEventsL(CFileEventHandlerAO &aCFileEventHandlerAO);
	void AddFileEvent(TMdsFSPStatus &aEvent);
	TPtrC OldName();
	TPtrC NewName();

private:
	CRenameItem();
	void ConstructL(const TDesC &aOldName, const TDesC &aNewName);

private:
	HBufC *iOldName;
	HBufC *iNewName;
	RPointerArray <TMdsFSPStatus> iFileEvents;		
	};

class CFolderRenamer : public CActive
	{
public:
	virtual ~CFolderRenamer();
	
	void RenameL(const TDesC &aOldName, const TDesC &aNewName);
	void HandleFileEventL(TMdsFSPStatus &aEvent);
	void HandleFileEventsL( CArrayFixSeg< TMdsFSPStatus >* aEvents );
	
	static CFolderRenamer * NewL(CFileEventHandlerAO &aCFileEventHandlerAO);

private:

	enum TFolderRenameState
	{
	ERenameStateIdle,
	ERenameStateRenaming,
	ERenameStateWaitingMdeRename,
	};

	void ConstructL();
	void DoCancel();
	void RunL();
    TInt RunError( TInt aError );
	void SetNextRequest(TFolderRenameState aState);
	CFolderRenamer(CFileEventHandlerAO &aCFileEventHandlerAO);

private:
	RPointerArray <CRenameItem> iRenamedFolders;		
	TFolderRenameState iState;
	CFileEventHandlerAO &iCFileEventHandlerAO;	
public:
	TBool iIsRunning;
	};

#endif /*FOLDERRENAMER_H_*/
