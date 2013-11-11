
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
* Description:  Location Trigger Manager monitoring
*
*/

#include "lbttriggermanager.h"

_LIT(KPluginSettingRAMFolder, "\\private\\10282BC4\\Settings\\200267F3\\");

// ---------------------------------------------------------------------------
// CLocationTriggerManager::CLocationTriggerManager
// ---------------------------------------------------------------------------
//
CLocationTriggerManager::CLocationTriggerManager() :
CActive( EPriorityStandard )
    {
        CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// CLocationTriggerManager::~CLocationTriggerManager
// ---------------------------------------------------------------------------
//
CLocationTriggerManager::~CLocationTriggerManager()
    {
    CActive::Cancel();

    iLbtManager.Close();
    iLbtServer.Close();
    }

// ---------------------------------------------------------------------------
// CLocationTriggerManager::NewLC
// ---------------------------------------------------------------------------
//
CLocationTriggerManager* CLocationTriggerManager::NewLC( )
    {
    CLocationTriggerManager* self = new (ELeave) CLocationTriggerManager();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CLocationTriggerManager::NewL
// ---------------------------------------------------------------------------
//
CLocationTriggerManager* CLocationTriggerManager::NewL()
    {
    CLocationTriggerManager* self = CLocationTriggerManager::NewLC( );
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CLocationTriggerManager::ConstructL
// ---------------------------------------------------------------------------
//
void CLocationTriggerManager::ConstructL()
    {
    User::LeaveIfError( iLbtServer.Connect() );
    User::LeaveIfError( iLbtManager.Open(iLbtServer) );
    }


// ---------------------------------------------------------------------------
// CLocationTriggerManager::RunL
// ---------------------------------------------------------------------------
//
void CLocationTriggerManager::RunL()
    {
    if(iStatus.Int()!=KErrNone)
        {    
        return;
        }

    //Get triggering system management settings
    TLbtTriggeringSystemStatus triggeringSystemStatus;
    iLbtManager.GetTriggeringSystemStatusL(triggeringSystemStatus);
    
    if ( triggeringSystemStatus.iLocationAcquisitionStatus == ELocationAcquisitionInactive )
        {    
        // Delete all setting files belonging to LBT Context Source plugin
        RFs fsSession;
        CleanupClosePushL( fsSession );
        User::LeaveIfError(fsSession.Connect()); 
        CFileMan* fileMan = CFileMan::NewL(fsSession);
        CleanupStack::PushL(fileMan);
        // Delete settings from RAM
        TInt err = fileMan->RmDir(KPluginSettingRAMFolder);

        CleanupStack::PopAndDestroy(fileMan); 
        fsSession.Close();
        CleanupStack::PopAndDestroy( &fsSession );
        }
        
    iLbtManager.NotifyTriggeringSystemStatusChange(iStatus);
    SetActive();
    
    }

// ---------------------------------------------------------------------------
// CLocationTriggerManager::RunError
// ---------------------------------------------------------------------------
//
TInt CLocationTriggerManager::RunError( TInt aError )
    {
    if( aError != KErrNone )
        {
        iLbtManager.NotifyTriggeringSystemStatusChange(iStatus);
        SetActive();
        return aError;
        }
    
    iLbtManager.NotifyTriggeringSystemStatusChange(iStatus);
    SetActive();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CLocationTriggerManager::DoCancel
// ---------------------------------------------------------------------------
//
void CLocationTriggerManager::DoCancel()
    {    
    iLbtManager.CancelNotifyTriggeringSystemStatusChange();
    }
  
// ---------------------------------------------------------------------------
// CLocationTriggerManager::StartManagerObserverL
// ---------------------------------------------------------------------------
//
void CLocationTriggerManager::StartManagerObserver()
    {    
        iLbtManager.NotifyTriggeringSystemStatusChange(iStatus);
        SetActive();
    }
