/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Comparator algorithms for notifier*
*/

#ifndef __MDSNOTIFYCOMPARATOR_H__
#define __MDSNOTIFYCOMPARATOR_H__

#include <e32base.h>

#include "mdccommon.h"

class CMdCSerializationBuffer;
class TMdCRelation;
class TMdCRelationCondition;

/**
* A collection of algorithms for comparing events against
* criteria profile of notifier. The event either does not
* or does match the profile set by the given criteria.
*/
class CMdSNotifyComparator : public CBase
    {
    public: // public construction

        /** static construction */
        static CMdSNotifyComparator* NewL();
        static CMdSNotifyComparator* NewLC();

        /** destructor */
        virtual ~CMdSNotifyComparator();

    private: // private construction

        /** constructor */        
        CMdSNotifyComparator();

        /** 2nd phase constructor */
        void ConstructL();

    public: // methods

        /**
        * Matches items against the given condition
        * 
        * @param aType listener's type
        * @param aSerializedCondition serialized condition to be matched 
        *                             against
        * @param aSerializedItems the items to compare
        * @param aSerializedItemIds the item IDs (successful and failed IDs)
        * @param aMatchingItemIdArray matching object IDs
        * @param aAllItemsIdArray array to store the ids if all matched
        * @param aMatchingItemUriArray matching object URIs
        * @param aAllItemsUriArray array to store the uris if all matched
        * @param aAllMatched to determine if there is no specific conditions to match
        * @param aAllUrisFetched to determine if all uris have been previously fetched
        * 
        * @return If true at least some item matches.
        */
        TBool MatchL(TDefId aNamespaceId, TUint32 aType,
        		CMdCSerializationBuffer& aSerializedCondition,
        		CMdCSerializationBuffer& aSerializedItems,
		    	CMdCSerializationBuffer& aSerializedItemIds,
		    	RArray<TItemId>& aMatchingItemIdArray,
		    	RArray<TItemId>& aAllItemsIdArray,
		    	RPointerArray<HBufC>& aMatchingItemUriArray, 
		    	RPointerArray<HBufC>& aAllItemsUriArray,
		    	TBool aAllowConfidential,
		    	TBool uriNotify,
		    	TBool& aAllMatched,
		    	TBool& aAllUrisFetched);

		/**
        * Matches object against the given condition. This variant matches only
        * object conditions.
        * 
        * @param aSerializedCondition serialized condition to be matched 
        *                             against
        * @param aSerializedItem the object to compare (buffer's position is 
        *                        in the begin of object)
        * @param aObjectId object ID
        * @param aAllowConfidential allow confidential objects
        * 
        * @return Does object match with condition.
		*/
		TBool MatchObjectL(CMdCSerializationBuffer& aSerializedCondition,
		    	CMdCSerializationBuffer& aSerializedItem,
		    	TItemId aObjectId, TBool aAllowConfidential);

		/**
        * Matches event against the given condition. This variant matches only
        * event conditions.
        * 
        * @param aSerializedCondition serialized condition to be matched 
        *                             against
        * @param aSerializedItem the event to compare (buffer's position is in 
        *                        the begin of event)
        * @param aEventId event ID
        * 
        * @return Does event match with condition.
		*/
		TBool MatchEventL(CMdCSerializationBuffer& aSerializedCondition,
		    	CMdCSerializationBuffer& aSerializedItem, TItemId aEventId);

		/**
        * Matches relation against the given condition. This variant matches 
        * only relation conditions.
        * 
        * @param aSerializedCondition serialized condition to be matched 
        *                             against
        * @param aSerializedItem the relation to compare (buffer's position is 
        *                        in the begin of relation)
        * @param aRelationId relation ID
        * 
        * @return Does relation match with condition.
		*/
		TBool MatchRelationL(CMdCSerializationBuffer& aSerializedCondition,
		    	CMdCSerializationBuffer& aSerializedItem, TItemId aRelationId);

        /**
        * Matches object IDs against the given condition. This variant matches 
        * only object ID conditions.
        * 
        * @param aSerializedCondition serialized condition to be matched 
        *                             against
        * @param aItemIdArray the object IDs to compare
        * @param aMatchingItemIdArray matching object IDs
        * 
        * @return If true all objects matches and result is same as 
        *         aItemIdArray, so aMatchingItemIdArray might not contain any 
        *         IDs in that case.
        */
		TBool MatchObjectIdsL(CMdCSerializationBuffer& aSerializedCondition,
		    	const RArray<TItemId>& aItemIdArray,
		    	RArray<TItemId>& aMatchingItemIdArray);

		/**
	     * Matches relation items against the given condition.
	     * This variant matches only relation conditions.
	     * 
	     * @param aSerializedCondition serialized condition to be matched 
	     *                             against
	     * @param aSerializedItems the relation to compare (buffer's position 
	     *                         is in the begin of relation)
	     * @param aMatchingItemIdArray relation ID array
	     * 
	     * @return Does relation match with condition.
		 */
		TBool MatchRelationItemsL(CMdCSerializationBuffer& aSerializedCondition,
				CMdCSerializationBuffer& aSerializedItems,
			    RArray<TItemId>& aMatchingItemIdArray);

		/**
	     * Matches relation items against the given condition.
	     * This variant matches only relation conditions.
	     * 
	     * @param aSerializedCondition serialized condition to be matched 
	     *                             against
	     * @param aRelation relation
	     * 
	     * @return Does relation match with condition.
		 */
		TBool MatchRelationItemL(CMdCSerializationBuffer& aSerializedCondition,
				const TMdCRelation& aRelation);

    private:
        /**
        * Compare string begins against the given string
        * @param aDes contains reference to the string  
        * @param aCompareDes reference to the compared string
        * @return compare result
        */
	    TBool CompareDesBeginsWith( TDesC16& aDes, TDesC16& aCompareDes );

        /**
        * Compare string ends with the given string
        * @param aDes contains reference to the string  
        * @param aCompareDes reference to the compared string
        * @return compare result
        */
        TBool CompareDesEndsWith( TDesC16& aDes, TDesC16& aCompareDes );

        /**
        * Finds string with the given string
        * @param aDes contains reference to the string  
        * @param aCompareDes reference to the searched string
        * @return compare result
        */
 		TBool FindDes( TDesC16& aDes, TDesC16& aFindDes );

		/**
         * Matches object ID against the given logic condition which contains 
         * object ID conditions. Caller must set condition buffer to begin of 
         * the logic condition.
         * 
         * @param aSerializedCondition serialized logic condition to be 
         *                             matched against
         * @param aObjectId object ID
         * 
         * @return Does relation match with condition.
		 */
		TBool MatchObjectIdToObjectIdConditionsL(
				CMdCSerializationBuffer& aSerializedCondition, 
				TItemId aObjectId);

		/**
         * Matches object ID against the given logic condition which contains 
         * object ID conditions. Caller must set condition buffer to begin of 
         * the logic condition.
         * 
         * @param aSerializedCondition serialized logic condition to be 
         *                             matched against
         * @param aSerializedItem serializad object
         * @param aAllowConfidential is confidential objects allowed
         * 
         * @return Does object match with condition.
		 */
        TBool CheckObjectL( CMdCSerializationBuffer& aSerializedCondition, 
        		CMdCSerializationBuffer& aSerializedItem,
        		TBool aAllowConfidential );
		/**
         * Matches object's property against the given logic condition which 
         * contains object ID conditions.
         * 
         * @param aSerializedCondition serialized logic condition to be 
         *                             matched against
         * @param aSerializedItem serializad object
         * 
         * @return Does object's propertys match with condition.
		 */
        TBool CheckPropertyL( CMdCSerializationBuffer& aSerializedCondition,
    			CMdCSerializationBuffer& aSerializedItem );

        /**
         * Matches relation IDs in given condition against given one.
         * 
         * @param aRelationCondition relation condition
         * @param aSerializedCondition serialized ids to be matched against
         * @param aRelationId the relation IDs to compare
         * 
         * @return ETrue if relation match
         */
        TBool MatchRelationIdsL(
        		const TMdCRelationCondition& aRelationCondition, 
        		CMdCSerializationBuffer& aSerializedCondition,
        		TItemId aRelationId);

        inline TBool BoolEqual( TBool aLeft, TBool aRight )
        	{
        	return (aLeft && aRight || !aLeft && !aRight);
        	}
    };

#endif	// __MDSNOTIFYCOMPARATOR_H__
