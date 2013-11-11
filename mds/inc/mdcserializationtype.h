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
* Description:  common item type for all MdE objects
*
*/

#ifndef __MDCSERIALIZATIONTYPE_H__
#define __MDCSERIALIZATIONTYPE_H__

#include "mdscommoninternal.h"
#include "mdcserializationbuffer.h"

NONSHARABLE_CLASS(TMdCSerializationType)
	{
	protected:
		enum TMdCStructTypes
			{
			//EMdCItem,
			EMdCObject,
			EMdCProperty,
			EMdCRelation,
			EMdCEvent,
			EMdCObjectDef,
			EMdCPropertyDef,
			EMdCRelationDef,
			EMdCEventDef,
			EMdCNamespaceDef,
			EMdCSchema,
			EMdCItems,
			EMdCItemIds,
			EMdCItemCount,
			EMdCLogicCondition,
			EMdCObjectCondition,
			EMdCEventCondition,
			EMdCRelationCondition,
			EMdCPropertyCondition,
			EMdCSearchCriteria,
			EMdCOrderRule,
			};
	
		typedef TInt16 TMdCStructType;
	
	protected:
		TMdCSerializationType(TMdCStructTypes aType) : iType( aType )
			{
			}

		/**
		 * Those methods MUST be always use for serializing
		 */
		inline void SerializeL(CMdCSerializationBuffer& aBuffer, TUint32 aSize)
			{
			aBuffer.InsertL( *this, aSize );
			}

		inline void DeserializeL(CMdCSerializationBuffer& aBuffer, TUint32 aSize)
			{
			const TMdCStructTypes type = iType;
			
			aBuffer.ReceiveL( *this, aSize );
			
			if( type != iType )
				{
				_LIT( KTMdCSerializationType, "TMdCSerializationType" );
				__ASSERT_DEBUG( EFalse, User::Panic( KTMdCSerializationType, KErrCorrupt ) );

				User::Leave( KErrCorrupt );
				}
			}
		
	protected:
		const TMdCStructTypes iType;
	};

#endif /* __MDCSERIALIZATIONTYPE_H__ */
