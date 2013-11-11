/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  ?Description
*
*/


#ifndef __MDERELATION_H__
#define __MDERELATION_H__


#include <mdeinstanceitem.h>


class CMdERelationDef;
class CMdCSerializationBuffer;
class CMdENamespaceDef;


NONSHARABLE_CLASS(TMdERelation)
	{
	public:

		TMdERelation();
	
		IMPORT_C TItemId Id() const;
	
		/**
		 * Returns the definition of the relation.
		 *
		 * @return  relation definition
		 */
		IMPORT_C CMdERelationDef& DefL() const;
	
		/**
		 * Returns the identifier of the left object of the relation.
		 *
		 * @return  object identifier
		 */
		IMPORT_C TItemId LeftObjectId() const;
	
		/**
		 * Returns the identifier of the right object of the relation.
		 *
		 * @return  object identifier
		 */
		IMPORT_C TItemId RightObjectId() const;

	public:
		/**
		 * Calculates required size for buffer to serialize relation
		 *
		 * @return necessary buffer size
		 */
		TUint32 RequiredBufferSize() const;
		
		/**
		 * DeSerialize relation
		 *
		 * @param aBuffer buffer where is serialized relation
		 */
		void DeSerializeL(CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef);

	private:

		TItemId iId;

		TItemId iLeftObjectId;
		TItemId iRightObjectId;

		/** Relation definition. */
		CMdERelationDef* iDef;
	};

/**
 * Relation between two objects.
 */
NONSHARABLE_CLASS(CMdERelation) : public CMdEInstanceItem
	{
public:
	
	/* Constructors and destructor. */

	/**
	 * Create new realation from serialized buffer
	 *
	 * @param aBuffer serialized relation
	 * @return new relation
	 */
	static CMdERelation* NewL(CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef);

	/**
	 * Create new realation from serialized buffer and leaves it on stack
	 *
	 * @param aBuffer serialized relation
	 * @return new relation
	 */
	static CMdERelation* NewLC(CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef);


	static CMdERelation* NewL(CMdERelationDef& aDef, TItemId aLeftObjectId, 
			 TItemId aRightObjectId, TInt32 aParameter);
	
	static CMdERelation* NewLC(CMdERelationDef& aDef, TItemId aLeftObjectId, 
			 TItemId aRightObjectId, TInt32 aParameter);
	
	/**
	 * Destructor.
	 */
	virtual ~CMdERelation();


	/* Methods. */

	/**
	 * Returns the definition of the relation.
	 *
	 * @return  relation definition
	 */
	IMPORT_C CMdERelationDef& Def() const;

	/**
	 * Returns the identifier of the left object of the relation.
	 *
	 * @return  object identifier
	 */
	IMPORT_C TItemId LeftObjectId() const;

	/**
	 * Returns the identifier of the right object of the relation.
	 *
	 * @return  object identifier
	 */
	IMPORT_C TItemId RightObjectId() const;

	/**
	 * Returns the parameter value associated with this relation
	 *
	 * @return The parameter value
	 */
	IMPORT_C TInt32 Parameter() const;
	
	/**
	 * Set the parameter.
	 * @param aParameter new value for parameter
	 */
	IMPORT_C void SetParameter(TInt32 aParameter);

	/**
	 * Sets left object ID.
	 * @param aLeftObjectId left object ID
	 */
	IMPORT_C void SetLeftObjectIdL( TItemId aLeftObjectId );

	/**
	 * Sets right object ID.
	 * @param aRightObjectId right object ID
	 */
	IMPORT_C void SetRightObjectIdL( TItemId aRightObjectId );

	/**
	 * Returns the GUID of the relation.
	 *
	 * @param aGuidHigh reference where GuidHigh will be stored
	 * @param aGuidLow reference where GuidLow will be stored
	 */
	IMPORT_C void Guid(TInt64& aGuidHigh, TInt64& aGuidLow) const;

	/**
	 * Sets the GUID of the relation.
	 *
	 * @param aGuidHigh new GuidHigh value
	 * @param aGuidLow new GuidLow value
	 */
	IMPORT_C void SetGuid(const TInt64& aGuidHigh, const TInt64& aGuidLow);

	/**
	 * Returns last modified date of the relation.
	 *
	 * @return last modified date
	 */
	IMPORT_C TTime LastModifiedDate() const;

	/**
	 * Sets last modified date of the relation.
	 *
	 * @param aLastModifiedDate last modified date
	 */
	IMPORT_C void SetLastModifiedDate(TTime aLastModifiedDate);

	/**
	 * Calculates required size for buffer to serialize relation
	 *
	 * @return necessary buffer size
	 */
	TUint32 RequiredBufferSize() const;

	/**
	 * Serialize relation
	 *
	 * @param aBuffer buffer where serialized relation should go
	 */
	TMdCOffset SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreespaceOffset) const;
	
protected:

	/**
	 * Constructs a new relation.
	 *
	 * @param aDef            definition of the new relation
	 * @param aLeftObjectId   identifier of the left object of the new relation
	 * @param aRightObjectId  identifier of the right object of the new relation
	 * @param aParameter value for parameter
	 * 
	 */
	CMdERelation(CMdESession* aSession, TItemId aId, 
			     CMdERelationDef& aDef, TItemId aLeftObjectId, 
				 TItemId aRightObjectId, TInt32 aParameter);
	
	/**
	 * Second-phase constructor.
	 */
	void ConstructL();

	TMdEInstanceType InstanceType() const;

	
private:

	/** Relation definition. */
	CMdERelationDef& iDef;

	/** Left object ID. */
	TItemId iLeftObjectId;

	/** Right object ID. */
	TItemId iRightObjectId;
	
	/** Parameter value */
	TInt32 iParameter;
	
	/** Guid High and Low */
	TInt64 iGuidHigh;
	TInt64 iGuidLow;
	
	/** Relations last modified date */
	TTime iLastModifiedDate;
	};

#endif  // __MDERELATION_H__
