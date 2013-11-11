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
* Description:  Includes predefined SQL clauses for usage of*
*/

#ifndef __MDSFINDSQLCLAUSEDEF_H__
#define __MDSFINDSQLCLAUSEDEF_H__

// ---------------------------------------------------------------------------
// Misc definitions
// ---------------------------------------------------------------------------
//

_LIT( KEmpty, "" );
_LIT( KSpace, " " );
_LIT( KLeftBracket, "(" );
_LIT( KRightBracket, ")" );
_LIT( KComma, "," );
_LIT( KSemicolon, ";" );
_LIT( KVariable, "?" );
_LIT( KWildcard, "%" );
_LIT( KEmptyTrue, "1" ); // used from always pass true tests

_LIT( KCommaVariable, ",?" );

_LIT( KNotIn, " NOT IN" );
_LIT( KIn, " IN" );
_LIT( KOrderBy, " ORDER BY " );
_LIT( KNoCaseSensitive, " COLLATE NOCASE ");
_LIT( KLimit, "LIMIT " );
_LIT( KOffset, "OFFSET " );
_LIT( KAsc, " ASC");
_LIT( KDesc, " DESC" );

_LIT( KIsNull,  " ISNULL" );
_LIT( KNotNull, " NOTNULL" );

_LIT( KUnion, " UNION " );
_LIT( KIntersect, " INTERSECT " );

_LIT( KUnionAll, " UNION ALL " );

_LIT( KAsBoJoin, " AS BO JOIN " );

_LIT( KBaseObjectObjectId, "BO.ObjectId" );
_LIT( KObjectObjectId,     "O.ObjectId" );

_LIT( KRelationId, "RelationId" );
_LIT( KEventId,    "EventId" );

_LIT( KSelectPropertyFilterBegin, "SELECT BO.ObjectId,BO.ObjectDefId,BO.Flags,BO.MediaId,BO.UsageCount,BO.GuidHigh,BO.GuidLow,BO.URI" );

_LIT( KBaseObjectDot, "BO." );
_LIT( KObjectDot,     "O." );

_LIT( KGroupByObjectId, "GROUP BY BO.ObjectId " );

// ---------------------------------------------------------------------------
// Object flags
// ---------------------------------------------------------------------------
//

_LIT( KGetFlagAnd, "(Flags&?)AND " );        // Flag is set
_LIT( KGetNotFlagAnd, "(NOT Flags&?)AND " ); // Flag is not set

_LIT( KGetNotFlag, "(NOT Flags&?) " );        // Flag is not set

_LIT( KWhereNotFlag, "WHERE(NOT Flags&?)" ); // Flag is not set

// ---------------------------------------------------------------------------
// SELECT clauses
// ---------------------------------------------------------------------------
//

_LIT( KSelect, "SELECT " );

_LIT( KSelectCount, 
	"SELECT count(*) "
	);

_LIT( KSelect1,
	"SELECT 1 "
	);

_LIT( KSelectObjectIds,
    "SELECT BO.ObjectId "
    );

_LIT( KSelectAllFromBaseObject, 
	"SELECT BO.* "
	);

_LIT( KAllFromObject, 
	",O.* "
	);

_LIT( KSelectFreeTextCountAndTotalLength, 
	",count(D.Word),sum(length(D.Word)) "
	);

// distict queries
_LIT( KSelectDistinct, 
	"SELECT DISTINCT "
	);

_LIT( KSelectAllFrom, 
	"SELECT * "
	);

_LIT( KSelectEventIds, 
	"SELECT Event"
	);

_LIT( KDotEventId, 
	".EventId "
	);

_LIT( KSelectRelationIds, 
	"SELECT Relations"
	);

_LIT( KDotRelationId, 
	".RelationId "
	);


// ---------------------------------------------------------------------------
// FROM clauses
// ---------------------------------------------------------------------------
//

_LIT( KFromBaseObject, 
	"FROM Object"
	);

_LIT( KAsBaseObject, 
	" AS BO "
	);

_LIT( KAsObjectOnEqual, 
	" AS O ON BO.ObjectId=O.ObjectId "
	);

_LIT( KFreeTextLeftJoinBegin,
	"LEFT JOIN TextSearchDictionary"
	);

