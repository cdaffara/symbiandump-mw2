/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This is NPP PSY Positioning Plug-in ( PSY ).
*
*/



// INCLUDE FILES
#include <e32std.h>            
#include <lbs/epos_mpositionerstatus.h>
#include <lbscommon.h>

#include "npppsystatushandler.h"
#include "npppsysingletonhandler.h"
#include "npppsylogging.h"
#include "npppsypanic.h"
#include "npppsy.hrh"


// ========================== == MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CNppPsyStatusHandler::CNppPsyStatusHandler
// C++default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CNppPsyStatusHandler::CNppPsyStatusHandler( MPositionerStatus& aStatusInterface )
    : iStatusInterface( aStatusInterface )
    {
    //Set to initialising
    iModuleStatus.SetDeviceStatus( TPositionModuleStatus::EDeviceInitialising );
    iModuleStatus.SetDataQualityStatus( TPositionModuleStatus::EDataQualityNormal );
    iStatusInterface.ReportStatus( iModuleStatus );
    }


// -----------------------------------------------------------------------------
// CNppPsyStatusHandler::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CNppPsyStatusHandler::ConstructL()
    {
    TRACESTRING( "CNppPsyStatusHandler::ConstructL start" )
    
    //Set to ready
    iModuleStatus.SetDeviceStatus( TPositionModuleStatus::EDeviceReady );
    iStatusInterface.ReportStatus( iModuleStatus );
    
    TRACESTRING( "CNppPsyStatusHandler::ConstructL end" )
    }


// -----------------------------------------------------------------------------
// CNppPsyStatusHandler::NewL
// -----------------------------------------------------------------------------
//
CNppPsyStatusHandler* CNppPsyStatusHandler::NewL( 
        MPositionerStatus& aStatusInterface )
    {
    CNppPsyStatusHandler* self = new( ELeave ) CNppPsyStatusHandler( 
        aStatusInterface );
    
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;
    }

    
// -----------------------------------------------------------------------------
// CNppPsyStatusHandler::~CNppPsyStatusHandler
// -----------------------------------------------------------------------------
//
CNppPsyStatusHandler::~CNppPsyStatusHandler()
    {
    //Set as inactive and data quality unknown
    iModuleStatus.SetDeviceStatus( TPositionModuleStatus::EDeviceInactive );
    iModuleStatus.SetDataQualityStatus( 
        TPositionModuleStatus::EDataQualityUnknown );
    iStatusInterface.ReportStatus( iModuleStatus );

    TRACESTRING( "CNppPsyStatusHandler:: destructed" )
    }

// -----------------------------------------------------------------------------
// CNppPsyStatusHandler::SetPsyListStatus
// -----------------------------------------------------------------------------
//
void CNppPsyStatusHandler::SetPsyListStatus( TBool aHealthy )
    {
    if ( aHealthy && iModuleStatus.DeviceStatus() == 
        TPositionModuleStatus::EDeviceError )
        {
        if ( iRequests > 0 )
            {
            iModuleStatus.SetDeviceStatus( 
                TPositionModuleStatus::EDeviceActive );
            }
        else
            {
            iModuleStatus.SetDeviceStatus( 
                TPositionModuleStatus::EDeviceReady );
            }
        iStatusInterface.ReportStatus( iModuleStatus );
        }
        
    if ( !aHealthy && iModuleStatus.DeviceStatus() != 
        TPositionModuleStatus::EDeviceError )
        {
        iModuleStatus.SetDeviceStatus( TPositionModuleStatus::EDeviceError );
        iStatusInterface.ReportStatus( iModuleStatus );
        }
    }

// -----------------------------------------------------------------------------
// CNppPsyStatusHandler::IncRequestCount
// -----------------------------------------------------------------------------
//
void CNppPsyStatusHandler::IncRequestCount()
    {
    ++iRequests;
    UpdateStatus();
    }
        
// -----------------------------------------------------------------------------
// CNppPsyStatusHandler::DecRequestCount
// -----------------------------------------------------------------------------
//
void CNppPsyStatusHandler::DecRequestCount()
    {
    if ( iRequests > 0 )
        {
        --iRequests;
        }
    UpdateStatus();
    }

// -----------------------------------------------------------------------------
// CNppPsyStatusHandler::UpdateStatus
// -----------------------------------------------------------------------------
//
void CNppPsyStatusHandler::UpdateStatus()
    {
    if ( iRequests > 0 && iModuleStatus.DeviceStatus() == 
        TPositionModuleStatus::EDeviceReady )
        {
        iModuleStatus.SetDeviceStatus( TPositionModuleStatus::EDeviceActive );
        iStatusInterface.ReportStatus( iModuleStatus );
        }
    
    if ( iRequests <= 0 && iModuleStatus.DeviceStatus() == 
        TPositionModuleStatus::EDeviceActive )
        {
        iModuleStatus.SetDeviceStatus( TPositionModuleStatus::EDeviceReady );
        iStatusInterface.ReportStatus( iModuleStatus );
        }
    }

//  End of File  
