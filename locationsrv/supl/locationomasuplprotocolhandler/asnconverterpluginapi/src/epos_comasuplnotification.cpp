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
* Description:  Header file for Notification in Supl Init
*
*/



#include "epos_comasuplnotification.h"
#include "epos_suplconstants.h"
#include  "lbs/epos_eomasuplposerrors.h"

// ============================ MEMBER FUNCTIONS of COMASuplNotification===============================
//
// ---------------------------------------------------------
// Static Factory method to create the COMASuplNotification
// implementation
//
// (other items were commented in a header).
// ---------------------------------------------------------
    
EXPORT_C  COMASuplNotification* COMASuplNotification::NewL()
    {
    COMASuplNotification* self = new(ELeave) COMASuplNotification;
    return self;
    }
//
// ---------------------------------------------------------
//  destructor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C COMASuplNotification::~COMASuplNotification()
    {
    if(iRequestorId)
    	{
    	delete iRequestorId;
    	iRequestorId = NULL;
    	}
    if(iClientName)
    	{
    	delete iClientName;
    	iClientName = NULL;    	
    	}
    }
//
// ---------------------------------------------------------
//  Retrieves the type of Notification
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C COMASuplNotification::TOMASuplNotificationType COMASuplNotification::SuplNotification()const
    {
    return iNotificatonType;
    }

//
// ---------------------------------------------------------
// Sets the type of Notification
// implementation
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
    
EXPORT_C  void COMASuplNotification::SetSuplNotification(TOMASuplNotificationType aNotificationType)
    {
    iNotificatonType = aNotificationType;
	}           

//
// -------------------------------------------------------------
// Retrieves the requestor id and type of requestor
// Implementation
//
// (other items were commented in a header).
// -------------------------------------------------------------
//
EXPORT_C  TInt COMASuplNotification::RequestorId(HBufC8*& aRequestorId,
												 TOMASuplFormatIndicator& aRequestorType)const
    {
    
	aRequestorId   = iRequestorId;
	aRequestorType = iRequestorType;

    if(iOptionalMask & (1<<(2)))
    	{
//		aRequestorId   = iRequestorId;
//		aRequestorType = iRequestorType;
    	return KErrNone;
    	}
    else
    	{
    	return KErrOMASuplParamNotSet;
    	}
    
    }

//
// ---------------------------------------------------------
//  Sets the requestor id and type of requestor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
EXPORT_C void COMASuplNotification::SetRequestorId(HBufC8* aRequestorId,
													TOMASuplFormatIndicator aRequestorType)
    {
    delete iRequestorId;
    iRequestorId= NULL;
    iRequestorId = aRequestorId;
    if(iRequestorId)
    	{
    	iOptionalMask |= LEFTSHIFT(2);
    	iRequestorType = aRequestorType;		
    	}
    }

//
// ---------------------------------------------------------
// Retrieves the client name and type of client
// implementation
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
    
EXPORT_C  TInt COMASuplNotification::ClientName(HBufC8*& aClientName,
													TOMASuplFormatIndicator& aClientType)const
    {
    
       	aClientName = iClientName;
    	aClientType = iClientType;

    if(iOptionalMask & (1<<(3)))
    	{
//    	aClientName = iClientName;
//    	aClientType = iClientType;
    	return KErrNone;		
    	}
    else
    	{
    	return KErrOMASuplParamNotSet;
    	}
    
	}           

//
// -------------------------------------------------------------
// Sets the client name and type of client
// Implementation
//
// (other items were commented in a header).
// -------------------------------------------------------------
//
EXPORT_C  void COMASuplNotification::SetClientName(HBufC8* aClientName,
														TOMASuplFormatIndicator aClientType)
    {
    delete iClientName;
    iClientName = NULL;
    iClientName = aClientName;
    if(iClientName)
    	{
    	iClientType    = aClientType;
    	iOptionalMask |= LEFTSHIFT(3);	
    	}
    }
        
//
// -------------------------------------------------------------
// Retrieves the values of EncodingType
// Implementation
//
// (other items were commented in a header).
// -------------------------------------------------------------
//
EXPORT_C  TInt COMASuplNotification::EncodingType(TOMASuplEncodingType& aEncodingType)const
    {
	if(iOptionalMask & (1<<(1)))
		{
		aEncodingType = iEncodingType;	
		return KErrNone;
		}
	else
		{
		return KErrOMASuplParamNotSet;
		}

    }
//
// ---------------------------------------------------------
// Sets the values of EncodingType
// implementation
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
    
EXPORT_C  void COMASuplNotification::SetEncodingType(TOMASuplEncodingType aEncodingType)
    {
    iEncodingType = aEncodingType;
    iOptionalMask |= LEFTSHIFT(1);
    }
//
// ---------------------------------------------------------
//  constructor
//
// (other items were commented in a header).
// ---------------------------------------------------------
//
COMASuplNotification::COMASuplNotification():
        iOptionalMask(0)
	{
	}
    
