/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Sub Session Factory.
*
*/


#ifndef PHSRVSUBSESSIONFACTORY_H
#define PHSRVSUBSESSIONFACTORY_H

// INCLUDES
#include <e32std.h>


// FORWARD DECLARATIONS
class CPhSrvSession;
class CPhSrvSubSessionBase;


// CLASS DECLARATION

/**
*  Subsession creation factory.
*
*  @since 1.0
*/
class PhSrvSubSessionFactory
    {
    public: // New functions

        /**
        * Whether or not a specified function should result in a new 
        * subsession being created.
        *
        * @param aFunction Function to be evaluated.
        * @return ETrue if the function (opcode) requires a new subsession.
        */
        static TBool PhSrvSubSessionFactoryIsCreationFunction(TInt aFunction);

        /**
        * Create a new subsession according to the function (opcode) type.
        *
        * @param aFunction Function to be created.
        * @param aSession Session where the subsession belongs.
        * @return A new subsession, left on the cleanup stack.
        */
        static CPhSrvSubSessionBase* PhSrvSubSessionFactoryCreateLC( 
            TInt aFunction, 
            CPhSrvSession& aSession );
            
        /**
        * Check Secure ID for a new subsession.
        *
        * @param aFunction Function to be created.
        * @param aSID Secure ID of the process
        * @return ETrue if a new subsession creation is allowed
        */     
        static TBool PhSrvSubSessionFactoryIsCreationAllowed(
            TInt aFunction,
            TUint32 aSID );   
    
    };

#endif      // PHSRVSUBSESSIONFACTORY_H


// End of File
