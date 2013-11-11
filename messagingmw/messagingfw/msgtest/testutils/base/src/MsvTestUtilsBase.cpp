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

#include <logcli.h>
#include <logview.h>
#include <logwrap.h>
#include <txtetext.h>
#include <msventry.h>
#include <msvapi.h>
#include <msvids.h>
#include <barsc.h>

#include "msvtestutilsbase.h"

#include <cntdb.h>
#include <cntitem.h>
#include <cntfldst.h>
#include <bautils.h>
#include <e32property.h>
#include "msvtestutils.h"
#include <simtsy.h>

_LIT(KMsvTestFileInputBase,"MsgTest");
_LIT(KMsvTestFileOutputBase,"MsgLogs");

// this is used if we can't find MsgLogs anywhere
_LIT(KMsvTestFileDefaultOutputBase,"\\MsgLogs\\");
_LIT(KMsvPathSep,"\\");
#define KMaxLogLineLength 256


//
//
// CTestActive
//
//

EXPORT_C CTestActive::CTestActive()
: CActive(0)
	{
	CActiveScheduler::Add(this);
	}

EXPORT_C CTestActive::~CTestActive()
	{
	Cancel();
	}

EXPORT_C void CTestActive::DoCancel()
	{
	TRequestStatus* s=&iStatus;
	User::RequestComplete(s, KErrNone);
	}

EXPORT_C void CTestActive::StartL()
	{
	SetActive();
	}

EXPORT_C void CTestActive::RunL() 
	{
	CActiveScheduler::Stop();
	}


//
//
// CTestTimer
//
//

EXPORT_C CTestTimer::CTestTimer()
: CTimer(EPriorityLow)
	{}

EXPORT_C void CTestTimer::RunL()
	{
	if (!iReport)
		{
		CActiveScheduler::Stop();
		}
	else
		{
		User::RequestComplete(iReport, iStatus.Int());
		iReport = NULL;
		}
	}

EXPORT_C CTestTimer* CTestTimer::NewL()
	{
	CTestTimer* self = new(ELeave) CTestTimer();
	CleanupStack::PushL(self);
	self->ConstructL(); // CTimer
	CActiveScheduler::Add(self);
	CleanupStack::Pop();
	return self;
	}

EXPORT_C void CTestTimer::AfterReq(TTimeIntervalMicroSeconds32 aSec, TRequestStatus& aStatus)
	{
	iReport = &aStatus;
	aStatus = KRequestPending;
	After(aSec);
	}


//
//
// CTestUtils
//
//

EXPORT_C CTestUtils::CTestUtils(RTest& aRTest)
: iRTest(aRTest), iLogToConsole(ETrue), iLogToFile(ETrue)
	{
	TBuf<256> command;
		User::CommandLine(command);

	command.Trim();
	SetRunAuto(command.FindF(KTestShowMenu) == KErrNotFound);

	if (!RunAuto())
		iRTest.Printf(_L("Not Run Auto. Command line: %S\n"), &command);
	}

EXPORT_C CTestUtils::~CTestUtils()
	{
	ResetMenu();
	delete iMenu;

	CloseMainLog();
	iFs.Close();
	}

EXPORT_C void CTestUtils::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	OpenMainLogL();
	WriteToLog(KTestHeader);
	iMenu = new (ELeave) CArrayPtrFlat<HBufC>(10);
	}

TBool CTestUtils::CheckLogStatus(const TBool aEventViewRet, const TRequestStatus& aStatus)
	{
	TBool ret = ETrue;
	TBufC<256> txtBuf;
	TPtr txt = txtBuf.Des();

	if (!aEventViewRet)
		{
		txt.Zero();
		txt.Append(_L("No more events in the LogView!"));
		AppendEol(txt, EFalse);
		Printf(txt);
		ret = EFalse;
		}
	else if (aStatus.Int() != KErrNone)
		{
		Printf(_L("\nError: CLogViewEvent async request completed with error %d.\n"), aStatus.Int());
		ret = EFalse;
		}

	return ret;
	}

EXPORT_C void CTestUtils::DoBuf(TDes8& buf, const TDesC& label, const TDesC& data)
	{
	buf.Zero();
	buf.Copy(label); 
	buf.Append(data);
	buf.Append(_L("\r\n"));
	}

