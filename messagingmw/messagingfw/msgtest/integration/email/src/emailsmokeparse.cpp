// Copyright (c) 1999-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "emailsmokeparse.h"
#include "emailsmokecommands.h"
#include "smtptestparsers.h"
#include "poptestparsers.h"
#include "imaptestparsers.h"
#include <commsdattypesv1_1.h>
#include <commsdat.h>
using namespace CommsDat;

#ifndef _NO_IAP_PREFS
#include <iapprefs.h>
#endif

// New test commands
_LIT(KCommandEmailClientTest,		"email_client_test");

// Msv client commands
_LIT(KCommandStartClientSession,	"start_client_session");		// 0 parameters, initialises CMsvTestUtils, and frees resources. Should be called at the start of every section
_LIT(KCommandResetClient,			"reset_client");				// 0 parameters, resets CMsvTestUtils, and frees resources. Should be called at the end of every section

// Msv utility commands
_LIT(KCommandCleanMessageFolder,	"clean_message_folder");		// 0 parameters, deletes message store
_LIT(KCommandCheckMessageStore,		"check_message_store");
_LIT(KCommandAddComment,			"add_comment");					// n parameters, adds a comment to the log, concatenates parameters
_LIT(KCommandCountChildren,			"count_children");				// 0 parameters, counts entries in current folder
_LIT(KCommandCheckService,			"check_service");				// 0 parameters, checks that a service has been created or selected
_LIT(KCommandWait,					"wait");						// 1 parameter, waits for n seconds...
_LIT(KCommandTestEntry,				"test_entry");					// 1 parameter, tests the attributes of the currently selected message
_LIT(KCommandChangeDrive,			"change_to_drive");				// 1 parameter, changes mail location to specified drive
_LIT(KCommandRemoveFileOrDir,		"remove_file_or_dir");			// 1 parameter, removes the specified directory
_LIT(KCommandDumpMailStore,			"dump_mail_store");				// 1 parameter, dumps the contents of the mail store into specified directory
_LIT(KCommandCompareFiles,			"compare_files");				// 2 parameters, compares the contents of MsgLogs\T_EMAIL\dump\[param1] with MsgTest\emailsmoke\reference\[param2]
_LIT(KCommandDumpBodyText,			"dump_body_text");				// 1 parameter, dumps the body text of the selected messages into \MsgLogs\T_EMAIL\dump\param1 file
_LIT(KCommandChangeCharset,			"change_charset");				// 1 parameter, sets the new charset info on the selected messages to the specified value.
_LIT(KCommandGetAttachmentName,		"dump_attachment_names");		// 1 parameter, dumps the attachment names of the selected entries into \MsgLogs\T_EMAIL\dump\param1 file
_LIT(KCommandDumpHeaders,			"dump_header_fields");			// 1 parameter, dumps the header fields of the selected entries into \MsgLogs\T_EMAIL\dump\param1 file

// Msv selection commands
_LIT(KCommandSelectFolder,			"select_folder");				// 1 parameter, selects a folder
_LIT(KCommandSelectEntry,			"select_entry");				// 1 parameter, select an entry by looking in iDetails and iDescription (Note: can only take 1 parameter)
_LIT(KCommandSelectRootFolder,		"select_root_folder");			// 0 parameters, selects root
_LIT(KCommandSelectAllMessages,		"select_all_messages");			// 0 parameters, selects all entries in current folder
_LIT(KCommandSelectDestination,		"select_destination_folder");	// 1 parameter, selects a destination folder used for copying and moving
_LIT(KCommandCheckSelectionCount,	"check_selection_count");		// 1 parameter, tests the current selection count == parameter
_LIT(KCommandSelectLastInSelection,	"select_last_in_selection");	// 1 parameter, reduces the current selection to its last N entries
_LIT(KCommandSelectFirstInSelection,"select_first_in_selection");	// 1 parameter, reduces the current selection to its first N entries
_LIT(KCommandCheckRemoteFolderSize,	"check_remote_folder_size");	// 1 parameter, checks the remote folder size is what is expected
_LIT(KCommandSelectPopulated,		"select_populated");			// 0 parameter, selects populated messages in the current selection
_LIT(KCommandCheckNewFlag,			"check_new_flag");				// 2 parameters, tests if the selected entry(with the index Parameter1) has its New flag == parameter2
_LIT(KCommandCheckPriority,			"check_priority");				// 2 parameters, tests if the selected entry(with the index Parameter1) has its Priority flag == parameter2
_LIT(KCommandCheckAttachment,		"check_attachment");			// 2 parameters, tests if the selected entry(with the index Parameter1) has its Attachment flag == parameter2
_LIT(KCommandCheckBodyText,			"check_body_text");				// 2 parameters, tests if the selected entry(with the index Parameter1) has Body Text == parameter2
_LIT(KCommandCheckChildren,			"check_children");				// 2 parameters, tests if the selected entry(with the index Parameter1) has children == parameter2

