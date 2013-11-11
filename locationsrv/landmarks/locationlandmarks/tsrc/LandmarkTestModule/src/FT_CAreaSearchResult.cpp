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
#include "FT_CAreaSearchResult.h"

//CONSTANTS
const TInt KMaxFieldLength = 80;

// C++ Constructor 
CAreaSearchResult::CAreaSearchResult() 
    {
    }

CAreaSearchResult* CAreaSearchResult::NewL(const TDesC& aDes) 
    {
    CAreaSearchResult* self = new(ELeave) CAreaSearchResult;
    CleanupStack::PushL(self);
    self->ConstructL(aDes);
    CleanupStack::Pop();
    return self;
    }

// C++ destructor 
CAreaSearchResult::~CAreaSearchResult() 
    {

    }

void CAreaSearchResult::ConstructL(const TDesC& aDes) 
    {
    TLex line(aDes);
    line.Mark();
    TInt redefined=0;

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
                    case ESouthLat:
                        val.Val(iSouthLat, TChar(','));   
                        break;
                    case ENorthLat:
                        val.Val(iNorthLat, TChar(','));   
                        break;
                    case EWestLong:
                        val.Val(iWestLong, TChar(','));   
                        break;
                    case EEastLong:
                        val.Val(iEastLong, TChar(','));   
                        break;
                    case EErrorCode:
                        val.Val(iErrorCode);   
                        break;
                    case ESearchResult:
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
    
    }

// ---------------------------------------------------------
// CAreaSearchResult::SouthLat
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CAreaSearchResult::SouthLat() const
    {
    return iSouthLat;
    }

// ---------------------------------------------------------
// CAreaSearchResult::NortLat()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TReal64 CAreaSearchResult::NorthLat() const
    {
    return iNorthLat;
    }

// ---------------------------------------------------------
// CAreaSearchResult::WestLong()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//        
TReal64 CAreaSearchResult::WestLong() const
    {
    return iWestLong;
    }

// ---------------------------------------------------------
// CAreaSearchResult::EastLong()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//        
TReal64 CAreaSearchResult::EastLong() const
    {
    return iEastLong;
    }

// ---------------------------------------------------------
// CAreaSearchResult::ErrorCode()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CAreaSearchResult::ErrorCode() const
    {
    return iErrorCode;
    }

// ---------------------------------------------------------
// CAreaSearchResult::Print()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CAreaSearchResult::Print(TDes& aPrint)
    {
    
    
    TChar del(',');

    TBuf<KMaxFieldLength> info;
    _LIT(KPosInfo, "SouthLat=%g, NortLat=%g, WestLong=%g, EastLong=%g");
    
    info.Format(KPosInfo, iSouthLat, iNorthLat, iWestLong, iEastLong);
    aPrint.Append(info);
    aPrint.Append(del);
    
    _LIT(KRedefined, "Redefined=%d");
    info.Format(KRedefined, iRedefined);
    aPrint.Append(info);
    aPrint.Append(del);

    _LIT(KErrorCode, "ErrorCode = %d");
    info.Format(KErrorCode, iErrorCode);
    aPrint.Append(info);
    aPrint.Append(del);

    aPrint.Append(_L("Search result = "));

    for (TInt i=0; i<iSearchResult.Count(); i++)
        {
        _LIT(KSearchResult, "%d");
        info.Format(KSearchResult, iSearchResult[i]);
        aPrint.Append(info);
        aPrint.Append(del);
        }
    
    
    }


//  End of File
