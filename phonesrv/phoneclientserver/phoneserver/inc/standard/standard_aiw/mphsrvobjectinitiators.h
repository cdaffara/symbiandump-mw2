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
* Description:  Object Initiators.
*
*/


#ifndef MPHSRVOBJECTINITIATORS_H
#define MPHSRVOBJECTINITIATORS_H


// INCLUDES
#include <e32base.h>


// FORWARD DECLARATIONS
class CPhCltExtPhoneDialData;

// CLASS DECLARATION

/**
*  Initiator functions class.
*
*  @since 1.0 
*/
class MPhSrvInitiatorCall
    {
    public: // New functions

        /**
        * Called by the call manager when it wants to make a call (in response
        * to a request from the external call interface).
        *
        * @param aArgs The call parameters.
        */
        virtual void InitiatorMakeAsynchronousCallL( 
            CPhCltExtPhoneDialData& aArgs ) =0;
        /**
        * Called by the call manager to find out if there is a registered
        * notification handler.
        *
        * @return ETrue if there is pending notification request.
        */
        virtual TBool InitiatorIsReadyToMakeCall() const = 0;
       
    };

#endif // MPHSRVOBJECTINITIATORS_H


// End of File
