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
* Description: encapsulates the attributes that can be set for
*  a landmark database.
*
*
*/


// INCLUDE FILES
#include    "EPos_TPosLmDatabaseSettings.h"
#include    <EPos_LandmarksErrors.h>



// CONSTANTS


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
EXPORT_C TPosLmDatabaseSettings::TPosLmDatabaseSettings()
    {
    iAttributes = 0;
    iName = KNullDesC;
    TPtr8 buf(reinterpret_cast<TUint8*> (iReserved),
        KPOSLMDATABASESETTINGSRESERVEDSIZE,
        KPOSLMDATABASESETTINGSRESERVEDSIZE);
    buf.FillZ();
    }


// ---------------------------------------------------------
// TPosLmDatabaseSettings::IsAttributeSet
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TBool TPosLmDatabaseSettings::IsAttributeSet(
    TAttribute aDbAttribute) const
    {
    __ASSERT_ALWAYS(aDbAttribute == EName,
                    Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));
    return ((iAttributes & aDbAttribute) != 0);
    }

// ---------------------------------------------------------
// TPosLmDatabaseSettings::UnsetAttribute
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPosLmDatabaseSettings::UnsetAttribute(
    TAttribute aDbAttribute)
    {
    __ASSERT_ALWAYS(aDbAttribute == EName,
                    Panic(KPosLandmarksClientPanic, EPosInvalidEnumValue));
    iAttributes &= ~aDbAttribute;
    }

// ---------------------------------------------------------
// TPosLmDatabaseSettings::DatabaseName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TPtrC TPosLmDatabaseSettings::DatabaseName() const
    {
    return IsAttributeSet(EName) ? TPtrC(iName) : TPtrC();
    }

// ---------------------------------------------------------
// TPosLmDatabaseSettings::SetDatabaseName
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void TPosLmDatabaseSettings::SetDatabaseName(
    const TPosLmDatabaseName& aDatabaseName)
    {
    iAttributes |= EName;
    iName = aDatabaseName;
    }

//  End of File
