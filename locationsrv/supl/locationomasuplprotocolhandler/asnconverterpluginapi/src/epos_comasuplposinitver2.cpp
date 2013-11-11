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
* Description:  Header file for supl 2.0 pos init  message
*
*/

// INCLUDE FILES

#include <e32def.h>

#include "epos_comasuplposinitver2.h"
#include "epos_comasuplsetcapabilitiesver2.h"
#include "epos_comasupllocationidver2.h"
#include "lbs/epos_eomasuplposerrors.h"


// ============================ MEMBER FUNCTIONS of COMASuplPosInitVer2===============================
// -----------------------------------------------------------------------------
// COMASuplPosInitVer2::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPosInitVer2* COMASuplPosInitVer2::NewL()
    {
    COMASuplPosInitVer2* self = new( ELeave ) COMASuplPosInitVer2;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplPosInitVer2::~COMASuplPosInitVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplPosInitVer2::~COMASuplPosInitVer2()
    {
    delete iSETCapabilities2;	
    delete iLocationId2;
    }
// -----------------------------------------------------------------------------
// COMASuplPosInitVer2::SetLocationId2
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplPosInitVer2::SetLocationId2(COMASuplLocationIdVer2* aLocationId2)
    {
    if(aLocationId2)
        iOptionalMask |= LEFTSHIFT(5); //check optional nos....and assign new number
    delete iLocationId2;
    iLocationId2 = NULL;
    iLocationId2 = aLocationId2;
    }
// -----------------------------------------------------------------------------
// COMASuplPosInitVer2::SetSETCapabilities2
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplPosInitVer2::SetSETCapabilities2(COMASuplSETCapabilitiesVer2* aSETCapabilities)
    {
    if(aSETCapabilities)
        iOptionalMask |= LEFTSHIFT(6); //check optional nos....and assign new number

    delete iSETCapabilities2;
    iSETCapabilities2 = NULL;
    iSETCapabilities2 = aSETCapabilities;
    }
 
#if 0
// -----------------------------------------------------------------------------
// COMASuplPosInitVer2::RequestAssistanceData() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplPosInitVer2::RequestAssistanceData(COMASuplReqAsstData*& aReqAsstData)const
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
// COMASuplPosInitVer2::SetRequestAssistanceData() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPosInitVer2::SetRequestAssistanceData(COMASuplReqAsstData* aReqAsstData)
{	
	delete iReqAsstData;
	iReqAsstData = NULL;
	
	iReqAsstData = aReqAsstData;
	if(iReqAsstData)
		iOptionalMask |= LEFTSHIFT(0);
}	

#endif

// -----------------------------------------------------------------------------
// COMASuplPosInitVer2::COMASuplPosInitVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplPosInitVer2::COMASuplPosInitVer2()
	{

	}
// -----------------------------------------------------------------------------
// COMASuplPosInitVer2::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplPosInitVer2::ConstructL()
    {
    COMASuplPosInit::ConstructL();
    }

// -----------------------------------------------------------------------------
// COMASuplPosInitVer2::CloneL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplPosInitVer2::Clone(const COMASuplAsnMessageBase* aPosInit,TInt& aErrorCode)
{
	COMASuplPosInit::Clone(aPosInit,aErrorCode);
	COMASuplPosInitVer2* posInit = (COMASuplPosInitVer2*)aPosInit;
	if(posInit->iOptionalMask &  (1<<5))
	    TRAP(aErrorCode,SetSETCapabilities2(posInit->iSETCapabilities2->CloneL()));
	if(posInit->iOptionalMask & (1<<6))
	    TRAP(aErrorCode,SetLocationId2(posInit->iLocationId2->CloneL()));
	
}		

