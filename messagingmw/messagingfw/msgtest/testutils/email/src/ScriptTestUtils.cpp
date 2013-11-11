// Copyright (c) 2000-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "emailtestutils.h"
#include "scripttestutils.h"

_LIT8(K_IM_CR_LF, "\r\n");
_LIT(K_IM_LOG_FILE_NAME, "c:\\logs\\email\\imlog%d.txt");
_LIT(K_IM_END_OF_FILE, "END OF FILE");
_LIT(K_IM_LOG_TEMP_FILE_NAME, "temp.out");
_LIT8(K_IM_READ_CANCELLED, "** Read cancelled");

_LIT8(K_IM_TEST_DATE, "Date: ");
_LIT8(K_IM_TEST_DATE_REPLACEMENT, "date-replacement-string-");
_LIT8(K_IM_TEST_MESSAGE_ID, "Message-ID: ");
_LIT8(K_IM_TEST_MESSAGE_ID_REPLACEMENT, "message-id-replacement-string-");
_LIT8(K_IM_TEST_CONTENT_ID, "Content-ID: ");
_LIT8(K_IM_TEST_CONTENT_ID_REPLACEMENT, "content-id-replacement-string-");
_LIT8(K_IM_TEST_BOUNDARY, "  boundary=");
_LIT8(K_IM_TEST_BOUNDARY_REPLACEMENT, "boundary-replacement-string-");
_LIT8(K_IM_TEST_HELO, "HELO ");
_LIT8(K_IM_TEST_HELO_REPLACEMENT, "");
const TInt K_IM_TEST_STRING_MAX = 128;
const TInt K_IM_TEST_ID_LEN = 5;


CScriptTestUtils::CImTestField::~CImTestField()
	{
	delete iFieldName;
	delete iReplacementString;
	}

CScriptTestUtils::CImFindReplace::~CImFindReplace()
	{
	delete iFind;
	delete iReplace;
	}

EXPORT_C void CScriptTestUtils::LogToInputFileL(const TDesC& aSourceFileName, const TDesC& aDestFileName, TBool aStandardiseDateFields)
// Create an email script file from a log file.
	{
	StripLogFileL(aSourceFileName, aDestFileName, aStandardiseDateFields, EImLogLineIn);
	}

EXPORT_C void CScriptTestUtils::LogToOutputFileL(const TDesC& aSourceFileName, const TDesC& aDestFileName, TBool aStandardiseDateFields)
// Create an email output file from a log file.
	{
	StripLogFileL(aSourceFileName, aDestFileName, aStandardiseDateFields, EImLogLineOut);
	}

CScriptTestUtils::CImFindReplace* CScriptTestUtils::FindVariableDataL(TDes8& aLine)
// Find any variable data fields in the given line.
// The field value and its replacement is returned.
// If the line doesn't contain any variable data then a NULL pointer is returned.
	{
	CImFindReplace* replacement = NULL;

	TInt index = iFieldList->Count();
	HBufC8* fieldName;
	TPtrC8 startOfLine;
	TInt lineIndex = 0;
	while (index--)
	// Look for each variable field in turn.
		{
		fieldName = ((*iFieldList)[index]->iFieldName);
		lineIndex = fieldName->Des().Size();
		if (lineIndex <= aLine.Size())
			{
			startOfLine.Set(aLine.Left(lineIndex));

			if (fieldName->Des().CompareF(startOfLine) == 0)
				{
				TPtrC8 fieldValue;
				fieldValue.Set(aLine.Mid(lineIndex, aLine.Size() - lineIndex));
				if (fieldValue.Size() > 1)
				// Don't include enclosing '<' '>' '"' characters in the value part.
					{
					if (((fieldValue[0] == '<') && (fieldValue[fieldValue.Size() - 1] == '>'))
						|| ((fieldValue[0] == '"') && (fieldValue[fieldValue.Size() - 1] == '"'))
						|| ((fieldValue[0] == '[') && (fieldValue[fieldValue.Size() - 1] == ']')))
						{
						fieldValue.Set(fieldValue.Mid(1, fieldValue.Size() - 2));
						}
					}
		
				// Set 
				if (fieldValue.Size() > 0)
					{
					replacement = new (ELeave) CImFindReplace;
					replacement->iFind = HBufC8::NewL(fieldValue.Size());
					*(replacement->iFind) = fieldValue;

					replacement->iReplace = HBufC8::NewL((*iFieldList)[index]->iReplacementString->Des().Size() + K_IM_TEST_ID_LEN);
					*(replacement->iReplace) = (*iFieldList)[index]->iReplacementString->Des();
					// Append the ID number to the replacement string.
					TPtr8 replacementString = replacement->iReplace->Des();
					TBuf8<K_IM_TEST_ID_LEN> numberString;
					(*iFieldList)[index]->iNumberFound++;
					numberString.Num((*iFieldList)[index]->iNumberFound);
					if ((replacementString.Size() != 0) && (index != 0))
						replacementString.Append(numberString);
					}
				}
			}
		}

	return replacement;
	}

