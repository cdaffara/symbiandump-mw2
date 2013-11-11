/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  SAT specific message services provided to
*                Secondary Display software.
*
*/


#ifndef SECONDARYDISPLAYSATAPI_H
#define SECONDARYDISPLAYSATAPI_H


#include <e32base.h>
#include <etelsat.h>


/*
* ==============================================================================
* This file contains the following sections:
*   - Command definitions
*   - Event definitions
*   - Parameter definitions
* ==============================================================================
*/


/**  The category UID for the messages in this header file. */
const TUid KCatSAT = { 0x102078E4 };

/**  Version between SAT mediator Event provider - consumer */
const TInt KVersionMajor( 1 );
const TInt KVersionMinor( 0 );
const TInt KVersionBuild( 0 );

/**  Duration of SAT command for EMsgAnySatCommand message */
/**  Unit is TTimeUnit, see etelsat.h */
const TUint8 KSatDurationNotSet = 0;
const TUint8 KSatDefaultDuration = 3;
const TUint8 KSatLongDuration = 60;
const TUint8 KSatSustainedDuration = 255;

/*
* ==============================================================================
* SAT related events for Secondary Display.
* ==============================================================================
*/
/**  SAT related events for Secondary Display. */
enum TSecondaryDisplaySatEvents
    {
    /**
    * A message delivering base information about any SAT command
    *
    * @input    TSatCommandPckg     Base information about SAT command
    */
    EMsgAnySatCommand = 0x0101

    };

/*
* ==============================================================================
* Parameter definitions for the messages in this file.
* ==============================================================================
*/

// Parameter definitions for EMsgAnySatCommand message
// Output:

//lint -e{1401,1926,1927} No relevant initialisation can be done
class TSatCommandData
    {
public:
    TSatCommandData(){};
public:
    RSat::TPCmd iPCmdNumber;
    RSat::TAlphaId iAlphaId;
    RSat::TDuration iDuration;
    RSat::TIconId iIconID;
    };

typedef TPckgBuf<TSatCommandData> TSatCommandPckg;

#endif // SECONDARYDISPLAYSATAPI_H

