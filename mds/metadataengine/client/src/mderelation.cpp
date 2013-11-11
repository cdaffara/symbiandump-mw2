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


#include "mderelation.h"

#include "mdcitem.h"
#include "mdcserializationbuffer.h"
#include "mderelationdef.h"
#include "mdenamespacedef.h"


//----------------------------------------------------------
//               TMdERelation
//----------------------------------------------------------

TMdERelation::TMdERelation()
	: iId( KNoId ), iLeftObjectId( KNoId ),
	  iRightObjectId( KNoId ), iDef( NULL )
	{
	}

EXPORT_C TItemId TMdERelation::Id() const
	{
	return iId;
	}

EXPORT_C CMdERelationDef& TMdERelation::DefL() const
	{
	if ( !iDef )
		{
		User::Leave( KErrNotFound );
		}
	return *iDef;
	}

EXPORT_C TItemId TMdERelation::LeftObjectId() const
	{
	return iLeftObjectId;
	}

EXPORT_C TItemId TMdERelation::RightObjectId() const
	{
	return iRightObjectId;
	}

TUint32 TMdERelation::RequiredBufferSize() const
	{
	return sizeof(TMdCRelation);
	}

void TMdERelation::DeSerializeL(CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef)
	{
	const TMdCRelation& relation = TMdCRelation::GetFromBufferL( aBuffer );
	iId = relation.iId;
	iLeftObjectId = relation.iLeftObjectId;
	iRightObjectId = relation.iRightObjectId;
	if (iId == KNoId || iLeftObjectId == KNoId || iRightObjectId == KNoId)
		{
		User::Leave(KErrNotFound);
		}
	if(iLeftObjectId == iRightObjectId )
		{
		User::Leave( KErrArgument );
		}
	iDef = aNamespaceDef.GetRelationDefL( relation.iDefId );
	}

//----------------------------------------------------------
//               CMdERelation
//----------------------------------------------------------

CMdERelation::CMdERelation(CMdESession* aSession, TItemId aId, 
						   CMdERelationDef& aDef, TItemId aLeftObjectId, 
						   TItemId aRightObjectId, TInt32 aParameter)
		: CMdEInstanceItem(aSession, aId), iDef(aDef),
		  iLeftObjectId(aLeftObjectId), iRightObjectId(aRightObjectId), 
		  iParameter(aParameter), iGuidHigh( 0 ), iGuidLow( 0 ), iLastModifiedDate( 0 )
	{
	}


void CMdERelation::ConstructL()
	{
	// Same object cannot be on both side of the relation
	if(iLeftObjectId == iRightObjectId )
		{
		User::Leave( KErrArgument );
		}

	InstanceItemBaseConstruct();
	}


CMdERelation::~CMdERelation()
	{
	}

CMdERelation* CMdERelation::NewL(CMdERelationDef& aDef, TItemId aLeftObjectId, 
		 TItemId aRightObjectId, TInt32 aParameter)
	{
	CMdERelation* ret = CMdERelation::NewLC( aDef, aLeftObjectId, aRightObjectId, aParameter );
	CleanupStack::Pop( ret );
	return ret;
	}

CMdERelation* CMdERelation::NewLC(CMdERelationDef& aDef, TItemId aLeftObjectId, 
		 TItemId aRightObjectId, TInt32 aParameter)
	{
	CMdERelation* ret = new (ELeave) CMdERelation( NULL, KNoId, aDef, aLeftObjectId, aRightObjectId, aParameter );
	CleanupStack::PushL( ret );
	ret->ConstructL();
	return ret;
	}

CMdERelation* CMdERelation::NewL(CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef)
	{
	CMdERelation* ret = CMdERelation::NewLC( aSession, aBuffer, aNamespaceDef );
	CleanupStack::Pop( ret );
	return ret;
	}

