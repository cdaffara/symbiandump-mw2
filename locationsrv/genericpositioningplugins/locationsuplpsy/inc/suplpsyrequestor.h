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
* Description:  SUPL PSY position requestor header file
*
*/



#ifndef SUPLPSYREQUESTOR_H
#define SUPLPSYREQUESTOR_H

//  INCLUDES
#include <e32base.h>
#include <lbs.h>
#include <epos_suplterminal.h>

// FORWARD DECLARATIONS
// CLASS DECLARATION

/**
*  This class implements position requestor of SUPL PSY
*  
*  @lib nlasuplpsy.dll
*  @since S60 3.1
*/
class CSuplPsyRequestor : public CActive
    {
    public:  // Constructors and destructor
        
        /**
        * Two - phased constructor.
        */
        static CSuplPsyRequestor* NewL( 
            const TCallBack& aObserver );
        
        /**
        * Destructor.
        */
        ~CSuplPsyRequestor();
        

    public:
        /**
        * Request location
        */
        void RequestLocation();

        /**
        * Get location request result
        * @return Error code
        */
        TInt GetRequestResult();

        /**
        * Get location from the request
        * @return The data is valid only if the location request
        * succeed and this function is called inside callback function.
        */
        const HPositionGenericInfo& GetLocation();
        
        /**
        * Reset last known position
        */
        void ResetLastPosition();

    private:
        /**
        * From CActive
        */
        void RunL();

        /**
        * From CActive
        */
        void DoCancel();

        /**
        * From CActive
        */
        TInt RunError( TInt aError );

    private:

        /**
        * C++default constructor.
        */
        CSuplPsyRequestor( 
            const TCallBack& aObserver );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Construct HPositionGenericInfo and set requested fields
        */
        HPositionGenericInfo* ConstructGenericInfoL( 
            TInt aBufferSize = KPositionGenericInfoDefaultBufferSize ) const;

        /**
         * Checks if request is made within the Ttnf time
         */
        TBool IsRequestInTtnf();
    private:    // Data
        // Callback function for get location request complete event
        TCallBack iObserver;
        
        // Supl terminal server
        RSuplTerminalServer iSuplServer;

        // Supl subsession
        RSuplTerminalSubSession iSuplSession;

        // position info
        HPositionGenericInfo* iPositionInfo;

        // Flag to indicate first request
        TBool iFirstReq;

        // TTNF from NPP PSY
        TInt iTtnf;

        // TTNF flag to return the position
        TBool iTtnfFlag;

        // Result of previous request and getposition
        TBool iPrevFixSuccess;
    };

#endif      // SUPLPSYREQUESTOR_H   
            
// End of File
