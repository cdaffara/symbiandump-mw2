/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  The observer interface for the Sat Client. Register a
*                concrete impl of this to receive SAT events from the session
*                object.
*
*/


#ifndef MSATUIOBSERVER_H
#define MSATUIOBSERVER_H

#include <e32base.h>
#include <e32svr.h>
#include <badesca.h>    // for descriptor arrays
#include <etelsat.h>

/**
 * The UI response codes, enumerations values are from ETSI 11.14 specification.
 */
enum TSatUiResponse
    {
    ESatFailure = -1,                       // OOM or other error
    ESatSuccess = 0x00,                     // success
    ESatSuccessToneNotPlayed = 0x09,        // Warning and game tones are
                                            // disabled from current profile
    ESatSessionTerminatedByUser = 0x10,     // user exited the app
    ESatBackwardModeRequestedByUser = 0x11, // user pressed back
    ESatNoResponseFromUser = 0x12,          // no response from user
    EHelpRequestedByUser = 0x13,            // help request from the UI
    EPCmdNotAcceptedByUser = 0x22,
    ESatCmdDataNotUnderstood = 0x32
    };

/**
 * Tone values are dictated by ETSI 11.14.
 * Use by the SAT Play Tone command.
 */

enum TSatTone
    {
    ESatToneNotSet                          = -1,
    ESatDialTone                            = 0x01,
    ESatCalledSubscriberBusy                = 0x02,
    ESatCongestion                          = 0x03,
    ESatRadioPathAcknowledge                = 0x04,
    ESatRadioPathNotAvailableCallDropped    = 0x05,
    ESatErrorSpecialInfo                    = 0x06,
    ESatCallWaitingTone                     = 0x07,
    ESatRingingTone                         = 0x08,
    ESatGeneralBeep                         = 0x10,
    ESatPositiveTone                        = 0x11,
    ESatNegativeTone                        = 0x12,
    ESatUserSelectedToneIncomingSpeech      = 0x13,
    ESatUserSelectedToneIncomingSms         = 0x14
    };

enum TSatAlphaIdStatus
    {
    ESatAlphaIdNotNull,
    ESatAlphaIdNull,
    ESatAlphaIdNotProvided
    };

/**
 * ControlResult can be used e.g. to supply
 * Call Control / MO Short Message Control result
 * from SIM to UI Client
 */

enum TSatControlResult
    {
    ESatAllowedNoModification,
    ESatNotAllowed,
    ESatAllowedWithModifications
    };

/**
 * The SAT next action indicators, localized by the UI.
 * Values commented out will not be used, with accordance to ETSI 11.14.
 */

enum TSatAction
    {
    //ESatNoAction = 0x00,
    //ESatRefreshAction = 0x01,
    //ESatMoreTimeAction = 0x02,
    //ESatPollIntervalAction = 0x03,
    //ESatPollingOffAction = 0x04,
    //ESatSetUpEventListAction = 0x05,
    ESatSetUpCallAction = 0x10,
    ESatSendSsAction = 0x11,
    ESatSendUssdAction = 0x12,
    ESatSendSmAction = 0x13,
    //ESatSendDtmfAction = 0x14,
    ESatLaunchBrowserAction = 0x15,
    ESatPlayToneAction = 0x20,
    ESatDisplayTextAction = 0x21,
    ESatGetInkeyAction = 0x22,
    ESatGetInputAction = 0x23,
    ESatSelectItemAction = 0x24,
    ESatSetUpMenuAction = 0x25,
    //ESatProvideLocalInformationAction = 0x26,
    //ESatTimerManagementAction = 0x27,
    ESatSetUpIdleModeTextAction = 0x28,
    ESatPerformCardApduAction = 0x30,
    ESatPowerOnCardAction = 0x31,
    ESatPowerOffCardAction = 0x32,
    ESatGetReaderStatusAction = 0x33,
    //ESatRunAtCommandAction = 0x34,
    ESatEndOfSimSession = 0x81
    };

/**
 * The SAT input commands' data formats.
 */
enum TSatCharacterSet
    {
    ESatDigitOnly,
    ESatCharSmsDefaultAlphabet,
    ESatCharUcs2Alphabet,
    ESatYesNo
    };

/**
 * Dictates how the icons should be used.
 */
enum TSatIconQualifier
    {
    ESatIconQualifierNotSet,// Icon qualifier is not set
    ESatENoIconId,          // Icon qualifier not present
    ESatSelfExplanatory,    // Icon qualifier is self explanatory
                            // (to display instead of the alpha id or text
                            // string)
    ESatNotSelfExplanatory  // Icon qualifier is not self explanatory
                            // (to display along the alpha id or text string)
    };

/**
 * Identifies the quering command
 */
