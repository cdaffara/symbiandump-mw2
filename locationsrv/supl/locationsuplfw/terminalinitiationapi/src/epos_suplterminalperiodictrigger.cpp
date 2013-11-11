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
* Description:  Client-Server communication
*
*/

// Includes

#include <epos_suplterminaltrigger.h>
#include <epos_csuplsettingsconstants.h>
#include <epos_suplterminalconstants.h>


/*! \file epos_suplterminalperiodictrigger.cpp
    \brief Periodic Trigger Type 
*/

// Constants
/*
 * Trigger Constants Data
 */ 
/** Maximum value for number of fixes for Triggering */
const TUint KTrgMaxNoOfFixes 	= 8639999;
/** Maximum value for time interval in seconds for Triggering */
const TUint KTrgMaxInterval 	= 8639999;
/** Maximum value for starttime in seconds for Triggering */
const TUint KTrgMaxStartTime 	= 2678400;

/** Minimum value for number of fixes for Triggering */
const TUint KTrgMinNoOfFixes 	= 1;
/** Minimum value for time interval in seconds for Triggering */
const TUint KTrgMinInterval 	= 1;




// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//		TSuplTriggerFireInfo
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TSuplTerminalPeriodicTrigger::Get()
//
// Sets PeriodicTrigger parameters.
// ---------------------------------------------------------------------------
//
EXPORT_C TSuplTriggerFireInfo::TSuplTriggerFireInfo()
	{	
	iIsValueSet = EFalse;
	}

// ---------------------------------------------------------------------------
// TSuplTerminalPeriodicTrigger::Get()
//
// Sets PeriodicTrigger parameters.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TSuplTriggerFireInfo::Set(const TPositionInfo& aPositionInfo)
	{
	iFiredPositionInfo = aPositionInfo;
	iIsValueSet = ETrue;
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// TSuplTerminalPeriodicTrigger::Get()
//
// Sets PeriodicTrigger parameters.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TSuplTriggerFireInfo::Get(TPositionInfo& aPositionInfo) const
	{
	if( iIsValueSet == EFalse)
		{
		return KErrNotFound;
		}
	aPositionInfo = iFiredPositionInfo;
	return KErrNone;
	}


// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
//		TSuplTerminalPeriodicTrigger
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// TSuplTerminalPeriodicTrigger::Set()
//
// Sets PeriodicTrigger parameters.
// ---------------------------------------------------------------------------
//
EXPORT_C TSuplTerminalPeriodicTrigger::TSuplTerminalPeriodicTrigger()
	{		
	iAreParamsSet = EFalse;
	}

// ---------------------------------------------------------------------------
// TSuplTerminalPeriodicTrigger::Set()
//
// Sets PeriodicTrigger parameters.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TSuplTerminalPeriodicTrigger::Set(
			const TUint& aNoOfFixes, 
			const TUint& aInterval, 
			const TUint& aStartTime)
	{		
	if( aNoOfFixes < KTrgMinNoOfFixes || aNoOfFixes >  KTrgMaxNoOfFixes
			|| aInterval < KTrgMinInterval || aInterval > KTrgMaxInterval
			|| aStartTime > KTrgMaxStartTime )	
		{
		return KErrArgument;
		}
		
	iNoOfFixes 	= aNoOfFixes;
	iInterval 	= aInterval;
	iStartTime 	= aStartTime;		
	iAreParamsSet = ETrue;
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// TSuplTerminalPeriodicTrigger::Get()
//
// Gets PeriodicTrigger parameters.
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TSuplTerminalPeriodicTrigger::Get(
				TUint& aNoOfFixes, 
				TUint& aInterval, 
				TUint& aStartTime) const
	{	
	if( !iAreParamsSet )
		{
		return KErrNotFound;
		}
	
	if( iNoOfFixes < KTrgMinNoOfFixes || iNoOfFixes >  KTrgMaxNoOfFixes
			|| iInterval < KTrgMinInterval || iInterval > KTrgMaxInterval
			|| iStartTime > KTrgMaxStartTime )	
		{
		return KErrNotFound;
		}

	aNoOfFixes 	= iNoOfFixes;
	aInterval 	= iInterval;
	aStartTime 	= iStartTime;		
	return KErrNone;
	}

// End of file
