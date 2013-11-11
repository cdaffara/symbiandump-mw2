// Copyright (c) 2001-2009 Nokia Corporation and/or its subsidiary(-ies).
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

// Local includes
//
#include "CWapPushMessageFilter.h"

// System includes
//
#include <ecom/ecom.h>

inline CWapPushMessageFilter::CWapPushMessageFilter()
: CBase()
	{
	}

inline CWapPushMessageFilter::~CWapPushMessageFilter()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}


inline CWapPushMessageFilter* CWapPushMessageFilter::NewL()
	{
	TBuf8<1> data;
	_LIT8(KWildCard, "*");
	data.Copy(KWildCard);

	// Set resolving parameters
	TEComResolverParams resolverParams;
	resolverParams.SetDataType(data);
	resolverParams.SetWildcardMatch(ETrue);

	// Get the instantiation
	CWapPushMessageFilter* pushFilter = REINTERPRET_CAST(CWapPushMessageFilter*,
										REComSession::CreateImplementationL(KUidPushMessageFilter,
																			_FOFF(CWapPushMessageFilter, iDtor_ID_Key),
																			resolverParams));
	return pushFilter;
	}
