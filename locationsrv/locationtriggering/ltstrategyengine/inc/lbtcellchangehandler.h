/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Class declaration of cell change handler class.
*
*/


#ifndef C_LBTCELLCHANGEHANDLER_H
#define C_LBTCELLCHANGEHANDLER_H

// INCLUDES
#include <e32base.h>
#include <etelmm.h>
#include "lbtcellchangeobserver.h"

    
/**
 * Cell change handler. It takes care of issuing requests to Etel for getting
 * notified about cell changes and it will report such events to its observer.
 */
class CLbtCellChangeHandler : public CActive
    {
    public:
    
        /**
         * Instantiation method.
         * 
         * @param aMPhone handle to Etel API
         * @return pointer to new instance
         */
        static CLbtCellChangeHandler* NewL(RMobilePhone& aMPhone );

        /**
         * Default destructor
         */
        virtual ~CLbtCellChangeHandler();
        
        /**
         * Issues a new request and sets this object active.
         */
        void GetNetworkInfo();
        
        /**
         * Sets the observer to this class
         */
        void SetObserver( MCellChangeObserver* aObserver );

        /**
         * Remove the observer from the array.
         */
        void Remove( MCellChangeObserver* aObserver ); 		
    protected:
    
        // methods from CActive
        virtual void RunL();
        virtual void DoCancel();
        
    private:

        
        /**
         * Constructor.
         * 
         * @param aMPhone handle to Etel API
         * @param aObserver reference to observer object
         */
        CLbtCellChangeHandler(RMobilePhone& aMPhone);

        /**
         * Second level constructor
         */
        void ConstructL();
        
    private:
    
        // data
        // etel api
        RMobilePhone& 								iMPhone; 
        // Handle to cell change observers
        RPointerArray<MCellChangeObserver> 			iObserverArray;
        // Command id 
        TInt 										iCommandId; 
        // Network info
        RMobilePhone::TMobilePhoneNetworkInfoV1		iNwInfo;
        // Network info package
        RMobilePhone::TMobilePhoneNetworkInfoV1Pckg	iNwInfoPckg; 
        // location info
        RMobilePhone::TMobilePhoneLocationAreaV1	iLocArea; 
        
        TInt                                        iLastStatusInfo;
        
    };

#endif //C_LBTCELLCHANGEHANDLER_H

// End of File

