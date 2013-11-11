/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Virtual class defining an interface for providing topic subscriptions
*                information and routing CB messages.
*
*/




#ifndef MCBSMCNSUBSCRIPTIONSPROVIDER_H
#define MCBSMCNSUBSCRIPTIONSPROVIDER_H

//  INCLUDES

#include <e32base.h> 

//  FORWARD DECLARATIONS

class CCbsMessage;

//  CLASS DECLARATION 

/**
*   This class defines the interface for providing CB topic subscriptions.
*/
class MCbsMcnSubscriptionsProvider
    {
    public:        // New functions
        /*
        *   Returns the number of topic subscriptions of this MCN client.
        *
        *   @return                     Number of subscribed CB topics
        */
        virtual TUint NumberOfSubscriptions() const = 0;

        /**
        *   Route CB message given in aMessage to a MCN client.
		*	@param aMessage The CBS message to be routed
        */
        virtual void RouteMessageL( const CCbsMessage& aMessage ) = 0;
    };

#endif      //  MCBSMCNSUBSCRIPTIONSPROVIDER_H   
            
// End of File
