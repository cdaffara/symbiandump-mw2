/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
*     This file contains the data structure definitions
*     needed by both the server and the client interfaces at compile
*     time.
*
*
*/


#ifndef SATSTYPES_H
#define SATSTYPES_H

//  INCLUDES
#include <e32base.h>
#include <etelsat.h>
#include <msatuiobserver.h>

// CONSTANTS

// The length of the IPC data buffer.
const TInt KSatIpcBufferLength = 256;

// The size of the name buffer of SAT application.
const TInt KSatAppNameSize = 255;

// The size of the name buffer of SAT application.
const TInt KSatGetInputEntrySize = 256;

// DATA TYPES

// The query IPC data structure.
struct TSatDisplayTextV1
    {
    TUint8 iPCmdNumber;
    TBuf<KSatIpcBufferLength> iText;
    TBuf<KSatIpcBufferLength> iSimApplicationName;
    RSat::TIconId iIconId;
    TBool iSustainedText;
    TTimeIntervalSeconds iDuration;
    RSat::TClearScreenTrigger iClearScreen;
    };

// The query IPC data package.
typedef TPckg<TSatDisplayTextV1> TSatDisplayTextV1Pckg;

// The event IPC data structure.
struct TSatEventV1
    {
    TSatSEvent iEvent;
    TSatSEventStatus iStatus;
    TInt iError;
    };

// The event IPC data package.
typedef TPckg<TSatEventV1> TSatEventV1Pckg;

// The query IPC data structure.
struct TSatQueryV1
    {
    TBuf<KSatIpcBufferLength> iQueryText;
    TSatSQueryCommand iCommand;
    TBuf<KSatIpcBufferLength> iSimApplicationName;
    TBuf<KSatIpcBufferLength> iDestinationName;
    TSatAlphaIdStatus iAlphaIdStatus;
    TSatIconId iIconId;
    };

// The query IPC data package.
typedef TPckg<TSatQueryV1> TSatQueryV1Pckg;

// The query response IPC data structure.
struct TSatQueryRspV1
    {
    TSatSQueryCommand iCommand;
    TBool iAccepted;
    TInt iWgId;
    TBool iRequestedIconDisplayed;
    TBool iSessionTerminatedByUser;
    };

// The query response IPC data package.
typedef TPckg<TSatQueryRspV1> TSatQueryRspV1Pckg;

// The notification IPC data structure.
struct TSatNotificationV1
    {
    RSat::TAlphaIdBuf iText;
    TSatSNotifyCommand iCommand;
    TSatAlphaIdStatus iAlphaIdStatus;
    TSatControlResult iControlResult; // CC and MoSm
    TSatIconId iIconId;
    };
    
// The notification response IPC data package.
typedef TPckg<TSatNotificationV1> TSatNotificationV1Pckg;

// The notification IPC data structure.
struct TSatNotificationRspV1
    {
    TSatSNotifyCommand iCommand;
    TBool iAccepted;
    TInt iWgId;
    TBool iRequestedIconDisplayed;
    TBool iSessionTerminatedByUser;
    };
    
// The notification response IPC data package.
typedef TPckg<TSatNotificationRspV1> TSatNotificationRspV1Pckg;

// Types for session termination
enum TSatTerminationType
    {
    ETerminatedByCancel,
    ETerminatedByEndKey
    };

// Session termination data structure
struct TSatTermination
    {
    TSatTerminationType type;
    };

// Session termination IPC data package.
typedef TPckg<TSatTermination> TSatTerminationPckg;

#endif      // SATSTYPES_H

// End of File
