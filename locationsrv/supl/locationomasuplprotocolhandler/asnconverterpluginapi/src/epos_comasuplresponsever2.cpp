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
* Description:  Header file for supl 2.0 response  message
*
*/

// INCLUDE FILES

#include <e32def.h>

#include "epos_comasuplresponsever2.h"
#include "epos_comasuplsessionid.h"
#include "lbs/epos_eomasuplposerrors.h"



// ============================ MEMBER FUNCTIONS of COMASuplSessionID===============================
   
EXPORT_C TOMASuplSPCSETKey::TOMASuplSPCSETKey()
    {
    iIsKeySet = EFalse;
    }
EXPORT_C TInt TOMASuplSPCSETKey::GetSuplSPCSetKey(TDes8& keyStr)const
    {
    if(iIsKeySet)
        {
        if(keyStr.MaxLength() >= st.Length())
            {
            keyStr.Copy(st);
            return KErrNone;
            }
        return KErrArgument;
        }
    return KErrOMASuplParamNotSet;
    }

EXPORT_C void TOMASuplSPCSETKey::SetSuplSPCSetKey(TDes8& keyStr)
    {
    iIsKeySet = ETrue;
    if(keyStr.Length() <= st.MaxLength())
        st = keyStr;
    }



EXPORT_C TBool TOMASuplSuppWCDMAInfo::GetMRL()
    {
    return iMRL;
    }
EXPORT_C void TOMASuplSuppWCDMAInfo::SetMRL(TBool aMrl)
    {
    iMRL = aMrl;
    }
EXPORT_C TOMASuplSuppNetworkInformation::TOMASuplSuppNetworkInformation()
    {
    iOptionalMask = 0;
    
    iGSM = EFalse;
    iWCDMA = EFalse;
    iCDMA = EFalse;
    iHistoric = EFalse;
    iNonServing = EFalse;
    iUTRANGPSReferenceTime = EFalse;
    iUTRANGANSSReferenceTime = EFalse;

    }
EXPORT_C void TOMASuplSuppNetworkInformation::SetGSM(TBool aGSM)
    {
    iGSM = aGSM;
    }
EXPORT_C void TOMASuplSuppNetworkInformation::SetWCDMA(TBool aWCDMA)
    {
    iWCDMA = aWCDMA;
    }
EXPORT_C void TOMASuplSuppNetworkInformation::SetCDMA(TBool aCDMA)
    {
    iCDMA = aCDMA;
    }
EXPORT_C void TOMASuplSuppNetworkInformation::SetHistoric(TBool aHistoric)
    {
    iHistoric = aHistoric;
    }
EXPORT_C void TOMASuplSuppNetworkInformation::SetNonServing(TBool aNonServing)
    {
    iNonServing = aNonServing;
    }
EXPORT_C void TOMASuplSuppNetworkInformation::SetUTRANGPSRefTime(TBool aRefTime)
    {
    iUTRANGPSReferenceTime = aRefTime;
    }
EXPORT_C void TOMASuplSuppNetworkInformation::SetUTRANGANSSRefTime(TBool aRefTime)
    {
    iUTRANGANSSReferenceTime = aRefTime;
    }
EXPORT_C void TOMASuplSuppNetworkInformation::SetSupWCDMAInfo(TOMASuplSuppWCDMAInfo aSupportedWCDMAInfo)
    {
    iOptionalMask |= (1);
    iSupportedWCDMAInfo = aSupportedWCDMAInfo;
    }

EXPORT_C TBool TOMASuplSuppNetworkInformation::GetGSM()
    {
    return iGSM;
    }
EXPORT_C TBool TOMASuplSuppNetworkInformation::GetWCDMA()
    {
    return iWCDMA;
    }
EXPORT_C TBool TOMASuplSuppNetworkInformation::GetCDMA()
    {
    return iCDMA;
    }
EXPORT_C TBool TOMASuplSuppNetworkInformation::GetHistoric()
    {
    return iHistoric;
    }
EXPORT_C TBool TOMASuplSuppNetworkInformation::GetNonServing()
    {
    return iNonServing;
    }
EXPORT_C TBool TOMASuplSuppNetworkInformation::GetUTRANGPSRefTime()
    {
    return iUTRANGPSReferenceTime;
    }
EXPORT_C TBool TOMASuplSuppNetworkInformation::GetUTRANGANSSRefTime()
    {
    return iUTRANGANSSReferenceTime;
    }