// The main section
_LIT(KMainSectionName, "main");

// Email utility commands
_LIT(KCommandEmailCreateMessage,	"create_email_message");		// 2 parameters, creates a message from a text file (param 2) from directory (param 1)
																	// Optional param howmany messages, if not specified it is 1

_LIT(KCommandEmailAddIAP,			"add_iap");						// 1 parameter, uses an IAP (internet access point) as defined in CommDB
_LIT(KCommandEmailAddIAPByName,		"add_iap_by_name");				// 1 parameter, finds an IAP in CommDB using its name

// Msv client operations
_LIT(KCommandMsvCopySelection,		"copy");						// 0 parameters, copies a 'selection' to a 'destination folder'
_LIT(KCommandMsvMoveSelection,		"move");						// 0 parameters, moves a 'selection' to a 'destination folder'
_LIT(KCommandMsvDeleteSelection,	"delete");
_LIT(KCommandDeleteChildren,		"delete_children");				// 1 parameter, deletes the children of the selected entry (with the index Parameter).
																	// NOTE: this command sets 'selection' to the selected entry's children.
// Test entry commands
_LIT(KCommandTestEntryVisible,		"visible");						// 0 parameters, copies a 'selection' to a 'destination folder'
_LIT(KCommandTestEntryComplete,		"complete");					// 0 parameters, moves a 'selection' to a 'destination folder'

// True or False ?
_LIT(KParameterETrue,				"true");	// !EFalse
_LIT(KParameterEFalse,				"false");	// EFalse

//
//
// CMainSectionParser
//

