// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef __CBIOASYNCWAITER_H__
#define __CBIOASYNCWAITER_H__

#include <e32base.h>

/**
Utility class for waiting for asychronous requests.

This class allows asynchronous requests to be made from synchronous
objects. This object is used by passing its iStatus to an asynchronous
request and then calling Start(). The result of the request can be 
obtained by calling the Result() method.

@publishedAll
@released
*/
class CBioAsyncWaiter : public CActive
	{
public:
	IMPORT_C static CBioAsyncWaiter* NewLC();
	IMPORT_C ~CBioAsyncWaiter();
	
	IMPORT_C void StartAndWait();
	IMPORT_C TInt Result() const;
	
private:
	CBioAsyncWaiter();
	
	// from CActive
	virtual void RunL();
	virtual void DoCancel();
	
private:
	TInt iError;
	};

#endif // __CBIOASYNCWAITER_H__
