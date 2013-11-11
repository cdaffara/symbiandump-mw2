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
* Description: Class holding icon path data from a resource file.
*
*
*/


// INCLUDE FILES
#include <EPos_Landmarks.h>
#include "EPos_CPosLmGlobalCategoryData.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryData::CPosLmGlobalCategoryData
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmGlobalCategoryData::CPosLmGlobalCategoryData() :
    CBase()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryData::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmGlobalCategoryData::ConstructL(
    TResourceReader& aReader)
    {
    iId = aReader.ReadUint32();
    iName = aReader.ReadHBufCL();
    iIconIndex = aReader.ReadInt32();
    iIconMaskIndex = aReader.ReadInt32();
    iIconPathNr = aReader.ReadInt8();

    if (iIconIndex < 0)
        {
        iIconIndex = -1;
        iIconMaskIndex = -1;
        iIconPathNr = 0;
        }
    else if (iIconMaskIndex < KPosLmIconMaskNotUsed)
        {
        iIconMaskIndex = KPosLmIconMaskNotUsed;
        }
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryData::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmGlobalCategoryData* CPosLmGlobalCategoryData::NewLC(
    TResourceReader& aReader)
    {
    CPosLmGlobalCategoryData* self = new( ELeave ) CPosLmGlobalCategoryData();
    CleanupStack::PushL(self);
    self->ConstructL(aReader);
    return self;
    }

// Destructor
CPosLmGlobalCategoryData::~CPosLmGlobalCategoryData()
    {
    delete iName;
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryData::Id
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TUint CPosLmGlobalCategoryData::Id() const
    {
    return iId;
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryData::Name
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPtrC CPosLmGlobalCategoryData::Name() const
    {
    return iName->Des();
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryData::IconIndex
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmGlobalCategoryData::IconIndex() const
    {
    return iIconIndex;
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryData::IconMaskIndex
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmGlobalCategoryData::IconMaskIndex() const
    {
    return iIconMaskIndex;
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryData::IconPathNumber
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPosLmGlobalCategoryData::IconPathNumber() const
    {
    return iIconPathNr;
    }

//  End of File
