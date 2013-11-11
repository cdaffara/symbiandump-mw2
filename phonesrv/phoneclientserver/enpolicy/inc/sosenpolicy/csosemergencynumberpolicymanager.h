/*
* Copyright (c) 2004-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides SOS emergency number policy API.
*
*/



#ifndef CSOSEMERGENCYNUMBERPOLICYMANAGER_H
#define CSOSEMERGENCYNUMBERPOLICYMANAGER_H

// INCLUDES
#include <cemergencynumberpolicy.h> 

// FORWARD DECLARATIONS
class CSosEmergencyNumberPolicyHandler;
class CSosEnPolicySatRefresh;

// CLASS DECLARATION

/**
*  CSosEmergencyNumberPolicyManager
*  Description: Defines SOS emergency number policy.
*
*  @since Series60_2.6
*/

NONSHARABLE_CLASS( CSosEmergencyNumberPolicyManager ) : 
    public CEmergencyNumberPolicy
    {
    public:  // Constructors and destructor
      
        /**
        * Symbian 2nd phase constructor is private.
        * @param aETelServer a reference of RTelServer.
        */
        void ConstructL(  RTelServer& aETelServer );

        /**
        * Destructor.
        */
        ~CSosEmergencyNumberPolicyManager();
        
    public: // New
        /**
        * Emergency number check.
        * @param aTelNumber a telephone number to be checked.
        * @param aMode a check mode, Normal/Advanced.
        * @param aIsEmergencyNumber  return True if the number is emergency.                                
        * @return error code.
        */
        TInt IsEmergencyNumber( 
                                TPhCltTelephoneNumber& aTelNumber, 
                                TPhCltEmergencyNumberQueryMode aMode,
                                TBool& aIsEmergencyNumber );  
           
    private:// Data

        // Pointer to the instance of SOS emergency number handler.
        CSosEmergencyNumberPolicyHandler* iSosEnPolicyHandler;

    };

#endif  // CSOSEMERGENCYNUMBERPOLICYMANAGER_H
// End of File