CMdERelation* CMdERelation::NewLC(CMdESession* aSession, CMdCSerializationBuffer& aBuffer, CMdENamespaceDef& aNamespaceDef)
	{
	const TMdCRelation& serializedRelation = TMdCRelation::GetFromBufferL( aBuffer );
	if (serializedRelation.iId == KNoId)
		{
		User::Leave( KErrNotFound );
		}
	if (serializedRelation.iDefId == KNoDefId)
		{
		User::Leave( KErrNotFound );
		}
	CMdERelationDef* relationDef = aNamespaceDef.GetRelationDefL( serializedRelation.iDefId );
	if ( !relationDef )
		{
		User::Leave( KErrNotFound );
		}
	if (serializedRelation.iLeftObjectId == KNoId)
		{
		User::Leave( KErrNotFound );
		}
	if (serializedRelation.iRightObjectId == KNoId)
		{
		User::Leave( KErrNotFound );
		}

	CMdERelation* relation = new (ELeave) CMdERelation( aSession, serializedRelation.iId, *relationDef,
			serializedRelation.iLeftObjectId, serializedRelation.iRightObjectId, serializedRelation.iParameter );
	CleanupStack::PushL( relation );

	relation->SetGuid( serializedRelation.iGuidHigh, serializedRelation.iGuidLow );

	relation->SetLastModifiedDate( serializedRelation.iLastModifiedDate );

	return relation;
	}

EXPORT_C CMdERelationDef& CMdERelation::Def() const
	{
	return iDef;
	}


EXPORT_C TItemId CMdERelation::LeftObjectId() const
	{
	return iLeftObjectId;
	}


EXPORT_C TItemId CMdERelation::RightObjectId() const
	{
	return iRightObjectId;
	}
	
EXPORT_C TInt32 CMdERelation::Parameter() const
	{
	return iParameter;
	}

EXPORT_C void CMdERelation::SetParameter(TInt32 aParameter)
	{
	iParameter = aParameter;
	}
	
EXPORT_C void CMdERelation::SetLeftObjectIdL( TItemId aLeftObjectId )
	{
	// Same object cannot be on both side of the relation
	if( aLeftObjectId == iRightObjectId )
		{
		User::Leave( KErrArgument );
		}

	iLeftObjectId = aLeftObjectId;
	}

EXPORT_C void CMdERelation::SetRightObjectIdL( TItemId aRightObjectId )
	{
	// Same object cannot be on both side of the relation
	if( aRightObjectId == iLeftObjectId )
		{
		User::Leave( KErrArgument );
		}

	iRightObjectId = aRightObjectId;
	}

EXPORT_C void CMdERelation::Guid(TInt64& aGuidHigh, TInt64& aGuidLow) const
	{
	aGuidHigh = iGuidHigh;
	aGuidLow = iGuidLow;
	}

EXPORT_C void CMdERelation::SetGuid(const TInt64& aGuidHigh, const TInt64& aGuidLow)
	{
	iGuidHigh = aGuidHigh;
	iGuidLow = aGuidLow;
	}

EXPORT_C TTime CMdERelation::LastModifiedDate() const
	{
	return iLastModifiedDate;
	}

EXPORT_C void CMdERelation::SetLastModifiedDate(TTime aLastModifiedDate)
	{
	iLastModifiedDate = aLastModifiedDate;
	}

TUint32 CMdERelation::RequiredBufferSize() const
	{
	return sizeof(TMdCRelation);
	}

TMdCOffset CMdERelation::SerializeL(CMdCSerializationBuffer& aBuffer, TMdCOffset aFreespaceOffset) const
	{
	TMdCRelation relation;
	relation.iId = Id();
	relation.iDefId = iDef.Id();
	relation.iLeftObjectId = iLeftObjectId;
	relation.iRightObjectId = iRightObjectId;
	relation.iParameter = iParameter;
	relation.iGuidHigh = iGuidHigh;
	relation.iGuidLow = iGuidLow;
	relation.iLastModifiedDate = iLastModifiedDate;
	
	relation.SerializeL( aBuffer );
	return aFreespaceOffset;
	}

TMdEInstanceType CMdERelation::InstanceType() const
	{
	return EMdETypeRelation;
	}
