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
* Description:  Utility class for impementing Parent/Child class
*
*/

#include "ParentChildIntfc.h"

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

TInt CParentIntfc::GetChildCount()
    {
    return iChildrenList.Count();
    }

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

TInt CParentIntfc::SetChild( CChildIntfc& aChild )
    {
    TInt status(KErrAlreadyExists);
    if ( iChildrenList.Find(&aChild) == KErrNotFound )
        {
        status = iChildrenList.Append(&aChild);
        }
    return status;
    }

void CParentIntfc::ChildDeleted( CChildIntfc& aChild )
    {
    TInt index(iChildrenList.Find(&aChild));
    if ( index != KErrNotFound )
        {
        iChildrenList.Remove(index);
        }
    }

CChildIntfc::~CChildIntfc()
    {
    if ( iParent )
        {
        iParent->ChildDeleted(*((CChildIntfc*)this));
        }
    }

void CChildIntfc::ParentDeleted( CParentIntfc& aParent )
    {
    if ( iParent == &aParent )
        {
        iParent = NULL;
        }
    }

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
