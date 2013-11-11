/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Implementation of CVmbxSatRefreshObserver class.
*
*/


// INCLUDE FILES
#include "vmbxlogger.h"
#include "vmbxsatrefreshobserver.h"

// CONSTANTS
const TInt KVmbxFile = 0x6f17;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CVmbxSatRefreshObserver::CVmbxSatRefreshObserver
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CVmbxSatRefreshObserver::CVmbxSatRefreshObserver( 
                            MVmbxSatRefreshObserver& aObserver )
                            : iRSatSession(),
                              iSatRefresh( *this ),
                              iObserver( aObserver )
    {
    VMBLOGSTRING( "VMBX:CVmbxSatRefreshObserver::CVmbxSatRefreshObserver <=>" );
    }

// -----------------------------------------------------------------------------
// CVmbxSatRefreshObserver::NewL
// two phase constructor
// -----------------------------------------------------------------------------
//
CVmbxSatRefreshObserver* CVmbxSatRefreshObserver::NewL( 
                                MVmbxSatRefreshObserver& aObserver )
    {
    VMBLOGSTRING( "VMBX: CVmbxSatRefreshObserver::NewL =>" );
    CVmbxSatRefreshObserver* self = new( ELeave ) CVmbxSatRefreshObserver(
         aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    VMBLOGSTRING( "VMBX: CVmbxSatRefreshObserver::NewL <=" );
    return self;
    }
// ---------------------------------------------------------------------------
// CVmbxSatRefreshObserver::ConstructL
// two phase constructor
// ---------------------------------------------------------------------------
//
void CVmbxSatRefreshObserver::ConstructL()
    {
    VMBLOGSTRING( "VMBX: CVmbxSatRefreshObserver::ConstructL =>" );
    iRSatSession.ConnectL();
    iSatRefresh.OpenL( iRSatSession );
    iObservedFileList.Append( KMbdnEf );
    iObservedFileList.Append( KMbiEf );
    iObservedFileList.Append( KVmbxFile );
    iSatRefresh.NotifyFileChangeL( iObservedFileList );
    VMBLOGSTRING( "VMBX: CVmbxSatRefreshObserver::ConstructL <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxSatRefreshObserver::~CVmbxSatRefreshObserver
// Destructor
// ---------------------------------------------------------------------------
//
CVmbxSatRefreshObserver::~CVmbxSatRefreshObserver()
    {
    VMBLOGSTRING( "VMBX:CVmbxSatRefreshObserver::~CVmbxSatRefreshObserver =>" );
    iSatRefresh.Cancel();
    iSatRefresh.Close();
    iRSatSession.Close();
    VMBLOGSTRING( "VMBX:CVmbxSatRefreshObserver::~CVmbxSatRefreshObserver <=" );
    }

// ---------------------------------------------------------------------------
// CVmbxSatRefreshObserver::AllowRefresh
// Refresh query callback for USim
// ---------------------------------------------------------------------------
 TBool CVmbxSatRefreshObserver::AllowRefresh( TSatRefreshType /*aType*/,
                        const TSatRefreshFiles& /*aFiles*/ )
    {
    VMBLOGSTRING( "VMBX: CVmbxSatRefreshObserver::AllowRefresh =>" );
    VMBLOGSTRING( "VMBX: CVmbxSatRefreshObserver::AllowRefresh <=" );
    return ETrue;
    }

// ---------------------------------------------------------------------------
//  CVmbxSatRefreshObserver::Refresh
// Refresh notification callback for USim
// ---------------------------------------------------------------------------
void CVmbxSatRefreshObserver::Refresh( TSatRefreshType aType, 
                                const TSatRefreshFiles& aFiles )
    {
    VMBLOGSTRING( "VMBX: CVmbxSatRefreshObserver::Refresh =>" );
    if ( EFileChangeNotification == aType )
        {
        const TInt elFile1( aFiles.Locate( KMbdnEf ) );// 6fc7 file
        VMBLOGSTRING2( "VMBX: CVmbxSatRefreshObserver::Refresh: elFile1%I",
         elFile1 );
        // 6fc9 file, indecator for 6fc7 file
        const TInt elFile2( aFiles.Locate( KMbiEf ) );
        VMBLOGSTRING2( "VMBX: CVmbxSatRefreshObserver::Refresh: elFile2%I",
         elFile2 );
        // 6f17 file
        const TInt elFile3( aFiles.Locate( KVmbxFile ) );
        VMBLOGSTRING2( "VMBX: CVmbxSatRefreshObserver::Refresh: elFile3%I",
         elFile3 );

        if ( KErrNotFound != elFile1 || KErrNotFound != elFile2 
                || KErrNotFound != KVmbxFile )
            {
            iObserver.HandleSatRefresh();
            }
        iSatRefresh.RefreshEFRead( EFalse );
        }
    VMBLOGSTRING( "VMBX: CVmbxSatRefreshObserver::Refresh <=" );
    }

// End of file
