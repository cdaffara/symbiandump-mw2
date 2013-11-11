/*
* Copyright (c) 2006, 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of Status Manager
*
*/



#include <e32base.h>
#include <lbtstatuspskeys.h>
#include "lbtstatusmanager.h"
#include "lbtlogger.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// CLbtStatusManager::NewL
// CLbtStatusManager instantiation method.
// -----------------------------------------------------------------------------
//
CLbtStatusManager* CLbtStatusManager::NewL()
    {
    CLbtStatusManager* self = new ( ELeave ) CLbtStatusManager;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }
	

// -----------------------------------------------------------------------------
// CLbtStatusManager::~CLbtStatusManager
// Destructor
// -----------------------------------------------------------------------------
//
CLbtStatusManager::~CLbtStatusManager()
    {
    iStatusInfo.Close();
    }
	
	
// -----------------------------------------------------------------------------
// CLbtStatusManager::ReportLocationAcquisitionStatus
// 
// -----------------------------------------------------------------------------
//
void CLbtStatusManager::ReportLocationAcquisitionStatus( 
                            TAcquisitionStatus aStatus )
    {
    TInt msg;
    if( KErrNone == aStatus )
        {
        msg = iStatusInfo.Set( ELbtLocationTriggeringSupervisionSuccessful );
        }
    else
        {
        msg = iStatusInfo.Set( ELbtLocationTriggeringSupervisionFailed );
        }
        
    iLastLocAcqStatus = aStatus;        
    if( KErrNone != msg )
        {
        LOG1("Error updating status info:%d",msg);
        }
    }


// -----------------------------------------------------------------------------
// CLbtStatusManager::ReportLocationAcquisitionStatus
// 
// -----------------------------------------------------------------------------
//
TBool CLbtStatusManager::LocationAcqSuccess()
    {
    if( KErrNone == iLastLocAcqStatus )
        {
        return ETrue;
        }
    return EFalse;
    }


// -----------------------------------------------------------------------------
// CLbtStatusManager::CLbtStatusManager
// Constructor
// -----------------------------------------------------------------------------
//
CLbtStatusManager::CLbtStatusManager()
    {
    }
	

// -----------------------------------------------------------------------------
// CLbtStatusManager::ConstructL
// 2nd phase constructor
// -----------------------------------------------------------------------------
//
void CLbtStatusManager::ConstructL()
    {
    User::LeaveIfError( iStatusInfo.Attach( 
                KPSUidLbtStatusInformation,
                KLbtLocationTriggeringSupervisionStatus,
                EOwnerThread ) );
    }
