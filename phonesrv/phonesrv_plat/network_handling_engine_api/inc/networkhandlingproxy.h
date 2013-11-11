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
* Description:  This file contains the header file of the class NetworkHandlingProxy.
*
*/


#ifndef NETWORKHANDLINGPROXY_H
#define NETWORKHANDLINGPROXY_H

//  INCLUDES
#include "nwhandlingengine.h" 


// FORWARD DECLARATIONS
class CNWSession;
class MNWMessageObserver;

    
        /**
        *  Creates protocol specific Network handling.
        *  @param 
        *  @param 
        *  @return networkHandling: Instance of the NetworkHandling
        */
        IMPORT_C CNWSession* CreateL( MNWMessageObserver& aMessage,
                                     TNWInfo& aTNWInfo );



#endif      // NetworkHandlingProxy_H  
            
// End of File
