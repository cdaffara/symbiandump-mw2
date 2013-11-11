/*
* Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Object locking features*
*/

#ifndef __MDSOBJECTLOCKLIST_H__
#define __MDSOBJECTLOCKLIST_H__

#include <e32base.h>

#include "mdscommoninternal.h"

class TMdSLockEntry;
class CMdSServerSession;
class CMdsNamespaceDef;

/**
* Metadata object locking.
* When opening objects for modifications, objects
* is locked so that no other session can modify the
* same object at same time.
*/
class CMdSObjectLockList : public CBase
    {
    public: // Constructors and destructor

        virtual ~CMdSObjectLockList();

        static CMdSObjectLockList* NewL();
        static CMdSObjectLockList* NewLC();

    private: // Private constructors

        void ConstructL();

    public: // Public functions
        /**
        * Locks object for modifying.
        * @param aSession   Session where object is being modified.
        * @param aNamespace Object's owner namespace
        * @param aObjId     Object id that is being modified.
        * @return           Returns whether object could be locked or not.
        */
        void LockObjectL( const CMdSServerSession& aSession, 
        		const CMdsNamespaceDef& aNamespace, const TItemId aObjectId );

        /**
        * Unlocks all locked objects of particular session.
        * @param aSession   Session which locked objects are to be released.
        */
        void UnlockBySession( const CMdSServerSession& aSession );

        /**
        * Unlocks locked object by Id.
        * @param aNamespace Object's owner namespace
        * @param aObjId     Object id that is being modified.
        */
        void UnlockById( const CMdsNamespaceDef& aNamespace, 
        		const TItemId aObjectId );

        /**
        * Checks if object is locked for modifying.
        * @param aNamespace Object's owner namespace
        * @param aObjId     An objects id which is going to be checked.
        * @return           <code>ETrue</code> if locked.
        *                   <code>EFalse</code> if not.
        */
        TBool IsLocked( const CMdsNamespaceDef& aNamespace, 
        		const TItemId aObjectId ) const;

    	/**
    	 * Empties the object lock list.
    	 */
        void Reset();

	protected:
		TInt Find( const CMdsNamespaceDef& aNamespace, 
				const TItemId aObjectId ) const;

    private:
        /**
         * Array which have the locked objects.
         */
        RArray<TMdSLockEntry> iLockListArray;
    };

/**
 * Internal data structure for storing information about locked objects.
 */
class TMdSLockEntry
    {
    public:
        /**
    	* Constructs a new lock list object.
        *
    	* @param aSession   session to which object id belongs
        * @param aNamespace Object's owner namespace
    	* @param aId        object id being modified
    	*/
    	TMdSLockEntry( const CMdSServerSession* aSession, 
    			const CMdsNamespaceDef& aNamespace, const TItemId aId )
    	: iSession( aSession ) , iNamespace( aNamespace ), iId( aId )
    	    {}

		static TInt Compare( const TMdSLockEntry& aFirst, 
				const TMdSLockEntry& aSecond );
    public:
        /* Session where object is being modified */
        const CMdSServerSession* iSession;

        /* Object's owner namespace */
        const CMdsNamespaceDef& iNamespace;

        /* Object's id which is being modified */
        TItemId iId;
    };

#endif  // __MDSOBJECTLOCKLIST_H__
