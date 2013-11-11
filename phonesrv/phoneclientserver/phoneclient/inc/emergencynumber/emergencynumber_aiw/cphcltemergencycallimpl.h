/*
* Copyright (c) 2002-2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Client side access to emergency call
*
*/


#ifndef CPHCLTEMERGENCYCALLIMPL_H
#define CPHCLTEMERGENCYCALLIMPL_H

//  INCLUDES
#include <e32std.h>
#include "rphcltserver.h" 
#include "cphcltemergencycall.h" 
#include "rphcltemergencycall.h" 


// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
*  Client side access to emergency call
*
*  @lib phoneclient.lib
*  @since Series60_1.0
*/
NONSHARABLE_CLASS( CPhCltEmergencyCallImpl ) : public CPhCltEmergencyCall
    {
    public: // Constructors and destructors.

        static CPhCltEmergencyCallImpl* NewL( 
            MPhCltEmergencyCallObserver* aObserver );
        
        /**
        * Destructor.
        */
        virtual ~CPhCltEmergencyCallImpl();

    public: // Emergency number API

        /**
        * @See CPhCltEmergencyCall 
        */
        virtual TInt IsEmergencyPhoneNumber(
            const TDesC& aNumber, 
            TBool& aIsEmergencyNumber );

        /**
        * @See CPhCltEmergencyCall
        */
        virtual TInt FindEmergencyPhoneNumber(
            TDes& aNumber, 
            TBool& aIsEmergencyNumber );

        /**
        * @See CPhCltEmergencyCall
        */
        virtual void DialEmergencyCallL( const TDesC& aNumber );
        
        /**
        * @See CPhCltEmergencyCall
        */
        virtual TInt IsEmergencyPhoneNumber(
            const TDesC& aNumber, 
            TDes& aMatchedEmergencyNumber, 
            TBool& aIsEmergencyNumber );                    

    private:
    
        /**
        * C++ constructor.
        */
        CPhCltEmergencyCallImpl( MPhCltEmergencyCallObserver* aObserver );
        
        /**
        * Symbian OS constructor.
        */
        void ConstructL();
        
        /**
        * From CActive, it is called when object is active and 
        * request has been processed.
        */
        void RunL();

        /**
        * From CActive, calcels pending request.
        */
        void DoCancel();
        
        /**
        * Check that active object is not active and subsession
        * before attempting a dial
        */
        void DialEmergencyPreconditionCheckL();
        
    private:
        
        // RClass instance
        RPhCltEmergencyCall iEmergencyCall;
        
        // wrapper for emergencynumber
        TPtrC8 iEmergencyCallNumberPtr;
        
        //RPhCltServer
        RPhCltServer iServer;
        
       //EmergencyCallNumber
       TPhCltEmergencyNumber iEmergencyCallNumber;
    };

#endif // CPHCLTEMERGENCYCALLIMPL_H

// End of File