void CScriptTestUtils::ReplaceVariableFields(TDes8& aLine)
// Replaces any variable data in the given descriptor with a standard equivalent.
	{
	TInt index = 0;
	index = iFindReplaceList->Count();
	TInt stringPosition;
	HBufC8* findString;
	HBufC8* replaceString;
	while (index--)
	// Search for each bit of variable data in turn
		{
		findString = (*iFindReplaceList)[index]->iFind;
		replaceString = (*iFindReplaceList)[index]->iReplace;
		stringPosition = aLine.FindF(findString->Des());
		if (stringPosition != KErrNotFound)
			{
			aLine.Replace(stringPosition, findString->Des().Size(), replaceString->Des());
			}
		}
	}

void CScriptTestUtils::StandardiseVariableFieldsL(const TDesC& aSourceFileName)
// Write over all the MIME variable field values with standard values.
	{
	TInt index = 0;
	index = iFieldList->Count();
	while (index--)
		{
		(*iFieldList)[index]->iNumberFound = 0;
		}

	// Open and create the files
	RFile sourceFile;
	RFile tempFile;
	TFileName tempFileName;
	index = 0;
	while (aSourceFileName[index] != L'.')
		{
		tempFileName.Append(aSourceFileName[index]);
		index++;
		}
	tempFileName.Append(_L(".tmp"));

	TInt err = tempFile.Replace(iTestUtils.FileSession(), tempFileName, EFileShareAny);

	// Open the log file.
	err = sourceFile.Open(iTestUtils.FileSession(), aSourceFileName, EFileShareReadersOnly);
	TFileReader fileReader(sourceFile);

	HBufC8* sourceLine = GetLineL(fileReader);
	CImFindReplace* findReplace;
	while (0 != sourceLine)
	// Fill in the find and replace table.
		{
		TPtr8 linePtr = sourceLine->Des();
		CleanupStack::PushL(sourceLine);
		findReplace = FindVariableDataL(linePtr);
		if (findReplace)
		// Add the find and replace strings to the list
			{
			iFindReplaceList->AppendL(findReplace);
			}

		CleanupStack::PopAndDestroy(); // sourceLine
		sourceLine = GetLineL(fileReader);
		}

	// Go back to the begining of the source file.
	fileReader.Seek(0);

	sourceLine = GetLineL(fileReader);
	// Replace all of the variable field values with the appropriate fixed string.
	while (0 != sourceLine)
		{
		TPtr8 linePtr = sourceLine->Des();
		CleanupStack::PushL(sourceLine);

		TPtr8 sourceLinePtr = sourceLine->Des();
		ReplaceVariableFields(sourceLinePtr);

		tempFile.Write(sourceLinePtr);
		tempFile.Write(K_IM_CR_LF);

		CleanupStack::PopAndDestroy(); // sourceLine
		sourceLine = GetLineL(fileReader);
		if (sourceLine)
			linePtr = sourceLine->Des();
		}

	// Close the files.
	sourceFile.Close();
	tempFile.Close();

	// Delete the source file.
	err = iTestUtils.FileSession().Delete(aSourceFileName);

	// Rename the temp file as the source file.
	err = iTestUtils.FileSession().Rename(tempFileName, aSourceFileName);
	}


