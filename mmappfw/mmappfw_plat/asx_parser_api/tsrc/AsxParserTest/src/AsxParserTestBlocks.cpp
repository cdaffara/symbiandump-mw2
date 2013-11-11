/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  AsxParserTestBlocks.cpp
*
*/

// Version : %version: 3.1.2 %




// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "AsxParserTest.h"
#include "AsxParser.h"

#include <coeaui.h>
#include "EdwinTestControl.h"
#include <EIKENV.H>

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
// CAsxParserTest::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CAsxParserTest::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CAsxParserTest::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "Example", CAsxParserTest::ExampleL ),
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
        // test cases for the header "AsxParser.h"
        ENTRY( "CreateNew",            CAsxParserTest::CreateNew ),
        ENTRY( "CreateNewRFile",       CAsxParserTest::CreateNewRFile ),
        ENTRY( "GetUrlCount",          CAsxParserTest::GetUrlCount ),
        ENTRY( "GetUrl",               CAsxParserTest::GetUrl ),
        ENTRY( "FileVersion",          CAsxParserTest::FileVersion ),
    	ENTRY( "PrintUrl",             CAsxParserTest::PrintUrl ),
    	ENTRY( "GetUrlCountRFile",     CAsxParserTest::GetUrlCountRFile ),
        ENTRY( "GetUrlRFile",          CAsxParserTest::GetUrlRFile ),
        ENTRY( "FileVersionRFile",     CAsxParserTest::FileVersionRFile ),
    	ENTRY( "PrintUrlRFile",        CAsxParserTest::PrintUrlRFile ),
    	ENTRY( "CAsxParserDestructor", CAsxParserTest::CAsxParserDestructor),
        ENTRY( "GetAsxStruct",         CAsxParserTest::GetAsxStruct )

    	  
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CAsxParserTest::ExampleL
// Example test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::ExampleL( CStifItemParser& aItem )
    {

    // Print to UI
    _LIT( KAsxParserTest, "AsxParserTest" );
    _LIT( KExample, "In Example" );
    TestModuleIf().Printf( 0, KAsxParserTest, KExample );
    // Print to log file
    iLog->Log( KExample );

    TInt i = 0;
    TPtrC string;
    _LIT( KParam, "Param[%i]: %S" );
    while ( aItem.GetNextString ( string ) == KErrNone )
        {
        TestModuleIf().Printf( i, KAsxParserTest, 
                                KParam, i, &string );
        i++;
        }

    return KErrNone;

    }

//
// -----------------------------------------------------------------------------
// CAsxParserTest::CreateNew
// GetUrlCount method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::CreateNew( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::CreateNew"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
           {
			TBuf<120> KFrom;
			KFrom.Append(KAsxParserTest_testPath);
			KFrom.Append(string);		
		   	TRAP(err,iAsxParser=CAsxParser::NewL(KFrom));
		   	if ( err == KErrNone )
		   	{
		   		iLog->Log(_L("CreateNew returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: CreateNew returned: %d"), err);
		   	}
           }
    return err;
	}

//
// -----------------------------------------------------------------------------
// CAsxParserTest::CreateNewRFile
// GetUrl method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::CreateNewRFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::CreateNewRFile"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
          {
   			TBuf<120> KFrom;
   			KFrom.Append(KAsxParserTest_testPath);
   			KFrom.Append(string);
   			
   			RFile file;
   			RFs fs;
   			TRAP(err, fs.Connect());
   		    file.Open(fs, KFrom, EFileRead | EFileShareReadersOnly );       
            TRAP(err, iAsxParser=CAsxParser::NewL(file));
            fs.Close();
   			
		   	if ( err == KErrNone )
			{
		   		iLog->Log(_L("CreateNewRFile returned no error %d"), err);
			}
		   	else 
			{
		   		iLog->Log(_L("Error: CreateNewRFile returned: %d"), err);
			}
		  
          }
   	 return err;
    }

//
// -----------------------------------------------------------------------------
// CAsxParserTest::GetUrlCount
// GetUrlCount method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::GetUrlCount( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::GetUrlCount"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
           {
			TBuf<120> KFrom;
			KFrom.Append(KAsxParserTest_testPath);
			KFrom.Append(string);
			TRAP(err,iAsxParser=CAsxParser::NewL(KFrom));
		    TUint aHowManyUrls;
		    
		    if ( err == KErrNone )
		    {
		        iAsxParser->GetUrlCount(aHowManyUrls);
		    }
		   	
		    if ( err == KErrNone )
		   	{
		   		iLog->Log(_L("GetUrlCount returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: GetUrlCount returned: %d"), err);
		   	}
           }
    return err;
	}

