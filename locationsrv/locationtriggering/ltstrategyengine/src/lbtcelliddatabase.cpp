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
* Description:  Implements the cell id database
*
*/
#include "lbtcelliddatabase.h"

/**
 * Maximum entry that database can hold 
 */
static const TInt KMaxEntry = 25;

/**
 * Index of the entry that needs to removed.
 */
static const TInt EIndexToBeRemoved = 0;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CLbtCellIdDatabase::NewL
// The Symbian 2 phase constructor
// ---------------------------------------------------------------------------
//
CLbtCellIdDatabase* CLbtCellIdDatabase::NewL()
    {
    CLbtCellIdDatabase* self = new(ELeave) CLbtCellIdDatabase;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

// ---------------------------------------------------------------------------
// CLbtCellIdDatabase::CLbtCellIdDatabase
// Default constructor
// ---------------------------------------------------------------------------
//
CLbtCellIdDatabase::CLbtCellIdDatabase()
    {
    
    }

// ---------------------------------------------------------------------------
// CLbtCellIdDatabase::ConstructL
//
// ---------------------------------------------------------------------------
//
void CLbtCellIdDatabase::ConstructL()
    {
    
    }

// ---------------------------------------------------------------------------
// CLbtCellIdDatabase::~CLbtCellIdDatabase
// Destructor
// ---------------------------------------------------------------------------
//
CLbtCellIdDatabase::~CLbtCellIdDatabase()
    {
    iCellIdDbArray.Close();
    }

// ---------------------------------------------------------------------------
// CLbtCellIdDatabase::InsertCidLocationL
// 
// ---------------------------------------------------------------------------
//
void CLbtCellIdDatabase::InsertCidLocation( TInt32 aCountryCode, 
                                            TInt32 aNetworkID, 
                                            TInt32 aLac, 
                                            TInt32 aCellId,
                                            TLocality aLocality )
    {
    if( IsDuplicateEntry( aCountryCode,aNetworkID,aLac,aCellId,aLocality ) )
        {
        return;
        }
    TCellIdCoordinateData cellIdCoordinateData;
    cellIdCoordinateData.iCountryCode = aCountryCode;
    cellIdCoordinateData.iNetworkID = aNetworkID;
    cellIdCoordinateData.iLac = aLac;
    cellIdCoordinateData.iCellId = aCellId;
    cellIdCoordinateData.iLocality.SetCoordinate( aLocality.Latitude(),
                                                  aLocality.Longitude(),
                                                  aLocality.Altitude() );
    cellIdCoordinateData.iLocality.SetHorizontalAccuracy( aLocality.HorizontalAccuracy() );
    
    if( iCellIdDbArray.Count() == KMaxEntry )
        {
        iCellIdDbArray.Remove( EIndexToBeRemoved );
        }
    TInt error = iCellIdDbArray.Append( cellIdCoordinateData );
    }

// ---------------------------------------------------------------------------
// CLbtCellIdDatabase::GetLocationForCid
// 
// ---------------------------------------------------------------------------
//
TInt CLbtCellIdDatabase::GetLocationForCid( TInt32 aCountryCode, 
                                            TInt32 aNetworkID, 
                                            TInt32 aLac,
                                            TInt32 aCellId,
                                            TLocality& aLocality )
    {
    for( TInt i=0;i<iCellIdDbArray.Count();i++ )
        {
        if( iCellIdDbArray[i].iCountryCode == aCountryCode &&
            iCellIdDbArray[i].iNetworkID == aNetworkID && 
            iCellIdDbArray[i].iLac == aLac &&
            iCellIdDbArray[i].iCellId == aCellId )
            {
            aLocality.SetCoordinate( iCellIdDbArray[i].iLocality.Latitude(),
                                     iCellIdDbArray[i].iLocality.Longitude(),
                                     iCellIdDbArray[i].iLocality.Altitude() );
            aLocality.SetHorizontalAccuracy( iCellIdDbArray[i].iLocality.HorizontalAccuracy() );
            
            // This entry has been used and should be deleted last compared to 
            // unused entry. This is pushed to the end because we delete 
            // entry from first when max cap is reached.Remove operation should be 
            // performed only if append succeeds
            TInt error = iCellIdDbArray.Append( iCellIdDbArray[i] );
            if( error == KErrNone )
                {
                iCellIdDbArray.Remove( i );
                }
            
            return KErrNone;
            }
        }
    return KErrNotFound;
    }

// ---------------------------------------------------------------------------
// CLbtCellIdDatabase::IsDuplicateEntry
// 
// ---------------------------------------------------------------------------
//
TBool CLbtCellIdDatabase::IsDuplicateEntry( TInt32 aCountryCode, 
                                            TInt32 aNetworkID, 
                                            TInt32 aLac, 
                                            TInt32 aCellId,
                                            TLocality aLocality )
    {
    for( TInt i=0;i<iCellIdDbArray.Count();i++ )
        {
        if( iCellIdDbArray[i].iCountryCode == aCountryCode &&
            iCellIdDbArray[i].iNetworkID == aNetworkID && 
            iCellIdDbArray[i].iLac == aLac &&
            iCellIdDbArray[i].iCellId == aCellId )
            {
            // If the HA of the current entry is less than the existing entry,
            // replace the old entry with new one.
            if( aLocality.HorizontalAccuracy() < iCellIdDbArray[i].iLocality.HorizontalAccuracy() )
                {
                iCellIdDbArray[i].iLocality.SetCoordinate( aLocality.Latitude(),
                                                           aLocality.Longitude(),
                                                           aLocality.Altitude() );
                iCellIdDbArray[i].iLocality.SetHorizontalAccuracy( aLocality.HorizontalAccuracy() );
                }
            return ETrue;
            }
        }
    return EFalse;
    }
