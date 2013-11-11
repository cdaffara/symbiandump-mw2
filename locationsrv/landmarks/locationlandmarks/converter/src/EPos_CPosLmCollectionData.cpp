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
* Description: Class for holding collection data information while parsing
* a Landmark XML file to a landmark(s).
*
*
*/


// INCLUDE FILES
#include "EPos_CPosLmCollectionData.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPosLmCollectionData::CPosLmCollectionData(
    TPosLmCollectionDataId aId) :
    iCollectionDataId(aId)
    {
    }

// EPOC default constructor can leave.
void CPosLmCollectionData::ConstructL(
    const TDesC& aDesC)
    {
    iCollectionDataName = aDesC.AllocL();
    }

// Two-phased constructor.
CPosLmCollectionData* CPosLmCollectionData::NewLC(
    const TDesC& aDesC,
    TPosLmCollectionDataId aId)
    {
    CPosLmCollectionData* self = new (ELeave) CPosLmCollectionData(aId);
    CleanupStack::PushL(self);
    self->ConstructL(aDesC);
    return self;
    }

// Destructor
CPosLmCollectionData::~CPosLmCollectionData()
    {
    delete iCollectionDataName;
    }

// -----------------------------------------------------------------------------
// CPosLmCollectionData::GetCollectionData
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmCollectionData::GetCollectionData(TPtrC& aName) const
    {
    aName.Set(*iCollectionDataName);
    }

// -----------------------------------------------------------------------------
// CPosLmCollectionData::CollectionDataId
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TPosLmCollectionDataId CPosLmCollectionData::CollectionDataId() const
    {
    return iCollectionDataId;
    }

//  End of File