enum TSatSQueryCommand
    {
    ESatSSendSmQuery,
    ESatSSendSsQuery,
    ESatSSendUssdQuery,
    ESatSRefreshQuery,
    ESatSLaunchBrowserQuery,
    ESatOpenChannelQuery,
    ESatSSetUpCallQuery
    };

/**
 * Identifies the notification command
 */
enum TSatSNotifyCommand
    {
    ESatSSendDataNotify,
    ESatSReceiveDataNotify,
    ESatSCloseChannelNotify,
    ESatSMoSmControlNotify,
    ESatSCallControlNotify,
    ESatSSendSsNotify,
    ESatSSendUssdNotify,
    ESatSSendDtmfNotify,
    ESatSSendSmsNotify
    };

/**
 * The icon id struct.
 */
struct TSatIconId
    {
    TUint8 iIdentifier;     // Identifies the EF_IMG in SIM.
    TSatIconQualifier iIconQualifier;
    };

/**
 * Specifies whether soft keys is the preferred selection mode or not.
 */
enum TSatSelectionPreference
    {
    ESatSelectionPreferenceNotSet,
    ESatNoSelectionPreference,
    ESatSoftKeyPreferred
    };

enum TSatBIPCommandIdentifier
    {
    ESendDataIdentier,
    EReceiveDataIdentifier,
    ECloseChannelIdentifier
    };

enum TSatSessionTerminationType
    {
    ESessionCancel,
    EEndKeyUsed
    };

// Codes for event notifications.
enum TSatSEvent
    {
    ESatSRefreshStartEvent,
    ESatSRefreshEndEvent,
    ESatSSmEndEvent,
    ESatSClearScreenEvent,
    ESatSCloseSatUiAppEvent,
    ESatSsEndEvent,
    ESatSsErrorEvent,
    ESatSDtmfEndEvent
    };

enum TSatSEventStatus
    {
    ESatEventNone,
    ESatEventCompleteOk,
    ESatEventFailure,
    ESatEventCancel
    };

/**
 *  An abstract observer interface.
 *  Register to receive SAT events from the session object.
 *
 *  @lib SatClient.lib
 *  @since Series 60 2.0
 */
