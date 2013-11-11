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
* Description:  Stores and load preference values to and from DB
*
*/

#ifndef __MDSPREFERENCES_H__
#define __MDSPREFERENCES_H__

#include <e32base.h>
#include "mdsclausebuffer.h"
#include "mdssqliteconnection.h"
#include "mdsdbconnectionpool.h"
#include "mdsfindsqlclausedef.h"

_LIT( KMdsPreferencesDeleteKey,   "DELETE FROM MdE_Preferences WHERE Key = ?;");

/**
 *  Metadata preference class
 *
 *  This class is responsible for storing and loading preferences to and from DB
 *
 */
class MMdsPreferences
    {
public:

	enum TMdsPreferencesFlags
		{
		EPreferenceNone          = 0x00000000,
		EPreferenceValueSet      = 0x00000001,
		EPreferenceValueGet      = 0x00000002,
		EPreferenceExtraSet      = 0x00000004,
		EPreferenceExtraGet      = 0x00000008,
		EPreferenceValueSortAsc  = 0x00000010,
		EPreferenceValueSortDesc = 0x00000020,
		EPreferenceExtraSortAsc  = 0x00000040,
		EPreferenceExtraSortDesc = 0x00000080,
		EPreferenceBothSet       = EPreferenceValueSet | EPreferenceExtraSet,
		EPreferenceBothGet       = EPreferenceValueGet | EPreferenceExtraGet,
		EPreferenceValueUse      = EPreferenceValueSet | EPreferenceValueGet,
		EPreferenceExtraUse      = EPreferenceExtraSet | EPreferenceExtraGet,
		EPreferenceAllUse        = EPreferenceValueUse | EPreferenceExtraUse
		};

	/**
	 * HOW TO USE
    _LIT( KTestNameValue, "TestNameValue" );
    _LIT( KTestNameExtra, "TestNameExtra" );
    _LIT( KTestNameAll,   "TestNameAll" );
    CMdsPreferences::InsertToDefaultDBL( db, KTestNameValue, CMdsPreferences::EPreferenceValueSet, 12.5f );
    CMdsPreferences::InsertToDefaultDBL( db, KTestNameExtra, CMdsPreferences::EPreferenceExtraSet, 99 );
    CMdsPreferences::InsertToDefaultDBL( db, KTestNameAll, CMdsPreferences::EPreferenceBothSet, KTestNameExtra, -10 );
    
    CMdsPreferences::UpdateDefaultDBL( db, KTestNameValue, CMdsPreferences::EPreferenceValueSet, 578 );
    CMdsPreferences::UpdateDefaultDBL( db, KTestNameExtra, CMdsPreferences::EPreferenceExtraSet, 12 );
    CMdsPreferences::UpdateDefaultDBL( db, KTestNameAll, CMdsPreferences::EPreferenceBothSet, 345.64f, -666 );

    CMdsPreferences::DeleteFromDefaultDBL( db, KTestNameValue );
    CMdsPreferences::DeleteFromDefaultDBL( db, KTestNameValue, CMdsPreferences::EPreferenceValueSet, 578 );
    CMdsPreferences::DeleteFromDefaultDBL( db, KTestNameExtra, CMdsPreferences::EPreferenceExtraSet, 12 );
    CMdsPreferences::DeleteFromDefaultDBL( db, KTestNameAll, CMdsPreferences::EPreferenceBothSet, 345.64f, -666 );
    */


	template<class T>
	static TInt InsertL( const TDesC& aKey, TUint32 aFlags, const T& aValue, TInt64 aExtraValue = 0 )
		{
		_LIT( KMdsPreferencesInsertValue,   "INSERT INTO MdE_Preferences(Key, Value) VALUES(?, ?);");
		_LIT( KMdsPreferencesInsertAll,     "INSERT INTO MdE_Preferences(Key, Value, ExtraValue) VALUES(?, ?, ?);");
		
		TInt result = 0;
		RRowData data;
		CleanupClosePushL( data );
		data.AppendL( TColumn(aKey) );
		data.AppendL( TColumn(aValue) );
		
		CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
		if ( aFlags & EPreferenceValueSet && aFlags & EPreferenceExtraSet )
			{
			data.AppendL( TColumn(aExtraValue) );
			result = connection.ExecuteL( KMdsPreferencesInsertAll, data );
			}
		else if ( aFlags & EPreferenceValueSet )
			{
			result = connection.ExecuteL( KMdsPreferencesInsertValue, data );
			}
		
		else
			{
			User::Leave( KErrCorrupt );
			}
		
		CleanupStack::PopAndDestroy( &data );
		return result;
		}

	template<class T>
	static TInt UpdateL( const TDesC& aKey, TUint32 aFlags, const T& aValue, TInt64 aExtraValue = 0 )
		{
		_LIT( KMdsPreferencesUpdateValue, "UPDATE MdE_Preferences SET Value = ? WHERE Key = ?;");
		_LIT( KMdsPreferencesUpdateExtra, "UPDATE MdE_Preferences SET ExtraValue = ? WHERE Key = ? AND Value = ?;");
		_LIT( KMdsPreferencesUpdateBoth,  "UPDATE MdE_Preferences SET Value = ?, ExtraValue = ? WHERE Key = ?;");
		
		TInt result = 0;
		RRowData data;
		CleanupClosePushL( data );
		
		CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
		if ( aFlags & EPreferenceValueSet && aFlags & EPreferenceExtraSet )
			{
			data.AppendL( TColumn(aValue) );
			data.AppendL( TColumn(aExtraValue) );
			data.AppendL( TColumn(aKey) );
			result = connection.ExecuteL( KMdsPreferencesUpdateBoth, data );
			}
		else if ( aFlags & EPreferenceValueSet )
			{
			data.AppendL( TColumn(aValue) );
			data.AppendL( TColumn(aKey) );
			result = connection.ExecuteL( KMdsPreferencesUpdateValue, data );
			}
		else if ( aFlags & EPreferenceExtraSet )
			{
			data.AppendL( TColumn(aExtraValue) );
			data.AppendL( TColumn(aKey) );
			data.AppendL( TColumn(aValue) );
			result = connection.ExecuteL( KMdsPreferencesUpdateExtra, data );
			}
		else
			{
			User::Leave( KErrCorrupt );
			}
		
		CleanupStack::PopAndDestroy( &data );
		return result;
		}

	template<class T>
	static TInt DeleteL( const TDesC& aKey )
		{	
		TInt result = 0;
		RRowData data;
		CleanupClosePushL( data );
		data.AppendL( TColumn(aKey) );
		
		CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
		result = connection.ExecuteL( KMdsPreferencesDeleteKey, data );
		
		CleanupStack::PopAndDestroy( &data );
		return result;
		}

	template<class T>
	static TInt DeleteL( const TDesC& aKey, TUint32 aFlags, const T& aValue, TInt64 aExtraValue = 0 )
		{
		_LIT( KMdsPreferencesDeleteValue, "DELETE FROM MdE_Preferences WHERE Key = ? AND Value = ?;");
		_LIT( KMdsPreferencesDeleteExtra, "DELETE FROM MdE_Preferences WHERE Key = ? AND ExtraValue = ?;");
		_LIT( KMdsPreferencesDeleteAll,   "DELETE FROM MdE_Preferences WHERE Key = ? AND Value = ? AND ExtraValue = ?;");
		
		TInt result = 0;
		RRowData data;
		CleanupClosePushL( data );
		data.AppendL( TColumn(aKey) );
		
		CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
		if ( aFlags == EPreferenceNone )
			{
			result = connection.ExecuteL( KMdsPreferencesDeleteKey, data );
			}
		else if ( aFlags & EPreferenceValueSet && aFlags & EPreferenceExtraSet )
			{
			data.AppendL( TColumn(aValue) );
			data.AppendL( TColumn(aExtraValue) );
			result = connection.ExecuteL( KMdsPreferencesDeleteAll, data );
			}
		else if ( aFlags & EPreferenceValueSet )
			{
			data.AppendL( TColumn(aValue) );
			result = connection.ExecuteL( KMdsPreferencesDeleteValue, data );
			}
		else if ( aFlags & EPreferenceExtraSet )
			{
			data.AppendL( TColumn(aExtraValue) );
			result = connection.ExecuteL( KMdsPreferencesDeleteExtra, data );
			}
		else
			{
			User::Leave( KErrCorrupt );
			}
		
		CleanupStack::PopAndDestroy( &data );
		return result;
		}

	template<class T>
	static TInt GetL( const TDesC& aKey, TUint32 aFlags, T& aValue, TInt64* aExtraValue = NULL )
		{
		_LIT( KMdsPreferencesGetKey,        "SELECT Value, ExtraValue FROM MdE_Preferences WHERE Key = ? ");

		_LIT( KMdsPreferencesGetValue,      "AND Value = ? ");
		_LIT( KMdsPreferencesGetExtra,      "AND ExtraValue = ? ");
		_LIT( KMdsPreferencesGetAll,        "AND Value = ? AND ExtraValue = ? ");

		_LIT( KMdsPreferencesSortBegin,     "ORDER BY ");
		_LIT( KMdsPreferencesSortValueAsc,  "Value ASC ");
		_LIT( KMdsPreferencesSortValueDesc, "Value DESC ");
		_LIT( KMdsPreferencesSortExtraAsc,  "ExtraValue ASC ");
		_LIT( KMdsPreferencesSortExtraDesc, "ExtraValue DESC ");
		_LIT( KMdsPreferencesSortEnd,       "LIMIT 1;");

		const TUint32 KSortFlags = 	EPreferenceValueSortAsc  | 
									EPreferenceValueSortDesc | 
									EPreferenceExtraSortAsc  | 
									EPreferenceExtraSortDesc;

		CMdsClauseBuffer* sortBuffer = CMdsClauseBuffer::NewLC( 12 ); // minimum reserved lenght for sort rules

		// check if there is some order rules
		if( KSortFlags & aFlags )
			{
			sortBuffer->AppendL( KMdsPreferencesSortBegin );
			
			TBool notFirstOrderRule = EFalse;
			
			if( EPreferenceValueSortAsc & aFlags )
				{
				sortBuffer->AppendL( KMdsPreferencesSortValueAsc );
				
				notFirstOrderRule = ETrue;
				}
			
			if( EPreferenceValueSortDesc & aFlags )
				{
				if( notFirstOrderRule )
					{
					sortBuffer->AppendL( KComma );
					}
				sortBuffer->AppendL( KMdsPreferencesSortValueDesc );
				notFirstOrderRule = ETrue;
				}
			
			if( EPreferenceExtraSortAsc & aFlags )
				{
				if( notFirstOrderRule )
					{
					sortBuffer->AppendL( KComma );
					}
				sortBuffer->AppendL( KMdsPreferencesSortExtraAsc );
				notFirstOrderRule = ETrue;
				}

			if( EPreferenceExtraSortDesc & aFlags )
				{
				if( notFirstOrderRule )
					{
					sortBuffer->AppendL( KComma );
					}
				sortBuffer->AppendL( KMdsPreferencesSortExtraDesc );
				notFirstOrderRule = ETrue;
				}
			}

		// always added because it limits result count to 1
		sortBuffer->AppendL( KMdsPreferencesSortEnd );

		CMdsClauseBuffer* selectBuffer = CMdsClauseBuffer::NewLC( 
				64 + sortBuffer->ConstBufferL().Length() ); // estimated minimum lenght for select and sort rules
		
		/// always added default select
		selectBuffer->AppendL( KMdsPreferencesGetKey );

		TInt result = 0;
		RRowData data;
		CleanupClosePushL( data );
		data.AppendL( TColumn(aKey) );
		
		RRowData getData;
		CleanupClosePushL( getData );
		getData.AppendL( TColumn(aValue) );
		getData.AppendL( TColumn(EColumnInt64) );
		
		if ( !aExtraValue && (aFlags & EPreferenceExtraSet || aFlags & EPreferenceExtraGet) )
			{
			User::Leave( KErrArgument );
			}
		
		CMdSSqLiteConnection& connection = MMdSDbConnectionPool::GetDefaultDBL();
		RMdsStatement query;
		CleanupClosePushL( query );
		if ( aFlags == EPreferenceNone )
			{
			// do nothing
			}
		else if ( aFlags & EPreferenceValueSet && aFlags & EPreferenceExtraSet )
			{
			data.AppendL( TColumn(aValue) );
			data.AppendL( TColumn(*aExtraValue) );
			
			selectBuffer->AppendL( KMdsPreferencesGetAll );
			selectBuffer->AppendL( sortBuffer->ConstBufferL() );

			connection.ExecuteQueryL( selectBuffer->ConstBufferL(), query, data );
			
			
			if ( connection.NextRowL( query, getData ) )
				{
				result = 1;
				if (aFlags & EPreferenceValueGet)
					{
					getData.Column( 0 ).Get( aValue );
					}
				
				if (aFlags & EPreferenceExtraGet)
					{
					getData.Column( 1 ).Get( *aExtraValue );
					}
				}
			}
		else if ( aFlags & EPreferenceValueSet )
			{
			data.AppendL( TColumn(aValue) );
			
			selectBuffer->AppendL( KMdsPreferencesGetValue );
			selectBuffer->AppendL( sortBuffer->ConstBufferL() );
			
			connection.ExecuteQueryL( selectBuffer->ConstBufferL(), query, data );
			if ( connection.NextRowL( query, getData ) )
				{
				result = 1;
				if (aFlags & EPreferenceValueGet)
					{
					getData.Column( 0 ).Get( aValue );
					}
				
				if (aFlags & EPreferenceExtraGet)
					{
					getData.Column( 1 ).Get( *aExtraValue );
					}
				}
			}
		else if ( aFlags & EPreferenceExtraSet )
			{
			data.AppendL( TColumn(*aExtraValue) );
			
			selectBuffer->AppendL( KMdsPreferencesGetExtra );
			selectBuffer->AppendL( sortBuffer->ConstBufferL() );

			connection.ExecuteQueryL( selectBuffer->ConstBufferL(), query, data );
			if ( connection.NextRowL( query, getData ) )
				{
				result = 1;
				if (aFlags & EPreferenceValueGet)
					{
					getData.Column( 0 ).Get( aValue );
					}
				
				if (aFlags & EPreferenceExtraGet)
					{
					getData.Column( 1 ).Get( *aExtraValue );
					}
				}
			}
		else
			{
			selectBuffer->AppendL( sortBuffer->ConstBufferL() );

			connection.ExecuteQueryL( selectBuffer->ConstBufferL(), query, data );
			if ( connection.NextRowL( query, getData ) )
				{
				result = 1;
				if (aFlags & EPreferenceValueGet)
					{
					getData.Column( 0 ).Get( aValue );
					}
				
				if (aFlags & EPreferenceExtraGet)
					{
					getData.Column( 1 ).Get( *aExtraValue );
					}
				}
			}

		CleanupStack::PopAndDestroy( &query );
		CleanupStack::PopAndDestroy( &getData );
		CleanupStack::PopAndDestroy( &data );
		
		CleanupStack::PopAndDestroy( selectBuffer );
		CleanupStack::PopAndDestroy( sortBuffer );

		return result;
		}    
    
private:

    };


#endif // __MDSPREFERENCES_H__
