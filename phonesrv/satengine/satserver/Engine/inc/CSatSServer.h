/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Server class
*
*/


#ifndef CSATSSERVER_H
#define CSATSSERVER_H

#include <e32base.h>
#include "TSatEventMediator.h"
#include "MSatSSessions.h"

class CSatCommandContainer;
class CSatSUiClientHandler;
class MSatUtils;

// ----------------------------------------------------------------------------------------
// Server's policy here
// ----------------------------------------------------------------------------------------

//Total number of ranges
const TUint KSatServerCount = 14;  // 13 + 0

//Definition of the ranges of IPC numbers
const TInt satServerRanges[KSatServerCount] =
        {
        // Illegal service request.
        0,  //ESatSNone,   //Range 0

        /*// Server test commands
        ESatSDisplayText,
        ESatSNotifyTest,
        ESatSNotifyTestCancel,
        ESatSShutdown,*/
        1,   // Range 1.

        /*These proactive commands need no capability checling
        Proactive commands (asynchronous)
        ESatSProactiveDisplayText,
        ESatSProactiveGetInkey,
        ESatSProactiveGetInput,
        ESatSProactiveSetUpMenu,
        ESatSProactiveSelectItem,
        ESatSProactivePlayTone,
        ESatSProactiveRefresh,*/
        5,  // Range 2.

        /* This range needs capability checking, NetworkServices.
        //ESatSProactiveSendSs,
        ESatSProactiveSendUssd,
        ESatSProactiveSendDtmf,
        ESatSProactiveQuery, // for Send SM and Set Up Call
        ESatSProactiveEvent, // for server event notification
        ESatSProactiveSetUpCall,*/
        12,  // Range 3.

        // Envelope commands (synchronous)
        // ESatSActiveMenuSelection,
        18,  // Range 4

        // CallControl command
        //ESatSCallControl,
        19, // Range 5


        //  MoSmControl command
        // ESatSMoSmControl,
        20,  // Range 6

        /* Command responses (synchronous)
        ESatSProactiveDisplayTextResponse,
        ESatSProactiveGetInkeyResponse,
        ESatSProactiveGetInputResponse,
        ESatSProactiveSetUpMenuResponse,
        ESatSProactiveSelectItemResponse,
        ESatSProactivePlayToneResponse,
        ESatSProactiveSendSsResponse,
        ESatSProactiveSendUssdResponse,
        ESatSProactiveSendDtmfResponse,
        ESatSProactiveQueryResponse,
        ESatSProactiveEventResponse,
        ESatSProactiveSetUpCallResponse,*/
        21, // Range 7

        // SAT UI related call control response
        //ESatSCallControlUIResponse,
        33,  // Range 8

        // ESatSProactiveRefreshResponse,
        34, // Range 9

        // SAT UI related response
        // ESatSMoSmControlUIResponse,
        35,  // Range 10

        /* Session indicators (for internal use)
        ESatSOpenUiSubSession,
        ESatSGetSIMAppStatus,
        ESatSCloseUiSubSession,*/

        36, // Range 11

        /* Refresh handling
        ESatSOpenRefreshSubSession,
        ESatSCloseRefreshSubSession,
        ESatSRefreshAllowed,
        ESatSNotifyAllowRefresh,
        ESatSNotityAllowRefreshCancel,
        ESatSNotifyRefresh,
        ESatSCancelNotifyRefresh,*/

        39, // Range 12

        /* Icon handling
        ESatSOpenIconSubSession,
        ESatSCloseIconSubSession,
        ESatSGetIconInfo,
        ESatSGetIconInstance,
        ESatSIconInstanceGot*/

        46, // Range 13

        };

//Policy to implement for each of the above ranges
const TUint8 satServerElementsIndex[KSatServerCount] =
        {
        CPolicyServer::EAlwaysPass, // 0
        CPolicyServer::EAlwaysPass, // 1
        CPolicyServer::EAlwaysPass, // 2
        0,   // Check Network Capabilites for this range
        CPolicyServer::EAlwaysPass, // 4
        CPolicyServer::EAlwaysPass, // 5
        CPolicyServer::EAlwaysPass, // 6
        CPolicyServer::EAlwaysPass, // 7
        CPolicyServer::EAlwaysPass, // 8
        CPolicyServer::EAlwaysPass, // 9
        CPolicyServer::EAlwaysPass, // 10
        CPolicyServer::EAlwaysPass, // 11
        CPolicyServer::EAlwaysPass, // 12
        CPolicyServer::EAlwaysPass  // 13
        };

//Specific capability checks
const CPolicyServer::TPolicyElement satServerElements[] =
    {
    // policy "0"; fail call if NetworkServices not present.
    //lint -e{778,1924} Macro can not be affected.
        {
        _INIT_SECURITY_POLICY_C1( ECapabilityNetworkServices ),
        CPolicyServer::EFailClient
        },
    };

//Package all the above together into a policy
const CPolicyServer::TPolicy satServerPolicy =
        {
        CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
        KSatServerCount, //number of ranges
        satServerRanges,    //ranges array
        satServerElementsIndex, //elements<->ranges index
        satServerElements,      //array of elements
        };

// End Server's Policy

/**
*  Manages sessions
*
*  @lib SatServer.exe
*  @since Series 60 3.0
*/
class CSatSServer : public CPolicyServer,
                    public MSatSSessions
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CSatSServer* NewL();

        /**
        * Destructor.
        */
        virtual ~CSatSServer();

    public: // New functions

        /**
        * Completes current message from client.
        * @param aError Standard Symbian error code.
        */
        void CompleteMessage( TInt aError ) const;

        /**
        * Creates new container for sessions.
        * @return New container.
        */
        CObjectCon* CreateNewContainerL();

        /**
        * Gets MSatUtils interface
        * @return MSatUtils interface
        */
        MSatUtils* SatUtils();

        /**
        * Adds refresh subsession.
        * @param aSubSession refresh subsession.
        */
        void AddL( const MSatRefreshSubSession* aSubSession );

        /**
        * Removes refresh subsession.
        * @param aSubSession Sub session to be removed.
        */
        void Remove( const MSatRefreshSubSession* aSubSession );

    public: // Functions from base classes

        /**
        * From MSatSSessions Gives ui session interface.
        * @return Interface for ui session
        */
        MSatUiSession* UiSession();

        /**
        * From MSatSSessions Gives refresh subsessions.
        * @return Pointer array containing refresh subsessions
        */
        const RPointerArray<MSatRefreshSubSession>& RefreshSubSessions();

    private: // Functions from base classes

        /**
        * From CServer2, creates a new server session.
        * @param aVersion Client interface version.
        * @param aMessage 'connect' message from the client.
        * @return Pointer to the created server session.
        */
        CSession2* NewSessionL(
            const TVersion& aVersion,
            const RMessage2& aMessage ) const;

    private:

        /**
        * C++ default constructor.
        */
        CSatSServer();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Data

        // SAT Command handler container
        CSatCommandContainer* iCmdContainer;

        // SAT Ui client handler
        CSatSUiClientHandler* iUiHandler;

        // Event mediator
        TSatEventMediator iEventMediator;

        // CObjectCon factory.
        CObjectConIx* iObjectConIx;

        // Refresh subsessions
        RPointerArray<MSatRefreshSubSession> iRefreshSubSessions;

    };

#endif      // CSATSSERVER_H
