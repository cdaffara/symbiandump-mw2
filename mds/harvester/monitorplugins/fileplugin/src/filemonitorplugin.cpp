/*
* Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies). 
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

#include "mdeharvestersession.h"
#include "filemonitorplugin.h"
#include "processoriginmapper.h"
#include "harvestercenreputil.h"

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::NewL()
// ---------------------------------------------------------------------------
//
CFileMonitorPlugin* CFileMonitorPlugin::NewL()
	{
	CFileMonitorPlugin* self = new (ELeave) CFileMonitorPlugin();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::~CFileMonitorPlugin()
// destruct
// ---------------------------------------------------------------------------
//
CFileMonitorPlugin::~CFileMonitorPlugin() 
	{
	delete iFileMonitor;
	iFileMonitor = NULL;
	}

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::StartMonitoring()
// ---------------------------------------------------------------------------
//
TBool CFileMonitorPlugin::StartMonitoring( MMonitorPluginObserver& aObserver,
		CMdESession* aMdEClient, CContextEngine* /*aCtxEngine*/,
		CHarvesterPluginFactory* aHarvesterPluginFactory )
	{
	_LIT( KDrive, "c:" );
	_LIT( KMonitorPath, "\\Data" );
	_LIT( KFullPath, "c:\\Data" );
		
	TInt err = KErrNone;
	TBool success = iFileMonitor->StartMonitoring( aObserver, aMdEClient, KFullPath,
			aHarvesterPluginFactory );
	
	if( success )
		{
		CHarvesterCenRepUtil* cenRepoUtil = NULL;
		TRAP( err, cenRepoUtil = CHarvesterCenRepUtil::NewL() );
		if( cenRepoUtil )
			{
			TRAP( err, cenRepoUtil->AddIgnorePathsToFspL( KDrive, KMonitorPath ));
			}
		delete cenRepoUtil;
		cenRepoUtil = NULL;
		}
	return success && err == KErrNone;
	}

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::StopMonitoring()
// ---------------------------------------------------------------------------
//
TBool CFileMonitorPlugin::StopMonitoring()
	{
	return iFileMonitor->StopMonitoring();
	}

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::ResumeMonitoring()
// ---------------------------------------------------------------------------
//
TBool CFileMonitorPlugin::ResumeMonitoring( MMonitorPluginObserver& /*aObserver*/,
		CMdESession* /*aMdEClient*/, CContextEngine* /*aCtxEngine*/,
		CHarvesterPluginFactory* /*aHarvesterPluginFactory*/ )
	{
	iFileMonitor->SetCachingStatus( EFalse );
	return ETrue;
	}

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::PauseMonitoring()
// ---------------------------------------------------------------------------
//
TBool CFileMonitorPlugin::PauseMonitoring()
	{
	iFileMonitor->SetCachingStatus( ETrue );
	return ETrue;
	}

// constructor support
// don't export these, because used only by functions in this DLL, eg our NewLC()
// ---------------------------------------------------------------------------
// CFileMonitorPlugin::CFileMonitorPlugin()
// first-phase C++ constructor
// ---------------------------------------------------------------------------
//
CFileMonitorPlugin::CFileMonitorPlugin() : iFileMonitor( NULL )
	{
	// No implementation required
	}

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::ConstructL()
// second-phase constructor
// ---------------------------------------------------------------------------
//
void CFileMonitorPlugin::ConstructL() 
	{
	iFileMonitor = CFileMonitorAO::NewL();
	}

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::RegisterProcessOriginL()
// Registers an origin mapping for a process
// in file monitor CProcessOriginMapper.
// ---------------------------------------------------------------------------
//
void CFileMonitorPlugin::RegisterProcessL( const TUid& aProcessId,
        const TOrigin& aOrigin )
    {
    iFileMonitor->Mapper().RegisterProcessL( aProcessId, aOrigin );
    }

// ---------------------------------------------------------------------------
// CFileMonitorPlugin::UnregisterProcessOriginL()
// Unregisters an origin mapping for a process.
// ---------------------------------------------------------------------------
//
void CFileMonitorPlugin::UnregisterProcessL( const TUid& aProcessId )
    {
    iFileMonitor->Mapper().UnregisterProcessL( aProcessId );
    }

