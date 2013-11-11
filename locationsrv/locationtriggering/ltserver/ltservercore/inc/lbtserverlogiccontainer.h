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
*				 server logic ecom plugin
*
*/


#ifndef C_LBTSERVERLOGICCONTAINER_H
#define C_LBTSERVERLOGICCONTAINER_H

// INCLUDE FILES
#include <e32base.h>

// FORWARD DECLARATIONS
class CLbtServerLogicBase;
class REComSession;

/**
 * Class definition for LBT Server Logic Container implementation.
 * This class is responsible for loading, unloading and updation
 * of server logic ecom plugin.
 *
 * @since S60 v4.0
 *
 */
class CLbtServerLogicContainer : public CActive
	{
	public:
		/**
	     * Instantiates a new object of 
	     * CLbtServerLogicContainer
	     *
	     */
	    static CLbtServerLogicContainer* NewL();
	    
	    /**
	     * Destructor
	     *
	     */
	    ~CLbtServerLogicContainer();
	    
	    /**
	     * This loads the server logic ecom plugin
	     *
	     */
	    void LoadServerLogicL();
	    
	    /**
	     * Returns the intance of the server logic
	     *
	     * @return pointer to the server logic intance
	     */
	    CLbtServerLogicBase* ServerLogic();
	    
	    /**
	     * Unloads the server logic dll
	     *
	     */ 
	    void UnloadServerLogicL();
	    
	    	    
	protected: // from CActive		
		void RunL();
		TInt RunError(TInt aError);
		void DoCancel();

	private: 
	    /**
	     * C++ Default constructor
	     *
	     */
	    CLbtServerLogicContainer();
	
		/**
	     * 2nd phase constuctor for instantiating 
	     * member variables
	     *
	     */
		void ConstructL();
		
	private: // Data
	
		// Instance of server logic 
		CLbtServerLogicBase* iLogicInterface;

        // Pointer to the ECom session
		REComSession* iEComSession;
				
		// Holds the version number of the currently 
		// loaded logic interface
		TInt iVersion;
	};

#endif // C_LBTSERVERLOGICCONTAINER_H

