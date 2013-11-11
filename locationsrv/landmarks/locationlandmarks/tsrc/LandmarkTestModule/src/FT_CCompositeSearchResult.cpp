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

#include <EPos_CPosLmCompositeCriteria.h>
#include <EPos_CPosLmCategoryCriteria.h>
#include <EPos_CPosLmCatNameCriteria.h>
#include <EPos_CPosLmTextCriteria.h>
#include <EPos_CPosLmIdListCriteria.h>
#include <EPos_CPosLmAreaCriteria.h>
#include <EPos_CPosLmNearestCriteria.h>
#include <LbsPosition.h>

#include "FT_CCompositeSearchResult.h"

#include <StifLogger.h>

//CONSTANTS
const TInt KMaxLineLength=255;

// C++ Constructor 
CCompositeSearchResult::CCompositeSearchResult() 
    {
    }

CCompositeSearchResult* CCompositeSearchResult::NewL(CStifLogger* aLog) 
    {
    CCompositeSearchResult* self = new(ELeave) CCompositeSearchResult;
    CleanupStack::PushL(self);
    self->ConstructL(aLog);
    CleanupStack::Pop();
    return self;
    }

// C++ destructor 
CCompositeSearchResult::~CCompositeSearchResult() 
    {
    iFile.Close();
    iFileSession.Close();

    iResultIds.Close();
    iIdList.Close();
    iPositionList.Close();

    delete iTextCriteria;
    iTextCriteria = NULL;

    delete iCategoryName;
    iCategoryName = NULL;

    }

void CCompositeSearchResult::ConstructL(CStifLogger* aLog) 
    {
    iLog = aLog;

    _LIT(KFileTestValues, "c:\\system\\test\\testdata\\LmDbCompositeSearchResult.txt");
    
    User::LeaveIfError(iFileSession.Connect());
    User::LeaveIfError(iFile.Open(iFileSession, KFileTestValues,  EFileRead));
    iTextEnabled = EFalse;
    iAreaEnabled = EFalse;
    iNearestEnabled = EFalse;
    iCategoryEnabled = EFalse;
    iIdListEnabled = EFalse;
    }

// ---------------------------------------------------------
// CCompositeSearchResult::GetCompositeSearchResultsL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//

