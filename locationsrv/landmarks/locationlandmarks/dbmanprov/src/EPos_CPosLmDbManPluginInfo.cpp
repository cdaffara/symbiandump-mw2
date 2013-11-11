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
* Description: This class is a placeholder for a database manager plugin and its
*  associated destructor id key.
*
*
*/


// INCLUDE FILES
#include <ecom/ecom.h>
#include <EPos_CPosLmDatabaseManagerPluginBase.h>
#include "EPos_CPosLmDbManPluginInfo.h"


// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmDbManPluginInfo::CPosLmDbManPluginInfo
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDbManPluginInfo::CPosLmDbManPluginInfo()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmDbManPluginInfo::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmDbManPluginInfo::ConstructL(
    TUid aImplUid,
    TAny* aConstructionParams)
    {
    iPlugin = reinterpret_cast<CPosLmDatabaseManagerPluginBase*>
    (REComSession::CreateImplementationL(aImplUid,
                                         iDtorIdKey,
                                         aConstructionParams));
    }

// -----------------------------------------------------------------------------
// CPosLmDbManPluginInfo::NewLC
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmDbManPluginInfo* CPosLmDbManPluginInfo::NewLC(
    TUid aImplUid,
    TAny* aConstructionParams)
    {
    CPosLmDbManPluginInfo* self = new(ELeave) CPosLmDbManPluginInfo();
    CleanupStack::PushL(self);
    self->ConstructL(aImplUid, aConstructionParams);
    return self;
    }


// Destructor
CPosLmDbManPluginInfo::~CPosLmDbManPluginInfo()
    {
    delete iPlugin;
    REComSession::DestroyedImplementation(iDtorIdKey);
    }


// ---------------------------------------------------------
// CPosLmDbManPluginInfo::Instance
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
CPosLmDatabaseManagerPluginBase* CPosLmDbManPluginInfo::Instance()
    {
    return iPlugin;
    }


// End of File
