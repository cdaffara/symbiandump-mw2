/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: This file contains testclass implementation.
* This file containts the test blocks that tests the encoder functionality.
*
*/

// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <LbsPosition.h>
#include <LbsFieldIds.h>
#include <BAUTILS.h>
#include <EPos_CPosLmCategoryManager.h>
#include <e32math.h> 
#include <gulutil.h>

#include "CUTLandmarkURL.h"
#include "EPos_LandmarksErrors.h"



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

//Tags used in cfg file
    _LIT(KName, "NAME= ");
    _LIT(KLandmarkDescription, "LMDESCRIPTION= ");
    _LIT(KVerticalAccuracy, "VERT_ACC= ");
    _LIT(KHorizontalAccuracy, "HACCURACY= ");
    _LIT(KLatitude, "LATITUDE= ");
    _LIT(KLongitude, "LONGITUDE= ");
    _LIT(KCountry, "COUNTRY= ");
    _LIT(KState, "STATE= ");
    _LIT(KStreet, "STREET= ");
    _LIT(KHouseNumber, "HOUSENO.= ");
    _LIT(KPostalCode, "POSTALCODE= ");
    _LIT(KCity, "CITY= ");
    _LIT(KTelephone, "TELEPHONE= ");
    _LIT(KWeb, "WEB= ");
    _LIT(KAltitude, "ALTITUDE= ");
    _LIT(KSpeed, "SPEED= ");
    _LIT(KHeading, "HEADING= ");
    _LIT(KCategory, "CATEGORY= ");
    _LIT(K2ndCategory, "CATEGORYTWO= ");
    _LIT(KPID, "PID= ");
    _LIT(KTS, "TS= ");

    _LIT(KOutputFilePath, "c:\\testframework\\EncodedLandmarks.txt");
    _LIT(KInputFile, "c:\\testframework\\URLParser.txt");
    _LIT(KInputFileHandle, "c:\\testframework\\InputFilehandle.txt");
    
    //255 
    _LIT(KMAXSTRING, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890");
    //254
    _LIT(KMAXSTRING1, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789");
    //256
    _LIT(KMAXSTRING2, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901");

    //4095
    _LIT(KMAXDESSTRING, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890");
    //4094
    _LIT(KMAXDESSTRING1, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789");
    //4096
    _LIT(KMAXDESSTRING2, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901");
    
    //124
    _LIT(KMAXCAT, "12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234");
    //123
    _LIT(KMAXCAT1, "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123");
    //125
    _LIT(KMAXCAT2, "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345");

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCUTLandmarkURL::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CreateInstanceL", CCUTLandmarkURL::CreateInstanceL ),
        ENTRY( "CleanUp", CCUTLandmarkURL::CleanUp ),
        ENTRY( "CreateLandmarkL", CCUTLandmarkURL::CreateLandmarkL ),
        
        //Test Methods for parsing
        ENTRY( "SetInputBuffer", CCUTLandmarkURL::SetInputBufferL ),
        ENTRY( "SetInputFile", CCUTLandmarkURL::SetInputFile ),
        ENTRY( "SetInputFileHandle", CCUTLandmarkURL::SetInputFileHandleL ),
        ENTRY( "ParseContent", CCUTLandmarkURL::ParseContent ),
        ENTRY( "NumOfParsedLandmarks", CCUTLandmarkURL::NumOfParsedLandmarksL ),
        ENTRY( "Landmark", CCUTLandmarkURL::LandmarkL ),
        ENTRY( "LandmarkCategory", CCUTLandmarkURL::LandmarkCategoryL ),
        ENTRY( "TestMisc", CCUTLandmarkURL::MiscL ),


        //Test methods for encoder
        ENTRY( "SetOutputFileHandle", CCUTLandmarkURL::SetOutputFileHandle ),
        ENTRY( "SetOutputBuffer", CCUTLandmarkURL::SetOutputBuffer ),        
        ENTRY( "AddLandmark", CCUTLandmarkURL::AddLandmark ),
        ENTRY( "AddCategoryForLatestLandmark", CCUTLandmarkURL::AddCategoryForLatestLandmarkL),
        ENTRY( "FinalizeEncoding", CCUTLandmarkURL::FinalizeEncoding ),
        ENTRY( "VerifyEncoding", CCUTLandmarkURL::VerifyEncodingL ), 

        


        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove

        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::CreateInstanceL
// This function creates instances for URL parser and encoder
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::CreateInstanceL( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt Case;
	RArray<CPosLandmarkEncoder> ArrayOfEncoders;
	RArray<CPosLandmarkParser> ArrayOfParsers;

    aItem.GetNextInt(Case);
    _LIT8( KUrlMIMEType, "maps.ovi.com" );
    
    switch(Case)
    	{
    	case CREATE_PARSER:
     		iLog->Log(_L("Creating Instance of parser....."));
    		 TRAP(result, iPosLandmarkParser = CPosLandmarkParser::NewL(KUrlMIMEType));
    		 if(result != KErrNone || !(iPosLandmarkParser))
    			 {
    			 iLog->Log(_L("FAILED: Creating instance for Parser fails, error = %d"),result);
    			 result = KErrGeneral;
    			 }
    		 else
    			 iLog->Log(_L("PASSED: Creating instance for Parser passes"));

    		break;
    	
    	case CREATE_ENCODER:
    		iLog->Log(_L("Creating Instance of encoder....."));
			 TRAP(result, iPosLandmarkEncoder = CPosLandmarkEncoder::NewL(KUrlMIMEType));
			 if(result != KErrNone || !(iPosLandmarkEncoder))
				 {
				 iLog->Log(_L("FAILED: Creating instance for encoder fails, error = %d"),result);
				 result = KErrGeneral;
				 }
			 else
				 iLog->Log(_L("PASSED: Creating instance for Encoder passes "));
    		break;
    		
    		
    	case MULTIPLE_ENCODER:
			 TRAPD(err, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err1, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err2, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err3, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err4, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err5, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err6, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err7, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err8, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 TRAPD(err9, ArrayOfEncoders.AppendL(*CPosLandmarkEncoder::NewL(KUrlMIMEType)));
			 if((ArrayOfEncoders.Count() != 10)||(err||err1||err2||err3||err4||err5||err6||err7||err8||err9 != KErrNone))
				 result = KErrNone;
			 break;
			 
    	case MULTIPLE_PARSER:
			 TRAPD(parseerr, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr1, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr2, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr3, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr4, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr5, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr6, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr7, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr8, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 TRAPD(parseerr9, ArrayOfParsers.AppendL(*CPosLandmarkParser::NewL(KUrlMIMEType)));
			 if((ArrayOfParsers.Count() != 10)||(parseerr||parseerr1||parseerr2||parseerr3||parseerr4||parseerr5||parseerr6||parseerr7||parseerr8||parseerr9 != KErrNone))
				 result = KErrNone;
			 break;


    	default:
    		break;
    	}
    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::CleanUp
// Releases memory and other resources
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::CleanUp( CStifItemParser& /*aItem*/ )
    {
    ReleaseLandmarkResources(); 
    if(iPosLandmark)
    	{
    	delete iPosLandmark;
    	iPosLandmark = NULL;
    	}
    
    if(iEncoderBuffer)
    	{
    	delete iEncoderBuffer;
    	iEncoderBuffer = NULL;
    	}
    
    if(iDatabase)
     	{
     	delete iDatabase;
     	iDatabase = NULL;
     	}
    
    if(iOperation)
    	{
    	delete iOperation;
    	iOperation = NULL;
    	}

    if(iPosLandmarkParser)
    	{
    	delete iPosLandmarkParser;
    	iPosLandmarkParser = NULL;
    	}
    
    if(iPosLandmarkEncoder)
    	{
    	delete iPosLandmarkEncoder;
    	iPosLandmarkEncoder = NULL;
    	}
    
    iFile.Close();
    iFileSession.Close();    
    TestModuleIf().SetBehavior( CTestModuleIf::ETestLeaksHandles );   
    User::After( 20000 );
    iLog->Log(_L("PASSED: Cleanup Succesful"));
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::CreateLandmark
// This functions creates landmarks	
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::CreateLandmarkL( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt Case = 0;
    TRAP(result, iPosLandmark = CPosLandmark::NewL());
    User::LeaveIfError(result);
    aItem.GetNextInt(Case);
   
    //Reading location values from the cfg files 
    GetStringAfterTag(aItem, KName, iName);    
    GetStringAfterTag(aItem, KLandmarkDescription, iDescription);
    aItem.GetNextString(KVerticalAccuracy,iVerticalAccuracy);
    aItem.GetNextString(KHorizontalAccuracy,iHorizontalAccuracy);
    aItem.GetNextString(KLatitude,iLatitude);
    aItem.GetNextString(KLongitude,iLongitude);
    aItem.GetNextString(KAltitude,iAltitude);

    
    //Reading postion fields from cfg fields
    aItem.GetNextString(KSpeed,iSpeed);
    aItem.GetNextString(KHeading,iHeading);
    GetStringAfterTag(aItem, KCountry,iCountry);    
    GetStringAfterTag(aItem, KState,iState);
    GetStringAfterTag(aItem, KCity,iCity);
    GetStringAfterTag(aItem, KStreet ,iStreet);
    GetStringAfterTag(aItem, KPostalCode,iPostalCode);
    GetStringAfterTag(aItem, KHouseNumber,iHouseNumber);
    GetStringAfterTag(aItem, KTelephone,iTelephone);
    aItem.GetNextString(KCategory,iCategory);
    GetStringAfterTag(aItem, KWeb,iWeb);
    GetStringAfterTag(aItem, KPID,iPID);
    GetStringAfterTag(aItem, KTS,iTS);   
    

    //Converting string to TReal
    TReal32 VA = 0;
    TLex16 VerticalAcc(iVerticalAccuracy);
    VerticalAcc.Val(VA);
    
    TReal32 HA= 0;
    TLex16 HorizontalAcc(iHorizontalAccuracy);
    HorizontalAcc.Val(HA);

	TReal64 RLat = 0;
	TLex16 Lat(iLatitude);
	Lat.Val(RLat);
    
    TReal64 RLong = 0;
    TLex16 Long(iLongitude);
    Long.Val(RLong);   

    TReal64 RAlt = 0;
    TLex16 Alt(iAltitude);
    Alt.Val(RAlt); 
    
    //This part of the code sets the position params to the landmark
    TLocality Locality;
    if (!iAltitude.Compare(_L("")))
    	Locality.SetCoordinate(RLat, RLong);
    else
    	Locality.SetCoordinate(RLat,RLong, RAlt);
    
    if (iHorizontalAccuracy.Compare(_L("")))
		Locality.SetHorizontalAccuracy(HA);
    
    if (iVerticalAccuracy.Compare(_L("")))
    	Locality.SetVerticalAccuracy(VA);
    
    TUint Cat;
    TLex16 Categ(iCategory);
    Categ.Val(Cat, EDecimal);

    //Sets the time stamp to the landmark
   TTime TS;
   TS.Set(iTS);
  
   if (iTS.Compare(_L("")))
	   {
	   TRAP(result ,iPosLandmark->SetTimeStampL(TS));
	   User::LeaveIfError(result);
	   }
   
    switch(Case)
    	{
    	//This case tests the normal condition for all the landmark param fields
    	default:
    		//fall through
    	case 0:    	    

    	   if (iDescription.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetLandmarkDescriptionL(iDescription));
    		   User::LeaveIfError(result);
    		   }
    	   if (iName.Compare(_L("")))
    		   {
    		   TRAP(result ,iPosLandmark->SetLandmarkNameL(iName));
    		   User::LeaveIfError(result);
    		   }
    	       	    
    	    //Setting positions fields
    	   if (iSpeed.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldHorizontalSpeed, iSpeed));
    		   User::LeaveIfError(result);
    		   }
    	   if (iHeading.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldHeading, iHeading));
    		   User::LeaveIfError(result);
    		   }
    	   if (iCountry.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCountry, iCountry));
    		   User::LeaveIfError(result);
    		   }
    	   if (iState.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldState, iState));
    		   User::LeaveIfError(result);
    		   }
    	   if (iCity.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCity, iCity));
    		   User::LeaveIfError(result);
    		   }
    	   if (iStreet.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreet, iStreet));
    		   User::LeaveIfError(result);
    		   }
    	   if (iPostalCode.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldPostalCode, iPostalCode));
    		   User::LeaveIfError(result);
    		   }
    	   if (iHouseNumber.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreetExtension, iHouseNumber));
    		   User::LeaveIfError(result);
    		   }
    	   if (iTelephone.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, iTelephone));
    		   User::LeaveIfError(result);
    		   }
    	   if (iWeb.Compare(_L("")))
    		   {
    		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart, iWeb));
    		   User::LeaveIfError(result);
    		   }
    	   if (iPID.Compare(_L("")))
    		   {
    		   TRAP(result ,iPosLandmark->SetPlaceIdL(iPID));
    		   User::LeaveIfError(result);
    		   }
    		break;
    		
    	//The following cases sets the location params to test the boundry and error cases	
    	case TEST_NULL_NAME:
    		TRAP(result ,iPosLandmark->SetLandmarkNameL(iName));
    		User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_STREET:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreet, iStreet));
 		   User::LeaveIfError(result);
 		   break;
    		
    	case TEST_NULL_HNO:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreetExtension, iHouseNumber));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_POSTALCODE:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldPostalCode, iPostalCode));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_CITY:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCity, iCity));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_STATE:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldState, iState));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_COUNTRY:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCountry, iCountry));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_TELEPHONE:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, iTelephone));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_WEB:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart, iWeb));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_DESCRIPTION:
 		   TRAP(result, iPosLandmark->SetLandmarkDescriptionL(iDescription));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_SPEED:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldHorizontalSpeed, iSpeed));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_HEADING:
 		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldHeading, iHeading));
 		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_NULL_PID:
 		   TRAP(result ,iPosLandmark->SetPlaceIdL(iPID));
 		   User::LeaveIfError(result);
 		   break;
    		
    	case TEST_MAXLENGTH_NAME:
    		TRAP(result ,iPosLandmark->SetLandmarkNameL(KMAXSTRING));
    		User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_NAME:
    		TRAP(result ,iPosLandmark->SetLandmarkNameL(KMAXSTRING1));
    		User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_NAME:
    		TRAP(result ,iPosLandmark->SetLandmarkNameL(KMAXSTRING2));
    		if(result != KErrArgument)
    			User::Leave(result);
    		else
				result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_STREET:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreet, KMAXSTRING));
  		   User::LeaveIfError(result);    		
    		break;
    		
    	case TEST_MAXLENGTH1_STREET:
   		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreet, KMAXSTRING1));
   		   User::LeaveIfError(result);  
    		break;
    	
    	case TEST_MAXLENGTH2_STREET:
   		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreet, KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
			break;
    		
    	case TEST_MAXLENGTH_HNO:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreetExtension, KMAXSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_HNO:
   		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreetExtension, KMAXSTRING1));
   		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_HNO:
   		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldStreetExtension, KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_POSTALCODE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldPostalCode, KMAXSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_POSTALCODE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldPostalCode, KMAXSTRING1));
   		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_POSTALCODE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldPostalCode, KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_CITY:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCity, KMAXSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_CITY:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCity, KMAXSTRING1));
  		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_CITY:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCity, KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_STATE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldState, KMAXSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_STATE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldState, KMAXSTRING1));
  		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_STATE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldState, KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_COUNTRY:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCountry, KMAXSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_COUNTRY:
   		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCountry, KMAXSTRING1));
   		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_COUNTRY:
   		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldCountry, KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_TELEPHONE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, KMAXSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_TELEPHONE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, KMAXSTRING1));
  		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_TELEPHONE:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldBuildingTelephone, KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_WEB:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart, KMAXSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_WEB:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart, KMAXSTRING1));
  		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_WEB:
  		   TRAP(result, iPosLandmark->SetPositionFieldL(EPositionFieldMediaLinksStart, KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_DESCRIPTION:
  		   TRAP(result, iPosLandmark->SetLandmarkDescriptionL(KMAXDESSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_DESCRIPTION:
  		   TRAP(result, iPosLandmark->SetLandmarkDescriptionL(KMAXDESSTRING1));
  		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_DESCRIPTION:
  		   TRAP(result, iPosLandmark->SetLandmarkDescriptionL(KMAXDESSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAXLENGTH_PID:
 		   TRAP(result ,iPosLandmark->SetPlaceIdL(KMAXSTRING));
  		   User::LeaveIfError(result);
    		break;
    		
    	case TEST_MAXLENGTH1_PID:
 		   TRAP(result ,iPosLandmark->SetPlaceIdL(KMAXSTRING1));
  		   User::LeaveIfError(result);
    		break;
    	
    	case TEST_MAXLENGTH2_PID:
 		   TRAP(result ,iPosLandmark->SetPlaceIdL(KMAXSTRING2));
			if(result != KErrArgument)
				User::Leave(result);
			else
					result = KErrNone;
    		break;
    		
    	case TEST_MAX_HA:
    		Locality.SetHorizontalAccuracy(KMaxTReal32);
    		break;
    	
    	case TEST_MAX_VA:
    		Locality.SetVerticalAccuracy(KMaxTReal32-1);
    		break;
    	}
    
    //Adding Global Category
    iDatabase = CPosLandmarkDatabase::OpenL();
    if (iDatabase->IsInitializingNeeded())
       {
       ExecuteAndDeleteLD(iDatabase->InitializeL());
       }

    if (iCategory.Compare(_L("")))
    	{
		CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
		CleanupStack::PushL(categoryManager);
		TUint16 id = categoryManager->GetGlobalCategoryL(Cat);
		CPosLandmarkCategory* landmarkCategory = categoryManager->ReadCategoryLC(id);
		TRAP(result, iPosLandmark->AddCategoryL(id));
		CleanupStack::PopAndDestroy(landmarkCategory);
	    CleanupStack::PopAndDestroy(categoryManager);
	    User::LeaveIfError(result);
    	}

    TRAP(result, iPosLandmark->SetPositionL(Locality));
    if(result!= KErrNone)
    	{
    	result = KErrGeneral;
    	iLog->Log(_L("FAILED: Creating Landmark fails!!"));
    	}
    else
    	iLog->Log(_L("PASSED: Creating Landmark Success!!"));
        return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::SetOutputFileHandle
// This function sets the output file to which the encoded URLs will be written
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::SetOutputFileHandle( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt caseno = 0;  
    aItem.GetNextInt(result);
    
    switch(caseno)
    	{
    	default:
    		//fall through
    	case 0:
			if ( BaflUtils::FileExists(iFileSession, KOutputFilePath))
				{
				BaflUtils :: DeleteFile( iFileSession, KOutputFilePath );
				}
			TRAP(result,iPosLandmarkEncoder->SetOutputFileL(KOutputFilePath));
			if(result != KErrNone)
				{
				iLog->Log(_L("FAILED: Setting output file fails with error %d"), result);
				result = KErrGeneral;
				}
			else 
				iLog->Log(_L("PASSED: Setting output file passes"));
			break;
			
    	case ERROR_VERIFICATION_OPFILE:
			if ( BaflUtils::FileExists(iFileSession, KOutputFilePath))
				{
				BaflUtils ::DeleteFile( iFileSession, KOutputFilePath );
				}
			TRAP(result,iPosLandmarkEncoder->SetOutputFileL(KOutputFilePath));
			TRAPD(err,iPosLandmarkEncoder->SetOutputFileL(KOutputFilePath));

			if(err != KErrAlreadyExists)
				{
				iLog->Log(_L("FAILED: Error verification for setting output file fails with %d"), err);
				result = KErrGeneral;
				}
			else 
				iLog->Log(_L("PASSED: Error Verification for setting output file passed!!"));

    		break;
    	}
	
    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::SetOutputFileHandle
// This function sets the output buffer to which the encoded URLs will be written
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::SetOutputBuffer( CStifItemParser& /*aItem*/ )
    {
    TInt result = KErrNone;

    TRAP(result, iEncoderBuffer = iPosLandmarkEncoder->SetUseOutputBufferL());
    if(result != KErrNone)
    	iLog->Log(_L("FAILED: Setting Output buffer fails with error %d"), result);
    	
    else
    	iLog->Log(_L("PASSED: Setting Output buffer passed"));

    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::AddLandmark
// This function adds the landmark to be encoded
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::AddLandmark( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt caseno = 0;
    aItem.GetNextInt(caseno);
    
    switch(caseno)
    	{
    	default:
    		//fall through
    	case 0:
			TRAP(result, iPosLandmarkEncoder->AddLandmarkL(* iPosLandmark));
			if(result != KErrNone)
				iLog->Log(_L("FAILED: Adding Landmark for encoding fails with error %d"), result);
				
			else
				iLog->Log(_L("PASSED: Adding Landmark for encoding passed"));
			break;
		
    	case PANIC_CODE_VERIIFCATION:    
    		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, EPosLmProtocolBreak );   
			TRAP(result, iPosLandmarkEncoder->AddLandmarkL(* iPosLandmark));
			break;
    	}
			
    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::AddCategoryForLatestLandmark
// This function adds category to the latest landmark
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::AddCategoryForLatestLandmarkL( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt Category;
    TInt caseno = 0;

    aItem.GetNextInt(Category);
    aItem.GetNextInt(caseno);
    
	iDatabase = CPosLandmarkDatabase::OpenL();
	if (iDatabase->IsInitializingNeeded())
	   {
	   ExecuteAndDeleteLD(iDatabase->InitializeL());
	   }

	CPosLmCategoryManager* categoryManager = CPosLmCategoryManager::NewL(*iDatabase);
	CleanupStack::PushL(categoryManager);
	TUint16 id = categoryManager->GetGlobalCategoryL(Category);
	CPosLandmarkCategory* landmarkCategory = categoryManager->ReadCategoryLC(id);
    
    switch(caseno)
    	{
    	default:
    		//fall through
    	case 0:				
			TRAP(result, iPosLandmarkEncoder->AddCategoryForLatestLandmarkL(* landmarkCategory));
			if(result != KErrNone)
				iLog->Log(_L("FAILED: Adding Category for latest landmark fails with error %d"), result);

			else
				iLog->Log(_L("PASSED: Adding Category for latest landmark passed"));
			break;
			
    	case PANIC_CODE_VERIIFCATION:
    		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, EPosLmProtocolBreak );   
			TRAP(result, iPosLandmarkEncoder->AddCategoryForLatestLandmarkL(* landmarkCategory));
    		break;
    	}
	CleanupStack::PopAndDestroy(landmarkCategory);
	CleanupStack::PopAndDestroy(categoryManager);
    return result;
    }
// -----------------------------------------------------------------------------
// CCUTLandmarkURL::FinalizeEncoding
// This function finalizes the encoding
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::FinalizeEncoding( CStifItemParser& /*aItem*/ )
    {
    TInt result = KErrNone;
    TRAP(result, iOperation = iPosLandmarkEncoder->FinalizeEncodingL());
    TRAPD(result1, iOperation->ExecuteL());
    if(result != KErrNone||result1 != KErrNone)
    	iLog->Log(_L("FAILED: Finalize Encoding fails with error %d"), result);

    else
    	iLog->Log(_L("PASSED: Encoding Completed Successfully"));

    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::SetInputBuffer
// This function sets the input buffer containing the URL to be parsed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::SetInputBufferL( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt caseno = 0;
    TPtrC inputBuffer = _L("");
    aItem.GetNextInt(caseno);
    aItem.GetNextString(inputBuffer);
    
    HBufC *buffer = HBufC::NewLC(inputBuffer.Size());
    *buffer = inputBuffer;

    HBufC8 *urlBuffer = HBufC8::NewL((inputBuffer.Size() + 25));
    
    switch(caseno)
    	{
    	default:
    		//fall through
    	case 0:
			urlBuffer->Des().Copy(_L8("http://www.maps.ovi.com/?"));
			break;
			
    	case NOPROTOCOL:
			urlBuffer->Des().Copy(_L8("maps.ovi.com/?"));
			break;
			
    	case HTTPS:
			urlBuffer->Des().Copy(_L8("https://maps.ovi.com/?"));
			break;
			
    	case INVALIDPROTOCOL:
			urlBuffer->Des().Copy(_L8("ftp://maps.ovi.com/?"));
			break;
			
    	case NOHOSTNAME:
			urlBuffer->Des().Copy(_L8("/?"));
			break;
			
    	case INVALIDHOSTNAME:
			urlBuffer->Des().Copy(_L8("http://www.maps.google.com/?"));
			break;
			
    	case INVALIDHOSTNAME1:
			urlBuffer->Des().Copy(_L8("http://www.maps.ovi.com?/"));
			break;
			
    	case INVALIDHOSTNAME2:
			urlBuffer->Des().Copy(_L8("http://www.maps.ovi.com/"));
			break;
    	}
    
	urlBuffer->Des().Append(*buffer);
	iPosLandmarkParser->SetInputBuffer(* urlBuffer);
	
    CleanupStack::PopAndDestroy(buffer);
    return result;    
    }
// -----------------------------------------------------------------------------
// CCUTLandmarkURL::SetInputFile
// This function sets the input file containing the URL to be parsed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::SetInputFile( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt caseno = 0;    
    aItem.GetNextInt(caseno);
    
    if(BaflUtils::FileExists(iFileSession, KInputFile))
    	{
    	TRAP(result,iPosLandmarkParser->SetInputFileL(KInputFile));
        if(result != KErrNone)
        	iLog->Log(_L("FAILED: Setting Input File Fails with error, %d"), result);

        else
        	iLog->Log(_L("PASSED: Setting Input File passes"));
    	}
    else
    	result = KErrNotFound;
    
    if(caseno == DELETEFILE)
    	{
    	result = BaflUtils::DeleteFile(iFileSession, KInputFile);
    	if (result != KErrInUse)
    		iLog->Log(_L("FAILED: Input File Deleted after Setting"));
    	else
    		{
    		iLog->Log(_L("PASSED: Input File cannot be deleted after Setting"));
    		result = KErrNone;
    		}
    	}  
    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::SetInputFileHandle
// This function sets the handle of file containing the URL to be parsed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::SetInputFileHandleL( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    _LIT(KTestMode, "TESTMODE= " );
    if(BaflUtils::FileExists(iFileSession, KInputFileHandle))
    	{
    	BaflUtils::DeleteFile(iFileSession, KInputFileHandle);
    	}
    TInt caseno = 0;
    aItem.GetNextInt(caseno);
  
    TPtrC inputURL;
    aItem.GetNextString(inputURL);
    
    TInt testmode = 0;
    aItem.GetInt(KTestMode, testmode);
    
    
    HBufC *url = HBufC::NewLC(inputURL.Size());
    *url = inputURL;

    HBufC8 *urlfile = HBufC8::NewLC(inputURL.Size());
    urlfile->Des().Copy(*url);
    
    User::LeaveIfError(iFile.Create(iFileSession,KInputFileHandle, EFileWrite));
    switch(caseno)
    	{
    	default:
    		//fall through
    	case 0:
    	    User::LeaveIfError(iFile.Write(0, _L8("http://www.maps.ovi.com/?")));
			break;
			
    	case NOPROTOCOL:
    	    User::LeaveIfError(iFile.Write(0, _L8("maps.ovi.com/?")));
			break;
			
    	case HTTPS:
    	    User::LeaveIfError(iFile.Write(0, _L8("https://maps.ovi.com/?")));
			break;
			
    	case INVALIDPROTOCOL:
    	    User::LeaveIfError(iFile.Write(0, _L8("ftp://maps.ovi.com/?")));
			break;
			
    	case NOHOSTNAME:
    	    User::LeaveIfError(iFile.Write(0, _L8("/?")));
			break;
			
    	case INVALIDHOSTNAME:
    	    User::LeaveIfError(iFile.Write(0, _L8("http://www.maps.google.com/?")));
			break;
			
    	case INVALIDHOSTNAME1:
    	    User::LeaveIfError(iFile.Write(0, _L8("http://www.maps.ovi.com?/")));
			break;
			
    	case INVALIDHOSTNAME2:
    	    User::LeaveIfError(iFile.Write(0, _L8("http://www.maps.ovi.com/")));
			break;
			
    	case DESC_BV1:
    		User::LeaveIfError(iFile.Write(0, _L8("http://www.maps.ovi.com/?lo=-179.99999&la=-89.99999&d=123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678906789012345678901234567890123456789012345678901234567890")));
    		break;
    		
    	case DESC_BV2:
    		User::LeaveIfError(iFile.Write(0, _L8("http://www.maps.ovi.com/?lo=-179.99999&la=-89.99999&d=12345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890678901234567890123456789012345678901234567890123456789")));
    		break;
    		
    	case DESC_BV3:
    		User::LeaveIfError(iFile.Write(0, _L8("http://www.maps.ovi.com/?lo=-179.99999&la=-89.99999&d=1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789067890123456789012345678901234567890123456789012345678901")));
    		break;
    		
    	}
    
    User::LeaveIfError(iFile.Write(25, *urlfile));  
    
    CleanupStack::PopAndDestroy(urlfile);
    CleanupStack::PopAndDestroy(url);
    
    //Closing the file and opening in ReadMode
    TInt expectedResult = KErrNone; 
    if(testmode == KRead)
    	{
		iFile.Close();
		User::LeaveIfError(iFile.Open(iFileSession, KInputFileHandle, EFileRead));
		expectedResult = KErrNone;
    	}
    if(testmode == KReadWrite)
    	expectedResult = KErrAccessDenied;
    	
    
	TRAP(result,iPosLandmarkParser->SetInputFileHandleL(iFile));
		{
		if(result != expectedResult)
			{
			iLog->Log(_L("FAILED: Setting Input File Handle Fails with error, %d"), result);
			result = KErrGeneral;
			}
		else
			iLog->Log(_L("PASSED: Setting Input File Handle passes"));
		}
    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::ParseContent
// This function calls the parser method to parse the URL set either in buffer or file
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::ParseContent( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt caseno = 0;
    aItem.GetNextInt(caseno);
    TReal32 progress;
    TRequestStatus status;
    
    switch(caseno)
    	{
    	default:
    		//fall through
    	case 0:
    	    TRAP(result, iOperation = iPosLandmarkParser->ParseContentL(EFalse));
    	    TRAPD(error, iOperation->NextStep(status, progress));
    	    User::WaitForRequest(status);
    	    if(result != KErrNone || error != KErrNone && status != KPosLmOperationNotComplete)
    			{
    			iLog->Log(_L("FAILED: ParseContent Fails with Error: %d and %d"), result, error);
    			result = KErrGeneral;
    			}
    	    else
    			iLog->Log(_L("PASSED: ParseContent passes"));
    	    break;
    	    
		case UNKNOWNFORMAT:
			TRAP(result, iOperation = iPosLandmarkParser->ParseContentL(EFalse));
			TRAP(result, iOperation->ExecuteL());
			if(result != KErrPosLmUnknownFormat && status == KErrNone)
				{
				iLog->Log(_L("FAILED:Parse Content fails for unknown format with error, %d"), result);
				result = KErrGeneral;
				}
			else
				{
				iLog->Log(_L("PASSED: Parse Content passes for unknown format"));
				result = KErrNone;
				}
		break;
		
    	case PANIC_CODE_VERIIFCATION:
    		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, EPosLmProtocolBreak );   
			TRAP(result, iPosLandmarkParser->ParseContentL(EFalse));
    		break;
    	}
    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::NumOfParsedLandmarks
// This function gives the number of parsed landmarks
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::NumOfParsedLandmarksL( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt value = 0;
    User::LeaveIfError(aItem.GetNextInt(value));
    
    if(iPosLandmarkParser->NumOfParsedLandmarks() != value)
    	{
    	iLog->Log(_L("FAILED: Number of parsed Landmark does not matches"));
    	result = KErrGeneral;
    	}
    
    else
    	iLog->Log(_L("PASSED: NumOfParsedLandmarks returns the correct value"));

    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::Landmark 
// This function returns the landmark that is parsed from the URL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::LandmarkL( CStifItemParser& aItem )
    {
    TInt result = KErrNone;
    TInt caseno = 0;
    
    aItem.GetNextInt(caseno);
    
    CPosLandmark* landmark = NULL;
    
    switch(caseno)
    	{
    	default:
    		//fall through
    	case 0:
    		   landmark = iPosLandmarkParser->LandmarkLC();
    			TRAP(result, VerifyParsingL(aItem, landmark));
    			if(result != KErrNone)
    				{
    				iLog->Log(_L("FAILED: Verification of Parsing Failed with error %d"), result);
    				result = KErrGeneral;
    				}
    			else
    				iLog->Log(_L("PASSED: Verification of parsing passed"));
    			
    			
    		    iDatabase = CPosLandmarkDatabase::OpenL();    
    		    if (iDatabase->IsInitializingNeeded())
    		       {
    		       ExecuteAndDeleteLD(iDatabase->InitializeL());
    		       }
    		    iPosLandmark = CPosLandmark::NewL(*landmark);
    		    CleanupStack::PopAndDestroy(landmark);
    		    
    		break;
    		
    	case NOLANDMARK:
    		TRAP(result, landmark = iPosLandmarkParser->LandmarkLC(); CleanupStack::PopAndDestroy(landmark));
    		if(result != KErrNotFound)
    			{
    			iLog->Log(_L("FAILED:LandmarkLC fails for no landmark parsed, %d"), result);
    			result = KErrGeneral;
    			}
    		else
    			{
    			iLog->Log(_L("PASSED: LandmarkLC passes no landmark parsed"));
    			result = KErrNone;
    			}
    		break;
    		
    	case PANIC_CODE_VERIIFCATION:
    		TestModuleIf().SetExitReason( CTestModuleIf::EPanic, EPosInvalidIndex );   
			TRAP(result, iPosLandmarkParser->LandmarkLC(4); CleanupStack::PopAndDestroy(landmark));
    		break;	
    	}
 
    return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::LandmarkCategory
// This function returns the category of the landmark that was most recently parsed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::LandmarkCategoryL( CStifItemParser& aItem )
    {
    TInt result = KErrGeneral;
    TInt caseno = 0;
    
    aItem.GetNextInt(caseno);
    
    GetStringAfterTag(aItem, KCategory,iMatchCategory);
    GetStringAfterTag(aItem, K2ndCategory,iMatchCategory2);
    
    RArray<TPosLmItemId> array;
    CPosLandmark* landmark = NULL;
    landmark = iPosLandmarkParser->LandmarkLC();
	TRAP(result, landmark->GetCategoriesL(array));
	switch(caseno)
		{
		default:
			//fall through
		case 0:
		    for (TInt i=0;i<array.Count();i++)
		        {
		        TBuf<150> buf;
		        CPosLandmarkCategory* category = iPosLandmarkParser->LandmarkCategoryLC(array[i]);

		        if (category->GlobalCategory()) buf.Append(_L("Global category: "));
		        else buf.Append(_L("Local category: "));
		        TPtrC catName;
		        User::LeaveIfError(category->GetCategoryName(catName));
		        result = (catName.Compare(iMatchCategory));
		        if (result == KErrNone)
		        	goto CONTINUE;
		        result = (catName.Compare(iMatchCategory2));
		        if (result == KErrNone)
		        	goto CONTINUE;
				result = (catName.Compare(KMAXCAT));
		        if (result == KErrNone)
		        	goto CONTINUE;
				result = (catName.Compare(KMAXCAT1));
		        if (result == KErrNone)
		        	goto CONTINUE;
				result = (catName.Compare(KMAXCAT2));
		        if (result == KErrNone)
		        	goto CONTINUE;
		        CONTINUE:
					{
					buf.Append(catName);
					iLog->Log(buf);
					CleanupStack::PopAndDestroy(category);
					if (result != KErrNone)
						break;
					}
		        }
		    break;
		    
		case NULLCAT:
			if(array.Count()== 0)
				result = KErrNone;
		
		}
	if(result == KErrNone)
		iLog->Log(_L("PASSED: LandmarkLC Passed!!"));
	else
		iLog->Log(_L("FAILED: LandmarkLC fails!!"));		
    CleanupStack::PopAndDestroy(landmark);
    return result;
    }
// -----------------------------------------------------------------------------
// CCUTLandmarkURL::Misc
// This method verifies the encoded url as follows:
// 1. Complies with the URL grammer
// 2. Compares the parameters are same as they were set
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCUTLandmarkURL::MiscL( CStifItemParser& /*aItem*/)
    {
    TInt result = KErrNone;
    TPosLmCollectionDataId collectionDataId;
    
    //Testing CPosLandmarkParser::CollectionData()
    if( iPosLandmarkParser->CollectionData(EPosLmCollDataNone)!= KNullDesC)
    	{
    	iLog->Log(_L("FAILED: CollectionData fails"));
    	User::Leave(KErrGeneral);
    	}
    else
    	iLog->Log(_L("PASSED: Collection Data passed"));
    
    //Testing CPosLandmarkParser::FirstCollectionDataId()
    collectionDataId= iPosLandmarkParser->FirstCollectionDataId();
    if(collectionDataId != EPosLmCollDataNone)
    	{
    	iLog->Log(_L("FAILED: FirstCollectionDataId fails"));
    	User::Leave(KErrGeneral);
    	}
    else
    	iLog->Log(_L("PASSED: FirstCollectionDataId Data passed"));
    
    //Testing CPosLandmarkParser::NextCollectionDataId()
    collectionDataId = iPosLandmarkParser->NextCollectionDataId(EPosLmCollDataNone);
    if(collectionDataId != EPosLmCollDataNone)
    	{
    	iLog->Log(_L("FAILED: NextCollectionDataId fails"));
    	User::Leave(KErrGeneral);
    	}
    else
    	iLog->Log(_L("PASSED: NextCollectionDataId Data passed"));
    TBuf<16> collectionData;
    TRAP(result,iPosLandmarkEncoder->AddCollectionDataL(EPosLmCollDataNone,collectionData));
	User::Leave(result);
    }
// -----------------------------------------------------------------------------
// CCUTLandmarkURL::VerifyEncoding
// This method verifies the encoded url as follows:
// 1. Complies with the URL grammer
// 2. Compares the parameters are same as they were set
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::VerifyEncodingL( CStifItemParser& aItem)
    {
    TInt result = KErrNone;
    TInt caseno = 0;
    TInt NumberofParams = 0;
    aItem.GetNextInt(caseno);
    aItem.GetNextInt(NumberofParams);

    HBufC8* aDes = NULL;
    HBufC8 *BufferedURL = HBufC8::NewLC(KMaxTestBuf); 
	TInt size = 0;
 
     switch(caseno)
    	{
    	default:
    	case BUFFER_OUTPUT:
    		TRAP(result, aDes = HBufC8::NewL(iEncoderBuffer->Size()));
    		CleanupStack::PushL(aDes);
     	    TPtr8 ptr = aDes->Des();
    	    iEncoderBuffer->Read(0, ptr, iEncoderBuffer->Size());
    	    result = ParseURL(_L8("?"),ptr ,aItem);
    	    iEncoderBuffer->Reset();
    	    CleanupStack::PopAndDestroy(aDes);
    		break;
    		
    	case FILE_OUTPUT:
    		User::LeaveIfError(iFile.Open(iFileSession, KOutputFilePath,EFileRead));
    		TPtr8 fileptr = BufferedURL->Des();
    		User::LeaveIfError(iFile.Read(fileptr)); 
    	    result = ParseURL(_L8("?"),fileptr,aItem);
    		break;
    		
    	case NO_ENCODING_FILE:
    	  	User::LeaveIfError(iFile.Open(iFileSession, KOutputFilePath,EFileRead));
    		User::LeaveIfError(iFile.Size(size));
    		iLog->Log(_L("Size of the file is %d"), size);
    		if (size != -1)
    			User::Leave(KErrGeneral);
    		else
    			User::Leave(KErrNone);    			
    		break;
    		
    	case NO_ENCODING_BUFFER:
    		size = iEncoderBuffer->Size();
    		if (size != -1)
    			User::Leave(KErrGeneral);
    		else
    			User::Leave(KErrNone);    			
    		break;    		
    	}
	iLog->Log(_L("PASSED: All the location params values matches!!"));
    if(iCount != (NumberofParams))
    	{
    	result = KErrGeneral;
    	iLog->Log(_L("FAILED: number of location params encoded does not match with the cfg file!!"));
    	}
    CleanupStack::PopAndDestroy(BufferedURL);
    return result;
    }
// -----------------------------------------------------------------------------
// CCUTLandmarkURL::ParseURL
// This method parses the URL obtained after encoding the landmark
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::ParseURL(const TDesC8 & aDelimiter, TPtr8 aBufferedURL, CStifItemParser& aItem)
    {
    TInt result = KErrNone;
    TInt ParamStart = 0;
    TInt column = 0;
    TPtrC parsedURL;
    HBufC * bufferedURL = HBufC::NewLC(KMaxTestBuf);
    HBufC * remainingURL = HBufC::NewLC(KMaxTestBuf);
    ParamStart = aBufferedURL.Find(aDelimiter);
    if(ParamStart != -1)
    	{
    	remainingURL->Des().Copy(aBufferedURL.Mid((ParamStart+1),aBufferedURL.Length() -(ParamStart+1) ));
    	bufferedURL->Des().Copy(remainingURL->Des());
    	while(result != KErrNotFound)
    		{
    		result = TextUtils::ColumnText(parsedURL, column, bufferedURL, KColumnSeperator);
    		HBufC8 *parsedURL8 = HBufC8::NewLC(parsedURL.Length());
    		parsedURL8->Des().Copy(parsedURL);
    		ParseValueL(parsedURL8->Des(), aItem);
    		column++;
    		CleanupStack::PopAndDestroy(parsedURL8);
    		}
    	}
    CleanupStack::PopAndDestroy(2);
	return KErrNone;   
	}

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::ParseValue
// This method obtains the value of the location parameters in the URL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt CCUTLandmarkURL::ParseValueL(TPtr8 aBufferedURL, CStifItemParser& aItem)
    {
	TInt result = KErrNone;
	TInt paramStart = 0;
	paramStart = aBufferedURL.Find(_L8("="));
	TBuf8<KMaxTagName> tagName;
	HBufC8* temp = HBufC8::NewLC(KMaxTestBuf);
	TPtr8 temp8 = temp->Des();
	if(paramStart != -1)
		{
		tagName .Copy(aBufferedURL.Mid(0,paramStart )) ;
		temp8.Copy(aBufferedURL .Mid((paramStart+1),aBufferedURL .Length() -(paramStart+1) ));
		temp8.TrimAll();
		TPtrC8 Value(temp8);
		result = CompareParamValueL(tagName, Value, aItem);
		}
	CleanupStack::PopAndDestroy(temp);
	return result;
    }

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::CompareParamValue
// This method compares the values of the parameters in the URL and in the cfg file
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::CompareParamValueL(TBuf8<KMaxTagName> aTagName , TPtrC8 aValue, CStifItemParser& aItem)
	{
	TInt result = KErrGeneral;
	TInt tagCompare = -1;

	HBufC *value = HBufC::NewLC(aValue.Length());
	value->Des().Copy(aValue);
	aTagName.TrimAll();
	
   //Reading location values from the cfg files 
    GetStringAfterTag(aItem, KName, iName);    
    GetStringAfterTag(aItem, KLandmarkDescription, iDescription);
    aItem.GetNextString(KVerticalAccuracy,iVerticalAccuracy);
    aItem.GetNextString(KHorizontalAccuracy,iHorizontalAccuracy);
    aItem.GetNextString(KLatitude,iLatitude);
    aItem.GetNextString(KLongitude,iLongitude);
    aItem.GetNextString(KAltitude,iAltitude);
    
    
    //Reading postion fields from cfg fields
    aItem.GetNextString(KSpeed,iSpeed);
    aItem.GetNextString(KHeading,iHeading);
    GetStringAfterTag(aItem, KCountry,iCountry);    
    GetStringAfterTag(aItem, KState,iState);
    GetStringAfterTag(aItem, KCity,iCity);
    GetStringAfterTag(aItem, KStreet ,iStreet);
    GetStringAfterTag(aItem, KPostalCode,iPostalCode);
    GetStringAfterTag(aItem, KHouseNumber,iHouseNumber);
    GetStringAfterTag(aItem, KTelephone,iTelephone);
    GetStringAfterTag(aItem, KCategory,iMatchCategory);
    GetStringAfterTag(aItem, KWeb,iWeb);
    GetStringAfterTag(aItem, K2ndCategory,iMatchCategory2);
    GetStringAfterTag(aItem, KPID,iPID);
    GetStringAfterTag(aItem, KTS,iTS);


	
    tagCompare = aTagName.Compare(_L8("la"));
	if(tagCompare == 0)
		result = value->Compare(iLatitude);
	
	tagCompare = aTagName.Compare(_L8("lo"));
	if(tagCompare == 0)
		result = value->Compare(iLongitude);
	
	tagCompare = aTagName.Compare(_L8("n"));
	if(tagCompare == 0)
		result = value->Compare(iName);
	
	tagCompare = aTagName.Compare(_L8("s"));
	if(tagCompare == 0)
		result = value->Compare(iStreet);
	
	tagCompare = aTagName.Compare(_L8("sn"));
	if(tagCompare == 0)
		result = value->Compare(iHouseNumber);
	
	tagCompare = aTagName.Compare(_L8("pz"));
	if(tagCompare == 0)
		result = value->Compare(iPostalCode);
	
	tagCompare = aTagName.Compare(_L8("c"));
	if(tagCompare == 0)
		result = value->Compare(iCity);
	
	tagCompare = aTagName.Compare(_L8("sp"));
	if(tagCompare == 0)
		result = value->Compare(iState);
	
	tagCompare = aTagName.Compare(_L8("cr"));
	if(tagCompare == 0)
		result = value->Compare(iCountry);
	
	tagCompare = aTagName.Compare(_L8("t"));
	if(tagCompare == 0)
		result = value->Compare(iTelephone);
	
	tagCompare = aTagName.Compare(_L8("w"));
	if(tagCompare == 0)
		result = value->Compare(iWeb);
	
	tagCompare = aTagName.Compare(_L8("cat"));
	if(tagCompare == 0)
		result = value->Compare(iMatchCategory);
	if (result != KErrNone)
		result = value->Compare(iMatchCategory2);
	
	tagCompare = aTagName.Compare(_L8("d"));
	if(tagCompare == 0)
		result = value->Compare(iDescription);
	
	tagCompare = aTagName.Compare(_L8("pa"));
	if(tagCompare == 0)
		result = value->Compare(iHorizontalAccuracy);
	
	tagCompare = aTagName.Compare(_L8("a"));
	if(tagCompare == 0)
		result = value->Compare(iAltitude);
	
	tagCompare = aTagName.Compare(_L8("aa"));
	if(tagCompare == 0)
		result = value->Compare(iVerticalAccuracy);
	
	tagCompare = aTagName.Compare(_L8("v"));
	if(tagCompare == 0)
		result = value->Compare(iSpeed);
	
	tagCompare = aTagName.Compare(_L8("h"));
	if(tagCompare == 0)
		result = value->Compare(iHeading);
	
	tagCompare = aTagName.Compare(_L8("pid"));
	if(tagCompare == 0)
		result = value->Compare(iPID);
	
	tagCompare = aTagName.Compare(_L8("ts"));
	if(tagCompare == 0)
		result = value->Compare(iTS);
	
	if (result != 0)
		{
		result = value->Compare(KMAXSTRING);
		}
	if (result != 0)
		{
		result = value->Compare(KMAXSTRING1);
		}
	if (result != 0)
		{
		result = value->Compare(KMAXSTRING2);
		}
	if (result != 0)
		{
		result = value->Compare(KMAXDESSTRING);
		}
	if (result != 0)
		{
		result = value->Compare(KMAXDESSTRING1);
		}
	if (result != 0)
		{
		result = value->Compare(KMAXDESSTRING2);
		}
	
	
	iCount++;
	if(result != KErrNone)
		{
		iLog->Log(_L("FAILED: Params Values in the URL and cfg file does not match, Param: %S"),&aTagName);
		User::Leave(KErrGeneral);
		}
	CleanupStack::PopAndDestroy(value);
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CCUTLandmarkURL::CompareParamValue
// This method compares the values of the parameters in the URL and in the cfg file
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CCUTLandmarkURL::GetStringTillQuote(CStifItemParser& aItem, TName& aName)
    {
     TPtrC temp;
     

     //Assuming that a " has already been seen.
     if(aItem.GetNextString(temp) != KErrNone)
        {
         return KErrArgument;
        }
     
     //The name should have atleast one word.
     if(temp == _L("\""))
        {
         return KErrArgument;
        }

     
     //Append the first word.
     aName.Append(temp);
    
     if(aItem.GetNextString(temp) != KErrNone)
        {
         return KErrArgument;
        }


     while(temp!=_L("\""))
        {
         //Append a space at the beginning
         aName.Append(_L(" "));
         aName.Append(temp);

         //Keep on appending the next words.
         if(aItem.GetNextString(temp) != KErrNone)
            {
             return KErrArgument;
            }
        }

     return KErrNone;
     }


// -----------------------------------------------------------------------------
// CCUTLandmarkURL::CompareParamValue
// This method compares the values of the parameters in the URL and in the cfg file
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//

TInt CCUTLandmarkURL::GetStringAfterTag(CStifItemParser& aItem, const TDesC& aTag, TName& aString)
    {
     TPtrC temp;
     aString.Delete(0,aString.Length());

     if(aItem.GetString(aTag, temp) != KErrNone)
        {
         return KErrArgument;
        }

     //Now temp should contain a "
     if(temp != _L("\""))
        {
         return KErrArgument;
        }

     //Get the Country name till a " is found.
     if(GetStringTillQuote(aItem, aString) != KErrNone)
        {
         return KErrArgument;
        }

     return KErrNone;
    }
// -----------------------------------------------------------------------------
// CCUTLandmarkURL::VerifyParsing
// This method verifies the parsed url as follows:
// 1. Compares with the URL grammer
// 2. Compares the parameters are set properly in the landmark object
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CCUTLandmarkURL::VerifyParsingL( CStifItemParser& aItem,CPosLandmark* aPosLandmark )
    {	
    TInt result = KErrGeneral;
        
    //Reading location values from the cfg files 
    GetStringAfterTag(aItem, KName, iName);    
    GetStringAfterTag(aItem, KLandmarkDescription, iDescription);
    aItem.GetNextString(KVerticalAccuracy,iVerticalAccuracy);
    aItem.GetNextString(KHorizontalAccuracy,iHorizontalAccuracy);
    aItem.GetNextString(KLatitude,iLatitude);
    aItem.GetNextString(KLongitude,iLongitude);
    aItem.GetNextString(KAltitude,iAltitude);
    
    
    //Reading postion fields from cfg fields
    aItem.GetNextString(KSpeed,iSpeed);
    aItem.GetNextString(KHeading,iHeading);
    
    GetStringAfterTag(aItem, KCountry,iCountry); 
    GetStringAfterTag(aItem, KState,iState);
    GetStringAfterTag(aItem, KCity,iCity);
    GetStringAfterTag(aItem, KStreet ,iStreet);
    GetStringAfterTag(aItem, KPostalCode,iPostalCode);
    GetStringAfterTag(aItem, KHouseNumber,iHouseNumber);
    GetStringAfterTag(aItem, KTelephone,iTelephone);
    GetStringAfterTag(aItem, KWeb,iWeb);
    GetStringAfterTag(aItem, KPID,iPID);
    GetStringAfterTag(aItem, KTS,iTS);
    
    
   
    //Converting string to TReal
    TRealX floatX;
    floatX.SetNaN();
    
    TReal32 VA = 0;
    if(iVerticalAccuracy != _L(""))
    	{
		TLex16 VerticalAcc(iVerticalAccuracy);
		VerticalAcc.Val(VA);
    	}
    else 
    	VA = floatX;
    	    
    TReal32 HA= 0;
    if(iHorizontalAccuracy != _L(""))
    	{
    	TLex16 HorizontalAcc(iHorizontalAccuracy);
    	HorizontalAcc.Val(HA);
    	}
    else
    	HA = floatX;

	TReal64 RLat = 0;
	TLex16 Lat(iLatitude);
	Lat.Val(RLat);
    
    TReal64 RLong = 0;
    TLex16 Long(iLongitude);
    Long.Val(RLong);   

    TReal64 RAlt = 0;
    if(iAltitude != _L(""))
    	{
    	TLex16 Alt(iAltitude);
    	Alt.Val(RAlt);
    	}
    else
    	RAlt = floatX;

   
    TPtrC landmarkDescription;
    aPosLandmark->GetLandmarkDescription(landmarkDescription);
	result = landmarkDescription.Compare(iDescription);
	if(result != KErrNone)
		result = landmarkDescription.Compare(KMAXDESSTRING);
	if(result != KErrNone)
		result = landmarkDescription.Compare(KMAXDESSTRING1);
	if(result != KErrNone)
		result = landmarkDescription.Compare(KMAXDESSTRING2);
	
	TPtrC name;
	aPosLandmark->GetLandmarkName(name);
	result = name.Compare(iName);
	if(result != KErrNone)
		result = name.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = name.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = name.Compare(KMAXSTRING2);
	
	TPtrC pid;
	aPosLandmark->GetPlaceId(pid);
	result = pid.Compare(iPID);
	if(result != KErrNone)
		result = pid.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = pid.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = pid.Compare(KMAXSTRING2);
		
	TLocality position;
	TReal64 latitude, longitude = 0;
	TReal32 altitude, pa, aa = 0;
	aPosLandmark->GetPosition(position);
	latitude = position.Latitude();
	if(latitude != RLat)
		User::Leave(KErrGeneral);
	longitude = position.Longitude();
	if(longitude != RLong)
		User::Leave(KErrGeneral);
	altitude = position.Altitude();
	if(Math::IsNaN(altitude)!= Math::IsNaN(RAlt) )
		{
		if(altitude != RAlt)
			User::Leave(KErrGeneral);
		}
	
	pa = position.HorizontalAccuracy();
	if(Math::IsNaN(pa)!= Math::IsNaN(HA) )
		{
		if(pa != HA)
			User::Leave(KErrGeneral);
		}
	
	aa = position.VerticalAccuracy();
	if(Math::IsNaN(aa)!= Math::IsNaN(VA) )
		{
		if(aa != VA)
			User::Leave(KErrGeneral);
		}
	
	TPtrC speed;
	aPosLandmark->GetPositionField(EPositionFieldHorizontalSpeed, speed);
	result = speed.Compare(iSpeed);
	if(result != KErrNone)
		result = speed.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = speed.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = speed.Compare(KMAXSTRING2);
		
	
	
	TPtrC heading;
	aPosLandmark->GetPositionField(EPositionFieldHeading, heading);
	result = heading.Compare(iHeading);
	if(result != KErrNone)
		result = heading.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = heading.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = heading.Compare(KMAXSTRING2);
	
	TPtrC country;
	aPosLandmark->GetPositionField(EPositionFieldCountry, country);
	result = country.Compare(iCountry);
	if(result != KErrNone)
		result = country.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = country.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = country.Compare(KMAXSTRING2);
	
	TPtrC state;
	aPosLandmark->GetPositionField(EPositionFieldState, state);
	result = state.Compare(iState);
	if(result != KErrNone)
		result = state.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = state.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = state.Compare(KMAXSTRING2);
	
	TPtrC city;
	aPosLandmark->GetPositionField(EPositionFieldCity, city);
	result = city.Compare(iCity);
	if(result != KErrNone)
		result = city.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = city.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = city.Compare(KMAXSTRING2);
	
	
	TPtrC street;
	aPosLandmark->GetPositionField(EPositionFieldStreet, street);
	result = street.Compare(iStreet);
	if(result != KErrNone)
		result = street.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = street.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = street.Compare(KMAXSTRING2);
	
	TPtrC postalCode;
	aPosLandmark->GetPositionField(EPositionFieldPostalCode, postalCode);
	result = postalCode.Compare(iPostalCode);
	if(result != KErrNone)
		result = postalCode.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = postalCode.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = postalCode.Compare(KMAXSTRING2);
	
	TPtrC houseNumber;
	aPosLandmark->GetPositionField(EPositionFieldStreetExtension, houseNumber);
	result = houseNumber.Compare(iHouseNumber);
	if(result != KErrNone)
		result = houseNumber.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = houseNumber.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = houseNumber.Compare(KMAXSTRING2);
	
	TPtrC telephone;
	aPosLandmark->GetPositionField(EPositionFieldBuildingTelephone, telephone);
	result = telephone.Compare(iTelephone);
	if(result != KErrNone)
		result = telephone.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = telephone.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = telephone.Compare(KMAXSTRING2);
	
	TPtrC web;
	aPosLandmark->GetPositionField(EPositionFieldMediaLinksStart, web);
	result = web.Compare(iWeb);
	if(result != KErrNone)
		result = web.Compare(KMAXSTRING);
	if(result != KErrNone)
		result = web.Compare(KMAXSTRING1);
	if(result != KErrNone)
		result = web.Compare(KMAXSTRING2);
	
	User::LeaveIfError(result);
	
	// This part of the code Tests the timestamp
	TTime timeStamp;
	result = aPosLandmark->GetTimeStamp(timeStamp);
	TTime ts;
	if(iTS != _L(""))
		ts.Set(iTS);
	if(result != KErrNotFound && iTS != _L(""))
		{
		if(timeStamp != ts)
			User::Leave(KErrGeneral);
		}
	}

    
// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
