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


#include "mdeorderrule.h"
#include "mdepanic.h"
#include "mdepropertydef.h"
#include "mdcserializationbuffer.h"
#include "mdcquery.h"


EXPORT_C TMdEOrderRule::TMdEOrderRule(TOrderRuleType aType, TBool aAscending)
        : iType(aType), 
          iAscending(aAscending),
          iCaseSensitive(ETrue)
    {
    }
        

EXPORT_C TMdEOrderRule::TMdEOrderRule(const CMdEPropertyDef& aPropertyDef, TBool aAscending)
        : iType(EOrderRuleTypeProperty), iPropertyDef(&aPropertyDef), 
          iAscending(aAscending), iCaseSensitive(ETrue)
    {
    }


EXPORT_C TOrderRuleType TMdEOrderRule::Type() const
    {
    return iType;
    }


EXPORT_C void TMdEOrderRule::SetType(TOrderRuleType aType)
    {
    iType = aType;
    }


EXPORT_C TBool TMdEOrderRule::Ascending() const
    {
    return iAscending;
    }
    

EXPORT_C void TMdEOrderRule::SetAscending(TBool aAscending)
    {
    iAscending = aAscending;
    }


EXPORT_C const CMdEPropertyDef* TMdEOrderRule::PropertyDef() const
    {
    return iPropertyDef;
    }


EXPORT_C void TMdEOrderRule::SetPropertyDef(const CMdEPropertyDef& aPropertyDef)
    {
    iPropertyDef = &aPropertyDef;
    }

EXPORT_C void TMdEOrderRule::SetCaseSensitive(TBool aCaseSensitive)
    {
    iCaseSensitive = aCaseSensitive;
    }

EXPORT_C TBool TMdEOrderRule::CaseSensitive()
    {
    return iCaseSensitive;
    }

TUint32 TMdEOrderRule::RequiredBufferSize() const
	{
	return sizeof( TMdCOrderRule );
	}

void TMdEOrderRule::SerializeL(CMdCSerializationBuffer& aBuffer, 
		TMdCOffset& aFreespaceOffset) const
	{
	TMdCOrderRule rule;

	rule.iOrderRule = iType;
	rule.iAscending = iAscending;
	rule.iCaseSensitive = iCaseSensitive;

	// If it's property order insert property def ID
	if( EOrderRuleTypeProperty == iType )
		{
		const TDefId propertyDefId = iPropertyDef->Id();
		rule.iPropertyDefId = propertyDefId;
		}
	else
		{
		rule.iPropertyDefId = KNoDefId;
		}
	
	rule.SerializeL( aBuffer );
	
	aFreespaceOffset = aBuffer.Position();
	}
