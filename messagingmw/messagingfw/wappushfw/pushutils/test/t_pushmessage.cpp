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
    
#include <e32std.h>
#include <e32test.h>
#include <s32file.h>

#include <push/pushmessage.h>
#include "cmultipartbiniterator.h"
#include "cmultiparttextiterator.h"

#include <push/pushlog.h>
#include "testlog.h"
#include "t_pushmessage.h"

/** literals */
_LIT8(KWBContentType, "X-WBML-Content-Type: ");
_LIT8(KHeaderBoundary, "\r\n\r\n");
_LIT(KTestTitle,"T_PushMessage");
TChar KEOL('\n');
TChar KCOMMA(',');
TChar KSEMICOLON(';');

/** global variables */
RTest gTest(KTestTitle);
RFs gFs;



/** 
factory construction method to allocate and construct a new CPushMessageTester 
@param aWapLog Implementation of logging interface
*/
CPushMessageTester* CPushMessageTester::NewLC(CWapPushLog& aWapLog)
	{
	CPushMessageTester* self =  new (ELeave) CPushMessageTester(aWapLog);
	CleanupStack::PushL(self);
	return self;
	}

/** 
factory construction method to allocate and construct a new CPushMessageTester 
@param aWapLog Implementation of logging interface
*/
CPushMessageTester* CPushMessageTester::NewL(CWapPushLog& aWapLog)
	{
	CPushMessageTester* self = CPushMessageTester::NewLC(aWapLog);
	CleanupStack::Pop();
	return self;
	}

/** 
constructor 
@param aWapLog Implementation of logging interface
*/
CPushMessageTester::CPushMessageTester(CWapPushLog& aWapLog)
:iLog(aWapLog)
	{
	}

/** destructor */
CPushMessageTester::~CPushMessageTester()
	{
	delete iPushMessage1;   
	delete iPushMessage2;
	delete iPushMessage3;
	delete iPushMessage4;
	delete iPushMessage5;
	delete iPushMessage6;
	delete iPushMessage7;
	delete iPushMultiMessage;
	}


/* creates a descriptors containing the data from arrays */
HBufC8* CPushMessageTester::CreateMsgDataLC(const TUint8* aMessage, TInt aLength)
	{
	HBufC8* msgData = HBufC8::NewLC(aLength);
	TBuf8<140> tempBuf;
	TUint8 value=0;
	
	for (TInt i=0; i<aLength; i++)
		{
		value = aMessage[i];
		tempBuf.Append(value);
		}

	msgData->Des().Copy(tempBuf);
	return msgData;
	}


/** create push messages for the tests */
void CPushMessageTester::CreateMessagesL()
	{
	__LOG_ALWAYS("Test Results");
	__LOG_ALWAYS(" ");

	HBufC8* header1 = CreateMsgDataLC(KMessageHeader2,15);
	HBufC8* body1 = KLongPushMessageBody().AllocLC();
	HBufC8* address1 = KPushMessageServerAddress().AllocLC();

	iPushMessage1 = CPushMessage::NewL(header1, body1, 22, address1);
	CleanupStack::Pop(3, header1); //header1, body1, address1

	HBufC8* header2 = CreateMsgDataLC(KMessageHeader3,9);
	HBufC8* body2 = KShortPushMessageBody().AllocLC();
	iPushMessage2 = CPushMessage::NewL(header2, body2);
	CleanupStack::Pop(2, header2); //header2, body2
	
	HBufC8* header3 = CreateMsgDataLC(KMessageHeader4,32);
	HBufC8* body3 = KShortPushMessageBody().AllocLC();
	iPushMessage3 = CPushMessage::NewL(header3, body3);
	CleanupStack::Pop(2, header3); //header3, body3
	
	HBufC8* header4 = CreateMsgDataLC(KMessageHeaderMultipart,42);
	HBufC8* body4 = CreateMsgDataLC(KMessageBodyMultipart,42);
	iPushMultiMessage = CPushMessage::NewL(header4, body4);
	CleanupStack::Pop(2, header4); // header4, body4

	HBufC8* header5 = CreateMsgDataLC(KMessageHeader5,22);
	HBufC8* body5 = KLongPushMessageBody().AllocLC();
	HBufC8* address2 = KPushMessageServerAddress().AllocLC();
	iPushMessage4 = CPushMessage::NewL(header5, body5, 255, address2);
	CleanupStack::Pop(3, header5); // header5, body5, address2

	HBufC8* header6 = CreateMsgDataLC(KMessageHeader6,51);
	HBufC8* body6 = KLongPushMessageBody().AllocLC();
	iPushMessage5 = CPushMessage::NewL(header6, body6);
	CleanupStack::Pop(2, header6); // header6, body6

	HBufC8* header7 = CreateMsgDataLC(KMessageHeader7,9);
	HBufC8* body7 = KLongPushMessageBody().AllocLC();
	iPushMessage6 = CPushMessage::NewL(header7, body7);
	CleanupStack::Pop(2, header7); // header7, body7

	HBufC8* header8 = CreateMsgDataLC(KMessageHeader8,139);
	HBufC8* body8 = KLongPushMessageBody().AllocLC();
	iPushMessage7 = CPushMessage::NewL(header8, body8);
	CleanupStack::Pop(2, header8); // header8, body8
	}


