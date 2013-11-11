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
* Description:  Manages object search from database using SQL,
*
*/

#ifndef __MDSFINDSEQUENCE_H__
#define __MDSFINDSEQUENCE_H__

#include <e32base.h>
#include <badesca.h>

#include "mdscommoninternal.h"

// FORWARD DECLARATIONS
class CMdsSchema;
class CMdSServer;
class CMdSSqLiteConnection;
class CMdSFindEngine;
class CMdSSqlFindOperation;
class CMdCSerializationBuffer;

// CONSTANTS
const TInt KFindSetReady = 1;

/**
* CMdSFindSequence.
* Handles on sequence of find.
* A sequence is a logical operation, which may consist
* of several parallel physical operations from different
* physical entities (tables)
*/
class CMdSFindSequence : public CActive
    {
    public: // Constructors and destructor

        /**
        * NewL.
        * Two-phased constructor.
        * @param aServer reference to server
        * @param aSchema the schema
        * @param aObserver reference to find engine observer
        * @return Pointer to created CMdSManipulationEngine object.
        */
        static CMdSFindSequence* NewL( 
        	CMdSServer& aServer, CMdsSchema& aSchema, CMdSFindEngine& aObserver );

        /**
        * NewLC.
        * Two-phased constructor.
        * @param aServer reference to server
        * @param aSchema the schema
        * @param aObserver reference to find engine observer
        * @return Pointer to created CMdSManipulationEngine object.
        */
        static CMdSFindSequence* NewLC( 
        	CMdSServer& aServer, CMdsSchema& aSchema, CMdSFindEngine& aObserver );

        /**
        * Destructor.
        */
        virtual ~CMdSFindSequence();

    private: // Private constructors

        /**
        * 2nd phase constructor
        */
        void ConstructL();

        /**
        * C++ default constructor.
        * @param aServer reference to server
        * @param aSchema the schema
        * @param aObserve reference to find engine observer
        */
        CMdSFindSequence( 
        	CMdSServer& aServer, CMdsSchema& aSchema, CMdSFindEngine& aObserve );

    public:

		void SetFindParams( TUint32 aNotifyCount );

	    /**
	    * Finds objects synchronously
	    * @param aConnection database connection to use in find
	    * @param aSerializedCriteria serialized criteria buffer
	    * @param aUserLevel client's access level
        * @return KErrNone or KSetReady
	    */
        TInt FindL( CMdCSerializationBuffer& aSerializedCriteria,
            TUserLevel aUserLevel );

	    /**
	    * Continues ongoing synchronous find with the next set
	    * @param aConnection database connection to use in find
        * @return KErrNone or KSetReady
	    */
        TInt ContinueL();

	    /**
	    * Start Find objects asynchronously
	    * @param aSerializedCriteria serialized criteria buffer
	    */
        void FindAsync( CMdCSerializationBuffer& aSerializedCriteria,
            TUserLevel aUserLevel );

	    /**
	    * Continues ongoing asynchronous find with the next set
	    */
	    void ContinueAsync();

	    /**
	    * Returns whether find sequence is complete or not.
	    * Completed sequence can be safely delete.
	    */
		TBool IsComplete() const;

	    /**
	    * Returns whether find sequence's query is complete or not.
	    */
		TBool IsQueryComplete() const;

    public: // methods called by CMdSFindOperation

        /** access to the schema */
        const CMdsSchema& Schema() const;

        /** access to the server */
        CMdSServer& Server() const;

        /** access to the result buffer */
        CMdCSerializationBuffer& ResultsL() const;

		void SetResultMode( TBool aResultModeItems );

    protected: // From CActive

        /**
        * Handles an active object’s request completion event.
        * CMdEFindSequence calls this methot by itself.
        */
        void RunL();

        /**
        * Handles a leave occurring in the RunL(). The active
        * scheduler calls this function RunL() function leaves.
        * @param aError leave code
        * @return KErrNone. Returning any other value results
        *  in the active scheduler function CActiveScheduler::Error()
        *  being called.
        */
        TInt RunError( TInt aError );

	    /**
	    * Implementation of asynchronous loop canceling.
        * calls Cancel method in FindOperation and then
        * ExecuteL (or ContinueL) should finish pretty soon.
	    */
        void DoCancel();

    protected: // Find sequence's new methods
        /**
        * creates the find operation
        * @param aResults find results container
        * @param aCriteria find criteria
        * @param aRules find rules
        * @param aConnection the database connection reserved for the find
        * @return the created operation
        */    
        CMdSSqlFindOperation* CreateOperationLC(
            CMdCSerializationBuffer& aSerializedCriteria );

        /**
        * creates the find operation
        */
        CMdSSqlFindOperation* CreateOperationL(
            CMdCSerializationBuffer& aSerializedCriteria );

		/**
		 * Helper function for PostProcessL
		 * return freetext which belongs to object(id)
		 */
		void GetFreeTextForObjectL( CDesCArray& aResultWordBuffer,
				TDefId aNamespaceDefId, TItemId aObjectId );


        /**
        * Post-processing of found objects.
        * This routine may launch sub-operations based on the results.
        * @param aSerializedResultBuffer found objects serialized in buffer
        */
        void PostProcessL( CMdCSerializationBuffer& aSerializedResultBuffer );

        /**
        * Deletes iFindOperation and iFindResults.
        */
        void CleanUp();

	private:

		/**
		* Sort objects by free text hit count if no text search plug-in found
		* @param aFreeTextArray array which contains all free texts in criteria 
		* @param aObjGroup found object by criterias
		*/
		TUint32 GetFreeTextHitCountL(const CDesCArray& aObjectFreeText, 
				const RPointerArray<HBufC>& aSearchFreeText);

    private:

    	/**
    	 * server
    	 */
    	CMdSServer& iServer;

        /**
         * the schema 
         */
        CMdsSchema& iSchema;

		TUint32 iNotifyCount;

        /**
        * the find results
        */
        CMdCSerializationBuffer* iFindResults;
        
        /**
        * Serialized criteria buffer. Not owned by CMdSFindSequence.
        */        
        CMdCSerializationBuffer* iSerializedCriteria;

        /**
        * asynchronous operation observer
        */
        CMdSFindEngine* iObserver;

        /**
        * Database connection for asynchronous use
        */
        CMdSSqlFindOperation* iFindOperation;

		TDesC8* iFindSqlClauseBuffer;

        // if last query result was Items result
        TBool iLastResultModeItems;

        // client application's user level
        TUserLevel iUserLevel;
	};


#endif //__MDSFINDSEQUENCE_H__

