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

#include "mdeeventdef.h"

#include "mdcdef.h"
#include "mdenamespacedef.h"
#include "mdcserializationbuffer.h"


CMdEEventDef::CMdEEventDef( const TMdCEventDef& aEventDef, CMdENamespaceDef& aNamespaceDef )
    : iEventDef(aEventDef), iNamespaceDef(aNamespaceDef)
    {
    }

CMdEEventDef* CMdEEventDef::NewL( const TMdCEventDef& aEventDef,
		CMdCSerializationBuffer& aSchemaBuffer, CMdENamespaceDef& aNamespaceDef )
    {
    CMdEEventDef* def = CMdEEventDef::NewLC( aEventDef, aSchemaBuffer, aNamespaceDef );
    CleanupStack::Pop( def );
    return def;
    }

CMdEEventDef* CMdEEventDef::NewLC( const TMdCEventDef& aEventDef,
		CMdCSerializationBuffer& aSchemaBuffer, CMdENamespaceDef& aNamespaceDef )
    {
    CMdEEventDef* def = new( ELeave ) CMdEEventDef( aEventDef, aNamespaceDef );
    CleanupStack::PushL( def );
    def->ConstructL( aSchemaBuffer );
    return def;
    }

void CMdEEventDef::ConstructL( CMdCSerializationBuffer& aSchemaBuffer )
    {
	aSchemaBuffer.PositionL( iEventDef.iName.iPtr.iOffset );
	iName.Set( aSchemaBuffer.ReceivePtr16L() );
    }

CMdEEventDef::~CMdEEventDef()
    {
    }

TDefId CMdEEventDef::Id() const
    {
    return iEventDef.iDefId;
    }

EXPORT_C CMdENamespaceDef& CMdEEventDef::NamespaceDef() const
    {
    return iNamespaceDef;
    }

EXPORT_C const TDesC& CMdEEventDef::Name() const
    {
    return iName;
    }
    
EXPORT_C TInt CMdEEventDef::Compare( const CMdEEventDef& aEventDef ) const
    {
    TInt result = iNamespaceDef.Compare( aEventDef.iNamespaceDef );
    if ( result == 0 )
        {
        result = iEventDef.iDefId - aEventDef.Id();
        }
    return result;
    }
