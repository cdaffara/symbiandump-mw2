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
* Description:  Header file for supl report message
*
*/


#include "epos_comasuplreport.h"
#include "epos_comasuplstartver2.h"
#include "epos_suplconstants.h"
#include "epos_comasuplresponse.h"
#include "lbs/epos_eomasuplposerrors.h"

const TInt KSessionListShift = 3;
const TInt KSetCapsShift = 2;
const TInt KRepDataShift = 1;

// -----------------------------------------------------------------------------
// COMASuplPositionData::COMASuplPositionData 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplPositionData::COMASuplPositionData()
    {   
    // No implementation required
    }
// -----------------------------------------------------------------------------
// COMASuplPositionData::~COMASuplPositionData
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPositionData::~COMASuplPositionData()
    {
    if(iPosition)
        {
        delete iPosition;
        iPosition = NULL;
        }
    }
// -----------------------------------------------------------------------------
// COMASuplPositionData::NewLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPositionData* COMASuplPositionData::NewLC()
    {
    COMASuplPositionData* self = new (ELeave)COMASuplPositionData();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplPositionData::NewL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPositionData* COMASuplPositionData::NewL()
    {
    COMASuplPositionData* self=COMASuplPositionData::NewLC();
    CleanupStack::Pop(self); // self;
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplPositionData::ConstructL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplPositionData::ConstructL()
    {

    }
// -----------------------------------------------------------------------------
// COMASuplPositionData::SetPosition()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplPositionData::SetPosition(COMASuplPosition* aPosition)
    {
    if(iPosition)
        {
        delete iPosition;
        iPosition = NULL;
        }
    iPosition = aPosition; 
    }

// -----------------------------------------------------------------------------
// COMASuplPositionData::SetPosMethod()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplPositionData::SetPosMethod(const TOMASuplPosMethod aPosMethod)
    {
    iPosMethod = aPosMethod;
    }

// -----------------------------------------------------------------------------
// COMASuplPositionData::GetPosition()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C void COMASuplPositionData::GetPosition(COMASuplPosition*& aPosition)const
    {
    aPosition = iPosition;
    }
// -----------------------------------------------------------------------------
// COMASuplPositionData::GetPosMethod()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplPositionData::GetPosMethod(TOMASuplPosMethod& aPosMethod)const
    {
    aPosMethod = iPosMethod;
    }


// -----------------------------------------------------------------------------
// COMASuplPositionData::CloneL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPositionData* COMASuplPositionData::CloneL()
    {
    COMASuplPositionData* positionDataToRet = COMASuplPositionData::NewL();
    positionDataToRet->SetPosMethod(iPosMethod);
    positionDataToRet->SetPosition(static_cast<COMASuplPosition*> (iPosition->CloneL()));

    return positionDataToRet;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::~COMASuplReportData()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplReportData::~COMASuplReportData()
    {
    if(iPositionData)
        {
        delete iPositionData;
        iPositionData = NULL;
        }
    if(iMultipleLocIds)
        {
        delete iMultipleLocIds;
        iMultipleLocIds = NULL;
        }
    }

// -----------------------------------------------------------------------------
// COMASuplReportData::NewLC
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplReportData* COMASuplReportData::NewLC()
    {
    COMASuplReportData* self = new (ELeave)COMASuplReportData();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplReportData* COMASuplReportData::NewL()
    {
    COMASuplReportData* self=COMASuplReportData::NewLC();
    CleanupStack::Pop(self); // self;
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::ConstructL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplReportData::ConstructL()
    {

    }

// -----------------------------------------------------------------------------
// COMASuplReportData::COMASuplReportData
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
COMASuplReportData::COMASuplReportData()
    {
    //no implementation
    }

// -----------------------------------------------------------------------------
// COMASuplReportData::SetPosData
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplReportData::SetPosData(COMASuplPositionData* aPositionData)
    {
    if(iPositionData)
        {
        delete iPositionData;
        iPositionData = NULL;
        }
    iPositionData = aPositionData;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::SetTimeStamp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplReportData::SetTimeStamp(const TOMASuplTimeStamp aTimeStamp)
    {
    iTimeStamp = aTimeStamp;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::SetResultCode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplReportData::SetResultCode(const TOMASuplReportDataResultCode aResultCode)
    {
    iResultCode = aResultCode;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::SetMutipleLocId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplReportData::SetMutipleLocId(COMASuplMultipleLocIds* aMultipleLocIds)
    {
    if(iMultipleLocIds)
        {
        delete iMultipleLocIds;
        iMultipleLocIds = NULL;
        }
    iMultipleLocIds = aMultipleLocIds;
    }

// -----------------------------------------------------------------------------
// COMASuplReportData::GetPosData
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplReportData::GetPosData(COMASuplPositionData*& aPositionData)const
    {
    aPositionData = iPositionData;
    }
    
// -----------------------------------------------------------------------------
// COMASuplReportData::GetTimeStamp
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplReportData::GetTimeStamp(TOMASuplTimeStamp& aTimeStamp)const
    {
    aTimeStamp = iTimeStamp;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::GetResultCode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplReportData::GetResultCode(COMASuplReportData::TOMASuplReportDataResultCode& aResultCode)const
    {
    aResultCode = iResultCode;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::GetMutipleLocId
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplReportData::GetMutipleLocId(COMASuplMultipleLocIds*& aMultipleLocIds)const
    {
    aMultipleLocIds = iMultipleLocIds;
    }
// -----------------------------------------------------------------------------
// COMASuplReportData::CloneL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplReportData* COMASuplReportData::CloneL()
    {
    COMASuplReportData* repDataToRet = COMASuplReportData::NewL();
    CleanupStack::PushL( repDataToRet );
    repDataToRet->SetMutipleLocId(iMultipleLocIds->CloneL());
    repDataToRet->SetResultCode(iResultCode);
    repDataToRet->SetTimeStamp(iTimeStamp);
    repDataToRet->SetPosData(iPositionData->CloneL());
    CleanupStack::Pop(repDataToRet);
    return repDataToRet;
    }
// -----------------------------------------------------------------------------
// COMASuplReport::COMASuplReport
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplReport::COMASuplReport()
    {
    // No implementation required
    }
// -----------------------------------------------------------------------------
// COMASuplReport::~COMASuplReport
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplReport::~COMASuplReport()
    {
       
    if(iSETCapabilities)
        {
        delete iSETCapabilities;
        iSETCapabilities = NULL;
        }
    iSessionList.ResetAndDestroy();
    iReportDataList.ResetAndDestroy();
    }
// -----------------------------------------------------------------------------
// COMASuplReport::NewLC()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplReport* COMASuplReport::NewLC()
    {
    COMASuplReport* self = new (ELeave)COMASuplReport();
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplReport::NewL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplReport* COMASuplReport::NewL()
    {
    COMASuplReport* self=COMASuplReport::NewLC();
    CleanupStack::Pop(self); // self;
    return self;
    }

// -----------------------------------------------------------------------------
// COMASuplReport::ConstructL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplReport::ConstructL()
    {
    iOptionalMask = 0;
       
    SetMessageType(COMASuplAsnMessageBase::ESUPL_REPORT);
    }

// -----------------------------------------------------------------------------
// COMASuplReport::Clone() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//              
EXPORT_C void COMASuplReport::Clone(const COMASuplAsnMessageBase* aReport,TInt& aErrorCode)
    {

    TRAP(aErrorCode, CloneBaseL(aReport,aErrorCode));
    COMASuplReport* reportToClone = (COMASuplReport*)aReport;
    
    RPointerArray<COMASuplSessionID> sessionList;
    /* SetCapabilities*/
    COMASuplSETCapabilitiesVer2* sETCapabilities = NULL; 
    RPointerArray<COMASuplReportData> reportDataList;
    //ver field/ of Supl_end
    TBuf8<KVer> ver;

    reportToClone->GetReportDataList(reportDataList);
    
    for(TInt reportListIter = 0; reportListIter < reportDataList.Count();reportListIter++)
        {
        TRAP(aErrorCode,(iReportDataList.Append(reportDataList[reportListIter]->CloneL())));
        }
        
    //set the optional mask to indicate report data list is set
    iOptionalMask |= 1<<KRepDataShift;
    
    reportToClone->GetSetCapabilities(sETCapabilities);
    if(sETCapabilities) 
    	{
		TRAP(aErrorCode,SetSetCapabilities(sETCapabilities->CloneL()));
    	}
    	
    reportToClone->GetVer(ver);
    SetVer(ver);
    
    reportToClone->GetSessionList(sessionList);
   
    for(TInt count = 0; count < sessionList.Count();count++)
        {
        TRAP(aErrorCode,iSessionList.Append(sessionList[count]->CloneL()));
        } 
        
    iOptionalMask |= KSessionListShift;
    
    sessionList.Close();
    reportDataList.Close();
    
    
    }

/*
 * Getter method for Session List
 */
EXPORT_C TInt COMASuplReport::GetSessionList(RPointerArray<COMASuplSessionID>& aSessionList)const
    {
    if(iOptionalMask & (1<<KSessionListShift))
        {
        for(TInt count = 0; count < iSessionList.Count();count++)
            {
            TInt error = aSessionList.Append(iSessionList[count]);
            if( error != KErrNone )
                {
                return error;
                }
            }
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }

/*
 * Getter method for Session List
 */
EXPORT_C TInt COMASuplReport::GetSetCapabilities(COMASuplSETCapabilitiesVer2*& aSETCapabilities)const
    {
    if(iOptionalMask & (1<<KSetCapsShift))
        {
        aSETCapabilities = iSETCapabilities;
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }

/*
 * Getter method for Session List
 */
EXPORT_C TInt COMASuplReport::GetReportDataList(RPointerArray<COMASuplReportData>& aReportDataList)const
    {
    if(iOptionalMask & (1<<KRepDataShift))
        {
        for(TInt count = 0; count < iReportDataList.Count();count++)
            {
            TInt error = aReportDataList.Append(iReportDataList[count]);
            if( error != KErrNone )
                {
                return error;
                }
            }
        return KErrNone;
        }
    else
        return KErrOMASuplParamNotSet;
    }
/*
 * getter method for version
 */
EXPORT_C TInt COMASuplReport::GetVer(TDes8& aVer)const
    {
    if(iOptionalMask & (1))
        {
        if(aVer.MaxLength() >= iVer.Length())
            {
            aVer = iVer;
            return KErrNone;
            }
        return KErrArgument;
        }
    else
        return KErrOMASuplParamNotSet;
    }
/*
 * Getter method for Session List
 */
EXPORT_C void COMASuplReport::SetSessionList(const RPointerArray<COMASuplSessionID>& aSessionList)
    {
    iSessionList.Reset();
    iOptionalMask |= 1<<KSessionListShift;
    for(TInt count = 0; count < aSessionList.Count();count++)
        {
        iSessionList.Append(aSessionList[count]);
        }  
    }
/*
 * Getter method for Set Capabilities
 */
EXPORT_C void COMASuplReport::SetSetCapabilities(COMASuplSETCapabilitiesVer2* aSETCapabilities)
    {
    if(aSETCapabilities)
        {
        iOptionalMask |= 1<<KSetCapsShift;
        iSETCapabilities = aSETCapabilities;
        }
    }
/*
 * Getter method for Report Data List
 */
EXPORT_C void COMASuplReport::SetReportDataList(const RPointerArray<COMASuplReportData>& aReportDataList)
    {
    iReportDataList.Reset();
    iOptionalMask |= 1<<KRepDataShift;
    for(TInt count = 0; count < aReportDataList.Count();count++)
        {
        iReportDataList.Append(aReportDataList[count]);
        }        
    }

/*
 * Getter method for Version
 */
EXPORT_C void COMASuplReport::SetVer(const TDesC8& aVer)
    {
    if(aVer.Length() <= iVer.MaxLength())
        {
        iOptionalMask |= 1;
        iVer = aVer;
        }
    }
