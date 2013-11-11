/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Factory function to create the harvester
*
*/


#include <e32base.h>
#include "mpxharvesterutility.h"
#include "mpxharvesterutilityimp.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Two Phased Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C MMPXHarvesterUtility* CMPXHarvesterFactory::NewL()
    {
    return CMPXHarvesterUtilityImp::NewL();
    }

