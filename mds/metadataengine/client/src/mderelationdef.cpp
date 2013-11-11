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
* Description:  
*
*/

#include "mderelationdef.h"

#include "mdcdef.h"
#include "mdenamespacedef.h"
#include "mdcserializationbuffer.h"


CMdERelationDef::CMdERelationDef( const TMdCRelationDef& aRelationDef, CMdENamespaceDef& aNamespaceDef )
	: iRelationDef(aRelationDef), iNamespaceDef( aNamespaceDef )
	{
	}

CMdERelationDef* CMdERelationDef::NewL( const TMdCRelationDef& aRelationDef, 
    CMdCSerializationBuffer& aSchemaBuffer, CMdENamespaceDef& aNamespaceDef )
	{
    CMdERelationDef* def = CMdERelationDef::NewLC( aRelationDef, aSchemaBuffer, aNamespaceDef );
    CleanupStack::Pop( def );
    return def;
    }

CMdERelationDef* CMdERelationDef::NewLC( const TMdCRelationDef& aRelationDef, 
    CMdCSerializationBuffer& aSchemaBuffer, CMdENamespaceDef& aNamespaceDef )
	{
    CMdERelationDef* def = new( ELeave ) CMdERelationDef( aRelationDef, aNamespaceDef);
    CleanupStack::PushL( def );
    def->ConstructL( aSchemaBuffer );
    return def;
	}

void CMdERelationDef::ConstructL( CMdCSerializationBuffer& aSchemaBuffer )
	{
	aSchemaBuffer.PositionL( iRelationDef.iName.iPtr.iOffset );
	iName.Set( aSchemaBuffer.ReceivePtr16L() );
	}

CMdERelationDef::~CMdERelationDef()
	{
	}

TDefId CMdERelationDef::Id() const
	{
	return iRelationDef.iDefId;
	}
	
EXPORT_C CMdENamespaceDef& CMdERelationDef::NamespaceDef() const
	{
	return iNamespaceDef;
	}

EXPORT_C const TDesC& CMdERelationDef::Name() const
	{
	return iName;
	}

EXPORT_C TInt CMdERelationDef::Compare( const CMdERelationDef& aRelationDef ) const
	{
	TInt result = iNamespaceDef.Compare( aRelationDef.iNamespaceDef );
	if ( result == 0 )
		{
		result = iRelationDef.iDefId - aRelationDef.Id();
		}
	return result;
	}
