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
* Description:  implementation of message 
*
*/

#include "mpxmessage.h"

// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
EXPORT_C TMPXMessage::TMPXMessage(TInt aEvent,
                                TInt aType /*= 0*/,
                                TInt aData /*= 0*/)
    : iEvent(aEvent),iType(aType),iData(aData)
    {}
        
// -----------------------------------------------------------------------------
// C++ constructor
// -----------------------------------------------------------------------------
//
EXPORT_C TMPXMessage::TMPXMessage()
    : iEvent(0),iType(0),iData(0)
    {} 

// -----------------------------------------------------------------------------
// Copy constructor
// -----------------------------------------------------------------------------
//
EXPORT_C TMPXMessage::TMPXMessage(const TMPXMessage& aMsg)
    : iEvent(aMsg.iEvent), iType(aMsg.iType),iData(aMsg.iData) 
    {
    }

// -----------------------------------------------------------------------------
// Overloaded assignment operator
// -----------------------------------------------------------------------------
//
EXPORT_C TMPXMessage& TMPXMessage::operator=(const TMPXMessage& aMsg)
    {
    if (this != &aMsg)
        {
        iEvent = aMsg.iEvent;
        iType = aMsg.iType;
        iData = aMsg.iData;
        }
    return *this;    
    }
    
// -----------------------------------------------------------------------------
// Message event
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMPXMessage::Event() const
    {
    return iEvent;
    }

// -----------------------------------------------------------------------------
// Message type
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMPXMessage::Type() const
    {
    return iType;
    }

// -----------------------------------------------------------------------------
// Message data
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TMPXMessage::Data() const
    {
    return iData;
    }
    

