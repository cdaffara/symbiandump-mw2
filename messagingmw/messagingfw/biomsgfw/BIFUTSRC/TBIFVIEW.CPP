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
// TBIFVIEW.CPP
//
#include <bif.h>

#include <barsc.h>
#include <barsread.h>

#include <e32cons.h>
#include <e32std.h>

#pragma warning(disable:4238)  // nonstandard extension used : class rvalue used as lvalue

LOCAL_D CConsoleBase* console;
const TInt KTestCleanupStack=0x40;
LOCAL_D CTrapCleanup* TheTrapCleanup;
LOCAL_D RFs Fs;
const TInt KMaxBioCommandLine=256;
TBuf<KMaxBioCommandLine> Arg;

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


LOCAL_D void ReadBIFL()
	{
	//
	// Open BIF file
	CBioInfoFileReader* bifFile=CBioInfoFileReader::NewLC(Fs,Arg);

	//
	// Read data from BIF file
	console->Printf(_L("message_type_uid: %x(hex)\n"),bifFile->MessageTypeUid());
	console->Printf(_L("message_parser_name: %S\n"),bifFile->MessageParserName());
	console->Printf(_L("message_app_uid: %x(hex)\n"),bifFile->MessageAppUid());
	console->Printf(_L("message_appctrl_name: %s\n"), bifFile->MessageAppCtrlName());
	console->Printf(_L("general_data: {%x, %x, %x} (hex)\n"),bifFile->GeneralData1(),bifFile->GeneralData2(),bifFile->GeneralData3());
	TBuf<80> buf = bifFile->Description();
	console->Printf(_L("description: %S\n"), &buf);
	buf = bifFile->IconsFilename();
	console->Printf(_L("icons_filename: %S\n"), &buf);
	buf = bifFile->FileExtension();
	console->Printf(_L("file_extension: %S\n"), &buf);

	const CArrayFixFlat<TInt16>* zoomlevels=bifFile->ZoomLevelsLC();
	const TInt count=zoomlevels->Count();
	console->Printf(_L("zoom_levels_count: %d\n"),count);
	console->Printf(_L("zoom_levels: {"));
	for(TInt cc=0;cc<count;++cc)
		console->Printf(_L("%d "),zoomlevels->At(cc));
	console->Printf(_L("}\n"));
	CleanupStack::PopAndDestroy();		// zoomlevels
	
	const CArrayFixFlat<TBioMsgId>* ids=bifFile->IdsLC();
	const TInt idCount=ids->Count();
	console->Printf(_L("ids_count: %d\n"),idCount);
	
	for(TInt dd=0;dd<idCount;dd++)
		{
		console->Printf(_L("*** id#%d ***\n"),dd);

		switch(ids->At(dd).iType)
			{
		case EBioMsgIdUnknown:
			console->Printf(_L("type: EBioMsgIdUnknown\n"));
			break;
		case EBioMsgIdIana:
			console->Printf(_L("type: Iana\n"));
			break;
		case EBioMsgIdNbs:
			console->Printf(_L("type: Nbs\n"));
			break;
		case EBioMsgIdWap:
			console->Printf(_L("type: Wap\n"));
			break;
		case EBioMsgIdWapSecure:
			console->Printf(_L("type: WapSecure\n"));
			break;
		default:
			console->Printf(_L("type: UNKNOWN (%d)\n"),ids->At(dd).iType);
			};


		switch(ids->At(dd).iConfidence)
			{
		case CApaDataRecognizerType::ECertain:
			console->Printf(_L("confidence: Certain\n"));
			break;
		case CApaDataRecognizerType::EProbable:
			console->Printf(_L("confidence: Probable\n"));
			break;
		case CApaDataRecognizerType::EPossible:
			console->Printf(_L("confidence: Possible\n"));
			break;
		case CApaDataRecognizerType::EUnlikely:
			console->Printf(_L("confidence: Unlikely\n"));
			break;
		case CApaDataRecognizerType::ENotRecognized:
			console->Printf(_L("confidence: NotRecognized\n"));
			break;
	
		default:
			console->Printf(_L("confidence: UNKNOWN (%d)\n"),ids->At(dd).iConfidence);
			};
		
		buf = ids->At(dd).iText;
		console->Printf(_L("text: %S\n"), &buf);
		console->Printf(_L("port: %d\n"),ids->At(dd).iPort);
		console->Printf(_L("character_set: %x(hex)\n"),ids->At(dd).iCharacterSet);
		console->Printf(_L("general_data: %x(hex)\n"),ids->At(dd).iGeneralIdData);
		}

	CleanupStack::PopAndDestroy();		// ids
	
	CleanupStack::PopAndDestroy();	// bifFile
	}

void DoConsole()
	{
	console=Console::NewL(_L("EPOC32 - BIF Viewer"),TSize(KConsFullScreen,KConsFullScreen));
	CleanupStack::PushL(console);

	Fs.Connect();

	User::CommandLine(Arg);

	console->Printf(_L("Reading from bif %S\n"),&Arg);
	TRAPD(err,ReadBIFL());
	if (err!=KErrNone)
		console->Printf(_L("Leave code: %d\n"),err);
	else
		console->Printf(_L("BIF read success\n"));

	Fs.Close();

	console->Getch();

	CleanupStack::PopAndDestroy(); // console
	}

GLDEF_C TInt E32Main()
	{
	__UHEAP_MARK;
	setupCleanup();
	TRAPD(err,DoConsole());
	delete TheTrapCleanup;
	__UHEAP_MARKEND;
	return KErrNone;
	}
