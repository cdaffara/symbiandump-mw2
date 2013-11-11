// Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// msvsearchsortconstants.h
// 
//
/**
 * @file 
 * @internalComponent
 * @released
 */
#ifndef __MSVSEARCHSORTCONSTANT_H__
#define __MSVSEARCHSORTCONSTANT_H__


/**
@internalComponent
@released
*/
const TInt KMsvMaxSearchTextLength = 300;
const TInt KMaxLevelOfSearchAndSort = 5;
const TInt KContentOfMessagePart = 50;

/** 
Result type
@internalComponent
@released
*/
const TInt KFinalResult=1;                   
const TInt KPartialResult=2;
const TInt KNewQuery=3;

/** 
For Invalid query id
@internalComponent
@released
*/
const TInt KMsvSearchSortQueryIdNotFound=-1;

/** 
TMsvIdWithSortField structure contains TMsvId and corresponding TMessagePart content to sort
@internalComponent
@released
*/
struct TMsvIdWithSortField
	{
	TMsvId iMessageId;
	TBuf <KContentOfMessagePart> iContentMessagePart;
	};

/** 
TMsvQueryTable, this contains Message part filled, Relation Operator & Query string
@internalComponent
@released
*/
struct TMsvQueryTable
	{
		TMsvMessagePart iMessagePart;
		TMsvRelationOp iRelationOp;
		TBuf<KMsvMaxSearchTextLength> iQueryString;
	};//maximum limit of query table is depends on size of KMaxLevelOfSearchAndSort macro

#endif //__MSVSEARCHSORTCONSTANT_H__