_LIT( KFreeTextLeftJoinMiddle,
	" AS D ON D.WordId IN(SELECT WordId FROM TextSearch"
	);

_LIT( KFreeTextLeftJoinEnd,
	" WHERE BO.ObjectId=ObjectId) "
	);

_LIT( KFromEvent, 
    "FROM Event"
    );

_LIT( KFromRealtions, 
    "FROM Relations"
    );

// ---------------------------------------------------------------------------
// WHERE statement and comparison operators
// ---------------------------------------------------------------------------
//
_LIT( KWhere, "WHERE" );

// Comparison operators
_LIT( KEqual,          "=?" );
_LIT( KNotEqual,       "!=?" );
_LIT( KLess,           "<?" );
_LIT( KLessOrEqual,    "<=?" );
_LIT( KGreater,        ">?" );
_LIT( KGreaterOrEqual, ">=?" );
_LIT( KBetween,        " BETWEEN ? AND ?" );
_LIT( KNotBetween,     " NOT BETWEEN ? AND ?" );
_LIT( KLike,           " LIKE ?" );

// Logic operators
_LIT( KAnd,          "AND " );
_LIT( KOr,           "OR "  );
_LIT( KSpaceWithNot, " NOT" );
_LIT( KNotWithSpace, "NOT " );
_LIT( KNot, "NOT" );

// Object conditions
_LIT( KObjectIdEqual,       "ObjectId=?" );
_LIT( KObjectIdIn,          "ObjectId IN" );
_LIT( KObjectDefIdEqual,    "ObjectDefId=?" );
_LIT( KObjectGuidEqual,     "(GuidHigh=? AND GuidLow=?)" );
_LIT( KObjectUsageCount,    "UsageCount" );
_LIT( KObjectUriEqual,      "URI=?" );
_LIT( KObjectUriBeginsWith, "URI LIKE ?" );

// Object searches
_LIT( KSelectObjectIdFromBaseObject, "SELECT ObjectId FROM Object" );

_LIT( KSelectObjectIdFrom, "SELECT ObjectId FROM " );

// Freetext searches
_LIT( KObjectFreeTextSearchStart,           "(ObjectId IN(SELECT ObjectID FROM TextSearch");
_LIT( KObjectFreeTextSearchWhere,           " WHERE WordId IN(SELECT WordId FROM TextSearchDictionary");
_LIT( KObjectFreeTextSearchDictionaryWhere, " WHERE Word" );
_LIT( KObjectFreeTextSearchEnd,             ")))" );

// Event searches
_LIT( KSelectEventIdFromEvent,  "SELECT EventId FROM Event" );
_LIT( KSelectObjectIdFromEvent, "SELECT ObjectId FROM Event" );
_LIT( KEventObjectIdIn,         "ObjectId IN" );
_LIT( KEventTimeStamp,          "TimeStamp" );
_LIT( KEventIdEqual,            "EventId=?" );
_LIT( KEventDefIdEqual,         "EventDefId=?" );
_LIT( KEventSourceEqual,        "Source=?" );
_LIT( KEventParticipantEqual,   "Participant=?" );

// Relation searches
_LIT( KSelectRelationIdFromRelation,    "SELECT RelationId FROM Relations" );
_LIT( KSelectLeftObjectIdFromRelation,  "SELECT LeftObjectId FROM Relations" );
_LIT( KSelectRightObjectIdFromRelation, "SELECT RightObjectId FROM Relations" );
_LIT( KRelationIdEqual,                 "RelationId=?" );
_LIT( KRelationIdIn,                  	"RelationId IN" );
_LIT( KRelationDefIdEqual,              "RelationDefId=?" );
_LIT( KRelationGuidEqual,               "GuidHigh=? AND GuidLow=?" );
_LIT( KRelationParameter,               "Parameter" );
_LIT( KRelationLastModifiedDate,        "LastModifiedDate" );
_LIT( KRelationLeftObjectIdIn,          "LeftObjectId IN" );
_LIT( KRelationRightObjectIdIn,         "RightObjectId IN" );


#endif // __MDSFINDSQLCLAUSEDEF_H__
