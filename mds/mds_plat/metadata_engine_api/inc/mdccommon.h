/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Common message definitions and utilities
*
*/

#ifndef __MDCCOMMON_H__
#define __MDCCOMMON_H__

// INCLUDE FILES
#include <e32base.h>
#include <mdeerror.h>

// MDS version
#define MDS_VERSION_2_5

// Item id
typedef TUint32 TItemId;
typedef TUint32 TDefId;
typedef TUint32 TMdCOffset;

const TItemId KNoId = 0;
const TItemId KSystemCapturedAlbumId = 2;
const TDefId KNoDefId = 0;

// CONSTANTS
_LIT( KMdEDefaultNameSpace, "http://sw.nokia.com/MdE" );

// Type of property
enum TPropertyType
    {
    EPropertyBool   = 0,
    EPropertyInt8   = 1,
    EPropertyUint8  = 2,
    EPropertyInt16  = 3,
    EPropertyUint16 = 4,
    EPropertyInt32  = 5,
    EPropertyUint32 = 6,
    EPropertyInt64  = 7,
    EPropertyReal32 = 8,
    EPropertyReal64 = 9,
    EPropertyTime   = 10,
    EPropertyText   = 11,
    };

/**
 * Query types.
 */
enum TQueryType
    {
    EQueryTypeFirst = 0,    // must always be the first one

    /** Query that returns CMdEObject items, IDs, count or distinct property values. */
    EQueryTypeObject,

    /** Query that returns CMdERelation items, IDs or count. */
    EQueryTypeRelation,

    /** Query that returns CMdEEvent items, IDs or count. */
    EQueryTypeEvent,

    EQueryTypeLast   // must always be the last one
    };  

/** 
 * Query's result type.
 */
enum TQueryResultMode
    {
    EQueryResultModeFirst = 0, // must always be the first one

    /** Query returns whole items (objects are returned without freetexts). */
    EQueryResultModeItem,

    /** Query returns set of IDs. */
    EQueryResultModeId,

    /** Query returns count of items */
    EQueryResultModeCount,

	/** Query returns distinct values from specified property definition. 
	 *  Query's type must be EQueryTypeObject. */
    EQueryResultModeDistinctValues,

	/** Query returns whole items with freetexts. */
	EQueryResultModeObjectWithFreetexts,

    EQueryResultModeLast // must always be the last one
    };

/** Types of sort order. */
enum TMdESortOrder
	{
	ESortAscending = ETrue, 
	ESortDescending = EFalse
	};

/** Types of result ordering rules. */
enum TOrderRuleType
    {
    EOrderRuleTypeFirst = 0,

    /** Sort results with item ID. */
    EOrderRuleTypeItemID,

    /** Sort results by object URI. */
    EOrderRuleTypeObjectURI,

    /** Sort results by object definition. */
    EOrderRuleTypeObjectDef,

    /** Sort results by the value of a property. */
    EOrderRuleTypeProperty,

    /** Sort objects by usage count. */
    EOrderRuleTypeUsageCount,

    /** Sort events by creation time. */
    EOrderRuleTypeCreationTime,

    /** Sort events by source URI. */
    EOrderRuleTypeSourceURI,

    /** Sort events by participant URI. */
    EOrderRuleTypeParticipantURI,

    /** Sort events by target object's ID. */
    EOrderRuleTypeTargetObjectID,

    /** Sort events by event definition. */
    EOrderRuleTypeEventDef,

    /** Sort relations by last modified date. */
    EOrderRuleTypeLastModifiedDate,

    /** Sort relations by parameter. */
    EOrderRuleTypeParameterValue,

    /** Sort relations by left object's ID. */
    EOrderRuleTypeLeftObjectID,

    /** Sort relations by right object's ID. */
    EOrderRuleTypeRightObjectID,

    /** Sort relations by relation definition. */
    EOrderRuleTypeRelationDef,
            
    EOrderRuleTypeLast
    };


/**
 * Condition types.
 */
enum TConditionType
	{
	EConditionTypeFirst	= 0x0000,   // must always be the first one
    
    /** logic conditions combine other conditions with logic operators */
    EConditionTypeLogic,

    /** conditions for object ID, URI, definition */
    EConditionTypeObject,            

    /** conditions for the property definition */
    EConditionTypeProperty,          
    
    /** condition for the value of a int property */
    EConditionTypePropertyIntRange,  
    
    /** condition for the value of a int64 property */
    EConditionTypePropertyInt64Range,

    /** condition for the value of a uint property */
    EConditionTypePropertyUintRange,  

    /** condition for the value of a real property */
    EConditionTypePropertyRealRange, 

    /** condition for the value of a time property */
    EConditionTypePropertyTimeRange, 

    /** condition for the value of a string property */
    EConditionTypePropertyText,    

    /** condition for the value of a boolean property */
    EConditionTypePropertyBool,      

    /** conditions for relation items */
    EConditionTypeRelation,          

    /** conditions for an event */
    EConditionTypeEvent,             

	EConditionTypeLast				// must always be the last one
	};

