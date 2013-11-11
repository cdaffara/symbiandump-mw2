/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  This class is responsible for loading and holding the 
*				 stratergy ecom plugin
*
*/


#ifndef C_LBTSTRATERGYCONTAINER_H
#define C_LBTSTRATERGYCONTAINER_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class CLbtStrategyBase;
class REComSession;
class MLbtTriggeringSupervisionObserver;

/**
 * Class definition for LBT Stratergy container implementation
 * @since S60 v4.0
 *
 */
class CLbtStratergyContainer : public CActive
	{
	public:
		/**
	     * Instantiates a new object of 
	     * CLbtStratergyContainer
	     *
	     * @return a pointer to an instance of
	     * CLbtStratergyContainer
	     */
	    static CLbtStratergyContainer* NewL();
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtStratergyContainer();
	    
	    /**
	     * This loads the stratergy ecom plugin and starts 
	     * listening to ecom upgrade events.On receiving an
	     * ecom upgrade event, a check is made to determine if
	     * the stratergy plugin is upgraded. If the plugin is
	     * upgraded then the cuurently loaded plugin is unloaded
	     * and the new plugin is loaded.	     
	     *
	     * @param[in] the construction parameters to be sent to
	     * stratergy
	     *
	     */
	    void LoadStratergyL(MLbtTriggeringSupervisionObserver* aPtr);
	    
	    /**
	     * Returns the intance of stratergy 
	     *
	     * @return pointer to the stratergy intance
	     */
	    CLbtStrategyBase* Stratergy();
	    
	    /**
	     * Unloads the stratergy ecom dll and stops
	     * listening to ecom plugin upgrade events
	     *
	     */ 
	    void UnloadStratergy();
	    
	    	    
	protected: // from CActive		
		void RunL();
		TInt RunError(TInt aError);
		void DoCancel();

	private: 
	    /**
	     * C++ Default constructor
	     *
	     */
	    CLbtStratergyContainer();
	
		/**
	     * 2nd phase constuctor for instantiating 
	     * member variables
	     *
	     */
		void ConstructL();
		
	private: // Data
	
		// Instance of stratergy
		CLbtStrategyBase* iStratergy;
		
		// Handle to RProperty
		RProperty     iProperty;

        // Pointer to the ECom session
		REComSession* iEComSession;
				
		// Holds the version number of the currently 
		// loaded stratergy dll
		TInt iVersion;
	};

#endif // C_LBTSTRATERGYCONTAINER_H

