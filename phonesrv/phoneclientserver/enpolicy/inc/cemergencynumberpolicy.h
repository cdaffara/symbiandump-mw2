/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides emergency number policy API to verify an emergency
*                number.
*
*/



#ifndef CEMERGENCYNUMBERPOLICY_H
#define CEMERGENCYNUMBERPOLICY_H

// INCLUDES
#include <e32base.h>  
#include <rphcltserver.h> 
#include <rphcltemergencycall.h> 

// CONSTANTS
const TUid KEnPolicyUid={ 0x101F86D1 };
// length of list in shared data.
const TInt KEnPolicySDEmergencyNumberListLength( 100 );
// separator between emergency numbers.
const TText KEnPolicySDNumberSeparator =  ' ';

// CLASS DECLARATION

/**
*  CEmergencyNumberPolicy
*  Description: Defines emergency number policy.
*
*  @since Series60_2.6
*/

class CEmergencyNumberPolicy : public CBase
    
    {    
    public:  // Abstract
        /**
        * Constructor.
        * @since Series60_2.6
        * @param aETelServer a reference of RTelServer.        
        */     
        virtual void ConstructL( RTelServer& aETelServer ) = 0;
    
        /**
        * Emergency number check.
        * @since Series60_2.6
        * @param aTelNumber a telephone number to be checked.
        * @param aMode a check mode, Normal/Advanced.
        * @param aIsEmergencyNumber  return True if the number is emergency.                                
        * @return error code.
        */
        virtual TInt IsEmergencyNumber( 
                                TPhCltTelephoneNumber& aTelNumber, 
                                TPhCltEmergencyNumberQueryMode aMode,
                                TBool& aIsEmergencyNumber ) = 0;
        
    };

#endif      // CEMERGENCYNUMBERPOLICY_H

// End of File
