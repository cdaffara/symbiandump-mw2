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
* Description:  CMnrpAppServerBack class implementation
*
*/


#include <mnappserviceuids.hrh>
#include "debug.h"

#include <lbsposition.h>
#include <epos_cposlandmark.h>

#include "mnrpgeocodingservice.h"

#ifdef RD_MAPNAV_BITMAP_ACCESS
#include "mnrpmapimageservice.h"
#endif

#include "mnrpappuiback.h"
#include "mnrpengine.h"
#include "mnrpappserverback.h"

// ============================ LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppServerBack::~CMnrpAppServerBack()
    {
    delete iEngine;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppServerBack* CMnrpAppServerBack::NewL()
    {
    CMnrpAppServerBack* self = new (ELeave) CMnrpAppServerBack();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpAppServerBack::ConstructL()
    {
    iEngine = CMnrpEngine::NewL();
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CApaAppServiceBase* CMnrpAppServerBack::CreateServiceL(TUid aServiceType) const
    {
    LOG1("MnRefProviderBack::CreateServiceL, svcUid = %X", aServiceType.iUid);
    switch ( aServiceType.iUid )
        {
        case KMnAppGeocodingService:
            return CreateGeocodingServiceL();

#ifdef RD_MAPNAV_BITMAP_ACCESS
        case KMnAppMapImageService:
            return CreateMapImageServiceL();
#endif
        default:
            return CAknAppServer::CreateServiceL( aServiceType );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpGeocodingService* CMnrpAppServerBack::CreateGeocodingServiceL() const
    {
    LOG( "MnRefProviderBack: CreateGeocodingServiceL" );
    CMnrpGeocodingService* svc = CMnrpGeocodingService::NewL( *iEngine );
    return svc;
    }

#ifdef RD_MAPNAV_BITMAP_ACCESS
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapImageService* CMnrpAppServerBack::CreateMapImageServiceL() const
    {
    LOG( "MnRefProviderBack: CreateGeocodingServiceL" );
    CMnrpMapImageService* svc = CMnrpMapImageService::NewL( *iEngine );
    return svc;
    }
#endif
