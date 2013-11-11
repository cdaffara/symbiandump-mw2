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
// The implementation of the accessor class upon the CSmsSchemeHandler
// 
//

/**
 @file
 @SYMCreationDate Created : 01/06/01 14:07:23
*/

_LIT(KTSmsSchemeHandler_DumpName,"C:\\System\\Data\\Logs\\TSmsSchemeHandler_StateDump.bin");

inline TInt TSmsSchemeHandler_StateAccessor::InvariantTest(TAny* aTestObject)
	{
	if(aTestObject)
		{
		// CSmsSchemeHandler* SmsSchemeHandler = REINTERPRET_CAST(CSmsSchemeHandler*, aTestObject);
		}
	return KErrNone;
	}

inline TInt TSmsSchemeHandler_StateAccessor::Internalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		CSmsSchemeHandler* SmsSchemeHandler = REINTERPRET_CAST(CSmsSchemeHandler*, aTestObject);
		TRAP(error,InternalizeL(SmsSchemeHandler));
		}
	return error;
	}

inline TInt TSmsSchemeHandler_StateAccessor::Externalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		CSmsSchemeHandler* SmsSchemeHandler = REINTERPRET_CAST(CSmsSchemeHandler*, aTestObject);
		TRAP(error,ExternalizeL(SmsSchemeHandler));
		}
	return error;
	}

inline void TSmsSchemeHandler_StateAccessor::InternalizeL(RFileReadStream& /* aStream */, CSmsSchemeHandler* /* aLoadManager */)
	{
	}

inline void TSmsSchemeHandler_StateAccessor::ExternalizeL(RFileWriteStream& /* aStream */, CSmsSchemeHandler* /* aLoadManager */)
	{
	}

inline void TSmsSchemeHandler_StateAccessor::ParseUrlL(CSmsSchemeHandler& aSms, const TDesC& aUrl)
	{
	aSms.ParseUrlL(aUrl);
	}

inline RArray<SSmsNumber>& TSmsSchemeHandler_StateAccessor::GetSmsArray(CSmsSchemeHandler& aSms)
	{
	return aSms.iNumbers;
	}

// internal helpers
inline void TSmsSchemeHandler_StateAccessor::InternalizeL(CSmsSchemeHandler* aSmsSchemeHandler)
	{
	// Read in the dumped data
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Read the object dump
	TParse filename;
	filename.Set(KTSmsSchemeHandler_DumpName(),NULL,NULL);
	RFileReadStream stream;
	User::LeaveIfError(stream.Open(fs,filename.FullName(), EFileRead));
	CleanupClosePushL(stream);
	InternalizeL(stream, aSmsSchemeHandler);
	CleanupStack::PopAndDestroy();	// stream
	CleanupStack::PopAndDestroy();	// fs
	}

inline void TSmsSchemeHandler_StateAccessor::ExternalizeL(CSmsSchemeHandler* aSmsSchemeHandler)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Write out the object dump
	TParse filename;
	filename.Set(KTSmsSchemeHandler_DumpName(),NULL,NULL);
	RFileWriteStream stream;
	User::LeaveIfError(stream.Replace(fs,filename.FullName(), EFileWrite));
	CleanupClosePushL(stream);
	ExternalizeL(stream, aSmsSchemeHandler);
	CleanupStack::PopAndDestroy();	// stream
	CleanupStack::PopAndDestroy();	// fs
	}