TBool CCompositeSearchResult::GetCompositeSearchResultsL(CPosLmCompositeCriteria& aComposite)
    {
    TBuf<KMaxLineLength> line;
    TBool done = EFalse;

    iTextEnabled = EFalse;
    iAreaEnabled = EFalse;
    iNearestEnabled = EFalse;
    iCategoryEnabled = EFalse;
    iIdListEnabled = EFalse;

    iResultIds.Close();
    iIdList.Close();
    iPositionList.Close();

    delete iTextCriteria;
    iTextCriteria = NULL;

    delete iCategoryName;
    iCategoryName = NULL;

    iCategoryId = 0;
    iRedefined = 0;
    iUseCoverageRadius = EFalse;
    iMaxDistance = -1;

    iSouthLat=0;iNorthLat=0;iWestLong=0;iEastLong=0;
    iTestCounter++;

    TBuf<50> buf;
    buf.Format(_L(">>>>>>>>>>>>Test Round %d<<<<<<<<<<"), iTestCounter);
    iLog->Log(buf);

    aComposite.ClearArguments();

    while (ReadLn(iFile, line) != KErrEof && !done)
        {
        TLex lex(line);
        if (lex.Peek() != '#' && lex.Peek() != '\t')
            {
            // Found "# TEXT" line
            if (lex.Peek() == 'T')
                {
                ParseTextL(line);
                CPosLmTextCriteria* textCriteria = CPosLmTextCriteria::NewLC();
                
                textCriteria->SetTextL(*iTextCriteria);
                textCriteria->SetAttributesToSearch(iTextAttributes);
                textCriteria->SetPositionFieldsToSearchL(iPositionList); 
                
                aComposite.AddArgument(textCriteria);
                CleanupStack::Pop(textCriteria);
                iTextEnabled = ETrue;
                }
            // Found "# AREA" line
            else if (lex.Peek() == 'A')
                {
                ParseAreaL(line);
                CPosLmAreaCriteria* areaCriteria = CPosLmAreaCriteria::NewLC(iSouthLat, 
                    iNorthLat, 
                    iWestLong, 
                    iEastLong
                    );
                
                aComposite.AddArgument(areaCriteria);
                CleanupStack::Pop(areaCriteria);
                iAreaEnabled = ETrue;
                }
            // Found "# CATEGORY" line
            else if (lex.Peek() == 'C')
                {
                ParseCategoryL(line);
                CPosLmCategoryCriteria* catSearchCriteria = CPosLmCategoryCriteria::NewLC();
                if (iCategoryId>0)
                    {
                    iLog->Log(_L("SetCategoryItemId"));
                    catSearchCriteria->SetCategoryItemId(iCategoryId);
                    }
                if (iCategoryName != NULL)
                    {
                    iLog->Log(_L("SetCategoryNameL"));
                    catSearchCriteria->SetCategoryNameL(*iCategoryName);
                    }
                aComposite.AddArgument(catSearchCriteria);
                CleanupStack::Pop(catSearchCriteria);
                iCategoryEnabled = ETrue;
                }
            // Found "# NEAREST" line
            else if (lex.Peek() == 'N')
                {
                ParseNearestL(line);
                TCoordinate coord(iLatitude, iLongitude);
                
                CPosLmNearestCriteria* nearestCriteria = CPosLmNearestCriteria::NewLC(coord, iUseCoverageRadius);
                if (iMaxDistance > 0)
                    {
                    nearestCriteria->SetMaxDistance(iMaxDistance);
                    }
                nearestCriteria->SetUseCoverageRadius(iUseCoverageRadius);
                aComposite.AddArgument(nearestCriteria);
                CleanupStack::Pop(nearestCriteria);

                iNearestEnabled = ETrue;
                }
            // Found "# IDLIST" line
            else if (lex.Peek() == 'I')
                {
                ParseIdListL(line);
                RArray<TPosLmItemId> list;
                CleanupClosePushL(list);
                
                CPosLmIdListCriteria* idList = CPosLmIdListCriteria::NewLC();
                
                for (TInt i=0;i<iIdList.Count();i++)
                {
                    list.Append(iIdList[i]);
                }
                idList->SetLandmarkIdsL(list);
                
                aComposite.AddArgument(idList);
                CleanupStack::Pop(idList);
                CleanupStack::PopAndDestroy(&list);
                iIdListEnabled = ETrue;
                }
            // Found "# RESULT" line
            else if (lex.Peek() == 'R')
                {
                ParseResultL(line);
                }
            // Found "# SEPARATOR" line
            else if (lex.Peek() == 'S')
                {
                done = ETrue;
                }
            }
        }

    if (done != EFalse) return EFalse;
    else return ETrue;

    }

// ---------------------------------------------------------
// CCompositeSearchResult::ReadLn
// 
// (other items were commented in a header).
// ---------------------------------------------------------
//
TInt CCompositeSearchResult::ReadLn(RFile& aFile, TDes& aDes)
    {
    aDes = KNullDesC;
    TBuf8<KMaxLineLength> line;

	TInt err = aFile.Read(line);
    if (line.Length() == 0)
        {
        return KErrEof;
        }
	if (err)
		{
		return err;
		}

    // Find line feed
    TInt i = 0;
	TLex8 fileData;
	fileData.Assign(line);
	while (fileData.Peek() != '\n' && !(fileData.Peek()).Eos())
		{
        fileData.Inc();
        if (++i == KMaxLineLength)
            {
            // In case no carriage return or end of string characters are present.
            break;
            }
        }

    // Update file position
    TInt diff = -(line.Length() - i - 1);
    aFile.Seek(ESeekCurrent, diff);

    // Convert to 16-bit
    line.SetLength(i);
    aDes.Copy(line);

    return KErrNone;
    }

