/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Base class for subsessions.
*
*
*/


#include "EPos_CPosLmSubsessionBase.h"

// -----------------------------------------------------------------------------
// CPosLmSubsessionBase::CPosLmSubsessionBase
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmSubsessionBase::CPosLmSubsessionBase(
    TSubsessionType aSubsessionType,
    CPosLmServer* aLmServer) :
    iSubsessionType(aSubsessionType),
    iLmServer(aLmServer)
    {
    }

// Destructor
CPosLmSubsessionBase::~CPosLmSubsessionBase()
    {
    }

CPosLmSubsessionBase::TSubsessionType CPosLmSubsessionBase::SubsessionType(
    ) const
    {
    return iSubsessionType;
    }

// -----------------------------------------------------------------------------
// CPosLmSubsessionBase::Server
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLmServer* CPosLmSubsessionBase::Server()
    {
    return iLmServer;
    }

// End of File
