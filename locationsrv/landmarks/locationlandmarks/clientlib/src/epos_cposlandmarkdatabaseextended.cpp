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
* Description:  This class defines extensions to the functionality of CPosLandmarkDatabase class.
*
*/


#include <ecom/ecom.h>
#include <uri16.h>
#include "EPos_LandmarksUids.hrh"
#include <epos_cposlandmarkdatabaseextended.h>
#include "epos_mposlmdatabasefastcounters.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabaseExtended::CPosLandmarkDatabaseExtended()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabaseExtended* CPosLandmarkDatabaseExtended::OpenL()
    {
    TUid implementationUid;
    implementationUid.iUid = KPosLmLocalDatabaseExtImplUid;

    // Check if any implementation supports this protocol
    TUid interfaceUid;
    interfaceUid.iUid = KPosLmDatabaseExtendedIfUid;

    RImplInfoPtrArray implInfoArray;
    REComSession::ListImplementationsL( interfaceUid, implInfoArray );
    if ( implInfoArray.Count() == 0 )
        {
        User::Leave( KErrNotSupported );
        }
    else
        {
        implInfoArray.ResetAndDestroy();
        }

    TAny* ptr = REComSession::CreateImplementationL(
        implementationUid,
        _FOFF( CPosLandmarkDatabaseExtended, iDtorIdKey ),
        NULL );

    return reinterpret_cast<CPosLandmarkDatabaseExtended*>( ptr );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabaseExtended* CPosLandmarkDatabaseExtended::OpenL(
    const TDesC&  aDatabaseUri )
    {
    // Determine which implementation id to use by checking the protocol of
    // the URI (e.g. http). If no protocol is specified "file" is used.
    _LIT8( KDefaultProtocol, "file" );
    _LIT( KProtocolDelimiter, "://" );

    TInt position = aDatabaseUri.Find( KProtocolDelimiter );

    TEComResolverParams params;
    HBufC8* protocol8 = NULL;

    if ( position != KErrNotFound && position != 0 )
        {
        protocol8 = HBufC8::NewLC( position + 1 );
        protocol8->Des().FillZ();
        protocol8->Des().Copy( aDatabaseUri.Left( position ) );
        params.SetDataType( *protocol8 );
        }
    else
        {
        params.SetDataType( KDefaultProtocol );
        }

    // Check if any implementation supports this protocol
    TUid interfaceUid;
    interfaceUid.iUid = KPosLmDatabaseExtendedIfUid;

    RImplInfoPtrArray implInfoArray;
    REComSession::ListImplementationsL( interfaceUid, params, implInfoArray );
    if ( implInfoArray.Count() == 0 )
        {
        User::Leave( KErrNotSupported );
        }
    else
        {
        implInfoArray.ResetAndDestroy();
        }

    // Send the protocol name to the Ecom server
    // which determines the implementation.
    HBufC* strPtr = aDatabaseUri.AllocLC();

    TAny* ptr = REComSession::CreateImplementationL(
        interfaceUid,
        _FOFF( CPosLandmarkDatabaseExtended, iDtorIdKey ),
        strPtr,
        params );

    CleanupStack::PopAndDestroy( strPtr );

    if ( protocol8 )
        {
        CleanupStack::PopAndDestroy( protocol8 );
        }

    return reinterpret_cast<CPosLandmarkDatabaseExtended*>( ptr );
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabaseExtended::~CPosLandmarkDatabaseExtended()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLandmarkDatabaseExtended::LandmarksCount()
    {
    MPosLmDatabaseFastCounters* extension = 
        reinterpret_cast<MPosLmDatabaseFastCounters*>( GetExtension( TUid::Uid( KPosLmDatabaseFastCountersIfUid ) ) );
    if ( extension )
        {
        return extension->TotalLandmarksCount();
        }
    else
        {
        return KErrNotSupported;
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLandmarkDatabaseExtended::CategoriesCount()
    {
    MPosLmDatabaseFastCounters* extension = 
        reinterpret_cast<MPosLmDatabaseFastCounters*>( GetExtension( TUid::Uid( KPosLmDatabaseFastCountersIfUid ) ) );
    if ( extension )
        {
        return extension->TotalCategoriesCount();
        }
    else
        {
        return KErrNotSupported;
        }
    }
