// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This class contains the parsing and processing of BIO messages.
// This functionality was originally carried out in the BIO Server MTM but
// has been moved to the Client MTM for reasons of capability. Potentially,
// BIO parsers may need additional capabilities for their processing and it would
// be difficult to ensure that the Message Server (and all other server MTMs) had
// the same capabilities.  By moving the processing to the Client MTM it is only 
// the client UI app that needs the additional capabilities.
// Parsing and processing of a message is initiated by calling StartCommandL().
// The command function can be one of the following:
// KBiosMtmParse - just parse the message
// KBiosMtmParseThenProcess - parse the message and then process it
// KBiosMtmProcess - just process the mesage (must have been parsed previously)
// Parsing a message involves finding the relevant BIO parser, loading it and then calling
// ParseL() on the parser.
// Processing a message involves finding the relevant BIO parser, loading it and then
// calling ProcessL() on the parser.
// To avoid unresponsiveness, a parse-then-process command is carried out in two steps.
// The parsing is carried out first and then the AO is set up so the processing is
// carried out in a separate RunL().
// Both the parser ParseL() and parser ProcessL() are asynchronous methods.
// 
//

#include <e32uid.h>   	
#include <msvstd.h>
#include <biodb.h>		// bio database
#include <biouids.h>	// contains panic codes
#include <txtrich.h>
#include <txtfmlyr.h>

#include "BIOSCMDS.H"
#include "REGPSDLL.H"   // Parser Registry - used to load the parser
#include "BSP.H"		// CBaseScriptParser, CBaseScriptParser2
#include "BIOOP.H"
#ifdef SYMBIAN_ENABLE_SPLIT_HEADERS
#include <biomessageuids.h>
#include "tmsvbioinfo.h"
#endif

// panic text
_LIT(KBioOpPanic, "BIOO");
	
