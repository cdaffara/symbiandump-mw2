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
* Description:  Triggering classes for S60 SUPL Terminal Initiation API
*
*/

/*! \file epos_suplterminaltrigger.h
    \brief Triggering classes 
*/

#ifndef __EPOS_SUPLTERMINAL_TRIGGER_H__
#define __EPOS_SUPLTERMINAL_TRIGGER_H__

#include <e32std.h>
#include <e32base.h>
#include <lbspositioninfo.h>

// ==========================================

/**
 * Class defines detailed information of a trigger firing event.
 *
 * @since S60 5.2
 */
class TSuplTriggerFireInfo
    {
public:

    /**
     * Default constructor to Reset the object.
     * @param 	None
     * @return 	None
     */
	IMPORT_C TSuplTriggerFireInfo();

    /**
     * Sets Triggering Info parameters.
     * @param [IN] aPositionInfo   Position Information 
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if any of the argument is not in the specified range 
     *			or values specified are not reset value
     */
	IMPORT_C TInt Set(const TPositionInfo& aPositionInfo);

    /**
     * Returns Triggering Info parameters. 
     * @param [OUT] aPositionInfo 	Number of position fixes  
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if all of the parameters has reset value 
     */
	IMPORT_C TInt Get(TPositionInfo& aPositionInfo) const;
    
private:
    /**
     * Position information of when the trigger was fired. Only
     * basic position information(class TPositionInfo) is returned by Location 
     * Triggering Server.
     */
    TPositionInfo iFiredPositionInfo;
    
    /**
     * For future use.
     */
    TUint8 iUnused[8];
    
    TBool iIsValueSet;
    };







/**
 * Class defines detailed information of a Periodic Type of Trigger.
 *
 * @since S60 5.2
 */
class TSuplTerminalPeriodicTrigger
	{
public: // functions

    /**
     * Default constructor to Reset the object.
     * @param 	None
     * @return 	None 
     */
	IMPORT_C TSuplTerminalPeriodicTrigger(); 
	
    /**
     * Sets Periodic Trigger Parameters
     * The client have some limitations while setting the periodic trigger parameters 
     * A client can set the number of fixes ranging from 1 to 8639999.
     * A client can set the time interval ranging from 1 to 8639999 in seconds
     * A client can set the start time ranging from 0 seconds to 2678400 in seconds
     *
     * @param [IN] aNoOfFixes 	Number of position fixes required 
     * @param [IN] aInterval 	Time interval between each fixes in seconds 
     * @param [IN] aStartTime   Time in seconds at which triggering will start 
     *
     * @return following error codes 
     *      - KErrNone if successful
     *      - KErrArgument if any of the argument is not in the specified range or 
     *			values specified are not reset value
     */
	IMPORT_C TInt Set( 
			const TUint& aNoOfFixes,		
			const TUint& aInterval, 
			const TUint& aStartTime
			); 

    /**
     * Returns periodic trigger parameters. 
     * Client must set the values before calling Get()
     *
     * @param [OUT] aNoOfFixes 	Number of position fixes  
     * @param [OUT] aInterval 	Time interval between each fixes in seconds
     * @param [OUT] aStartTime 	Time in seconds at which triggering will start
     *
     * @return following error codes
     *      - KErrNone if successful
     *      - KErrNotFound if all of the parameters have not set
     */
	IMPORT_C TInt Get( 
			TUint&	aNoOfFixes,
			TUint&	aInterval,
			TUint&	aStartTime		
	) const;

private:
	TUint	iNoOfFixes;		
	TUint 	iInterval;		
	TUint	iStartTime;		
	TBool	iAreParamsSet;
	};


// ===========================================

#endif // __EPOS_SUPLTERMINAL_TRIGGER_H__
