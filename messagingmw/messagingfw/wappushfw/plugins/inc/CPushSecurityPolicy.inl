// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Purpose:		Inline implementations of CPushSecurityPolicy functions
// 
// 
//

// Local includes
//
#include "CPushSecurityPolicy.h"

// System includes
//
#include <ecom/ecom.h>

inline CPushSecurityPolicy::CPushSecurityPolicy()
: CBase()
	{
	}

inline CPushSecurityPolicy::~CPushSecurityPolicy()
	{
	REComSession::DestroyedImplementation(iDtor_ID_Key);
	}

inline CPushSecurityPolicy* CPushSecurityPolicy::NewL(const TDesC& aMatchData)
	{
	// Convert match data to 8-bits
	HBufC8* buf = HBufC8::NewLC(aMatchData.Length());
	TPtr8 data = buf->Des();
	data.Copy(aMatchData);

	// Set resolving parameters
	TEComResolverParams resolverParams;
	resolverParams.SetDataType(data);
	resolverParams.SetWildcardMatch(ETrue);

	// Get the instantiation

	CPushSecurityPolicy* securityPolicy = REINTERPRET_CAST(CPushSecurityPolicy*,
										  REComSession::CreateImplementationL(KUidPushSecurityPolicy,
																			  _FOFF(CPushSecurityPolicy, iDtor_ID_Key),
																			  resolverParams));
	CleanupStack::PopAndDestroy(buf);
	return securityPolicy;
	}

