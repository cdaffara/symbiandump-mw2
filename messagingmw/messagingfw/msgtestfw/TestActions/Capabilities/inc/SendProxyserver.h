// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// server.h
// Transient server example - server classes
// 
//

#ifndef __SENDPROXYSERVER_H__
#define __SENDPROXYSERVER_H__


#include <e32base.h>
#include <e32std.h>

#include "SendProxyClientServer.h"

#include <mclient.h>
	// Required for RMsvServerSession


enum TSendProxyServerPanic
	{
	EPanicBadDescriptor,
	EPanicIllegalFunction,
	EPanicAlreadyReceiving
	};

void PanicClient(const RMessagePtr2& aMessage,TSendProxyServerPanic TSendProxyServerPanic);

class CShutdown : public CTimer
	{
	enum {KSendProxyShutdownDelay=0x200000};	// approx 2s
public:
	inline CShutdown();
	inline void ConstructL();
	inline void Start();
private:
	void RunL();
	};




//
// Used to get acces to send recieve. 
// It is better to encapsulate this than use inheritence into the step.
//

class RMsvServerSessionTest: public RMsvServerSession
{
public :
	TInt SendReceive( TInt aFunction,const TIpcArgs& aArgs ) const;
	TInt SendReceive( TInt aFunction ) const;
		// All we want is to access. Since RSessionBase::SendReceive is protected so is RMsvServerSession::SendReceive
		// So as long as we derive from RSessionBase base we should be get access to SendReceive.
		
	void SendReceive( TInt aFunction,const TIpcArgs& aArgs,TRequestStatus& aRequestStatus ) const;		
	void SendReceive( TInt aFunction,TRequestStatus& aRequestStatus) const;
	
	void SendReceiveAsync( TInt aFunction,const TIpcArgs& aArgs,TRequestStatus& aRequestStatus ) const;		
	void SendReceiveAsync( TInt aFunction,TRequestStatus& aRequestStatus) const;

	TInt CreateSession(const TDesC& aServer,const TVersion& aVersion,TInt aAsyncMessageSlots);

};


class CSendProxyServer : public CServer2
	{
public:
	static CServer2* NewLC();
	void AddSession();
	void DropSession();
	
private:
	CSendProxyServer();
	void ConstructL();
	CSession2* NewSessionL(const TVersion& aVersion, const RMessage2& aMessage) const;
private:
	TInt iSessionCount;
	CShutdown iShutdown;
			
	};



// Just used to get acces to the array .iArgs. 
// This may be an inadequency of the API we can get buffers according to args
// but not the RInts.
// 
class RMessage2Derv : public RMessage2
{
public: 
	IMPORT_C TInt GetiArgs(TInt aPos) const;	
};



class CSendProxyServerSession : public CSession2
	{
public:
	CSendProxyServerSession();
	void CreateL();
	
private:
	~CSendProxyServerSession();
	inline CSendProxyServer& Server();
	void ServiceL(const RMessage2& aMessage);
	void ServiceError(const RMessage2& aMessage,TInt aError);
	inline TBool ReceivePending() const;
	

private :
	void SetupTIpcArgL( const RMessage2Derv& aMessage, TInt aPos, TIpcArgs& atipcArgs);
	void SetupTIpcArgsL( const RMessage2Derv& aMessage, TIpcArgs& atipcArgs);
	
	void SetupReturnTIpcArgL(const RMessage2Derv& aMessage, TInt aPos);
	void SetupReturnTIpcArgsL(const RMessage2Derv& aMessage);
	
	void ReadCapabilities(TInt32& aServerCaps);
	
	void ClearBuffer(TInt aPos);
	void ClearBuffers();
	TInt DoCommandL(const RMessage2Derv& aMessage );						
	
private:
	RMessagePtr2 iReceiveMsg;
	TInt iReceiveLen;
	RMsvServerSessionTest iMsvServerSession;
	TInt iSessionCount;

	TSendProxyMessages iCommand;	// Which type of send we are expected to perform.
	TInt iFlags; // from TIpcArgs, we use these flags to set up buffers if we require.
	RBuf8 iBuffer8[KMaxMessageArguments];
	TPtr8* iTPtr8[KMaxMessageArguments];

	RBuf16 iBuffer16[KMaxMessageArguments];
	TPtr16* iTPtr16[KMaxMessageArguments];
	
	};






//TInt StartMsvServer();


#endif // __SENDPROXYSERVER_H__

