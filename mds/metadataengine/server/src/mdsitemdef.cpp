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
* Description:  Class to hold description about one item definition
*
*/

#include "mdsitemdef.h"
#include "mdcserializationbuffer.h"

// reserved words for schema definitions
#define MDS_SCHEMA_DEFINITION_RESERVED_WORD(ReservedWord) \
    _LIT( K##ReservedWord##Name, " #ReservedWord " );

MDS_SCHEMA_DEFINITION_RESERVED_WORD(NamespaceDef)
MDS_SCHEMA_DEFINITION_RESERVED_WORD(ObjectDef)
MDS_SCHEMA_DEFINITION_RESERVED_WORD(PropertyDef) 
MDS_SCHEMA_DEFINITION_RESERVED_WORD(RelationDef) 
MDS_SCHEMA_DEFINITION_RESERVED_WORD(EventDef)
MDS_SCHEMA_DEFINITION_RESERVED_WORD(Relations) 
MDS_SCHEMA_DEFINITION_RESERVED_WORD(Event)
MDS_SCHEMA_DEFINITION_RESERVED_WORD(TextSearch)
MDS_SCHEMA_DEFINITION_RESERVED_WORD(TextSearchDictionary)

#undef MDS_SCHEMA_DEFINITION_RESERVED_WORD

/**
 * ConstructL
 */
void CMdsItemDef::ConstructL( const TDesC& aName )
	{
	// check name of schema definition against reserved words
	CheckReservedWordsL( aName );
	
	iName = aName.AllocL();
	iStoredFlags = EStoredFlagsNone;
	}

/**
 * Destructor
 */
CMdsItemDef::~CMdsItemDef()
	{
	delete iName;
	iName = NULL;
	}

TUint32 CMdsItemDef::RequiredBufferSize()
	{
	// itemDefId, name
	return CMdCSerializationBuffer::RequiredSize( *iName );
	}

TMdCOffset CMdsItemDef::SerializeL(CMdCSerializationBuffer& aBuffer)
	{		
	return aBuffer.InsertL( *iName );
	}

void CMdsItemDef::CheckAllowerCharatersL( const TDesC& aName, TBool aAllowNumbers )
	{
	const TInt count = aName.Length();

	// name can't be empty
	if( count <= 0 )
		{
		User::Leave( KErrBadDescriptor );
		}

	if( aAllowNumbers )
		{
		// first character can't be number
		TUint16 c = aName[0];
		CheckAlphabetL( c );

		for( TInt i = 1; i < count; i++ )
			{
			c = aName[i];
			CheckAlphabetOrNumberL( c );
			}
		}
	else
		{
		for( TInt i = 0; i < count; i++ )
			{
			TUint16 c = aName[i];
			CheckAlphabetL( c );
			}
		}
	}

void CMdsItemDef::CheckReservedWordsL( const TDesC& aName )
	{
#define MDS_DEFINE_RESERVED_WORD(ReservedWord) \
    &K##ReservedWord##Name,

	const TDesC* reservedWords[] =
		{
		MDS_DEFINE_RESERVED_WORD(NamespaceDef)
		MDS_DEFINE_RESERVED_WORD(ObjectDef)
		MDS_DEFINE_RESERVED_WORD(PropertyDef)
		MDS_DEFINE_RESERVED_WORD(RelationDef)
		MDS_DEFINE_RESERVED_WORD(EventDef)
		MDS_DEFINE_RESERVED_WORD(Relations)
		MDS_DEFINE_RESERVED_WORD(Event)
		MDS_DEFINE_RESERVED_WORD(TextSearch)
		MDS_DEFINE_RESERVED_WORD(TextSearchDictionary)
		};

#undef MDS_SCHEMA_DEFINITION_RESERVED_WORD

	const TInt count = sizeof ( reservedWords ) / sizeof( TDesC* );

	for ( TInt i = 0; i < count; ++i )
		{
		if ( *reservedWords[ i ] == aName )
			{
			User::Leave( KErrGeneral );
			}
		}
	}
