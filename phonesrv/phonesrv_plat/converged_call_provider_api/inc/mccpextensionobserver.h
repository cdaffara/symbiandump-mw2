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
* Description:  CCP extension observer. 
*
*/


#ifndef MCCPEXTENSIONOBSERVER_H
#define MCCPEXTENSIONOBSERVER_H

// INCLUDES
#include <e32base.h>

/**
* CCP extension related actions observer-class.
* Results of plug-in specific requests  or custom notifications 
* and errors are returned using this class
*
* @since S60 3.2
*/
class MCCPExtensionObserver
    {
protected:
    /** 
    * Protects the observer being deleted through the observer interface 
    * @since S60 3.2
    */
    virtual inline ~MCCPExtensionObserver() {};

public:
    /**
    * HandleExtensionEvents.
    * @since S60 3.2
    * @param aServiceId Service-id used for the event.
    * @param aEvent Event.
    * @param aStatus Status code
    * @return none
    */
    virtual void HandleExtensionEvents( TUint32 aServiceId,
                                        TInt aEvent,
                                        TInt aStatus ) = 0;
    };

#endif // MCCPEXTENSIONOBSERVER_H

// End of File