// ---------------------------------------------------------
// CCompositeSearchResult::ParseAreaL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCompositeSearchResult::ParseAreaL(const TDesC& aDes) 
    {
    iLog->Log(_L("ParseAreaL")); 
    TLex line(aDes);
    line.Mark();

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
// CCompositeSearchResult::ParseNearestL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCompositeSearchResult::ParseNearestL(const TDesC& aDes) 
    {
    iLog->Log(_L("ParseNearestL"));
    TLex line(aDes);
    line.Mark();
    TInt useCoverageRadius = 0;

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
                    case ELatitude:
                        val.Val(iLatitude, TChar(','));   
                        break;
                    case ELongitude:
                        val.Val(iLongitude, TChar(','));   
                        break;
                    case EUseCoverageRadius:  
                        val.Val(useCoverageRadius);
                        if (useCoverageRadius == 1)
                            {
                            iUseCoverageRadius = ETrue;
                            }
                        break;
                    case EMaxDistance:
                        val.Val(iMaxDistance);   
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
// CCompositeSearchResult::ParseTextL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCompositeSearchResult::ParseTextL(const TDesC& aDes) 
    {
    iLog->Log(_L("ParseTextL"));
    TLex line(aDes);
    line.Mark();

    if (line.Peek() != '#' && line.Peek() != '\t')
        { 
        TInt index=0;
        while (line.Peek() != '\n' && !(line.Peek()).Eos())
            {
            line.Inc();
            if (line.Peek() == '\t')
                {
                TPtrC token = line.MarkedToken();
                
                switch(index)
                    {
                    case ETextCriteria:
                        iTextCriteria = token.AllocL();
                        break;
                    case EAttributes:
                        ParseAttributesL(token);
                        iTextAttributes = iAttributes;
                        break;
                    case EPositionFields:
                        ExtractIdFields(token, iPositionList);
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
// CCompositeSearchResult::ParseCategoryL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCompositeSearchResult::ParseCategoryL(const TDesC& aDes) 
    {
    iLog->Log(_L("ParseCategoryL"));
    TLex line(aDes);
    line.Mark();
    TInt tmp;

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
                    case ECategoryId:
                        val.Val(tmp);
                        iCategoryId = tmp;
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
// CCompositeSearchResult::ParseIdListL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCompositeSearchResult::ParseIdListL(const TDesC& aDes)
    {
    iLog->Log(_L("ParseIdListL"));
    TLex line(aDes);
    line.Mark();
    
    RArray<TUint> list;
    CleanupClosePushL(list);
    TInt i=0;

    if (line.Peek() != '#' && line.Peek() != '\t')
        { 
        TInt index=0;
        while (line.Peek() != '\n' && !(line.Peek()).Eos())
            {
            line.Inc();
            if (line.Peek() == '\t')
                {
                TPtrC token = line.MarkedToken();
                //TLex val(token);              

                switch(index)
                    {
                    case ELmIdList:
                        ExtractIdFields(token, list);
                        for (i=0;i<list.Count();i++)
                            {
                            iIdList.Append(list[i]);
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
    CleanupStack::PopAndDestroy(&list);
    }  

// ---------------------------------------------------------
// CCompositeSearchResult::ParseResultL()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCompositeSearchResult::ParseResultL(const TDesC& aDes)
    {
    iLog->Log(_L("ParseResultL"));
    TLex line(aDes);
    line.Mark();
    TInt redefined=0;
    
    RArray<TUint> list;
    CleanupClosePushL(list);
    TInt i=0;

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
                    case ELmResultIdList:
                        ExtractIdFields(token, list);
                        for (i=0;i<list.Count();i++)
                            {
                            iResultIds.Append(list[i]);
                            }
                            break;
                    case ERedefined:
                        val.Val(redefined);
                        if (redefined == 1)
                            {
                            iLog->Log(_L("REDEFINED"));
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
    CleanupStack::PopAndDestroy(&list);
    }  

// ---------------------------------------------------------
// CCompositeSearchResult::ExtractIdFields()
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
void CCompositeSearchResult::ExtractIdFields(const TDesC& aDes, RArray<TUint>& aList)
    {
    iLog->Log(_L("ExtractIdFields"));
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
            aList.Append(startValue);

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
                    aList.Append(i);
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

//  End of File
