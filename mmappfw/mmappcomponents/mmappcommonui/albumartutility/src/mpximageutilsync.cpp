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
* Description:  Implementation of CMPXImageUtilSync.
*
*/


// INCLUDE FILES
#include <e32std.h>
#include <mpxlog.h>

#include "mpximageutil.h"
#include "mpximageutilsync.h"

// CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CMPXImageUtilSync* CMPXImageUtilSync::NewL()
    {
    MPX_FUNC("CMPXImageUtil::NewL");
    CMPXImageUtilSync* self = new ( ELeave ) CMPXImageUtilSync();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// -----------------------------------------------------------------------------
// C++ constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
CMPXImageUtilSync::CMPXImageUtilSync() : 
    CActive( EPriorityStandard )  
    {
    MPX_FUNC( "CMPXImageUtilSync::CMPXImageUtilSync" );
    }

// -----------------------------------------------------------------------------
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CMPXImageUtilSync::ConstructL()
    {
    MPX_FUNC( "CMPXImageUtilSync::ConstructL" );
    CActiveScheduler::Add(this);

    iImageUtil = CMPXImageUtil::NewL();
    iActiveSchedulerWait = new ( ELeave ) CActiveSchedulerWait();
    }

// -----------------------------------------------------------------------------
// Destructor
// -----------------------------------------------------------------------------
//
CMPXImageUtilSync::~CMPXImageUtilSync()
    {
    MPX_FUNC( "CMPXImageUtilSync::~CMPXImageUtilSync" );
    delete iImageUtil;
    delete iActiveSchedulerWait;
    }

// -----------------------------------------------------------------------------
// Decodes an JPG synchronously
// -----------------------------------------------------------------------------
//
CFbsBitmap* CMPXImageUtilSync::DecodeL(
    const TDesC8& aJPGData,
    TSize aSize,
    TDisplayMode aDisplayMode /* = EColor64K */)
    {
    MPX_FUNC( "CMPXImageUtilSync::DecodeL" );

    iBitmap = new ( ELeave ) CFbsBitmap();
    
    iImageUtil->Decode( iStatus, aJPGData, *iBitmap, aSize, aDisplayMode );
    SetActive();

    // block waiting for completion
    iActiveSchedulerWait->Start();
    User::LeaveIfError( iError );
    
    // transfer the ownership
    CFbsBitmap* bmp = iBitmap;
    
    iBitmap = NULL;
    return bmp;
    }

// -----------------------------------------------------------------------------
// Decodes an JPG synchronously
// -----------------------------------------------------------------------------
//
CFbsBitmap* CMPXImageUtilSync::DecodeL(
    const TDesC& aSourceJPGFile, 
    TSize aSize, 
    TDisplayMode aDisplayMode /* = EColor64K */)
    {
    MPX_FUNC( "CMPXImageUtilSync::DecodeL" );

    iBitmap = new ( ELeave ) CFbsBitmap();

    iImageUtil->Decode( iStatus, aSourceJPGFile, *iBitmap, aSize, aDisplayMode );
    SetActive();

    // block waiting for completion
    iActiveSchedulerWait->Start();
    User::LeaveIfError( iError );
    
    // transfer the ownership
    CFbsBitmap* bmp = iBitmap;

    iBitmap = NULL;
    return bmp;
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtilSync::EncodeL
// -----------------------------------------------------------------------------
//
HBufC8* CMPXImageUtilSync::EncodeL(
    const CFbsBitmap& aSourceBMP)
    {
    MPX_FUNC( "CMPXImageUtilSync::EncodeL" );

    iImageUtil->Encode( iStatus, aSourceBMP, iData );
    SetActive();

    // block waiting for completion
    iActiveSchedulerWait->Start();
    User::LeaveIfError( iError );

    // transfer the ownership
    HBufC8* dataTmp = iData;
    iData = NULL;
    return dataTmp;
    }
    
// -----------------------------------------------------------------------------
// CMPXImageUtilSync::ScaleL
// -----------------------------------------------------------------------------
//
CFbsBitmap* CMPXImageUtilSync::ScaleL(
    CFbsBitmap& aSourceBMP, 
    TSize aSize, 
    TDisplayMode aDisplayMode /* = EColor64K */)
    {
    MPX_FUNC( "CMPXImageUtilSync::ScaleL" );

    iBitmap = new ( ELeave ) CFbsBitmap();
    
    iImageUtil->Scale( iStatus, aSourceBMP, *iBitmap, aSize, aDisplayMode );
    SetActive();

    // block waiting for completion
    iActiveSchedulerWait->Start();
    User::LeaveIfError( iError );

    // transfer the ownership
    CFbsBitmap* bmp = iBitmap;

    iBitmap = NULL;
    return bmp;
    }   

// -----------------------------------------------------------------------------
// CMPXImageUtilSync::DoCancel
// -----------------------------------------------------------------------------
//
void CMPXImageUtilSync::DoCancel()
    {
    MPX_FUNC( "CMPXImageUtilSync::DoCancel" );
    iImageUtil->CancelRequest();
    }

// -----------------------------------------------------------------------------
// CMPXImageUtilSync::RunL
// -----------------------------------------------------------------------------
//
void CMPXImageUtilSync::RunL()
    {
    MPX_FUNC( "CMPXImageUtilSync::RunL" );
    iActiveSchedulerWait->AsyncStop();
    }

// -----------------------------------------------------------------------------
// CMPXImageUtilSync::RunError
// -----------------------------------------------------------------------------
//
TInt CMPXImageUtilSync::RunError(
    TInt aError)
    {
    MPX_FUNC( "CMPXImageUtilSync::RunL" );

    iError = aError;
    iActiveSchedulerWait->AsyncStop();

    return KErrNone;
    }

//  End of File
