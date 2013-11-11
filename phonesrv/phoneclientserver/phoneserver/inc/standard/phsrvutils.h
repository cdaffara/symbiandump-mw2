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
* Description:  Phone Server Utils.
*
*/


#ifndef PHSRVUTILS_H
#define PHSRVUTILS_H

// DATA TYPES

/* 
*   EPhoneServerPanicNoWaitingRequestsToDeque         No waiting requests.
*   EPhoneServerPanicModeLogicFault                   Logic failure.
*   EPhoneServerPanicCouldNotStartServer              Server startup failure.
*   EPhoneServerPanicSchedulerError                   Schedule failure.
*   EPhoneServerPanicNoInitiatingCallObject           Missing call object.
*   EPhoneServerPanicExistingCallInitiatorFault       Existing initiator fail.
*   EPhoneServerPanicNoCallInitiatorFault             Missing initiator failure.
*   EPhoneServerPanicInitiatorHandleMisMatchFault     Mismatch failure.
*   EPhoneServerPanicNoPendingSendRequestFault        Sending failure.
*   EPhoneServerPanicNoPendingTimeOutRequestFault     Timeout failure.
*   EPhoneServerPanicUssdAutoReplyTimerAlreadyActive  Timer already active.
*   EPhoneServerPanicInvalidInitiatorPriority         Initiator failure.
*   EPhoneServerPanicCannotOpenPrimaryVoiceLine       Primary line open failure.
*   EPhoneServerPanicCouldNotRenameServerThread       Renaming of thread failed.
*   EPhoneServerPanicLineTypeInvalid                  Invalid line type.
*   EPhoneServerPanicSharedDataConnectionFailure      Shared data failure.
*   EPhoneServerPanicSystemAgentConnectionFailure     System agent failure.
*   EPhoneServerPanicEmergencyCallRequestFailure      Emergency call failure.
*   EPhoneServerPanicExistingComHandInitiatorFault    Existing ComHand fail.
*   EPhoneServerPanicTooManyRequestsOngoing           More than one req/subses.
*   EPhoneServerPanicMissingComInitiator              Com initiator missing
*   EPhoneServerPanicMissingMessNegotiator            Messenger negotiator missing
*   EPhoneServerPanicMissingEmerNegotiator            Emergency call negotiator missing
*   
*   EPhoneServerPanicLast                             Keep as last value!
*/
enum TPhoneServerPanic
    {
    EPhoneServerPanicNoWaitingRequestsToDeque = 0,
    EPhoneServerPanicModeLogicFault = 1,
    EPhoneServerPanicCouldNotStartServer = 2,
    EPhoneServerPanicSchedulerError = 3,
    EPhoneServerPanicNoInitiatingCallObject = 4,
    EPhoneServerPanicExistingCallInitiatorFault = 5,
    EPhoneServerPanicNoCallInitiatorFault = 6,
    EPhoneServerPanicInitiatorHandleMisMatchFault = 7,
    EPhoneServerPanicNoPendingSendRequestFault = 8,
    EPhoneServerPanicNoPendingTimeOutRequestFault = 9,
    EPhoneServerPanicUssdAutoReplyTimerAlreadyActive = 10,
    EPhoneServerPanicInvalidInitiatorPriority = 11,
    EPhoneServerPanicCannotOpenPrimaryVoiceLine = 12,
    EPhoneServerPanicCouldNotRenameServerThread = 13,
    EPhoneServerPanicLineTypeInvalid = 14,
    EPhoneServerPanicSharedDataConnectionFailure = 15,
    EPhoneServerPanicSystemAgentConnectionFailure = 16,
    EPhoneServerPanicEmergencyCallRequestFailure = 17,
    EPhoneServerPanicExistingComHandInitiatorFault = 18,
    EPhoneServerPanicTooManyRequestsOngoing = 19,
    EPhoneServerPanicMissingComInitiator = 20,
    EPhoneServerPanicMissingMessNegotiator = 21,
    EPhoneServerPanicMissingEmerNegotiator = 22,

    EPhoneServerPanicLast   // Keep as last!
    };

// CLASS DECLARATION

/**
*  Phone Server utils class.
*
*  @since 1.0
*/
class PhoneServerUtils
    {
    public: // New functions

        /**
        * Panic function for Phone Server usage.
        * 
        * @param aPanic Panic code.
        */
        static void Panic( TPhoneServerPanic aPanic );


        /**
        * Panic the client using Phone Server.
        * @since 2.6
        * 
        * @param aPanic Panic code.
        * @param aMessage The client to be paniced.
        */
        static void PanicClient( 
            TPhoneServerPanic aPanic,
            const RMessage2& aMessage );
    };


#endif // PHSRVUTILS_H


// End of file
