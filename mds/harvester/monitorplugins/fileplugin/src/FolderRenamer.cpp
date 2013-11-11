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

#include <e32std.h>

#include "FolderRenamer.h"
#include "harvesterlog.h"
#include "mdeharvestersession.h"
#include "mdeharvestersession.h"
#include "harvesterdata.h"
#include "fileeventhandlerao.h"
#include "harvestercommon.h"

CRenameItem * CRenameItem::NewL(
		const TDesC &aOldName, const TDesC &aNewName)
	{	
	CRenameItem *self = new (ELeave) CRenameItem();
	CleanupStack::PushL(self);
	self->ConstructL(aOldName,aNewName);
	CleanupStack::Pop(self);
	return self;
	}
	
CRenameItem::~CRenameItem()
	{
	delete iOldName;
	iOldName = NULL;
	delete iNewName;
	iNewName = NULL;
	iFileEvents.ResetAndDestroy();
	}
	

void CRenameItem::ConstructL(const TDesC &aOldName, const TDesC &aNewName)
	{
	iOldName = HBufC::NewL( aOldName.Length() );
	iNewName = HBufC::NewL( aNewName.Length() );

	// convert paths to lower case
	// Note: CopyLC doesn't push anything to cleanup stack
	iOldName->Des().CopyLC( aOldName );
	iNewName->Des().CopyLC( aNewName );
	}

CRenameItem::CRenameItem() : iOldName( NULL ), iNewName( NULL )
	{
	//No implementation required
	}

void CRenameItem::AddFileEvent(TMdsFSPStatus &aEvent)
	{
	TMdsFSPStatus* event = NULL;
	event = new TMdsFSPStatus(aEvent);
	
	if (event)
		{
		if(iFileEvents.Append(event) != KErrNone)
		    {
            delete event;
            event = NULL;
		    }
		}
	}

void CRenameItem::HandleFileEventsL(CFileEventHandlerAO &aCFileEventHandlerAO)
	{
	const TInt count = iFileEvents.Count();
	for (TInt i = 0; i < count; i++)
		{
		aCFileEventHandlerAO.HandleNotificationL(* (iFileEvents[i]));
		}
	iFileEvents.ResetAndDestroy();
	}

TPtrC CRenameItem::OldName()
	{
	return iOldName->Des();
	}


TPtrC CRenameItem::NewName()
	{
	return iNewName->Des();
	}


