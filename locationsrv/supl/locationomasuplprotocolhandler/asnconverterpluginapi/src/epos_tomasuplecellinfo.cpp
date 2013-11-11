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
* Description:  This class is used to hold different values related to E-Cell id
*
*/


// INCLUDE FILES

#include "epos_tomasuplecellinfo.h"

// ============================ MEMBER FUNCTIONS OF TOMASuplECellInfo ===============================

// -----------------------------------------------------------------------------
// TOMASuplECellInfo::TOMASuplECellInfo,constuctor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

EXPORT_C TOMASuplECellInfo::TOMASuplECellInfo()
    {  
    iOMASuplNwMode = EOMASuplUnknown;
    }

// -----------------------------------------------------------------------------
// TOMASuplECellInfo::SetWCDMACellInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplECellInfo::SetWCDMACellInfo(TOMASuplWCDMACellinfo&  aOMASuplWCDMACellinfo)
    {
    iOMASuplNwMode = EOMASuplWCDMA;					
    iOMASuplWcdmaCellInfo = aOMASuplWCDMACellinfo;
    }

// -----------------------------------------------------------------------------
// TOMASuplECellInfo::SetGSMCellInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplECellInfo::SetGSMCellInfo(TOMASuplGSMCellinfo& aOMASuplGSMCellinfo)
    {
    iOMASuplNwMode = EOMASuplGSM;
    iOMASuplGsmCellInfo = aOMASuplGSMCellinfo;
    }

// -----------------------------------------------------------------------------
// TOMASuplECellInfo::WCDMACellInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplECellInfo::WCDMACellInfo(TOMASuplWCDMACellinfo&  aOMASuplWCDMACellinfo) const
    {
    aOMASuplWCDMACellinfo = iOMASuplWcdmaCellInfo;
    }

// -----------------------------------------------------------------------------
// TOMASuplECellInfo::GSMCellInfo
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void TOMASuplECellInfo::GSMCellInfo(TOMASuplGSMCellinfo& aOMASuplGSMCellinfo) const
    {
    aOMASuplGSMCellinfo = iOMASuplGsmCellInfo;
    }

// -----------------------------------------------------------------------------
// TOMASuplECellInfo::NetworkMode
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C TOMASuplNwMode TOMASuplECellInfo::NetworkMode() const
    {
    return iOMASuplNwMode;
    }
