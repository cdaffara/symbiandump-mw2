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


#ifndef MCLFCHANGEDITEMOBSERVER_H
#define MCLFCHANGEDITEMOBSERVER_H

//  INCLUDES
#include <CLFContentListing.h>
#include <e32std.h>

// FORWARD DECLARATIONS
class MCLFChangedItemObserverExt;

// CLASS DECLARATION

/**
*  Changed Item Observer interface.
*  Implement this interface to get notifications of changed,
*  new or deleted items.
*
*  @lib ContentListingFramework.lib
*  @since S60 3.1
*/
class MCLFChangedItemObserver
    {
    public: // New functions

        /**
        * Abstract method for handling item change event. This method is
        * called when, for example, some item is renamed.
        * @since S60 3.1
        * @param aItemIDArray List of Item IDs of changed items.
        */
        virtual void HandleItemChangeL(
                            const TArray<TCLFItemId>& aItemIDArray ) = 0;

        /**
        * Abstract method for handling errors. This method can be used to
        * handle errors in changed item event.
        * @since S60 3.1
        * @param aError System wide error code.
        */
        virtual void HandleError( TInt aError ) = 0;

    protected:

        /**
        * Destructor.
        */
        virtual ~MCLFChangedItemObserver() {}

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MCLFChangedItemObserverExt* Extension() { return NULL; }

    };

#endif      // MCLFCHANGEDITEMOBSERVER_H

// End of File
