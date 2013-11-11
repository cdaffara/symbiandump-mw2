/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Interface for subsessions to get send event.
*
*/


#ifndef MPHSRVUSSDMESSAGESENTOBSERVER_H
#define MPHSRVUSSDMESSAGESENTOBSERVER_H

// CLASS DECLARATIONS

/**
*  USSD message send event observer.
*
*  @since 1.0
*/
class MPhSrvUssdMessageSentObserver
    {
    public: // New functions

        /**
        * Called by the USSD manager when it's sent a message.
        * 
        * @param aError Complete code.
        */
        virtual void UssdMessageSentObserverHandleResult( TInt aError ) = 0;
    };

#endif // MPHSRVUSSDMESSAGESENTOBSERVER_H


// End of File