EXPORT_C TInt TOMASuplSuppNetworkInformation::GetSupWCDMAInfo(TOMASuplSuppWCDMAInfo& aSupportedWCDMAInfo)
    {
    if(iOptionalMask & (1))
          {
          iSupportedWCDMAInfo = aSupportedWCDMAInfo;
          return KErrNone;
          }
      return KErrOMASuplParamNotSet;

    }
    

// -----------------------------------------------------------------------------
// COMASuplSPCTID::NewL()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplSPCTID* COMASuplSPCTID::NewL()
    {
    COMASuplSPCTID* self = new( ELeave ) COMASuplSPCTID;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplSPCTID::~COMASuplSPCTID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplSPCTID::~COMASuplSPCTID()
    {
    if(iFqdn)        
        {
        delete iFqdn;
        iFqdn = NULL;
        }
    }
// -----------------------------------------------------------------------------
// COMASuplSPCTID::GetRAND() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplSPCTID::GetRAND(TDes8& aRAND)const
    {
    if(iOptionalMask & (1))
        {
        aRAND.Append(iRAND);
        return KErrNone;
        }
    return KErrOMASuplParamNotSet;
    }
// -----------------------------------------------------------------------------
// COMASuplSPCTID::GetFqdn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt COMASuplSPCTID::GetFqdn(HBufC8*& aFqdn)const
    {
    if(iOptionalMask & (1<<1))
           {
           aFqdn = iFqdn;
           return KErrNone;
           }
       return KErrOMASuplParamNotSet;

    }
// -----------------------------------------------------------------------------
// COMASuplSPCTID::SetRAND() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSPCTID::SetRAND(const TDesC8& aRAND)
    {
    if(aRAND.Length() <= iRAND.MaxLength())
        {
        iOptionalMask |= (1);
        iRAND.Copy(aRAND);
        }
    }
// -----------------------------------------------------------------------------
// COMASuplSPCTID::SetFqdn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplSPCTID::SetFqdn(HBufC8* aFqdn)
    {
    if(iFqdn)
        {
        delete iFqdn;
        iFqdn = NULL;
        }
    iOptionalMask |= (1<<1);
    iFqdn = aFqdn;
    }
// -----------------------------------------------------------------------------
// COMASuplSPCTID::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplSPCTID* COMASuplSPCTID::CloneL()
    {
    COMASuplSPCTID* spctIDToRet = COMASuplSPCTID::NewL();
    CleanupStack::PushL(spctIDToRet);
    HBufC8* fqdn = HBufC8::NewL(iFqdn->Length());  
    if(iOptionalMask & (1<<1))
        {
        *fqdn = *iFqdn;
        spctIDToRet->SetFqdn(fqdn);
        }

    
    TBuf8<KRANDStrLen>  rAND;
    if(iOptionalMask & (1))
        {
        rAND.Append(iRAND);
        spctIDToRet->SetRAND(rAND);
        }
    CleanupStack::Pop(spctIDToRet);
    return spctIDToRet;
    }
// -----------------------------------------------------------------------------
// COMASuplSPCTID::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplSPCTID::ConstructL()
    {
    
    }    
// -----------------------------------------------------------------------------
// COMASuplSPCTID::COMASuplSPCTID() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
COMASuplSPCTID::COMASuplSPCTID()
    {
    iOptionalMask = 0;
    }
     
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C COMASuplResponseV2Extn* COMASuplResponseV2Extn::NewL()
    {
    COMASuplResponseV2Extn* self = new (ELeave) COMASuplResponseV2Extn;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::~COMASuplResponseV2Extn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplResponseV2Extn::~COMASuplResponseV2Extn()
    {
    if(iPosition)
        {
        delete iPosition;
        iPosition = NULL;
        }
    if(iSPCTID)
        {
        delete iSPCTID;
        iSPCTID = NULL;
        }
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::ConstructL()
    {
    
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::COMASuplResponseV2Extn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C COMASuplResponseV2Extn::COMASuplResponseV2Extn()
    {
    iSPCSETKeylifetime = 0;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::GetSupNetworkInfo() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::GetSupNetworkInfo(TOMASuplSuppNetworkInformation& aSupNwInfo)
    {
    aSupNwInfo = iSupportedNetworkInformation;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::GetSPCSETKey() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::GetSPCSETKey(TOMASuplSPCSETKey& aSupSPCSETKey)
    {
    aSupSPCSETKey = iSPCSETKey;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::GetSPCTId() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::GetSPCTId(COMASuplSPCTID*& aSupSPCTId)
    {
    aSupSPCTId = iSPCTID;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::GetSPCLifetime() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::GetSPCLifetime(TInt8& aSPCSETLifeTime)
    {
    aSPCSETLifeTime = iSPCSETKeylifetime;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::GetSuplPos() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::GetSuplPos(COMASuplPosition*& aSupPosition)
    {
    aSupPosition = iPosition;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::SetSupNetworkInfo() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::SetSupNetworkInfo(TOMASuplSuppNetworkInformation aSupNwInfo)
    {
    iSupportedNetworkInformation = aSupNwInfo;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::SetSPCSETKey() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::SetSPCSETKey(TOMASuplSPCSETKey aSupSPCSETKey)
    {
    iSPCSETKey = aSupSPCSETKey;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::SetSPCTId() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::SetSPCTId(COMASuplSPCTID* aSupSPCTId)
    {
    if(iSPCTID)
        {        
        delete iSPCTID;
        iSPCTID = NULL;
        }
    iSPCTID = aSupSPCTId;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::SetSPCLifetime() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::SetSPCLifetime(TInt8 aSPCSETLifeTime)
    {
    iSPCSETKeylifetime = aSPCSETLifeTime;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::SetSuplPos() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplResponseV2Extn::SetSuplPos(COMASuplPosition* aSupPosition)
    {
    if(iPosition)
        {
        delete iPosition;
        iPosition = NULL;
        }
    iPosition = aSupPosition;
    }

// -----------------------------------------------------------------------------
// COMASuplResponseV2Extn::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplResponseV2Extn* COMASuplResponseV2Extn::CloneL()
    {
    COMASuplResponseV2Extn* ver2ExtnToRet = COMASuplResponseV2Extn::NewL();
    ver2ExtnToRet->SetSPCLifetime(iSPCSETKeylifetime);
    ver2ExtnToRet->SetSPCSETKey(iSPCSETKey);
    ver2ExtnToRet->SetSupNetworkInfo(iSupportedNetworkInformation);
    if(iPosition)
        ver2ExtnToRet->SetSuplPos(static_cast<COMASuplPosition*>(iPosition->CloneL()));
    if(iSPCTID)
        ver2ExtnToRet->SetSPCTId(iSPCTID->CloneL());
    return ver2ExtnToRet;
    }


// -----------------------------------------------------------------------------
// COMASuplResponseVer2::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplResponseVer2* COMASuplResponseVer2::NewL()
    {
    COMASuplResponseVer2* self = new( ELeave ) COMASuplResponseVer2;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplResponseVer2::~COMASuplResponseVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplResponseVer2::~COMASuplResponseVer2()
    {
    if(iVer2Extn)
        {
        delete iVer2Extn;
        iVer2Extn = NULL;
        }
    }
 

   
// -----------------------------------------------------------------------------
// COMASuplResponseVer2::COMASuplResponseVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplResponseVer2::COMASuplResponseVer2()
    {
        
    }
// -----------------------------------------------------------------------------
// COMASuplResponseVer2::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void COMASuplResponseVer2::ConstructL()
    {
    COMASuplResponse::ConstructL();
    }
// -----------------------------------------------------------------------------
// COMASuplResponseVer2::SetResponseV2Extn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C void COMASuplResponseVer2::SetResponseV2Extn(COMASuplResponseV2Extn* aVer2Extn)
    {
    if (iVer2Extn)
        {
        delete iVer2Extn;
        iVer2Extn = NULL;
        }
    iVer2Extn = aVer2Extn;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseVer2::GetResponseV2Extn() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//  
EXPORT_C TInt COMASuplResponseVer2::GetResponseV2Extn(COMASuplResponseV2Extn*& aVer2Extn)
    {
    aVer2Extn = iVer2Extn;
    if(iVer2Extn)
        return KErrNone;
    return KErrOMASuplParamNotSet;
    }
// -----------------------------------------------------------------------------
// COMASuplResponseVer2::Clone() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//              
EXPORT_C void COMASuplResponseVer2::Clone(const COMASuplAsnMessageBase* aResponse,TInt& aErrorCode)
    {
    TRAP(aErrorCode, CloneBaseL(aResponse,aErrorCode));
    if(aErrorCode)
        return;
    COMASuplResponse::Clone(aResponse,aErrorCode);
    COMASuplResponseVer2* response = (COMASuplResponseVer2*)aResponse;
    if(response->iVer2Extn)
        TRAP(aErrorCode,iVer2Extn = response->iVer2Extn->CloneL());
    }
