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


#ifndef MCLFOPERATIONOBSERVER_H
#define MCLFOPERATIONOBSERVER_H

//  INCLUDES
#include <e32def.h>

// DATA TYPES
/**
* Content Listing Framework list model operation events
*/
enum TCLFOperationEvent
    {
    /// Refresh operation is completed
    ECLFRefreshComplete = 0x0,
    /// Model content is obsolated and should be refreshed
    ECLFModelOutdated = 0x1
    };

// FORWARD DECLARATIONS
class MCLFOperationObserverExt;

// CLASS DECLARATION

/**
*  Operation Observer is for observing operation events of the Content Listing
*  Framework. Client application gets notified when asynchronous operations
*  are completed. For example, ECLFRefreshComplete event is received when the
*  asynchronous refresh operation is compeleted.
*
*  @lib ContentListingFramework.lib
*  @since S60 3.1
*/
class MCLFOperationObserver
    {
    public: // New functions

        /**
        * Abstract method to get list model operation events. This method is
        * called when an event is received.
        * @since S60 3.1
        * @param aOperationEvent Operation event code of the event
        * @param aError System wide error code if the operation did not
        *        succeed.
        */
        virtual void HandleOperationEventL( TCLFOperationEvent aOperationEvent,
                                            TInt aError ) = 0;

    protected:

        /**
        * Destructor.
        */
        virtual ~MCLFOperationObserver() {}

    private: // Extension interface

        /**
        * This member is internal and not intended for use.
        */
        virtual MCLFOperationObserverExt* Extension() { return NULL; }


    };

#endif      // MCLFOPERATIONOBSERVER_H

// End of File
