// Copyright (c) 1997-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <bif.h>

#include <barsc.h>
#include <barsread.h>

#include <e32cons.h>
#include <e32test.h>


#define KBifFileName _L("out.bif")

#pragma warning(disable:4238)  // nonstandard extension used : class rvalue used as lvalue
#pragma warning(disable:4701)  // warning: local variable 'bitmap' may be used without having been initialized

//
// Test data...


#define KMessageParserName _L("ParserName")

#define KDescription _L("Description text. This is all very exciting...")
#define KFileExtension _L("vcf")


// Icons array
const TFileName KIconsFilename=_L("test.mbm");

// Id array
// end of test data.
//


RTest test(_L("BIF Read/Write Test"));
const TInt KTestCleanupStack=0x40;
LOCAL_D CTrapCleanup* TheTrapCleanup;
LOCAL_D RFs Fs;

LOCAL_C void setupCleanup()
//
// Initialise the cleanup stack.
//
    {
	TheTrapCleanup=CTrapCleanup::New();
	TRAPD(r,\
		{\
		for (TInt i=KTestCleanupStack;i>0;i--)\
			CleanupStack::PushL((TAny*)1);\
		CleanupStack::Pop(KTestCleanupStack);\
		});
	}




GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;

	setupCleanup();

	test(Fs.Connect()==KErrNone);

	test.Start(_L("CBioInfoFileWriter removed in Messaging API V2"));
	
	test.Printf(_L("Press a key to continue..."));
	test.Getch();	// pause for user

	Fs.Close();

	delete TheTrapCleanup;
	test.End();
	test.Close();
	
	__UHEAP_MARKEND;
	return KErrNone;
	}
