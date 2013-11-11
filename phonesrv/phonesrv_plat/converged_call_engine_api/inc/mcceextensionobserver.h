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


#ifndef MCCEEXTENSIONOBSERVER_H
#define MCCEEXTENSIONOBSERVER_H

// INCLUDES
#include <e32base.h>

/**
* Extension related actions observer-class.
* Results of plug-in specific requests  or custom notifications 
* and errors are returned using this class.
*
* @since S60 3.2
*/
class MCCEExtensionObserver
    {
protected:
    /** 
    * Protects the observer being deleted through the observer interface 
    * @since S60 3.2
    */
    virtual ~MCCEExtensionObserver() {};

public:
    /**
    * HandleExtensionEvents.
    * @since S60 3.2
    * @param aServiceId From which plug-in the event originated from.
    * @param aEvent Event type
    * @param aStatus Status code
    * @return none
    */
    virtual void HandleExtensionEvents( TUint32 aServiceId,
                                        TInt aEvent,
                                        TInt aStatus ) = 0;
    };

#endif // MCCEEXTENSIONOBSERVER_H

// End of File
