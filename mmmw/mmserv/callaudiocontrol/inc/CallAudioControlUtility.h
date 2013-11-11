/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Holds data types for CallAudioControl
*
*/

#ifndef CALLAUDIOCONTROLUTILITY_H
#define CALLAUDIOCONTROLUTILITY_H

// consts for volume:

const TInt KMinVolume = 0;
const TInt KMaxVolume = 10;

#ifdef __SERIES60_31__ /* 3.1 only */
    const TUid KUidCallInfo = {0x101F8787}; 
    const TUid KUidVolume = {0x101F8784}; 
    const TUid KUidMute = {0x100052FB};	
    const TUint32 KTelephonyCallState = 0x00000004;
    enum 
        {
        ECmdMuteMic = 5
        };
#else		
    /* 3.2 and beyond: */
    const TUid KUidCallInfo = {0x102029AC}; 
    const TUid KUidVolume = {0x102828B1}; 
    const TUid KUidMute = {0x102029A9}; 
    const TUint32 KTelephonyCallState = 0x00000001;
    #ifndef __SERIES60_32__ /* 5.0 and beyond */
        /* CallAudioControl is in mw and TPSTelMicMuteState is defined in app
         * layer (epoc32/include/platform/app/telmicmutestatuspskeys.h in 9.2).
         * Including this header will create upward dependency . Hence we are
         * redefining the enum here. */
        enum TPSTelMicMuteState
            {
            EPSTelMicMuteStateUninitialized = 0,
            EPSTelMicMuteOn,
            EPSTelMicMuteOff
            };
    #endif /*#ifndef __SERIES60_32__*/
#endif /*#ifdef __SERIES60_31__*/

const TUint32 KTelephonyEPVolume = 0x00000001;
const TUint32 KTelephonyIHFVolume = 0x00000002;

const TUint32 KTelephonyMute = 0x00000001;

enum TCallState {
    ECallStateUninitialized,
    ECallStateNone,
    ECallStateAlerting,
    ECallStateRinging,
    ECallStateDialling,
    ECallStateAnswering,
    ECallStateDisconnecting,
    ECallStateConnected,
    ECallStateHold
};

enum TMicMuteState 
{
	EMuteUninitialized,
	EMuteOn,
	EMuteOff
};
   
enum TAction 
{
	ECallState,
	EEPVolume,
	EIHFVolume,
	EMute
};

#endif // CALLAUDIOCONTROLUTILITY_H

// End of file
