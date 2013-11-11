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


#include "mdenamespacedef.h"

#include "mdcdef.h"
#include "mdeobjectdef.h"
#include "mderelationdef.h"
#include "mdeeventdef.h"
#include "mdcserializationbuffer.h"


CMdENamespaceDef::CMdENamespaceDef( const CMdESession& aMdeSession, 
		const TMdCNamespaceDef& aNamespaceDef )
	: iMdeSession( aMdeSession ), iNamespaceDef( aNamespaceDef )
	{
	}

CMdENamespaceDef* CMdENamespaceDef::NewL( const CMdESession& aMdeSession, 
		const TMdCNamespaceDef& aNamespaceDef, 
		CMdCSerializationBuffer& aSchemaBuffer )
	{
    CMdENamespaceDef* def = CMdENamespaceDef::NewLC( aMdeSession, 
    		aNamespaceDef, aSchemaBuffer );
    CleanupStack::Pop( def );
    return def;
    }

CMdENamespaceDef* CMdENamespaceDef::NewLC( const CMdESession& aMdeSession, 
		const TMdCNamespaceDef& aNamespaceDef, 
		CMdCSerializationBuffer& aSchemaBuffer )
	{
    CMdENamespaceDef* def = new( ELeave ) CMdENamespaceDef( aMdeSession, 
    		aNamespaceDef );
    CleanupStack::PushL( def );
    def->ConstructL( aSchemaBuffer );
    return def;
	}

void CMdENamespaceDef::ConstructL(CMdCSerializationBuffer& aSchemaBuffer)
	{
	aSchemaBuffer.PositionL( iNamespaceDef.iName.iPtr.iOffset );
	iName.Set( aSchemaBuffer.ReceivePtr16L() );
	
	TUint32 count = iNamespaceDef.iObjectDefs.iPtr.iCount;
	TMdCOffset offset = iNamespaceDef.iObjectDefs.iPtr.iOffset;
	iObjectDefs.ReserveL( count );
	for ( TUint32 i = 0; i < count; ++i )
		{
		aSchemaBuffer.PositionL( offset + i * sizeof(TMdCObjectDef) );
		const TMdCObjectDef& objectDef = TMdCObjectDef::GetFromBufferL(aSchemaBuffer);
		iObjectDefs.AppendL( CMdEObjectDef::NewL( objectDef, aSchemaBuffer, *this ) );
		}

	count = iNamespaceDef.iEventDefs.iPtr.iCount;
	offset = iNamespaceDef.iEventDefs.iPtr.iOffset;
	iEventDefs.ReserveL( count );
	for ( TUint32 i = 0; i < count; ++i )
		{
		aSchemaBuffer.PositionL( offset + i * sizeof(TMdCEventDef) );
		const TMdCEventDef& eventDef = TMdCEventDef::GetFromBufferL(aSchemaBuffer);
		iEventDefs.AppendL( CMdEEventDef::NewL( eventDef, aSchemaBuffer, *this ) );
		}

	count = iNamespaceDef.iRelationDefs.iPtr.iCount;
	offset = iNamespaceDef.iRelationDefs.iPtr.iOffset;
	iRelationDefs.ReserveL( count );
	for ( TUint32 i = 0; i < count; ++i )
		{
		aSchemaBuffer.PositionL( offset + i * sizeof(TMdCRelationDef) );
		const TMdCRelationDef& relationDef = TMdCRelationDef::GetFromBufferL(aSchemaBuffer);
		iRelationDefs.AppendL( CMdERelationDef::NewL( relationDef, aSchemaBuffer, *this ) );
		}
	}

CMdENamespaceDef::~CMdENamespaceDef()
	{

	iObjectDefs.ResetAndDestroy();
	iObjectDefs.Close();

	iEventDefs.ResetAndDestroy();
	iEventDefs.Close();

	iRelationDefs.ResetAndDestroy();
	iRelationDefs.Close();
	}

TDefId CMdENamespaceDef::Id() const
	{
	return iNamespaceDef.iDefId;
	}

EXPORT_C TBool CMdENamespaceDef::ReadOnly() const
	{
	return iNamespaceDef.iReadOnly;
	}

