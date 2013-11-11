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
* Description:  Header file for supl Pos init message
*
*/

// INCLUDE FILES

#include <e32def.h>

#include "epos_comasuplposinit.h"
#include "lbs/epos_comasuplreqasstdata.h"
#include "lbs/epos_comasuplposition.h"
#include "lbs/epos_comasuplvelocity.h"
#include "lbs/epos_comasuplsetcapabilities.h"
#include "lbs/epos_comasuplpospayload.h"
#include "epos_comasuplpos.h"
#include "epos_comasuplsessionid.h"
#include "epos_comasupllocationid.h"
#include "lbs/epos_eomasuplposerrors.h"

// ============================ MEMBER FUNCTIONS of COMASuplPosInit===============================
// -----------------------------------------------------------------------------
// COMASuplPosInit::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPosInit* COMASuplPosInit::NewL()
    {
    COMASuplPosInit* self = new( ELeave ) COMASuplPosInit;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplPosInit::~COMASuplPosInit() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplPosInit::~COMASuplPosInit()
    {
	if(iReqAsstData)
		{
		delete iReqAsstData;
		iReqAsstData = NULL;
		}
	if(iPosPayload)
		{
		delete iPosPayload;
		iPosPayload = NULL;
		}
	if(iVelocity)
		{
		delete iVelocity;
		iVelocity = NULL;
		}
	if(iPosition)
		{
		delete iPosition;
		iPosition = NULL;
		}
	}

// -----------------------------------------------------------------------------
// COMASuplPosInit::SetSuplPosInit() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPosInit::SetSuplPosInit(COMASuplSETCapabilities* aSuplSETCaps,
						  			         COMASuplLocationId* aSuplLocationId,
						  			         TBool aeCID)
{	
	delete iLocationId;
	iLocationId = NULL;
	iSETCapabilities = aSuplSETCaps;
	iLocationId = aSuplLocationId;
	ieCID  = aeCID;
}	

// -----------------------------------------------------------------------------
// COMASuplPosInit::RequestAssistanceData() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplPosInit::RequestAssistanceData(COMASuplReqAsstData*& aReqAsstData)const
{
	if(iOptionalMask & (1<<(0)))
		{
		aReqAsstData = iReqAsstData;		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplPosInit::SetRequestAssistanceData() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPosInit::SetRequestAssistanceData(COMASuplReqAsstData* aReqAsstData)
{	
	delete iReqAsstData;
	iReqAsstData = NULL;
	
	iReqAsstData = aReqAsstData;
	if(iReqAsstData)
		iOptionalMask |= LEFTSHIFT(0);
}	


// -----------------------------------------------------------------------------
// COMASuplPosInit::Position()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplPosInit::Position(COMASuplPosition*& aPosition)const
{
	if(iOptionalMask & (1<<(1)))
		{
		aPosition = iPosition;		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplPosInit::SetPosition() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPosInit::SetPosition(COMASuplPosition* aPosition)
{	
	delete iPosition;
	iPosition = NULL;
	iPosition = aPosition;
	if(iPosition)
		iOptionalMask |= LEFTSHIFT(1);
}	


// -----------------------------------------------------------------------------
// COMASuplPosInit::Ver()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplPosInit::Ver(TDes8& aVer)const
{
	if(iOptionalMask & (1<<(2)))
		{
		aVer.Copy(iVer);		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplPosInit::SetVer() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPosInit::SetVer(TDesC8& aVer)
{
	iVer.Copy(aVer);
	iOptionalMask |= LEFTSHIFT(2);
}  

// -----------------------------------------------------------------------------
// COMASuplPosInit::SuplPosPayload()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   
	
EXPORT_C TInt COMASuplPosInit::SuplPosPayload(COMASuplPosPayload*& aPosPayload)const
{
	if(iOptionalMask & (1<<(3)))
		{
		aPosPayload = iPosPayload;		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplPosInit::SetSuplPosPayload() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   	
EXPORT_C void COMASuplPosInit::SetSuplPosPayload(COMASuplPosPayload* aPosPayload)
{	
	delete iPosPayload;
	iPosPayload = NULL;
	iPosPayload = aPosPayload;
	if(iPosPayload)
		{
		iOptionalMask |= LEFTSHIFT(3);	
		}		
}	  

// -----------------------------------------------------------------------------
// COMASuplPosInit::SuplPosPayload()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   
	
EXPORT_C TInt COMASuplPosInit::Velocity(COMASuplVelocity*& aVelocity)const
{
	if(iOptionalMask & (1<<(4)))
		{
		aVelocity = iVelocity;		
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplPosInit::SetSuplPosPayload() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   	
EXPORT_C void COMASuplPosInit::SetVelocity(COMASuplVelocity* aVelocity)
{	
	delete iVelocity;
	iVelocity = NULL;
	iVelocity = aVelocity;
	if(iVelocity)
		{
		iOptionalMask |= LEFTSHIFT(4);	
		}		
}	  

// -----------------------------------------------------------------------------
// COMASuplPosInit::COMASuplPosInit() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPosInit::COMASuplPosInit()
	{
	ieCID = EFalse;	
	}
// -----------------------------------------------------------------------------
// COMASuplPosInit::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplPosInit::ConstructL()
    {
    iOptionalMask = 0;
    SetMessageType(COMASuplAsnMessageBase::ESUPL_POSINIT);
    }

// -----------------------------------------------------------------------------
// COMASuplPosInit::IntializeECellId() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPosInit::SetECellId(TOMASuplECellInfo& aECId)
	{
		iMmCellInfo = aECId;
		ieCID = ETrue ;
	}

// -----------------------------------------------------------------------------
// COMASuplPosInit::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPosInit::Clone(const COMASuplAsnMessageBase* aPosInit,TInt& aErrorCode)
    {
    TRAP(aErrorCode, CloneBaseL(aPosInit,aErrorCode));
    COMASuplPosInit* posInit = (COMASuplPosInit*)aPosInit;
    if(posInit->iSETCapabilities && posInit->iLocationId)
        TRAP(aErrorCode,SetSuplPosInit(static_cast<COMASuplSETCapabilities*>(posInit->iSETCapabilities->CloneL()),posInit->iLocationId->CloneL(),posInit->ieCID));
    if(posInit->iPosition)
        TRAP(aErrorCode,SetPosition(static_cast<COMASuplPosition*>(posInit->iPosition->CloneL())));
    if(posInit->iReqAsstData)
        TRAP(aErrorCode,SetRequestAssistanceData(static_cast<COMASuplReqAsstData*>(posInit->iReqAsstData->CloneL())));
    
    TBuf8<KVer> ver;
    ver.Append(posInit->iVer);
    SetVer(ver);
    if(posInit->iPosPayload)
        TRAP(aErrorCode,SetSuplPosPayload(static_cast<COMASuplPosPayload*>(posInit->iPosPayload->CloneL())));
    if(posInit->iVelocity)
        TRAP(aErrorCode,SetVelocity(static_cast<COMASuplVelocity*>(posInit->iVelocity->CloneL())));
    if(posInit->ieCID)
        SetECellId(posInit->iMmCellInfo);
    }		

