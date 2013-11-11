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
* Description: Class containing static exported functions needed by the implementation
*  of components in the Landmarks subsystem.
*
*
*/


// INCLUDE FILES
#include    "EPos_PosLmDbInfoExtension.h"
#include    "EPos_HPosLmDatabaseInfo.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// ---------------------------------------------------------
// PosLmDbInfoExtension::SetDefault
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void PosLmDbInfoExtension::SetDefault(
    HPosLmDatabaseInfo& aDatabaseInfo,
    TBool aIsDefault)
    {
    aDatabaseInfo.SetDefault(aIsDefault);
    }

// ---------------------------------------------------------
// PosLmDbInfoExtension::SetMediaType
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void PosLmDbInfoExtension::SetMediaType(
    HPosLmDatabaseInfo& aDatabaseInfo,
    TMediaType aMediaType)
    {
    aDatabaseInfo.SetMediaType(aMediaType);
    }

// ---------------------------------------------------------
// PosLmDbInfoExtension::SetDatabaseDrive
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void PosLmDbInfoExtension::SetDatabaseDrive(
    HPosLmDatabaseInfo& aDatabaseInfo,
    TChar aDatabaseDrive)
    {
    aDatabaseInfo.SetDatabaseDrive(aDatabaseDrive);
    }

//  End of File