/**
 * Observer notification types
 */
enum TObserverNotificationType
	{
	ENotifyAdd    = 0x0001,
	ENotifyModify = 0x0002,
	ENotifyRemove = 0x0004,
	};

/** Logic condition operators. */
enum TLogicConditionOperator
    {
    ELogicConditionOperatorFirst = 0x0000,

    /** Boolean AND operator. */
    ELogicConditionOperatorAnd,

    /** Boolean OR operator. */
    ELogicConditionOperatorOr,

    ELogicConditionOperatorLast
    };

/** Methods of comparing object data. */
enum TObjectConditionCompareMethod 
    {
    EObjectConditionCompareFirst = 0,

    /** NONE. Just for sending condition flags */
    EObjectConditionCompareNone,

    /** Object ID. */
    EObjectConditionCompareId,

    /** Object IDs. */
    EObjectConditionCompareIds,

	/** Object GUID. */
	EObjectConditionCompareGuid,

    /** Object definition. */
    EObjectConditionCompareObjectDef,

    /** Object URI. */
    EObjectConditionCompareUri,

	/** Object URI begins with specified string. */
    EObjectConditionCompareUriBeginsWith,
    
    /** Free text keyword associated with the object must be an 
        exact match. */
    EObjectConditionCompareFreeText,

    /** Free text keyword associated with the object contains 
        specified string. */
    EObjectConditionCompareFreeTextContains,

    /** Free text keyword associated with the object begins with 
        specified string. */
    EObjectConditionCompareFreeTextBeginsWith,

    /** Free text keyword associated with the object ends with 
        specified string. */
    EObjectConditionCompareFreeTextEndsWith,

    /** Usage count */
    EObjectConditionCompareUsageCount,
    
    /** Object URIcontains specified string. */
    EObjectConditionCompareUriContains,

    EObjectConditionCompareLast
    };

/** Methods of filtering confidential and non-confidential objects. */
enum TObjectConditionConfidentialityLevel
    {
    EObjectConditionLevelFirst = 0,

    /** Both confidential and non-confidential objects */
    EObjectConditionLevelIgnoreConfidentiality,

    /** Selects ONLY non-confidential objects */
    EObjectConditionLevelNormal,

    /** Selects ONLY confidential objects. */
    EObjectConditionLevelConfidential,

    EObjectConditionLevelLast
    };

/** Methods of comparing event data. */
enum TEventConditionCompareMethod 
    {
    EEventConditionCompareFirst = 0x0000,
    
    /** No comparison specified. */
    EEventConditionCompareNone,
    
    /** Specified string must match the source URI. */
    EEventConditionCompareSourceURI,
    
    /** Specified string must match the participant URI. */
    EEventConditionCompareParticipantURI,
    
    /**Specified id must match events id*/
    EEventConditionCompareId,

    //ECompareSourceURIBeginsWith,
    //ECompareParticipantRIBeginsWith,
    
    EEventConditionCompareLast
    };

/** Side on which the relation must be in relation to a matched object. */
enum TRelationConditionSide
    {
    /** Object must be on either the left or the right side of 
        the relation. */
    ERelationConditionSideEither,

    /** Object must be on the left side of the relation. */
    ERelationConditionSideLeft,

    /** Object must be on the right side of the relation. */
    ERelationConditionSideRight
    };

/** Text value comparison methods. */
enum TTextPropertyConditionCompareMethod
    {
    ETextPropertyConditionCompareFirst = 0x0000,
    ETextPropertyConditionCompareEquals,          // LIKE "str"
    ETextPropertyConditionCompareContains,        // LIKE "%str%"
    ETextPropertyConditionCompareBeginsWith,      // LIKE "str%"
    ETextPropertyConditionCompareEndsWith,        // LIKE "%str"
    ETextPropertyConditionCompareLast
    };


/**
 * Min and max value
 * There are used like:
 * for Real types:   iMinValue.iReal, iMaxValue.iReal
 * for Int64 types:  iMinValue.iInt64, iMaxValue.iInt64
 * for Uint32 types: iMinValue.iUint32, iMaxValue.iUint32
 * (!!) others:      iMinValue.iInt32, iMaxValue.iInt32
 */
union TMdCValueUnion 
	{
	TInt32   iInt32;
	TUint32  iUint32;
	TInt64   iInt64;
	TReal64  iReal;
	HBufC16* iText;

	struct
		{
		TUint32 iCount;
		TMdCOffset iOffset;
		} iPtr;
	};

/**
* Common utilities.
* Used as a static function library -
* This class is not to be instantiated.
*/
class MMdCCommon
    {
    public: // public definitions
        /** Creates a panic */
        IMPORT_C static void Panic( TInt aPanicCode );
    };
        
#endif // __MDCCOMMON_H__