void CScriptTestUtils::StripLogFileL(const TFileName& aSourceFileName, const TFileName& aDestFileName, TBool aStandardiseDateFields, TImLogLineType aLogLineType)
	{
	TBuf8<2> crlf = _L8("\r\n");

	RFile rSourceFile;
	RFile rOutputFile;

	User::LeaveIfError(rOutputFile.Replace(iTestUtils.FileSession(), aDestFileName, EFileShareAny));

	// Open the log file.
	User::LeaveIfError(rSourceFile.Open(iTestUtils.FileSession(), aSourceFileName, EFileShareAny));

	TBuf8<2048> scriptLine;
	TFileReader fileReader(rSourceFile);

	// Copy the incoming data from the log to the script file, removing any formatting.
	TInt lineCounter = 0;
	TInt lineIndex;
	TImLogLineType lastLineType = aLogLineType;
	TBool waitingForCrLf = EFalse;

	HBufC8* logLine = GetLineL(fileReader);
	while (logLine != 0)
		{
		CleanupStack::PushL(logLine);
		lineCounter++;
		lineIndex = 0;

		scriptLine.Zero();
		TImLogLineType lineType = LineType(logLine);
		if (EImEmptyLine == lineType)
			{
			if (waitingForCrLf)
				{
				waitingForCrLf = EFalse;
				rOutputFile.Write(crlf);
				}
			}
		else if (aLogLineType == lineType)
			{
			if ((lastLineType != EImLogLineComment) && (waitingForCrLf))
				{
				rOutputFile.Write(crlf);
				}

			// Copy the line of incoming data to the script file.
			if (logLine->Size() > 21)
				{
				// Copy the logLine from character 20 to the end into the output line.
				lineIndex = 21;
				while (lineIndex < logLine->Size())
					{
					scriptLine.Append((*logLine)[lineIndex]);
					lineIndex++;
					}
				
				// Write the stripped line to the script file
				User::LeaveIfError(rOutputFile.Write(scriptLine));
				}

			waitingForCrLf = ETrue;
				// 
			}
		else if (EImLogLineComment == lineType)
			{}
		else if (EImReadCancelled == lineType)
			{
			if (EImLogLineIn == aLogLineType)
				{
				User::LeaveIfError(rOutputFile.Write(K_IM_READ_CANCELLED));
				rOutputFile.Write(crlf);
				}
			}
		else
			{
			// If we're waiting for a crlf then write one regardless of the last line...
			if (waitingForCrLf)
				{
				rOutputFile.Write(crlf);
				}
			}

		lastLineType = LineType(logLine);

		CleanupStack::PopAndDestroy(); // logLine
		logLine = GetLineL(fileReader);
		}

	if (waitingForCrLf)
		{
		rOutputFile.Write(crlf);
		}
	
	rSourceFile.Close();
	rOutputFile.Close();

	if (aStandardiseDateFields)
		{
		StandardiseVariableFieldsL(aDestFileName);
		}
	}

EXPORT_C CScriptTestUtils* CScriptTestUtils::NewLC(CEmailTestUtils& aTestUtils)
	{
	CScriptTestUtils* self = new (ELeave) CScriptTestUtils(aTestUtils);
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

void CScriptTestUtils::ConstructL()
	{
	iFieldList =  new (ELeave) CArrayPtrFlat<CImTestField>(6);

	CImTestField* removeReplaceItem = new (ELeave) CImTestField;
	removeReplaceItem->iFieldName = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iFieldName) = K_IM_TEST_DATE;
	removeReplaceItem->iReplacementString = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iReplacementString) = K_IM_TEST_DATE_REPLACEMENT;
	removeReplaceItem->iNumberFound=0;
	iFieldList->AppendL(removeReplaceItem);

	removeReplaceItem = new (ELeave) CImTestField;
	removeReplaceItem->iFieldName = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iFieldName) = K_IM_TEST_MESSAGE_ID;
	removeReplaceItem->iReplacementString = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iReplacementString) = K_IM_TEST_MESSAGE_ID_REPLACEMENT;
	removeReplaceItem->iNumberFound=0;
	iFieldList->AppendL(removeReplaceItem);

	removeReplaceItem = new (ELeave) CImTestField;
	removeReplaceItem->iFieldName = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iFieldName) = K_IM_TEST_CONTENT_ID;
	removeReplaceItem->iReplacementString = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iReplacementString) = K_IM_TEST_CONTENT_ID_REPLACEMENT;
	removeReplaceItem->iNumberFound=0;
	iFieldList->AppendL(removeReplaceItem);

	removeReplaceItem = new (ELeave) CImTestField;
	removeReplaceItem->iFieldName = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iFieldName) = K_IM_TEST_BOUNDARY;
	removeReplaceItem->iReplacementString = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iReplacementString) = K_IM_TEST_BOUNDARY_REPLACEMENT;
	removeReplaceItem->iNumberFound=0;
	iFieldList->AppendL(removeReplaceItem);

	removeReplaceItem = new (ELeave) CImTestField;
	removeReplaceItem->iFieldName = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iFieldName) = K_IM_TEST_HELO;
	removeReplaceItem->iReplacementString = HBufC8::NewL(K_IM_TEST_STRING_MAX);
	*(removeReplaceItem->iReplacementString) = K_IM_TEST_HELO_REPLACEMENT;
	removeReplaceItem->iNumberFound=0;
	iFieldList->AppendL(removeReplaceItem);

	iFindReplaceList = new (ELeave) CArrayPtrFlat<CImFindReplace>(6);
	}

