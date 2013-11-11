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
* Description:  Implementation of the class CCbsMessageClient.
*
*/


// INCLUDE FILES
#include <ccbsmessageclient.h>
#include "ccbsmessageclientimpl.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCbsMessageClient::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CCbsMessageClient* CCbsMessageClient::NewL()
    {
    return CCbsMessageClientImpl::NewL();
    }

// -----------------------------------------------------------------------------
// CCbsMessageClient::CCbsMessageClient
// C++ constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CCbsMessageClient::CCbsMessageClient()
    {
    }



//  End of File  
