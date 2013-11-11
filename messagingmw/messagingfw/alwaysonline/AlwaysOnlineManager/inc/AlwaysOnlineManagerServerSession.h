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
* Description: AlwaysOnline server session
*
*/


#ifndef __ALWAYSONLINEMANAGERSERVERSESSION_H__
#define __ALWAYSONLINEMANAGERSERVERSESSION_H__

#include <e32base.h>
#include <msvapi.h>
#include <mtclreg.h>

//forward declarations
class CAlwaysOnlineManagerServer;


/**
* CAlwaysOnlineManagerServerSession
*
* Server side session class
*/
class CAlwaysOnlineManagerServerSession : public CSession2
    {
public:
	/**
    *
    **/
	static CAlwaysOnlineManagerServerSession* NewL( /*RThread &aClient,*/ CAlwaysOnlineManagerServer * aServer );

    /**
    *
    **/
	CAlwaysOnlineManagerServerSession( /*RThread& aClient*/ );

    /**
    *
    **/
	void ConstructL( CAlwaysOnlineManagerServer* aServer );

    /**
    *
    **/
    virtual ~CAlwaysOnlineManagerServerSession();
	
    
	/**
    *
    **/
	virtual void ServiceL( const RMessage2 &aMessage );

    /**
    *
    **/
	void DispatchMessageL( const RMessage2 &aMessage );

	/**
	* PanicClient
    * Panics client if something goes wrong
    * @since Series60 3.0
    * @param aMessage, panic message
    * @param aPanic, panic code
    **/
    void PanicClient( const RMessage2& aMessage, TInt aPanic ) const;

private:
	CAlwaysOnlineManagerServer *iAOMServer; // pointer to owning server

    };

#endif
//EOF
