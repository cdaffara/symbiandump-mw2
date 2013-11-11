/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Emergency Number Manager.
*
*/


#ifndef MPHSRVEMERGENCYNUMBERMANAGER_H
#define MPHSRVEMERGENCYNUMBERMANAGER_H

// INCLUDES
#include "phcltclientserver.h" 
#include <rphcltemergencycall.h> 

// FORWARD DECLARATIONS


// CLASS DECLARATION

/**
*  The interface to the emergency number manager.
*
*  @since 1.0 
*/
class MPhSrvEmergencyNumberManager
    {
    public:  // New functions

        /**
        * Checks aNumber to see if it qualifies as an emergency number.
        * 
        * @param aNumber Number to be checked.
        * @param aMode Emergency number query check mode.
        * @return True if the specified number is an emergency number.
        */
        virtual TBool EmergencyNumberManagerIsEmergencyNumberL(
            TDes& aNumber, 
            TPhCltEmergencyNumberQueryMode aMode ) = 0;
    };


#endif // MPHSRVEMERGENCYNUMBERMANAGER_H


// End of File