CScriptTestUtils::CScriptTestUtils(CEmailTestUtils& aTestUtils) : iTestUtils(aTestUtils)
	{}


CScriptTestUtils::TImLogLineType CScriptTestUtils::LineType(const HBufC8* aLine)
	{
	TImLogLineType lineType = EImLogLineComment;

	if (aLine->Size() == 0)
		{
		lineType = EImEmptyLine;
		}
	else if (aLine->Size() > 19)
		{
		if ((((*aLine)[18]) == '<')
			&& (((*aLine)[19]) == '<'))
			{
			lineType = EImLogLineIn;
			}
		else if ((((*aLine)[18]) == '>')
			&& (((*aLine)[19]) == '>'))
			{
			lineType = EImLogLineOut;
			}
		else if ((aLine->Right(17)).Compare(K_IM_READ_CANCELLED) == 0)
			{
			lineType = EImReadCancelled;
			}
		}

	return lineType;
	}

HBufC8* CScriptTestUtils::GetLineL(TFileReader& aFileReader)
	{
	TBuf8<2048> currentLine;
	char character;
	currentLine.Zero();


	TBool crlfFound = EFalse;
	TBool read = ETrue;

	// Append characters one at a time until we reach a cr/lf pair or the end of the file.
	while ((read) && (crlfFound == EFalse))
		{
		read = aFileReader.Read(character);

		if (read)
			{
			if (0x0D == character)
				{
				read = aFileReader.Read(character);
				if (read)
					{
					if (character == 0x0A)
						{
						crlfFound = ETrue;
						}
					else
						{
						// The cr was not part of the cr/lf pair so go back one space.
						aFileReader.SeekCurrent(-1);
						}
					}
				}

			if ((read) && (EFalse == crlfFound))
				{
				currentLine.Append(character);
				}
			}
		}
	
	HBufC8* lineCopy;
	if ((currentLine.Size() != 0) || (read))
		{
		lineCopy = HBufC8::NewL(2048);
		(*lineCopy) = currentLine;
		}
	else
		{
		lineCopy = 0;
		}

	return lineCopy;
	}


CScriptTestUtils::TFileReader::TFileReader(RFile &rFile) : iFile(rFile), iBufferIndex(0), iBufferStart(0)
	{
	}

TBool CScriptTestUtils::TFileReader::Read(char &rChar)
	{
	// Is the character we want not in the buffer ?
	TBool read = ETrue;
	if ((iBufferIndex >= iBuffer.Size()) 
		|| (iBufferIndex < 0))
		// If it's not then get a new buffer starting at this position.
		{
		TInt seekPosition = iBufferIndex + iBufferStart;
		TInt err = iFile.Seek(ESeekStart, seekPosition);
		if (KErrNone != err)
			{
			iBuffer.SetLength(0);
			read = EFalse;
			}
		else
			{
			iFile.Read(iBuffer);
			if (iBuffer.Size() == 0)
				{
				read = EFalse;
				}
			iBufferStart += iBufferIndex;
			iBufferIndex = 0;
			}

		if (iBuffer.Size() > 0)
			{
			rChar = iBuffer[0];
			}
		}
	else
		{
		rChar = iBuffer[iBufferIndex];
		}

	if (read)
		{
		iBufferIndex++;
		}

	return read;
	}

void CScriptTestUtils::TFileReader::Seek(TInt aOffset)
	{
	iBufferIndex = aOffset;
	iBuffer.Zero();
	iBufferStart = 0;
	}

void CScriptTestUtils::TFileReader::SeekCurrent(TInt aOffset)
	{
	iBufferIndex += aOffset;
	}


CScriptTestUtils::~CScriptTestUtils()
	{
	delete iExpectedLine;
	delete iActualLine;
	iFieldList->ResetAndDestroy();
	delete iFieldList;
	iFindReplaceList->ResetAndDestroy();
	delete iFindReplaceList;
	}

// Functions for checking the IM output.

