/*
* Copyright (c) 2006,2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Active Object operation for serving server logic requests
*
*/



#ifndef C_LBTCREATETRIGGERAOOPEATION_H
#define C_LBTCREATETRIGGERAOOPEATION_H

// INCLUDE FILES
#include <e32base.h>
#include "lbtcontainerextendedtriggerinfo.h"
#include "lbtaooperationbase.h"


// FORWARD DECLARATIONS
class MLbtAOObserver;
class RMessage2;

// FORWARD DECLARATIONS
class CLbtTriggerEntry;
class CLbtContainer;
class CLbtSettingsManager;
class CLbtContainerTriggerEntry;
class CLbtGeoCircle;

/**
 * Class declaration of active object operation
 * to service client requests comming from
 * server core. The IPC messages are provided to
 * this class to service the request asynchronously
 *
 * @since S60 v4.0
 */
class CLbtCreateTriggerAOOperation : public CLbtAOOperationBase
	{
	public:
	
		/**
	     * Instantiates a new object of 
	     * CLbtCreateTriggerAOOperation
	     *
	     * @param[in] aObserver reference to the active
	     * object observer
	     * @param[in] aMessage the IPC request which the active 
	     * object has to service. The active object takes responsibility
	     * of completing this IPC request
	     * @param aContainer reference to the container which holds
	     * the reference to the logic processor
	     */
	    static CLbtCreateTriggerAOOperation* NewL( MLbtAOObserver& aObserver,
				    							   const RMessage2& aMessage,
				    							   CLbtContainer& aContainer,
				    							   TInt aClientType,
				    							   CLbtSettingsManager& aSettingsManager );
				    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtCreateTriggerAOOperation();
	    
	    /**
		 * Cancels create trigger operation
		 *
		 */
		void CancelCreateTrigger();
	    
	private:
	
		/**
	     * 2nd phase constuctor for instantiating member variables
	     *
	     */
		void ConstructL();
		
		/**
	     * Default C++ Constructor
	     *
	     * @param[in] aObserver reference to the active object observer
	     * @param[in] aMessage the IPC request which the active 
	     * object has to service. The active object takes responsibility
	     * of completing this IPC request
	     * @param aContainer reference to the container which holds
	     * the reference to the logic processor
	     */
	    CLbtCreateTriggerAOOperation( MLbtAOObserver& aObserver, 
				    				  const RMessage2& aMessage,
				    				  CLbtContainer& aContainer,
				    				  TInt aClientType,
				    				  CLbtSettingsManager& aSettingsManager );
	
	public: // From CLbtAOOperationBase
	
	    /**
	     * Starts the AO operation of serving the request	     
	     *
	     */ 
		void StartOperationL();
		
		/**
		 * Returns the trigger id of the newly created trigger
		 *
		 * @return the trigger of the newly create trigger
		 */ 
		TLbtTriggerId GetTriggerId();
	
	private: // helper methods
		/**
		 * Calculates the hysteresis area 
		 *
		 * @param[in] the trigger for which the hysteresis
		 * are has to be created
		 * @return the hysteresis area
		 */
		TInt CalculateHysteresisAreaForTrigger(CLbtTriggerEntry& aTrigger);
		
		/**
		 * Handles create trigger operation
		 *
		 */
		void CreateTriggerL();
		
		/**
		 * Checks if the system cap for trigger will be satisfied if one 
		 * more trigger is created.
		 *
		 * @return ETrue if one more trigger can be created, EFalse otherwise
		 */
		TBool CheckTriggerCap();
		
		/**
		 * Calculates the Rectangular approximation of a trigger
		 * with a circular trigger area.
		 */
		void CalculateRectangularArea(
                        CLbtExtendedTriggerInfo::TLbtTriggerRectArea& aArea,
                        CLbtGeoCircle* aCircle);

		    
    protected: // From CActive
    	void RunL();
    	void DoCancel();

	private: // Data

		// Refernce to triggering settings manager
		CLbtSettingsManager& iSettingsManager;
		
				
		// Reference to container trigger entry
		CLbtContainerTriggerEntry* iContainerTriggerEntry;
		
	};

#endif // C_LBTCREATETRIGGERAOOPEATION_H