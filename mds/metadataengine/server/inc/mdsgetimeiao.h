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

#ifndef _MDSGETIMEIAO_H_
#define _MDSGETIMEIAO_H_

#include <e32base.h>
#include <etel3rdparty.h>


class CMdsGetImeiAO: public CActive
	{
	private:
		CTelephony *iTelephony;
		CTelephony::TPhoneIdV1 iV1;	
		TBuf<CTelephony::KPhoneSerialNumberSize> iImei;
		CActiveSchedulerWait iAsw;
	public:
	   	virtual ~CMdsGetImeiAO();
	   	static CMdsGetImeiAO* NewL();
	   	static CMdsGetImeiAO* NewLC();
		void GetIMEI(TDes &aImei);
		TInt64 GetIMEI();

		void DoCancel();

		void RunL();

	private:
		CMdsGetImeiAO(): CActive(CActive::EPriorityStandard), iTelephony(NULL)
		{}
		void ConstructL();

	};

#endif //_MDSGETIMEIAO_H_