EXPORT_C void CTestUtils::DoBuf(TDes8& buf, const TDesC& label, const TDesC8& data)
	{
	buf.Zero();
	buf.Copy(label); 
	buf.Append(data);
	buf.Append(_L("\r\n"));
	}


EXPORT_C void CTestUtils::WriteToFileL(RFile& aFile, TDesC8& aInputLine)
	{
	User::LeaveIfError(aFile.Write(aInputLine));
	}

EXPORT_C void CTestUtils::WriteToFileL(RFile& aFile, const TDesC& aInputLine)
	{
	HBufC8* pOutputLine = HBufC8::NewLC(aInputLine.Length()*2);
	TPtr8 pOutput = pOutputLine->Des();
	pOutput.Zero();

	TUint lowChar;
	TUint highChar;
	TInt i=0;
	
	while(i<aInputLine.Length())
		{
		lowChar=((TUint)aInputLine[i])&0x00FF;
		pOutput.Append(lowChar);
		highChar=((TUint)aInputLine[i++])&0xFF00;
		pOutput.Append(highChar);
		}

	aFile.Write(pOutput);
	CleanupStack::PopAndDestroy(); // pBodyText
	}

EXPORT_C void CTestUtils::RemoveRichtextFormating(TDes& aSourceLine)
	{
	TUint8* ptr = (TUint8*)aSourceLine.Ptr();
	TUint8* start = ptr;

	TInt totalLength = aSourceLine.Length()*2;
	do {
		if(*ptr==CEditableText::EParagraphDelimiter || *ptr==CEditableText::ELineBreak || *ptr==CEditableText::EPageBreak)
			*ptr=0x0A;
		} while((++ptr-start)<totalLength);
	}

EXPORT_C TInt CTestUtils::DisplayFile(const TDesC& aFileName)
	{
	RFile file;

	TInt err = file.Open(iFs, aFileName, EFileShareAny | EFileRead | EFileStreamText);

	if (err != KErrNone)
		return err;

	TBuf8<256> text8;
	TBuf<512> text16;

	while (file.Read(text8) == KErrNone && text8.Length())
		{
		text16.Copy(text8);
		Printf(text16);
		}

	file.Close();

	return KErrNone;
	}

EXPORT_C TInt CTestUtils::DisplayMenu(const TDesC& aTitle, TInt aMaxAttempts, TBool aAllowExit, TBool aClearScreenBefore, TBool aClearScreenAfter)
	{
	TBool logToFile = SetLogToFile(EFalse);
	TInt ret = DoDisplayMenu(aTitle, aMaxAttempts, aAllowExit, aClearScreenBefore, aClearScreenAfter);
	SetLogToFile(logToFile);
	return ret;
	}

TInt CTestUtils::DoDisplayMenu(const TDesC& aTitle, TInt& aMaxAttempts, TBool aAllowExit, TBool aClearScreenBefore, TBool aClearScreenAfter)
	{
	if (aMaxAttempts < 1)
		{
		ClearScreen(aClearScreenAfter);
		return 0;
		}

	aMaxAttempts--;

	TInt count = MenuCount();

	__ASSERT_ALWAYS(count > 0 && count < 10, Panic(KErrArgument));
	
	ClearScreen(aClearScreenBefore);
	
	TBuf<256> buf = aTitle;
	AppendEol(buf, EFalse);
	AppendEol(buf, EFalse);

	Printf(buf);


	for (TInt curOption = 0; curOption < count; curOption++)
		{
		buf.Zero();
		buf.Append(_L("%d : "));
		buf.Append(*(iMenu->At(curOption)));
		AppendEol(buf, EFalse);
		Printf(buf, curOption + 1); 
		}

	if (aAllowExit)
		{
		Printf(_L("\nX : Exit\n"));
		}

	Printf(_L("\nEnter your selection from 1 to %d\n"), count);

	TInt result = iRTest.Getch();

	if (aAllowExit && (result == 'x' || result == 'X'))
		{
		ClearScreen(aClearScreenAfter);
		return 0;
		}
	else
		{
		result = result - '0';

		if (result > 0 && result <= count)
			{
			ClearScreen(aClearScreenAfter);
			return result;
			}
		else
			{
			return DoDisplayMenu(aTitle, aMaxAttempts, aAllowExit, aClearScreenBefore, aClearScreenAfter);
			}
		}
	}

