// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// A test proxy class for the CWapBoundDatagramService class
// 
//

#ifndef __TE_WAPBOUNDDATAGRAMSERVICE_H__
#define __TE_WAPBOUNDDATAGRAMSERVICE_H__

/**
@file
@internalComponent
@prototype
*/

#include <e32base.h>

class CWapBoundDatagramService;

/**
A test proxy class for the CWapBoundDatagramService class 
*/
class CTe_WapBoundDatagramService : public CBase
	{
private:
	static CTe_WapBoundDatagramService* iLastWdpConn;
	
public:
	static CTe_WapBoundDatagramService& WdpConn();
	
public:
	static CTe_WapBoundDatagramService* NewL();
	virtual ~CTe_WapBoundDatagramService();
	
	TInt Connect(Wap::TBearer aBearer, Wap::TPort aPort);
	TInt AwaitRecvDataSize(TPckg< TUint16 > &aDataSizePckg, TRequestStatus &aReqStatus);
	TInt RecvFrom(TDes8 &aRemoteHost, Wap::TPort &aRemotePort, TDes8 &aBuffer, TBool &aTruncated,
			TRequestStatus &aReqStatus, TUint32 aTimeout);
	void CancelRecv();
	TInt GetBearer(Wap::TBearer &aBearer);
	
	
private:	
	CTe_WapBoundDatagramService();
	void ConstructL();
		
private:
	/** A pointer on the original class object */
	CWapBoundDatagramService* iWdpConn;
	};
	
#endif //__TE_WAPBOUNDDATAGRAMSERVICE_H__
