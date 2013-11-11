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
* Description: 
*   ?description_line
*
*/


//  INCLUDES
#include <e32math.h>
#include <e32svr.h>
#include <e32std.h>
#include "FT_CSearchResultBase.h"

//CONSTANTS

// C++ Constructor 
CSearchResultBase::CSearchResultBase() :
    iRedefined(EFalse)
    {
    
    }
   
// C++ destructor 
CSearchResultBase::~CSearchResultBase() 
    {  
    iSearchResult.Close();
    }


// ---------------------------------------------------------
// CSearchResultBase::ParseSearchResult()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSearchResultBase::ParseSearchResult(const TDesC& aDes)
    {
    TLex line(aDes);
    line.Mark();
    TInt startValue=0;
    TInt endValue=0;
    
    while (!(line.Peek()).Eos())
        {
        line.Inc();
    
        if (line.Peek() == ',' || line.Peek() == '-' || (line.Peek()).Eos())
            {
            TPtrC token = line.MarkedToken();
            TLex val(token);
            val.Val(startValue);
            iSearchResult.Append(startValue);
            if (line.Peek() == '-')
                {
                line.Inc();
                line.Mark();
                while (line.Peek() != ',' && !(line.Peek()).Eos())
                    {
                    line.Inc();               
                    }
                TPtrC endToken = line.MarkedToken();
                TLex endVal(endToken);
                endVal.Val(endValue);
                for (TInt i=startValue+1; i<=endValue; i++)
                    {
                    iSearchResult.Append(i);
                    }
                }
             if (line.Peek() == ',')
                {
                line.Inc();
                line.Mark();
                }
            }
        }
    }

// ---------------------------------------------------------
// CSearchResultBase::ParseAttribute()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CSearchResultBase::ParseAttributesL(const TDesC& aDes)
    {
    TLex line(aDes);
    line.Inc();
    line.Inc();
    User::LeaveIfError(line.Val(iAttributes, EHex));
    }

// ---------------------------------------------------------
// CSearchResultBase::Attributes
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TUint CSearchResultBase::Attributes() const
    {
    return iAttributes;  
    }

// ---------------------------------------------------------
// CSearchResultBase::SearchResult
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
RArray<TPosLmItemId>& CSearchResultBase::SearchResult() 
    {
    return iSearchResult;
    }

// ---------------------------------------------------------
// CSearchResultBase::Redefined
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CSearchResultBase::Redefined() const
    {
    return iRedefined;
    }

// ---------------------------------------------------------
// CSearchResultBase::FindSearchResult
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TBool CSearchResultBase::FindSearchResult(const TPosLmItemId aId)
    {
    for (TInt i=0; i<iSearchResult.Count(); i++)
        {
            if (iSearchResult[i] == aId)
                {
                return ETrue;
                }
        }
    return EFalse;
    }

//  End of File
