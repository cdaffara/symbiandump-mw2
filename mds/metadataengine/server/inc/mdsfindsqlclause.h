/*
* Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Constructs SQL clauses for object find*
*/

#ifndef __MDSFINDSQLCLAUSE_H__
#define __MDSFINDSQLCLAUSE_H__

#include <e32base.h>
#include <badesca.h>

#include "mdccommon.h"
#include "mdssqliteconnection.h"

// FORWARD DECLARATIONS
class CMdCSerializationBuffer;
class CMdsSchema;
class CMdsNamespaceDef;
class CMdsObjectDef;
class CMdsClauseBuffer;
class CMdsPropertyDef;
class TMdCSearchCriteria;
class TMdCRelationCondition;

// CLASS DECLARATION
/**
* CMdSFindSqlClause.
* Class constructs SQL search clauses.
*/
class CMdSFindSqlClause : public CBase
    //public MMdSFindCriteria
    {
    private:
		enum TAppendIn
		{
			EAppendInFalse,
			EAppendInTrue,
			EAppendInRelationLeft,
			EAppendInRelationRight
		};

		enum TRangeValueType
		{
			EInt32RangeValue,
			EUint32RangeValue,
			EInt64RangeValue
		};
		
    public: // Constructors and destructor

        /**
        * NewL.
        * Two-phased constructor.
        * @param aSchema reference to metadata server's internal schema.
        * @return Pointer to created CMdSFindEngine object.
        */
        static CMdSFindSqlClause* NewL(CMdsSchema& aSchema);

        /**
        * NewLC.
        * Two-phased constructor.
        * @param aSchema reference to metadata server's internal schema.
        * @return Pointer to created CMdSManipulationEngine object.
        */
        static CMdSFindSqlClause* NewLC(CMdsSchema& aSchema);

        /**
        * Destructor.
        */
        virtual ~CMdSFindSqlClause();

    private: // Private constructors

        /**
        * CMdSSqlClause.
        * C++ default constructor.
        * @param aSchema reference to metadata server's internal schema.
        */
        CMdSFindSqlClause(CMdsSchema& aSchema);

        /**
        * ConstructL.
        * 2nd phase constructor.
        */
        void ConstructL();

    public: // Public functions

	    /**
	    * Constructs criteria the LONG way. ALL criteria and rule params
	    * are taken in account.
	    * @param aSerializedCriteria search criteria in serialized form.
	    * @param aUserLevel
	    */
        void CreateL( 
        	CMdCSerializationBuffer& aSerializedCriteria,
        	TUserLevel aUserLevel );

        /**
	    * Returns the decoded value as some kind of textual description
	    * @return search clause as TDesC8 descriptor
	    */
        const TDesC& AsTextL() const;

        /**
        * Returns constructed variable stack.
	    * @return clause variables.
        */
        RRowData& Variables();

        /**
        * Returns constructed result row stack.
	    * @return clause variables.
        */
        RRowData& ResultRow();

        /**
        * Returns limit count.
	    * @return limit.
        */
		TUint32 Limit();

		/**
		* Does SQL clause include freetext count and total length
		* @return SQL clause includes freetext count and total length
		*/
		TBool IncludesFreetexts();

        /**
        * Returns object query's object definition.
        * @return Object definition if query is object query else NULL.
        */
        CMdsObjectDef* ObjectDef();

        /**
        * Returns object query's property denitions used in property filter.
        * @return Object definition if query is object query else NULL.
        */
        RPointerArray<CMdsPropertyDef>& PropertyFilters();

        /**
        * Returns query's namespace definition.
        * @return Namespace definition if NULL query buffer is corrupted.
        */
        CMdsNamespaceDef* NamespaceDef();

        /**
        * Returns query type.
        * @return Query type
        */
        TQueryType QueryType();

        /**
        * Returns query result mode.
        * @return Query result mode
        */
        TQueryResultMode ResultMode();

        /**
         * Returns query freetext array
         * @return freetext array
         */
        RPointerArray<HBufC>& QueryFreeText();

        /**
         * Result objects are not locked
         * @return object locking status
         */
        TBool NoObjectLocking();
        
    private: // Private functions

        /**
        * appends an array of OR'ed aColumn=ID conditions.
        * @param aColumn A column of variable set where id is appended
        * @param aSet container of item ids
        */
        void AppendArrayL( /*const TDesC& aColumn, const RArray<TItemId>& aSet*/ );

		void AppendPropertyFiltersL( TBool aDistinct = EFalse );

        /**
        * Creates "SELECT column list FROM table name" SQL clause query
        * based on iType.
        */
        void AppendSelectStatementL();

        /**
        * Creates "SELECT count(*) FROM table name" SQL clause query
        * based on iType.
        */
        void AppendSelectCountStatementL();
        
        /**
        * Creates "SELECT * FROM table name" SQL clause query
        * based on iType.
        */
        void AppendSelectAllFromEventStatementL();
        
        void AppendSelectAllFromRelationStatementL();
        
        void AppendSelectEventItemStatementL();
        
        void AppendSelectRealtionItemStatementL();
        
        /**
        * Creates "SELECT DISTINCT "wanted property's column" FROM table name" 
        * SQL clause query based on iType.
        */
        void AppendSelectDistinctStatementL();

		void AppendSelectObjectItemStatementL();

		void AppendSelectObjectIdStatementL();
		
		void AppendFromForObjectL();

	    void AppendFromForEventL();
		
		void AppendFromForRelationL();
		
		/**
		* Creates "WHERE(...)"
		*/
		void AppendWhereStatementL();

        /**
        * Helper which creates "SELECT statement for object query.
        */
        void AppendSelectForObjectL();

        /**
        * Helper which creates "SELECT statement for relation query.
        * @param aRules find rules
        */
        void AppendSelectForRelationL( /*const TMdCFolder& aRules*/ );

        /**
        * Helper which creates "SELECT statement for event query.
        * @param aRules find rules
        */
        void AppendSelectForEventL( /*const TMdCFolder& aRules*/ );

        /**
        * Help method for where clause parsing and is used recursively.
        */
        void WhereStatementL();

		void ConditionStatementL(TBool aNegated = EFalse, TAppendIn aAppendIdIn = EAppendInTrue);

		void AppendObjectIdL();
		
		void AppendIdInL();

        /**
        * Help method for appending logic condition.
        * @param aNegated is condition negated
        */
		void AppendLogicConditionL(TBool aNegated, TAppendIn aAppendIdIn = EAppendInTrue);

        /**
        * Help method for appending object condition.
        * @param aNegated is condition negated
        */
		void AppendObjectConditionL(TBool aNegated);

		/**
		 * Help method for appending range.
		 * @param aRangeValueType type of range's values
		 */
		void AppendRangeL(TRangeValueType aRangeValueType);

        /**
        * Help method for appending property condition.
        *
        * @param aNegated is condition negated
        */
		void AppendPropertyConditionL(TBool aNegated);

		/** 
		 * Helper method for getting object definition for property condition.
		 * 
		 * @param aObjectDefId property condition's object definition
		 * 
		 * @return object definition for query
		 */
		TDefId ObjectDefForPropertyCondition(TDefId aObjectDefId);
		
        /**
        * Help method for appending property range condition.
        *
        * @param aNegated is condition negated
        */
		void AppendPropertyRangeConditionL(TBool aNegated);

        /**
        * Help method for appending property text condition.
        * @param aNegated is condition negated
        */
		void AppendPropertyTextConditionL(TBool aNegated);

        /**
        * Help method for appending property bool condition.
        * @param aNegated is condition negated
        */
		void AppendPropertyBoolConditionL(TBool aNegated);

        /**
        * Help method for appending event condition for object query.
        * @param aNegated is condition negated
        */
		void AppendEventConditionL(TBool aNegated);

        /**
        * Help method for appending relation condition for object query.
        * @param aNegated is condition negated
        */
		void AppendRelationConditionL(TBool aNegated);

		void AppendRelationSideConditionL(TBool aRelationConditions, 
				const TMdCRelationCondition& aRelationCondition, 
				TBool aLeftSide, TUint32 aRelationObjectConditionOffset);

        /**
        * Help method for searching object definition's table,
        * based on object definition's ID, and appending those to buffer.
        * @param aObjectDefId object definition's ID
        */
		void AppendTableByObjectDefIdL(TDefId aObjectDefId);

        /**
        * Help method for searching property definition's column,
        * based on property definition's ID, and appending those to buffer.
        * @param aPropertyDefId property definition's ID
        * @param aObjectDefId object definition's ID
        * @param aResult append property to expected result row
        */
		void AppendColumnByPropertyDefIdL(TDefId aPropertyDefId, TDefId aObjectDefId = KNoDefId, TBool aResult = EFalse);

        /**
        * Help method which appends GROUP BY statements.
        */
        void AppendGroupByL();

        /**
        * Help method which appends ORDER BY statements.
        */
        void AppendOrderByL();

        /**
        * Help method which appends LIMIT and OFFSET.
        */
        void AppendLimitAndOffsetL();

        /** 
        * Help method that copies the rightmost part of the variables to
        * RowData. The function does not cut or remove any data but doubles
        * a number of variables.
        * @param aStart index of the first variable to be copied.
        * @param aEnd index of the last variable to be copied.
        */
        void CopyVariablesL( TInt aStart, TInt aEnd );

    private: // Member data

        /**
        * variables used in the clause.
        * Note: RRowData constructor cannot leave, since it is initialized (aInStack==EFalse)
        */
        RRowData iVariables;

        /**
        * Expected result row from the clause.
        * Note: RRowData constructor cannot leave, since it is initialized (aInStack==EFalse)
        */
        RRowData iResultRow;

        /**
        * Text buffer for whole query clause. CMdsClauseBuffer class takes care
        * of memory allocation for buffer.
        */
        CMdsClauseBuffer* iQueryBuf;

		/**
		* Metadata Server's internal schema
		*/
		CMdsSchema& iSchema;

		/**
		* Serialized query critertia 
		*/
		TMdCSearchCriteria* iSearchCriteria;

		/**
		* Serialized query critertia 
		*/
		CMdCSerializationBuffer* iSerializedBuffer;       

		/**
		* Client application's user level
		*/
		TUserLevel iUserLevel;

		/**
		* Namespace definition where to search
		*/
		CMdsNamespaceDef* iNamespaceDef;

        /**
        * Object definition for where to (WHERE expression) search in object 
        * queries
        */
        CMdsObjectDef* iObjectDef;

        /**
         * Object definition for what to (SELECT result and FROM table list) 
         * search in object queries
         */
        CMdsObjectDef* iSourceObjectDef;

        /**
         * Object definitions IDs for what to (SELECT result and FROM table list) 
         * search in object queries
         */
        RArray<TDefId>* iSourceObjectDefs;
        
        /**
         * Append columns to result row (ETrue only for first ID from iSourceObjectDefs).
         */
        TBool iAppendToResultRow;
        
        /**
        * Descriptor which contains namespace definition's ID in text format
        */
        TBuf16<16> iNamespaceIdDes;                
        
        /**
        * Filter property definition references
        */
        RPointerArray<CMdsPropertyDef> iPropertyFilters;

		/**
		* Include freetexts to result.
		*/
		TBool iIncludeFreetexts;
		
		/**
		* Include not present items to result.
		*/
		TBool iIncludeNotPresent;

		/**
		 * Free text which belong to query.
		 */
		RPointerArray<HBufC> iFreeText;
		
		/**
		 * Result objects are not locked.
		 */
		TBool iNoObjectLocking;
		
		TBool iPlaceholdersOnly;
        };

#endif //__MDSFINDSQLCLAUSE_H__
