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
* Description:  This is find engine to manage object searching,*
*/

#ifndef __MDSFINDENGINE_H__
#define __MDSFINDENGINE_H__

#include <e32base.h>

#include "mdscommoninternal.h"

// FORWARD DECLARATIONS
class CMdSFindSequence;
class CMdSServerSession;
class CMdSObjectLockList;
class CMdCSerializationBuffer;
class CMdsSchema;
class CMdSServer;

// CLASS DECLARATION
/**
* CMdSFindEngine.
*  Class for searching objects from metadata database.
*/
class CMdSFindEngine : public CBase
    {
    public: // Constructors and destructors

        /**
        * NewL.
        * Two-phased constructor.
        * @param aQueryId query id
        * @param aParent the session back reference
        * @param aLockList list of locked objects
        * @param aSchemaNew schema reference
        * 
        * @return Pointer to created CMdSFindEngine object.
        */
        static CMdSFindEngine* NewL(
        	TUint32 aQueryId, 
            CMdSServerSession& aParent,
            CMdSObjectLockList& aLockList,
            CMdsSchema& aSchemaNew);

        /**
        * NewLC.
        * Two-phased constructor.
        * @param aQueryId query id
        * @param aParent the session back reference
        * @param aLockList list of locked objects
        * @param aSchemaNew schema reference
        * @return Pointer to created CMdSFindEngine object.
        */
        static CMdSFindEngine* NewLC(
        	TUint32 aQueryId, 
            CMdSServerSession& aParent,
            CMdSObjectLockList& aLockList,
            CMdsSchema& aSchemaNew);

        /**
        * 2nd phase constructor
        */
        void ConstructL( CMdsSchema& aSchema );

        /**
        * ~CMdSFindEngine.
        * Destructor.
        */
        virtual ~CMdSFindEngine();

    public:

	    /**
	    * Returns query's ID
	    * @return query ID
	    */
	    TUint32 QueryId();

        /**
        * Creates the result set and sets message pointer
        * @param aMessage message for asynchronous client messaging
        */
        void CreateResultSet(RMessage2 aMessage);

        /**
        * Sets parameters for the find. Ownership is transferred.
        */
        void SetFindParams( TUint32 aNotifyCount );

        /**
        * Sets serialized criteria buffer for the find. Ownership is transferred.
        */
        void SetFindCriteria( CMdCSerializationBuffer* aSerializedCriteria );

	    /**
	    * Initiates find
	    * @param aUserLevel defines user's access rights
	    * @return error code or KErrNone
	    */
        TInt FindL( TUserLevel aUserLevel );

	    /**
	    * Continues ongoing sync find
        * @return error code or KErrNone
	    */
        TInt ContinueL();

	    /** 
	    * Initiates asynchronous find
        * @param aUserLevel defines user's access rights
	    */
        void FindAsyncL( TUserLevel aUserLevel );

	    /** 
	    * Continues ongoing find
	    */
	    void ContinueAsync();

	    /** 
	    * Cancels find
	    */
        void Cancel( TInt aError );

        /**
        * Transfer result set to client
        * @aError error code or KErrNone
        */
        void SetComplete( TInt aError, TBool aSet );

		const CMdCSerializationBuffer& ResultsL() const;

		/**
		 * Locks query result objects.
		 * @param aLockList list of objects that must be locked
		 */
		void LockFindResultObjectsL( CMdSObjectLockList& aLockList );

		/**
		 * Sets result mode.
		 * @param aResultModeItems ETrue if result mode is item mode
		 */
		void SetResultMode( TBool aResultModeItems );

	    /**
	    * Returns whether find engine is complete or not.
	    * Completed sequence can be safely delete.
	    */
		TBool IsComplete() const;

	    /**
	    * Returns whether find engine's query is complete or not.
	    */
		TBool IsQueryComplete();

		CMdSServer& GetServer() const;

    private: // Constructors and destructors

        /**
        * CMdSManipulationEngine.
        * C++ default constructor.
        * @param .
        */
        CMdSFindEngine(
        	TUint32 aQueryId, 
            CMdSServerSession& aParent,
            CMdSObjectLockList& aLockList);

    private: // Data
    
    	TUint32 iQueryId;

        CMdSServerSession& iParent;

        CMdSObjectLockList& iLockList;

        CMdSFindSequence* iSequence;

		CMdCSerializationBuffer* iSerializedCriteria;

        RMessage2 iMessage;

        TBool iAsyncPending;
        
        // if last query result was Items result
        TBool iLastResultModeItems;

    };

#endif //__MDSFINDENGINE_H__