class MSatUiObserver
    {
    public: // New functions

        /**
         * Notification of the SAT Display Text command.
         * @param aText The text string to be displayed.
         * @param aSimApplicationName  Sim Application name
         * @param aIconId The id of icon.
         * @param aRequestedIconDisplayed Informs if icon is not used.
         * @param aSustainedText Indicates is this text sustained ie. no timers
         *        used if ETrue.
         * @param aDuration Duration for showing the text
         * @param aWaitUserToClear Indication if user is needed to clear
         *        message
         * @return The response of the UI to this command.
         */
        virtual TSatUiResponse DisplayTextL(
            const TDesC& aText,
            const TDesC& aSimApplicationName,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed,
            const TBool aSustainedText,
            const TTimeIntervalSeconds aDuration,
            const TBool aWaitUserToClear ) = 0;

        /**
         * Notification of the SAT Get Inkey command.
         * @param aText The query text.
         * @param aCharacterSet The character range allowed.
         * @param aInput The input character.
         * @param aHelpIsAvailable indicates if help can be requested.
         * @param aIconId The id of icon.
         * @param aRequestedIconDisplayed Informs if icon is not used.
         * @param aDuration Duration for showing the dialog.
         * @param aImmediateDigitResponse Indication if Immediate digit response
         *        is needed.
         * @return The response of the UI to this command.
         */
        virtual TSatUiResponse GetInkeyL(
            const TDesC& aText,
            const TSatCharacterSet aCharacterSet,
            TChar& aInput,
            const TBool aHelpIsAvailable,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed,
            TUint& aDuration,
            const TBool aImmediateDigitResponse ) = 0;

        /**
         * Notification of the SAT Get Input command.
         * @param aText The query text.
         * @param aCharacterSet The character range allowed.
         * @param aInput The input string.
         * @param aMaxLength The maximum length of the input allowed.
         * @param aMinLength The minimum length of the input allowed.
         * @param aHideInput A flag indicating if the input should be hidden.
         * @param aHelpIsAvailable indicates if help can be requested.
         * @param aIconId The id of icon.
         * @param aRequestedIconDisplayed Informs if icon is not used.
         * @return The response of the UI to this command.
         */
        virtual TSatUiResponse GetInputL(
            const TDesC& aText,
            const TSatCharacterSet aCharacterSet,
            TDes& aInput,
            const TInt aMinLength,
            const TInt aMaxLength,
            const TBool aHideInput,
            const TBool aHelpIsAvailable,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed ) = 0;

        /**
         * Notification of the SAT Set Up Menu command.
         * @param aText The query text.
         * @param aMenuItems The array of menu item captions.
         * @param aMenuItemNextActions The array of menu item next action
         * indicator captions.
         * @param aHelpIsAvailable indicates if help can be requested.
         * @param aIconId The id of icon.
         * @param aMenuIcons List of icon identifiers for menu items.
         * @param aIconListQualifier Indicates how to use icons in the icon
         * list.
         * @param aSelectionPreference Specifies is the soft keys preferred.
         * @return The response of the UI to this command.
         */
        virtual TSatUiResponse SetUpMenuL(
            const TDesC& aText,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TBool aHelpIsAvailable,
            const TSatIconId& aIconId,
            const CArrayFixFlat<TInt>* aMenuIcons,
            const enum TSatIconQualifier aIconListQualifier,
            const enum TSatSelectionPreference aSelectionPreference ) = 0;

        /**
         * Notification of the SAT Select Item command.
         * @param aText The query text.
         * @param aMenuItems The array of menu item captions.
         * @param aMenuItemNextActions The array of menu item next action
         * indicator codes.
         * @param aDefaultItem The item selected by default.
         * @param aSelection The item index selected by the user.
         * @param aHelpIsAvailable indicates if help can be requested.
         * @param aIconId The id of title icon.
         * @param aMenuIcons List of icon identifiers for menu items.
         * @param aIconListQualifier Indicates how to use icons in the icon
         * list.
         * @param aRequestedIconDisplayed Informs if icon is not used.
         * @param aSelectionPreference Specifies is the soft keys preferred.
         * @return The response of the UI to this command.
         */
        virtual TSatUiResponse SelectItemL(
            const TDesC& aText,
            const MDesCArray& aMenuItems,
            const CArrayFixFlat<TSatAction>* aMenuItemNextActions,
            const TInt aDefaultItem, TUint8& aSelection,
            const TBool aHelpIsAvailable,
            const TSatIconId& aIconId,
            const CArrayFixFlat<TInt>* aMenuIcons,
            const enum TSatIconQualifier aIconListQualifier,
            TBool& aRequestedIconDisplayed,
            const enum TSatSelectionPreference aSelectionPreference ) = 0;

        /**
         * Notification of the SAT Play Tone command.
         * @param aText The text to be displayed.
         * @param aTone The tone to be played.
         * @param aDuration The duration of the tone to be played.
         * @param aIconId The id of icon.
         * @param aRequestedIconDisplayed Informs if icon is not used.
         * @return The response of the UI to this command.
         */
        virtual TSatUiResponse PlayTone(
            const TDesC& aText,
            const TSatTone aTone,
            const TTimeIntervalMicroSeconds aDuration,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed ) = 0;


        /**
         * General confirmation request
         * @param aCommandId ID of the quering command
         * @param aAlphaIdStatus Alpha Identifier status
         * @param aText The text to be displayed.
         * @param aAdditionalText Additional text to be used in queries.
         * @param aActionAccepted Indicates whether the command was accepted.
         * @param aIconId The id of icon.
         * @param aRequestedIconDisplayed Informs if icon is not used.
         * @param aTerminatedByUser Informs if end key is used.
         */
        virtual void ConfirmCommand(
            const TSatSQueryCommand aCommandId,
            const TSatAlphaIdStatus aAlphaIdStatus,
            const TDesC& aText,
            const TDesC& aAdditionalText,
            TBool& aActionAccepted,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed,
            TBool& aTerminatedByUser ) = 0;

        /**
         * General notification
         * @param aCommandId ID of the notifying command
         * @param aAlphaIdStatus Alpha Identifier status
         * @param aText Alpha Identifier
         * @param aIconId The id of icon.
         * @param aRequestedIconDisplayed Informs if icon is not used.
         * @param aControlResult Control result of the MoSm and CallControl
         * @return The response of the UI to this command.
         */
        virtual TSatUiResponse Notification(
            const TSatSNotifyCommand aCommandId,
            const TSatAlphaIdStatus aAlphaIdStatus,
            const TDesC& aText,
            const TSatIconId& aIconId,
            TBool& aRequestedIconDisplayed,
            const TSatControlResult aControlResult ) = 0;

        /**
         * General event notification. Used for example to tell UI that command
         * has completed its execution.
         * @param aEventId, identifies the event
         * @param aEventStatus, status of the event, used as additional info for
         *        the event
         * @param aError, possible error code that may affect on event handling.
         *        This is also used as additional info
         */
        virtual void EventNotification(
            const TSatSEvent aEventId,
            const TSatSEventStatus aEventStatus,
            const TInt aError ) = 0;
    };

#endif // MSATUIOBSERVER_H
