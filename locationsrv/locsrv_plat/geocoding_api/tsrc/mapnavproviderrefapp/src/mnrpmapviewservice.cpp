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
* Description:  CMnrpMapViewService class implementation
*
*/


#include <e32base.h>
#include <eikenv.h>

#include <lbsposition.h>

#include <epos_cposlandmark.h>
#include <epos_cposlandmarkdatabase.h>

#include "debug.h"
#include <mnmapviewservicebase.h>

#include "mnrpappui.h"
#include "mnrpmapview.h"
#include "mnrpengine.h"
#include "mnrpdocument.h"
#include "mnrpappserver.h"

#include "mnrpmapviewservice.h"

// ============================ MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpMapViewService* CMnrpMapViewService::NewL()
    {
    CMnrpMapViewService* self = new (ELeave) CMnrpMapViewService();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpMapViewService::CMnrpMapViewService()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpMapViewService::~CMnrpMapViewService()
    {
    if ( iSelecting )
        {
        DoCancel();
        CompleteRequest( KErrCancel );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::ConstructL()
    {
    BaseConstructL();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpAppUi* CMnrpMapViewService::AppUi()
    {
    CMnrpAppServer* server = ( CMnrpAppServer* ) Server();
    if ( server )
        {
        return server->AppUi();
        }
    return NULL;
    }

// ---------------------------------------------------------------------------
// From class CMnMapViewServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::HandleShowMapL()
    {
    LOG("MnRefProvider::HandleShowMapL in");

    /** This implementation cannot show map if selection is active */
    if ( iSelecting )
        {
        User::Leave( KErrInUse );
        }

    if ( AppUi() )
        {
        AppUi()->ShowMapL( *this );
        }

    LOG("MnRefProvider::HandleShowMapL out");
    }

// ---------------------------------------------------------------------------
//  From class CMnMapViewServiceBase
//
//  This callback differs from HandleShowMapL in a way that central point
//  setting is ignored and current location is used instead as central point.
//
//  The implementation is the example of how to reject unsupported requests 
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::HandleShowCurrentLocationL()
    {
    User::Leave( KErrNotSupported );
    }

// ---------------------------------------------------------------------------
//  From class CMnMapViewServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::HandleSelectFromMapL()
    {
    LOG("MnRefProvider::HandleSelectFromMapL in");

    /** Only one selection must be active */
    if ( iSelecting )
        {
        User::Leave( KErrInUse );
        }

    if ( AppUi() )
        {
        iSelecting = ETrue;
        AppUi()->StartSelectionL( *this, *this );
        }

    LOG("MnRefProvider::HandleSelectFromMapL out");
    }

// ---------------------------------------------------------------------------
//  From class CMnServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::DoCancel()
    {
    if ( iSelecting )
        {
        iSelecting = EFalse;
        StopSelection();
        }
    }

// ---------------------------------------------------------------------------
//  From class MMapSelectionCallback
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::HandleSelectionErrorL( TInt aError )
    {
    __ASSERT_DEBUG( iSelecting, Panic( KErrGeneral ) );

    iSelecting = EFalse;
    // selection view dismissed
    CompleteRequest( aError );
    }

// ---------------------------------------------------------------------------
//  From class MMapSelectionCallback
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::HandleSelectionL( const TCoordinate& aSelection )
    {
    __ASSERT_DEBUG( iSelecting, Panic( KErrGeneral ) );

    iSelecting = EFalse;
    TInt err = KErrNone;
    TRAP( err, CompleteSelectionL( aSelection ) );
    if ( err )
        {
        CompleteRequest( err );
        }
    StopSelection();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::CompleteSelectionL( const TCoordinate& aSelection )
    {
    TLocality loc( aSelection, 0 );
    CPosLandmark* lm = CPosLandmark::NewLC();
    lm->SetPositionL( loc );

    // send landmark to client
    CompleteSelectionRequestL( *lm );

    CleanupStack::PopAndDestroy( lm );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::StopSelection()
    {
    if ( AppUi() )
        {
        AppUi()->StopSelection();
        }
    }
    
// ---------------------------------------------------------------------------
//  From class MMapSelectionCallback
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::HandleSelectionL( TInt aLandmarkIndex )
    {
    __ASSERT_DEBUG( iSelecting, Panic( KErrGeneral ) );

    iSelecting = EFalse;
    CompleteSelectionRequest( aLandmarkIndex );
    StopSelection();
    }
    
// ---------------------------------------------------------------------------
//  From class MMapSelectionCallback
// ---------------------------------------------------------------------------
//
void CMnrpMapViewService::HandleSelectionL( TPosLmItemId aLandmarkId, TInt aDbIndex )
    {
    __ASSERT_DEBUG( iSelecting, Panic( KErrGeneral ) );

    iSelecting = EFalse;
    CompleteSelectionRequest( aLandmarkId, aDbIndex );
    StopSelection();
    }
