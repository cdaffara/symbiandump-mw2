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
* Description: class is used to manage landmark databases.
*
*
*/


// INCLUDE FILES
#include <ecom/ecom.h>
#include "EPos_CPosLmDatabaseManager.h"
#include "EPos_LandmarksDbManagerUids.hrh"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C CPosLmDatabaseManager::CPosLmDatabaseManager()
    {
    }

// Two-phased constructor.
EXPORT_C CPosLmDatabaseManager* CPosLmDatabaseManager::NewL()
    {
    TUid implementationUid;
    implementationUid.iUid = KPosLmDbManagerImplUid;

    TAny* ptr = REComSession::CreateImplementationL(
        implementationUid,
        _FOFF(CPosLmDatabaseManager, iDtorIdKey));

    return reinterpret_cast<CPosLmDatabaseManager*>(ptr);
    }

// Destructor
EXPORT_C CPosLmDatabaseManager::~CPosLmDatabaseManager()
    {
    // Destroy any instance variables and then inform the framework that this
    // specific instance of the interface has been destroyed.
    REComSession::DestroyedImplementation(iDtorIdKey);
    }

//  End of File
