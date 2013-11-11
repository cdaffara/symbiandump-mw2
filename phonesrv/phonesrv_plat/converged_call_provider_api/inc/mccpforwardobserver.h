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
* Description:  CCP Call Forward observer
*
*/


#ifndef MCCPFORWARDOBSERVER_H
#define MCCPFORWARDOBSERVER_H

// INCLUDES
#include <e32base.h>
#include "mccpcall.h"


/**
* CCP API call forward observer class
*
*  @since S60 3.2
*/
class MCCPForwardObserver
    {
public: 
    enum TCCPForwardEvent
        {
        /** Call is being forwarded by the remote party. */
        ECCPRemoteForwarding,
        /** Call is forwarded */
        ECCPForwarded,
        /** Multiple call forward choices are available */
        ECCPMultipleChoices,
        /** Call moved temporarily */
        ECCPMovedTemporarily,
        /** Call moved permanently */
        ECCPMovedPermanentlyEvent
        };

protected:
    /** 
    * Protects the observer being deleted through the observer interface 
    * @since S60 3.2
    */
    virtual inline ~MCCPForwardObserver() {};
    
public:
    /**
    * A Forward event has occurred concerning a specific call.
    * @since S60 3.2
    * @param aEvent Occurred event.
    * @return none
    */
    virtual void ForwardEventOccurred( 
                 const MCCPForwardObserver::TCCPForwardEvent aEvent ) = 0;
    };

#endif // MCCPFORWARDOBSERVER_H

// End of File
