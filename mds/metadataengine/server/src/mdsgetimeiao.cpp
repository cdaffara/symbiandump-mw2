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
* Description:  Helper class to get IMEI number.
*
*/

/**
 * Only used in CMdESessionImpl::GetImei() method.
 */

#include "mdsgetimeiao.h"

CMdsGetImeiAO* CMdsGetImeiAO::NewL()
	{
	CMdsGetImeiAO *self = CMdsGetImeiAO::NewLC();
	CleanupStack::Pop(); //self
	return self;
	}

CMdsGetImeiAO* CMdsGetImeiAO::NewLC()
	{
	CMdsGetImeiAO *self = new (ELeave) CMdsGetImeiAO();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CMdsGetImeiAO::ConstructL()
   {
   iTelephony = CTelephony::NewL();
   CActiveScheduler::Add(this);
   }

CMdsGetImeiAO::~CMdsGetImeiAO()
	{
	Cancel();

	delete iTelephony;
	iTelephony = NULL;
	}

TInt64 CMdsGetImeiAO::GetIMEI()
	{	
	const TInt64 KRandomImei = 123456789012345;
	TInt64 serial = KRandomImei;

#ifdef __ARMV5__ // skip getting IMEI for emulator
	CTelephony::TPhoneIdV1Pckg phoneIdPckg( iV1 ); 	
	
	iTelephony->GetPhoneId( iStatus, phoneIdPckg );
	SetActive();
	iAsw.Start();
	Deque();
	TLex imeiConverter(iImei);
	
    imeiConverter.Val(serial, EDecimal);
#endif
    
    return serial;
	}

void CMdsGetImeiAO::DoCancel()
	{
	iTelephony->CancelAsync(CTelephony::EGetPhoneIdCancel);
	}
   
void CMdsGetImeiAO::RunL()
	{
	if(iStatus == KErrNone)
    	{
    	iImei = iV1.iSerialNumber;
    	}
    iAsw.AsyncStop();
   	}
