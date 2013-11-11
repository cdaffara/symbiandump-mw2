/*
* Copyright (c) 2002-2010 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Handles all the Ussd sessions.
*
*/

#ifndef CPHSRVUSSDMANAGER_H
#define CPHSRVUSSDMANAGER_H


// INCLUDES
#include <etelmm.h>            // ETel
#include "phcltclientserver.h" 
#include "mphsrvussdnetworkobserver.h" 
#include "mphsrvussdreplytimerobserver.h" 
#include <badesca.h>
#include <hbdevicemessageboxsymbian.h>

// FORWARD DECLARATIONS
class RFs;
class CPhSrvResourceManager;
class CPhSrvUssdSendHandler;
class CPhSrvUssdReceiveHandler;
class CPhSrvUssdReplyTimer;
class MPhSrvPhoneInterface;
class MPhSrvUssdMessageSentObserver;
class CPhSrvUssdSessionCancelWaiter;
class CPhSrvUssdNotifyNWRelease;

// CLASS DECLARATION

/**
*  Handles all the Ussd sessions.
*
*  @since 1.0
*/
class CPhSrvUssdManager : 
    public CActive, 
    public MPhSrvUssdNetworkObserver, 
    public MPhSrvUssdReplyTimerObserver,
    public MHbDeviceMessageBoxObserver
    {
    public:  // Constructors and destructor

        /**
        * C++ constructor.
        *
        * @param aFsSession The file session.
        * @param aResourceManager The resource manager.
        */
        CPhSrvUssdManager( 
            RFs& aFsSession, 
            CPhSrvResourceManager& aResourceManager
         );

        /**
        * C++ destructor.
        */
        ~CPhSrvUssdManager();
        
        /**
        * Symbian OS second phase constructor
        *
        * @param aPhoneInterface The phone interfaces.
        */
        void ConstructL( MPhSrvPhoneInterface& aPhoneInterface );
        
    private:
        /*
         * TDialogInfoType is used to identify dialogs with different
         * buttons.
         */
        enum TDialogInfoType{
                EInvalidType = -1,
                EYesNo,
                ENextExit,
                EReplyExit,
                EOnlyExit
            };
        
    public: // New functions
        
        /**
        * Send the USSD message.
        * 
        * @param aMsgData The USSD message to be sent.
        * @param aMsgAttribute The USSD message attributes.
        * @param aObserver Observer for sent event.
        */
        void SendUssdL( 
            const TDesC8& aMsgData, 
            RMobileUssdMessaging::TMobileUssdAttributesV1& aMsgAttribute, 
            MPhSrvUssdMessageSentObserver& aObserver );

        /**
        * Cancel sending the USSD.
        */
        void SendUssdCancel();

        
        /**
        * These are called when status of Ussd application changes.
        * 
        * @return 0 if editor should not be cleared.
        */
        void InformUssdApplicationStarting();
        void InformUssdApplicationTerminatingL( const RMessage2& aMessage );
        TInt InformUssdApplicationToForeground();      
        void InformUssdApplicationToBackground();
        
        /**
        * Starts the editor.
        */
        void RequestStartEditingL();

        /**
        * SAT starts.
        *
        * @param aSatMessage Message to be comleted when session ends.
        */
        void InformStartSAT( const RMessage2& aSatMessage );

        /**
        * SAT ends.
        */
        void InformStopSAT();
        
        /**
        * Completes SAT request if it's ongoing.
        *
        * @param aReceiveString If session end with a string it is passed
        *        as a paramter.
        * @param aError Completion code.
        * @return The error code of the SAT request.
        */
         void CompleteSatL( 
            TDesC* aReceiveString, 
            TInt aError );
        
    protected: // From base classes
        
        /**
        * @see MPhSrvUssdNetworkObserver
        */
        void UssdNetworkObserverHandleSendEventL( TInt aError );
        
        /**
        * @see MPhSrvUssdNetworkObserver
        */
        void UssdNetworkObserverHandleReceivedEventL(
            const TDes8& aMsgData, 
            const RMobileUssdMessaging::TMobileUssdAttributesV1& 
                  aMsgAttributes,
            TInt aError );

        void UssdNetworkObserverHandleNotifyNWReleaseL( 
        const RMobilePhone::TMobilePhoneSendSSRequestV3 & aReturnResult, 
        TInt aError );
        
        /**
        * @see MPhSrvUssdReplyTimerObserver
        */
        void UssdReplyTimerObserverHandleExpiredL( TInt aError );
        
        /**
        * @see MHbDeviceMessageBoxObserver
        */        
        void MessageBoxClosed(const CHbDeviceMessageBoxSymbian* aMessageBox,
            CHbDeviceMessageBoxSymbian::TButtonId aButton);
        
        /*
        * @see CActive
        */
        void RunL();
        
        /*
        * @see CActive
        */
        void DoCancel();
        
        /*
        * @see CActive
        */
        TInt RunError( TInt aError );

    private: // New functions
        
        CPhSrvUssdSendHandler& SendHandlerL();

        void ShowErrorNoteL( TInt aError );

        void DecodeL( 
            const TDesC8& aSrc, 
            TDes& aDes, 
            TUint8 aDcs );
       
        void FindFirstCarriageReturnL( 
            const TDesC8& aBuffer ,
            TUint& aSkipChars , 
            TUint& aStartBit );
        
        /** Display the "Done" note */
        void ShowDoneNoteL();
        
        /** Check is the ussd app running 
         * @return app running or not
         */
        TBool UssdAppTaskExistsL();
        
        // If the notification array is empty, close the session.
        void TryCloseSession();
        
        // Close the session; clear state etc.
        void CloseSession();
        
        /** Second stage handler for received messages 
         */
        void UssdHandleReceivedEventL(
            const TDes8& aMsgData, 
            const RMobileUssdMessaging::TMobileUssdAttributesV1& 
                  aMsgAttributes);
        // Restart the reply timer
        void RestartReplyTimerL();
        
        /** Is the reply timer up and running? */
        TBool NetworkWaitingForAnAnswer();
        
        // Set timer and activate it if there are notifications available
        void SetActiveIfPendingNotificationsExist();
        
        // Launch the global message query (used from RunL)
        void LaunchGlobalMessageQueryL();
        
        // Send an MO ACK message if any such are pending
        void ProcessMoAcksL();
        
        // Trigger an asynchronous call to call ProcessMoAcksL later
        void AsyncProcessMoAcks();
        
        // The static callback function for AsyncProcessMoAcks
        static TInt MoAckCallback(TAny *);
        
        /**
        * Return boolean value that states whether or not 
        * indicated feature is supported or not.
        * 
        * @since 2.1
        * @param aFeatureId The feature that is inspected 
        *        (see values from Telephony_Variant.hrh)
        * @return ETrue if the feature is supported, EFalse otherwise.
        */
        TBool IsTelephonyFeatureSupported( const TInt aFeatureId );

        /**
        * Play USSD tone.
        * 
        * @since 2.1
        * @return KErrNone if successful, otherwise Symbian error code.
        */
        TInt PlayUssdTone();

        /**
        * Returns telephony variant read-only data.
        * 
        * @since 2.1
        * @return Error code.
        */
        TInt GetTelephonyVariantData();

        /**
        * Send the MO acknowledgement.
        * 
        * @since 2.6
        */
        void SendMoAcknowledgementL();
        
        /**
        * Return boolean value that states whether notes are shown or not
        * 
        * @since 3.0
        */
        TBool ShowNotesL();
        
        /**
        * Check notify message array
        * 
        * @since 3.1
        */
        void CheckArray( );
        
        /**
        * Clears notify message array
        * 
        * @since 3.1
        */
        void ClearArrayL();
        
        /**
        * Returns notify message count
        * 
        * @since 3.1
        */
        TInt NotifyCount();
        
        
        /**
        * Updates Notify Message
        * 
        * @since 3.1
        */
        void UpdateNotifyMessageL();
        
        /**
        * Turn lights on
        * 
        * @since 3.1
        */
        void TurnLightsOn();
        
        /**
        * Load default string by QT style localization
        * @param aText default string id defined by _LIT
        */        
        const TPtrC LoadDefaultString( const TDesC& aText );

    private:     // Data
        
        // The file session reference.
        RFs& iFsSession;
        
        // The resource manager reference.
        CPhSrvResourceManager& iResourceManager;

        // The USSD functionality in lower level (TSY).
        RMobileUssdMessaging iMobileUssdMessaging;
        
        // The observer waiting message sent notification.
        MPhSrvUssdMessageSentObserver* iObserver;

        // The send operation handler.
        CPhSrvUssdSendHandler* iUssdSendHandler;

        // The receive operation handler.
        CPhSrvUssdReceiveHandler* iUssdReceiveHandler;
        
        // The network release notifier.
         CPhSrvUssdNotifyNWRelease* iUssdNotifyNWRelease;
        
        // The reply timer.
        CPhSrvUssdReplyTimer* iUssdReplyTimer;
        
        // The buffer for received message.
        TBuf< KPhCltUssdMax8BitCharacters > iReceivedMessage;
        
        // The message query for showing USSD operation queries.
        CHbDeviceMessageBoxSymbian* iDeviceDialog;
       
        // Is editor emptied.
        TBool iEmptyEditor;
        
        // Will the editor be started.
        TBool iStartEditor;
        
        // The timer.
        RTimer iTimer;

        // Is there pending SAT message.
        TBool iHavePendingSatMessagePointer;

        // The pending SAT message.
        RMessage2 iPendingSatMessagePointer;

        // Reference to phone interfaces.
        MPhSrvPhoneInterface* iPhoneInterface;

        // The used DCS in the SAT message
        TUint iDCS;

        // Softkeys in Global MessageQuery.
        TInt iSoftkeys;

        // Global MessageQuery launching indicator.
        TBool iLaunchGMQ;

        // Local Telephony variant read-only data.
        TInt iVariantReadOnlyValues;

        // The buffer for received decoded message.
        TBuf< KPhCltUssdMax8BitCharacters > iDecodedMessage;

        // If ETrue, then the last received USSD message was EUssdMtNotify.
        TBool iNotifyMessage;
        
        //NotifyNetWorkRelease return result
        RMobilePhone::TMobilePhoneSendSSRequestV3 iReturnResult;
        
        //NotifyNetWorkRelease return result
        RMobilePhone::TMobilePhoneSendSSRequestV3Pckg iReturnResultPckg;
        
        // Array for notify messages
        CDesCArray* iNotifyArray;
        
        //Sending Release status
        TBool iSendRelease;

        // Notify array reset status
        TBool iClearArray;

        // Sat cancel status
        TBool iSatCanceled;
        
        // Show Done note
        TBool iShowDone;

        // Checks if received message type is Reply
        TBool iMsgTypeReply;
        
        // Has the transaction been closed or not
        TBool iNetworkReleased;
        
        // Is the current send operation an MO ACK message or not 
        TBool iSendingAck;
        
        // Number of MO ACK messages pending to be sent. Normally max 2.
        TInt iAcksToBeSent;
        
        // An asynchronous callback for sending MO ACK messages
        CAsyncCallBack* iMoAckCallback;
        
        HBufC* iTextBuffer;

        // The dialogs with different button.
        TDialogInfoType iDialogType;

    };
    
#endif // CPHSRVUSSDMANAGER_H


// End of File