/** dump relevant bits of the message to the console */
void CPushMessageTester::OutputMessageDataL()
	{
	__LOG_MSG_DEBUG(*iPushMessage1);
	__LOG_MSG_DEBUG(*iPushMessage2);
	__LOG_MSG_DEBUG(*iPushMessage3);
	__LOG_MSG_DEBUG(*iPushMessage4);
	__LOG_MSG_DEBUG(*iPushMessage5);
	__LOG_MSG_DEBUG(*iPushMessage6);
	__LOG_MSG_DEBUG(*iPushMessage7);

	TPtrC contentType;
	iPushMultiMessage->GetContentType(contentType);

	_LIT(KMultipartMixedBin, "application/vnd.wap.multipart.mixed");
	if (contentType==KMultipartMixedBin)
		{
		CMultipartBinIterator* multiMsg = CMultipartBinIterator::NewL(*iPushMultiMessage);
		CleanupStack::PushL(multiMsg);
		do 
			{
			CPushMessage * pm = multiMsg->PartL();
			CleanupStack::PushL(pm);
			__LOG_MSG_DEBUG(*pm);
			CleanupStack::PopAndDestroy(pm); 
			} while (multiMsg->NextL());
		CleanupStack::PopAndDestroy(multiMsg); 
		}
	}






/** create multipart messages from text? */
void CPushMessageTester::TestMultiTextMsgFromFilesL()
	{
	_LIT(KPushTxtFilePath, "C:\\tpushmessage\\"); 
	CDir* dir;
	TInt err = gFs.GetDir(KPushTxtFilePath, KEntryAttMatchMask, ESortByName, dir);
	CleanupStack::PushL(dir);
	if (err == KErrPathNotFound)
		User::LeaveIfError(gFs.MkDirAll(KPushTxtFilePath));		
	else 
		User::LeaveIfError(err);
	CleanupStack::PopAndDestroy(dir);


	// Set the session path for the RFs
    gFs.SetSessionPath(KPushTxtFilePath);
	_LIT(KPushTxtFilePattern,"*.txt");
	dir=NULL;
	User::LeaveIfError(gFs.GetDir(KPushTxtFilePattern, KEntryAttNormal, ESortByName, dir));
	CleanupStack::PushL(dir);
    TInt count = dir->Count();
    if(count != 0)
        {
		TFileName currentFile;
		TBuf8<256> msgType;
		for(TInt i = 0; i < count; i++)
			{
			currentFile=( (*dir)[i].iName );
			GenMessageTypeL(currentFile, msgType);

			//if (msgType[0] == 0x8C && msgType[0] <= 0x8E) 
				{
				CPushMessage* pushMessage = CreateMultiTextPushMessageL(currentFile, msgType);
				CleanupStack::PushL(pushMessage);

				CMultipartTextIterator* multiMsg = CMultipartTextIterator::NewL(*pushMessage);
				CleanupStack::PushL(multiMsg);
				do 
					{
					CPushMessage * pm = multiMsg->PartL();
					__LOG_MSG_DEBUG(*pm);
					delete pm;
					} while (multiMsg->NextL());
				CleanupStack::PopAndDestroy(2); //multiMsg, pushMessage
				}
			}
		}
	CleanupStack::PopAndDestroy(dir);
    }




