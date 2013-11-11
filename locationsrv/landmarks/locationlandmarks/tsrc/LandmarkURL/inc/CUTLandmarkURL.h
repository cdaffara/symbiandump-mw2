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
* Description: STIF testclass declaration
*
*/

#ifndef CUTLANDMARKURL_H
#define CUTLANDMARKURL_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>

#include <EPos_CPosLandmark.h>
#include <EPos_CPosLandmarkEncoder.h>
#include <EPos_CPosLandmarkParser.h>
#include <EPos_CPosLandmarkDatabase.h>


// CONSTANTS
//const ?type ?constant_var = ?constant;
const TInt KMaxTestBuf= 10240;
const TInt KMaxTagName= 24;
const TInt CREATE_PARSER = 0;
const TInt CREATE_ENCODER =1;
const TInt MULTIPLE_ENCODER =2;
const TInt MULTIPLE_PARSER =3;
const TInt BUFFER_OUTPUT =0;
const TInt FILE_OUTPUT =1;
const TInt NO_ENCODING_FILE =2;
const TInt NO_ENCODING_BUFFER =3;
const TInt ERROR_VERIFICATION_OPFILE =1;
const TInt PANIC_CODE_VERIIFCATION =1;
const TInt TEST_NULL_NAME = 1;
const TInt TEST_NULL_STREET = 2;
const TInt TEST_NULL_HNO = 3;
const TInt TEST_NULL_POSTALCODE = 4;
const TInt TEST_NULL_CITY = 5;
const TInt TEST_NULL_STATE = 6;
const TInt TEST_NULL_COUNTRY = 7;
const TInt TEST_NULL_TELEPHONE = 8;
const TInt TEST_NULL_WEB = 9;
const TInt TEST_NULL_DESCRIPTION = 10;
const TInt TEST_NULL_SPEED = 41;
const TInt TEST_NULL_HEADING = 42;
const TInt TEST_NULL_PID = 43;
const TInt TEST_MAXLENGTH_NAME = 11;
const TInt TEST_MAXLENGTH1_NAME = 12;
const TInt TEST_MAXLENGTH2_NAME = 13;
const TInt TEST_MAXLENGTH_STREET = 14;
const TInt TEST_MAXLENGTH1_STREET = 15;
const TInt TEST_MAXLENGTH2_STREET = 16;
const TInt TEST_MAXLENGTH_HNO = 17;
const TInt TEST_MAXLENGTH1_HNO = 18;
const TInt TEST_MAXLENGTH2_HNO = 19;
const TInt TEST_MAXLENGTH_POSTALCODE = 20;
const TInt TEST_MAXLENGTH1_POSTALCODE = 21;
const TInt TEST_MAXLENGTH2_POSTALCODE = 22;
const TInt TEST_MAXLENGTH_CITY = 23;
const TInt TEST_MAXLENGTH1_CITY = 24;
const TInt TEST_MAXLENGTH2_CITY = 25;
const TInt TEST_MAXLENGTH_STATE = 26;
const TInt TEST_MAXLENGTH1_STATE = 27;
const TInt TEST_MAXLENGTH2_STATE = 28;
const TInt TEST_MAXLENGTH_COUNTRY = 29;
const TInt TEST_MAXLENGTH1_COUNTRY = 30;
const TInt TEST_MAXLENGTH2_COUNTRY = 31;
const TInt TEST_MAXLENGTH_TELEPHONE = 32;
const TInt TEST_MAXLENGTH1_TELEPHONE = 33;
const TInt TEST_MAXLENGTH2_TELEPHONE = 34;
const TInt TEST_MAXLENGTH_WEB = 35;
const TInt TEST_MAXLENGTH1_WEB = 36;
const TInt TEST_MAXLENGTH2_WEB = 37;
const TInt TEST_MAXLENGTH_DESCRIPTION = 38;
const TInt TEST_MAXLENGTH1_DESCRIPTION = 39;
const TInt TEST_MAXLENGTH2_DESCRIPTION = 40;
const TInt TEST_MAXLENGTH_PID = 44;
const TInt TEST_MAXLENGTH1_PID = 45;
const TInt TEST_MAXLENGTH2_PID = 46;
const TInt TEST_MAX_HA = 47;
const TInt TEST_MAX_VA = 48;

//constants for testing the Parser
const TInt NOPROTOCOL= 1;
const TInt HTTPS= 2;
const TInt INVALIDPROTOCOL= 3;
const TInt NOHOSTNAME= 4;
const TInt INVALIDHOSTNAME= 5;
const TInt INVALIDHOSTNAME1= 6;
const TInt INVALIDHOSTNAME2= 7;
const TInt DESC_BV1= 8;
const TInt DESC_BV2= 9;
const TInt DESC_BV3= 10;
const TInt UNKNOWNFORMAT= 2;
const TInt NOLANDMARK= 2;
const TInt NULLCAT= 1;
const TInt DELETEFILE= 1;
const TInt KRead= 0;
const TInt KReadWrite= 1;
const TUint KColumnSeperator='&';




// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

// Logging path
_LIT( KCUTLandmarkURLLogPath, "\\logs\\testframework\\CUTLandmarkURL\\" ); 
// Log file
_LIT( KCUTLandmarkURLLogFile, "CUTLandmarkURL.txt" ); 
_LIT( KCUTLandmarkURLLogFileWithTitle, "CUTLandmarkURL_[%S].txt" );

// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CCUTLandmarkURL;

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CCUTLandmarkURL test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CCUTLandmarkURL) : public CScriptBase
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CCUTLandmarkURL* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CCUTLandmarkURL();

    public: // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    public: // Functions from base classes

        /**
        * From CScriptBase Runs a script line.
        * @since ?Series60_version
        * @param aItem Script line containing method name and parameters
        * @return Symbian OS error code
        */
        virtual TInt RunMethodL( CStifItemParser& aItem );

    protected:  // New functions

        /**
        * ?member_description.
        * @since ?Series60_version
        * @param ?arg1 ?description
        * @return ?description
        */
        //?type ?member_function( ?type ?arg1 );

    protected:  // Functions from base classes

        /**
        * From ?base_class ?member_description
        */
        //?type ?member_function();

    private:

        /**
        * C++ default constructor.
        */
        CCUTLandmarkURL( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        // Prohibit copy constructor if not deriving from CBase.
        // ?classname( const ?classname& );
        // Prohibit assigment operator if not deriving from CBase.
        // ?classname& operator=( const ?classname& );

        /**
        * Frees all resources allocated from test methods.
        * @since ?Series60_version
        */
        void Delete();

        /**
        * Test methods are listed below. 
        */

        /**
        * Example test method.
        * @since ?Series60_version
        * @param aItem Script line containing parameters.
        * @return Symbian OS error code.
        */
       
        TInt CreateInstanceL(CStifItemParser& aItem);
        TInt CleanUp (CStifItemParser& aItem);
        TInt CreateLandmarkL (CStifItemParser& aItem); 
        
        //Test methods for parser        
        TInt SetInputBufferL(CStifItemParser& aItem);
        TInt SetInputFile(CStifItemParser& aItem);
        TInt SetInputFileHandleL(CStifItemParser& aItem);
        TInt ParseContent(CStifItemParser& aItem);
        TInt NumOfParsedLandmarksL(CStifItemParser& aItem);
        TInt LandmarkL(CStifItemParser& aItem);
        TInt LandmarkCategoryL(CStifItemParser& aItem);
        void MiscL(CStifItemParser& aItem);

        
        //Test methods for encoder        
        TInt SetOutputFileHandle (CStifItemParser& aItem);
        TInt SetOutputBuffer (CStifItemParser& aItem);
        TInt AddLandmark (CStifItemParser& aItem);
        TInt AddCategoryForLatestLandmarkL (CStifItemParser& aItem);
        TInt FinalizeEncoding (CStifItemParser& aItem);
        TInt VerifyEncodingL (CStifItemParser& aItem);

        //Helper methods
        void VerifyParsingL(CStifItemParser& aItem, CPosLandmark* aPosLandmark);
        TInt ParseURL(const TDesC8 & aDelimiter, TPtr8 aBufferedURL, CStifItemParser& aItem);
        TInt ParseValueL(TPtr8 aBufferedURL, CStifItemParser& aItem);
        TInt CompareParamValueL(TBuf8<KMaxTagName> aTagName , TPtrC8 aValue, CStifItemParser& aItem);
        TInt GetStringTillQuote(CStifItemParser& aItem, TName& aName);
        TInt GetStringAfterTag(CStifItemParser& aItem, const TDesC& aTag, TName& aString);


        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
    	
    	CPosLandmarkParser *iPosLandmarkParser;
    	CPosLandmarkEncoder *iPosLandmarkEncoder;
    	CPosLandmark *iPosLandmark;
        CPosLmOperation*    iOperation;
        CBufBase* iEncoderBuffer;
        HBufC8 * iParsedURL ;
        HBufC8 * iRemainingURL ;
        CPosLandmarkDatabase*   iDatabase;
        RFs iFileSession;
        RFile iFile;

        // These data types stores the values from the cfg files
        TPtrC iVerticalAccuracy;
        TPtrC iHorizontalAccuracy;
        TName iName;
        TName iDescription;
        TPtrC iLatitude;
        TPtrC iLongitude;
        TPtrC iAltitude;
        TPtrC iCategory;
        TName iMatchCategory;
        TName iMatchCategory2;
        TName iTS;
        TName  iPID;
        TPtrC  iYear;
        TPtrC  iMonth;
        TPtrC  iDay;
        TPtrC  iHour;
        TPtrC  iMinutes;
        TPtrC  iSeconds;
        TPtrC  iMSeconds;
        TPtrC iSpeed;
        TName iTelephone;
        TName iWeb;
        TPtrC iHeading;
        TName iCountry;
        TName iState;
        TName iCity;
        TName iStreet;
        TName iHouseNumber;
        TName iPostalCode;
        
        TInt iCount;


        // ?one_line_short_description_of_data
        //?data_declaration;

        // Reserved pointer for future extension
        //TAny* iReserved;

    public:     // Friend classes
        //?friend_class_declaration;
    protected:  // Friend classes
        //?friend_class_declaration;
    private:    // Friend classes
        //?friend_class_declaration;

    };

#endif      // CUTLANDMARKURL_H

// End of File
