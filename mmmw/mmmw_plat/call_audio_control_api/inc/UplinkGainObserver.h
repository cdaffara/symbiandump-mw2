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
* Description:  Observer class that the client must implement to receive
*                event notifications related to call audio.
*
*/


#ifndef UPLINKGAINOBSERVER_H
#define UPLINKGAINOBSERVER_H

// INCLUDES

// CLASS DECLARATION
class CCallAudioControl;

/**
*  Defines functions that client must implement in order to receive
*  request completion and event notifications.
*
*  Clients should ignore events in which they are not interested or unknown
*  events.
*
*  The callbacks will only occur during an active call.
*
*  @since Series 60 3.1, 3.2
*/
class CUplinkGainObserver
    {
    public:

        /**
        * Event signalling uplink call audio is either muted or unmuted.
        *
        * The client will get this event only when the uplink call audio is
        * muted/unmuted due some other application changing it.
        *
        */
        static const TUint KUplinkMuteStateChanged = 1;

        /*
        * Callback function called by aCallAudioControl object when Call Audio
        * Control Event occurs.
        *
        * @since Series 60 3.1, 3.2
        *
        * @param CCallAudioControl& aCallAudioControl reference.
        * @param TUint. A event identifier. Possible values are
        *        1. KUplinkMuteStateChanged.
        *
        */
        inline virtual void UplinkGainEvent( CCallAudioControl& aCallAudioControl, TUint aEvent ) = 0;

    };

#endif      // UPLINKGAINOBSERVER_H

// End of File
