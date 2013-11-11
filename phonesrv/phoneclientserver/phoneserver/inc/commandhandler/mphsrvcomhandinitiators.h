/*
* Copyright (c) 2003-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Command Handler Object Initiators.
*
*/


#ifndef MPHSRVCOMHANDINITIATORS_H
#define MPHSRVCOMHANDINITIATORS_H


// INCLUDES
#include <e32base.h>
#include "phcltclientserver.h" 
#include <cphcltcommandhandler.h> 
// FORWARD DECLARATIONS



// CLASS DECLARATION

/**
*  Initiator functions class.
*
*  @since 2.6
*/
class MPhSrvComHandInitiator
    {
    public: // New functions

        /**
        * Called by the command handler manager when it wants to make a request,
        * in response to a request from the external command handler interface.
        *
        * @param aParams The command handler parameters.
        */
        virtual void InitiatorMakeAsynchronousComHandRequest( 
            const TPhCltComHandCommandParameters& aParams ) = 0;

        /**
        * Called by the command handler manager to find out if there is a 
        * registered notification handler.
        *
        * @return ETrue if there is pending notification request.
        */
        virtual TBool InitiatorIsReadyToPerformRequest() const = 0;
    };

#endif // MPHSRVCOMHANDINITIATORS_H


// End of File