EXPORT_C void CTestUtils::Printf(TRefByValue<const TDesC> aFmt,...)
	{
	// Print to the log file.
	TTestOverflow overflow(*this);
	VA_LIST list;
	VA_START(list, aFmt);
	TBuf<0x100> aBuf;
	aBuf.AppendFormatList(aFmt, list, &overflow);

	if (iLogToConsole)
		{
		iRTest.Printf(aBuf);
		}

	if (iLogToFile)
		{
		WriteComment(aBuf);
		}
	}



EXPORT_C void CTestUtils::AppendText(const TDesC& aToken, TDes& aTxt, TInt aMaxTokenLength)
	{
	TInt totalLen = aTxt.Length() + aMaxTokenLength;

	aTxt.Append(aToken);
	aTxt.TrimRight();

	if (aTxt.Length() < totalLen)
		{
		aTxt.AppendFill(' ', totalLen - aTxt.Length());
		}

	aTxt.SetLength(totalLen);
	}

EXPORT_C void CTestUtils::ClearLogL(const CLogFilter& aFilter)
	{
	CLogClient* client = CLogClient::NewL(iFs);
	CleanupStack::PushL(client);

	CLogViewEvent* eventView = CLogViewEvent::NewL(*client);
	CleanupStack::PushL(eventView);

	CTestActive* testAct = new (ELeave) CTestActive();
	CleanupStack::PushL(testAct);

	CLogClient* logCli = CLogClient::NewL(iFs);
	CleanupStack::PushL(logCli);

	TBool found = eventView->SetFilterL(aFilter, testAct->iStatus);

	if (found)
		{
		testAct->StartL();
		CActiveScheduler::Start();	
		}

	if (!CheckLogStatus(found, testAct->iStatus))
		{
		CleanupStack::PopAndDestroy(4);
		return;
		}

	found = eventView->FirstL(testAct->iStatus);

	if (found)
		{
		testAct->StartL();
		CActiveScheduler::Start();	
		}

	if (!CheckLogStatus(found, testAct->iStatus))
		{
		CleanupStack::PopAndDestroy(4);
		return;
		}

	const CLogEvent& event = eventView->Event();

	Printf(_L("Clearing System Log\n"));

	do
		{
		iRTest.Printf(_L("."));
		logCli->DeleteEvent(event.Id(), testAct->iStatus);
		testAct->StartL();
		CActiveScheduler::Start();	

		found = eventView->FirstL(testAct->iStatus);

		if (found)
			{
			testAct->StartL();
			CActiveScheduler::Start();	
			}

		found = CheckLogStatus(found, testAct->iStatus);
		}
	while (found);

	CleanupStack::PopAndDestroy(4);
	}

