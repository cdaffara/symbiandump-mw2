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
#include "FT_CCategoryLandmarkSearchResult.h"

//CONSTANTS
const TInt KMaxFieldLength = 80;
const TInt KMaxLineLength=255;

//const TReal KReal32Accuracy = 5.96E-8;
/*
const TReal KAccuracyHigh = 5.96E-8;
const TReal KAccuracyMedium = 4E-5;
const TReal KAccuracyLow = 4E-3;
*/

// C++ Constructor 
CCategoryLandmarkSearchResult::CCategoryLandmarkSearchResult() 
    {
    }

CCategoryLandmarkSearchResult* CCategoryLandmarkSearchResult::NewL(const TDesC& aDes) 
    {
    CCategoryLandmarkSearchResult* self = new(ELeave) CCategoryLandmarkSearchResult;
    CleanupStack::PushL(self);
    self->ConstructL(aDes);
    CleanupStack::Pop();
    return self;
    }

// C++ destructor 
CCategoryLandmarkSearchResult::~CCategoryLandmarkSearchResult() 
    {
    delete iCategoryName;
    iCategoryName = NULL;
     
    iPositionFields.Close();
    }

void CCategoryLandmarkSearchResult::ConstructL(const TDesC& aDes) 
    {
    TLex line(aDes);
    line.Mark();
    TInt redefined=0;
    TInt id=0;

    if (line.Peek() != '#' && line.Peek() != '\t')
        { 
        TInt index=0;
        while (line.Peek() != '\n' && !(line.Peek()).Eos())
            {
            line.Inc();
            if (line.Peek() == '\t')
                {
                TPtrC token = line.MarkedToken();
                TLex val(token);

                switch(index)
                    {
                    case ECategoryName:
                        iCategoryName = token.AllocL();
                        break;
/*                    case EAttributes:
                        ParseAttributesL(token);
                        break;
*/
                    case ECategoryId:
                        //iTextCriteria = token.AllocL();
                        val.Val(id);
                        iCategoryId = id;
                        break;
/*
                    case EPositionFields:
                        ParsePositionFields(token);
                        break;
*/
                    case ELmSearchResult:
                        ParseSearchResult(token);
                        break;
                    case ERedefined:
                        val.Val(redefined);
                        if (redefined == 1)
                            {
                            iRedefined = ETrue;
                            }
                        break;
                    }    
                line.Inc();
                while (line.Peek() == '\t') // Empty value
                    {
                    line.Inc();
                    ++index;
                    }
                line.Mark();
                ++index;
                }
            }  
        }
    //Print();
    }   

// ---------------------------------------------------------
// CCategoryLandmarkSearchResult::ParsePositionFields()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
/*
void CCategoryLandmarkSearchResult::ParsePositionFields(const TDesC& aDes)
    {    
    TLex line(aDes);
    line.Mark();
    TInt value=0;
  
    while (!(line.Peek()).Eos())    
        {
        line.Inc();
        if (line.Peek() == ',' || (line.Peek()).Eos())
            {
            TPtrC token = line.MarkedToken();
            TLex val(token);
            val.Val(value);
            iPositionFields.Append(value);
            
            if (line.Peek() == ',')
                {
                line.Inc();
                line.Mark();
                }
            }
        }
    }
*/
// ---------------------------------------------------------
// CCategoryLandmarkSearchResult::Print()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCategoryLandmarkSearchResult::Print()
    {
    TBuf<KMaxLineLength> print;
    
    TChar del(',');
      
    print.Append(*iCategoryName);
    print.Append(del);

    TBuf<KMaxFieldLength> info;
    _LIT(KAttrInfo, "%x");
    info.Format(KAttrInfo, iAttributes);
    print.Append(info);
    print.Append(del);

    _LIT(KIntInfo, "%d");
    TInt i=0;
    for (i=0; i<iPositionFields.Count(); i++)
        {
        info.Format(KIntInfo, iPositionFields[i]);
        print.Append(info);
        print.Append(del);
        }
    
    for (i=0; i<iSearchResult.Count(); i++)
        {
        info.Format(KIntInfo, iSearchResult[i]);
        print.Append(info);
        print.Append(del);
        }

    RDebug::Print(print);
    }

// ---------------------------------------------------------
// CCategoryLandmarkSearchResult::CategoryName()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPtrC CCategoryLandmarkSearchResult::CategoryName() const
    {
    return *iCategoryName;
    }


// ---------------------------------------------------------
// CCategoryLandmarkSearchResult::CategoryId()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TPosLmItemId CCategoryLandmarkSearchResult::CategoryId() const
    {
    return iCategoryId;
    }

// ---------------------------------------------------------
// CCategoryLandmarkSearchResult::PositionFields
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
const RArray<TUint>& CCategoryLandmarkSearchResult::PositionFields() const
    {
    return iPositionFields;
    }

//  End of File
