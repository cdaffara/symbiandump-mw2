/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Service provider settings notify change observer
*
*/



#ifndef MSPNOTIFYCHANGEOBSERVER_H
#define MSPNOTIFYCHANGEOBSERVER_H

#include <e32base.h>
#include "spdefinitions.h"


/**
 *  Call back interface for notify change events
 *
 *  Client has to implement this interface if client 
 *  need to receive settings notification change.
 *
 *  @lib serviceprovidersettings.lib
 *  @since S60 V3.2
 */
class MSPNotifyChangeObserver
    {
    public:

        /**
         * Handle notify change event
         * 
         * @param aServiceId the service ID of added/changed/deleted service
         * @since S60 3.2
         */
        virtual void HandleNotifyChange( TServiceId aServiceId ) = 0;
        
        /**
         * Handle error
         * 
         * @param aError error code
         * @since S60 3.2
         */
        virtual void HandleError( TInt aError ) = 0;
    };

#endif // MSPNOTIFYCHANGEOBSERVER_H