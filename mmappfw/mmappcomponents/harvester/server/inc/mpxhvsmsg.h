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
* Description:  Encapsulates a message to send
*
*/



#ifndef RHVSMSG_H
#define RHVSMSG_H

/**
* Encapsulates an outstanding request. Allows the client to have multiple
* asynchronous methods outstanding
*/
class RHvsMsg
    {
public:
    /**
    *  C++ constructor
    *
    *  @param aMsg message object
    *  @param aOp server function code
    */
    inline RHvsMsg(const RMessage2 aMsg,TMPXHarvesterServerOp aOp);
    

    /**
    *  Return message reference
    */
    inline const RMessage2& Message();
    
    /**
    * Return the op
    */
    inline const TMPXHarvesterServerOp Op();
    
private:
    RMessage2 iMsg;
    TMPXHarvesterServerOp iOp;
    };

#include "mpxhvsmsg.inl"

#endif // RHVSMSG_H
