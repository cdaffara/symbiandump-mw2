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
* Description:  Responsible for settings and retreiving all location 
*				 triggering settings
*
*/


#ifndef LBTSETTINGSMANAGER_H
#define LBTSETTINGSMANAGER_H

// INCLUDE FILES
#include <e32base.h>
#include <lbscommon.h>
#include <lbtcommon.h>

// FORWARD DECLARATIONS
class CRepository;

/**
 * Class declaration for location triggering settings manager
 * All reads and write to the Central Repository keys of
 * location triggering settings has to be done through this 
 * interface. This is a singelton class
 * 
 * @since S60 v4.0
 */
class CLbtSettingsManager : public CBase
	{
	public:
		/**
	     * Instantiates a new object of 
	     * CLbtSettingsManager
	     *
	     * @return NULL when there isn't enough memory otherwise
	     * pointer to an object of type CLbtSettingsManager
	     *
	     */
	    static CLbtSettingsManager* NewL( );
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtSettingsManager();
	    
	private:
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();
		
	public: // New functions
		
		/**
		 * Sets the optimization
		 *
		 * @param[in] aOptimizationLevel the optimization level which 
		 * is the balance between accuracy and optimization level
		 */
		void SetOptimizationLevel(TInt aOptimizationLevel);
		
		/**
		 * Get the optimization settings which is the balance
		 * between accuracy and optimization level
		 *
		 * @return interger optimization level
		 */
		TInt GetOptimizationLevel();
		
		/**
		 * Sets the module id that the trigger server has to
		 * use to obtain location fixes
		 *
		 * @param[in] aModuleId position module id
		 */
		void SetModuleId(TPositionModuleId aModuleId);
		
		/**
		 * Gets the module id which has to used to get
		 * position fixes
		 * 
		 * @return the module id used
		 */ 
		TPositionModuleId GetModuleId();
		
		/**
		 * Gets the module id of NPP PSY which has to used to get
		 * network based position fixes
		 * 
		 * @return the module id of NPP PSY
		 */ 
		TPositionModuleId GetNppModuleId();
		
		/**
		 * Sets the minumum update interal that has to be set
		 * to get position updates
		 *
		 * @param[in] aUpdateInterval the minimum update interval
		 */ 
		void SetMinimumUpdateInterval(TInt aUpdateInterval);
		
		/**
		 * Gets the minumum update interal that has to be set
		 * to get position updates
		 *
		 * @return the minimum update interval
		 */ 
		TInt GetMinimumUpdateInterval();
		
		/**
		 * Sets the minimum update interavl when
		 * GPS fails
		 *
		 * @param[in] aUpdateInterval the minimum update interval 
		 */
		void SetMinimumUpdateIntervalOnGpsFailure(TInt aUpdateInterval);
		
		/**
		 * Gets the minimum update interavl when
		 * GPS fails
		 *
		 * @return the minimum update interval 
		 */
		TInt GetMinimumUpdateIntervalOnGpsFailure();
		
		/**
		 * Sets the maximum user speed used for update interval
		 * calculations
		 * 
		 * @param[in] aSpeed maximum user speed to be set
		 */
		void SetMaximumUserSpeed(TInt aSpeed);
		
		/**
		 * Gets the maximum user speed used for update interval
		 * calculations
		 * 
		 * @return maximum user speed to be set
		 */
		TInt GetMaximumUserSpeed();
		
		/**
		 * Sets the triggering mechanism state
		 *
		 * @param[in] aState state of the triggering mechanism
		 */
		void SetTriggeringMechanismState(TLbtTriggeringMechanismState aState);
		
		/**
		 * Gets the triggering mechanism state
		 *
		 * @return state of the triggering mechanism
		 */
		TInt GetTriggeringMechanismState();
		
	public: // Getter routines for trigger configurable parameters
		
		/**
		 * Gets the minimum area that a trigger entry should have
		 *
		 * @return the triggering area
		 */
		TInt MinimumTriggeringArea();
		
		/**
		 * Gets the tracking mode interval
		 *
		 * @return gets the tracking mode interval
		 */
		TInt TrackingModeInterval();
		
		/**
		 * Gets the location request time out value
		 *
		 * @return location request time out
		 */
		TInt GetLocationRequestTimeOut();
		
		/**
		 * Gets the minimum size of hysteresis area. This
		 * is basically the radius of the hysteresis ring
		 *
		 * @return minimum size of hysteresis area
		 */
		TInt MinimumHysteresisArea();
		
		/**
		 * Get the number of days that start up triggers can
		 * be preserved in the system when the owner application
		 * is not present in the system.
		 *
		 * @return number of days
		 */
		TInt NumberOfDaysForCleanup();
		
		/**
		 * Gets the count of maximum enabled and valid triggers that
		 * can be present in the system at any point in time.
		 *
		 * @return count of enabled and valid triggers
		 */
		TInt EnabledValidTriggerCount();

	private: // Member variables
	
		// Optimization level
		TInt iOptimizationLevel;
		
		// Module id to be used to make update intervals
		TInt iModuleId;
		
		// Update interval for position requests
		TInt iMinUpdateInterval;
		
		// Minimum update interval on gps failure
		TInt iMinUpdateIntervalOnGpsFailure;
		
		// Maximum user speed setting
		TInt iMaxUserSpeed;
		
		// Triggering mechanism. i.e. on or off
		TInt iTriggeringMechanismState;
		
		// Minimum trigger area
		TInt iMinimumTriggerArea;
		
		// Track mode interval
		TInt iTrackModeInterval;
		
		// Location request time out
		TInt iLocRequestTimeOut;
		
		// Minimum hysteresis area
		TInt iMinHysteresisArea;
		
		// Number of days for cleanup of triggers in system
		TInt iDaysForCleanup;
		
		// Count of enabled and valid triggers that can be present in tht system
		TInt iEnabledValidTriggerCount;
		
		// Module ID of NPP (Network positioning) PSY
		TInt iNppModuleId;

		// Pointer an cenrep object
		CRepository* iRepository;
	};

#endif // LBTSETTINGSMANAGER_H