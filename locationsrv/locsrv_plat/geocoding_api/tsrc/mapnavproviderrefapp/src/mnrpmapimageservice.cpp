/*
* Copyright (c) 2006-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ÑMnrpMapImageService class implementation
*
*/


#include <e32base.h>
#include <e32math.h>
#include <e32property.h>
#include <eikenv.h>
#include <gdi.h>

#include <lbsposition.h>

#include <epos_cposlandmark.h>
#include <epos_poslandmarkserialization.h>

#include <mnmapimage.h>
#include <mnmapimageparams.h>
#include <mnmapimageservicebase.h>

#include "debug.h"
#include "mnrpengine.h"
#include "mnrpappuiback.h"
#include "mnrpappserverback.h"
#include "mnrpmapmodel.h"
#include "mnrpmapimageservice.h"

const TInt KTextFontHeightInTwips = 100;
const TInt KItemFontHeightInTwips = 120;

const TTimeIntervalMicroSeconds32 KProcessingDelay = 200 * 1000;

// ======== LOCAL FUNCTIONS ========

extern void CoordinateToString( TReal aCoord, TDes& aStr );

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpMapImageService* CMnrpMapImageService::NewL( CMnrpEngine& aEngine )
    {
    CMnrpMapImageService* self = new (ELeave) CMnrpMapImageService( aEngine );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpMapImageService::CMnrpMapImageService( CMnrpEngine& aEngine )
  : iEngine( aEngine )
    {
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
CMnrpMapImageService::~CMnrpMapImageService()
    {
	if ( iProcessor )
		{
	    iProcessor->Cancel();
	    delete iProcessor;
		}
    delete iModel;
    DestroyFonts();
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapImageService::ConstructL()
    {
    BaseConstructL();

    iEikon = CEikonEnv::Static();
    CreateFonts();

    iModel = CMnrpMapModel::NewL( iEngine );
    iProcessor = CPeriodic::NewL( CActive::EPriorityIdle );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapImageService::LeaveIfBusyL()
    {
    if ( iProcessor->IsActive() )
        {
        LOG("CMnRefProviderMapViewService::LeaveIfBusyL BUSY");
        User::Leave( KErrInUse );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapImageService::StartProcessingL()
    {
    iProcessor->Start( KProcessingDelay, KProcessingDelay,
        TCallBack( CMnrpMapImageService::MapImageProcessingCallBack, this ) );
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
TInt CMnrpMapImageService::MapImageProcessingCallBack( TAny* aPtr )
    {
    CMnrpMapImageService* self =
        static_cast<CMnrpMapImageService*> ( aPtr );

    TRAP_IGNORE( self->HandleMapImageCompletedL() );
    return EFalse;
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapImageService::HandleMapImageCompletedL()
    {
    iProcessor->Cancel();
    TInt err = KErrNone;
    TRAP( err, FinishProcessingL() );
    if ( err )
        {
        CompleteRequest( err );
        }
    }

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapImageService::RenderL()
    {
    TMnMapImageParams& params = MapImageParams();

    if ( params.Direction() != 0 ||
         params.ImageType() != TMnMapImageParams::ETypeVectorMap )
        {
        User::Leave( KErrNotSupported );
        }

    CFbsBitmap& bitmap = TargetBitmap();

    iModel->SetScreenSize( params.Size() );
    iModel->UpdateModelL( *this );
    iModel->RenderL( bitmap, TRect( params.Size() ), iTextFont, iItemFont );

    // complete request
    params.SetProjectionId(0);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapImageService::CreateFonts()
	{
    CGraphicsDevice* screenDevice = iEikon->ScreenDevice();

    const CFont* denseFont = iEikon->DenseFont();
    TFontSpec fs = denseFont->FontSpecInTwips();

    fs.iHeight = KTextFontHeightInTwips;
    screenDevice->GetNearestFontInTwips( iTextFont, fs );

    fs.iHeight = KItemFontHeightInTwips;
    screenDevice->GetNearestFontInTwips( iItemFont, fs );
	}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
void CMnrpMapImageService::DestroyFonts()
	{
	CGraphicsDevice* screenDevice = iEikon->ScreenDevice();

    screenDevice->ReleaseFont( iItemFont );
    screenDevice->ReleaseFont( iTextFont );
	}

// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//
void CMnrpMapImageService::FinishProcessingL()
    {
    LOG1("FinishProcessingL in, cursvc %d", iCurrentService);
    TCoordinate coord;
    switch ( iCurrentService )
        {
        case ERender:
            {
            RenderL();
            }
            break;

        default:
            User::Leave( KErrGeneral );
        }

    iCurrentService = ENone;
    CompleteRendering();
    LOG("FinishProcessingL out");
    }

// ---------------------------------------------------------------------------
// From class CMnMapImageServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpMapImageService::HandleRenderingL()
    {
    LeaveIfBusyL();

    iCurrentService = ERender;
    StartProcessingL();
    }

// ---------------------------------------------------------------------------
// From class CMnServiceBase
// ---------------------------------------------------------------------------
//
void CMnrpMapImageService::DoCancel()
    {
    iProcessor->Cancel();
    iCurrentService = ENone;
    }

