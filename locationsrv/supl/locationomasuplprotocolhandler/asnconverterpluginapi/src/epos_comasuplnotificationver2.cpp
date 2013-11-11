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
* Description:  Header file for supl 2.0 notification ver2  message
*
*/


#include "epos_comasuplnotificationver2.h"
#include "epos_suplconstants.h"
#include  "lbs/epos_eomasuplposerrors.h"
#include "epos_comasuplsessionid.h"




EXPORT_C COMASuplNotificationExtnV2* COMASuplNotificationExtnV2::NewL()
    {
    COMASuplNotificationExtnV2* self = new(ELeave) COMASuplNotificationExtnV2;
    return self;
    }

EXPORT_C COMASuplNotificationExtnV2::~COMASuplNotificationExtnV2()
    {
    ;
    }

COMASuplNotificationExtnV2::COMASuplNotificationExtnV2()
    {
    ;
    }

// ============================ MEMBER FUNCTIONS of COMASuplNotificationVer2===============================
//
// ---------------------------------------------------------
// Static Factory method to create the COMASuplNotificationVer2
// implementation
//
// (other items were commented in a header).
// ---------------------------------------------------------
    
EXPORT_C  COMASuplNotificationVer2* COMASuplNotificationVer2::NewL()
    {
    COMASuplNotificationVer2* self = new(ELeave) COMASuplNotificationVer2;
    return self;
    }
//
// ---------------------------------------------------------
//  destructor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C COMASuplNotificationVer2::~COMASuplNotificationVer2()
    {
    if(iV2NotificationExtn)
        {
        delete iV2NotificationExtn;
        iV2NotificationExtn = NULL;
        }
    }
//
// ---------------------------------------------------------
//  Setter method for notification extension
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void COMASuplNotificationVer2::SetV2NotifyExtn(COMASuplNotificationExtnV2* aV2NotificationExtn)
    {
    if(iV2NotificationExtn)
        {
        delete iV2NotificationExtn;
        iV2NotificationExtn = NULL;
        }
    iV2NotificationExtn = aV2NotificationExtn;
    }
//
// ---------------------------------------------------------
//  Getter method for notification extension
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void COMASuplNotificationVer2::GetV2NotifyExtn(COMASuplNotificationExtnV2*& aV2NotificationExtn)
    {
    aV2NotificationExtn = iV2NotificationExtn;
    }
// ---------------------------------------------------------
//  constructor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
COMASuplNotificationVer2::COMASuplNotificationVer2()
	{
	}
    
