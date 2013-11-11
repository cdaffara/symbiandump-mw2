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
* Description: Implementation of CPosLmDatabaseManagerPluginBase class
*
*
*/


// INCLUDE FILES
#include <ecom/ecom.h>
#include "EPos_CPosLmDatabaseManagerPluginBase.h"
#include "EPos_CPosLmDbManPluginBaseExtension.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//

EXPORT_C CPosLmDatabaseManagerPluginBase::CPosLmDatabaseManagerPluginBase()
   {
   }

// Destructor
EXPORT_C CPosLmDatabaseManagerPluginBase::~CPosLmDatabaseManagerPluginBase()
    {
    delete iExtension;
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerPluginBase::BaseConstructL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void CPosLmDatabaseManagerPluginBase::BaseConstructL(
    TAny* aConstructionParameters)
    {
    iExtension = new(ELeave) CPosLmDbManPluginBaseExtension(
    reinterpret_cast<RPosLandmarkServer*>(aConstructionParameters));
    }

// ---------------------------------------------------------
// CPosLmDatabaseManagerPluginBase::Session
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C RPosLandmarkServer& CPosLmDatabaseManagerPluginBase::Session() const
    {
    return *(iExtension->iSession);
    }

//  End of File

