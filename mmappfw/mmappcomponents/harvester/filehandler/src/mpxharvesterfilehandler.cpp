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
* Description:  Factory method to construct a file handler
*
*/


#include <e32base.h>
#include <f32file.h>
#include "mpxharvesterfilehandler.h"
#include "mpxharvesterfilehandlerimp.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two-Phased Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CMPXHarvesterFileHandler* CMPXHarvesterFileHandler::NewL( RFs& aFs )
    {
    return CMPXHarvesterFileHandlerImp::NewL(aFs);
    }
    
// ---------------------------------------------------------------------------
// Virtual Destructor
// ---------------------------------------------------------------------------
//
CMPXHarvesterFileHandler::~CMPXHarvesterFileHandler()
    {
    // do nothing
    }

// END OF FILE
