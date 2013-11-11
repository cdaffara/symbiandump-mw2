// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

#ifndef __MSGERR_H__
#define __MSGERR_H__

/*
The search-sort query is too big to be executed. 
This could be due to a bigger value for the search-sort option provided 
as part of the query. The query value added to a particular option is 
greater then KMsvMaxFindTextLength( = 300)
@publishedPartner
@released
**/
const TInt KErrQueryTooBig = -60;

/*
An addition of search-sort operation has  lead to a condition where the query 
options have exceeded the maximum number of search-sort options supported by 
messaging framework. The maximum search options that can be added to a 
search-sort query is 'five' and the maximum sort option that can be can be added is 'one'
@publishedPartner
@released
**/
const TInt KErrMsvQueryMaxLimit = -61;

/*
If the result type requested during execution of query was set to TMsvId, 
but the messaging client is trying to receive TMsvEntry objects 
@publishedPartner
@released
**/
const TInt KErrMsvInvalidResultRequest = -62;

/*
An invalid operation is being performed on a particular message part.
example: TMsvMessagePart is EMsvBcc and TMsvRelationOp is EMsvLessThanOrEqual
@publishedPartner
@released
**/ 
const TInt KErrMsvInvalidOperator = -63;


#endif // __MSGERR_H__
