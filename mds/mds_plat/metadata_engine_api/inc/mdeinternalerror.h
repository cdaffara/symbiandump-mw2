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
* Description:  Common internal error code definitions
*
*/

#ifndef MDEINTERNALERROR_H
#define MDEINTERNALERROR_H

#include <e32def.h>

/*
 * Internal error. Unknown namespace definition.
 */
const TInt KErrMdEUnknownNamespaceDef = -36240;

/*
 * Internal error. Unknown object definition.
 */
const TInt KErrMdEUnknownObjectDef = -36241;

/*
 * Internal error. Unknown property definition.
 */
const TInt KErrMdEUnknownPropertyDef = -36242;

/*
 * Internal error. Unknown relation definition.
 */
const TInt KErrMdEUnknownRelationDef = -36243;

/*
 * Internal error. Unknown event definition.
 */
const TInt KErrMdEUnknownEventDef = -36244;

/*
 * Internal error. Unknown property type.
 */
const TInt KErrMdEUnknownPropertyType = -36245;

/*
 * Internal error. Unknown confidentiality level.
 */
const TInt KErrMdEUnknownConfidentialityLevel = -36246;

/*
 * Internal error. Unknown compare method.
 */
const TInt KErrMdEUnknownCompareMethod = -36247;

/*
 * Internal error. Unknown query type.
 */
const TInt KErrMdEUnknownQueryType = -36248;

/*
 * Internal error. Unknown query result mode.
 */
const TInt KErrMdEUnknownQueryResultMode = -36249;

/*
 * Internal error. Range type is not expected.
 */
const TInt KErrMdEUnknownRangeType = -36250;

/*
 * Internal error. Unknown condition type.
 */
const TInt KErrMdEUnknownConditionType = -36251;

/*
 * Internal error. Unknown condition compare method.
 */
const TInt KErrMdEUnknownConditionCompareMethod = -36252;

/*
 * Internal error. Condition operator is not expected.
 */
const TInt KErrMdEIncorrectConditionOperator = -36253;

/*
 * Internal error. Serialized type is not expected.
 */
const TInt KErrMdEIncorrectSerializedType = -36254;

/*
 * Internal error. Size of serialized type is not expected.
 */
const TInt KErrMdEIncorrectSerializedTypeSize = -36255;

/*
 * Internal error. Schema contains illegal character.
 */
const TInt KErrMdESchemaContainsIllegalCharacter = -36256;

/*
 * Internal error. Schema contains reserved word.
 */
const TInt KErrMdESchemaContainsReservedWord = -36257;

#endif /*MDEINTERNALERROR_H*/
