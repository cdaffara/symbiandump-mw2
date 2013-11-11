/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Implementation of class CSuplTerminalParams
*
*/



// INCLUDE FILES

#include "epos_suplterminalparams.h"
#include "epos_csuplsettingsconstants.h"

// CONSTANTS

// ==================== LOCAL FUNCTIONS ====================


// ================= MEMBER FUNCTIONS =======================

// C++ default constructor can NOT contain any code, that
// might leave.
//
CSuplTerminalParams ::CSuplTerminalParams ():iSlpId (-1),
                                        iServerAddress (NULL),
                                        iIap (NULL),
                                        iServerEnabled(EFalse),                                            
                                        iSimChangeRemove  (EFalse),
                                        iUsageInHomeNw (EFalse),
                                        iEditable (EFalse),
                                        
                                        iIsValueSet(EFalse)
    {
    }
    
//---------------------------------------------------------
//CSuplTerminalParams::NewL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CSuplTerminalParams* CSuplTerminalParams::NewL()
    {
    CSuplTerminalParams* self = new (ELeave) CSuplTerminalParams();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }
    
//---------------------------------------------------------
//CSuplTerminalParams::NewLC
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C CSuplTerminalParams* CSuplTerminalParams::NewLC()
    {
    CSuplTerminalParams* self = new (ELeave) CSuplTerminalParams();

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

//---------------------------------------------------------
//CSuplTerminalParams::ConstructL
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
// EPOC default constructor can leave.
void CSuplTerminalParams::ConstructL()
    {      
    }

// Destructor
EXPORT_C  CSuplTerminalParams::~CSuplTerminalParams()
    {
    if(iServerAddress)
        {
        delete iServerAddress;
        iServerAddress = NULL;
        }

    if(iIap)
        {
        delete iIap;
        iIap = NULL;
        }
    }
    
//---------------------------------------------------------
//CSuplTerminalParams::Set
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CSuplTerminalParams::Set( const TDesC& aServerAddress,          
                const TDesC& aIapName, const TBool aServerEnabled,
                const TBool aSimChangeRemove, const TBool aUsageInHomeNw,
                const TBool aEditable,const TInt64 aSlpId)
    {
    iIsValueSet = EFalse;
    iSlpId = aSlpId;
    if(aServerAddress.Length() > 0 && aServerAddress.Length() < KMaxHSLPAddrLen)
        {
        delete iServerAddress;
        iServerAddress = NULL;
        iServerAddress = HBufC::New(aServerAddress.Length());
        if(!iServerAddress)
            {
            return KErrNoMemory;
            }
        iServerAddress->Des().Copy(aServerAddress);
        }
    else
        return KErrArgument;
        
    if(aIapName.Length() > 0 && aIapName.Length() < KMaxIAPLen)
        {
        delete iIap;
        iIap = NULL;
        iIap = HBufC::New(aIapName.Length());
        if(!iIap)
            {
            return KErrNoMemory;
            }
        iIap->Des().Copy(aIapName);
        }
    else
        return KErrArgument;

    iServerEnabled = aServerEnabled;
    iSimChangeRemove = aSimChangeRemove;
    iUsageInHomeNw = aUsageInHomeNw;
    iEditable = aEditable;
    
    iIsValueSet = ETrue;
    return KErrNone;            
    }
    
//---------------------------------------------------------
//CSuplTerminalParams::Get
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C TInt CSuplTerminalParams::Get(TInt64 &aSlpId, TPtr aServerAddress,         
                TPtr aIapName,TBool& aServerEnabled,
                TBool& aSimChangeRemove, TBool& aUsageInHomeNw, TBool& aEditable) const
    {
    if(!iIsValueSet) //if values were not set before using the Set method, return error
        return KErrNotFound;
    
    aSlpId = iSlpId;
    
    if(iServerAddress)
        aServerAddress = iServerAddress->Des();
    
    if(iIap)
        aIapName = iIap->Des();
    
    aServerEnabled = iServerEnabled;
    aSimChangeRemove = iSimChangeRemove;
    aUsageInHomeNw = iUsageInHomeNw;
    aEditable = iEditable;
    return KErrNone;            
    }

//  End of File

