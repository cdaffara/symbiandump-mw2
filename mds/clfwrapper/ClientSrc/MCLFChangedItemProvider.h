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



#ifndef MCLFCHANGEDITEMPROVIDER_H
#define MCLFCHANGEDITEMPROVIDER_H

//  INCLUDES
#include <CLFContentListing.h>
#include <bamdesca.h>

// CLASS DECLARATION

/**
*  .
*  @lib ContentListingFramework.lib
*  @since Series 60 3.1
*/
class MCLFChangedItemProviderObserver
    {
    public:
    
        /**
        * Two-phased constructor.
        */
        virtual void NewChangedItemsL() = 0;

    protected:

        /**
        * Destructor.
        */
        virtual ~MCLFChangedItemProviderObserver() {}

    };

class MCLFChangedItemProvider
    {

    public:
    
        virtual TBool IsItemsForModel( const MDesCArray& aMimeTypes,
                                       const TArray<TInt>& aMediaTypes,
                                       const TArray<TCLFItemId>& aModelItemIds ) = 0;

        virtual void RemoveChangedItemProviderObserver( 
                                    MCLFChangedItemProviderObserver* aObserver ) = 0;


    protected:

        /**
        * Destructor.
        */    
        virtual ~MCLFChangedItemProvider() {}

    };


#endif      // MCLFCHANGEDITEMPROVIDER_H

// End of File