//
// -----------------------------------------------------------------------------
// CAsxParserTest::GetUrl
// GetUrl method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::GetUrl( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::GetUrl"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
          {
   			TBuf<120> KFrom;
   			KFrom.Append(KAsxParserTest_testPath);
   			KFrom.Append(string);
   			TRAP(err,iAsxParser=CAsxParser::NewL(KFrom));
   		    
   			if ( err == KErrNone )
   		    {
                TUint aHowManyUrls = 1;
                TPtrC8 aUrl;
                err = iAsxParser->GetUrl(aHowManyUrls,aUrl);
   		    }
   			
		   	if ( err == KErrNone )
		   	{
		   	   iLog->Log(_L("GetUrl returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: GetUrl returned: %d"), err);
		   	}
          }
   	 return err;
    }

//
// -----------------------------------------------------------------------------
// CAsxParserTest::FileVersion
// FileVersion method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::FileVersion( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::FileVersion"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
          {
			TBuf<120> KFrom;
			KFrom.Append(KAsxParserTest_testPath);
			KFrom.Append(string);
			TRAP(err,iAsxParser=CAsxParser::NewL(KFrom));

			if ( err == KErrNone )
			{
			    iAsxParser->FileVersion();
			}
			
			if ( err == KErrNone )
		   	{
		   		
		   		iLog->Log(_L("FileVersion returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: FileVersion returned: %d"), err);
		   	}
          }
    return err;
    }

//
// -----------------------------------------------------------------------------
// CAsxParserTest::PrintUrl
// GetUrl method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::PrintUrl( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::PrintUrl"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
          {
   			TBuf<120> KFrom;
   			KFrom.Append(KAsxParserTest_testPath);
   			KFrom.Append(string);
   			TRAP(err,iAsxParser=CAsxParser::NewL(KFrom));
   			
   			if ( err == KErrNone )
   			{
       		    TPtrC8 aUrl;
       		    HBufC16* urlBuffer = NULL;
       		    TUint aHowManyUrls = 1;
       		    
       		    err =iAsxParser->GetUrl(aHowManyUrls,aUrl);
    	    	
       		    if ( err == KErrNone )
       		    {
       		        TRAP(err, urlBuffer = HBufC16::NewL(100));
           		    TPtr16   url = urlBuffer->Des();
        		    iAsxParser->PrintUrl(aUrl, url);
       		    }
   			}
   		    	
		   	if ( err == KErrNone )
		   	{
		   		iLog->Log(_L("PrintUrl returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: PrintUrl returned: %d"), err);
		   	}
          }
   	 return err;
    }

//
// -----------------------------------------------------------------------------
// CAsxParserTest::GetUrlCountRFile
// GetUrlCount method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::GetUrlCountRFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::GetUrlCountRFile"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
           {
			TBuf<120> KFrom;
			KFrom.Append(KAsxParserTest_testPath);
			KFrom.Append(string);
			
			RFile file;
            RFs fs;
            User::LeaveIfError(fs.Connect());
            file.Open(fs, KFrom, EFileRead | EFileShareReadersOnly );       
            TRAP(err, iAsxParser=CAsxParser::NewL(file));
            fs.Close();
			      
            if ( err == KErrNone )
            {
		        TUint aHowManyUrls;
		   	    iAsxParser->GetUrlCount(aHowManyUrls);
            }
            
		   	if ( err == KErrNone )
		   	{
		   		iLog->Log(_L("GetUrlCountRFile returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: GetUrlCountRFile returned: %d"), err);
		   	}
           }
    return err;
	}

//
// -----------------------------------------------------------------------------
// CAsxParserTest::GetUrlRFile
// GetUrl method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::GetUrlRFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::GetUrlRFile"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
          {
   			TBuf<120> KFrom;
   			KFrom.Append(KAsxParserTest_testPath);
   			KFrom.Append(string);
   			
   			RFile file;
            RFs fs;
            User::LeaveIfError(fs.Connect());
            file.Open(fs, KFrom, EFileRead | EFileShareReadersOnly );       
            TRAP(err, iAsxParser=CAsxParser::NewL(file));
            fs.Close();
   		    
            if ( err == KErrNone )
            {
       		    TUint aHowManyUrls = 1;
    		    TPtrC8 aUrl;
    		    iAsxParser->GetUrl(aHowManyUrls,aUrl);
            }
            
		   	if ( err == KErrNone )
		   	{
		   		iLog->Log(_L("GetUrlRFile returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: GetUrlRFile returned: %d"), err);
		   	}
          }
   	 return err;
    }

