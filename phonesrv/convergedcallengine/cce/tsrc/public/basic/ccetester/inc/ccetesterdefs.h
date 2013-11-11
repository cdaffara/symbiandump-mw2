/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definitions
*
*/
/*
* ==============================================================================
*  Name        : ccetesterdefs.h
*  Part of     : CCCETester
*
*  Description : Definitions
*
* =============================================================================*/

#ifndef CCCETESTERDEF_H
#define CCCETESTERDEF_H

const TInt KMaxKeywordLength = 50;

const TInt KMaxAmountOfCalls = 8;

const TInt KEventTimerPeriod = 200;
const TInt KDelayLength = 500;

// Logging path
_LIT( KCCCETesterLogPath, "\\logs\\testframework\\CCETester\\" ); 

// Log file
_LIT( KCCCETesterLogFile, "CCETester.txt" ); 

// a UID for identifying P&S events, test UID used
const TUid KPSUidEventRequest1 = { 0x01282F0D };
const TUid KPSUidEventRequest2 = { 0x01282E3F };

// UIDs given to ICM that loads plugin
const TInt KImplementationUidPlugin1 = 0x10282589;
const TInt KImplementationUidPlugin2 = 0x01282E3F;

const TUint32 KPropertyKeyEventIdentifier = 0x00000001;

/* When this is enabled, CCE is not created
*  in CCCETester constructor
*/
//#define __DONT_CREATE_CCE_IN_CONSTRUCTOR

#endif //CCCETESTERDEF_H