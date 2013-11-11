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


#ifndef CCLFEVENTHANDLER_H
#define CCLFEVENTHANDLER_H

//  INCLUDES
#include "CCLFServerProxy.h"
#include <CLFContentListing.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MCLFEventHandlerObserver;

typedef void ( CCLFServerProxy::*TMemberFnOp )( TRequestStatus& aStatus );
typedef TInt ( CCLFServerProxy::*TMemberFnCancel )();
typedef void ( MCLFEventHandlerObserver::*TMemberFnObserver )( TInt aError );

// CLASS DECLARATION

/**
*  Item change event handler.
*
*  @lib ContentListingFramework.lib
*  @since Series 60 3.0
*/
NONSHARABLE_CLASS( CCLFEventHandler ) : public CActive
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCLFEventHandler* NewL( CCLFServerProxy& aEventHandler,
                                       MCLFEventHandlerObserver& aObserver );

        /**
        * Destructor.
        */
        virtual ~CCLFEventHandler();

    public: // New functions

        /**
        * Start event observing.
        * @since Series 60 3.0
        */
        void ObserverUpdateStartEvent();

        /**
        * Start event observing.
        * @since Series 60 3.0
        */
        void ObserverUpdateEndEvent();

        /**
        * Get changed items.
        * @since Series 60 3.0
        * @param aItemArray changed item ID array
        */
        void GetChangedItemsL( RArray<TCLFItemId>& aItemArray );

    protected:  // Functions from base classes

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        */
        TInt RunError( TInt aError );

    private:

        void DoStart();

    private:

        /**
        * C++ default constructor.
        */
        CCLFEventHandler( CCLFServerProxy& aEventHandler,
                                 MCLFEventHandlerObserver& aObserver );

    private:    // Data
        // Ref. 
        CCLFServerProxy& iEventHandler;

        // Ref. 
        MCLFEventHandlerObserver& iObserver;

        TMemberFnOp iOperationFunction;

        TMemberFnCancel iCancelFunction;
        
        TMemberFnObserver iObserverFunction;

    };

#endif // CCLFEVENTHANDLER_H

// End of File
