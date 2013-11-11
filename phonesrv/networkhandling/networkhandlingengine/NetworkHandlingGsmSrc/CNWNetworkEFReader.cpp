/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of 
*                CNWHZCbCacheReader class member functions.
*
*/



// INCLUDE FILES
#include "CNWNetworkEFReader.h"
#include "NWLogger.h"

// FORWARD DECLARATIONS

// DEFINITIONS
const TInt KNWCacheOne = 1;
const TInt KNWFirstRecordId = 0;
// LOCAL FUNCTIONS


// ----------------------------------------------------------------------------
// CNWNetworkEFReader::NewL
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
CNWNetworkEFReader* CNWNetworkEFReader::NewL( RMmCustomAPI& aCustomAPI, 
    MNWNetworkEFReaderObserver * aObserver )
    {
    NWLOGSTRING( KNWOBJECT, "NET CNWNetworkEFReader::NewL() Begin " );
    
    CNWNetworkEFReader * self = 
        new (ELeave) CNWNetworkEFReader( aCustomAPI, aObserver );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    
    NWLOGSTRING( KNWOBJECT, "NET CNWNetworkEFReader::NewL() End " );
    
    return self;
    }
 
// ----------------------------------------------------------------------------
// CNWNetworkEFReader::CNWNetworkEFReader
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CNWNetworkEFReader::CNWNetworkEFReader( RMmCustomAPI& aCustomAPI,
    MNWNetworkEFReaderObserver * aObserver ) : 
    CActive( EPriorityStandard ), iCustomAPI( aCustomAPI ), 
    iObserver( aObserver ) 
    {
    NWLOGSTRING( KNWOBJECT, 
        "NET CNWNetworkEFReader::CNWNetworkEFReader() called " );
    }

// ----------------------------------------------------------------------------
// CNWNetworkEFReader::ConstructL
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkEFReader::ConstructL()
    {
    CActiveScheduler::Add( this );
    }

       
// ----------------------------------------------------------------------------
// CNWNetworkEFReader::~CNWNetworkEFReader
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
CNWNetworkEFReader::~CNWNetworkEFReader()
    {
    NWLOGSTRING( KNWOBJECT, 
      "NW: CNWNetworkViagSimReader::~CNWNetworkViagSimReader." );
    }


// ----------------------------------------------------------------------------
// CNWNetworkEFReader::RunL
// Completes when SIM cache is ready for reading.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkEFReader::RunL()
    {
    NWLOGSTRING( KNWOBJECT, "NET CNWNetworkEFReader::RunL() Begin " );

    // Report the results to Observer
    NWLOGSTRING2( KNWINT, 
      "NW: CBS HZ: CNWNetworkEFReader, RunL status = %d", iStatus.Int() );
    iObserver->ReadCbCacheSync( iStatus.Int() );
    
    NWLOGSTRING( KNWOBJECT, "NET CNWNetworkEFReader::RunL() End " );
    }
    
    
// ----------------------------------------------------------------------------
// CNWNetworkEFReader::DoCancel
// 
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkEFReader::DoCancel()
    {
    NWLOGSTRING( KNWOBJECT, "NET CNWNetworkEFReader::DoCancel() called " );
    }


// ----------------------------------------------------------------------------
// CNWNetworkEFReader::ReadElementaryFileL
// Issues request to fetch sim cache information.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TInt CNWNetworkEFReader::StartMonitoringSimCache( 
        MNWNetworkEFReaderObserver * aObserver )
    {
    NWLOGSTRING( KNWINT, "NW: CBS HZ: StartMonitoringSimCache() Begin" );
    if( IsActive() )
        {
        NWLOGSTRING( KNWERROR, "NW: CBS HZ: StartMonitoringSimCache(),\
            Monitor has already been active." );
        return KErrInUse;
        }
    iObserver = aObserver;
    iCache.iCacheId = KNWCacheOne;
    iCache.iRecordId = KNWFirstRecordId;
    iCustomAPI.ReadViagHomeZoneCache( iStatus, iCache, iRecord );
    SetActive();
    NWLOGSTRING( KNWINT, "NW: CBS HZ: StartMonitoringSimCache() End" );
    return KErrNone;
    }

