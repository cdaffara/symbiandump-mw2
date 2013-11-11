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
* Description:  Provides the interface between SUPL Protocol hander and ASN	 Message plugin.
*
*/


#include <ecom/ecom.h>
#include <badesca.h>

#include "epos_comasuplasnbase.h"     // Our own base implementations for ECOM

//Resets & destroys array if leave occurs
void ResetAndDestroy(TAny* aArray)
    {
    ((RImplInfoPtrArray*)aArray)->ResetAndDestroy();
    }

// ============================ MEMBER FUNCTIONS ===============================


// Interface's (abstract base class's) constuctor
EXPORT_C COMASuplAsnHandlerBase::COMASuplAsnHandlerBase()
    {
    }

// Interface's (abstract base class's) destructor
EXPORT_C COMASuplAsnHandlerBase::~COMASuplAsnHandlerBase()
    {
    // Inform the ECOM framework that this specific instance of the 
    // interface has been destroyed.
    REComSession::DestroyedImplementation(iDtor_ID_Key);
    }

// Interface's (abstract base class's) static factory method implementation.
// Asks ECOM plugin framework to instantiate appropriate concret plugin 
// implementation.
// Returns instance of COMASuplAsnHandlerBase

EXPORT_C  COMASuplAsnHandlerBase* COMASuplAsnHandlerBase::NewL(const TInt& aOMASuplVersion)
    {

    RImplInfoPtrArray implInfoArray;
    CleanupStack::PushL(TCleanupItem(*ResetAndDestroy, &implInfoArray));
    REComSession::ListImplementationsL(KAsnHandlerInterfaceDefinitionUid, implInfoArray);

    TInt cnt = implInfoArray.Count();
    TUid ImplementationUid;

    TBuf<32> buffer(_L("V0"));
    buffer.AppendNum(aOMASuplVersion);

    TInt suplVersion = KErrNotFound;
    for (TInt index = 0; index < cnt; index++)
        {
        if ((implInfoArray[index]->DisplayName().Compare(buffer)) == 0)
            {
            ImplementationUid = implInfoArray[index]->ImplementationUid();
            suplVersion = aOMASuplVersion;
            break;
            }
        }

    CleanupStack::PopAndDestroy(&implInfoArray);

    if(suplVersion == KErrNotFound)
        {

        User::Leave(KErrNotFound);
        }

    TAny* ptr = REComSession::CreateImplementationL(ImplementationUid,_FOFF(COMASuplAsnHandlerBase,iDtor_ID_Key)); 

    COMASuplAsnHandlerBase* OMASuplAsnHandler = REINTERPRET_CAST(COMASuplAsnHandlerBase*, ptr);

    CleanupStack::PushL(OMASuplAsnHandler);
    OMASuplAsnHandler->ConstructL(ImplementationUid,suplVersion);	
    CleanupStack::Pop(OMASuplAsnHandler);


    return OMASuplAsnHandler;

    }           

// Returns implementation UID     
EXPORT_C  TUid COMASuplAsnHandlerBase::ImplementationUid() const
    {
    return  iImplementationUid;
    }           

// Returns Version of implementation 
EXPORT_C  TInt COMASuplAsnHandlerBase::Version() const
    {
    return iSuplVersion; 
    }           

//ConstructL 2 phase constructor
EXPORT_C void COMASuplAsnHandlerBase::ConstructL(const TUid &aUid,TInt aSuplVersion) 
    {
		iImplementationUid = aUid;
		iSuplVersion = aSuplVersion;
    }        
       
