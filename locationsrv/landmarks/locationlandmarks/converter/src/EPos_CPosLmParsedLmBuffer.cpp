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
* Description: Class for holding landmarks and landmark categories while parsing
* a Landmark XML file to a landmark(s).
*
*
*/


// INCLUDE FILES
#include "EPos_CPosLmParsedLmBuffer.h"

// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CPosLmParsedLmBuffer::CPosLmParsedLmBuffer()
    {
    }

// EPOC default constructor can leave.
void CPosLmParsedLmBuffer::ConstructL(
    CPosLandmark& aLm,
    RPointerArray<CPosLandmarkCategory>& aLmCategories)
    {
    iLandmark = CPosLandmark::NewL(aLm);
    for (TInt i=0; i<aLmCategories.Count(); i++)
        {
        User::LeaveIfError(iLandmarkCategories.Append(aLmCategories[i]));
        }
    }

// Two-phased constructor.
CPosLmParsedLmBuffer* CPosLmParsedLmBuffer::NewLC(
    CPosLandmark& aLm,
    RPointerArray<CPosLandmarkCategory>& aLmCategories)
    {
    CPosLmParsedLmBuffer* self = new (ELeave) CPosLmParsedLmBuffer();
    CleanupStack::PushL(self);
    self->ConstructL(aLm, aLmCategories);
    return self;
    }

// Destructor
CPosLmParsedLmBuffer::~CPosLmParsedLmBuffer()
    {
    delete iLandmark;
    iLandmarkCategories.ResetAndDestroy();
    iLandmarkCategories.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmParsedLmBuffer::Landmark
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPosLandmark* CPosLmParsedLmBuffer::Landmark() const
    {
    return iLandmark;
    }

// -----------------------------------------------------------------------------
// CPosLmParsedLmBuffer::Categories
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
RPointerArray<CPosLandmarkCategory> CPosLmParsedLmBuffer::Categories() const
    {
    return iLandmarkCategories;
    }

//  End of File
