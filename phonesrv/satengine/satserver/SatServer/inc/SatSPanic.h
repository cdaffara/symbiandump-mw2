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
* Description:  Declares panic function and panic codes for SatServer.
*
*/



#ifndef SATSPANIC_H
#define SATSPANIC_H

// DATA TYPES
enum TSatSPanicCode
    {
    ESatSBadRequest,
    ESatSBadDescriptor,
    ESatSMainSchedulerError,
    ESatSCreateServer,
    ESatSStartServer,
    ESatSCreateTrapCleanup,
    ESatSNotImplementedYet,
    ESatSNonNumericString,
    ESatSCommandError,
    ESatSUnsupportedEvent
    };

// FUNCTION PROTOTYPES
void PanicSatServer( TSatSPanicCode aPanicCode );

#endif      // SATSPANIC_H

// End of File
