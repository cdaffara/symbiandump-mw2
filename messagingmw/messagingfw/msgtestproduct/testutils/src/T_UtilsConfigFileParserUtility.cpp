// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This is the CPP file which contains utility functions for parsing the config file
// 
//

/**
 @file
*/


// User include
#include <t_utilsconfigfileparserutility.h>

// epoc inlcude
#include <f32file.h>

// contants and literals used
const TInt KFileBufferSize = 1024;
_LIT8(KComment,"*#*");


/**
NewL()
Allocates and creates a new CT_MsgUtilsConfigFileParserUtility object

@param aFileName
Name of the file to be parsed

@leave KErrNoMemory
@return
The newly created CT_MsgUtilsConfigFileParserUtility object
*/
EXPORT_C CT_MsgUtilsConfigFileParserUtility* CT_MsgUtilsConfigFileParserUtility::NewL(const TDesC& aFileName)
	{
	CT_MsgUtilsConfigFileParserUtility* self = new (ELeave) CT_MsgUtilsConfigFileParserUtility();
	CleanupStack::PushL(self);
	self->ConstructL(aFileName);
	CleanupStack::Pop();
	return self;
	}
	

/**
CT_MsgUtilsConfigFileParserUtility()
Constructor
*/
EXPORT_C CT_MsgUtilsConfigFileParserUtility::CT_MsgUtilsConfigFileParserUtility()
	{
	}


/**
~CT_MsgUtilsConfigFileParserUtility()
Destructor
*/
CT_MsgUtilsConfigFileParserUtility::~CT_MsgUtilsConfigFileParserUtility()
	{
	iName.ResetAndDestroy();
	iContent.ResetAndDestroy();
	iContent8.ResetAndDestroy();
	}


/**
ConstructL()
Parses a .txt file and creates Arrays of fields and there values

@param aFileName
Name of the file to be parsed.
*/
EXPORT_C void CT_MsgUtilsConfigFileParserUtility::ConstructL(const TDesC& aFileName)
	{
	RFs fileServerSession;

	fileServerSession.Connect();

	RFile file;
	User::LeaveIfError(file.Open(fileServerSession, aFileName, EFileRead));

	TInt eof = EFalse;
	TInt fileOffset = 0;
	TBuf8<KFileBufferSize> fileBuffer;

	while (!eof)
		{
		fileBuffer.SetLength(0);
		User::LeaveIfError(file.Read(fileOffset, fileBuffer, KFileBufferSize));
		TInt read = fileBuffer.Length();

		if (read < KFileBufferSize)
			{
			fileBuffer.Append('\n');
			eof = ETrue;
			}

		TInt lineOverflow = fileBuffer.Locate('\n');
		
		if ((lineOverflow == KErrNotFound) && (read == KFileBufferSize))
			{
			User::Leave(KErrOverflow);
			}

		TInt eol = EFalse;
		
		while (!eol)
			{
			TInt lineFeedLocation = fileBuffer.Locate('\n');
			
			if (lineFeedLocation == KErrNotFound)
				{
				eol = ETrue;
				}
			
			else
				{
				fileOffset += lineFeedLocation + 1;
				TInt lineLength;
				if ((lineFeedLocation != 0) && (fileBuffer[lineFeedLocation - 1] == '\r'))
					{
					lineLength = lineFeedLocation - 1;
					}
					
				else
					{
					lineLength = lineFeedLocation;
					}
					
				TPtrC8 line  = fileBuffer.Left(lineLength);
				TInt commentLocation = line.Match(KComment);
				
				if (commentLocation != KErrNotFound)
					{
					TPtrC8 skipComment = line.Left(commentLocation);
					line.Set(skipComment);
					}
					
				TInt seperatorLocation = line.Locate('=');
				
				if (seperatorLocation != KErrNotFound)
					{
					if ((seperatorLocation == 0) || (seperatorLocation == line.Length() - 1))
						{
						seperatorLocation = KErrNotFound;
						}
					}
					
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

					TPtrC8 contentPtr = line.Mid(seperatorLocation + 1);
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
				TPtrC8 theRest = fileBuffer.Mid(lineFeedLocation + 1);
				fileBuffer.Copy(theRest);
				}
			}
		}
	file.Close();
	fileServerSession.Close();
	}


/**
GetFieldAsInteger()
Retrives the content of a field name and Interpret it as an integer.

@param aFieldName
@param aValue
@return
Returns an integer corresponding to the field
*/
EXPORT_C TInt CT_MsgUtilsConfigFileParserUtility::GetFieldAsInteger(const TDesC& aFieldName, TInt& aValue)
	{
	TInt count = iName.Count();

	for (TInt i = 0; i<count; i++)
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


/**
GetFieldAsString()
Retrives the content of a field name and Interpret it as an string

@param aFieldName
@param aValue
@return
Returns an string corresponding to the field
*/
EXPORT_C TInt CT_MsgUtilsConfigFileParserUtility::GetFieldAsString(const TDesC& aFieldName, TPtrC& aString)
	{
	TInt count = iName.Count();

	for (TInt i = 0; i<count; i++)
		{
		if (iName[i]->Compare(aFieldName) == 0)
			{ 
			aString.Set(*iContent[i]);
			return (KErrNone);
			}
		}
	return (KErrNotFound);
	}


/**
GetFieldAsString8()
Retrives the content of a field name and Interpret it as an 8-bit descriptor

@param aFieldName
@param aValue
@return
Returns an 8-bit descriptor corresponding to the field
*/
EXPORT_C TInt CT_MsgUtilsConfigFileParserUtility::GetFieldAsString8(const TDesC& aFieldName, TPtrC8& aString)
	{
	TInt count = iName.Count();
	
	for (TInt i = 0; i<count; i++)
		{
		if (iName[i]->Compare(aFieldName) == 0)
			{ 
			aString.Set(*iContent8[i]);
			return (KErrNone);
			}
		}
	return (KErrNotFound);
	}



