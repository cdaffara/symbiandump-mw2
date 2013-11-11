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
* Description:  Sub-Session registry class
*
*/


#ifndef C_LBTSUBSESSIONREGISTRY_H
#define C_LBTSUBSESSIONREGISTRY_H

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CObjectCon;
class CObjectConIx;
class CLbtSubSession;

/**
 * Class definition for LBT sub-session registry
 * @since S60 v4.0
 */
class CLbtSubSessionRegistry : public CBase
	{
	public:
		/**
	     * Instantiates a new object of 
	     * CLbtSubSessionRegistry
	     *
	     * @return pointer to an instance of 
	     * CLbtSubSessionRegistry
	     */
	    static CLbtSubSessionRegistry* NewL();
	    
	    /**
         * C++ destructor
         *
         */
        ~CLbtSubSessionRegistry();
        
        /**
         * Adds an object instance to the registry and
         * takes ownership of the object
         *
         * @param aSubSession an sub-session object instance
         * @return a registry handle to the object instance.
         */
        TInt AddSubSessionL(CLbtSubSession* aSubSession);
        
        /**
         * Removes the subsession from the registry and 
         * also closes the sub-session handle
         *
         * @param aHandle a handle to the subsession
         * @return KErrNone if handle close successfully, 
         * ELbtServerBadHandle if handle not found
         *
         */
        TInt CloseSubSession( TUint aHandle );
        
        /**
         * Retreives the appropriate subsession handle
         * with the specified index
         *
         * @param aHandle index to the subsession handle
         * @return pointer the subsession 
         *
         */
        CLbtSubSession* SubSessionFromHandle( TUint aHandle );
        
        /**
         * Returns the number of sub-sessions stored in the 
         * registry
         *
         * @return the number of subsessions in the registry
         *
         */
        TInt Count();
        
        
        TInt GetSubSessionHandle(TInt aIndex);
        
    private:
    	/**
    	 * Default C++ Constructor
    	 *
    	 */
    	CLbtSubSessionRegistry();
    	
    	/**
	     * 2nd phase constuctor for instantiating 
	     * member variables
	     *
	     */
		void ConstructL();

	private:// Data
		// CObject to create the object container
		CObjectIx*      iRegistryIndex;
		
		// Object index container
		CObjectCon*     iRegistryContainer;
		
		// Object index
        CObjectConIx*   iRegistryContainerIndex;		
	};

#endif //C_LBTSUBSESSIONREGISTRY_H

// end of file
