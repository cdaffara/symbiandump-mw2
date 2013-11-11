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


#ifndef CONTENTLISTINGFACTORY_H
#define CONTENTLISTINGFACTORY_H

//  INCLUDES
#include <e32def.h>

// FORWARD DECLARATIONS
class MCLFContentListingEngine;
class MCLFModifiableItem;
class MCLFSortingStyle;
class TResourceReader;

// CLASS DECLARATION

/**
*  Factory for Content Listing Framework.
*  You can create new instances of Content Listing Engines, Modifiable items
*  and Sorting Styles by using this factory.<br><br>
*  Example:
*  @code
*  // Create a new instance of Content Listing Engine
*  MCLFContentListingEngine* engine =
*      ContentListingFactory::NewContentListingEngineLC();
*
*  // Create a new instance of Sorting style
*  MCLFSortingStyle* sortingStyle =
*      ContentListingFactory::NewSortingStyleLC();
*
*  // Create a new instance of Modifiable item
*  MCLFModifiableItem* modItem =
*      ContentListingFactory::NewModifiableItemLC();
*  @endcode
*
*  @lib ContentListingFramework.lib
*  @since S60 3.1
*/
class ContentListingFactory
    {
    public: // New functions

        /**
        * Create new instance of Content Listing Engine.
        * @since S60 3.1
        * @return New instance of Content Listing Engine.
        *         Ownership is transferred to the client application.
        */
        IMPORT_C static MCLFContentListingEngine* NewContentListingEngineLC();

        /**
        * Create new instance of Modifiable Item.
        * @since S60 3.1
        * @return New instance of Modifiable Item.
        *         Ownership is transferred to the client application.
        */
        IMPORT_C static MCLFModifiableItem* NewModifiableItemLC();

        /**
        * Create new instance of Sorting Style.
        * @since S60 3.1
        * @return New instance of the Sorting Style.
        *         Ownership is transferred to the client application.
        */
        IMPORT_C static MCLFSortingStyle* NewSortingStyleLC();

        /**
        * Create new instance of Sorting Style from resource of the
        * application by using TResourceReader.
        * @since S60 3.1
        * @param aResource Resource reader to sorting style resource.
        *                  Use resource struct CLF_SORTING_STYLE. See
        *                  CLFContentListing.rh
        * @return New instance of the Sorting Style.
        *         Ownership is transferred to the client application.
        */
        IMPORT_C static MCLFSortingStyle* NewSortingStyleLC(
                                                TResourceReader& aResource );

    private:
        // Prohibit C++ default constructor.
        ContentListingFactory();
        // Prohibit Destructor.
        ~ContentListingFactory();

    };

#endif      // CONTENTLISTINGFACTORY_H

// End of File