void CPushMessageTester::GenMessageTypeL(const TDesC& aFileName, TDes8& rMsgType)
	{
	TBuf8<256>		lineBuffer;   // Buffer from Read function.
	TInt			aPos = 0;

	HBufC8*			contentType = NULL;

	RFile			file;
	TInt err = file.Open(gFs,aFileName,EFileStreamText|EFileRead|EFileShareAny);

	 // Didn't find the file, so leave - should only get valid filenames!
	if(err != KErrNone)  
        User::Leave(KErrNotFound);


	file.Seek(ESeekAddress,aPos);
	err = file.Read(lineBuffer, 256);// Read upto 256 chars, '\n' and all...
	if (err==KErrNone)				// Made a valid read,
		if (lineBuffer.Length()==0) // but read 0 chars
			err=KErrEof;			// so set err value to end processing

	if(err == KErrNone)
		{
		// now we'ver read in a line 
		TInt ctOffset = lineBuffer.FindF(KWBContentType);
		if (ctOffset != KErrNotFound)
			{
			contentType = lineBuffer.Mid(ctOffset).AllocL();
			// we've found content-type ... make sure we've read the whole line
			TInt eolOffset = contentType->Locate(KEOL);
			if (ctOffset == KErrNotFound)
				{
				// must get the rest of the line
				err = file.Read(lineBuffer, 256);// Read upto 256 chars, '\n' and all...
				eolOffset = contentType->Locate(KEOL);
				contentType = contentType->ReAllocL(contentType->Length() + eolOffset);
				contentType->Des().Append(lineBuffer.Mid(eolOffset));
				}
			}
		}
	
	// set ContentType the whole  WBContent line 
	TInt eolOffset = contentType->Locate(KEOL);
	TPtrC8 wbContentType = contentType->Mid(KWBContentType().Length(), eolOffset - KWBContentType().Length() - 1);

	// First value is the binary value of content type;
	TInt commaOffset = wbContentType.Locate(KCOMMA);
	TPtrC8	binType = wbContentType.Mid(0, commaOffset);

	TLex8 ctTypeLex(binType);
	TUint8 ctVal = 0;
	ctTypeLex.Val(ctVal, EHex);

	TBuf8<256> binHeader;
	binHeader.Zero();
	binHeader.Append(ctVal);

	// is this a multipart/
	if (ctVal >= 0x8C && ctVal <= 0x8E)
		{
		// next field is boundary
		TPtrC8	boundary = wbContentType.Mid(commaOffset+1);
		binHeader.Append(KBoundaryParam);
		binHeader.Append(0);
		binHeader.Append(boundary);
		binHeader.Append(0);
		}

	rMsgType.Zero();

	TInt len = binHeader.Length();
	if (len <= 30)
		rMsgType.Append(len);
	else if (len < 128)
		{
		// make sure the top bit isn't set
		TUint8 wapUint = 31;
		len &= 0x7F;
		rMsgType.Append(wapUint);
		rMsgType.Append(len);
		}
	else
		{
		

		}

	rMsgType.Append(binHeader);
	delete contentType;
	file.Close();
	}