CBIOOperation* CBIOOperation::NewL(RFs aRFs, CMsvSession& aSession, CBIODatabase* aBDB, TRequestStatus& aCompletionStatus)
	{
	CBIOOperation* self=new(ELeave) CBIOOperation(aRFs, aSession, aBDB, aCompletionStatus);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


CBIOOperation::CBIOOperation(RFs aRFs, CMsvSession& aSession, CBIODatabase* aBDB, TRequestStatus& aCompletionStatus)
: CMsvOperation(aSession, EPriorityStandard, aCompletionStatus), 
iFs(aRFs), iBioDatabase(aBDB), iInOperation(EFalse)
	{
	}

	
void CBIOOperation::ConstructL()
//
// 2nd phase construction
//
	{
	// initiallise progress
	iProgress.iBioState = TBioProgress::EBiosWaiting;

	// create msv entry
	iCurrentEntry = CMsvEntry::NewL(iMsvSession, KMsvRootIndexEntryIdValue, TMsvSelectionOrdering());
	
	// add active object
	CActiveScheduler::Add(this);
	}


CBIOOperation::~CBIOOperation()
	{
	Cancel();
	delete iParser;
	delete iCurrentEntry;
	delete iRegisteredParserDll;
	delete iMessageBody;
	}


void CBIOOperation::StartCommand(const CMsvEntrySelection& aSelection, TInt aCommand, TRequestStatus& aStatus)
//
// BIOServer specific commands
//
	{
	__ASSERT_DEBUG(iBioOperationState == EBiooWaiting, User::Panic(KBioOpPanic, KBIOMessageOperationNotInactive));
	__ASSERT_ALWAYS(aSelection.Count() >= 1, User::Panic(KBioOpPanic, KBIOMessageNotFound));
	
	// to notify caller on completion
	iReportStatus = &aStatus;
	aStatus = KRequestPending;

	TMsvId service;
	TMsvEntry entry;

	TInt error = iMsvSession.GetEntry(aSelection.At(0), service, entry);

	iMtm = entry.iMtm;   
	iService = entry.iServiceId; 

	// perform the command, no need to go on if we already got an error
	if (error == KErrNone)
		{
		switch (aCommand)
			{
		case KBiosMtmParse:
			TRAP(error, ParseL(aSelection, EFalse));
			break;
		case KBiosMtmParseThenProcess:
			TRAP(error, ParseL(aSelection, ETrue));
			break;
		case KBiosMtmProcess:
			TRAP(error, ProcessL(aSelection));
			break;
		default:
			error = KErrNotSupported;
			break;
			}
		};
	
	// check for errors	
	if (error != KErrNone)
		{
		// complete
		iStatus = KRequestPending;
		TRequestStatus* stat = &iStatus;
		SetActive();
		User::RequestComplete(stat, error);
		}
	}


void CBIOOperation::DoCancel()
//
// Cancel
//
	{
	if (iParser)
		iParser->Cancel();
	iInOperation = EFalse;
	User::RequestComplete(iReportStatus, KErrCancel);
	}


void CBIOOperation::RunL()
//
// Active object RunL
//
	{
	TInt error = iStatus.Int();
	if (error == KErrNone)
		{
		switch (iBioOperationState)
			{
		case EBiooParsing:
			{
			iCurrentEntry->SetEntryL(iCurrentMsvId);

			if (iBioOperationOperation == EBioParseAndProcess)
				{
				iBioOperationState = EBiooWaiting;
				ProcessL();
				}
			else
				{
				iBioOperationState = EBiooComplete;
				}
			break;
			}
		case EBiooProcessing:
			iBioOperationState = EBiooComplete;
		case EBiooCreating:
		case EBiooWaiting:
		default:
			break;
			}
		}

	if (iBioOperationState == EBiooComplete || error != KErrNone)
		{
		RunError(error);
		}
	}


TInt CBIOOperation::RunError(TInt aError)
	{
	// also called with aError == KErrNone
	TMsvEntry entry = iCurrentEntry->Entry();
	entry.SetReadOnly(ETrue);
	TInt error = KErrNone;
	TRAP(error,iCurrentEntry->ChangeL(entry));
		
	iBioOperationState = EBiooWaiting;
	iInOperation = EFalse;
	iProgress.iErrorCode = aError;
		
	User::RequestComplete(iReportStatus, aError);
	
	return error;
	}


void CBIOOperation::ParseL(const CMsvEntrySelection& aSelection, TBool aCommit)
	{
	// parse and process?
	iBioOperationOperation = aCommit ? EBioParseAndProcess : EBioParseOnly;

	// set context
    iCurrentMsvId = aSelection[0];
	iCurrentEntry->SetEntryL(iCurrentMsvId);

	// check if it's alreadly parsed
	// iMtmData3 == 0 => not yet parsed
	// iMtmData3 == 1 => parsed
	// iMtmData3 == 2 => processed
	if (iCurrentEntry->Entry().MtmData3()==EBioMsgNotParsed)
		{
		// put in check that it's a valid SMS/smart message	
		StartParserL();
		}
	else if (iCurrentEntry->Entry().MtmData3()==EBioMsgParsed || 
			 iCurrentEntry->Entry().MtmData3()==EBioMsgProcessed)
		{
		iStatus = KRequestPending;
		TRequestStatus* ps = &iStatus;
		SetActive();
		User::RequestComplete(ps, KErrNone);
		iProgress.iBioState = TBioProgress::EBiosCreating;

		if (iBioOperationOperation == EBioParseAndProcess)
			{
			iBioOperationState = EBiooParsing;
			CreateParserL();
			}
		else
			{
			iBioOperationState = EBiooComplete;
			}
		}
	else
		{
		User::Leave(KErrCorrupt);
		}
	iInOperation = ETrue;
	}


void CBIOOperation::CreateParserL()
	{

	// get bio type uid from message	
	TUid parserUid;
	parserUid.iUid = iCurrentEntry->Entry().iBioType;

	// check if we already have a parser and if it's the correct one
	if ((iParser != NULL) && (parserUid != iParser->ParserUid()))
		{
		// first delete the old parser before creating the new one
		delete iParser;
		iParser = NULL;
		}
		
	if (iParser == NULL)
		{	
	  	// create the local variables 
		if (parserUid.iUid == 0)
			User::Leave(KBspSmartMessageNoParserDefined);
		
		// get parser filename for loading
		TFileName parserDllName(iBioDatabase->GetBioParserNameL(parserUid));
		
		// create dll registry
		delete iRegisteredParserDll;
		iRegisteredParserDll = NULL;
		iRegisteredParserDll = CRegisteredParserDll::NewL(parserDllName);

		// get handle to dll		
		RLibrary parserlibrary;
		User::LeaveIfError(iRegisteredParserDll->GetLibrary(iFs, parserlibrary));

		// create a typedef so that we can make a call to the dll to create a parser.
		typedef CBaseScriptParser2* (*NewParserL)(CRegisteredParserDll& registeredparserdll, CMsvEntry& aEntry, RFs& aFs);
		
		// entry point for NewL function found at ordinal 1
		TLibraryFunction libFunc = parserlibrary.Lookup(1);
		
		// check that we found the entry point
		if (libFunc == NULL) 
			User::Leave(KErrBadLibraryEntryPoint) ;

		// create a function pointer to NewL
		NewParserL pFunc = (NewParserL)libFunc;  
		CBaseScriptParser2* parser(NULL);
	
		TInt refcount = iRegisteredParserDll->DllRefCount();
		TRAPD(ret, parser = ((*pFunc)(*iRegisteredParserDll, *iCurrentEntry, iFs)));

	    // unload the library if allocation failed and refcount == 1
		if ((ret != KErrNone) && (iRegisteredParserDll->DllRefCount() == refcount))
			iRegisteredParserDll->ReleaseLibrary();
		
		// leave if error
		User::LeaveIfError(ret);
		iParser = parser;
		}
	}


void CBIOOperation::StartParserL()
//
// We have a parser parse the message (fire up the parser)
//
	{
	__ASSERT_DEBUG(iBioOperationState == EBiooWaiting, User::Panic(KBioOpPanic, KBIOMessageOperationNotInactive));
	
	// get the msg body
	ExtractMessageBodyL();

	// create parser
	CreateParserL();

	// set the entry and start the parser (parser takes ownership of entry)
    // make sure it points to the message
    iCurrentEntry->SetEntryL(iCurrentMsvId);

	// make the entry editable
	TMsvEntry entry(iCurrentEntry->Entry());
	entry.SetReadOnly(EFalse);
	iCurrentEntry->ChangeL(entry);
	
	// parse 
	iParser->ParseL(iStatus, *iMessageBody);
	SetActive();
	
	iBioOperationState = EBiooParsing;
	iProgress.iBioState = TBioProgress::EBiosParsing;
	}


void CBIOOperation::ExtractMessageBodyL()
//
// Get the message body
//
	{
	// build a CRichText object to read in message body
	CParaFormatLayer* paraFormatLayer = CParaFormatLayer::NewL();
    CleanupStack::PushL(paraFormatLayer);
    CCharFormatLayer* charFormatLayer = CCharFormatLayer::NewL();
    CleanupStack::PushL(charFormatLayer);
    CRichText* richText = CRichText::NewL(paraFormatLayer, charFormatLayer);
    CleanupStack::PushL(richText);

	// get store holding message body
    CMsvStore* store = iCurrentEntry->ReadStoreL();
	CleanupStack::PushL(store);

	// check for message body
    if (!store->HasBodyTextL())
    	User::Leave(KErrNotFound);
	
    // extract the body text
    store->RestoreBodyTextL(*richText);
    
    // create buffer to hold body text
	TInt messageLength = richText->DocumentLength();
	if (iMessageBody)
		{
		delete iMessageBody;
		iMessageBody = NULL;
		}
	iMessageBody = HBufC::NewL(messageLength);

	// read in message body
	TPtr messDes = iMessageBody->Des();
	TInt length = messDes.Length();
	while (length < messageLength)
		{
		TPtrC desc = richText->Read(length, messageLength-length);
		messDes.Append(desc);
		length+=desc.Length();
		}
	CleanupStack::PopAndDestroy(4, paraFormatLayer); // store, text, charFormatLayer, paraFormatLayer
	}


void CBIOOperation::ProcessL(const CMsvEntrySelection& aSelection)
//
// successfully parsed the message - do the settings
//
	{
	// set operation
	iBioOperationOperation = EBioProcess;

	// get the id of the bio message
    iCurrentMsvId = aSelection[0];
	iCurrentEntry->SetEntryL(iCurrentMsvId);

	CreateParserL();

	iInOperation = ETrue;
	ProcessL();
	}
	
	
void CBIOOperation::ProcessL()
//
// successfully parsed the message - do the settings
//
	{
	__ASSERT_DEBUG(iBioOperationState == EBiooWaiting, User::Panic(KBioOpPanic, KBIOMessageOperationNotInactive));

	if (iInOperation == EFalse)
		User::Leave(KErrDisconnected);
	
	__ASSERT_ALWAYS(iParser != NULL, User::Panic(KBioOpPanic, KBIOMessageNoParserCreated));
	iProgress.iBioState = TBioProgress::EBiosProcessing;
	
	// make entry readable so we can change things
	TMsvEntry entry = iCurrentEntry->Entry();
	if (entry.ReadOnly())
		{
		entry.SetReadOnly(EFalse);
		iCurrentEntry->ChangeL(entry);
		}

	// start to process message
	iParser->ProcessL(iStatus);
	SetActive();
	
	iBioOperationState = EBiooProcessing;
	}
	
	
const TDesC8& CBIOOperation::ProgressL()
//
// get progress information
//
	{
	iProgressBuf = TBioProgressBuf(iProgress);
	return iProgressBuf;
	}
