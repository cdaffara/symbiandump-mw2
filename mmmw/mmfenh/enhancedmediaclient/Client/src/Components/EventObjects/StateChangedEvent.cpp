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
* Description:  Implementation of StateChangedEvent class.
*
*/


#include "StateChangedEvent.h"

using namespace multimedia;

CStateChangedEvent::CStateChangedEvent(
    MStreamControl::TStreamState aState,
    TInt aReason):
    iState(aState),
    iReason(aReason)
    {
    // No impl
    }

CStateChangedEvent::~CStateChangedEvent()
    {
    }

TAny* CStateChangedEvent::GetInterface()
    {
    MStateChangedEvent* interface = this;
    return interface;
    }

MStreamControl::TStreamState CStateChangedEvent::GetState()
    {
    return iState;
    }

TInt CStateChangedEvent::GetErrorCode()
    {
    return iReason;        
    }
// End of file
