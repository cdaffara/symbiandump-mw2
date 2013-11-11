/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/

#ifndef SATAPPCONSTANT_H
#define SATAPPCONSTANT_H

#ifdef _UDEB
#define SAT_ASSERT Q_ASSERT
#else
#define SAT_ASSERT(cond) cond
#endif

//  CONSTANTS

const int KDisplayTxtUserClearTimeout( 60000 );
// As the QT for S60 is slow, we use 6 sec, in symbian sat ui we use 3 sec.
// Rollback to 3s
const int KDisplayTxtDefaultduration( 3000 );

const unsigned int KGetInputStringMaxSize = 0xEF;

const int KExtendTimeoutMseconds(3000);
// 1 minute
const int KDefaultSelectionTimeoutMseconds(60000);
const int KSymbianTimeConvertQtTime(100);
const int KMoSmControlTimeOut(3000);
const int KPlayToneSymbianConvertQtTime(1000);

// extensions to ConfirmAction
const int ESatConfirmAction = 0x35;
// extensions to TSatAction
const int ESatNotifyAction = 0x1001;

// SatAppAction
// parameter keys
const QString KeyText="text"; // QString
const QString KeyMenu="menu"; // QList<QVariant/QString>
const QString KeyDefault="default"; // int - default selected item index
const QString KeySelection="selection"; // int - real selected index
const QString KeyCharacterSet="charset"; // int(TSatCharacterSet)
const QString KeyInKey="inkey"; // int(char pressed)
const QString KeyDuration="duration"; // int(milliseconds)
const QString KeyImmediate="immediate"; // bool - immediate response
const QString KeyInputString="input"; // QString - user input string
const QString KeyMinLength="min"; // int 
const QString KeyMaxLength="max"; // int
const QString KeyHideInput="hide"; // bool - password mode
const QString KeyToneId="tone"; // int(TSatTone)
const QString KeySustainedText="sustained"; // bool - sustained text
const QString KeyWaitUserToClear="waitusertoclear"; // bool
const QString KeyHelpRequested="helprequested"; // bool
const QString KeyApplicationName="applicationname"; // QString - app.name
const QString KeyCommandId="commandid"; // int(TSatSNotifyCommand)
const QString KeyControlResult="controlresult"; // int(TSatControlResult)
const QString KeyAlphaIdStatus="alphaidstatus"; // int(TSatAlphaIdStatus)
const QString KeyAdditionalText="additionaltext"; //QString - aAdditionalText
const QString KeyQueryCommandId="querycommandid";//int(TSatSQueryCommand)
const QString KeyActionAccepted="actionaccepted";//bool

const QString KeyselfExp="sekfexp";//bool

// icon identifiers
const QString KeyIconId="iconid"; // int - icon id
const QString KeyIconIdArray="iconidarray"; // QList<QVariant/int> (icon id)
const QString KeyIconQualifier="iconqualifier"; // int (TSatIconQualifier)

// icons
const QString KeyIcon="icon"; // HbIcon
const QString KeyIconArray="icons"; // QList<QVariant/QPixmap>

/**
 * Play Tone Constants
 */
// Tone Sequence
const char KNetToneSeqNetBusy[] = "\x00\x11\x05\x4E\x30\x40\x30\x07\x0B";
const char KNetToneSeqNetCallWaiting[] = "\x00\x11\x05\x4E\x14\x40\x3C\x4E\x14"
"\x40\x64\x40\x64\x40\x64\x4E\x14\x40\x3C\x4E\x14\x40\x64\x40\x64\x40\x64"
"\x07\x0B";
const char KNetToneSeqNetCongestion[] = "\x00\x11\x05\x4E\x14\x40\x14\x07\x0B";
const char KNetToneSeqNetSpecialInformation[] = "\x00\x11\x05\x4F\x21\x50"
"\x21\x51\x21\x40\x64\x07\x0B";
const char KNetToneSeqDial[] = "\x00\x11\x05\x4E\xFE\x4E\xFE\x4E\xFE\x4E\xFE\x4E\xFE"
"\x4E\xFE\x4E\xFE\x4E\xFE\x07\x0B";
const char KNetToneSeqRingGoing[] = "\x00\x11\x05\x4E\x64\x05\x04\x40\x64\06\x07\x0B";
const char KGeneralBeep[] = "\x00\x11\x0A\x76\x00\x0B";
const char KErrorTone[] = "\x00\x11\x0A\x6C\x19\x0B";
const char KRadioPathNotAvailable[] = "\x00\x11\x0A\x05\x03\x4E\x14\x40\x14\x06\x0B";
const char KRadioPathAck[] = "\x00\x11\x0A\x02\xFC\x0A\x80\x4E\x14\x0A\x7f\x0B";

// PlayTone durations
const int KSatDur170ms  = 170;  // 170ms
const int KSatDur200ms  = 200;  // 200ms
const int KSatDur250ms  = 250;  // 250ms
const int KSatDur1200ms = 1200; // 1200ms

//enum for SendSms
/**Sat send sms user response*/
enum TSatAppConfirmType
    {
    ESatUiConfirmSendSms = 1,
    ESatUiConfirmSendSs,
    ESatUiConfirmSendUssd
    };
   
enum TSatAppUserResponse
{
    /**
     *  default, user no response
     */
    EUserNoResponse = 0,
    /**
     *  OK, user response
     */
    EUserPrimaryResponse,
    /**
     *  Back, user response
     */
    EUserSecondaryResponse,
    /**
     *  requested to clear by the server
     */
    EUserClearResponse,
    /**
     *  requested to close application by the server
     */
    EUserCloseResponse
};
    
enum TSatAppDialogActionType
    {
    ESatDialogDisplayText = 0, // Ok Back
    ESatDialogGetInput, // Cancel ->Ok Cancel
    ESatDialogGetInkey, // Cancel ->Ok Cancel
    ESatDialogGetYesNo, // Yes No
    ESatDialogGetDigitalRsp, // Cancel
    ESatDialogSetUpCall, // Call Cancel
    ESatDialogConfirmSend, // Yes No
    ESatDialogConfirmBip // Ok Cancel
    };

#endif 
