/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Harvester client active object
*
*/


#ifndef __CHARVESTERCLIENTAO_H__
#define __CHARVESTERCLIENTAO_H__

#include <e32msgqueue.h>
#include <e32base.h>
#include <badesca.h>

#include "harvesterclient.h"

class RHarvesterClient;
class MHarvestObserver;

NONSHARABLE_CLASS( CHarvesterClientAO ) : public CActive
    {
    public:
        /**
        * Construction.
        */
        static CHarvesterClientAO* NewL( RHarvesterClient &aHarvesterClient );

        /**
        * Destruction.
        */
        virtual ~CHarvesterClientAO();

        /**
        * Method for adding an observer.
        * @param aObserver  Pointer to observer object.
        */
        void SetObserver( MHarvestObserver* aObserver );

        /**
        * Method for removing an observer.
        * @param aObserver  Pointer to observer object.
        */		
        void RemoveObserver( MHarvestObserver* aObserver );
        
        void NotificateObserver( TInt aErr );
        
        /**
         * Set AO to active state. RunL is launched from harvesting complete
         * request which are subscribed from server.
         */ 
        void Active();
        
        void CancelRequest();
        
    protected:
		
        /**
         * RunL.
         * From CActive.
         */
        virtual void RunL();

        /**
         * DoCancel.
         * From CActive.
         */
        virtual void DoCancel();

        /**
         * RunError for handling leaves occuring in RunL.
         * From CActive.
         */
        virtual TInt RunError( TInt aError );

    private:

        /**
        * Private constructor
        * 
        * @param aHarvesterClient Reference to session class
        */	
        CHarvesterClientAO( RHarvesterClient &aHarvesterClient );

        /**
        * 2nd phase construction
        */	
        void ConstructL();        
		
    private:

        /**
        * Observer of the class
        */ 
        MHarvestObserver* iObserver;

        /**
        * Reference to Harvester client session
        */   	
        RHarvesterClient& iHarvesterClient;

        /**
         * Harvester server assigned file name
         */ 
        TFileName iURI;
        
        TBool iCancelled;
    };

#endif // __CHARVESTERCLIENTAO_H__
