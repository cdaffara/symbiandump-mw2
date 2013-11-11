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
//


#include <e32base.h>
#include <e32test.h>
#include <msvapi.h>

GLDEF_C TInt E32Main()
//
// Run the message server
//
    {
	TInt error = KErrNone;
	
	error = StartMessageServer(NULL);
	return error;
	}
