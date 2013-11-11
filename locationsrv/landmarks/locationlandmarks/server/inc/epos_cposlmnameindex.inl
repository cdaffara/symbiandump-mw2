/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Defininiton of inline methods of CPosLmNameIndex class
*
*
*/


//--------------------------------------------------------------------
// CLmNameIndex::CIndexItem
//--------------------------------------------------------------------
//

TPosLmItemId CPosLmNameIndex::CIndexItem::Id() const
    {
    return iId;
    }

TUint32* CPosLmNameIndex::CIndexItem::IdPtr()
    {
    return &iId;
    }
    
HBufC* CPosLmNameIndex::CIndexItem::NamePtr()
    {
    return iName;
    }

void CPosLmNameIndex::CIndexItem::SetId( TPosLmItemId aLmid )
    {
    iId = aLmid;
    }

TBool CPosLmNameIndex::CIndexItem::IsValid()
    {
    return iState == EStateValid;
    }

TBool CPosLmNameIndex::CIndexItem::IsTemp()
    {
    return iState == EStateTemp;
    }

void CPosLmNameIndex::CIndexItem::SetValid()
    {
    iState = EStateValid;
    }

void CPosLmNameIndex::CIndexItem::SetInvalid()
    {
    iState = EStateInvalid;
    }

//--------------------------------------------------------------------
// CLmNameIndex::CIndexItem
//--------------------------------------------------------------------
//
TTime CPosLmNameIndex::TimeStamp() const
    {
    return iTimeStamp;
    }

TInt CPosLmNameIndex::Status() const
    {
    return iStatus;
    }