//
// -----------------------------------------------------------------------------
// CAsxParserTest::FileVersionRFile
// FileVersion method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::FileVersionRFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::FileVersionRFile"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
          {
			TBuf<120> KFrom;
			KFrom.Append(KAsxParserTest_testPath);
			KFrom.Append(string);
			
			RFile file;
            RFs fs;
            User::LeaveIfError(fs.Connect());
            file.Open(fs, KFrom, EFileRead | EFileShareReadersOnly );       
            TRAP(err, iAsxParser=CAsxParser::NewL(file));
            fs.Close();
			
            if ( err == KErrNone )
            {
		        iAsxParser->FileVersion();
            }
            
		   	if ( err == KErrNone )
		   	{
		   		iLog->Log(_L("FileVersionRFile returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: FileVersionRFile returned: %d"), err);
		   	}
          }
    return err;
    }

//
// -----------------------------------------------------------------------------
// CAsxParserTest::PrintUrlRFile
// GetUrl method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::PrintUrlRFile( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::PrintUrlRFile"));
    TInt err = KErrNone;
    TPtrC string;
    while ( aItem.GetNextString( string ) == KErrNone )
          {
   			TBuf<120> KFrom;
   			KFrom.Append(KAsxParserTest_testPath);
   			KFrom.Append(string);
   			
   			RFile file;
            RFs fs;
            User::LeaveIfError(fs.Connect());
            file.Open(fs, KFrom, EFileRead | EFileShareReadersOnly );       
            TRAP(err, iAsxParser=CAsxParser::NewL(file));
            fs.Close();
   			  
            if ( err == KErrNone )
            {
       		    TPtrC8 aUrl;
       		    HBufC16* urlBuffer = NULL;
       		    TUint aHowManyUrls = 1;
       		    err =iAsxParser->GetUrl(aHowManyUrls,aUrl);
    	    	
       		    if ( err == KErrNone )
       		    {
           		    TRAP(err, urlBuffer = HBufC16::NewL(100));
           		    TPtr16   url = urlBuffer->Des();
        		    iAsxParser->PrintUrl(aUrl, url);
       		    }
            }
   		    	
		   	if ( err == KErrNone )
		   	{
		   		iLog->Log(_L("PrintUrlRFile returned no error"));
		   	}
		   	else 
		   	{
		   		iLog->Log(_L("Error: PrintUrlRFile returned: %d"), err);
		   	}
          }
   	 return err;
    }

// -----------------------------------------------------------------------------
//  CAsxParserTest::CAsxParserDestructor
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CAsxParserTest::CAsxParserDestructor(CStifItemParser& /*aItem*/)
    {
    TInt err = KErrNone;
    delete iAsxParser;
    iAsxParser = NULL;
	iLog->Log(_L("CAsxParserTest::CAsxParserDestructor testing CAsxParser::~ end err=%d"), err);
	return KErrNone;
    }


//
// -----------------------------------------------------------------------------
// CAsxParserTest::GetUrl
// GetUrl method function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAsxParserTest::GetAsxStruct( CStifItemParser& aItem )
    {
    iLog->Log(_L("CAsxParserTest::GetAsxStruct"));
    TInt err = KErrNone;
    TPtrC string;
    AsxStruct* asxStruct = NULL;
    
    while ( aItem.GetNextString( string ) == KErrNone )
          {
            TBuf<120> KFrom;
            KFrom.Append(KAsxParserTest_testPath);
            KFrom.Append(string);
            TRAP(err,iAsxParser=CAsxParser::NewL(KFrom));
            
            if ( err == KErrNone )
            {
                TUint aHowManyUrls = 1;
                TPtrC8 aUrl;
                asxStruct = iAsxParser->GetUrl(aHowManyUrls);
            }
            
            if ( asxStruct )
            {
               iLog->Log(_L("GetUrl returned no error"));
            }
            else 
            {
                err = KErrGeneral;
                iLog->Log(_L("Error: GetUrl returned: %d"), err);
            }
          }
     return err;
    }

// -----------------------------------------------------------------------------
// CAsxParserTest::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CAsxParserTest::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