EXPORT_C void CTestUtils::DisplayLogL(const CLogFilter& aFilter, TBool aForward)
	{
	TBuf<256> header;
	TBuf<256> txt;
	TBuf<64> temp;

	CLogClient* client = CLogClient::NewL(iFs);
	CleanupStack::PushL(client);

	CLogViewEvent* eventView = CLogViewEvent::NewL(*client);
	CleanupStack::PushL(eventView);

	CTestActive* testAct = new (ELeave) CTestActive();
	CleanupStack::PushL(testAct);

	TBool found = eventView->SetFilterL(aFilter, testAct->iStatus);

	if (found)
		{
		testAct->StartL();
		CActiveScheduler::Start();	
		}

	if (!CheckLogStatus(found, testAct->iStatus))
		{
		CleanupStack::PopAndDestroy(3);
		return;
		}

	TInt totalCount = eventView->CountL();

	txt.Zero();
	txt.Append(_L("Total Log Events: "));
	temp.Num(totalCount);
	txt.Append(temp);
	AppendEol(txt, EFalse);
	Printf(txt);

	const TInt numLen		= 3;
	const TInt timeLen		= 11;
	const TInt dateLen		= 10;
	const TInt statusLen	= 7;
	const TInt subjLen		= 20;
	const TInt dirLen		= 3;
	const TInt typeLen		= 10;

	header.Zero();
	AppendText(_L("Num"), header, numLen);
	header.Append('|');
	AppendText(_L("Date"), header, dateLen);
	header.Append('|');
	AppendText(_L("Time"), header, timeLen);
	header.Append('|');
	AppendText(_L("Status"), header, statusLen);
	header.Append('|');
	AppendText(_L("Subject"), header, subjLen);
	header.Append('|');
	AppendText(_L("Dir"), header, dirLen);
	header.Append('|');
	AppendText(_L("Type"), header, typeLen);

	AppendEol(header, EFalse);
	Printf(header);

	found = aForward ? eventView->FirstL(testAct->iStatus) : eventView->LastL(testAct->iStatus);

	if (found)
		{
		testAct->StartL();
		CActiveScheduler::Start();	
		}

	if (!CheckLogStatus(found, testAct->iStatus))
		{
		CleanupStack::PopAndDestroy(3);
		return;
		}

	TInt counter = 0;
	do
		{
		const CLogEvent& event = eventView->Event();
		counter++;
		if (!RunAuto() && (counter % 6 == 0))
			{
			iRTest.Printf(_L("\nPress any key for more...\n"));
			iRTest.Getch();
			Printf(header);
			}

		txt.Zero();

		if (aForward)
			{
			temp.Num(totalCount - counter + 1);
			}
		else
			{
			temp.Num(counter);
			}

		AppendText(temp, txt, numLen);
		txt.Append('|');

		event.Time().FormatL(temp, _L("%D%M%Y%/0%1%/1%2%/2%3%/3"));
		AppendText(temp, txt, dateLen);
		txt.Append('|');

	    event.Time().FormatL(temp,_L("%-B%:0%J%:1%T%:2%S%:3%+B"));
		AppendText(temp, txt, timeLen);
		txt.Append('|');

		AppendText(event.Status(), txt, statusLen);
		txt.Append('|');
		AppendText(event.Subject(), txt, subjLen);
		txt.Append('|');
		AppendText(event.Direction(), txt, dirLen);
		txt.Append('|');

		GetEventType(event.EventType(), temp);
		AppendText(temp, txt, typeLen);

		AppendEol(txt, EFalse);
		Printf(txt);

		DisplayLogEventData(event);

		found = aForward ? eventView->NextL(testAct->iStatus) : eventView->PreviousL(testAct->iStatus);

		if (found)
			{
			testAct->StartL();
			CActiveScheduler::Start();	
			}

		if (!CheckLogStatus(found, testAct->iStatus))
			{
			CleanupStack::PopAndDestroy(3);
			return;
			}
		}
	while (found);

	CleanupStack::PopAndDestroy(3);
	}

void CTestUtils::GetEventType(TUid aEventType, TDes& rOutput)
	{
	switch (aEventType.iUid)
		{
		case KLogCallEventType:
			rOutput = _L("Call");
			break;
		case KLogDataEventType:
			rOutput = _L("Data");
			break;
		case KLogFaxEventType:
			rOutput = _L("Fax");
			break;
		case KLogShortMessageEventType:
			rOutput = _L("SMS");
			break;
		case KLogMailEventType:
			rOutput = _L("Mail");
			break;
		case KLogTaskSchedulerEventType:
			rOutput = _L("SchSvr");
			break;
		default:
			rOutput.Num((TInt) aEventType.iUid);
		}
	}

EXPORT_C void CTestUtils::CreateAllTestDirectories()
	{
	TChar driveChar=RFs::GetSystemDriveChar();
 	TBuf<2> systemDrive;
 	systemDrive.Append(driveChar);
 	systemDrive.Append(KDriveDelimiter);
	
	iFs.SetSessionPath(systemDrive);

	if (!iFs.MkDir(KLogsDir))
		{
		Printf(_L("Created c:\\logs\\ directory\n"));
		}
	}

EXPORT_C TContactItemId CTestUtils::CreateContactL(const TDesC& aGivenName, const TDesC& aFamilyName, const TDesC& aPhoneNumber, const TDesC& aEmailAddress, TBool aReplaceContactsDb)
	{
	CContactDatabase* db = NULL;

	if (aReplaceContactsDb)
		{
		db = CContactDatabase::ReplaceL();
		}
	else
		{
		TRAPD(err, db = CContactDatabase::OpenL());
		
		if (err == KErrNotFound)
			{
			TRAP(err, db = CContactDatabase::CreateL());
			}

		User::LeaveIfError(err);
		}

	CleanupStack::PushL(db);

	CContactItem* item=CContactCard::NewLC();

	if (aFamilyName.Length())
		SetNameL(*item, KUidContactFieldFamilyName, aFamilyName, ETrue);

	if (aGivenName.Length())
		SetNameL(*item, KUidContactFieldGivenName, aGivenName, ETrue);

	if (aPhoneNumber.Length())
		SetNameL(*item, KUidContactFieldPhoneNumber, aPhoneNumber, ETrue);

	if (aEmailAddress.Length())
		SetNameL(*item, KUidContactFieldEMail, aEmailAddress, ETrue);

	CContactItemFieldSet& fieldSet= item->CardFields();

	TContactItemId id = KNullContactId;
	
	if (fieldSet.Count())
		id = db->AddNewContactL(*item);
	else
		User::Leave(KErrNotSupported);

	CleanupStack::PopAndDestroy(2); // item, db
	return id;
	}

