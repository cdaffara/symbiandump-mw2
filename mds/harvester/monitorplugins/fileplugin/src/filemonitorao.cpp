/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Monitors file creations, modifications and deletions.*
*/


#include <mdeobject.h>

#include "harvestercommon.h"
#include "filemonitorao.h"
#include "harvesterlog.h"
#include "fsutil.h"
#include "mdeharvestersession.h"
#include "processoriginmapper.h"
#include "fileeventhandlerao.h"
#include "harvesterpluginfactory.h"

using namespace MdeConstants;

_LIT( KMdsFSPluginFile, "mdsfileserverplugin" );
_LIT( KMdsFSPluginName, "MdsFileServerPlugin" );

// ---------------------------------------------------------------------------
// CFileMonitorAO::NewL()
// ---------------------------------------------------------------------------
//
CFileMonitorAO* CFileMonitorAO::NewL()
    {
    WRITELOG( "CFileMonitorAO::NewL" );

    CFileMonitorAO* self = new (ELeave) CFileMonitorAO;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::ConstructL()
// ---------------------------------------------------------------------------
//
void CFileMonitorAO::ConstructL()
    {
    WRITELOG( "CFileMonitorAO::ConstructL" );
    
    CActiveScheduler::Add( this );
    iFsConnectOk = EFalse;
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::~CFileMonitorAO()
// ---------------------------------------------------------------------------
//
CFileMonitorAO::~CFileMonitorAO()
    {
    WRITELOG( "CFileMonitorAO::~CFileMonitorAO" );
    
    Cancel();
    
    StopMonitoring();

    delete iMdeSession;
    iMdeSession = NULL;
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::StartMonitoring()
// ---------------------------------------------------------------------------
//
TBool CFileMonitorAO::StartMonitoring( MMonitorPluginObserver& aObserver,
	CMdESession* aMdeSession, const TDesC& aPath, CHarvesterPluginFactory* aHarvesterPluginFactory )
    {
    WRITELOG( "CFileMonitorAO::StartMonitoring" );
    
    iObserver = &aObserver;
    iHarvesterPluginFactory = aHarvesterPluginFactory;
    TRAPD( err, iMdeSession = CMdEHarvesterSession::NewL( *aMdeSession ) );
    if ( err != KErrNone )
    	{
    	return EFalse;
    	}
    
    TRAP( err, iFileEventHandler = CFileEventHandlerAO::NewL( aObserver, iMdeSession, aHarvesterPluginFactory ));
    if ( err != KErrNone )
    	{
    	return EFalse;
    	}
    
    TInt error = iFs.Connect();
    if ( error != KErrNone )
    	{
    	iFs.Close();
    	return EFalse;
    	}
	iFsConnectOk = ETrue;

    // add mds fileserver plugin
    error = iFs.AddPlugin( KMdsFSPluginFile );
    if ( error != KErrAlreadyExists )
        {
        error = iFs.MountPlugin( KMdsFSPluginName );
        if ( error != KErrNone )
            {
            return EFalse;
            }
        }
    
    error = iEngine.Open( iFs, KMdsFSPluginPosition );
    if ( error != KErrNone )
        {
        return EFalse;
        }

    iEngine.Enable();
    iEngine.AddNotificationPath( aPath );
    
    StartNotify();
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::StartNotify()
// ---------------------------------------------------------------------------
//
void CFileMonitorAO::StartNotify()
    {
    WRITELOG( "CFileMonitorAO::StartNotify" );
    
    ResetMdsFSPStatus();
        
    iEngine.RegisterNotification( iStatusPckg, iStatus );
    
    SetActive();
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::StopMonitoring()
// ---------------------------------------------------------------------------
//
TBool CFileMonitorAO::StopMonitoring()
    {
    WRITELOG( "CFileMonitorAO::StopMonitoring" );

    if ( iFsConnectOk == EFalse )
    	{
    	return ETrue;
    	}
    
    Cancel();

    delete iFileEventHandler;
    iFileEventHandler = NULL;
    
    // remove mds fileserver plugin
    iEngine.Disable();
    iEngine.Close();
    
	iFs.DismountPlugin( KMdsFSPluginName );
	iFs.RemovePlugin( KMdsFSPluginName );
    
    iFs.Close();
    iFsConnectOk = EFalse;
    
    return ETrue;
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::RunL()
// From CActive
// ---------------------------------------------------------------------------
//
void CFileMonitorAO::RunL()
    {
    WRITELOG( "CFileMonitorAO::RunL" );
    
    User::LeaveIfError( iStatus.Int() );
    TMdsFSPStatus& status = iStatusPckg();

    iFileEventHandler->AddToQueueL(status);

    StartNotify();
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::ResetMdsFSPStatus()
// ---------------------------------------------------------------------------
//
void CFileMonitorAO::ResetMdsFSPStatus()
    {
    WRITELOG( "CFileMonitorAO::ResetMdsFSPStatus" );
    
    TMdsFSPStatus& status = iStatusPckg();
    
    status.iDriveNumber = 0;
    status.iFileEventType = EMdsFileUnknown;
    status.iFileName.Zero();
    status.iNewFileName.Zero();
    status.iProcessId = TUid::Null();
    status.iDriveMediaId = 0;
    }
    
// ---------------------------------------------------------------------------
// CFileMonitorAO::RunError()
// From CActive
// ---------------------------------------------------------------------------
//
TInt CFileMonitorAO::RunError( TInt aError )
    {
    WRITELOG1( "CFileMonitorAO::RunError %d", aError );

    if ( aError == KErrInUse )
        {
        iEngine.NotificationCancel();
        }
    else
        {
        StartNotify();
        }

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::DoCancel()
// From CActive
// ---------------------------------------------------------------------------
//
void CFileMonitorAO::DoCancel()
    {
    WRITELOG( "CFileMonitorAO::DoCancel" );
    iEngine.Disable();
    iEngine.NotificationCancel();
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::CFileMonitorAO()
// Constructor
// ---------------------------------------------------------------------------
//
CFileMonitorAO::CFileMonitorAO() : CActive( KHarvesterPriorityMonitorPlugin )
    {
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::Mapper()
// Returns a handle to CProcessOriginMapper.
// ---------------------------------------------------------------------------
//
CProcessOriginMapper& CFileMonitorAO::Mapper()
    {
    return iFileEventHandler->Mapper();
    }

// ---------------------------------------------------------------------------
// CFileMonitorAO::Mapper()
// Set event caching status
// ---------------------------------------------------------------------------
//
void CFileMonitorAO::SetCachingStatus( TBool aCachingStatus )
	{
	iFileEventHandler->SetCachingStatus( aCachingStatus );
	}

