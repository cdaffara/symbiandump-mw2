// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// BIO-message Socket Port Watcher
// 
//

#ifndef WAPSOCKETWATCHER_H__
#define WAPSOCKETWATCHER_H__

#include <smsuaddr.h>
#include <gsmumsg.h>
#include <gsmubuf.h>
#include <smsustrm.h>
#include <etelmm.h>


#include <e32base.h>
#include "SmsSocketWatcher.h"
#include <es_wsms.h>

#include <wapmessage.h>

const TInt KReceiveBufferSize = 256;
const TInt KWspReceiveHeaderSize = 64;


//**********************************
// CWapPortWatcher
//**********************************
//
// 
//
class CWapPortWatcher : public CBaseSmsActiveSocketWatcher
	{
	public:
		static CWapPortWatcher* NewL(TMsvId aBioServiceId, 
									 TMsvId aSmsServiceId, 
									 CBIODatabase& aBioDb, 
									 CWatcherLog& aWatcherLog,
									 TInt aPriority, 
									 TUid aBioID, 
									 RFs& aFs, 
									 const TBioMsgId& aBioMsg); 
		~CWapPortWatcher();
		
	protected:
		CWapPortWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs, Wap::TPort aWapPort);
		void DoReceiveL(const TDesC8& aRecvFrom);

		void DoComplete(TInt&);
		virtual void CloseConn();
		void AppendToHBufC8L(HBufC8*& aBuf, const TDesC8& aAppend);

		void DoRunL();
		void CompleteSelf();
	protected:
		//The entire received message
		HBufC8* iRecvBuf;
		Wap::TPort iWapPort;
	};



//
// CWdpPortWatcher
//

class CWdpPortWatcher : public CWapPortWatcher
	{
	public:
		CWdpPortWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs, TUint16 aWapPort);
		~CWdpPortWatcher();
		void DoCancel();
		void DoSetupL();

	private:
		void CloseConn();
		void WaitForMessageL();
		void ReceiveL();

	private:
		//Members required for receiving
		TBuf8<RMobilePhone::KMaxMobileTelNumberSize> iRemoteAddress;
		Wap::TPort iRemotePort;
		CWapBoundDatagramService* iWdpConn;
		TBool	iTruncated;

		TPtr8 iRecvPtr;
		TUint16 iDataSize;
		TPckg<TUint16> iDataSizePckg;

		enum TWdpRecvState
			{
			ERecvSize,
			ERecv
			} iRecvNext;
	};



//
// CWspPortWatcher
//

class CWspPortWatcher : public CWapPortWatcher
	{
	public:
		CWspPortWatcher(CWatcherLog& aWatcherLog, TInt aPriority, TUid aBioID, RFs& aFs, TUint16 aWapPort);
		~CWspPortWatcher();

		void DoCancel();
		void DoSetupL();

	private:
		void CloseConn();
		void WaitForMessageL();
		void ReceiveL();

	private:
		//Members required for receiving
		HBufC8* iRecvHeader;
		TPckgBuf <TUint8> iPushPckgId;
		CWapBoundCLPushService* iWspConn;

		//Used to receive parts of each message.
		TBuf8<KReceiveBufferSize> iTempRecvBuf;
		TBuf8<KWspReceiveHeaderSize> iTempRecvHeader;
	};

#endif //WAPSOCKETWATCHER_H__
