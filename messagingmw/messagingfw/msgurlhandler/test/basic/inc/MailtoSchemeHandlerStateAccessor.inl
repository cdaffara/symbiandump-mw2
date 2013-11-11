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
// The implementation of the accessor class upon the CMailtoSchemeHandler
// 
//

/**
 @file
 @SYMCreationDate Created : 24/05/01 15:57:46
*/

// ______________________________________________________________________________
//
_LIT(KTMailtoSchemeHandler_DumpName,"C:\\System\\Data\\Logs\\TMailtoSchemeHandler_StateDump.bin");

inline TInt TMailtoSchemeHandler_StateAccessor::InvariantTest(TAny* aTestObject)
	{
	if(aTestObject)
		{
		// CMailtoSchemeHandler* MailtoSchemeHandler = REINTERPRET_CAST(CMailtoSchemeHandler*, aTestObject);
		}
	return KErrNone;
	}

inline TInt TMailtoSchemeHandler_StateAccessor::Internalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		CMailtoSchemeHandler* MailtoSchemeHandler = REINTERPRET_CAST(CMailtoSchemeHandler*, aTestObject);
		TRAP(error,InternalizeL(MailtoSchemeHandler));
		}
	return error;
	}

inline TInt TMailtoSchemeHandler_StateAccessor::Externalize(TAny* aTestObject)
	{
	TInt error = KErrNone;
	if(aTestObject != NULL)
		{
		CMailtoSchemeHandler* MailtoSchemeHandler = REINTERPRET_CAST(CMailtoSchemeHandler*, aTestObject);
		TRAP(error,ExternalizeL(MailtoSchemeHandler));
		}
	return error;
	}

inline void TMailtoSchemeHandler_StateAccessor::InternalizeL(RFileReadStream& /* aStream */, CMailtoSchemeHandler* /* aLoadManager */)
	{
	}

inline void TMailtoSchemeHandler_StateAccessor::ExternalizeL(RFileWriteStream& /* aStream */, CMailtoSchemeHandler* /* aLoadManager */)
	{
	}

inline void TMailtoSchemeHandler_StateAccessor::ParseUrlL(CMailtoSchemeHandler& aMailTo, const TDesC& aUrl)
	{
	aMailTo.ParseUrlL(aUrl);
	}

inline RArray<TPtrC>& TMailtoSchemeHandler_StateAccessor::GetRecipientArray(CMailtoSchemeHandler& aMailTo)
	{
	return aMailTo.iTo;
	}

inline TPtrC& TMailtoSchemeHandler_StateAccessor::GetSubject(CMailtoSchemeHandler& aMailTo)
	{
	return aMailTo.iSubject;
	}

inline TPtrC& TMailtoSchemeHandler_StateAccessor::GetBody(CMailtoSchemeHandler& aMailTo)
	{
	return aMailTo.iBody;
	}


// internal helpers
inline void TMailtoSchemeHandler_StateAccessor::InternalizeL(CMailtoSchemeHandler* aMailtoSchemeHandler)
	{
	// Read in the dumped data
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Read the object dump
	TParse filename;
	filename.Set(KTMailtoSchemeHandler_DumpName(),NULL,NULL);
	RFileReadStream stream;
	User::LeaveIfError(stream.Open(fs,filename.FullName(), EFileRead));
	CleanupClosePushL(stream);
	InternalizeL(stream, aMailtoSchemeHandler);
	CleanupStack::PopAndDestroy();	// stream
	CleanupStack::PopAndDestroy();	// fs
	}

inline void TMailtoSchemeHandler_StateAccessor::ExternalizeL(CMailtoSchemeHandler* aMailtoSchemeHandler)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	CleanupClosePushL(fs);
	// Write out the object dump
	TParse filename;
	filename.Set(KTMailtoSchemeHandler_DumpName(),NULL,NULL);
	RFileWriteStream stream;
	User::LeaveIfError(stream.Replace(fs,filename.FullName(), EFileWrite));
	CleanupClosePushL(stream);
	ExternalizeL(stream, aMailtoSchemeHandler);
	CleanupStack::PopAndDestroy();	// stream
	CleanupStack::PopAndDestroy();	// fs
	}

