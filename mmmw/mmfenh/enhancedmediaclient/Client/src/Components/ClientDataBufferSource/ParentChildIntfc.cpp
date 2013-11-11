/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of Parent Child Interface.
*
*/


#include "ParentChildIntfc.h"

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
CParentIntfc::~CParentIntfc()
    {
    TInt count = iChildrenList.Count();
    CChildIntfc* childPtr(NULL);
    for (TInt index(0); index < count; index++)
        {
        if (GetChild(index, childPtr) == KErrNone)
            {
            childPtr->ParentDeleted(*((CParentIntfc*)this));
            }
        }
    iChildrenList.Close();
    }

// ---------------------------------------------------------------------------
// GetChildCount
// ---------------------------------------------------------------------------
TInt CParentIntfc::GetChildCount()
    {
    return iChildrenList.Count();
    }

// ---------------------------------------------------------------------------
// GetChild
// ---------------------------------------------------------------------------
TInt CParentIntfc::GetChild( TInt aIndex, CChildIntfc*& aChild )
    {
    TInt status(KErrArgument);
    if ( (0 <= aIndex ) && ( aIndex < iChildrenList.Count() ) )
        {
        aChild = iChildrenList[aIndex];
        status = KErrNone;
        }
    return status;
    }

// ---------------------------------------------------------------------------
// SetChild
// ---------------------------------------------------------------------------
TInt CParentIntfc::SetChild( CChildIntfc& aChild )
    {
    TInt status(KErrAlreadyExists);
    if ( iChildrenList.Find(&aChild) == KErrNotFound )
        {
        status = iChildrenList.Append(&aChild);
        }
    return status;
    }

// ---------------------------------------------------------------------------
// ChildDeleted
// ---------------------------------------------------------------------------
void CParentIntfc::ChildDeleted( CChildIntfc& aChild )
    {
    TInt index(iChildrenList.Find(&aChild));
    if ( index != KErrNotFound )
        {
        iChildrenList.Remove(index);
        }
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
CChildIntfc::~CChildIntfc()
    {
    if ( iParent )
        {
        iParent->ChildDeleted(*((CChildIntfc*)this));
        }
    }

// ---------------------------------------------------------------------------
// ParentDeleted
// ---------------------------------------------------------------------------
void CChildIntfc::ParentDeleted( CParentIntfc& aParent )
    {
    if ( iParent == &aParent )
        {
        iParent = NULL;
        }
    }

// ---------------------------------------------------------------------------
// GetParent
// ---------------------------------------------------------------------------
TInt CChildIntfc::GetParent( CParentIntfc*& aParent )
    {
    TInt status(KErrNotFound);
    if ( iParent )
        {
        aParent = iParent;
        status = KErrNone;
        }
    return status;
    }

// ---------------------------------------------------------------------------
// SetParent
// ---------------------------------------------------------------------------
TInt CChildIntfc::SetParent( CParentIntfc& aParent )
    {
    TInt status(KErrAlreadyExists);
    if ( !iParent )
        {
        iParent = &aParent;
        status = KErrNone;
        }
    return status;
    }

// End of file