EXPORT_C TBool CScriptTestUtils::CheckLogOutputL(TInt aPortNumber, const TDesC& aTestFileName)
	{
	TBool matched = ETrue;

	// Clear the old error values
	delete iExpectedLine;
	iExpectedLine = 0;
	delete iActualLine;
	iActualLine = 0;

	RFile testFile; // The file to test against
	RFile tempFile; // The stripped-down log file to test

	// Create a temporary .out file from the log file.
	TFileName logFileName;
	logFileName.Format(K_IM_LOG_FILE_NAME, aPortNumber);

	TParse tempFileName;
	iTestUtils.ResolveLogFile(K_IM_LOG_TEMP_FILE_NAME, tempFileName);
	LogToOutputFileL(logFileName, tempFileName.FullName(), ETrue);
	tempFile.Close();

	// Open the test file.
	TInt err = testFile.Open(iTestUtils.FileSession(), aTestFileName, EFileShareReadersOnly);
	TFileReader testFileReader(testFile);

	// Open the temp file.
	TParse tempLogFileName;
	iTestUtils.ResolveLogFile(K_IM_LOG_TEMP_FILE_NAME, tempLogFileName);
	err = tempFile.Open(iTestUtils.FileSession(), tempFileName.FullName(), EFileShareReadersOnly);
	TFileReader tempFileReader(tempFile);

	HBufC8* testLine = GetLineL(testFileReader);

	if (testLine)
		CleanupStack::PushL(testLine);

	HBufC8* tempLine = GetLineL(tempFileReader);
	
	if (tempLine)
		CleanupStack::PushL(tempLine);

	TInt lineNumber = 1;

	while ((testLine != 0) && (tempLine != 0) && matched)
	// Compare the specified .out file to the temporary one, line-by-line.
		{
		if (testLine->Des().Compare(tempLine->Des()) != 0)
			{
			matched = EFalse;
			}

		if (matched)
			// If the lines didn't match then save the strings for later.
			{
			CleanupStack::PopAndDestroy(); // testLine
			testLine = 0;
			CleanupStack::PopAndDestroy(); // tempLine
			tempLine = 0;
			// Get the next line
			testLine = GetLineL(testFileReader);
			if (testLine)
				CleanupStack::PushL(testLine);

			tempLine = GetLineL(tempFileReader);
			if (tempLine)
				CleanupStack::PushL(tempLine);

			lineNumber++;
			}
		}

	if ((!matched) || (testLine != 0) || (tempLine != 0))
	// Store the details if the files don't match.
		{
		// Set up the new error values
		matched = EFalse;
		iErrorLine = lineNumber;
		TInt stringIndex;
		if (testLine)
			{
			iExpectedLine = HBufC::NewL(testLine->Size());
			// Copy the 8 bit buffer to the 16 bit buffer.
			stringIndex = 0;
			while (stringIndex < testLine->Size())
				{
				iExpectedLine->Des().Append((*testLine)[stringIndex]);
				stringIndex++;
				}
			}
		else
			{
			iExpectedLine = HBufC::NewL(((TDesC)K_IM_END_OF_FILE).Size());
			(*iExpectedLine) = K_IM_END_OF_FILE;
			}

		if (tempLine)
			{
			iActualLine = HBufC::NewL(tempLine->Size());
			// Copy the 8 bit buffer to the 16 bit buffer.
			stringIndex = 0;
			while (stringIndex < tempLine->Size())
				{
				iActualLine->Des().Append((*tempLine)[stringIndex]);
				stringIndex++;
				}
			}
		else
			{
			iActualLine = HBufC::NewL(((TDesC)K_IM_END_OF_FILE).Size());
			(*iActualLine) = K_IM_END_OF_FILE;
			}
		}

	if (tempLine)
		CleanupStack::PopAndDestroy(); // tempLine

	if (testLine)
		CleanupStack::PopAndDestroy(); // testLine

	testFile.Close();
	tempFile.Close();

	return matched;
	}

EXPORT_C TInt CScriptTestUtils::ErrorLineNumber()
	{
	return iErrorLine;
	}

EXPORT_C HBufC* CScriptTestUtils::ErrorExpectedL()
	{
	HBufC* stringCopy = HBufC::NewL(iExpectedLine->Size());
	(*stringCopy) = (*iExpectedLine);
	return stringCopy;
	}

EXPORT_C HBufC* CScriptTestUtils::ErrorActualL()
	{
	HBufC* stringCopy = HBufC::NewL(iActualLine->Size());
	(*stringCopy) = (*iActualLine);
	return stringCopy;
	}
