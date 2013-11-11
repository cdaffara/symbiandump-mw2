/*
* Copyright (c) 2004-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for accessing icon data in SIM.
*
*/


#include    "TUSatAPI.h"
#include    "MSatIconObserver.h"
#include    "CSatIconHandler.h"
#include    "SatLog.h"
#include    "EnginePanic.h"
#include    "csatsactivewrapper.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSatIconHandler::CSatIconHandler
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSatIconHandler::CSatIconHandler(
    MSatApi& aSatApi ) :
    CActive( EPriorityStandard ),
    MSatSIconAPI(),
    iSatApi( aSatApi ),
    iGetIconInfoActive( EFalse ),
    iGetIconInstanceActive( EFalse ),
    iGetClutActive( EFalse ),
    iSyncIconFetching( EFalse )
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::CSatIconHandler calling" )

    CActiveScheduler::Add( this );

    LOG( SIMPLE, "SATENGINE: CSatIconHandler::CSatIconHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIconHandler::~CSatIconHandler()
// Destructor
// -----------------------------------------------------------------------------
//
CSatIconHandler::~CSatIconHandler()
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::~CSatIconHandler calling" )

    iObserver = NULL;

    LOG( SIMPLE, "SATENGINE: CSatIconHandler::~CSatIconHandler exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIconHandler::GetIconInfo
// Gets the icon info from SIM through USAT-API.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatIconHandler::GetIconInfo(
    TUint8 aRecordNumber,
    RSat::TIcon& aIconEf,
    MSatIconObserver& aObserver )
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInfo calling" )

    iObserver = &aObserver;

    // Check is RSat connected
    if ( iSatApi.IsRSatConnected() )
        {
        LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInfo connected" )
        iSatApi.GetIcon( iStatus, aRecordNumber, aIconEf );
        iGetIconInfoActive = ETrue;
        SetActive();
        }
    else
        {
        LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInfo Not connected" )
        iObserver->NotifyFailure( KErrDisconnected );
        }

    LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInfo exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIconHandler::GetIconInstance
// Gets the icon data from SIM through USAT-API.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatIconHandler::GetIconInstance(
    const RSat::TInstanceInfoV3& aInstanceInfo,
    TDes8& aInstance,
    MSatIconObserver& aObserver )
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInstance calling" )

    iObserver = &aObserver;

    // Check is RSat connected
    if ( iSatApi.IsRSatConnected() )
        {
        LOG( SIMPLE,
            "SATENGINE: CSatIconHandler::GetIconInstance connected" )
        #ifndef __WINS__
            RSat::TInstanceInfoV3Pckg instanceInfoPckg( aInstanceInfo );
            if ( !iSyncIconFetching )
                {
                LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInstance \
                     iSyncIconFetching false" )
                iSatApi.GetImageInstance( 
                    iStatus, instanceInfoPckg, aInstance );
                iGetIconInstanceActive = ETrue;
                SetActive();
                }
            else
                {
                CSatSActiveWrapper* wrapper = NULL;
                wrapper = new CSatSActiveWrapper();
                TInt error( KErrNoMemory );
                if ( wrapper )
                    {
                    LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInstance\
                    wrapper" )
                    iSatApi.GetImageInstance( wrapper->RequestStatus(),
                        instanceInfoPckg, aInstance );
                    error = wrapper->SetActiveAndWait();
                    delete wrapper;
                    wrapper = NULL;
                    }

                LOG2(
                    SIMPLE,
                    "SATENGINE: CSatIconHandler::GetIconInstance iStatus: %i",
                     error )
                if ( KErrNone != error )
                    {
                    iObserver->NotifyFailure( error );
                    }
                else
                    {
                    TRAP_IGNORE( iObserver->NotifyGetIconInstanceL(); );
                    }
                }
        #else
            CSatSActiveWrapper* wrapper = NULL;
            wrapper = new CSatSActiveWrapper();
            TInt error( KErrNoMemory );
            if ( wrapper )
                {
                LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInstance\
                wrapper true" )
                RSat::TInstanceInfoV3Pckg instanceInfoPckg( aInstanceInfo );
                iSatApi.GetImageInstance( wrapper->RequestStatus(),
                    instanceInfoPckg, aInstance );
                error = wrapper->SetActiveAndWait();
                delete wrapper;
                wrapper = NULL;
                }

            LOG2(
                SIMPLE,
                "SATENGINE: CSatIconHandler::GetIconInstance iStatus: %i",
                 error )
            if ( KErrNone != error )
                {
                iObserver->NotifyFailure( error );
                }
            else
                {
                TRAP_IGNORE( iObserver->NotifyGetIconInstanceL(); );
                }
        #endif

        }
    else
        {
        LOG( SIMPLE,
            "SATENGINE: CSatIconHandler::GetIconInstance not connected" )
        iObserver->NotifyFailure( KErrDisconnected );
        }

    LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetIconInstance exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIconHandler::GetClut
