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
* Description:  Manages object search from database using SQL,*
*/

#ifndef __MDSSQLFINDOPERATION_H__
#define __MDSSQLFINDOPERATION_H__

#include <e32base.h>

#include "mdccommon.h"
#include "mdssqliteconnection.h"

// FORWARD DECLARATIONS
class CMdSFindSqlClause;
class CMdSFindSequence;
class CMdCSerializationBuffer;
class CDesC16Array;
class RRowData;


/**
* Find operation class represents a single synchronous
* or asynchronous find.
*/
class CMdSSqlFindOperation: public CBase
    {
    public:
	    /** state of operation */
	    enum TOperationState
	    	{
	        EStateIdle,     // not working
	        EStateRunning,  // active state
	        EStateStop,     // stop instructed
	        EStateDead,     // find stopped
	        };

        /**
        * constructs a find operation        
        * @param aFind the master class that runs this operation        
        * @param aSetSize maximum size of a single result set
        */
        static CMdSSqlFindOperation* NewL(            
            CMdSFindSequence& aFind,            
            TUint aSetSize
             );

        /**
        * constructs a find operation, leaves in cleanup stack.
        */
        static CMdSSqlFindOperation* NewLC(            
            CMdSFindSequence& aFind,            
            TUint aSetSize
             );

        virtual ~CMdSSqlFindOperation();

        /**
        * from MMdSFindOperation
        */
        TInt ExecuteL();

        /**
        * from MMdSFindOperation
        */
        TInt ContinueL();

        /**
        * from MMdSFindOperation
        */
        TInt State();

        /**
        * from MMdSFindOperation
        */
        void Cancel();

        /**
        * from MMdSFindOperation
        */
        CMdSFindSqlClause& FindCriteria();

        /**
        * Moves ownership of the result buffer to caller
        */
        CMdCSerializationBuffer* Results();

		void SetLimit(TUint32 aLimit);

		void SetOffset(TUint32 aOffset);

        /**
         * Returns query freetext array from CMdSFindSqlClause
         * @return freetext array
         */
        RPointerArray<HBufC>& QueryFreeText();

    private: // private construction for friends only

        /**
        * Construction
        */
        CMdSSqlFindOperation(            
            CMdSFindSequence& aFind,
            TUint aSetSize = KMaxTUint
             );

        /** 2nd phase constructor */
        void ConstructL();

    private: // Private functions

        /**
        * iterates results from database,
        * places them in results container
        * @return error code, KErrNone or KSetReady
        */
        TInt FetchResultsL();

		TInt EstimateBaseResultSize();

		TInt EstimateResultRowSizeL( RRowData& aRow );

        /**
        * Gets query results from the rowbuffer, creates
        * corresponding object structure and adds into the
        * results container.
        * @param aDb database connection
        * @param aRow database row to add into container
        * @return true, if data was added to container
        */
	    void AddToContainerL();

        /**
        * Special treatment for Object, which is complicated
        * because of heavy optimization
        * @param aRow database row to add into container
        * @param aFreespaceOffset offset to free space in buffer
        */
	    TMdCOffset AddObjectToContainerL( RRowData& aRow, TMdCOffset aFreespaceOffset );

        /**
        * Marks the current database rows consumed
        */
        void ConsumeRows();
        
        /**
        * Add data to container 
        * @param aRow database row to add into container
        * @param aFreespaceOffset offset to free space in buffer
        */
        TMdCOffset AddItemToContainerL( RRowData &aRow, TMdCOffset aFreespaceOffset );
    
        /**
        * Add data to container 
        * @param aId item ID to add into container
        */
        void AddIdToContainerL( TItemId aId );
    
        /**
        * Creates Item for serialization
        */
        void CreateItemL();
    
        /**
        * Creates count for serialization
        */
        void CreateCountL();
    
        /**
        * Creates Id for serialization
        */
        void CreateIdL();
                
        /**
        * Creates distinct properties for serialization
        */
        void CreateDistinctL();
    
    private: // Member data
    
        /** Find type */
        TQueryType iQueryType;

		/** Result mode */
		TQueryResultMode iResultMode;

        /**
        * Data row in find.
        * Note: RRowData constructor cannot leave, since it is initialized (aInStack==EFalse)
        */
		RRowData* iResultRow;

        /**
        * Fetched result rows. Used when result mode is items or distinct.
        * Note: RRowData constructor cannot leave, since it is initialized (aInStack==EFalse)
        */
        RPointerArray<RRowData> iResultRows;

        /**
        * Fetched result IDs. Used when result mode is IDs.
        */
        RArray<TItemId> iResultIds;

        /**
        * Fetched result count. Used when result mode is count.
        */
        TUint32 iResultCount;

        /** container for result objects */
        CMdCSerializationBuffer* iResults;

        /** the object find back reference */
        CMdSFindSequence& iFind;

        /** the find SQL clause */
        CMdSFindSqlClause* iFindClause;

        /** asynchronous run state */
        TOperationState iState;

        /** the query ID */
        RMdsStatement iQueryId;

        /** maximum size of a single set */
        const TUint32 iSetSize;

        /** counter for results in this set */
        TUint iSetCounter;

        /** maximum size of the whole search */
        TUint iLimit;

        /** maximum size of the whole search */
        TUint iLimitCounter;

        /** offset from beginning of the sequence */
        TUint iOffset;

        /** used memory from set buffer */
        TInt iMemoryLimit;
        
        /** harvesting prioritization URI count */
        TInt iHarvestingPrioritizationCount;
    };

#endif // __MDSSQLFINDOPERATION_H__
