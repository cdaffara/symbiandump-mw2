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
* Description:  implementation for supl 2.0 end message
*
*/

// INCLUDE FILES

#include <e32def.h>
#include "epos_comasuplendver2.h"
#include "lbs/epos_comasuplsetcapabilities.h"
#include "lbs/epos_comasuplposition.h"
#include "lbs/epos_eomasuplposerrors.h"

const TInt KSetCapsShift = 3;


// ============================ MEMBER FUNCTIONS of COMASuplEndVer2===============================
    
// -----------------------------------------------------------------------------
// COMASuplEndVer2::NewL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplEndVer2* COMASuplEndVer2::NewL()
    {
    COMASuplEndVer2* self = new( ELeave ) COMASuplEndVer2;
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }
    
// -----------------------------------------------------------------------------
// COMASuplEndVer2::~COMASuplEndVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
EXPORT_C COMASuplEndVer2::~COMASuplEndVer2()
    {
    if(iSETCapabilities)
        {
        delete iSETCapabilities;
        iSETCapabilities = NULL;
        }
    }
// -----------------------------------------------------------------------------
// COMASuplEndVer2::Position()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C TInt COMASuplEndVer2::SETCapabilities(COMASuplSETCapabilities *& aSETCapabilities)const
{
	if(iOptionalMask & (1<<(KSetCapsShift)))
		{
 		aSETCapabilities = iSETCapabilities;
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}
}						  			   

// -----------------------------------------------------------------------------
// COMASuplEndVer2::SetPosition() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   			
EXPORT_C void COMASuplEndVer2::SetSETCapabilities(COMASuplSETCapabilities* aSETCapabilities)
{	
	delete iSETCapabilities;
	iSETCapabilities = NULL;
	
	iSETCapabilities = aSETCapabilities;
	if(iSETCapabilities)
		{
		iOptionalMask |= LEFTSHIFT(KSetCapsShift);		
		}	
}	
// -----------------------------------------------------------------------------
// COMASuplEndVer2::Clone() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 
EXPORT_C void COMASuplEndVer2::Clone(const COMASuplAsnMessageBase* aEnd,TInt& aErrorCode)
    {
    TRAP(aErrorCode, CloneBaseL(aEnd,aErrorCode));
    if(aErrorCode)
        return;
    COMASuplEndVer2* suplEnd = (COMASuplEndVer2*)aEnd;
    iOptionalMask = suplEnd->iOptionalMask;
    if(suplEnd->iOptionalMask & (1<<(1)))
        {
        iStatusCode = suplEnd->iStatusCode; 
        }
    if(suplEnd->iOptionalMask & (1<<(2)))
        {
        iVer.Copy(suplEnd->iVer);
        }
    if(suplEnd->iOptionalMask & (1<<(0)))
        {
        delete iPosition;
        iPosition = NULL;
        TRAP(aErrorCode,iPosition =static_cast <COMASuplPosition *> (suplEnd->iPosition->CloneL()));
        }
    if(suplEnd->iOptionalMask & (1<<(KSetCapsShift)))
        {
        delete iSETCapabilities;
        iSETCapabilities = NULL;
        TRAP(aErrorCode,iSETCapabilities = static_cast <COMASuplSETCapabilities *> (suplEnd->iSETCapabilities->CloneL()));
        }
    }						  			   
// -----------------------------------------------------------------------------
// COMASuplEndVer2::ConstructL() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void COMASuplEndVer2::ConstructL()
    {
    	COMASuplEnd::ConstructL();
    }
// -----------------------------------------------------------------------------
// COMASuplEndVer2::COMASuplEndVer2() 
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C COMASuplEndVer2::COMASuplEndVer2()
	{
		
	}


