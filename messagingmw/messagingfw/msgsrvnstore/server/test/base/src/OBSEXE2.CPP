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
#include <f32file.h>

LOCAL_C void StartL()
	{
	RFs theFs;
	theFs.Connect();

	_LIT(KObserverExeTestFile, "obsexe2");

	RFile file;
	file.Create(theFs, KObserverExeTestFile, EFileShareExclusive);
	file.Close();

	theFs.Close();
	}
 
GLDEF_C TInt E32Main()
	{       
	__UHEAP_MARK;
	CTrapCleanup* theCleanup=CTrapCleanup::New();
	TRAPD(ret,StartL());          
	delete theCleanup;      
	__UHEAP_MARKEND;
	return(KErrNone);
	}


