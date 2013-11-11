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
* Description:  message observer
*
*/

 

#ifndef MMPXMESSAGEOBSERVER_H
#define MMPXMESSAGEOBSERVER_H


/**
*  Interface for receiving events.
*
*  @lib mpxcommon.lib
*/ 
NONSHARABLE_CLASS(MMPXMessageObserver)
    {
public: 
    /**
    *  Message received.
    *
    *  @since S60 3.2.3
    *  @param aMsgData message data 
    *  @param aError error code
    */
    virtual void MessageReceived(TInt aMsgData, TInt aError)=0;
    };

    
#endif // MMPXMESSAGEOBSERVER_H
