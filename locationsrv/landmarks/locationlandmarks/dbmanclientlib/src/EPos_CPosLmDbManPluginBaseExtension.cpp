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
* Description: This class is the extension class for  CPosLmDatabaseManagerPluginBase
*  which is needed for preserving binary compatibility.
*
*
*/


// INCLUDE FILES
#include <ecom/ecom.h>
#include "EPos_CPosLmDbManPluginBaseExtension.h"

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmDbManPluginBaseExtension::CPosLmDbManPluginBaseExtension
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDbManPluginBaseExtension::CPosLmDbManPluginBaseExtension(
RPosLandmarkServer* aSession)
   : iSession(aSession)
   {
   }

// Destructor
CPosLmDbManPluginBaseExtension::~CPosLmDbManPluginBaseExtension()
    {
    }



//  End of File

