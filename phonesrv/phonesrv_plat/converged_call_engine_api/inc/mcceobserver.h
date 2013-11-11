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
* Description:  Observer interface for CCE
*
*/


#ifndef MCCEOBSERVER_H
#define MCCEOBSERVER_H

#include <e32base.h>


class MCCECall;
class MCCEConferenceCall;

/**
 *  Observer interface for CCCE class.
 *  @lib cce.dll
 *  @since S60 v3.2
 */
class MCCEObserver
    {
public:

    /**
    * Provides incoming call notification to a client.
    * Incoming call has to have correct service id. This is checked from passed call 
    * using MCCECall::ServiceId method. 
    *
    * @since S60 v3.2
    * @param aCall Reference to incoming call
    * return none
    */
    virtual void IncomingCall( MCCECall& aCall ) = 0;

    /**
    * MO call created outside client control. 
    *
    * @since S60 v3.2
    * @param aCall Reference to created call
    * return none
    */
    virtual void MOCallCreated( MCCECall& aCall ) = 0;

    /**
    * Conference has been created outside client control.
    * @since S60 3.2
    * @param aConferenceCall Created conference
    * @return none
    */
    virtual void ConferenceCallCreated( MCCEConferenceCall& aConferenceCall ) = 0;

    /**
    * Notify data port information for video telephony. Information comes via CS only.
    * @since S60 3.2
    * @param Dataport information.
    * @return none
    */
    virtual void DataPortName( TName& aPortName ) = 0;
    };

#endif // MCCEOBSERVER_H

