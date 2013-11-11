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
* Description: Fast counters for local database
*
*
*/


#include <epos_cposlmlocaldbaccess.h>
#include <epos_poslmlandmarkhandler.h>
#include <epos_poslmcategoryhandler.h>

#include "epos_cposlmlocaldbfastcounters.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalDbFastCounters::CPosLmLocalDbFastCounters( CPosLmLocalDbAccess& aDbAccess )
: iDbAccess( aDbAccess )
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalDbFastCounters::~CPosLmLocalDbFastCounters()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CPosLmLocalDbFastCounters* CPosLmLocalDbFastCounters::NewL( CPosLmLocalDbAccess& aDbAccess )
    {
    return new (ELeave) CPosLmLocalDbFastCounters( aDbAccess );
    }

// -----------------------------------------------------------------------------
// From MPosDatabaseFastCounters
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDbFastCounters::TotalLandmarksCount()
    {
    TInt err( KErrNone );
    TUint count( 0 );
    TRAP( err, count = PosLmLandmarkHandler::TotalLandmarkCountL( iDbAccess ); );

    return err ? err : count;
    }

// -----------------------------------------------------------------------------
// From MPosDatabaseFastCounters
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDbFastCounters::TotalCategoriesCount()
    {
    TInt err( KErrNone );
    TUint count( 0 );
    TRAP( err, count = PosLmCategoryHandler::TotalCategoryCountL( iDbAccess ); );

    return err ? err : count;
    }

// -----------------------------------------------------------------------------
// From MPosDatabaseFastCounters
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDbFastCounters::UncategorizedLandmarksCount()
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MPosDatabaseFastCounters
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDbFastCounters::UnreferencedCategoriesCount()
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MPosDatabaseFastCounters
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDbFastCounters::CategorizedLandmarksCount()
    {
    return KErrNotSupported;
    }

// -----------------------------------------------------------------------------
// From MPosDatabaseFastCounters
// -----------------------------------------------------------------------------
//
TInt CPosLmLocalDbFastCounters::ReferencedCategoriesCount()
    {
    return KErrNotSupported;
    }
