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
* Description: Imported landmark iterator.
*
*
*/


// INCLUDE FILES
#include "EPos_CPosLmLocalImportedLmIterator.h"
#include "EPos_CPosLmLocalImportOp.h"
#include "EPos_CPosLmLocalInternalProxyOp.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmLocalImportedLmIterator::CPosLmLocalImportedLmIterator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmLocalImportedLmIterator::CPosLmLocalImportedLmIterator() :
    CPosLmLocalIterator()
    {
    }

// -----------------------------------------------------------------------------
// CPosLmLocalImportedLmIterator::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmLocalImportedLmIterator::ConstructL(
    CPosLmOperation* aImportOperation)
    {
    CPosLmLocalImportOp* importOp = static_cast<CPosLmLocalImportOp*>(
        static_cast<CPosLmLocalInternalProxyOp*>(
        aImportOperation)->Operation());

    importOp->ImportedLandmarkItemIdsL(iIdArray);
    }

// -----------------------------------------------------------------------------
// CPosLmLocalImportedLmIterator::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CPosLmLocalImportedLmIterator* CPosLmLocalImportedLmIterator::NewL(
    CPosLmOperation* aImportOperation)
    {
    CPosLmLocalImportedLmIterator* self =
        new(ELeave) CPosLmLocalImportedLmIterator();
    CleanupStack::PushL(self);
    self->ConstructL(aImportOperation);
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmLocalImportedLmIterator::~CPosLmLocalImportedLmIterator()
    {
    }

//  End of File
