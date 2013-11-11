/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Message class
*
*/

 

#ifndef TMPXMESSAGE_H
#define TMPXMESSAGE_H

#include<e32base.h>

/**
* MPX message class.
*
* @lib mpxcommon.lib
*/
class TMPXMessage
    {
public:
    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    *  @param aEvent event
    *  @param aType event type
    *  @param aData optional data 
    */
    IMPORT_C TMPXMessage(TInt aEvent,TInt aType = 0,TInt aData = 0);

    /**
    *  C++ copy constructor.
    *
    *  @since S60 3.2.3
    *  @param aMsg source message
    */
    IMPORT_C TMPXMessage(const TMPXMessage& aMsg);

    /**
    *  Overloaded assignment operator.
    *
    *  @since S60 3.2.3
    *  @param aMsg source message
    */
    IMPORT_C TMPXMessage& operator=(const TMPXMessage& aMsg);
    
    /**
    *  C++ constructor.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C TMPXMessage();

    /**
    *  return message event.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C TInt Event() const;
    
    /**
    *  return message type.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C TInt Type() const;

    /**
    *  return message data.
    *
    *  @since S60 3.2.3
    */
    IMPORT_C TInt Data() const;
protected: 
    TInt iEvent;
    TInt iType;
    TInt iData;
    };

#endif // TMPXMESSAGE_H
