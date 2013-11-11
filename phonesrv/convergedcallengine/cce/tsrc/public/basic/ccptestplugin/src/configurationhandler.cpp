/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Testing case configurator handler.
*
*/

#include <e32std.h>
#include <e32debug.h>


#include "configurationhandler.h"


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CConfigurationHandler::CConfigurationHandler()
	{
	}


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
CConfigurationHandler::~CConfigurationHandler()
	{

	}


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TInt CConfigurationHandler::FindActionIndex( const TDesC8& aValue )
	{
	for( unsigned i=0; i<sizeof(ActionList)/MaxStrSize; i++ )
		{
		TPtrC8 ptr = ActionList[i];
		
		if( aValue.Compare(ptr)==KErrNone )
			{
			return i;
			}
		}
	
	return KErrNotFound;
	}

// -----------------------------------------------------------------------------
// Is passed string one of those listed in ActionList in ccetestconfigurations.h
// -----------------------------------------------------------------------------
//
TBool CConfigurationHandler::IsConfigRequest( TDesC8& aValue )
	{
	HBufC8* result = GetTokenValue(aValue, KId);

	if( !result )
		{
		return EFalse;
		}
		
	if( FindActionIndex(*result)==KErrNotFound )
		{
		delete result;
		return EFalse;
		}

	delete result;
	return ETrue;
	}


// -----------------------------------------------------------------------------
// Get value for given id.
// -----------------------------------------------------------------------------
//
HBufC8* CConfigurationHandler::GetTokenValue( const TDesC8& aData, 
											  const TDesC8& aKey )
	{
	if( aData.Find(aKey)==KErrNotFound )
		{
		return NULL;
		}

	// id is in the string
	TLex8 lex(aData);

	while( !lex.Eos() )
		{
		TPtrC8 token = lex.NextToken();

		TInt spos = token.Find(aKey);
		if( spos==KErrNotFound )
			{
			continue;
			}

		// key was found return value		
		
		TPtrC8 value = token.Right(token.Length()-aKey.Length()); 

		HBufC8* retval = HBufC8::NewL(value.Length());
		TPtr8 des = retval->Des();
		des.Append(value);
		return retval;
		}
		
	return NULL;
	}


// -----------------------------------------------------------------------------
// Set data for a case
// -----------------------------------------------------------------------------
//
TBool CConfigurationHandler::SetConfigData( TDesC8& aData, 
											TBool aPersistantSet /*= EFalse*/ )
	{
	CONFIGURATION_ITEM item;
	
	if( !ParseData(aData,item) )
		{
		return EFalse;
		}

	// update information
	AddOrUpdateToList(item);

	return ETrue;
	}


// -----------------------------------------------------------------------------
// parse passed data
// ID + RETURN or LEAVE has to be found
// -----------------------------------------------------------------------------
//
TBool CConfigurationHandler::ParseData( TDesC8& aData, 
										CONFIGURATION_ITEM& aItem )
	{
	// get value for ID=
	HBufC8* id = GetTokenValue(aData, KId);

	if( !id )
		{
		RDebug::Print(_L("[TESTPLUGIN] CConfigurationHandler::ParseData ID= not found") );
		delete id;
		return EFalse;
		}

	// store index to action array
	aItem.iActionIndex = FindActionIndex(*id);
	delete id;


	// get value for TYPE=
	HBufC8* type = GetTokenValue(aData, KType);
	if( !type )
		{
		RDebug::Print(_L("[TESTPLUGIN] CConfigurationHandler::ParseData TYPE= not found") );
		delete type;
		return EFalse;
		}
	
	// todo is numeric check when needed
	// get value for RETURN=
	aItem.iIsLeaveValue = EFalse;
	HBufC8* strvalue = GetTokenValue(aData, KReturn);
	if( !strvalue )
		{
		delete strvalue; strvalue = NULL;
		// get value for LEAVE= as return value was not found
		strvalue = GetTokenValue(aData, KLeave);
		aItem.iIsLeaveValue = ETrue;
		}

	if( !strvalue )
		{
		RDebug::Print(_L("[TESTPLUGIN] CConfigurationHandler::ParseData: No RETURN= or LEAVE= found") );
		return EFalse;
		}


	// optional parameter
	HBufC8* persistant = GetTokenValue(aData, KPersistant );
	if( persistant )
		{
		TBool b;
		TLex8 l(*persistant);
		l.Val(b); 
		aItem.iIsPersistant = b;
		}
	else
		{
		aItem.iIsPersistant = EFalse;
		}
	delete persistant;
	
	if( type->Compare(_L8("TInt"))==KErrNone )
		{
		TInt v = 0;
		TLex8 lex(*strvalue);
		lex.Val(v);
		aItem.iValuePtr = new TInt(v); 
		}
	else if( type->Compare(_L8("TUint"))==KErrNone )
		{
		TUint v = 0;
		TLex8 lex(*strvalue);
		lex.Val(v);
		aItem.iValuePtr = new TUint(v); 
		}
	else if( type->Compare(_L8("TUid"))==KErrNone )
		{
		TInt v = 0;
		TLex8 lex(*strvalue);
		lex.Val(v);

		TUid u;
		u.iUid = v;
		aItem.iValuePtr = new TUid(u); 
		}
	else if( type->Compare(_L8("TBool"))==KErrNone )
		{
		TBool b;
		TLex8 l(*strvalue);
		l.Val(b); 
		aItem.iValuePtr = new TBool(b); 
		}
	else if( type->Compare(_L8("TDesC8"))==KErrNone )
		{
		HBufC8* buf = HBufC8::NewL(strvalue->Length()); 
		TPtr8 des = buf->Des();
		des.Append(*strvalue);
		aItem.iValuePtr = buf;
		}
	else if( type->Compare(_L8("CCCPCallParameters"))==KErrNone )
		{
		//CCCPCallParameters		
		}
	else
		{
		RDebug::Print(_L("[TESTPLUGIN] CConfigurationHandler::ParseData: No valid type found") );
		return EFalse;
		}


	delete type;
	delete strvalue;
	
	return ETrue;
	}


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CConfigurationHandler::AddOrUpdateToList( const CONFIGURATION_ITEM& aItem )
	{
	for( unsigned i=0; i<iItemList.Count(); i++ )
		{
		if( iItemList[i].iActionIndex==aItem.iActionIndex )
			{
			iItemList[i] = aItem;
			return ETrue;
			}
		}

	iItemList.Append(aItem);

	return ETrue;
	}

// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
TBool CConfigurationHandler::RemoveFromList( const CONFIGURATION_ITEM& aItem )
	{
	for( unsigned i=0; i<iItemList.Count(); i++ )
		{
		if( iItemList[i].iActionIndex==aItem.iActionIndex )
			{
			delete iItemList[i].iValuePtr;
			iItemList.Remove(i);
			return ETrue;
			}
		}
	return EFalse;
	}

	





// End of File