EXPORT_C void CTestUtils::SetNameL(CContactItem& aItem,TUid aType,const TDesC& aName, TBool aAddField)
//
// Set the contents of a text field, creating the field if required
//
	{
	CContactItemFieldSet& fieldSet=aItem.CardFields();
	const TInt pos=fieldSet.Find(aType);
	if (!aAddField && pos!=KErrNotFound)
		fieldSet[pos].TextStorage()->SetTextL(aName);
	else
		{
		CContactItemField* field=CContactItemField::NewLC(KStorageTypeText,aType);
   		field->SetMapping(KUidContactFieldVCardMapUnusedN);
		field->TextStorage()->SetTextL(aName);
		aItem.AddFieldL(*field);
		CleanupStack::Pop(); // field
		}
	}

EXPORT_C void CTestUtils::DeleteContactsFileL()
	{
	CContactDatabase::DeleteDefaultFileL();
	}

EXPORT_C HBufC8* CTestUtils::ReadFileLC(const TDesC& aFile)
	{
	//Returns a HBufC8 with the contents of aFile
	RFile file;
	User::LeaveIfError(file.Open(iFs, aFile, EFileShareAny | EFileRead));
	CleanupClosePushL(file);

	TInt size = 0;
	TInt err = file.Size(size);

	if (err)
		{
		Printf(_L("ReadFileLC(%S) leaving with %d. Unable to size file\n"), &aFile, err);
		User::Leave(err);
		}

	HBufC8* contents = HBufC8::NewLC(size + 4);
	TPtr8 contentsPtr(contents->Des());
	err = file.Read(0, contentsPtr);

	if (err)
		{
		Printf(_L("ReadFileLC(%S) leaving with %d. Unable to read file\n"), &aFile, err);
		User::Leave(err);
		}

	CleanupStack::Pop(); //contents
	CleanupStack::PopAndDestroy(); //file
	CleanupStack::PushL(contents);

	return contents;
	}

const TInt KNumberOfHex = 16;

EXPORT_C void CTestUtils::PrintHex(const TDesC& aDes)
	{
	TBuf<256> temp;
	TInt pos = 0;
	const TInt len = aDes.Length();

	while (pos < len)
		{
		temp.Zero();
		TPtrC ptr = aDes.Mid(pos, Min(KNumberOfHex, len - pos));
		const TInt ptrLen = ptr.Length();

		for (TInt i = 0; i < ptrLen; i++)
			{
			temp.AppendFormat(_L("%2.2x "), ptr[i]);
			}

		if (ptrLen < KNumberOfHex)
			{
			temp.AppendFill(' ', (KNumberOfHex - ptrLen) * 3);
			}

		temp.Append(ptr);
		Printf(temp);

		pos += KNumberOfHex;
		}
	}

EXPORT_C void CTestUtils::PrintHex(const TDesC8& aDes)
	{
	TBuf<256> temp;
	TInt pos = 0;
	const TInt len = aDes.Length();

	while (pos < len)
		{
		temp.Zero();
		TPtrC8 ptr = aDes.Mid(pos, Min(KNumberOfHex, len - pos));
		const TInt ptrLen = ptr.Length();

		for (TInt i = 0; i < ptrLen; i++)
			{
			temp.AppendFormat(_L("%2.2x "), ptr[i]);
			}

		if (ptrLen < KNumberOfHex)
			{
			temp.AppendFill(' ', (KNumberOfHex - ptrLen) * 3);
			}

		TBuf<KNumberOfHex> buf16;
		buf16.Copy(ptr);
		temp.Append(buf16);
		Printf(temp);

		pos += KNumberOfHex;
		}
	}

