/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Retrieves and manages the emergency numbers from EnPolicy.
*
*/



#ifndef CPHSRVEMERGENCYNUMBERMANAGER_H
#define CPHSRVEMERGENCYNUMBERMANAGER_H

// INCLUDES
#include <e32base.h>                        // cbase
#include "mphsrvemergencynumbermanager.h" // interface 

// FORWARD DECLARATIONS
class MPhSrvPhoneInterface;
class CEmergencyNumberPolicy;

// CLASS DECLARATION

/**
*  Retrieves and manages the emergency numbers from EnPolicy.
*
*  @since 1.0
*/
class CPhSrvEmergencyNumberManager :
    public CBase,
    public MPhSrvEmergencyNumberManager
    {
    public:  // Constructors and destructor

        /**
        * Constructor.
        * @param aPhone a reference of phone interface.
        */
        CPhSrvEmergencyNumberManager( MPhSrvPhoneInterface& aPhone );

        /**
        * Destructor.
        */
        ~CPhSrvEmergencyNumberManager();

        /**
        * Symbian 2nd phase constructor.
        * @param aIsDosEnPolicy ETrue if Dos emergency policy is enabled.
        */
        void ConstructL( TBool aIsDosEnPolicy );


    public: // Functions from base classes

        /**
        * @see MPhSrvEmergencyNumberManager
        */
        TBool EmergencyNumberManagerIsEmergencyNumberL( 
            TDes& aNumber, 
            TPhCltEmergencyNumberQueryMode aMode );

    private:     // Data

        // Contains Phoneserver phone interfaces.
        MPhSrvPhoneInterface&          iPhone;
      
        // Contains Emergency policy DLL interface.
        RLibrary                       iEnPolicyLib;    

        // Pointer to Emergency policy interface.
        CEmergencyNumberPolicy*        iEmergencyNumberPolicy;

    };

#endif // CPHSRVEMERGENCYNUMBERMANAGER_H
            
// End of File
