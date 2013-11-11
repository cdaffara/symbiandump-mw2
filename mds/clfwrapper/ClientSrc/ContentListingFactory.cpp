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
* Description:  
*
*/



// INCLUDE FILES
#include    <ContentListingFactory.h>
#include    "CCLFEngineImpl.h"
#include    "CCLFSortingStyleimpl.h"
#include    "CCLFItemImpl.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ContentListingFactory::NewContentListingEngineLC
// -----------------------------------------------------------------------------
//
EXPORT_C MCLFContentListingEngine*
                        ContentListingFactory::NewContentListingEngineLC()
    {
    return CCLFEngineImpl::NewLC();
    }

// -----------------------------------------------------------------------------
// ContentListingFactory::NewModifiableItemLC
// -----------------------------------------------------------------------------
//
EXPORT_C MCLFModifiableItem* ContentListingFactory::NewModifiableItemLC()
    {
    CCLFModifiableItemImpl* item = CCLFModifiableItemImpl::NewL();
    CleanupStack::PushL( item );
    return item;
    }

// -----------------------------------------------------------------------------
// ContentListingFactory::NewSortingStyleLC
// -----------------------------------------------------------------------------
//
EXPORT_C MCLFSortingStyle* ContentListingFactory::NewSortingStyleLC()
    {
    return CCLFSortingStyleImpl::NewLC();
    }

// -----------------------------------------------------------------------------
// ContentListingFactory::NewSortingStyleLC
// -----------------------------------------------------------------------------
//
EXPORT_C MCLFSortingStyle* ContentListingFactory::NewSortingStyleLC(
                                        TResourceReader& aResource )
    {
    return CCLFSortingStyleImpl::NewLC( aResource );
    }

//  End of File