EXPORT_C TInt CTestUtils::ResolveFile(const TDesC& aComponent, const TDesC& aFileName, TParse& aParseOut)
	{
	TInt err ;
	TFileName* savedPath = new TFileName;
	TFileName* fileName = new TFileName;
	if ((savedPath == NULL) || (fileName == NULL))
		{
		err=KErrNoMemory;
		}
	fileName->Append(KMsvPathSep);
	fileName->Append(KMsvTestFileInputBase);
	fileName->Append(KMsvPathSep);
	fileName->Append(aComponent);
	fileName->Append(KMsvPathSep);
	fileName->Append(aFileName);
	
	// file finder will look in the session drive first, then Y->A,Z
	// so set session drive to Y (save old and restore it afterwards)
	iFs.SessionPath(*savedPath);
	_LIT(KTopDrive,"Y:\\");
	iFs.SetSessionPath(KTopDrive);
    TFindFile file_finder(iFs);
    err = file_finder.FindByDir(*fileName,KNullDesC);
	if(err==KErrNone)
		aParseOut.Set(file_finder.File(),NULL,NULL);
	iFs.SetSessionPath(*savedPath);
	delete savedPath;
	delete fileName;
	return(err);
	}


EXPORT_C TInt CTestUtils::ResolveLogFile(const TDesC& aFileName, TParse& aParseOut)
	{
	TFileName* savedPath = new TFileName;
	TFileName* fileName = new TFileName;
	if ((savedPath == NULL) || (fileName == NULL))
		return KErrNoMemory;

	fileName->Append(KMsvPathSep);
	fileName->Append(KMsvTestFileOutputBase);
	fileName->Append(KMsvPathSep);
	
	// file finder will look in the session drive first, then Y->A,Z
	// so set session drive to Y (save old and restore it afterwards)
	iFs.SessionPath(*savedPath);
	_LIT(KTopDrive,"Y:\\");
	iFs.SetSessionPath(KTopDrive);
    TFindFile file_finder(iFs);
    TInt err = file_finder.FindByDir(*fileName,KNullDesC);

	if(err==KErrNone)
		{
		fileName->Copy(file_finder.File());
		AppendTestName(*fileName);
		fileName->Append(KMsvPathSep);
		fileName->Append(aFileName);
		iFs.MkDirAll(*fileName);
		aParseOut.Set(*fileName,NULL,NULL);
		}
	iFs.SetSessionPath(*savedPath);
	delete savedPath;
	delete fileName;
	return(err);
	}

EXPORT_C void CTestUtils::ResetMenu()
	{
	iMenu->ResetAndDestroy();
	}

EXPORT_C void CTestUtils::AppendToMenuL(const TDesC& aOption)
	{
	HBufC* buf = HBufC::NewLC(256);
	TPtr ptr = buf->Des();
	ptr.Copy(aOption);
	iMenu->AppendL(buf);
	CleanupStack::Pop();
	}

EXPORT_C TInt CTestUtils::MenuCount() const
	{
	return iMenu->Count();
	}

EXPORT_C void CTestUtils::ClearScreen(TBool aClearScreen)
	{
	if (aClearScreen)
		{
		iRTest.Console()->ClearScreen();
		iRTest.Console()->SetPos(0,0);
		}
	}


EXPORT_C TBool CTestUtils::SetLogToConsole(TBool aLogToConsole)
	{
	TBool save = iLogToConsole;
	iLogToConsole = aLogToConsole;
	return save;
	}

EXPORT_C TBool CTestUtils::SetLogToFile(TBool aLogToFile)
	{
	TBool save = iLogToFile;
	iLogToFile = aLogToFile;
	return save;
	}



void CTestUtils::AppendEol(TDes& aTxt, TBool aToFile)
	{
	aToFile ? aTxt.Append(_L("\r\n")) : aTxt.Append(_L("\n"));
	}

EXPORT_C TBool CTestUtils::RunAuto() const
	{
	return iRunAuto;
	}

EXPORT_C void CTestUtils::SetRunAuto(TBool aRunAuto)
	{
	iRunAuto = aRunAuto;
	}


