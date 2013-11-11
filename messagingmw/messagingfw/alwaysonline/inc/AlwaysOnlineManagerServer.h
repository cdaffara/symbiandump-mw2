/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: 
*     AlwaysOnline server. Governs always online plugins.
*
*/

#ifndef __ALWAYSONLINEMANAGERSERVER_H__
#define __ALWAYSONLINEMANAGERSERVER_H__

#include <e32base.h>
#include <msvapi.h>
#include "AlwaysOnlineManagerCommon.h"
#include "AlwaysOnlineManager.h"

//constants
const TInt KAlwaysOnlineMajorVersionNumber = 1;
const TInt KAlwaysOnlineMinorVersionNumber = 0;
const TInt KAlwaysOnlineVersionNumber = 0;

class TOAManagerStartupSignaller;

// Server's policy defitions
const TUint KAOMServerRangeCount = 6;

/* 
Available commands from alwaysonlinemanagerclient.h header file.

EServerAPIBaseCommand = 0,                  //common command IDs 0 - 999
EServerAPIBaseCommandStart,
EServerAPIBaseCommandStop,
EServerAPIEmailBaseCommand = 1000,          //email specific commands 1000 - 1999
EServerAPIEmailTurnOff,                     // 1001 switch off specified mailbox
EServerAPIEmailTurnOn,                      // 1002 switch on specified mailbox
EServerAPIEmailAgentRemove,                 // 1003 remove specified mailbox
EServerAPIEmailEMNReceived,                 // 1004 EMN received for specified mailbox
EServerAPIEmailUpdateMailWhileConnected,    // 1005 acts like imap connection except does not reconnect if connection dropped

EServerAPIPecBaseCommand = 2000,            //Pec specific commands 2000 - 2999

EServerAPIImBaseCommand = 3000              // IM specific commands 3000 - 3999
*/

const TInt KAOMServerRanges[KAOMServerRangeCount] = 
    {
    0, //range is 0-2 inclusive (EServerAPIBaseCommand, EServerAPIBaseCommandStart, EServerAPIBaseCommandStop)
    3, //range is 3-999 inclusive (Common commands not yet defined --> Unsupported)
    1000, //range is 1000-1005 inclusive (Email plugin commands)
    2000, //range is 2000-2999 inclusive (Pec plugin commands)
    3000, //range is 3000-3999 inclusive (IM plugin commands)
    4000, //range is 4000-KMaxTInt inclusive
    };

const TUint8 KAOMServerElementsIndex[KAOMServerRangeCount] = 
    {
    0,                              //applies to 0th range (req num: 0-2)
    CPolicyServer::ENotSupported,   //applies to 1st range (req num: 3-999)
    0,                              //applies to 2nd range (req num: 1000-1999)
    0,                              //applies to 2nd range (req num: 2000-2999)
    0,                              //applies to 2nd range (req num: 3000-3999)
    CPolicyServer::ENotSupported,   //applies to 4th range (req num: 4000-KMaxTInt)
    };
const CPolicyServer::TPolicyElement KAOMServerElements[] = 
    {
    {_INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData), CPolicyServer::EFailClient} //policy "0"; fail call if WriteDeviceData not present
    };
    
const CPolicyServer::TPolicy KAOMServerPolicy =
    {
    CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
    KAOMServerRangeCount,           
    KAOMServerRanges,
    KAOMServerElementsIndex,
    KAOMServerElements,
    };


/**
* CAlwaysOnlineManagerServer
* 
* Always online server class. Server which is started at bootup and will stay running forever
* in the background. Server controls "always online clients" ( ECom plugins )
* which contain the actual implementation of always online functionality. 
* Server's task is just to start things up and send messages to plugins allowing
* or restricting online operation. 
*/
class CAlwaysOnlineManagerServer : public CPolicyServer
    {
public:
	enum {EAlwaysOnlineManagerServerPriority=EPriorityBackground};

public:										
///////////////////////////////////////////////////////////////////////////////////////

	/**
	 * Start the server, returning an error if one occurs
	 */
	static TInt RunServer(TOAManagerStartupSignaller& aSignaller);

	/**
	 * Start the server, leaving if there's an error
	 */
	static void	RunServerL(TOAManagerStartupSignaller& aSignaller);

//#if defined (__WINS__)

	/**
	 * In WINS, the EPOCEXE target is a DLL with an entry point called WinsMain,
	 * taking no parameters and returning TInt. This is not really valid as a thread
	 * function which takes a TAny* parameter which we need.
	 * 
	 * So the DLL entry-point WinsMain() is used to return a TInt representing the
	 * real thread function within the DLL. This is good as long as
	 * sizeof(TInt)>=sizeof(TThreadFunction).
	 * 
	 */
	static TInt ServerThreadFunction(TAny* aParameters);

//#endif
    /*
    * PanicServer
    */
	static void PanicServer(TAlwaysOnlineManagerPanic aPanic);

    /*
    * ThreadFunction
    */
	IMPORT_C static TInt ThreadFunction(TAny* aStarted);
	
    /*
    * NewL constructor
    */
	IMPORT_C static CAlwaysOnlineManagerServer* NewL();

    /*
    * ConstructL 
    */
	void ConstructL();

    /*
    * ~CAlwaysOnlineManagerServer, destructor
    */
	~CAlwaysOnlineManagerServer();
	
    /**
    * HandleClientCommandL
    */
    IMPORT_C void HandleClientCommandL( TAlwaysOnlineServerAPICommands aCommand, 
    									TDes8& aParameter );

	/*
    * NewSessionL
    */
	virtual CSession2* NewSessionL( const TVersion& aVersion, 
                                    const RMessage2& aMessage ) const;


private:
    /*
    * CAlwaysOnlineManagerServer, c++ constructor
    */
	CAlwaysOnlineManagerServer( TInt aPriority );


	//data
private:
    CAlwaysOnlineManager* iManager;

    };


#endif
//EOF
