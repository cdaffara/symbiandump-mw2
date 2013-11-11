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
*                event notifications related to call audio routing.
*
*/


#ifndef ROUTINGROBSERVER_H
#define ROUTINGROBSERVER_H

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
*
*/
class CRoutingObserver
    {
    public:

        /**
        * Event signalling available audio outputs have changed. The applications
        * has to call CCallAudioControl::AvailableOutputs to get the
        * updated list.
        *
        * @since Series 60 3.1, 3.2
        *
        */
        static const TUint KAvailableOutputsChanged = 1;

        /**
        * Event signalling current output has changed. The applications has to
        * call CCallAudioControl::Output to get the current active output.
        *
        * The application will get this event only when the change in output is
        * due some other application's CCallAudioControl::SetOutput call OR
        * because of user has deleted audio link through long key press on the
        * BT device OR when adaptation routes audio from device to BT accessory
        * automatically when user initiated long key press event occurs.
        *
        * @since Series 60 3.1, 3.2
        *
        */
        static const TUint KOutputChanged = 2;

        /**
        * Event signalling CCallAudioControl::SetOutput request issued by this
        * application (on CCallAudioControl instance) was serviced.
        *
        * Only when the observer receives this callback, the application should
        * consider CCallAudioControl::SetOutput message to be completed.
        *
        * KErrNone if output requested by application is set, else system-wide
        * error code.
        */
        static const TUint KSetOutputComplete = 3;

        /*
        * Callback function called by aCallAudioControl object when Call Audio
        * Control Event occurs.
        *
        * @since Series 60 3.1, 3.2
        *
        * @param CCallAudioControl& aCallAudioControl reference.
        * @param TUint. A event identifier. Possible values are
        *        1. KAvailableOutputsChanged.
        *        2. KOutputChanged.
        *
        */
        inline virtual void RoutingEvent( CCallAudioControl& aCallAudioControl, TUint aEvent ) = 0;

        /*
        * Callback function called by aCallAudioControl object when Call Audio
        * Control Event occurs.
        *
        * @since Series 60 3.1, 3.2
        *
        * @param CCallAudioControl& aCallAudioControl reference.
        * @param TUint. A event identifier. Possible values are
        *        1. KSetOutputComplete.
        * @param TInt. Return code associated with the event.
        *
        */
        inline virtual void RoutingEvent( CCallAudioControl& aCallAudioControl, TUint aEvent, TInt aError ) = 0;

    };

//#include <RoutingObserver.inl>

#endif      // ROUTINGROBSERVER_H

// End of File
