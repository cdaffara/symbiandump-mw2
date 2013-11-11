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
* Description:  SDK Publish&Subscribe definitions of the
*                location triggering status information API.
*
*/


#ifndef LBTSTATUSPSKEYS_H
#define LBTSTATUSPSKEYS_H

// INCLUDES
#include <e32std.h>

/** 
* @file lbtstatuspskeys.h
* 
* This API deinfes the location triggering status information keys. These keys
* provide up-to-date information on status of location triggering system, which
* is updated by Location Triggering Server. Any application may get the status
* of location triggering system and subscribe to these P&S keys to receive
* notifications of any change in state. 
*
* This interface relies on the Publish and Subscribe (P&S) interface provided
* by Symbian. 
*
* The P&S UID is #KPSUidLbtStatusInformation and following keys are provided:
* - #KLbtLocationTriggeringSupervisionStatus
*/

/**
* P&S UID for all location triggering status information keys provided by this API.
*/
const TUid KPSUidLbtStatusInformation = { 0x1028312B }; 


/**
* P&S key to shows the status of the location triggering suppervision 
* process. The type of this property is RProperty::EInt.
*
* @see TLbtLocationTriggeringSupervisionStatus
*/
const TUint32 KLbtLocationTriggeringSupervisionStatus = 0x00000001;

/**
* Location triggering supervision status.
*/
enum TLbtLocationTriggeringSupervisionStatus
    {
    /**
    * Location triggering is turned off. No trigger is supervised
    * by Location Triggering Server.
    */
    ELbtLocationTriggeringSupervisionOff                  = 0,

    /**
    * Location triggering supervision is not active, because there
    * is no active and enabled triggers in system.
    */
    ELbtLocationTriggeringSupervisionNotActive = 1,

    /**
    * Triggering supervision is active and successful.
    */
    ELbtLocationTriggeringSupervisionSuccessful = 2,

    /**
    * Triggering supervision is active but not successful. Reason 
    * can be e.g. no positioning method is available, triggering 
    * is denied, or positioning failed.This information is published only when 
    * subsequent location request is made by the location triggering server. 
    */
    ELbtLocationTriggeringSupervisionFailed     = 3,

    /**
    * Location Triggering Server is not ready to accept triggers(i.e. during 
    * system boot-up.
    */
    ELbtLocationTriggeringSupervisionNotReady   = 4
    };



#endif      // LBTSTATUSSDKPSKEYS_H