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
* Description:  IPC enumerations and constant definition for server.
*
*/

#ifndef PRIVACYINTERNAL_H
#define PRIVACYINTERNAL_H



//CONSTANTS

// IPC enumerations
enum TPosPrivacyClientServer
    {
    // To get the size of modified buffer containing resolved requestors.
    ELocPrivacyGetSize = 0,
    // To get the resolved requestors.
    ELocPrivacyResolve
    };

// Buffer granularity
const TInt KPosBufFlatExpandSize = 100;

#endif      // PRIVACYINTERNAL_H

// End of File
