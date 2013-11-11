/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Implementation of CPosLandmarkSearch class
*
*
*/


// INCLUDE FILES
#include    <ecom/ecom.h>
#include    <EPos_CPosLmOperation.h>
#include    <EPos_CPosLandmarkDatabase.h>
#include    <EPos_PosLmImplExtension.h>
#include    <EPos_LandmarksErrors.h>
#include    "EPos_CPosLandmarkSearch.h"
#include    "EPos_LandmarksSearchUids.hrh"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLandmarkSearch::CPosLandmarkSearch
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkSearch::CPosLandmarkSearch()
:   iMaxNumOfMatches(KPosLmMaxNumOfMatchesUnlimited)
    {
    }

// -----------------------------------------------------------------------------
// CPosLandmarkSearch::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkSearch* CPosLandmarkSearch::NewL(
    CPosLandmarkDatabase& aDatabase)
    {
    TEComResolverParams params;
    TUid interfaceUid;
    interfaceUid.iUid = KPosLmSearchIfUid;

    TBuf8<KMaxUidName + 1> name(KMaxUidName + 1);

    name.FillZ();
    name.Copy(PosLmImplExtension::ImplementationId(aDatabase).Name());
    params.SetDataType(name);

    TAny* ptr = REComSession::CreateImplementationL(
        interfaceUid,
        _FOFF(CPosLandmarkSearch, iDtorIdKey),
        &aDatabase,
        params);

    return reinterpret_cast<CPosLandmarkSearch*>(ptr);
    }

// Destructor
EXPORT_C CPosLandmarkSearch::~CPosLandmarkSearch()
    {
    // Destroy any instance variables and then inform the framework that this
    // specific instance of the interface has been destroyed.
    REComSession::DestroyedImplementation(iDtorIdKey);
    }

// -----------------------------------------------------------------------------
// CPosLandmarkSearch::MaxNumOfMatches
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLandmarkSearch::MaxNumOfMatches() const
    {
    return iMaxNumOfMatches;
    }

// -----------------------------------------------------------------------------
// CPosLandmarkSearch::SetMaxNumOfMatches
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLandmarkSearch::SetMaxNumOfMatches(
    TInt aMaxNumOfMatches)
    {
    __ASSERT_ALWAYS(
        (aMaxNumOfMatches == KPosLmMaxNumOfMatchesUnlimited ||
        aMaxNumOfMatches > 0),
        Panic(KPosLandmarksClientPanic, EPosLmInvalidArgument));

    iMaxNumOfMatches = aMaxNumOfMatches;
    }

//  End of File
