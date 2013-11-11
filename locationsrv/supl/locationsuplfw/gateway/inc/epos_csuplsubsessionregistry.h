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
* Description:   Registry for subsessions
*
*/



#ifndef __CSuplSubSessionRegistry_H__
#define __CSuplSubSessionRegistry_H__

// INCLUDES
#include <e32base.h>

// FORWARD DECLARATIONS
class CSuplIPCSubSession;

// CLASS DECLARATION

/**
*  Class to handle subsession registry entries for a session 
*/
class CSuplSubsessionRegistry :public CBase
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor
        *
        * @return a new instance of this class
        */
		static CSuplSubsessionRegistry* NewL();

        /**
        * C++ destructor
        */
		~CSuplSubsessionRegistry();

	public:		// New functions

        /**
        * Retreives the appropriate subsession.
        *
        * @param aHandle the subsession handle
        * @return a subsession
        */
        CSuplIPCSubSession* SubSessionFromHandleL(
            /* IN  */   TUint aHandle
            );

        /**
        * Retreives the appropriate subsession.
        *
        * @param aIndex the index in the list
        * @return a subsession, NULL if no session at specified index.
        */
        CSuplIPCSubSession* SubSessionFromIndex(
            /* IN  */   TInt aIndex
            );

        /**
        * Returns the maximum number of reference counting 
		* objects that the object index can hold before 
		* internal buffers need to be expanded.
        * @return the maximum number of reference counting objects.
        */
        TInt Count();

        /**
        * Closes a subsession.
        *
        * @param aHandle a handle to the subsession
        */
        void CloseSubSession(
            /* IN  */        TUint aHandle
            );

        /**
        * Adds an object instance to the registry and
        * transfers object ownership to the registry.
        *
        * @param aSubSession an object instance
        * @return a registry handle to the object instance.
        */
        TInt AddInstanceL(
            /* IN  */     CSuplIPCSubSession* aSubSession
            );

    private:

        /**
        * C++ default constructor.
        */
        CSuplSubsessionRegistry();

        /**
        * By default EPOC constructor is private.
        */
        void ConstructL();

        // By default, prohibit copy constructor
        CSuplSubsessionRegistry( const CSuplSubsessionRegistry& );
        // Prohibit assigment operator
        CSuplSubsessionRegistry& operator= ( const CSuplSubsessionRegistry& );

    private:	// Data

        CObjectIx*      iRegistryIndex;
        CObjectCon*     iRegistryContainer;
        CObjectConIx*   iRegistryContainerIndex;
    };


#endif // __CSuplSubSessionRegistry_H__

// End of File
