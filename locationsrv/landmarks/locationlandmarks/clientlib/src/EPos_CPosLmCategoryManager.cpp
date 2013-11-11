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
* Description: Category management for a landmark database.
*
*
*/


#include    <ecom/ecom.h>
#include    "EPos_CPosLmCategoryManager.h"
#include    "EPos_LandmarksUids.hrh"

const TInt KMaxUidNameSize = KMaxUidName + 1;

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmCategoryManager::CPosLmCategoryManager()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmCategoryManager* CPosLmCategoryManager::NewL(
    CPosLandmarkDatabase& aLandmarkDatabase)
    {
    TEComResolverParams params;
    TUid interfaceUid;
    interfaceUid.iUid = KPosLmCatManagerIfUid;

    TBuf8<KMaxUidNameSize> name(KMaxUidNameSize);

    name.FillZ();
    name.Copy(aLandmarkDatabase.ImplementationId().Name());
    params.SetDataType(name);

    TAny* ptr = REComSession::CreateImplementationL(
        interfaceUid,
        _FOFF(CPosLmCategoryManager, iDtorIdKey),
        &aLandmarkDatabase,
        params);

    return reinterpret_cast<CPosLmCategoryManager*>(ptr);
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmCategoryManager::~CPosLmCategoryManager()
    {
    // Destroy any instance variables and then inform the framework that this
    // specific instance of the interface has been destroyed.
    REComSession::DestroyedImplementation( iDtorIdKey );
    }

