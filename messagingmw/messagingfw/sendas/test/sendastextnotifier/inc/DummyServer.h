// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#include <e32base.h>
#include <msvapi.h>

const TUint KCountServMajorVersionNumber=0;
const TUint KCountServMinorVersionNumber=1;
const TUint KCountServBuildVersionNumber=1;

_LIT(KDummyServerName, "DummyServer");

class CDummyObserver : public CBase, public MMsvSessionObserver
	{
public:
	void HandleSessionEventL(TMsvSessionEvent, TAny*, TAny*, TAny*) {};
	};
enum TServReqst
    {
    EServGetTMsvEntry = 1	
	};
//**********************************
//CDummyServer
//**********************************

class CDummyServer : public CServer2
    {
public:
     static CDummyServer* NewLC();
	CDummyServer();
	virtual ~CDummyServer();
	void ConstructL();
    CSession2* NewSessionL(const TVersion& aVersion,const RMessage2& aMessage) const;
    void StopServer() ;//const;
    };


//**********************************
//CDummyServerSession
//**********************************
/**
This class represents a session with the server.
Functions are provided to respond appropriately to client messages.
*/
class CDummyServerSession : public CSession2
    {
public:
    CDummyServerSession();
    //service request
    void ServiceL(const RMessage2& aMessage);
    void DispatchMessageL(const RMessage2& aMessage);
	void GetTMsvEntryL(const RMessage2& aMessage);
    virtual ~CDummyServerSession();

    };