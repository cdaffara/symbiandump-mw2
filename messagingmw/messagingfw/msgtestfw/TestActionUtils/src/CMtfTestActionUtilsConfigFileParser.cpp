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

/**
 @file
*/


#include "CMtfTestActionUtilsConfigFileParser.h"

#include <f32file.h>

const TInt KFileBufferSize = 1024;
_LIT8(KComment,"*#*");


CMtfTestActionUtilsConfigFileParser* CMtfTestActionUtilsConfigFileParser::NewL(const TDesC& aFileName)
	{
	CMtfTestActionUtilsConfigFileParser* self = new (ELeave) CMtfTestActionUtilsConfigFileParser();
	CleanupStack::PushL(self);
	self->ConstructL(aFileName);
	CleanupStack::Pop();
	return self;
	}
	

CMtfTestActionUtilsConfigFileParser::CMtfTestActionUtilsConfigFileParser()
	{
	}


CMtfTestActionUtilsConfigFileParser::~CMtfTestActionUtilsConfigFileParser()
	{
	iName.ResetAndDestroy();
	iContent.ResetAndDestroy();
	iContent8.ResetAndDestroy();
	}


void CMtfTestActionUtilsConfigFileParser::ConstructL(const TDesC& aFileName)
	{
	RFs fs;
	fs.Connect();

	RFile file;
	User::LeaveIfError(file.Open(fs,aFileName,EFileRead));

	TInt eof = EFalse;
	TInt fileOffset = 0;
	TBuf8<KFileBufferSize> fileBuffer;

	while (!eof)
		{
		fileBuffer.SetLength(0);
		User::LeaveIfError(file.Read(fileOffset,fileBuffer,KFileBufferSize));
		TInt read = fileBuffer.Length();

		if (read < KFileBufferSize)
			{
			fileBuffer.Append('\n');
			eof = ETrue;
			}

		TInt lineOverflow = fileBuffer.Locate('\n');
		if ((lineOverflow == KErrNotFound) && (read == KFileBufferSize))
			User::Leave(KErrOverflow);

		TInt eol = EFalse;
		while (!eol)
			{
			TInt lineFeedLocation = fileBuffer.Locate('\n');
			if (lineFeedLocation == KErrNotFound)
				eol = ETrue;
			else
				{
				fileOffset += lineFeedLocation + 1;
				TInt lineLength;
				if ((lineFeedLocation != 0) && (fileBuffer[lineFeedLocation-1] == '\r'))
					lineLength = lineFeedLocation-1;
				else
					lineLength = lineFeedLocation;
				TPtrC8 line  = fileBuffer.Left(lineLength);
				TInt commentLocation = line.Match(KComment);
				if (commentLocation != KErrNotFound)
					{
					TPtrC8 skipComment = line.Left(commentLocation);
					line.Set(skipComment);
					}
				TInt seperatorLocation = line.Locate('=');
				if (seperatorLocation != KErrNotFound)
					if ((seperatorLocation == 0) || (seperatorLocation == line.Length()-1))
						seperatorLocation = KErrNotFound;
				if (seperatorLocation != KErrNotFound)
					{
					TPtrC8 namePtr = line.Left(seperatorLocation);
					HBufC8* nameBuf8 = HBufC8::NewL(namePtr.Length());
					CleanupStack::PushL(nameBuf8);
					TPtr8 name8 = nameBuf8->Des();
					name8.Copy(namePtr);
					name8.Trim();
					HBufC* nameBuf16 = HBufC::NewL(namePtr.Length());
					TPtr name16 = nameBuf16->Des();
					name16.Copy(name8);
					iName.Append(nameBuf16);
					CleanupStack::PopAndDestroy(nameBuf8);

					TPtrC8 contentPtr = line.Mid(seperatorLocation+1);
					HBufC8* contentBuf8 = HBufC8::NewL(contentPtr.Length());
					CleanupStack::PushL(contentBuf8);
					TPtr8 content8 = contentBuf8->Des();
					content8.Copy(contentPtr);
					content8.Trim();
					HBufC* contentBuf16 = HBufC::NewL(contentPtr.Length());
					TPtr content16 = contentBuf16->Des();
					content16.Copy(content8);
					iContent.Append(contentBuf16);
					iContent8.Append(contentBuf8);
					CleanupStack::Pop(contentBuf8);
					}
				TPtrC8 theRest = fileBuffer.Mid(lineFeedLocation+1);
				fileBuffer.Copy(theRest);
				}
			}
		}
	
	fs.Close();
	}


TInt CMtfTestActionUtilsConfigFileParser::GetFieldAsInteger(const TDesC& aFieldName,TInt& aValue)
	{
	TInt count = iName.Count();
	
	for (TInt i=0; i<count; i++)
		{
		if (iName[i]->Compare(aFieldName) == 0)
			{ 
			TPtrC content = iContent[i]->Des();
			TLex lex(content);
			lex.Val(aValue);
			return (KErrNone);
			}
		}
	
	return (KErrNotFound);
	}


TInt CMtfTestActionUtilsConfigFileParser::GetFieldAsString(const TDesC& aFieldName,TPtrC& aString)
	{
	TInt count = iName.Count();
	
	for (TInt i=0; i<count; i++)
		{
		if (iName[i]->Compare(aFieldName) == 0)
			{ 
			aString.Set(*iContent[i]);
			return (KErrNone);
			}
		}
	
	return (KErrNotFound);
	}


TInt CMtfTestActionUtilsConfigFileParser::GetFieldAsString8(const TDesC& aFieldName,TPtrC8& aString)
	{
	TInt count = iName.Count();
	
	for (TInt i=0; i<count; i++)
		{
		if (iName[i]->Compare(aFieldName) == 0)
			{ 
			aString.Set(*iContent8[i]);
			return (KErrNone);
			}
		}
	
	return (KErrNotFound);
	}