CPushMessage* CPushMessageTester::CreateMultiTextPushMessageL(const TDesC& aFileName, const TDesC8& aHdr)
	{
	RFile          file;
	TBuf8<256>     lineBuffer;   // Buffer from Read function.
	TInt		   err=KErrNone;
	TInt		   aPos = 0;
	HBufC8*	headers = NULL;
	HBufC8*	body = NULL;
	TBool		bBodyFound = EFalse;

	err = file.Open(gFs,aFileName,EFileStreamText|EFileRead|EFileShareAny);

    if(err != KErrNone)  // Didn't find the file, so leave - should only get valid filenames!
        {
        User::Leave(KErrNotFound);
		}

	headers = aHdr.AllocLC();

	file.Seek(ESeekAddress,aPos);
	err = file.Read(lineBuffer, 256);// Read upto 256 chars, '\n' and all...
	if (err==KErrNone)				// Made a valid read,
		if (lineBuffer.Length()==0) // but read 0 chars
			err=KErrEof;			// so set err value to end processing

	if(err == KErrNone)
	do {
		// now we'ver read in a line 
		TInt bdOffset = lineBuffer.FindF(KHeaderBoundary);
		if ( !bBodyFound && bdOffset == KErrNotFound)
			{
			HBufC8* oldHeaders = headers;
			headers = headers->ReAllocL(headers->Length() + lineBuffer.Length());
			CleanupStack::Pop(oldHeaders);
			CleanupStack::PushL(headers);

			headers->Des().Append(lineBuffer);
			}
		
		else // we've found our break
			{
			if (!bBodyFound)
				{
				HBufC8* oldHeaders = headers;
				headers = headers->ReAllocL(headers->Length() + lineBuffer.Length() - bdOffset );
				CleanupStack::Pop(oldHeaders);
				CleanupStack::PushL(headers);
				
				headers->Des().Append(lineBuffer.Mid(0, lineBuffer.Length() - bdOffset));

				body = lineBuffer.Mid(bdOffset+2, lineBuffer.Length() - bdOffset-2).AllocLC();
				bBodyFound = ETrue;
				}

			else 
				{
				HBufC8* oldBody = body;
				body = body->ReAllocL(body->Length() + lineBuffer.Length() );
				CleanupStack::Pop(oldBody);
				CleanupStack::PushL(body);

				body->Des().Append(lineBuffer);
				}

			}
		err = file.Read(lineBuffer, 256);// Read upto 256 chars, '\n' and all...
		} while ( lineBuffer.Length());

	    
	file.Close();
	CPushMessage* rPush = CPushMessage::NewL(headers, body);
	if (body == NULL)
		CleanupStack::Pop(headers);
	else
		CleanupStack::Pop(2, headers); //headers, body
	return rPush;
	}

void CPushMessageTester::TestFinished()
	{
	__LOG_ALWAYS(" ");
	__LOG_ALWAYS("Tests Completed");
	}

/** what does this do? - i assume this was used to create messages */
LOCAL_C void CalcTimePeriod()
	{
	// Get num bytes encoding the date - 
	// we are positioned at that location in the source descriptor
	// TInt32 time = 0;

	// The WSP Date encoding is the number of seconds since the start of the
	// UNIX epoch (00:00:00.000, 01-Jan-1970), as a long integer
	TDateTime targetDT(2000, EAugust, 15,10,20,0,0);
	TDateTime unixEpocDT(1970, EJanuary, 0, 0, 0, 0, 0);
	TTime targetTime(targetDT);
	TTimeIntervalSeconds timeSeconds;

	targetTime.SecondsFrom(unixEpocDT,timeSeconds);

	timeSeconds.Int();
	}




/** Setup test environment and run tests */
LOCAL_C void doMainL()
	{
	gTest.Start(KTestTitle);
			
	gTest.Printf(_L("@SYMTestCaseID IWS-WAPBROWSER-PUSHUTILS-T_PUSHMESSAGE-0001 "));	
	
	CWapPushLog* log = CWapPushLog::NewL(*gTest.Console());
	log->SetLogFileName(_L("TPushMessage.txt"));
	CleanupStack::PushL(log);
	User::LeaveIfError(gFs.Connect());
	CleanupClosePushL(gFs);
	CalcTimePeriod();

	CPushMessageTester* myTester = CPushMessageTester::NewLC(*log);
	myTester->CreateMessagesL();
	myTester->OutputMessageDataL();
	myTester->TestMultiTextMsgFromFilesL();
	myTester->TestFinished();
	
	CleanupStack::PopAndDestroy(3); //gFs.Close(), log, myTester
	gTest.End();
	gTest.Close();
	}



/** Entry point to test executable */
GLDEF_C TInt E32Main()
	{	
	__UHEAP_MARK;
	CTrapCleanup* theCleanup = CTrapCleanup::New();
	TRAPD(ret,doMainL());		
	gTest(ret==KErrNone);
	delete theCleanup;	
	__UHEAP_MARKEND;
	return(KErrNone);
	}
