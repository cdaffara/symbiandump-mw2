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
* Description:  Header of Parent Child Interface.
*
*/


#ifndef PARENTCHILDINTFC_H_
#define PARENTCHILDINTFC_H_

#include <e32std.h>

class CChildIntfc;

class CParentIntfc
    {
public:
    virtual ~CParentIntfc();
    virtual TInt GetChildCount();
    virtual TInt GetChild( TInt aIndex, CChildIntfc*& aChild );
    virtual TInt SetChild( CChildIntfc& aChild );
    virtual void ChildDeleted( CChildIntfc& aChild );

protected:
    RPointerArray<CChildIntfc> iChildrenList;
    };

class CChildIntfc
    {
public:
    virtual ~CChildIntfc();
    virtual void ParentDeleted(CParentIntfc& aParent);
    virtual TInt GetParent(CParentIntfc*& aParent);
    virtual TInt SetParent(CParentIntfc& aParent);
private:
    CParentIntfc* iParent;
    };

#endif /*PARENTCHILDINTFC_H_*/

// End of file
