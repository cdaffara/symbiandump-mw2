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
* Description: This class is only used internally in CPosLmMultiDbSearch.
*
*
*/


// INCLUDE FILES
#include    <EPos_CPosLandmarkDatabase.h>
#include    <EPos_CPosLandmarkSearch.h>
#include    <EPos_CPosLmDisplayData.h>
#include    "EPos_CPosLmMultiDbSearchItem.h"

// CONSTANTS

// ================= MEMBER FUNCTIONS =======================

// -----------------------------------------------------------------------------
// CPosLmMultiDbSearchItem::CPosLmMultiDbSearchItem
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmMultiDbSearchItem::CPosLmMultiDbSearchItem(HBufC* aDbUriPtr)
    : iDbUriPtr(aDbUriPtr)
    {
    }

// Destructor
CPosLmMultiDbSearchItem::~CPosLmMultiDbSearchItem()
    {
    delete iSearcher;
    delete iDisplayData;
    delete iDb;
    delete iDbUriPtr;
    }

//  End of File
