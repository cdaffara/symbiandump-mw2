/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Contains a dummy implementation of the CCentralRepositoryHandler class. 
*
*/


//  INCLUDE FILES
#include "radiofmpresetcentralrepositoryhandler.h"
#include "trace.h"


// ============================ MEMBER FUNCTIONS ===============================

// ----------------------------------------------------
// CCentralRepositoryHandler::CCentralRepositoryHandler
// Default class constructor.
// ----------------------------------------------------
//
CCentralRepositoryHandler::CCentralRepositoryHandler()
    {
    FUNC_LOG;
    }


// ----------------------------------------------------
// CCentralRepositoryHandler::NewL
// Two-phased class constructor.
// ----------------------------------------------------
//
CCentralRepositoryHandler* CCentralRepositoryHandler::NewL()
    {
    FUNC_LOG;
    CCentralRepositoryHandler *self = new(ELeave) CCentralRepositoryHandler();
    return self;
    }

// ----------------------------------------------------
// CCentralRepositoryHandler::~CCentralRepositoryHandler
// Destructor of CCentralRepositoryHandler class.
// ----------------------------------------------------
//
CCentralRepositoryHandler::~CCentralRepositoryHandler()
    {
    FUNC_LOG;
    }

// end of file
