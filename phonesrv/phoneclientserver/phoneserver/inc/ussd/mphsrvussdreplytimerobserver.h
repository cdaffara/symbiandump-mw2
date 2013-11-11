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
* Description:  Abstract class which is called back when the USSD reply timer
*                expires.
*
*/


#ifndef MPHSRVUSSDREPLYTIMEROBSERVER_H
#define MPHSRVUSSDREPLYTIMEROBSERVER_H


// CLASS DECLARATION

/**
*  Abstract class which is called back when the USSD reply timer expires.
*
*  @since 1.0
*/
class MPhSrvUssdReplyTimerObserver
    {
    public: // New functions

        /**
        * Handle a timer call-back.
        * 
        * @param aError Error code.
        */
        virtual void UssdReplyTimerObserverHandleExpiredL( TInt aError ) = 0;
    };

#endif // MPHSRVUSSDREPLYTIMEROBSERVER_H


// End of File
