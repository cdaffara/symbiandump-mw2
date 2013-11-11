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
* Description:  Qop field in message
*
*/


// INCLUDE FILES
#include "epos_tomasuplqop.h"
#include "epos_suplconstants.h"
#include  "lbs/epos_eomasuplposerrors.h"

// ============================ MEMBER FUNCTIONS OF TOMASuplQop ===============================

// -----------------------------------------------------------------------------
// TOMASuplQop::TOMASuplQop,constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C TOMASuplQop::TOMASuplQop()
    {  
    iHorizAccuracy  = -1;
    iVertAccuracy   = -1;
    iMaxLocationAge = -1;
    iDelay          = -1;
    iOptionalMask = 0;
    }
// -----------------------------------------------------------------------------
// TOMASuplQop::SuplQop()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplQop::SuplQop() const
    {
    return(iHorizAccuracy);  
    }
// -----------------------------------------------------------------------------
// TOMASuplQop::VerticalAccuracy()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplQop::VerticalAccuracy(TInt& aVertAccuracy) const
    {	
    if(iOptionalMask & (1<<(0)))//check if vertical accuracy is set
        {
        aVertAccuracy   =  iVertAccuracy;    	
        return KErrNone;
        }
    else
        {
        return KErrOMASuplParamNotSet;
        }	 
    }
// -----------------------------------------------------------------------------
// TOMASuplQop::MaxLocationAge()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplQop::MaxLocationAge(TInt& aMaxLocationAge) const
    {
    if(iOptionalMask & (1<<(1)))//check if max location age is set
        {
        aMaxLocationAge   =  iMaxLocationAge;    	
        return KErrNone;
        }
    else
        {
        return KErrOMASuplParamNotSet;
        }	
    }
// -----------------------------------------------------------------------------
// TOMASuplQop::Delay()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TInt TOMASuplQop::Delay(TInt& aDelay) const
    {
    if(iOptionalMask & (1<<(2)))//check if delay is set
        {
        aDelay   =  iDelay;  	
        return KErrNone;
        }
    else
        {
        return KErrOMASuplParamNotSet;
        }	
    }
// -----------------------------------------------------------------------------
// TOMASuplQop::SetSuplQop()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplQop::SetSuplQop(TInt aHorizAccuracy)
    {
    iHorizAccuracy   =  aHorizAccuracy;  
    }
// -----------------------------------------------------------------------------
// TOMASuplQop::SetVerticalAccuracy()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplQop::SetVerticalAccuracy(TInt aVertAccuracy)
    {
    iVertAccuracy   =  aVertAccuracy;  
    iOptionalMask |= LEFTSHIFT(0);//set optional bit mask to indicate that vertical accuracy is set
    }
// -----------------------------------------------------------------------------
// TOMASuplQop::SetMaxLocationAge()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplQop::SetMaxLocationAge(TInt aMaxLocationAge)
    {
    iMaxLocationAge   =  aMaxLocationAge; 
    iOptionalMask |= LEFTSHIFT(1); //set optional bit mask to indicate that location age is set
    }
// -----------------------------------------------------------------------------
// TOMASuplQop::SetDelay()
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplQop::SetDelay(TInt aDelay)
    {
    iDelay   =  aDelay;  
    iOptionalMask |= LEFTSHIFT(2);//set optional mask to indicate that delay is set
    }

