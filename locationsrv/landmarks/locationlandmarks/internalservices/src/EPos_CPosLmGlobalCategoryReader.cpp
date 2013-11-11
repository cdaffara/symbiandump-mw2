/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Reads global categories from a resource file.
*
*
*/


// INCLUDE FILES
#include <EPos_LmCatDef.hrh>
#include <EPos_CPosLandmarkCategory.h>
#include <EPos_PosLmImplExtension.h>
#include "EPos_PosLmFileFinder.h"
#include "EPos_CPosLmGlobalCategoryReader.h"
#include "EPos_CPosLmGlobalCategoryData.h"

// CONSTANTS
_LIT(KGlobalCategoryResourceFile, "\\Resource\\eposlmglobalcategories.rsc");
const TInt KArrayGranularity = 3;
const TInt KPosLmGlCatResourceOffset = 1;

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryReader::CPosLmGlobalCategoryReader
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmGlobalCategoryReader::CPosLmGlobalCategoryReader()
    : CBase(),
    iFileSession(),
    iResourceFile(),
    iResourceBuffer(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryReader::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CPosLmGlobalCategoryReader::ConstructL()
    {
    User::LeaveIfError(iFileSession.Connect());

    iGlCatArray = new (ELeave)
        CArrayPtrFlat<CPosLmGlobalCategoryData>(KArrayGranularity);

    TFileName* filename = new (ELeave) TFileName();
    CleanupStack::PushL(filename);

    if (PosLmFileFinder::ResourceFileL(filename, iFileSession,
        KGlobalCategoryResourceFile) != KErrNotFound)
        {
        iResourceFile.OpenL(iFileSession, *filename);

        iResourceBuffer = iResourceFile.AllocReadL(KPosLmGlCatResourceOffset);
        iResourceReader.SetBuffer(iResourceBuffer);
        iPathArray = new (ELeave) CDesCArrayFlat(KArrayGranularity);

        ReadDataL();
        }

    CleanupStack::PopAndDestroy(filename);
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryReader::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmGlobalCategoryReader* CPosLmGlobalCategoryReader::NewL()
    {
    CPosLmGlobalCategoryReader* self =
        new( ELeave ) CPosLmGlobalCategoryReader();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// Destructor
CPosLmGlobalCategoryReader::~CPosLmGlobalCategoryReader()
    {
    if (iGlCatArray)
        {
        iGlCatArray->ResetAndDestroy();
        delete iGlCatArray;
        }
    delete iPathArray;
    delete iResourceBuffer;
    iResourceFile.Close();
    iFileSession.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryReader::Count
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt CPosLmGlobalCategoryReader::Count()
    {
    return iGlCatArray->Count();
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryReader::GlobalCategoryLC
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLandmarkCategory* CPosLmGlobalCategoryReader::GlobalCategoryLC(
    TInt aIndex)
    {
    CPosLandmarkCategory* glCat = NULL;

    if (aIndex >= 0 && aIndex < Count())
        {
        CPosLmGlobalCategoryData* data = iGlCatArray->At(aIndex);

        glCat = CPosLandmarkCategory::NewLC();
        PosLmImplExtension::SetGlobalCategory(*glCat,
            (TPosLmGlobalCategory)data->Id());
        glCat->SetCategoryNameL(data->Name());

        if (data->IconIndex() != KPosLMCategoryNoIcon)
            {
            TInt iconPathNr = data->IconPathNumber();

            if (iconPathNr >= 0 && iconPathNr < iPathArray->Count())
                {
                glCat->SetIconL(iPathArray->MdcaPoint(iconPathNr),
                    data->IconIndex(), data->IconMaskIndex());
                }
            else
                {
                User::Leave(KErrCorrupt);
                }
            }
        }
    else
        {
        User::Leave(KErrArgument);
        }

    return glCat;
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryReader::ReadDataL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmGlobalCategoryReader::ReadDataL()
    {
    ReadIconPathsL();
    ReadGlobalCategoriesL();
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryReader::ReadIconPathsL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmGlobalCategoryReader::ReadIconPathsL()
    {
    // Read the size of the icon path array.
    TInt size = iResourceReader.ReadInt8();

    for (TInt i = 0; i < size; i++)
        {
        HBufC* path = iResourceReader.ReadHBufCL();
        CleanupStack::PushL(path);
        iPathArray->AppendL(*path);
        CleanupStack::PopAndDestroy(path);
        }
    }

// -----------------------------------------------------------------------------
// CPosLmGlobalCategoryReader::ReadGlobalCategoriesL
//
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPosLmGlobalCategoryReader::ReadGlobalCategoriesL()
    {
    // Read the size of the icon path array.
    TInt size = iResourceReader.ReadInt16();

    for (TInt i = 0; i < size; i++)
        {
        CPosLmGlobalCategoryData* glCat =
            CPosLmGlobalCategoryData::NewLC(iResourceReader);
        iGlCatArray->AppendL(glCat);
        CleanupStack::Pop(glCat);
        }
    }

//  End of File
