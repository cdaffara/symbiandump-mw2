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

#include <e32test.h>
#include <f32file.h>

const TInt IMSCRIPTBUFFERLENGTH = 256;

class CEmailTestUtils;

class CScriptTestUtils : public CBase
	{
public:
	IMPORT_C void LogToInputFileL(const TDesC& aSourceFileName, const TDesC& aDestFileName, TBool aStandardiseDateFields);
	IMPORT_C void LogToOutputFileL(const TDesC& aSourceFileName, const TDesC& aDestFileName, TBool aStandardiseDateFields);

	IMPORT_C static CScriptTestUtils* NewLC(CEmailTestUtils& aTestUtils);
	~CScriptTestUtils();
	
	// Functions for checking the IM output.
	IMPORT_C TBool CheckLogOutputL(TInt aPortNumber, const TDesC& aTestFileName);
	IMPORT_C TInt ErrorLineNumber();
	IMPORT_C HBufC* ErrorExpectedL();
	IMPORT_C HBufC* ErrorActualL();

private:

	void ConstructL();

	enum TImLogLineType
		{
		EImLogLineComment,
		EImLogLineIn,
		EImLogLineOut,
		EImEmptyLine,
		EImReadCancelled
		};

	class TFileReader
		{
	public:
		TFileReader(RFile &rFile);
		TBool Read(char &rChar);
		void SeekCurrent(TInt aOffset);
		void Seek(TInt aOffset);

	private:
		RFile& iFile;
		TBuf8<IMSCRIPTBUFFERLENGTH> iBuffer;
		TInt iBufferIndex;
		TInt iBufferStart;
		};
	
	void StandardiseVariableFieldsL(const TDesC& aSourceFileName);
	void ReplaceVariableFields(TDes8& aDes);
	void StripLogFileL(const TFileName& aSourceFileName, const TFileName& aDestFileName, TBool aStandardiseDateFields, TImLogLineType aLogLineType);

	TImLogLineType LineType(const HBufC8* aLine);
	HBufC8* GetLineL(TFileReader& aFileReader);

	CScriptTestUtils(CEmailTestUtils& aTestUtils);

private:
	
	TInt iErrorLine;
	HBufC* iExpectedLine;
	HBufC* iActualLine;

	class CImTestField
		{
		public:
		HBufC8* iFieldName;
		HBufC8* iReplacementString;
		TInt iNumberFound;

		~CImTestField();
		};

	class CImFindReplace
		{
		public:
		HBufC8* iFind;
		HBufC8* iReplace;

		~CImFindReplace();
		};

	CImFindReplace* FindVariableDataL(TDes8& aLine);

	CArrayPtrFlat<CImTestField>* iFieldList;
	CArrayPtrFlat<CImFindReplace>* iFindReplaceList;

	CEmailTestUtils& iTestUtils;
	};