CMainSectionParser* CMainSectionParser::NewL(CMainTestHarness& aMainTest, CTestScript& aScript, TBool aKeepLogFile)
	{
	CMainSectionParser* self = new (ELeave) CMainSectionParser(aMainTest, aScript, aKeepLogFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMainSectionParser::LogComment(const TDesC& aComment)
	{
	iMainTest.TestConsole().Printf(aComment);
	}

void CMainSectionParser::ConstructL()
	{
	CBaseSectionParser::ConstructL(KMainSectionName);
	iCommandParsers->AppendL(CNewEmailTestCommandParser::NewL(iScript, iMainTest,iKeepLogFile));
	}

CMainSectionParser::CMainSectionParser(CMainTestHarness& aMainTest, CTestScript& aScript, TBool aKeepLogFile) : CBaseSectionParser(aScript), iMainTest(aMainTest), iKeepLogFile(aKeepLogFile)
	{
	}




//
//
// CEmailClientSectionParser
//

CEmailClientSectionParser* CEmailClientSectionParser::NewL(CEmailClientTest& aEmailClientTest, CTestScript& aScript, const TDesC& aNewSectionName)
	{
	CEmailClientSectionParser* self = new (ELeave) CEmailClientSectionParser(aEmailClientTest, aScript);
	CleanupStack::PushL(self);
	self->ConstructL(aNewSectionName);
	CleanupStack::Pop(self);
	return self;
	}

void CEmailClientSectionParser::LogComment(const TDesC& aComment)
	{
	iEmailTest.EmailTestUtils().WriteComment(aComment);
	}

void CEmailClientSectionParser::ConstructL(const TDesC& aNewSectionName)
	{
	CBaseSectionParser::ConstructL(aNewSectionName);
	TTestDebugInfo debugInfo = iEmailTest.GetDebugInfo();
	debugInfo.SetTestHarnessName(iSection->SectionPosition());
	iEmailTest.SetDebugInfo(debugInfo);
	iCommandParsers->AppendL(CNewSmtpTestCommandParser::NewL(iScript, iEmailTest));
	iCommandParsers->AppendL(CNewPop3TestCommandParser::NewL(iScript, iEmailTest));
	iCommandParsers->AppendL(CNewImapTestCommandParser::NewL(iScript, iEmailTest));
	iCommandParsers->AppendL(CMsvSelectParser::NewL(iEmailTest));
	iCommandParsers->AppendL(CMsvUtilitiesParser::NewL(iEmailTest));
	iCommandParsers->AppendL(CEmailUtilitiesParser::NewL(iEmailTest));
	iCommandParsers->AppendL(CMsvOperationParser::NewL(iEmailTest));
	}

CEmailClientSectionParser::CEmailClientSectionParser(CEmailClientTest& aEmailTest, CTestScript& aScript) : CBaseSectionParser(aScript), iEmailTest(aEmailTest)
	{
	}



//
//
// CNewEmailTestCommandParser
//

CNewEmailTestCommandParser* CNewEmailTestCommandParser::NewL(CTestScript& aScript, CMainTestHarness& aMainTest, TBool aKeepLogFile)
	{
	CNewEmailTestCommandParser* self = new (ELeave) CNewEmailTestCommandParser(aScript, aMainTest, aKeepLogFile);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CNewEmailTestCommandParser::ProcessL()
	{
	// Create an email test utilities
	CEmailTestUtils* emailTestUtils = CEmailTestUtils::NewL(iMainTest.TestConsole(),iKeepLogFile? ETuKeepLogFile:0);
	CleanupStack::PushL(emailTestUtils);
	if (iKeepLogFile)
		emailTestUtils->WriteComment(KTestHeader);

	// Create an email test, give it the test utilities
	CEmailClientTest* emailTest = CEmailClientTest::NewL(emailTestUtils, iMainTest.TestConsole());
	CleanupStack::Pop(emailTestUtils); // emailTest has taken ownership
	CleanupStack::PushL(emailTest);

	// Attach the email test to the main test
	iMainTest.AddStateL(emailTest, iDebugInfo);
	CleanupStack::Pop(emailTest); // emailTest is now owned by the iMainTest

	// Check that there is one argument, the email test section name
	CheckNumberOfArgumentsL(1);

	// Create an email section parser
	CEmailClientSectionParser* sectionParser = CEmailClientSectionParser::NewL(*emailTest, iScript, (*iArgumentList)[0]);
	CleanupStack::PushL(sectionParser);
	sectionParser->ParseL();
	CleanupStack::PopAndDestroy(sectionParser);
	}

void CNewEmailTestCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandEmailClientTest);
	}

CNewEmailTestCommandParser::CNewEmailTestCommandParser(CTestScript& aScript, CMainTestHarness& aMainTest, TBool aKeepLogFile) : iMainTest(aMainTest), iScript(aScript), iKeepLogFile(aKeepLogFile)
	{
	}

//
//
// CMsvUtilitiesParser
//

CMsvUtilitiesParser* CMsvUtilitiesParser::NewL(CMsvClientTest& aMsvClientTest)
	{
	CMsvUtilitiesParser* self = new (ELeave) CMsvUtilitiesParser(aMsvClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMsvUtilitiesParser::ProcessL()
	{
	if ((*iCurrentCommand) == KCommandCleanMessageFolder)
		{
		iMsvClientTest.AddStateL(new (ELeave) CCleanMessageFolder(iMsvClientTest.MsvTestUtils()), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandDumpMailStore)
		{
		CheckNumberOfArgumentsL(1); // directory where to dump the mail store contents
		iMsvClientTest.AddStateL(new (ELeave) CDumpMailStore(iMsvClientTest.MsvTestUtils(), (*iArgumentList)[0]), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCompareFiles)
		{
		CheckNumberOfArgumentsL(2); // files to compare
		iMsvClientTest.AddStateL(new (ELeave) CCompareFiles(iMsvClientTest.MsvTestUtils(), (*iArgumentList)[0], (*iArgumentList)[1]), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandDumpBodyText)
		{
		CheckNumberOfArgumentsL(1); // file where to dump the body text
		iMsvClientTest.AddStateL(new (ELeave) CDumpBodyText(*(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils(), (*iArgumentList)[0]), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandGetAttachmentName)
	{
		CheckNumberOfArgumentsL(1); // file where to dump the subject
		iMsvClientTest.AddStateL(new (ELeave) CGetAttachmentNames(*(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils(), (*iArgumentList)[0]), iDebugInfo);
	}
	else if ((*iCurrentCommand) == KCommandDumpHeaders)
	{
		CheckNumberOfArgumentsL(1); // file where to dump the subject
		iMsvClientTest.AddStateL(new (ELeave) CDumpHeaderFields(*(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils(), (*iArgumentList)[0]), iDebugInfo);
	}
	else if ((*iCurrentCommand) == KCommandChangeCharset)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TUint newCharset;
		User::LeaveIfError(lex.Val(newCharset));
		iMsvClientTest.AddStateL(new (ELeave) CChangeCharset(newCharset, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckMessageStore)
		{
		CheckNumberOfArgumentsL(2);
		TParse fileName;
		ResolveFile(iMsvClientTest.MsvTestUtils().FileSession(), (*iArgumentList)[0], (*iArgumentList)[1], fileName);
		iMsvClientTest.AddStateL(new (ELeave) CCheckMessageFolder(fileName.FullName(), iMsvClientTest.MsvTestUtils()), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandStartClientSession)
		{
		iMsvClientTest.AddStateL(new (ELeave) CGoClientSide(iMsvClientTest.MsvTestUtils()), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandAddComment)
		{
		iMsvClientTest.AddStateL(CAddComment::NewL(*iArgumentList, iMsvClientTest.MsvTestUtils()), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandResetClient)
		{
		iMsvClientTest.AddStateL(new (ELeave) CLongWait(), iDebugInfo);
		iMsvClientTest.AddStateL(new (ELeave) CResetMessageServer(iMsvClientTest.MsvTestUtils()), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandTestEntry)
		{
		iMsvClientTest.AddStateL(new (ELeave) CMsvTestEntry(iMsvClientTest), iDebugInfo);

		// eh xxxx, set up section parser for the details
		}
	else if ((*iCurrentCommand) == KCommandChangeDrive)
		{
		CheckNumberOfArgumentsL(1);
		
		TDriveUnit driveUnit((*iArgumentList)[0]);
		
		iMsvClientTest.AddStateL(new (ELeave) CChangeMsvDrive(iMsvClientTest, TInt(driveUnit)), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandRemoveFileOrDir)
		{
		CheckNumberOfArgumentsL(1);
		
		iMsvClientTest.AddStateL(new (ELeave) CRemoveFileOrDir(iMsvClientTest.MsvTestUtils(), (*iArgumentList)[0]), iDebugInfo);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

void CMsvUtilitiesParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandCleanMessageFolder);
	AddCommandL(KCommandCheckMessageStore);
	AddCommandL(KCommandAddComment);
	AddCommandL(KCommandStartClientSession);
	AddCommandL(KCommandResetClient);
	AddCommandL(KCommandTestEntry);
	AddCommandL(KCommandChangeDrive);
	AddCommandL(KCommandRemoveFileOrDir);
	AddCommandL(KCommandDumpMailStore);
	AddCommandL(KCommandCompareFiles);
	AddCommandL(KCommandDumpBodyText);
	AddCommandL(KCommandChangeCharset);
	AddCommandL(KCommandGetAttachmentName);
	AddCommandL(KCommandDumpHeaders);
	}

CMsvUtilitiesParser::CMsvUtilitiesParser(CMsvClientTest& aMsvClientTest) : iMsvClientTest(aMsvClientTest)
	{
	}








//
//
// CMsvSelectParser
//

CMsvSelectParser* CMsvSelectParser::NewL(CMsvClientTest& aMsvClientTest)
	{
	CMsvSelectParser* self = new (ELeave) CMsvSelectParser(aMsvClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}


void CMsvSelectParser::ProcessL()
	{
	if ((*iCurrentCommand) == KCommandSelectFolder)
		{
		CheckNumberOfArgumentsL(1);
//		iMsvClientTest.AddStateL(CSelectEntry::NewL((*iArgumentList)[0], iMsvClientTest.iSelectedFolder, iMsvClientTest.iSelectedFolder, iMsvClientTest.MsvTestUtils())
//									, iDebugInfo);
		iMsvClientTest.AddStateL(CSelectEntry::NewL((*iArgumentList)[0], iMsvClientTest.iSelectedFolder, &(iMsvClientTest.iSelectedFolder), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSelectEntry)
		{
		CheckNumberOfArgumentsL(1);
//		iMsvClientTest.AddStateL(CSelectEntry::NewL((*iArgumentList)[0], iMsvClientTest.iSelectedFolder, iMsvClientTest.iSelectedFolder, iMsvClientTest.MsvTestUtils())
//									, iDebugInfo);
		iMsvClientTest.AddStateL(CSelectEntry::NewL((*iArgumentList)[0], iMsvClientTest.iSelectedFolder, iMsvClientTest.iCurrentSelection, iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSelectRootFolder)
		{
		CheckNumberOfArgumentsL(0);
		iMsvClientTest.AddStateL(new (ELeave) CSelectEntryById(KMsvRootIndexEntryId, iMsvClientTest.iSelectedFolder, iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSelectAllMessages)
		{
		CheckNumberOfArgumentsL(0);
		iMsvClientTest.AddStateL(new (ELeave) CSelectAllMessages(iMsvClientTest.iSelectedFolder, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSelectDestination)
		{
		CheckNumberOfArgumentsL(1);
//		iMsvClientTest.AddStateL(CSelectEntry::NewL((*iArgumentList)[0], iMsvClientTest.iSelectedFolder, iMsvClientTest.iDestinationFolder, iMsvClientTest.MsvTestUtils() )
//									, iDebugInfo);
		iMsvClientTest.AddStateL(CSelectEntry::NewL((*iArgumentList)[0], iMsvClientTest.iSelectedFolder, &(iMsvClientTest.iDestinationFolder), iMsvClientTest.MsvTestUtils() )
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckSelectionCount)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt expectedCount;
		User::LeaveIfError(lex.Val(expectedCount));
		iMsvClientTest.AddStateL(new (ELeave) CCheckSelectionCount(expectedCount, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckNewFlag)
		{
		CheckNumberOfArgumentsL(2);

		// Get the index of the Selected Entry to Test
		TLex lex((*iArgumentList)[0]);
		TInt selectedIndex;
		User::LeaveIfError(lex.Val(selectedIndex));

		// Get the expected New Status for this entry
		lex = ((*iArgumentList)[1]);
		TInt argumentValue;
		User::LeaveIfError(lex.Val(argumentValue));
		TBool expectedNewStatus = STATIC_CAST(TBool, argumentValue);
		iMsvClientTest.AddStateL(new (ELeave) CCheckNewFlag(selectedIndex, expectedNewStatus, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckPriority)
		{
		CheckNumberOfArgumentsL(2);

		// Get the index of the Selected Entry to Test
		TLex lex((*iArgumentList)[0]);
		TInt selectedIndex;
		User::LeaveIfError(lex.Val(selectedIndex));

		// Get the expected New Status for this entry
		lex = ((*iArgumentList)[1]);
		TInt priority;
		User::LeaveIfError(lex.Val(priority));
		iMsvClientTest.AddStateL(new (ELeave) CCheckPriority(selectedIndex, priority, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckAttachment)
		{
		CheckNumberOfArgumentsL(2);

		// Get the index of the Selected Entry to Test
		TLex lex((*iArgumentList)[0]);
		TInt selectedIndex;
		User::LeaveIfError(lex.Val(selectedIndex));

		// Get the expected Attachment Status for this entry
		lex = ((*iArgumentList)[1]);
		TInt argumentValue;
		User::LeaveIfError(lex.Val(argumentValue));
		TBool testAttachment = STATIC_CAST(TBool, argumentValue);
		iMsvClientTest.AddStateL(new (ELeave) CCheckAttachment(selectedIndex, testAttachment, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckBodyText)
		{
		CheckNumberOfArgumentsL(2);

		// Get the index of the Selected Entry to Test
		TLex lex((*iArgumentList)[0]);
		TInt selectedIndex;
		User::LeaveIfError(lex.Val(selectedIndex));

		// Get the expected Body Text
		TBuf<KMaxTestBodyTxt> bodyText = (*iArgumentList)[1];
		iMsvClientTest.AddStateL(CCheckBodyText::NewL(selectedIndex, bodyText, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckChildren)
		{
		CheckNumberOfArgumentsL(2);

		// Get the index of the Selected Entry to Test
		TLex lex((*iArgumentList)[0]);
		TInt selectedIndex;
		User::LeaveIfError(lex.Val(selectedIndex));

		// Get the expected children Status for this entry
		lex = ((*iArgumentList)[1]);
		TInt argumentValue;
		User::LeaveIfError(lex.Val(argumentValue));
		TBool testChildren = STATIC_CAST(TBool, argumentValue);
		iMsvClientTest.AddStateL(new (ELeave) CCheckChildren(selectedIndex, testChildren, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSelectLastInSelection)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt expectedCount;
		User::LeaveIfError(lex.Val(expectedCount));
		iMsvClientTest.AddStateL(new (ELeave) CSelectLastInSelection(expectedCount, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSelectFirstInSelection)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt expectedCount;
		User::LeaveIfError(lex.Val(expectedCount));
		iMsvClientTest.AddStateL(new (ELeave) CSelectFirstInSelection(expectedCount, *(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckRemoteFolderSize)
		{
		CheckNumberOfArgumentsL(1);
		TLex lex((*iArgumentList)[0]);
		TInt expectedCount;
		User::LeaveIfError(lex.Val(expectedCount));
		iMsvClientTest.AddStateL(new (ELeave) CCheckRemoteFolderSize(expectedCount, iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandSelectPopulated)
		{
		CheckNumberOfArgumentsL(0);
		iMsvClientTest.AddStateL(new (ELeave) CSelectPopulatedInSelection(*(iMsvClientTest.iCurrentSelection), iMsvClientTest.MsvTestUtils())
									, iDebugInfo);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

void CMsvSelectParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandSelectRootFolder);
	AddCommandL(KCommandSelectFolder);
	AddCommandL(KCommandSelectEntry);
	AddCommandL(KCommandSelectAllMessages);
	AddCommandL(KCommandSelectDestination);
	AddCommandL(KCommandCheckSelectionCount);
	AddCommandL(KCommandSelectLastInSelection);
	AddCommandL(KCommandSelectFirstInSelection);
	AddCommandL(KCommandCheckRemoteFolderSize);
	AddCommandL(KCommandSelectPopulated);
	AddCommandL(KCommandCheckNewFlag);
	AddCommandL(KCommandCheckPriority);
	AddCommandL(KCommandCheckAttachment);
	AddCommandL(KCommandCheckBodyText);
	AddCommandL(KCommandCheckChildren);
	}

CMsvSelectParser::CMsvSelectParser(CMsvClientTest& aMsvClientTest) : iMsvClientTest(aMsvClientTest)
	{
	}



//
//
// CEmailUtilitiesParser
//

CEmailUtilitiesParser* CEmailUtilitiesParser::NewL(CEmailClientTest& aEmailClientTest)
	{
	CEmailUtilitiesParser* self = new (ELeave) CEmailUtilitiesParser(aEmailClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CEmailUtilitiesParser::ProcessL()
	{
	TInt howMany;

	if ((*iCurrentCommand) == KCommandEmailCreateMessage)
		{
		if (iArgumentList->Count() == 3) // If optional howmany is specified
			{
			TPtrC param = (*iArgumentList)[2];
			TLex lex(param);
			if (lex.Val(howMany)!=KErrNone)
				howMany=1;
			}
		else
			{
			howMany = 1;
			CheckNumberOfArgumentsL(2);
			}
		iEmailClientTest.AddStateL(new (ELeave) CLongWait(), iDebugInfo);
		iEmailClientTest.AddStateL(new (ELeave) CResetMessageServer(iEmailClientTest.EmailTestUtils()), iDebugInfo);
		TParse fileName;
		ResolveFile(iEmailClientTest.MsvTestUtils().FileSession(), (*iArgumentList)[0], (*iArgumentList)[1], fileName);
		iEmailClientTest.AddStateL(new (ELeave) CCreateEmailMessage(iEmailClientTest.iSelectedFolder, iEmailClientTest.iCurrentServiceId, fileName.FullName(), iEmailClientTest.EmailTestUtils(),howMany), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCountChildren)
		{
		iEmailClientTest.AddStateL(new (ELeave) CCountChildren(iEmailClientTest.iSelectedFolder, iEmailClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandCheckService)
		{
		iEmailClientTest.AddStateL(new (ELeave) CCheckService(iEmailClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandWait)
		{
		CheckNumberOfArgumentsL(1);

		// Get the Wait Time
		TLex lex((*iArgumentList)[0]);
		TInt waitTime;
		User::LeaveIfError(lex.Val(waitTime));

		iEmailClientTest.AddStateL(new (ELeave) CLongWait(waitTime), iDebugInfo);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

void CEmailUtilitiesParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandEmailCreateMessage);
	AddCommandL(KCommandCountChildren);
	AddCommandL(KCommandCheckService);
	AddCommandL(KCommandWait);
	}

CEmailUtilitiesParser::CEmailUtilitiesParser(CEmailClientTest& aEmailClientTest) : iEmailClientTest(aEmailClientTest)
	{
	}






//
//
// CMsvOperationParser
//

CMsvOperationParser* CMsvOperationParser::NewL(CMsvClientTest& aMsvClientTest)
	{
	CMsvOperationParser* self = new (ELeave) CMsvOperationParser(aMsvClientTest);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMsvOperationParser::ProcessL()
	{
	if ((*iCurrentCommand) == KCommandMsvCopySelection)
		{
		CheckNumberOfArgumentsL(0);
		iMsvClientTest.AddStateL(new (ELeave) CCopySelection(iMsvClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandMsvMoveSelection)
		{
		CheckNumberOfArgumentsL(0);
		iMsvClientTest.AddStateL(new (ELeave) CMoveSelection(iMsvClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandMsvDeleteSelection)
		{
		CheckNumberOfArgumentsL(0);
		iMsvClientTest.AddStateL(new (ELeave) CDeleteSelection(iMsvClientTest), iDebugInfo);
		}
	else if ((*iCurrentCommand) == KCommandDeleteChildren)
		{
		CheckNumberOfArgumentsL(1);
		// Get the index of the Selected Entry to Test
		TLex lex((*iArgumentList)[0]);
		TInt selectedIndex;
		User::LeaveIfError(lex.Val(selectedIndex));

		iMsvClientTest.AddStateL(new (ELeave) CDeleteChildren(iMsvClientTest, selectedIndex), iDebugInfo);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

void CMsvOperationParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandMsvCopySelection);
	AddCommandL(KCommandMsvMoveSelection);
	AddCommandL(KCommandMsvDeleteSelection);
	AddCommandL(KCommandDeleteChildren);
	}

CMsvOperationParser::CMsvOperationParser(CMsvClientTest& aMsvClientTest) : iMsvClientTest(aMsvClientTest)
	{
	}





//
//
// CEmailServiceSectionParser
//


CEmailServiceSectionParser::~CEmailServiceSectionParser()
	{
	}

void CEmailServiceSectionParser::LogComment(const TDesC& aComment)
	{
	iTestHarness.EmailTestUtils().WriteComment(aComment);
	}

void CEmailServiceSectionParser::ConstructL(const TDesC& /* aSectionName */, CCreateEmailService& aCreateServiceState)
	{
	// Set up the command parser that will add the IAP details to the state created above
	AddCommandParserL(CEmailServiceCommands::NewL(aCreateServiceState));
	}

CEmailServiceSectionParser::CEmailServiceSectionParser(CTestScript& aScript, CEmailClientTest& aTestHarness) : CBaseSectionParser(aScript), iTestHarness(aTestHarness)
	{
	}



//
//
// CEmailServiceCommands
//

TInt CEmailServiceCommands::FindIAPByNameL()
	{
	TInt found=KErrNotFound;

	CMDBSession* dbSession = CMDBSession::NewL(CMDBSession::LatestVersion());
	CleanupStack::PushL(dbSession);
	CCDIAPRecord *iapRecord = static_cast<CCDIAPRecord*>(CCDRecordBase::RecordFactoryL(KCDTIdIAPRecord));
	CleanupStack::PushL(iapRecord);
	
	iapRecord->iRecordName.SetMaxLengthL(((*iArgumentList)[0]).Length());
	iapRecord->iRecordName = ((*iArgumentList)[0]);
	if(iapRecord->FindL(*dbSession))
		{
		found = iapRecord->iRecordTag.RecordId();	
		}	
	else
		{
		User::Leave(KErrNotFound);	
		}	
		
	CleanupStack::PopAndDestroy(2);//dbSession, iapRecord
	return found;
	}

void CEmailServiceCommands::ProcessL()
	{

	if ((*iCurrentCommand) == KCommandEmailAddIAP)
		{
#ifndef _NO_IAP_PREFS
// eh xxxx
// Out of date code that needs updating.
// The new code should set the iCreateEmailServiceState so that it creates the IAP preferences.
// The CCreateEmailService state will need to be changed to support this
/*
		CheckNumberOfArgumentsL(1);
		TImIAPChoice iap;
		TLex lex((*iArgumentList)[0]);
		User::LeaveIfError(lex.Val(iap.iIAP, EDecimal));
		iap.iDialogPref = ECommDbDialogPrefDoNotPrompt;
		CImIAPPreferences& iapPreferences = iIapDetails.IAPPreferences();
		iapPreferences.AddIAPL(iap, iapPreferences.NumberOfIAPs());
*/
#else // _NO_IAP_PREFS

		CheckNumberOfArgumentsL(1);
		TUint8 iap;
		TLex lex((*iArgumentList)[0]);
		User::LeaveIfError(lex.Val(iap, EDecimal));
		iCreateEmailServiceState.Entry().iMtmData1 = iap;
#endif
		}
	else if ((*iCurrentCommand) == KCommandEmailAddIAPByName)
		{
#ifndef _NO_IAP_PREFS
		CheckNumberOfArgumentsL(1);
		iCreateEmailServiceState.Entry().SetMtmData1(FindIAPByNameL());
#endif
		}
	}

CEmailServiceCommands* CEmailServiceCommands::NewL(CCreateEmailService& aCreateServiceState)
	{
	CEmailServiceCommands* self = new (ELeave) CEmailServiceCommands(aCreateServiceState);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CEmailServiceCommands::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandEmailAddIAP);
	AddCommandL(KCommandEmailAddIAPByName);
	}

CEmailServiceCommands::CEmailServiceCommands(CCreateEmailService& aCreateServiceState) : iCreateEmailServiceState(aCreateServiceState)
	{
	}



//
//
// CMsvTestEntryCommandParser
//

void CMsvTestEntryCommandParser::ProcessL()
	{
	if ((*iCurrentCommand) == KCommandTestEntryVisible)
		{
		iTestEntry.TestVisible(GetBooleanArgumentL());
		}
	else if ((*iCurrentCommand) == KCommandTestEntryComplete)
		{
		iTestEntry.TestComplete(GetBooleanArgumentL());
		}
	}

CMsvTestEntryCommandParser* CMsvTestEntryCommandParser::NewL(CMsvTestEntry& aTestEntry)
	{
	CMsvTestEntryCommandParser* self = CMsvTestEntryCommandParser::NewL(aTestEntry);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
	return self;
	}

void CMsvTestEntryCommandParser::ConstructL()
	{
	CBaseCommandParser::ConstructL();
	AddCommandL(KCommandTestEntryVisible);
	AddCommandL(KCommandTestEntryComplete);
	}

CMsvTestEntryCommandParser::CMsvTestEntryCommandParser(CMsvTestEntry& aTestEntry) : iTestEntry(aTestEntry)
	{
	}

TBool CMsvTestEntryCommandParser::GetBooleanArgumentL()
	{
	TBool result = ETrue;

	CheckNumberOfArgumentsL(1);
	
	if ((*iArgumentList)[0] == KParameterETrue)
		{
		result = ETrue;
		}
	else if ((*iArgumentList)[0] == KParameterEFalse)
		{
		result = EFalse;
		}
	else
		{
		User::Leave(KErrArgument);
		}

	return result;
	}



//
//
// CMsvTestEntrySectionParser
//

CMsvTestEntrySectionParser* CMsvTestEntrySectionParser::NewL(const TDesC& aSectionName, CTestScript& aScript, CMsvTestEntry& aTestState)
	{
	CMsvTestEntrySectionParser* self = new (ELeave) CMsvTestEntrySectionParser(aScript, aTestState);
	CleanupStack::PushL(self);
	self->ConstructL(aSectionName);
	CleanupStack::Pop(self);
	return self;
	}

CMsvTestEntrySectionParser::~CMsvTestEntrySectionParser()
	{
	}

void CMsvTestEntrySectionParser::ConstructL(const TDesC& aSectionName)
	{
	CBaseSectionParser::ConstructL(aSectionName);
	AddCommandParserL(CMsvTestEntryCommandParser::NewL(iTestState));
	}

CMsvTestEntrySectionParser::CMsvTestEntrySectionParser(CTestScript& aScript, CMsvTestEntry& aTestState) 
: CBaseSectionParser(aScript),
  iTestState(aTestState)
	{
	}
	
void CMsvTestEntrySectionParser::LogComment(const TDesC& /*aComment*/)
	{
	// Log comment
	}