CFolderRenamer * CFolderRenamer::NewL(CFileEventHandlerAO &aCFileEventHandlerAO)
	{	
	CFolderRenamer *self = new (ELeave) CFolderRenamer(aCFileEventHandlerAO);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


CFolderRenamer::CFolderRenamer(CFileEventHandlerAO &aCFileEventHandlerAO) : 
	CActive(KHarvesterPriorityMonitorPlugin),
	iState(ERenameStateIdle),
	iCFileEventHandlerAO(aCFileEventHandlerAO),
	iIsRunning( EFalse )
	{
	}

CFolderRenamer::~CFolderRenamer()
	{
	Cancel();
	iRenamedFolders.ResetAndDestroy();
	}

void CFolderRenamer::ConstructL()
	{
	CActiveScheduler::Add(this);
	}
	
void CFolderRenamer::DoCancel()
	{	
	}
	

void CFolderRenamer::RunL()
	{
	switch (iState)
		{
		case ERenameStateIdle:
			{
			if (iRenamedFolders.Count() == 0)
				{
				iIsRunning = EFalse;
				return;
				}
			} //Flow to Renaming case -> no break
			
		case ERenameStateRenaming:
			{
			if (iRenamedFolders.Count())
				{
				iState = ERenameStateWaitingMdeRename;			
				CRenameItem &currItem = *iRenamedFolders[0];				
				CMdEHarvesterSession* session = iCFileEventHandlerAO.MdeHarvesterSession();
				session->ChangePath( currItem.OldName(), currItem.NewName(), iStatus );
				SetActive();
				}
			}
			break;
			
		case ERenameStateWaitingMdeRename:
			{
			CRenameItem *currItem = iRenamedFolders[0];
			iRenamedFolders.Remove(0);
			TRAP_IGNORE(currItem->HandleFileEventsL(iCFileEventHandlerAO));
			delete currItem;
			currItem = NULL;
			
			SetNextRequest(ERenameStateIdle);
			
			if (iRenamedFolders.Count() == 0)
				{
				iIsRunning = EFalse;
				iRenamedFolders.Compress();
				}
			}
			break;
			
		default:
			{
			WRITELOG("Unexpected state in FolderRenamer");
			SetNextRequest(ERenameStateIdle);
			}			
			break;
		}
	}

TInt CFolderRenamer::RunError( TInt aError )
    {
    WRITELOG("Unexpected error in FolderRenamer");
    SetNextRequest(ERenameStateIdle);
    return aError;
    }

void CFolderRenamer::RenameL(const TDesC &aOldName, const TDesC &aNewName)
	{
    //There comes multiple events for single rename, drop these 
	const TInt count = iRenamedFolders.Count();
	for (TInt i = 0; i < count; i++)
		{
		if ( iRenamedFolders[i]->OldName().CompareF(aOldName) == 0 && 
				iRenamedFolders[i]->NewName().CompareF(aNewName) == 0 )
			{
			return;
			}
		}
	
	CRenameItem *renameItem = CRenameItem::NewL(aOldName,aNewName);
    if( iRenamedFolders.Append( renameItem ) != KErrNone )
         {
         delete renameItem;
         renameItem = NULL;
         return;
         }
	
	iIsRunning = ETrue;
	//First item --> Kickstart
		
	if (iState == ERenameStateIdle)
		{
		SetNextRequest(ERenameStateRenaming);
		}
	}


void CFolderRenamer::SetNextRequest(TFolderRenameState aState)
	{
	iState = aState;	
	if (!IsActive())
		{
		SetActive();				
		TRequestStatus * status = &iStatus;
		User::RequestComplete(status,KErrNone);	
		}
	}


void CFolderRenamer::HandleFileEventL(TMdsFSPStatus &aEvent)
	{
	if (aEvent.iFileEventType == EMdsDirRenamed)
		{
		RenameL(aEvent.iFileName,aEvent.iNewFileName);
		return;
		}

	//Check whether new folder name of the rename events matches to received path. If it matches
    //queue events until folder is renamed, otherwise send notification to filemonitor immediatedly
    //has to be started from last event!
	if (iIsRunning)
		{
		const TInt count = iRenamedFolders.Count();	
		for (TInt i = count-1; i >= 0; i--)
			{
			if (aEvent.iFileName.FindF(iRenamedFolders[i]->NewName()) != KErrNotFound)
				{
				iRenamedFolders[i]->AddFileEvent(aEvent);
				return;
				}
			}		
		}
			
	iCFileEventHandlerAO.HandleNotificationL(aEvent);
	}

void CFolderRenamer::HandleFileEventsL( CArrayFixSeg< TMdsFSPStatus >* aEvents )
    {
    const TInt count( aEvents->Count() );  // Can hold only delete events
    
    if( iIsRunning )
        {
        for( TInt i( 0 ); i < count; i++ )
            {
            //Check whether new folder name of the rename events matches to received path. If it matches
            //queue events until folder is renamed, otherwise send notification to filemonitor immediatedly
            //has to be started from last event!
            const TInt count = iRenamedFolders.Count();   
            for (TInt i = count-1; i >= 0; i--)
                {
                if ((*aEvents)[i].iFileName.FindF(iRenamedFolders[i]->NewName()) != KErrNotFound)
                    {
                    iRenamedFolders[i]->AddFileEvent((*aEvents)[i]);
                    aEvents->Delete( i );
                    break;
                    } 
                }
            }
        aEvents->Compress();
        }
    
    if( aEvents->Count() )
        {
        iCFileEventHandlerAO.HandleMultideletionL(aEvents);
        }
    }
	


