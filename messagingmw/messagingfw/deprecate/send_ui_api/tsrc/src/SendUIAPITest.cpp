/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:   SendUIAPITest implementation*
*/




// INCLUDE FILES
#include <Stiftestinterface.h>
#include "SendUIAPITest.h"
#include <SettingServerClient.h>
#include <sendui.h>

// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;

// MACROS
//#define ?macro ?macro_def

// LOCAL CONSTANTS AND MACROS
//const ?type ?constant_var = ?constant;
//#define ?macro_name ?macro_def

// MODULE DATA STRUCTURES
//enum ?declaration
//typedef ?declaration

// LOCAL FUNCTION PROTOTYPES
//?type ?function_name( ?arg_type, ?arg_type );

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;

// ============================= LOCAL FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// ?function_name ?description.
// ?description
// Returns: ?value_1: ?description
//          ?value_n: ?description_line1
//                    ?description_line2
// -----------------------------------------------------------------------------
//
/*
?type ?function_name(
    ?arg_type arg,  // ?description
    ?arg_type arg)  // ?description
    {

    ?code  // ?comment

    // ?comment
    ?code
    }
*/

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendUIAPITest::CSendUIAPITest
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CSendUIAPITest::CSendUIAPITest( 
    CTestModuleIf& aTestModuleIf ):
        CScriptBase( aTestModuleIf ),
        iMessageData( NULL ),
        iMessageAddress( NULL )
    {
    }

// -----------------------------------------------------------------------------
// CSendUIAPITest::ConstructL
// Symbian 2nd phase constructor can leave.
// -----------------------------------------------------------------------------
//
void CSendUIAPITest::ConstructL()
    {
    //Read logger settings to check whether test case name is to be
    //appended to log file name.
    RSettingServer settingServer;
    TInt ret = settingServer.Connect();
    if(ret != KErrNone)
        {
        User::Leave(ret);
        }
    // Struct to StifLogger settigs.
    TLoggerSettings loggerSettings; 
    // Parse StifLogger defaults from STIF initialization file.
    ret = settingServer.GetLoggerSettings(loggerSettings);
    if(ret != KErrNone)
        {
        User::Leave(ret);
        } 
    // Close Setting server session
    settingServer.Close();

    TFileName logFileName;
    
    if(loggerSettings.iAddTestCaseTitle)
        {
        TName title;
        TestModuleIf().GetTestCaseTitleL(title);
        logFileName.Format(KSendUIAPITestLogFileWithTitle, &title);
        }
    else
        {
        logFileName.Copy(KSendUIAPITestLogFile);
        }

    iLog = CStifLogger::NewL( KSendUIAPITestLogPath, 
                          logFileName,
                          CStifLogger::ETxt,	
                          CStifLogger::EFile,
                          EFalse );
                          

    SendTestClassVersion();
    //Create the SendUI var
    iSendUi = CSendUi::NewL();
    }

// -----------------------------------------------------------------------------
// CSendUIAPITest::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CSendUIAPITest* CSendUIAPITest::NewL( 
    CTestModuleIf& aTestModuleIf )
    {
    CSendUIAPITest* self = new (ELeave) CSendUIAPITest( aTestModuleIf );

    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );

    return self;

    }

// Destructor
CSendUIAPITest::~CSendUIAPITest()
    { 

    // Delete resources allocated from test methods
    Delete();
    
    //Delete the SendUI var
    delete iSendUi;
 	
    // Delete logger
    delete iLog; 

    }

// -----------------------------------------------------------------------------
// CSendUIAPITest::?MsgDataSetSubjectL
// ?Test Case for Calling CMessageData's SetSubjectL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt CSendUIAPITest::MsgDataSetSubjectL( TPtrC& aSubject )
   {
   // Print to UI
   _LIT( KSendUIAPITest, "SendUIAPITest" );
   _LIT( KMsgDataSetSubjectL, "In MsgDataSetSubjectL" );
   TestModuleIf().Printf( 0, KSendUIAPITest, KMsgDataSetSubjectL );
   // Print to log file
   iLog->Log( KMsgDataSetSubjectL);
   
   //Create the instance of CMessageData
   CMessageData* messagedata = InitL();
   
   //Set the subject 
   TRAPD(err,messagedata->SetSubjectL(&aSubject));
   if(err == KErrNone)
		{
		return KErrNone;
		}
			
	else
		{
		return err;
		}
 
   }
   
 // -----------------------------------------------------------------------------
// CSendUIAPITest::?MsgDataCompSubjectL
// ?Test Case for Calling CMessageData's Subject fun and comparing it with passed param
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::MsgDataCompSubjectL( TPtrC& aSubject )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KMsgDataCompSubjectL, "In MsgDataCompSubjectL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KMsgDataCompSubjectL );
	// Print to log file
	iLog->Log( KMsgDataCompSubjectL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	//Get the subject from message data
	TPtrC msgdatasubject = messagedata->Subject();

	//Compare the two subjects
	if(msgdatasubject == aSubject)
		{
		return KErrNone;
		}	
	else
		{
		return KErrGeneral;
		}
	}
	
//-----------------------------------------------------------------------------
// CSendUIAPITest::SendTestClassVersion
// Method used to send version of test class
//-----------------------------------------------------------------------------
//
void CSendUIAPITest::SendTestClassVersion()
	{
	TVersion moduleVersion;
	moduleVersion.iMajor = TEST_CLASS_VERSION_MAJOR;
	moduleVersion.iMinor = TEST_CLASS_VERSION_MINOR;
	moduleVersion.iBuild = TEST_CLASS_VERSION_BUILD;
	
	TFileName moduleName;
	moduleName = _L("SendUIAPITest.dll");

	TBool newVersionOfMethod = ETrue;
	TestModuleIf().SendTestModuleVersion(moduleVersion, moduleName, newVersionOfMethod);
	}

// ========================== OTHER EXPORTED FUNCTIONS =========================

// -----------------------------------------------------------------------------
// LibEntryL is a polymorphic Dll entry point.
// Returns: CScriptBase: New CScriptBase derived object
// -----------------------------------------------------------------------------
//
EXPORT_C CScriptBase* LibEntryL( 
    CTestModuleIf& aTestModuleIf ) // Backpointer to STIF Test Framework
    {

    return ( CScriptBase* ) CSendUIAPITest::NewL( aTestModuleIf );

    }


//  End of File
