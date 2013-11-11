/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCE extension interface class. 
*
*/


#ifndef MCCEEXTENSIONINTERFACE_H
#define MCCEEXTENSIONINTERFACE_H

//  INCLUDES
#include <e32base.h>

/**
* Plug-in extension feature provider.
* E.g telephony engine might have plugin features specific to call providers 
* functionality. Plug-in specific actions can be requested syncronous or asyncronous via this class. 
* Async message results are returned using MCCEExtensionObserver
* One instance of extension interface is used in CCE. In CCP many (one for every plug-in). One is used 
* in order to avoid using an array in CCE interface and to notify disapearance of that interface in case 
* plug-in goes down.
*
*  @since S60 3.2
*/
class MCCEExtensionInterface
    {
protected:
    /** 
    * Protected destructor. Object cannot be deleted from client.
    * @since S60 3.2
    */
    virtual ~MCCEExtensionInterface() {};
    
public:

    /**
    * Request a custom command. Results are returned via MCCEExtensionObserver.
    * Many service-ids may map to same plug-in. 
    * Request-id is used for plug-in internal id of the request. 
    * Replies use also a service-id, but if e.g. one plug-in uses two service-ids 10 and 11 
    * then request might use 10 and reply use 11. The request id is the id used to determine 
    * what the request and respone relate to. Service-id is used as routing information in CCE and 
    * its client.
    * @since S60 3.2
    * @param aServiceId To which plug-in the event targeted to. 
    * @param aRequestId Id of request.
    * @param aData Request data.
    * @return KErrNone Request was started successful.
    * @return KErrNotFound No extension interface found for given service-id.
    * @return KErrNotSupported Service-id related plug-in does not support extension interface.
    * @return Other system wide error code.
    */
    virtual TInt RequestCommand( TUint32 iServiceId,
                                 TInt aRequestId, 
                                 TDesC8& aData ) = 0;
    /**
    * Cancels request. See MCCEExtensionInterface::RequestCommand for explanation of service-id 
    * handling.
    * @since S60 3.2
    * @param aServiceId To which plug-in the event targeted to.
    * @param aRequestId Id of request to cancel. 
    * @return KErrNone Request was started successful.
    * @return KErrNotFound No extension interface found for given service-id.
    * @return KErrNotSupported Service-id related plug-in does not support extension interface.
    * @return Other system wide error code.
    */
    virtual TInt CancelRequest( TUint32 iServiceId,
                                TInt aRequestId ) = 0;
    };


#endif // MCCEEXTENSIONINTERFACE_H

