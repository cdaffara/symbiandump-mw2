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
* Description:  CCP test plug-in implementation.
*
*/


#ifndef CCPREFEXTENSIONOBSERVER_H
#define CCREFPEXTENSIONOBSERVER_H

// INCLUDES
#include <e32base.h>
#include <mccpextensionobserver.h>


/**
* Implemented by CCE
*/
class CCPRefExtensionObserver : public CBase,
                                public MCCPExtensionObserver
    {
public:

    ~CCPRefExtensionObserver();
         
    /**
    * HandleExtensionEvents.
    * @since S60 3.2
    * @param aEvent Event type
    * @param aStatus Status code
    */
    void HandleExtensionEvents( const TInt aEvent,
                                const TInt aStatus );
    };

#endif      // CCPREFEXTENSIONOBSERVER_H

// End of File

