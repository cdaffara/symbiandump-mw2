/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/
#include <QDebug>
#include <smcmockclassincludes.h>
#include "etelmm.h"

// ============================ MEMBER FUNCTIONS ===============================



// -----------------------------------------------------------------------------
// RMobilePhone::GetMultimodeCaps
// -----------------------------------------------------------------------------
//
TInt RMobilePhone::GetMultimodeCaps( 
        TUint32 & aCaps ) const
    {
    SMC_MOCK_METHOD1( TInt, TUint32 &, aCaps )
    }


// -----------------------------------------------------------------------------
// RMobilePhone::GetCurrentMode
// -----------------------------------------------------------------------------
//
TInt RMobilePhone::GetCurrentMode( 
        TMobilePhoneNetworkMode & aNetworkMode ) const
    {
    SMC_MOCK_METHOD1( TInt, TMobilePhoneNetworkMode &, aNetworkMode )
    }


// -----------------------------------------------------------------------------
// RMobilePhone::GetSignalStrength
// -----------------------------------------------------------------------------
//
void RMobilePhone::GetSignalStrength( 
        TRequestStatus & aReqStatus,
        TInt32 & aSignalStrength,
        TInt8 & aBar ) const
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aReqStatus, 
        TInt32 &, aSignalStrength, 
        TInt8 &, aBar )
    }


// -----------------------------------------------------------------------------
// RMobilePhone::NotifySignalStrengthChange
// -----------------------------------------------------------------------------
//
void RMobilePhone::NotifySignalStrengthChange( 
        TRequestStatus & aReqStatus,
        TInt32 & aSignalStrength,
        TInt8 & aBar ) const
    {
    SMC_MOCK_METHOD3( void, TRequestStatus &, aReqStatus, 
        TInt32 &, aSignalStrength, 
        TInt8 &, aBar )
    }