// appends .<PLATFORM>.<VARIANT>.log to aFileName;
void CTestUtils::AppendVariantName(TDes& aFileName)
	{
#if (defined(__THUMB__) || defined(__MARM_THUMB__))
	aFileName.Append(_L(".THUMB."));
#endif
#if (defined(__ARMI__) || defined(__MARM_ARMI__))
	aFileName.Append(_L(".ARMI."));
#endif
#if (defined(__ARM4__) || defined(__MISA__) || defined(__MARM_ARM4__))
	aFileName.Append(_L(".ARM4."));
#endif
#if defined(__WINSCW__)
	aFileName.Append(_L(".WINSCW."));
#else 
#if defined(__WINS__)
	aFileName.Append(_L(".WINS."));
#endif
#endif

#if defined(_DEBUG)
	aFileName.Append(_L("DEB."));
#else
	aFileName.Append(_L("REL."));
#endif
	aFileName.Append(_L("LOG"));
	}

// appends the test harness name to aFileName
void CTestUtils::AppendTestName(TDes &aFileName)
	{
	TInt start = RProcess().FileName().LocateReverse('\\');
	TInt end = RProcess().FileName().LocateReverse('.');

	if (end == KErrNotFound)
		end = RProcess().FileName().Length();

	aFileName.Append(RProcess().FileName().Mid(start + 1, end - start - 1));
	}	



TInt CTestUtils::OpenMainLogL()
	{
	_LIT(KDisplayLogFile,"Log File %S\n");
	TParse loglocation;
	TFileName logfile;
	TInt err=ResolveLogFile(KNullDesC, loglocation);
	if(err!=KErrNone) 
		{
		TChar driveChar=RFs::GetSystemDriveChar();
 		TBuf<2> systemDrive;
 		systemDrive.Append(driveChar);
 		systemDrive.Append(KDriveDelimiter);
 		TPath pathName(systemDrive) ;
		pathName.Append(KMsvTestFileDefaultOutputBase);		
		iFs.MkDirAll(pathName);
		err=ResolveLogFile(KNullDesC, loglocation);
		}
	User::LeaveIfError(err);
	logfile.Copy(loglocation.FullName());
	logfile.Delete(logfile.Length()-1,1);
	AppendVariantName(logfile);
	iRTest.Printf(KDisplayLogFile, &logfile);
	iFs.MkDirAll(logfile);

	iLogBuf=HBufC::NewL(KMaxLogLineLength);
	iLogBuf8=HBufC8::NewL(KMaxLogLineLength);
	return(iFile.Replace(iFs,logfile,EFileWrite|EFileShareAny));
	}

TInt CTestUtils::AppendMainLogL()
	{
	_LIT(KDisplayLogFile,"Log File %S\n");
	TParse loglocation;
	TFileName logfile;
	TInt err=ResolveLogFile(KNullDesC, loglocation);
	if(err!=KErrNone) 
		{
		iFs.MkDirAll(KMsvTestFileDefaultOutputBase);
		err=ResolveLogFile(KNullDesC, loglocation);
		}
	User::LeaveIfError(err);
	logfile.Copy(loglocation.FullName());
	logfile.Delete(logfile.Length()-1,1);
	AppendVariantName(logfile);
	iRTest.Printf(KDisplayLogFile, &logfile);
	iFs.MkDirAll(logfile);

	iLogBuf=HBufC::NewL(KMaxLogLineLength);
	iLogBuf8=HBufC8::NewL(KMaxLogLineLength);
	TInt pos=0;
	TInt ret=KErrNone;
	ret=iFile.Open(iFs,logfile,EFileWrite|EFileShareAny);
	if (ret==KErrNotFound)
		ret=iFile.Create(iFs,logfile,EFileWrite|EFileShareAny);
	if (ret==KErrNone)
		return(iFile.Seek(ESeekEnd,pos));
	else
		return(ret);
	}

void CTestUtils::CloseMainLog()
	{
	delete iLogBuf;
	iLogBuf=0;
	delete iLogBuf8;
	iLogBuf8=0;

	iFile.Close();
	}


EXPORT_C void CTestUtils::TestStart(TInt aTest, const TDesC& aTestDescription)
	{
	WriteToLog(KTestStartingWithDesc, aTest, &aTestDescription);
	}

EXPORT_C void CTestUtils::TestStart(TInt aTest)
	{
	WriteToLog(KTestStarting, aTest);
	}



EXPORT_C void CTestUtils::TestFinish(TInt aTest, TInt aError)
	{
	aError ?
		WriteToLog(KTestFailed, aTest, aError)
		: WriteToLog(KTestPassed, aTest);
	}


