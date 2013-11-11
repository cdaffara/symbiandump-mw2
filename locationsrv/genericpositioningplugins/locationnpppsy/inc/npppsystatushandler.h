/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  NPP PSY status event report handler header file
*
*/



#ifndef NPPPSYSTATUSHANDLER_H
#define NPPPSYSTATUSHANDLER_H

//  INCLUDES
#include <lbscommon.h>
#include <e32base.h>

// FORWARD DECLARATIONS
class MPositionerStatus;

// CLASS DECLARATION

/**
*  This class implements the handler that responsible for device
*  status and data quality report.
*  
*  @lib nlanpppsy.dll
*  @since 3.1
*/
class CNppPsyStatusHandler : public CBase
    {
    public:  // Constructors and destructor
        
        /**
        * Two - phased constructor.
        * @param aStatusInterface Reference to MPositionerStatus to report
        *                         module status
        * @return The constructed instance of CNppPsyStatusHandler
        */
        static CNppPsyStatusHandler* NewL( 
            MPositionerStatus& aStatusInterface );
        
        /**
        * Destructor.
        */
        ~CNppPsyStatusHandler();
        

    public: // Functions from base classes
        /**
        * Set PSY List status
        * @param aHealth ETrue if PSY list is healthy. The PSY list contains 
        *        at least one PSY.
        */
        void SetPsyListStatus( TBool aHealthy );
        
        /**
        * Increase location request count
        */
        void IncRequestCount();
        
        /**
        * Decrease location request count
        */
        void DecRequestCount();

    private:

        /**
        * C++default constructor.
        */
        CNppPsyStatusHandler( MPositionerStatus& aStatusInterface );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        
        /**
        * Update status accouding to request count
        */
        void UpdateStatus();

    private:    // Data
        //Status report interface
        MPositionerStatus& iStatusInterface;
        
        //Module satus
        TPositionModuleStatus iModuleStatus;
        
        //Request count
        TInt iRequests;

    };

#endif      // NPPPSYSTATUSHANDLER_H   
            
// End of File
