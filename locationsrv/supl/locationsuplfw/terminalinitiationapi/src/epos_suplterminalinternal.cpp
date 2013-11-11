/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:   Client-Server communication
*
*/
#include <s32mem.h>
#include <e32std.h>

#include "epos_suplterminalinternal.h"
#include "epos_suplterminalipc.h"
#include "epos_suplterminalerrors.h"
#include "epos_suplterminalptrholder.h"

//-----------------------------------------------------------------------------
// RSuplTerminalInternal
//-----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// RSuplTerminalInternal::RSuplTerminalInternal()
// C++ default constructor can NOT contain any code, that might leave.
// -----------------------------------------------------------------------------
//
EXPORT_C RSuplTerminalInternal::RSuplTerminalInternal()
  : iConversionRequestIssued( EFalse )
   	{
	}

// ---------------------------------------------------------
// RSuplTerminalInternal::ConvertAreaInfoToCoordinates
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalInternal::ConvertAreaInfoToCoordinates( TRequestStatus& aStatus, 
                                                                   TGeoCellInfo& aGeoCellInfo )
    {  
    __ASSERT_ALWAYS(SubSessionHandle(), 
                    User::Panic(KSuplClientFault, ESuplServerBadHandle));
    
    // Currently only SUPL 1.0 supports cell to geo-coordinates conversion.
    if( iSuplService != ESUPL_1_0 )
        {
        TRequestStatus *status = &aStatus; 
        User::RequestComplete(status,KErrNotSupported); 
        return;
        }
        
    // Fill the values of data member
    iGeoCellInfo.iMNC = aGeoCellInfo.iMNC;
    iGeoCellInfo.iMCC = aGeoCellInfo.iMCC;
    iGeoCellInfo.iCid = aGeoCellInfo.iCid;
    iGeoCellInfo.iLac = aGeoCellInfo.iLac;
    iGeoCellInfo.iConnectionPrompt = aGeoCellInfo.iConnectionPrompt;
    iGeoCellInfo.iWlanOnly = aGeoCellInfo.iWlanOnly; //OCC
    iGeoCellInfo.iGeoCellType = aGeoCellInfo.iGeoCellType;
   
    iPtrHolder->Ptr(0).Set(
        reinterpret_cast<TUint8*>(&iGeoCellInfo),
        sizeof(iGeoCellInfo),sizeof(iGeoCellInfo));

    TIpcArgs args;

    args.Set(0,&iPtrHolder->Ptr(0));
    iConversionRequestIssued = ETrue;
    SendReceive(ESuplTerminalSubssnLocationConversion, args, aStatus);  
    }

// ---------------------------------------------------------
// RSuplTerminalInternal::CancelLocationConversion
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void RSuplTerminalInternal::CancelLocationConversion()
    {  
    __ASSERT_ALWAYS(SubSessionHandle(), 
            User::Panic(KSuplClientFault, ESuplServerBadHandle));   
    if( iConversionRequestIssued )
        {
        iConversionRequestIssued = EFalse; 
        SendReceive(ESuplTerminalSubssnCancelLocationConversion); 
        }    
    }
// end of file