// Gets the Clut from SIM through USAT-API.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatIconHandler::GetClut(
    TUint aInstanceNumber,
    TUint aOffset,
    TDes8& aClut,
    MSatIconObserver& aObserver )
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetClut calling" )

    iObserver = &aObserver;
    // Check is RSat connected
    if ( iSatApi.IsRSatConnected() )
        {
        LOG( SIMPLE,
            "SATENGINE: CSatIconHandler::GetClut connected" )
        #ifndef __WINS__
            if ( !iSyncIconFetching )
                {
                LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetClut \
                     iSyncIconFetching false" )
                iSatApi.GetClut( iStatus, aInstanceNumber, aOffset, aClut );
                iGetClutActive = ETrue;
                SetActive();
                }
            else
                {
                CSatSActiveWrapper* wrapper = NULL;
                wrapper = new CSatSActiveWrapper();
                TInt error( KErrNoMemory );
                if ( wrapper )
                    {
                    LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetClut\
                    wrapper" )
                    iSatApi.GetClut( wrapper->RequestStatus(),
                        aInstanceNumber, aOffset, aClut );
                    error = wrapper->SetActiveAndWait();
                    delete wrapper;
                    wrapper = NULL;
                    }

                LOG2(
                    SIMPLE,
                    "SATENGINE: CSatIconHandler::GetClut iStatus: %i", error )
                if ( KErrNone != error )
                    {
                    iObserver->NotifyFailure( error );
                    }
                else
                    {
                    TRAP_IGNORE( iObserver->NotifyGetClutL(); );
                    }
                }
        #else
            CSatSActiveWrapper* wrapper = NULL;
            wrapper = new CSatSActiveWrapper();
            TInt error( KErrNoMemory );
            if ( wrapper )
                {
                LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetClut\
                wrapper true" )
                iSatApi.GetClut( wrapper->RequestStatus(), aInstanceNumber,
                    aOffset, aClut );
                error = wrapper->SetActiveAndWait();
                delete wrapper;
                wrapper = NULL;
                }

            LOG2(
                SIMPLE,
                "SATENGINE: CSatIconHandler::GetClut iStatus: %i", error )
            if ( KErrNone != error )
                {
                iObserver->NotifyFailure( error );
                }
            else
                {
                TRAP_IGNORE( iObserver->NotifyGetClutL(); );
                }
        #endif

        }
    else
        {
        LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetClut Not connected" )
        iObserver->NotifyFailure( KErrDisconnected );
        }

    LOG( SIMPLE, "SATENGINE: CSatIconHandler::GetClut exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIconHandler::DoCancel
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatIconHandler::DoCancel()
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::DoCancel calling" )

    // Complete the request with cancel code.
    iObserver->NotifyFailure( KErrCancel );

    LOG( SIMPLE, "SATENGINE: CSatIconHandler::DoCancel exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIconHandler::RunL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CSatIconHandler::RunL()
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::RunL calling" )
    LOG2( SIMPLE,
            "SATENGINE: CSatIconHandler::RunL error: %i", iStatus.Int() )
    if ( KErrNone == iStatus.Int() )
        {
        if ( iGetIconInfoActive )
            {
            LOG( NORMAL, "SATENGINE:   iGetIconInfoActive" )
            iGetIconInfoActive = EFalse;
            iObserver->NotifyGetIconInfo();
            }

        else if ( iGetIconInstanceActive )
            {
            LOG( NORMAL, "SATENGINE:   iGetIconInstanceActive" )
            iGetIconInstanceActive = EFalse;
            iObserver->NotifyGetIconInstanceL();
            }

        else if ( iGetClutActive )
            {
            LOG( NORMAL, "SATENGINE:   iGetClutActive" )
            iGetClutActive = EFalse;
            iObserver->NotifyGetClutL();
            }

        else
            {
            LOG( SIMPLE, "SATENGINE:   Unexpected RunL" )
            PanicSatEngine( ESatSBadRequest );
            }
        }
    else
        {        
        iObserver->NotifyFailure( iStatus.Int() );
        }

    LOG( SIMPLE, "SATENGINE: CSatIconHandler::RunL exiting" )
    }

// -----------------------------------------------------------------------------
// CSatIconHandler::RunError
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSatIconHandler::RunError( TInt aError )
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::RunError calling" )

    // If leave happened before completion of iMessage, we do it now
    TInt err = iObserver->CompleteError( aError );
    // If completed succesfully, aError is sent forward in completion.
    // If not, we should return aError forward
    if ( KErrNone != err )
        {
        LOG( SIMPLE, "SATENGINE: CSatIconHandler::RunError KErrNone != err" )
        err = aError;
        }

    LOG2( SIMPLE, "SATENGINE: CSatIconHandler::RunError exiting, err: %i",
          err )
    return err;
    }
// -----------------------------------------------------------------------------
// Set a mode to determine whether to fetch icons from NAA syncronously or 
// asynchronously.
// -----------------------------------------------------------------------------
//
void CSatIconHandler::SetIconFetchingMode( TBool aMode )
    {
    LOG( SIMPLE, "SATENGINE: CSatIconHandler::SetIconFetchingMode calling" )

    iSyncIconFetching = aMode;

    LOG( SIMPLE, "SATENGINE: CSatIconHandler::SetIconFetchingMode exiting" )
    }
