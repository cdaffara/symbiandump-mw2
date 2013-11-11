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
* Description: A class encapsulating an RDbView.
*
*
*/


// INCLUDE FILES
#include <s32strm.h>
#include <EPos_TPosLmDatabaseSettings.h>
#include "EPos_CPosLmDatabaseIterator.h"
#include "EPos_PosDbRegistryConstants.h"

//CONSTANTS

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CPosLmDatabaseIterator::CPosLmDatabaseIterator
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CPosLmDatabaseIterator::CPosLmDatabaseIterator(RDbView& aView) :
    CBase(), iView(aView)
    {
    }

// -----------------------------------------------------------------------------
// CPosLmDatabaseIterator::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
EXPORT_C CPosLmDatabaseIterator* CPosLmDatabaseIterator::NewL(RDbView& aView)
    {
    CPosLmDatabaseIterator* self = new (ELeave) CPosLmDatabaseIterator(aView);
    return self;
    }

// Destructor
EXPORT_C CPosLmDatabaseIterator::~CPosLmDatabaseIterator()
    {
    delete iUri;
    iView.Close();
    }

// -----------------------------------------------------------------------------
// CPosLmDatabaseIterator::NextItemL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TBool CPosLmDatabaseIterator::NextItemL()
    {
    TBool res = iView.NextL();
    if (res)
        {
        iView.GetL();
        }
    return res;
    }

// -----------------------------------------------------------------------------
// CPosLmDatabaseIterator::UriL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TPtrC CPosLmDatabaseIterator::UriL()
    {
    CDbColSet* colset = iView.ColSetL();
    CleanupStack::PushL(colset);

    TDbColNo colNo = colset->ColNo(KPosLmFileNameCol);
    TPtrC prot = iView.ColDes(colset->ColNo(KPosLmProtocolCol));
    TInt nameLength = iView.ColLength(colset->ColNo(KPosLmFileNameCol));

    CleanupStack::PopAndDestroy(colset);

    HBufC* name = HBufC::NewLC(nameLength);
    TPtr namePtr = name->Des();

    RDbColReadStream in;
    in.OpenLC(iView, colNo);
    in.ReadL(namePtr, nameLength);
    CleanupStack::Pop(&in);
    in.Close();

    delete iUri;
    iUri = NULL;
    iUri = HBufC::NewL(prot.Length() + KProtocol().Length() + namePtr.Length());

    TPtr uriPtr = iUri->Des();
    uriPtr.Append(prot);
    uriPtr.Append(KProtocol);
    uriPtr.Append(*name);

    CleanupStack::PopAndDestroy(name);

    return TPtrC(*iUri);
    }

// -----------------------------------------------------------------------------
// CPosLmDatabaseIterator::GetSettingsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPosLmDatabaseIterator::GetSettingsL(
    TPosLmDatabaseSettings& aSettings)
    {
    CDbColSet* colset = iView.ColSetL();
    CleanupStack::PushL(colset);
    TPckg<TPosLmDatabaseSettings> dbPckg(aSettings);

    RDbColReadStream in;
    in.OpenLC(iView, colset->ColNo(KPosLmSettingsCol));
    in.ReadL(dbPckg);
    CleanupStack::Pop(&in);
    in.Close();
    CleanupStack::PopAndDestroy(colset);
    }

//  End of File