EXPORT_C void CTestUtils::TestHarnessCompleted()
	{
	WriteToLog(KTestHarnessCompleted);
	}

EXPORT_C void CTestUtils::TestHarnessFailed(TInt aError)
	{	
	WriteToLog(KTestHarnessFailed, aError);
	}

EXPORT_C void CTestUtils::WriteComment(const TDesC& aComment)
	{
	/* Prepends KTestCommentPrepend to aComment, then writes it to aFlogger */

	TBuf<0x100> buf;
	buf.Copy(aComment);
	buf.Trim();
	buf.Insert(0,KTestCommentPrepend);
	WriteToLog(buf);
	}

void CTestUtils::WriteToLog(TRefByValue<const TDesC> aFmt,...)
	{
	_LIT(KDateFormatString, "%D%M%*Y%1%/1%2%/2%3 %H%:1%T%:2%S ");
	_LIT(Kcr,"\r\n");
	iLogBuf->Des().Zero();
	TTime date;
	date.HomeTime();
	TBuf<18> dateString;
	TRAPD(error, date.FormatL(dateString,(KDateFormatString)));
	iLogBuf->Des().Copy(dateString);
	
	VA_LIST list;
	VA_START(list,aFmt);

	iLogBuf->Des().AppendFormatList(aFmt,list);
	iLogBuf->Des().Append(Kcr);
	iLogBuf8->Des().Copy(*iLogBuf);
	iFile.Write(*iLogBuf8);
	}

EXPORT_C void CTestUtils::OpenResourceFileL(const TDesC& aResFileName, RResourceFile& aResFile)
	{
	TFileName fileName;
    Dll::FileName(fileName);
    TParse parse;
    parse.Set(aResFileName, &fileName, 0);
	fileName=parse.FullName();
	BaflUtils::NearestLanguageFile(iFs, fileName);
	aResFile.OpenL(iFs, fileName);
	}

EXPORT_C void CTestUtils::ReadResourceStringL(RResourceFile& aResFile, TInt aId, TDes& rString)
	{
	HBufC8* buf = aResFile.AllocReadLC(aId);
	TResourceReader reader;
	reader.SetBuffer(buf);
	rString.Copy(reader.ReadTPtrC());
	CleanupStack::PopAndDestroy(buf);
	}

EXPORT_C void CTestUtils::ReadResourceStringL(const TDesC& aResFileName, TInt aId, TDes& rString)
	{
	RResourceFile resFile;
	OpenResourceFileL(aResFileName, resFile);
	CleanupClosePushL(resFile);
	ReadResourceStringL(resFile, aId, rString);
	CleanupStack::PopAndDestroy(); //resFile
	}

EXPORT_C void CTestUtils::SetSimTsyTestNumberL(TInt aTestNumber)
	{
	TInt testState;
	if(KErrNone != RProperty::Get(KUidSystemCategory, KUidSASimTsyTestNumberValue, testState))
		{
		User::LeaveIfError(RProperty::Define(KUidSystemCategory, KUidSASimTsyTestNumberValue, RProperty::EInt));
		}
	User::LeaveIfError(RProperty::Set(KUidSystemCategory, KUidSASimTsyTestNumberValue, aTestNumber));
	}

EXPORT_C void CTestUtils::CopyToSimTsyConfigFileL(RFs& aFs, const TDesC& aFilename)
	{
	_LIT(KFileName,"config.txt");
	TChar driveChar=RFs::GetSystemDriveChar();
 	TBuf<2> systemDrive;
 	systemDrive.Append(driveChar);
 	systemDrive.Append(KDriveDelimiter);	
	aFs.SetSessionPath(systemDrive);
	aFs.Delete(KFileName);					//< Ignore Error

	CFileMan* fileMan = CFileMan::NewL(aFs);
	CleanupStack::PushL(fileMan);

	User::LeaveIfError(fileMan->Copy(aFilename, KFileName));

	CleanupStack::PopAndDestroy(fileMan);
	}

EXPORT_C TInt CTestUtils::SeekLogFile(TSeek aMode,TInt& aPos)
	{
	return (iFile.Seek(aMode,aPos));
	}


void CTestUtils::ConstructKeepLogFileL()
	{
	User::LeaveIfError(iFs.Connect());
	AppendMainLogL();
	iMenu = new (ELeave) CArrayPtrFlat<HBufC>(10);
	}
