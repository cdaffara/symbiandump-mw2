/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Converts cce call information to mediator call information
*
*/


#include "cccecallinfoconverter.h"
#include "cccelogger.h"

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// Converts the cce call information to mediator call info type
// ---------------------------------------------------------------------------
//
TCallInfo CCCECallInfoConverter::Convert( MCCECallInfo& aCallInfo ) const
    {
    CCELOGSTRING( "CCCECallInfoConverter::Convert(): In" );
    TCallInfo info;
    info.iState = aCallInfo.State();
    info.iDirection = aCallInfo.Direction();
    info.iType =  aCallInfo.CallType();
    info.iIndex = aCallInfo.CallIndex();
    info.iServiceId = aCallInfo.ServiceId();
    info.iEmergencyCall = aCallInfo.IsEmergency();
    
    CCELOGSTRING2( "CCCECallInfoConverter::Convert(): info.iState = %d", info.iState );
    CCELOGSTRING2( "CCCECallInfoConverter::Convert(): info.iDirection = %d", info.iDirection );
    CCELOGSTRING2( "CCCECallInfoConverter::Convert(): info.iType = %d", info.iType );
    CCELOGSTRING2( "CCCECallInfoConverter::Convert(): info.iIndex = %d", info.iIndex );
    CCELOGSTRING2( "CCCECallInfoConverter::Convert(): info.iServiceId = %d", info.iServiceId );
    CCELOGSTRING2( "CCCECallInfoConverter::Convert(): info.IsEmergency = %d", info.iEmergencyCall );
    
    CCELOGSTRING( "CCCECallInfoConverter::Convert(): Out" );
    return info;
    }
 
