// Copyright (c) 2003-2009 Nokia Corporation and/or its subsidiary(-ies).
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
  
#include <e32std.h>



#ifndef EKA2
/** DLL entry point */
GLDEF_C TInt E32Dll(TDllReason/* aReason*/)	
	{
	return (KErrNone);
	}
#endif

EXPORT_C void DummyExport()
	{
	}

