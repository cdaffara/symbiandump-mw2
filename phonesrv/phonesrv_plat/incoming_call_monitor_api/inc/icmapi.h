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
* Description:  SDK Publish&Subscribe definitions of the
*                Incoming Call monitor API
*
*/


#ifndef __ICMAPI_H
#define __ICMAPI_H

// INCLUDES
#include <e32cmn.h>      // TInt etc definitions
#include <e32property.h> // P&S support

/** 
* @file icmapi.h
* 
* The ICM API provides P&S key for incoming call notifications. 
* ICM abbreviation comes from Incoming Call Monitor. Key can be updated by the 
* client application. This key provide UID to the ICM, that UID will then be forwared 
* to Converged Call Engine(CCE). CCE will address plugin component identified by the given 
* UID - in order to handle the pending incoming call. The plugin will be loaded if it 
* is not yet so.
*
* ICM has defined the property and monitors value changes made by clients to that property.
*
* This interface relies on the Publish and Subscribe (P&S) interface provided by Symbian. 
*
* The P&S UID is #KPSUidICMIncomingCall category and following key is provided:
* - #KPropertyKeyICMPluginUID
*
* Client application requires capabilities TODO 16112007: no capabilities defined yet! 
* in order to use the service
* 
*
* @code
* // example of notifying ICM about call provider plug-in (UID=0x123456789) having an 
* // incoming call 
* #include <icmapi.h>
* TICMUIDValueType pluginuid = 0x123456789
* 
* // KErrNone is returned if all was ok. 
* // KErrNotFound if property is not defined
* // KErrPermissionDenied  is client does have required capabilities
* TInt result = RProperty::Set( KPSUidICMIncomingCall, 
*                               KPropertyKeyICMPluginUID, 
*                               pluginuid );
* @endcode
*
*/

/** 
P&S UID for all incoming call monitor keys
*/
const TUid KPSUidICMIncomingCall = { 0x10202867 }; 


/**
* P&S key to represent the UID of the plugin to handle the incoming call.
* The client will place the UID of the call provider plug-in to this property.
* @see KPSUidICMIncomingCall
*/
const TUint32 KPropertyKeyICMPluginUID = 0x00000001;

/**
ICM P&S property parameter value type
*/
typedef TInt32 TICMUIDValueType;

/**
Property type, used by entity defining the property
*/
const TInt TICMUIDPropertyType = RProperty::EInt;


#endif //__ICMAPI_H


