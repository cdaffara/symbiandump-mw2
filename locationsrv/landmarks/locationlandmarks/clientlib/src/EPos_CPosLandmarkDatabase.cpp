/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Handle to a landmark database.
*
*
*/


#include <ecom/ecom.h>
#include <uri16.h>
#include "EPos_CPosLandmarkDatabase.h"
#include "EPos_LandmarksUids.hrh"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabase::CPosLandmarkDatabase()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabase* CPosLandmarkDatabase::OpenL()
    {
    TUid implementationUid;
    implementationUid.iUid = KPosLmLocalDatabaseImplUid;

    // Check if any implementation supports this protocol
    TUid interfaceUid;
    interfaceUid.iUid = KPosLmDatabaseIfUid;

    RImplInfoPtrArray implInfoArray;
    REComSession::ListImplementationsL(interfaceUid, implInfoArray);
    if (implInfoArray.Count() == 0)
        {
        User::Leave(KErrNotSupported);
        }
    else
        {
        implInfoArray.ResetAndDestroy();
        }

    TAny* ptr = REComSession::CreateImplementationL(
        implementationUid,
        _FOFF(CPosLandmarkDatabase, iDtorIdKey),
        NULL);

    return reinterpret_cast<CPosLandmarkDatabase*>(ptr);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabase* CPosLandmarkDatabase::OpenL(
    const TDesC&  aDatabaseUri)
    {
    // Determine which implementation id to use by checking the protocol of
    // the URI (e.g. http). If no protocol is specified "file" is used.
    _LIT8(KDefaultProtocol, "file");
    _LIT(KProtocolDelimiter, "://");

    TInt position = aDatabaseUri.Find(KProtocolDelimiter);

    TEComResolverParams params;
    HBufC8* protocol8 = NULL;

    if (position != KErrNotFound && position != 0)
        {
        protocol8 = HBufC8::NewLC(position + 1);
        protocol8->Des().FillZ();
        protocol8->Des().Copy(aDatabaseUri.Left(position));
        params.SetDataType(*protocol8);
        }
    else
        {
        params.SetDataType(KDefaultProtocol);
        }

    // Check if any implementation supports this protocol
    TUid interfaceUid;
    interfaceUid.iUid = KPosLmDatabaseIfUid;

    RImplInfoPtrArray implInfoArray;
    REComSession::ListImplementationsL(interfaceUid, params, implInfoArray);
    if (implInfoArray.Count() == 0)
        {
        User::Leave(KErrNotSupported);
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
        _FOFF(CPosLandmarkDatabase, iDtorIdKey),
        strPtr,
        params);

    CleanupStack::PopAndDestroy(strPtr);

    if (protocol8)
        {
        CleanupStack::PopAndDestroy(protocol8);
        }

    return reinterpret_cast<CPosLandmarkDatabase*>(ptr);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkDatabase::~CPosLandmarkDatabase()
    {
    // Destroy any instance variables and then inform the framework that this
    // specific instance of the interface has been destroyed.
    REComSession::DestroyedImplementation(iDtorIdKey);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
TUid CPosLandmarkDatabase::ImplementationId() const
    {
//coverity[naming_error :FALSE]
    return REComSession::GetImplementationUidL (iDtorIdKey);//iDtorIdKey;
    //return iDtorIdKey;
    }

