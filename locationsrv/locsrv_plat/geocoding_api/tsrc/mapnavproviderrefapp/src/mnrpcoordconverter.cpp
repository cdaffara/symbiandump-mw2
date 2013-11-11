/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CMnrpCoordConverter implementation
*
*/


#include <e32base.h>

#include "debug.h"
#include "mnrpengine.h"
#include "mnrpmapmodel.h"
#include "mnrpcoordconverter.h"

// ========================== MEMBER FUNCTIONS ===============================

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpCoordConverter* CMnrpCoordConverter::NewL()
    {
    CMnrpCoordConverter* self = new (ELeave) CMnrpCoordConverter;
    CleanupStack::PushL( self );

    self->iEngine = CMnrpEngine::NewL();
    self->iModel = CMnrpMapModel::NewL( *self->iEngine );

    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpCoordConverter::CMnrpCoordConverter()
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpCoordConverter::~CMnrpCoordConverter()
    {
    delete iModel;
    delete iEngine;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CMnrpCoordConverter::GetImageCoordinate(
    const TCoordinate& aWorldCoordinate,
    TPoint& aImagePoint )
    {
    const TMnMapImageParams& params = MapImageParams();

    if ( params.ImageType() == TMnMapImageParams::ETypeSatelliteImage )
        {
        return KErrArgument;
        }

    iModel->UpdateModel( params );

    aImagePoint = iModel->AbsoluteToScreen( aWorldCoordinate );
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CMnrpCoordConverter::GetWorldCoordinate(
    const TPoint& aImagePoint,
    TCoordinate& aWorldCoordinate )
    {
    const TMnMapImageParams& params = MapImageParams();

    if ( params.ImageType() == TMnMapImageParams::ETypeSatelliteImage )
        {
        return KErrArgument;
        }

    iModel->UpdateModel( params );

    aWorldCoordinate = iModel->ScreenToAbsolute( aImagePoint  );
    return KErrNone;
    }
