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
* Description:  CMnrpNavigationService class implementation
*
*/


#include <e32base.h>
#include <eikenv.h>

#include <lbsposition.h>

#include <epos_cposlandmark.h>

#include "debug.h"
#include <mnnavigationservicebase.h>

#include "mnrpappui.h"
#include "mnrpnavimodel.h"
#include "mnrpnaviview.h"
#include "mnrpappserver.h"
#include "mnrpnavigationservice.h"


// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpNavigationService* CMnrpNavigationService::NewL()
    {
    CMnrpNavigationService* self = new (ELeave) CMnrpNavigationService();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpNavigationService::CMnrpNavigationService()
    {
    }
    
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpNavigationService::~CMnrpNavigationService()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpNavigationService::ConstructL()
    {
    BaseConstructL();
    }

// ---------------------------------------------------------------------------
// From class CMnNavigationServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpNavigationService::HandleNavigateToL(
    const CPosLandmark& aLandmark)
    {
    CMnrpAppServer* server = ( CMnrpAppServer* ) Server();
    CMnrpAppUi* ui = server->AppUi();
    ui->ShowNavigationL( aLandmark );
    }
        
// ---------------------------------------------------------------------------
// From class CMnServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpNavigationService::DoCancel()
    {
    }
