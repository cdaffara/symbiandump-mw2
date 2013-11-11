/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Unit test replacement class for RPhCltUssd
*
*/


//
// This implementation glues USSD client directly to
// USSD Manager class so that the client/server mechanism is
// not used. This is practical in unit test environment.
//

// INCLUDE FILES
#include <e32base.h>
#include "mphsrvussdmessagesentobserver.h"
#include "ut_testenv.h"
#include "cphsrvussdmanager.h"
#include "rphcltussd.h"


// ========================================================================
// GLOBALS
// ========================================================================

class TUssdGlueAdapter: public MPhSrvUssdMessageSentObserver
    {
    public:
		void writeAttributes( const TDesC8& aAttributes )
			{
			RMobileUssdMessaging::TMobileUssdAttributesV1Pckg pAttributes(
					iUssdAttributes );
			pAttributes.Copy( aAttributes );
			}
        void Listen( TRequestStatus& aReqStatus )
            {
            aReqStatus = KRequestPending;
            iPendingStatus = &aReqStatus;
            }
        void UssdMessageSentObserverHandleResult( TInt aError )
            {
            // this causes the pending active object to signalled and run
            User::RequestComplete( iPendingStatus, aError );
            }
        
        RMobileUssdMessaging::TMobileUssdAttributesV1 iUssdAttributes;
        TRequestStatus* iPendingStatus;
    };

TUssdGlueAdapter glueAdapter;



// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
RPhCltUssd::RPhCltUssd()
    {
    }

// -----------------------------------------------------------------------------
TInt RPhCltUssd::Open( RPhCltServer& /*aServer*/ )
    {
    return KErrNone;
    glueAdapter.iPendingStatus = 0;
    }


// -----------------------------------------------------------------------------
void RPhCltUssd::Close()
    {    
    }


// -----------------------------------------------------------------------------
TInt RPhCltUssd::StartUssdEditor() const
    {
    TRAPD( leave,
        TestEnv::env().ussdManager().RequestStartEditingL() );
    return leave;
    }


// -----------------------------------------------------------------------------
TInt RPhCltUssd::AppStarting()
    {
    TestEnv::env().ussdManager().InformUssdApplicationStarting();
    return KErrNone;
    }


// -----------------------------------------------------------------------------
TInt RPhCltUssd::AppTerminating( 
    TPhCltUssdAppExitReason aExitReason )
    {
    RMessage2& fakeMessage = TestEnv::env().fakeMessage();

    TRAPD( leave,
       TestEnv::env().ussdManager().InformUssdApplicationTerminatingL( fakeMessage ) );

    return leave;
    }

// -----------------------------------------------------------------------------
TBool RPhCltUssd::AppToForeground()
    {
    return TestEnv::env().ussdManager().InformUssdApplicationToForeground();
    }

// -----------------------------------------------------------------------------
TInt RPhCltUssd::AppToBackground()
    {
    TestEnv::env().ussdManager().InformUssdApplicationToBackground();
    return KErrNone;
    }

// -----------------------------------------------------------------------------
void RPhCltUssd::SendUssd(
    TRequestStatus& aReqStatus, 
    const TDesC8& aMsgData, 
    const TDesC8& aMsgAttribute )
    {
	glueAdapter.Listen( aReqStatus );
	glueAdapter.writeAttributes( aMsgAttribute );

    TRAPD( leave, TestEnv::env().ussdManager().SendUssdL(
        aMsgData, glueAdapter.iUssdAttributes, glueAdapter ) );
    if ( leave != KErrNone )
        {
		glueAdapter.UssdMessageSentObserverHandleResult( leave );
        }
    }

// -----------------------------------------------------------------------------
void RPhCltUssd::CancelSend()
    {
    TestEnv::env().ussdManager().SendUssdCancel();
    }


// -----------------------------------------------------------------------------
void RPhCltUssd::StartSAT( 
    TRequestStatus& aStatus , 
    TDes& /*aReceiveMessage*/,
    TPckg< TUint >& /*aShowNotesAndDcs*/ )
    {
    RMessage2& fakeMessage = TestEnv::env().fakeMessage();

	TestEnv::env().ussdManager().InformStartSAT( fakeMessage );

    TRequestStatus* reqStatus = &aStatus;
    User::RequestComplete( reqStatus, KErrNone );
    }


// -----------------------------------------------------------------------------
void RPhCltUssd::StopSAT()
    {
	TestEnv::env().ussdManager().InformStopSAT();
    }


// End of File