EXPORT_C const TDesC& CMdENamespaceDef::Name() const
	{
	return iName;
	}

EXPORT_C TInt CMdENamespaceDef::Compare( const CMdENamespaceDef& aNamespaceDef ) const
	{
	return iNamespaceDef.iDefId - aNamespaceDef.Id();
	}

EXPORT_C TInt CMdENamespaceDef::ObjectDefCount() const
	{
	return iObjectDefs.Count();
	}

EXPORT_C CMdEObjectDef& CMdENamespaceDef::ObjectDefL(TInt aIndex)
	{
	return *iObjectDefs[aIndex];
	}

EXPORT_C CMdEObjectDef& CMdENamespaceDef::GetObjectDefL(const TDesC& aName)
	{
	const TInt objectDefsCount = iObjectDefs.Count();
	for ( TInt i = 0; i < objectDefsCount; ++i )
		{
		if ( !aName.Compare( iObjectDefs[i]->Name() ) )
			{
			return ObjectDefL( i );
			}
		}

	User::Leave( KErrNotFound );

	// to stop compile warnings
	CMdEObjectDef* objectDef = NULL;
	return *objectDef;
	}

CMdEObjectDef* CMdENamespaceDef::GetObjectDefL(TDefId aId)
	{
	const TInt objectDefsCount = iObjectDefs.Count();
	for ( TInt i = 0; i < objectDefsCount; ++i )
		{
		if ( aId == iObjectDefs[i]->Id() )
			{
			return &ObjectDefL( i );
			}
		}

	return NULL;
	}

EXPORT_C TInt CMdENamespaceDef::RelationDefCount() const
	{
	return iRelationDefs.Count();
	}

EXPORT_C CMdERelationDef& CMdENamespaceDef::RelationDefL(TInt aIndex)
	{
	return *iRelationDefs[aIndex];
	}

EXPORT_C CMdERelationDef& CMdENamespaceDef::GetRelationDefL(const TDesC& aName)
	{
	const TInt relationDefCount = iRelationDefs.Count();
	for ( TInt i = 0; i < relationDefCount; ++i )
		{
		if( !aName.Compare( iRelationDefs[i]->Name() ) )
			{
			return RelationDefL( i );
			}
		}

	User::Leave( KErrNotFound );

	// to stop compile warnings
	CMdERelationDef* relationDef = NULL;
	return *relationDef;
	}

CMdERelationDef* CMdENamespaceDef::GetRelationDefL(TDefId aId)
	{
	const TInt relationDefCount = iRelationDefs.Count();
	for ( TInt i = 0; i < relationDefCount; ++i )
		{
		if( iRelationDefs[i]->Id() == aId )
			{
			return &RelationDefL( i );
			}
		}

	return NULL;
	}

EXPORT_C TInt CMdENamespaceDef::EventDefCount() const
	{
	return iEventDefs.Count();
	}

EXPORT_C CMdEEventDef& CMdENamespaceDef::EventDefL(TInt aIndex)
	{
	return *iEventDefs[aIndex];
	}

EXPORT_C CMdEEventDef& CMdENamespaceDef::GetEventDefL(const TDesC& aName)
	{
	const TInt eventDefCount = iEventDefs.Count();
	for ( TInt i = 0; i < eventDefCount; ++i )
		{
		if( !aName.Compare( iEventDefs[i]->Name() ) )
			{
			return EventDefL( i );
			}
		}
	
	User::Leave( KErrNotFound );
	
	// to stop compile warnings
	CMdEEventDef* eventDef = NULL;
	return *eventDef;
	}

CMdEEventDef* CMdENamespaceDef::GetEventDefL(TDefId aId)
	{
	const TInt eventDefCount = iEventDefs.Count();
	for ( TInt i = 0; i < eventDefCount; ++i )
		{
		if( iEventDefs[i]->Id() == aId )
			{
			return &EventDefL( i );
			}
		}
	return NULL;
	}

const CMdESession& CMdENamespaceDef::Session() const
	{
	return iMdeSession;
	}
