/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:         SendUIAPITestBlocks implementation

*
*/










// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "SendUIAPITest.h"
#include <SendUi.h>
#include <EIKENV.H>
#include <cmessagedata.h>
#include <CMessageAddress.h>
#include <TSendingCapabilities.h>
#include <TXTRICH.H>
#include <EIKDEF.H>
#include <eikmobs.h>
#include <f32file.h>
#include <S32MEM.H> 
#include <CSendingServiceInfo.h>
#include <SendUiConsts.h>


// EXTERNAL DATA STRUCTURES
//extern  ?external_data;

// EXTERNAL FUNCTION PROTOTYPES  
//extern ?external_function( ?arg_type,?arg_type );

// CONSTANTS
//const ?type ?constant_var = ?constant;
_LIT( KTestText, "TestText1234567910");
const TInt KMessageBufferSize = 4 * sizeof(TUint32);
const TInt KFilePathSize = 256;
const TUid KServideId = {0x10005522};


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
CMessageData* CSendUIAPITest::InitL(
	 )
    {
	if(!iMessageData)
	{
	 	iMessageData = CMessageData::NewL();
	}
	return iMessageData;	
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CSendUIAPITest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CSendUIAPITest::Delete() 
	 {
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KDelete, "In Delete" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KDelete );
	// Print to log file
	iLog->Log( KDelete );

	if(iMessageData)	    
	{
		// Delete Message Data Object
		_LIT( KDeleteObj, "Delete CMessageData Object" );
		iLog->Log( KDeleteObj );
		delete iMessageData;	
		iMessageData = NULL;
	}
	if(iMessageAddress)
	{
		// Print to log file
		_LIT( KDeleteMsgAdrObj, "Delete CMessageAddress Object" );
		iLog->Log( KDeleteMsgAdrObj );
		delete iMessageAddress;	
		iMessageAddress = NULL;
	}
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CSendUIAPITest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        //ENTRY( "Example", CSendUIAPITest::ExampleL ),
        //CMessageData
        ENTRY( "MessageDataNewL", CSendUIAPITest::TestMessageDataNewL ),
		ENTRY( "MessageDataSetSubjectL", CSendUIAPITest::TestMessageDataSetSubjectL ),
		ENTRY( "MessageDataSubject", CSendUIAPITest::TestMessageDataSubjectL ),
		ENTRY( "MessageDataAppendAttachmentL", CSendUIAPITest::TestMessageDataAppendAttachmentL ),
		ENTRY( "MessageDataAttachmentArray", CSendUIAPITest::TestMessageDataAttachmentArrayL ),
		ENTRY( "MessageDataClearAttachmentArray", CSendUIAPITest::TestMessageDataClearAttachmentArrayL ),
		ENTRY( "MessageDataAppendToAddressL", CSendUIAPITest::TestMessageDataAppendToAddressL ),
		ENTRY( "MessageDataToAddressArray", CSendUIAPITest::TestMessageDataToAddressArrayL ),
		ENTRY( "MessageDataAppendCcAddressL", CSendUIAPITest::TestMessageDataAppendCcAddressL ),
		ENTRY( "MessageDataCcAddressArray", CSendUIAPITest::TestMessageDataCcAddressArrayL ),
		ENTRY( "MessageDataAppendBccAddressL", CSendUIAPITest::TestMessageDataAppendBccAddressL ),
		ENTRY( "MessageDataBccAddressArray", CSendUIAPITest::TestMessageDataBccAddressArrayL ),
		ENTRY( "MessageDataSetBodyTextL", CSendUIAPITest::TestMessageDataSetBodyTextL ),
		ENTRY( "MessageDataBodyText", CSendUIAPITest::TestMessageDataBodyTextL ),
		ENTRY( "MessageDataAppendAttachmentHandleL", CSendUIAPITest::TestAppendAttachmentHandleL ),
		ENTRY( "MessageDataAttachmentHandleArray", CSendUIAPITest::TestAttachmentHandleArrayL ),
		ENTRY( "MessageDataDataType", CSendUIAPITest::TestDataTypeL ),
		ENTRY( "MessageDataClearAddresses", CSendUIAPITest::ClearAddressesL ),
		ENTRY( "MessageDataExternalizeLnInternalizeL", CSendUIAPITest::TestExternalizeLnInternalizeL ),
		//CMessageAddress
		ENTRY( "MessageAddressNewL", CSendUIAPITest::TestCMessageAddressNewL ),
		ENTRY( "MessageAddressSetAddressL", CSendUIAPITest::TestMsgAdrsSetAddressL ),
		ENTRY( "MessageAddressAddress", CSendUIAPITest::TestMsgAdrsAddress ),
		ENTRY( "MessageAddressSetAliasL", CSendUIAPITest::TestMsgAdrsSetAliasL ),
		ENTRY( "MessageAddressAlias", CSendUIAPITest::TestMsgAdrsAlias ),
 
		ENTRY( "MessageAddressExternalizeL", CSendUIAPITest::TestMsgAdrsExternalizeL ),
		ENTRY( "MessageAddressInternalizeL", CSendUIAPITest::TestMsgAdrsInternalizeL ),
		ENTRY( "MessageDataSetOpaqueDataL", CSendUIAPITest::TestSetOpaqueDataL ),
		ENTRY( "MessageDataOpaqueDataL", CSendUIAPITest::TestOpaqueDataL ),
		ENTRY( "SendingServiceInfoCopyLC", CSendUIAPITest::TestCopyLC ),
		ENTRY( "SendingServiceInfoSetTechnologyTypeId", CSendUIAPITest::TestSetTechnologyTypeIdL ),

		//TSendingCapabilities
		ENTRY( "TSendingCapabilities", CSendUIAPITest::TestTSendingCapabilities ),
		//CSendingServiceInfo
		ENTRY( "SendingServiceInfoServiceID", CSendUIAPITest::TestServiceIDL ),
		ENTRY( "SendingServiceInfoServiceProviderID", CSendUIAPITest::TestServiceProviderIDL ),
        ENTRY( "SendingServiceInfoServiceCapabilities", CSendUIAPITest::TestServiceCapabilitiesL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CSendUIAPITest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CSendUIAPITest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KSendUIAPITest, "SendUIAPITest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KSendUIAPITest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KSendUIAPITest, 
                                KParam, i, &string );
        i++;
        }
    return KErrNone; 

    }

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataNewL
// ?Test Case for Calling CMessageData's NewL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt CSendUIAPITest::TestMessageDataNewL( CStifItemParser& aItem )
   {
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KNewL, "In NewL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KNewL );
	// Print to log file
	iLog->Log( KNewL);

	//Create the object of CSendUIAPITest
	CMessageData* messagedata = InitL();
	//Check the instance of CMessageData 
	if(messagedata)
		{
		return KErrNone;    
		}
	else
		{
		return KErrGeneral;
		}  
	}  
   
// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataSetSubjectL
// ?Test Case for Calling CMessageData's SetSubjectL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt CSendUIAPITest::TestMessageDataSetSubjectL( CStifItemParser& aItem )
   {
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KSetSubjectL, "In SetSubjectL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KSetSubjectL );
	// Print to log file
	iLog->Log( KSetSubjectL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	TPtrC subject;
	//Get the subject from cfg file
	aItem.GetNextString(subject);
	//Set the subject 
	TRAPD(err,messagedata->SetSubjectL(&subject));
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
// CSendUIAPITest::?TestMessageDataSubjectL
// ?Test Case for Calling CMessageData's Subject fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataSubjectL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KSubject, "In Subject" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KSubject );
	// Print to log file
	iLog->Log( KSubject);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	//Get the subject from message data
	TPtrC msgdatasubject = messagedata->Subject();

	TPtrC msgsubject;
	//Get the subject from cfg file
	aItem.GetNextString(msgsubject);
	//Compare the two subjects
	if(msgdatasubject == msgsubject)
		{
		return KErrNone;
		}	
	else
		{
		return KErrGeneral;
		}
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataAppendAttachmentL
// ?Test Case for Calling CMessageData's AppendAttachmentL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataAppendAttachmentL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KAppendAttachmentL, "In AppendAttachmentL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KAppendAttachmentL );
	// Print to log file
	iLog->Log( KAppendAttachmentL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	TPtrC filepath;
	//Get the file path from cfg file
	aItem.GetNextString(filepath);
	TDesC filepathdesc(filepath); 

	//Append the file path in message data
	TRAPD(err,messagedata->AppendAttachmentL(filepath));
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
// CSendUIAPITest::?TestMessageDataAttachmentArrayL
// ?Test Case for Calling CMessageData's AttachmentArray fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataAttachmentArrayL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KAttachmentArray, "In AttachmentArray" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KAttachmentArray );
	// Print to log file
	iLog->Log( KAttachmentArray);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	//Get the Array of attachment file paths and check the count
	TInt count =  messagedata->AttachmentArray().Count();
	if(count > 0)
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}	
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataClearAttachmentArrayL
// ?Test Case for Calling CMessageData's ClearAttachmentArray fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataClearAttachmentArrayL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KClearAttachmentArray, "In ClearAttachmentArray" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KClearAttachmentArray );
	// Print to log file
	iLog->Log( KClearAttachmentArray);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	//Clears the array of attachment file paths
	messagedata->ClearAttachmentArray();

	TInt attacharraycount;
	//Get the attachment arry count from cfg file
	aItem.GetNextInt(attacharraycount);
	//Compare the two counts
	if(attacharraycount == messagedata->AttachmentArray().Count())
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}	
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataAppendToAddressL
// ?Test Case for Calling CMessageData's AppendToAddressL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataAppendToAddressL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KAppendToAddressL, "In AppendToAddressL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KAppendToAddressL );
	// Print to log file
	iLog->Log( KAppendToAddressL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	//Get the address from cfg file to append
	TPtrC addressone;
	aItem.GetNextString(addressone);

	TPtrC aliasone;
	aItem.GetNextString(aliasone);

	//Append the address to array
	TRAPD(err, messagedata->AppendToAddressL( addressone, aliasone ));
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
// CSendUIAPITest::?TestMessageDataToAddressArrayL
// ?Test Case for Calling CMessageData's ToAddressArray fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataToAddressArrayL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KToAddressArray, "In ToAddressArray" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KToAddressArray );
	// Print to log file
	iLog->Log( KToAddressArray);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	TInt addresscount;
	aItem.GetNextInt(addresscount);

	//Get array of addresses and aliases and compare the count
	if(addresscount == messagedata->ToAddressArray().Count() )
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataAppendCcAddressL
// ?Test Case for Calling CMessageData's AppendCcAddressL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataAppendCcAddressL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KAppendCcAddressL, "In AppendCcAddressL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KAppendCcAddressL );
	// Print to log file
	iLog->Log( KAppendCcAddressL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	//Get the two ccaddresses from cfg file
	TPtrC ccaddressone;
	aItem.GetNextString(ccaddressone);

	TPtrC ccaliasone;
	aItem.GetNextString(ccaliasone);

	//Append the CC addresses in message data
	TRAPD(err , messagedata->AppendCcAddressL( ccaddressone, ccaliasone ));
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
// CSendUIAPITest::?TestMessageDataCcAddressArrayL
// ?Test Case for Calling CMessageData's CcAddressArray fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataCcAddressArrayL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KCcAddressArray, "In CcAddressArray" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KCcAddressArray );
	// Print to log file
	iLog->Log( KCcAddressArray);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	//Get the count of cc address from cfg file
	TInt ccaddresscount;
	aItem.GetNextInt(ccaddresscount);

	//Get the CC address count from message data and compare
	if(ccaddresscount == messagedata->CcAddressArray().Count())
		{
		return KErrNone;
		}
			
	else
		{
		return KErrGeneral;
		}
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataAppendBccAddressL
// ?Test Case for Calling CMessageData's AppendBccAddressL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataAppendBccAddressL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KAppendBccAddressL, "In AppendBccAddressL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KAppendBccAddressL );
	// Print to log file
	iLog->Log( KAppendBccAddressL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	//Get the two bccaddresses from cfg file
	TPtrC bccaddressone;
	aItem.GetNextString(bccaddressone);

	TPtrC bccaliasone;
	aItem.GetNextString(bccaliasone);

	//Append the  BCC addresses to message data
	TRAPD(err , messagedata->AppendBccAddressL( bccaddressone, bccaliasone ));
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
// CSendUIAPITest::?TestMessageDataBccAddressArrayL
// ?Test Case for Calling CMessageData's BccAddressArray fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataBccAddressArrayL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KBccAddressArray, "In BccAddressArray" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KBccAddressArray );
	// Print to log file
	iLog->Log( KBccAddressArray);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	//Get the appended cc address count  from cfg file 
	TInt ccaddresscount;
	aItem.GetNextInt(ccaddresscount);

	//Get the cc address count from message data and compare 
	if(ccaddresscount == messagedata->BccAddressArray().Count())
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}	   	
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataSetBodyTextL
// ?Test Case for Calling CMessageData's SetBodyTextL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataSetBodyTextL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KSetBodyTextL, "In SetBodyTextL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KSetBodyTextL );
	// Print to log file
	iLog->Log( KSetBodyTextL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	//Get the Message body from cfg file
	TPtrC messagesubject;
	aItem.GetNextString(messagesubject);

	CEikonEnv* eikonEnv = CEikonEnv::Static();
	//Create the rich text object
	CRichText* bodyText = CRichText::NewL( eikonEnv->SystemParaFormatLayerL(),
	                    eikonEnv->SystemCharFormatLayerL() );
	CleanupStack::PushL( bodyText );
	//Insert the text from cfg file to message body
	bodyText->InsertL( 0,messagesubject);

	TRAPD(err, messagedata->SetBodyTextL(bodyText));
	if(err == KErrNone)
		{
		CleanupStack::PopAndDestroy( bodyText );
		return KErrNone;
		}
	else
		{
		CleanupStack::PopAndDestroy( bodyText );
		return err;
		}		
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMessageDataBodyTextL
// ?Test Case for Calling CMessageData's BodyText fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMessageDataBodyTextL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KBodyText, "In BodyText" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KBodyText );
	// Print to log file
	iLog->Log( KBodyText);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	//Get the MessageBody text length
	TInt msgbodylen = messagedata->BodyText()->DocumentLength();
	TInt len;
	aItem.GetNextInt(len);
	//If the length passed from cfg file and and Body text length same 
	if(len == msgbodylen)
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestAppendAttachmentHandleL
// ?Test Case for Calling CMessageData's AppendAttachmentHandleL fun
// (other items were commented in a header).
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestAppendAttachmentHandleL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KAppendAttachmentHandleL, "In AppendAttachmentHandleL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KAppendAttachmentHandleL );
	// Print to log file
	iLog->Log( KAppendAttachmentHandleL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	RFs fs;
	User::LeaveIfError(fs.Connect());
	fs.ShareProtected();
	RFile file;
	//Get the attachment from cfg file
	TPtrC attachfile;
	aItem.GetNextString(attachfile);
	TDesC attachfiledesc(attachfile); 
	
	#ifdef __WINSCW__
	_LIT( KTestFilePath, "c:\\Data\\TestData\\SendUIAPITest\\%S" );
	#else
	_LIT( KTestFilePath, "e:\\testing\\data\\SendUIAPITest\\%S" );
	#endif
	
	TBuf16<KFilePathSize> fileName;
	fileName.Format( KTestFilePath, &attachfile );
	//Open the file
	TInt err = file.Open(fs, fileName, EFileRead);
	if(err != KErrNone)
		{
		_LIT( KFileError, "Unable to Open File" );
		iLog->Log(KFileError);
		fs.Close(); 
		return err;
		}
	CleanupClosePushL(file);
	//Appends attachment handle to the array of attachment handles
	TRAPD(errtwo, messagedata->AppendAttachmentHandleL(file));

	CleanupStack::PopAndDestroy( &file );
	fs.Close(); 
	if(errtwo == KErrNone)
		{
		return KErrNone;
		}
	else
		{
		return errtwo;
		}	
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestAttachmentHandleArrayL
// ?Test Case for Calling CMessageData's AttachmentHandleArray fun
// return Array of attachment handles.
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestAttachmentHandleArrayL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KAttachmentHandleArray, "In KAttachmentHandleArray" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KAttachmentHandleArray );
	// Print to log file
	iLog->Log( KAttachmentHandleArray);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	
	//Get the count of AttachmentHandle from cfg file
	TInt attachmentHandleCnt;
	aItem.GetNextInt(attachmentHandleCnt);
	
	const RArray<RFile>& attachments = messagedata->AttachmentHandleArray();
	//Compare the two counts
	if ( attachments.Count() == attachmentHandleCnt )
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?DataTypeL
// ?Test Case for Calling CMessageData's DataType fun
// return Data type id.
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestDataTypeL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KDataType, "In DataType" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KDataType );
	// Print to log file
	iLog->Log( KDataType);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	TUid dType = messagedata->DataType() ;
	if ( dType != KNullUid )
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?ClearAddressesL
// ?Test Case for Calling CMessageData's ClearAddresses fun
// Clears all addressee arrays.
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::ClearAddressesL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KClearAddresses, "In ClearAddresses" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KClearAddresses );
	// Print to log file
	iLog->Log( KClearAddresses);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	messagedata->ClearAddresses();
	if ( messagedata->ToAddressArray().Count() == 0 )
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}
	}
// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestExternalizeLnInternalizeL
// ?Test Case for Calling CMessageData's ExternalizeL and InternalizeL funs
// Externalizes and Internalize message data to and from stream
// -----------------------------------------------------------------------------
//
TInt CSendUIAPITest::TestExternalizeLnInternalizeL(CStifItemParser& aItem)
	{
	// Print to UI
	#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ )
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KTestExternalizeLnInternalizeL, "In TestExternalizeLnInternalizeL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KTestExternalizeLnInternalizeL );

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	//Get the two suject from cfg file
	TPtrC subjectone;
	aItem.GetNextString(subjectone);
	TPtrC subjecttwo;
	aItem.GetNextString(subjecttwo);

	//Set the first subject to message data
	MsgDataSetSubjectL( subjectone);
	//Externalize the message data class so that first subject is stored
	ExternalizeL();
	//Set the second subject to message data
	MsgDataSetSubjectL( subjecttwo );
	//Internalize the message data class so that stored data can be retrived
	InternalizeL();
	//Get the message-data class stored data and compare with the one stored earlier
	if(MsgDataCompSubjectL(subjectone) == KErrNone)
		{
		//Twwo datas are same 
		return KErrNone;
		}
	else
		{
		//Twwo datas differ
		return KErrGeneral;
		}	
	#endif		
	iLog->Log(_L("test case TestExternalizeLnInternalizeL not intended for 3.0 and 3.1"));
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?ExternalizeL
// ?Test Case for Calling CMessageData's ExternalizeL fun
// Externalizes message data to a stream
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::ExternalizeL(void)
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KExternalizeL, "In ExternalizeL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KExternalizeL );
	// Print to log file
	iLog->Log( KExternalizeL);

	ibuffer = CBufFlat::NewL( KMessageBufferSize );
	CleanupStack::PushL( ibuffer );
	//Create a Read Buffer Stream
	RBufWriteStream bufStream;
	//Open the Stream
	bufStream.Open( *ibuffer );
	//Externalise the MessageData object
	TInt err(KErrNone);
	#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ ) 
	TRAP(err , iMessageData->ExternalizeL(bufStream ));
	#endif
	//Comit and Close the Write Stream
	bufStream.CommitL();
	bufStream.Close();
	CleanupStack::Pop( ibuffer );
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
// CSendUIAPITest::?InternalizeL
// ?Test Case for Calling CMessageData's InternalizeL fun
// Internalizes message data to from a stream
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::InternalizeL(void)
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KInternalizeL, "In InternalizeL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KInternalizeL );
	// Print to log file
	iLog->Log( KInternalizeL);

	//Create the Read stream and Open it
	RBufReadStream bufStream;
	bufStream.Open(*ibuffer);
	//Internalise the MessageData Object
	TInt err(KErrNone);
	#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ ) 
	TRAP(err, iMessageData->InternalizeL(bufStream));
	#endif
	//Release and Close the Read Stream
	bufStream.Release();
	bufStream.Close();
	delete ibuffer;
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
// CSendUIAPITest::?TestSetOpaqueDataL
// ?Test Case for Calling CMessageData's SetOpaqueDataL fun
// Set opaque data
// -----------------------------------------------------------------------------
// 


TInt CSendUIAPITest::TestSetOpaqueDataL( CStifItemParser& aItem )
	{
#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ )
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KSetOpaqueDataL, "In SetOpaqueDataL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KSetOpaqueDataL );
	// Print to log file
	iLog->Log( KSetOpaqueDataL);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();

	TPtrC data;
	//Get the opaque data from cfg file
	aItem.GetNextString(data);
	// Copy the data to Tbuf8
	TBuf8<KMessageBufferSize> opaqueData;
	opaqueData.Copy(data);

	TUid dType = messagedata->DataType() ;
	//Set the Opaque Data to MessageData
	TRAPD( err , messagedata->SetOpaqueDataL(&opaqueData, dType ));
	if(err == KErrNone)
		{
		return KErrNone;
		}
	else
		{
		return err;
		}		
#endif		
iLog->Log(_L("test case TestSetOpaqueDataL not intended for 3.0 and 3.1"));
return KErrNone;
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestOpaqueDataL
// ?Test Case for Calling CMessageData's OpaqueData fun
// Get opaque data
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestOpaqueDataL( CStifItemParser& aItem )
	{
#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ )	
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest" );
	_LIT( KOpaqueData, "In OpaqueData" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KOpaqueData );
	// Print to log file
	iLog->Log( KOpaqueData);

	//Create the instance of CMessageData
	CMessageData* messagedata = InitL();
	TPtrC data;
	//Get the opaque data to compare from cfg file
	aItem.GetNextString(data);
	// Copy the data to Tbuf8
	TBuf8<KMessageBufferSize> opaqueData;
	opaqueData.Copy(data);
	//Get the opaque data from MessageData
	TPtrC8 ptr = messagedata->OpaqueData();
	//Compare the two values
	if(ptr.Compare(opaqueData) == 0)
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}		 
#endif
iLog->Log(_L("test case TestOpaqueDataL not intended for 3.0 and 3.1"));
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestCMessageAddressNewL
// ?Test Case for Two-phased constructor
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestCMessageAddressNewL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CMessageAddress" );
	_LIT( KCMessageAddressNewL, "In TestCMessageAddressNewL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KCMessageAddressNewL );
	// Print to log file
	iLog->Log( KCMessageAddressNewL);

	//Construct CMessageAddress object
	 iMessageAddress = CMessageAddress::NewL();
	if(iMessageAddress)
		{
		_LIT( KCMessageAddressNewL, "CMessageAddress NewL succesfulley called" );
		iLog->Log( KCMessageAddressNewL);
		return KErrNone;	
		}
	else
		{
		_LIT( KCMessageAddressNewLFail, "CMessageAddress NewL failed" );
		iLog->Log( KCMessageAddressNewLFail);
		return KErrGeneral;		
		}	
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMsgAdrsSetAddressL
// ?Test Case for API SetAddressL that Sets real address
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMsgAdrsSetAddressL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CMessageAddress" );
	_LIT( KCMessageAddressSetAddressL, "In TestCMessageAddressSetAddressL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KCMessageAddressSetAddressL );
	// Print to log file
	iLog->Log( KCMessageAddressSetAddressL);
	TPtrC address;
	//Get the address from cfg file
	aItem.GetNextString(address);
	//Set the same address  to CMessageAddress object
	TRAPD( err, iMessageAddress->SetAddressL( address ));
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
// CSendUIAPITest::?TestMsgAdrsAddress
// ?Test Case for API Address that get real address
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMsgAdrsAddress( CStifItemParser& aItem )
	{
	// Print to UI
#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ ) 	
	_LIT( KSendUIAPITest, "SendUIAPITest CMessageAddress" );
	_LIT( KCMessageAddressAddress, "In TestCMessageAddressAddress" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KCMessageAddressAddress );
	// Print to log file
	iLog->Log( KCMessageAddressAddress);

	TPtrC address;
	////Get the address from cfg file
	aItem.GetNextString(address);
	//Get the address in CMessageAddress and compare with above
	if(iMessageAddress->Address().Compare(address) == 0 )
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}
#endif
	return KErrNone;
	iLog->Log(_L("test case TestMsgAdrsAddress not intended for 3.0 and 3.1"));		
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMsgAdrsSetAliasL
// ?Test Case for API SetAliasL that Sets alias for the real address.
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMsgAdrsSetAliasL( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CMessageAddress" );
	_LIT( KCMessageAddressSetAliasL, "In TestCMessageAddressSetAliasL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KCMessageAddressSetAliasL );
	// Print to log file
	iLog->Log( KCMessageAddressSetAliasL);
	TPtrC alias;
	//Get the alias for address from cfg file
	aItem.GetNextString(alias);
	//Set the alias to CMessageAddress's address
	TRAPD( err, iMessageAddress->SetAliasL( alias ));
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
// CSendUIAPITest::?TestMsgAdrsAliasL
// ?Test Case for API Alias that returns alias for the address.
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMsgAdrsAlias( CStifItemParser& aItem )
	{
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CMessageAddress" );
	_LIT( KCMessageAddressAlias, "In TestCMessageAddressAlias" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KCMessageAddressAlias );
	// Print to log file
	iLog->Log( KCMessageAddressAlias);

	TPtrC alias;
	////Get the address from cfg file
	aItem.GetNextString(alias);
	//Get the address in CMessageAddress and compare with above
	if(iMessageAddress->Alias().Compare(alias) == 0 )
		{
		return KErrNone;
		}
	else
		{
		return KErrGeneral;
		}
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMsgAdrsExternalizeL
// ?Test Case for API ExternalizeL that Externalizes address data to a stream
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestMsgAdrsExternalizeL( CStifItemParser& aItem )
	{
#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ ) 
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CMessageAddress" );
	_LIT( KCMessageAddressExternalizeL, "In TestCMessageAddressExternalizeL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KCMessageAddressExternalizeL );
	// Print to log file
	iLog->Log( KCMessageAddressExternalizeL);

	iMsgAdrbuffer = CBufFlat::NewL( KMessageBufferSize );

	//Create a Read Buffer Stream
	RBufWriteStream bufStream;
	//Open the Stream
	bufStream.Open( *iMsgAdrbuffer );
	//Externalise the MessageData object
	TRAPD(err , iMessageAddress->ExternalizeL(bufStream ));
	//Comit and Close the Write Stream
	bufStream.CommitL();
	bufStream.Close();

	if(err == KErrNone)
		{
		return KErrNone;
		}
	else
		{
		return err;
		}
#endif
iLog->Log(_L("test case TestMsgAdrsExternalizeL not intended for 3.0 and 3.1"));
return KErrNone;
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestMsgAdrsInternalizeL
// ?Test Case for API InternalizeL that Internalizes message data from a stream
// -----------------------------------------------------------------------------
// 
 
TInt CSendUIAPITest::TestMsgAdrsInternalizeL( CStifItemParser& aItem )
	{
#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ )
	// Print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CMessageAddress" );
	_LIT( KCMessageAddressInternalizeL, "In TestCMessageAddressInternalizeL" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KCMessageAddressInternalizeL );
	// Print to log file
	iLog->Log( KCMessageAddressInternalizeL);

	RBufReadStream bufStream;
	bufStream.Open(*iMsgAdrbuffer);
	//Internalise the MessageData Object
	TRAPD(err, iMessageAddress->InternalizeL(bufStream));
	//Release and Close the Read Stream
	bufStream.Release();
	bufStream.Close();
	if(iMsgAdrbuffer)
	{
	delete iMsgAdrbuffer;
	iMsgAdrbuffer = NULL;
	}
	if(err == KErrNone)
		{
		return KErrNone;
		}
	else
		{
		return err;
		}	
#endif
iLog->Log(_L("test case TestMsgAdrsInternalizeL not intended for 3.0 and 3.1"));
return KErrNone;
	}

// -----------------------------------------------------------------------------
// CSendUIAPITest::?TestTSendingCapabilities
// ?Test Case for  functions in TSendingCapabilities  
// -----------------------------------------------------------------------------
// 

TInt CSendUIAPITest::TestTSendingCapabilities( CStifItemParser& aItem )
	{
	//print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest TSendingCapabilities" );
	_LIT( KTSendingCapabilities, "In TestTSendingCapabilities" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KTSendingCapabilities );
	// Print to log file
	iLog->Log( KTSendingCapabilities);

	_LIT( KTSendingCapabilitiesObj, "Creating TSendingCapabilities Object" );
	iLog->Log( KTSendingCapabilitiesObj);

	_LIT( KTSendingCapabilitiesCtor, "Calling TSendingCapabilities Constructor" );
	iLog->Log( KTSendingCapabilitiesCtor);

	//Create TSendingCapabilities Object
	TSendingCapabilities capabilitiesone;

	_LIT( KTSendingCapabilitiesCtord, "TSendingCapabilities Constructor called successfully" );
	iLog->Log( KTSendingCapabilitiesCtord);

	_LIT( KTSendingCapabilitiesParamCtor, "Calling TSendingCapabilities Paramaterised Constructor" );
	iLog->Log( KTSendingCapabilitiesParamCtor);

	//Create TSendingCapabilities Object with parameters
	TSendingCapabilities capabilitiestwo = TSendingCapabilities( 0, KMaxTInt,
	            TSendingCapabilities::ESupportsAttachments );
	            
	_LIT( KTSendingCapabilitiesParamCtord, "TSendingCapabilities Paramaterised Constructor called successfully" );
	iLog->Log( KTSendingCapabilitiesParamCtord);

	//Create another TSendingCapabilities Object with parameters
	TSendingCapabilities capabilitiesthree = TSendingCapabilities( 0, KMaxTInt,
	            TSendingCapabilities::ESupportsAttachments );

	_LIT( KTSendingCapabilitiesEqualOptr, "Calling TSendingCapabilities EqualTo Operator" );
	iLog->Log( KTSendingCapabilitiesEqualOptr);
	   
	//Comapre the two object with Equal to operator	                   
	if( capabilitiestwo == capabilitiesthree )
		{
		_LIT( KTSendingCapabilitiesEqual, "Two  TSendingCapabilities Objects are Equal" );
		iLog->Log( KTSendingCapabilitiesEqual);
		}
	_LIT( KTSendingCapabilitiesNotEqualOptr, "Calling TSendingCapabilities Not EqualTo Operator" );
	iLog->Log( KTSendingCapabilitiesNotEqualOptr);

	//Comapre the two object with Not Equal to operator	           
	if( capabilitiesone != capabilitiestwo )
		{
		_LIT( KTSendingCapabilitiesNotEqual, "Two  TSendingCapabilities Objects are Not Equal" );
		iLog->Log( KTSendingCapabilitiesNotEqual);
		}
	return KErrNone;	
	}
	
// -----------------------------------------------------------------------------
// CSendUIAPITest::TestCopyLC
// Test SetServiceMenuNameL,SetServiceAddressL,SetServiceNameL,SetServiceFeatures and 
// CopyLCtest method function.
// -----------------------------------------------------------------------------
//
 
TInt CSendUIAPITest::TestCopyLC( CStifItemParser& aItem )
    {
#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ )
    //print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CSendingServiceInfo" );
	_LIT( KTestCopyLC, "In TestCopyLC" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KTestCopyLC );
	// Print to log file
	iLog->Log( KTestCopyLC);

	_LIT( KNewL, "Calling CSendingServiceInfo::NewL" );
	iLog->Log( KNewL);
	//Create the CSendingServiceInfo var
	CSendingServiceInfo* serviceInfo = CSendingServiceInfo::NewL();
	
	_LIT( KSetServiceMenuNameL, "Calling CSendingServiceInfo's SetServiceMenuNameL" );
	iLog->Log( KSetServiceMenuNameL);
	serviceInfo->SetServiceMenuNameL( KTestText );
	
	_LIT( KSetServiceAddressL, "Calling CSendingServiceInfo's SetServiceAddressL" );
	iLog->Log( KSetServiceAddressL);
	serviceInfo->SetServiceAddressL( KTestText );
	
	_LIT( KSetServiceNameL, "Calling CSendingServiceInfo's SetServiceNameL" );
	iLog->Log( KSetServiceNameL);
	serviceInfo->SetServiceNameL( KTestText );
	
	_LIT( KSetServiceFeatures, "Calling CSendingServiceInfo's SetServiceFeatures" );
	iLog->Log( KSetServiceFeatures);
	serviceInfo->SetServiceFeatures( CSendingServiceInfo::EServiceInValid 
	| CSendingServiceInfo::EServiceHidden 
	| CSendingServiceInfo::EServiceCanSendDirectly );
	
	_LIT( KCopyLC, "Calling CSendingServiceInfo's CopyLC" );
	iLog->Log( KCopyLC);
	CSendingServiceInfo* copy;
	copy = serviceInfo->CopyLC();    

	//TPtrC 
	if ( serviceInfo->ServiceName().Compare( copy->ServiceName() ) != 0 ||
	    serviceInfo->ServiceMenuName().Compare( copy->ServiceMenuName() ) != 0 ||
	    serviceInfo->ServiceAddress().Compare( copy->ServiceAddress() ) != 0 ||
	    serviceInfo->ServiceFeatures() != copy->ServiceFeatures() )
		{
		iLog->Log( _L("CopyLC failed"));
		}

	CleanupStack::PopAndDestroy( copy );
	delete serviceInfo;
	return KErrNone;
#endif
iLog->Log(_L("test case TestCopyLC not intended for 3.0 and 3.1"));
	return KErrNone;
    }    
  
// -----------------------------------------------------------------------------
// CSendUIAPITest::TestSetTechnologyTypeId
// SetTechnologyTypeId test method function.
// -----------------------------------------------------------------------------
//   
 
TInt CSendUIAPITest::TestSetTechnologyTypeIdL( CStifItemParser& aItem )
	{
#if  ! defined( __SERIES60_31__ ) && ! defined( __SERIES60_30__ )	
	//print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CSendingServiceInfo" );
	_LIT( KTestSetTechnologyTypeId, "In TestSetTechnologyTypeId" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KTestSetTechnologyTypeId );
	// Print to log file
	iLog->Log( KTestSetTechnologyTypeId);
	
	//Create the CSendingServiceInfo Object
	CSendingServiceInfo* serviceInfo = CSendingServiceInfo::NewL();
	_LIT( KCSendingServiceInfoObj, "CSendingServiceInfo Object created" );
	iLog->Log( KCSendingServiceInfoObj);
	
	_LIT( KSetTechnologyTypeId, "Calling CSendingServiceInfo SetTechnologyTypeId" );
	iLog->Log( KSetTechnologyTypeId);	
	serviceInfo->SetTechnologyTypeId( KServideId );
	
	_LIT( KTechnologyTypeId, "Calling CSendingServiceInfo TechnologyTypeId" );
	iLog->Log( KTechnologyTypeId);	
	//Compare the Technology ID
	if ( serviceInfo->TechnologyTypeId() != KServideId )
	{
		delete serviceInfo;
		return KErrCancel;
	}
	else
	{
		delete serviceInfo;
		return KErrNone;
	}
#endif	
iLog->Log(_L("test case TestSetTechnologyTypeIdL not intended for 3.0 and 3.1"));
	return KErrNone;
	}
  

// -----------------------------------------------------------------------------
// CSendUIAPITest::TestServiceID
// ServiceID test method function.
// -----------------------------------------------------------------------------
//         

TInt CSendUIAPITest::TestServiceIDL( CStifItemParser& aItem )
    {
    //print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CSendingServiceInfo" );
	_LIT( KTestServiceID, "In TestServiceID" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KTestServiceID );
	// Print to log file
	iLog->Log( KTestServiceID);
	
    TUid id(KDrmLinkSender);
    //Create the CSendingServiceInfo var
    CSendingServiceInfo* serviceInfo = CSendingServiceInfo::NewL();
    
    _LIT( KSetServiceId, "Calling CSendingServiceInfo SetServiceId" );
	iLog->Log( KSetServiceId);	
    serviceInfo->SetServiceId( id );
    //Compare the ServiceID's
    if( serviceInfo->ServiceId() != id )
        {
        delete serviceInfo;
        return KErrCancel;
        }
    else
        {
        delete serviceInfo;
        return KErrNone;
        }
    }

// -----------------------------------------------------------------------------
// CSendUIAPITest::TestServiceProviderID
// ServiceProviderID test method function.
// -----------------------------------------------------------------------------
//    
    
TInt CSendUIAPITest::TestServiceProviderIDL( CStifItemParser& aItem )
    {
    //print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CSendingServiceInfo" );
	_LIT( KTestServiceProviderID, "In TestServiceProviderID" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KTestServiceProviderID );
	// Print to log file
	iLog->Log( KTestServiceProviderID);
    TUid id(KDrmLinkSender);
    //Create the CSendingServiceInfo var
    CSendingServiceInfo* serviceInfo = CSendingServiceInfo::NewL();
    
    _LIT( KSetServiceProviderId, "Calling CSendingServiceInfo SetServiceProviderId" );
	iLog->Log( KSetServiceProviderId);
    serviceInfo->SetServiceProviderId( id );
    //Compare the ServiceProviderID's
    if( serviceInfo->ServiceProviderId() != id )
        {
        delete serviceInfo;
        return KErrCancel;
        }
    else
        {
        delete serviceInfo;
        return KErrNone;
        }
     }

// -----------------------------------------------------------------------------
// CSendUIAPITest::TestServiceCapabilities
// ServiceCapabilities test method function.
// -----------------------------------------------------------------------------
//

TInt CSendUIAPITest::TestServiceCapabilitiesL( CStifItemParser& aItem )
    {
    //print to UI
	_LIT( KSendUIAPITest, "SendUIAPITest CSendingServiceInfo" );
	_LIT( KTestServiceCapabilities, "In TestServiceCapabilities" );
	TestModuleIf().Printf( 0, KSendUIAPITest, KTestServiceCapabilities );
	// Print to log file
	iLog->Log( KTestServiceCapabilities);
	//Create the Capabilities
    TSendingCapabilities cap(10,100,TSendingCapabilities::EAllServices);
    //Create the CSendingServiceInfo var
    CSendingServiceInfo* serviceInfo = CSendingServiceInfo::NewL();
    
    _LIT( KSetServiceCapabilities, "Calling CSendingServiceInfo SetServiceCapabilities" );
	iLog->Log( KSetServiceCapabilities);
    serviceInfo->SetServiceCapabilities( cap );
    //Compare the ServiceCapabilities
    if( serviceInfo->ServiceCapabilities() != cap )
        {
        delete serviceInfo;
        return KErrCancel;
        }
    else
        {
        delete serviceInfo;
        return KErrNone;
        }
    }
    
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
