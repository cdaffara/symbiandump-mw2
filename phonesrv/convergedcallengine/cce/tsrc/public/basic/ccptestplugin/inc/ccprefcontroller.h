/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  CCP test plug-in implementation.
*
*/


#ifndef CCPREFCONTROLLER_H
#define CCPREFCONTROLLER_H

//  INCLUDES
#include <e32base.h>
#include <e32property.h>	// RProperty
#include <mccpobserver.h>
#include "ccprefconvergedcallprovider.h"
#include <mccpcallobserver.h>
// Forward declarations

class CCPRefConvergedCallProvider;
class CCPRefCall;

/**
*
*/
class CCPRefController : public CActive
	{
	
	enum EPluginCommand
		{
		EPluginCmdCreateCall = 1,
		EPluginCmdHangupCall = 2,
		EPluginCmdTransferRequest = 3,
		EPluginCmdRemoteHold = 4,
		EPluginCmdRemoteResume = 5,
		EPluginCmdReportCallCount = 6,
		EPluginCmdSendTestEvents = 7,
		EPluginCmdErrorOccurred = 8,
		EPluginCmdSetAsyncMode = 9,
		EPluginCmdSetState = 10,
		EPluginCmdCancel = 11,
		EPluginInitiatedCSMoCall = 12,
		EPluginCmdCreateConferenceCall = 13
		};
	typedef EPluginCommand TPluginCommand;

public:	 //functions
		
	static CCPRefController* NewL( CCPRefConvergedCallProvider& aProvider,
        TUid aUid,
        CCPCall::TCallType aCallType,
        TInt aRequestValue );
	
	~CCPRefController();
	
private: //functions
	
	// Constructor and destructor
	CCPRefController( 
	    CCPRefConvergedCallProvider& aProvider,
        TUid aUid,
        CCPCall::TCallType aCallType,
        TInt aRequestValue );
    
	void ConstructL();
	
	// Active Object functions
	 
	void DoCancel();
	
	void RunL();
	
	void Delay( TUint aDelayInMilliseconds );
	
	TInt GetValue(TPtrC& aInput);
	
	TPluginCommand SetPluginCommand( const TDesC& aPluginCommand );
	
	static void DelayedHangUp( CCPRefCall* aCall, TInt aDelayInHundredths );
	
	static TInt DoHangUp( TAny* anyPtr );
	
	void PluginInitiatedCSMoCall( TInt aServiceId );
    void PluginInitiatedConferenceCall();
	void SendTestEvents1();
	void SendTestEvents2(TInt aServiceId);
	
	void SetPluginState( TInt aCallID, const MCCPCallObserver::TCCPCallState aState );
	
private: // Member data

    CActiveScheduler* iScheduler;
    
    TUid iPluginUid;
    
    CCPRefConvergedCallProvider& iProvider;
    
    CCPCall::TCallType iCallType;
    
    TInt iRequestValue;
    
    RProperty iProperty;
    
    HBufC* iPluginCommand;
    
    };

#endif CCPREFCONTROLLER_H
