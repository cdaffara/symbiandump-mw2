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


#ifndef CCONFIGURATIONHANDLER_H
#define CCONFIGURATIONHANDLER_H

//  INCLUDES
#include <e32cmn.h>

#include "ccetestconfigurations.h"

/**
*
* These calss handles test case configurations. E.g. ccetester might set method 
* CCPRefConvergedCallProvider::ExtensionProviderL to return NULL or to leave.
* That information is stored in this class. 
*
*
*/

class CConfigurationHandler;

typedef struct ConfigurationItem 
	{
	// index in ActionList
	TUint iActionIndex;
	// iValuePtr refers to leave value
	TBool iIsLeaveValue;
	// value as pointer. Set when parsed and fetched in CConfigurationHandler::IsActionConfigured.
	void* iValuePtr;
	// is value setting persistant or not.
	TBool iIsPersistant;
	
	ConfigurationItem() : 	iActionIndex(0), 
							iIsLeaveValue(EFalse),
							iValuePtr(NULL), 
							iIsPersistant(EFalse) 
		{
		};
	~ConfigurationItem() 
		{
		};
	
	} CONFIGURATION_ITEM;

class CConfigurationHandler
    {
public:

	/* ctor */
	CConfigurationHandler();

	/* dtor */
	~CConfigurationHandler();

	/*
	* Get value for given id from passed string.
	* @param aData Searched string.
	* @param aKey Key to value look for.
	* @return Newly allocated string containing the value. Caller has to delete it. NULL if not found.
	*/
	HBufC8* GetTokenValue( const TDesC8& aData, 
						   const TDesC8& aKey );

	/*
	* Check if value is in TCCETesterCfg enumeration=is a config action.
	* @param aValue to compare.
	* @return Index of ActionArray. KErrNotFound if not found.
	*/
	TInt FindActionIndex( const TDesC8& aValue );


	/*
	* Is action aValue configured.
	* @param aValue Action name to check if it has been configured with values.
	* @param aResult Test configuration data.
	* @param aResult aDoLeave optionally check if leave flag is set or not.
	* @return Action was configured or not.
	*/
	template <class T>
	TBool IsActionConfigured( const TDesC8& aValue, T& aResult, TBool* aDoLeave = NULL );

	/*
	* Check if value is a configured action ( is in ActionList).
	* @param aValue to compare.
	* @return Is value a configuration action or not.
	*/
	TBool IsConfigRequest( TDesC8& aValue  );

	/*
	* Set configuration data. 
	* @param aData Configuration data as a string.
	* @param aPersistantSet Is set persisrtant until next set or not.
	* @return Set was successful or not
	*/
	TBool SetConfigData( TDesC8& aData, 
						 TBool aPersistantSet = EFalse );

private:

	/*
	* Parse passed data for validy and set values to passed item structure.
	* @param aData Passed data
	* @param aItem Store data here.
	* @return Data was valid or not.
	*/
	TBool ParseData( TDesC8& aData, 
					CONFIGURATION_ITEM& aItem );

	/*
	* Add or update item in the list. aItem.iActionId as key;
	* @param aItem Item to add / update.
	* @return Action was ok.
	*/
	TBool AddOrUpdateToList( const CONFIGURATION_ITEM& aItem );

	/*
	* Rtemove item in the list. aItem.iActionId as key;
	* @param aItem Item to add / update.
	* @return Action was ok.
	*/
	TBool RemoveFromList( const CONFIGURATION_ITEM& aItem );	


private:
	RArray<CONFIGURATION_ITEM> iItemList;
    };


// -----------------------------------------------------------------------------
// 
// -----------------------------------------------------------------------------
//
template <class T>
TBool CConfigurationHandler::IsActionConfigured( const TDesC8& aValue, T& aResult, TBool* aDoLeave /*= NULL*/ )
	{
	for( unsigned i=0; i<iItemList.Count(); i++ )
		{
		// todo range checking ...
		TPtrC8 ptr = ActionList[iItemList[i].iActionIndex];
		
		if( aValue.Compare(ptr)==KErrNone )
			{
//		   	RDebug::Print(_L("[TESTPLUGIN] CConfigurationHandler::IsActionConfigured, custom value was configured for %S"), &aValue );
			
			CONFIGURATION_ITEM item = iItemList[i];

			T* tp= (T*)item.iValuePtr;			
			aResult = reinterpret_cast<T&>(*(T*)item.iValuePtr);
//			aResult = reinterpret_cast<T&>(*tp);
			
			if( aDoLeave )
				{
				*aDoLeave = item.iIsLeaveValue;
				}
				
			if( !item.iIsPersistant )
				{
				delete item.iValuePtr; item.iValuePtr = NULL;
				iItemList.Remove(i);
				iItemList.Compress();
				}
			
			
			return ETrue;
			}
		}
	return EFalse;
	}


#endif	// cconfigurationhandler_h

// End of File

