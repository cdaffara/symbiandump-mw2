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
* Description:  MetaDataDisplay test class for STIF Test Framework TestScripter.
*
*/



#ifndef METADATADISPLAY_H
#define METADATADISPLAY_H

//  INCLUDES
#include <StifLogger.h>
#include <TestScripterInternal.h>
#include <StifTestModule.h>
#include <TestclassAssert.h>
#include <MetaDataUtility.h>

#include "ChunkDataReader.h"  
#include "ChunkDataAPITester.h"
#include "MetaDataUtilityTestAppFileDetails.h"

#include <MetaDataFieldContainer.h>//added on 15,12,2008

// CONSTANTS
//const ?type ?constant_var = ?constant;
const TInt KErrConfigInvalid = KErrNone;	//-1100;
const TInt KErrExpectedValueDifferent = -1502;	//-1101
const TInt KErrSkippedEvents = -1102;
const TInt KErrNotExecuted = -1003;
const TInt KErrExecuted = KErrNone;
const TInt KErrLeft = -1503;
const TInt KBadParameter = 1004;
const TInt KNumMetaDataField = 19;
// MACROS
//#define ?macro ?macro_def
#define TEST_CLASS_VERSION_MAJOR 0
#define TEST_CLASS_VERSION_MINOR 0
#define TEST_CLASS_VERSION_BUILD 0

#ifdef __WINSCW__
// Logging path
_LIT( KMetaDataDisplayLogPath, "\\logs\\testframework\\MetaDataDisplay\\" ); 
// Log file
_LIT( KMetaDataDisplayLogFile, "MetaDataDisplay.txt" ); 
_LIT( KMetaDataDisplayLogFileWithTitle, "MetaDataDisplay_[%S].txt" );
_LIT( KMetaDataDisplay_testPath, "c:\\testing\\data\\" );
#else
// Logging path
_LIT( KMetaDataDisplayLogPath, "\\logs\\testframework\\MetaDataDisplay\\" ); 
// Log file
_LIT( KMetaDataDisplayLogFile, "MetaDataDisplay.txt" ); 
_LIT( KMetaDataDisplayLogFileWithTitle, "MetaDataDisplay_[%S].txt" );
_LIT( KMetaDataDisplay_testPath, "e:\\testing\\data\\" );
#endif

_LIT(KTagMimeFlag, "MimeTypeFlag");
_LIT(KTagWantedField, "WantedField");
// FUNCTION PROTOTYPES
//?type ?function_name(?arg_list);

// FORWARD DECLARATIONS
//class ?FORWARD_CLASSNAME;
class CMetaDataDisplay;
class CMetaDataUtility;
class CStifSectionParser;
class CChunkDataAPITester;

class CMetaDataFieldContainer;//added on 15,12,2008

// DATA TYPES
//enum ?declaration
//typedef ?declaration
//extern ?data_type;

// CLASS DECLARATION

/**
*  CMetaDataDisplay test class for STIF Test Framework TestScripter.
*  ?other_description_lines
*
*  @lib ?library
*  @since ?Series60_version
*/
NONSHARABLE_CLASS(CMetaDataDisplay) : public CScriptBase,
									  public MMetaDataProcessor
									 
    {
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CMetaDataDisplay* NewL( CTestModuleIf& aTestModuleIf );

        /**
        * Destructor.
        */
        virtual ~CMetaDataDisplay();

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
       
        //function from MMetaDataProcessor
        virtual void ProcessMetaDataL( CMetaDataUtility *metaDataUtility, 
    	        /*CMetaDataUtilityTestAppFileDetails* aFileDetails,*/
    	        TInt error );
        void SetFileDetailL( HBufC*& aBuf, const TDesC& aDetail );
    	void SetFileDetailL( HBufC*& aBuf, const TDesC8& aDetail );
        
        
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
        CMetaDataDisplay( CTestModuleIf& aTestModuleIf );

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();
        // 
        TInt DisplayFields(TInt aEntryCount, CMetaDataUtility* iMetaData, TBool aDisplay = ETrue);
        // Clear metadata fields
        void DeleteMetaDataFields();
        
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
        // Add test case for the header "MetaDataUtility.h" 
        TInt TestCreateUtility( CStifItemParser& aItem );
        TInt TestDeleteUtility( CStifItemParser& aItem );        
        TInt TestOpenFile( CStifItemParser& aItem );       
		TInt TestOpenFileWithFieldL( CStifItemParser& aItem );
		TInt TestOpenRFile( CStifItemParser& aItem );
		TInt TestOpenRFileWithFieldL( CStifItemParser& aItem );		
		TInt TestOpenDesL( CStifItemParser& aItem );				
		TInt TestOpenDesWithFieldL( CStifItemParser& aItem );
		TInt TestMetaDataCountL( CStifItemParser& aItem );		
		TInt TestMetaDataFieldsL( CStifItemParser& aItem );
		
		//to test void OpenDesL( const TDesC8& aDes,
        //						 const RArray<TMetaDataFieldId>& aWantedFields,
        //						 const TDesC8& aMimeType );
        TInt TestOpenDesLWith3paras( CStifItemParser& aItem );                           	//debug pass
        //to test void OpenDesL( const TDesC8& aDes, const TDesC8& aMimeType );
        TInt TestOpenDesLWith2paras( CStifItemParser& aItem );								//debug pass
        //to test void OpenFileL( RFile& aFile, 
        //						  const RArray<TMetaDataFieldId>& aWantedFields, 
        //						  const TDesC8& aMimeType );
        TInt TestOpenFileLWith3paras( CStifItemParser& aItem );								//debug pass
        //to test void OpenFileL( RFile& aFile, const TDesC8& aMimeType);
        TInt TestOpenFileLWith2paras( CStifItemParser& aItem );								//debug pass
        //to test IMPORT_C void OpenFileL( const TDesC& aFileName, 
        //								   const RArray<TMetaDataFieldId>& aWantedFields, 
        //								   const TDesC8& aMimeType );
        TInt TestOpenFileLFNFeildsMime( CStifItemParser& aItem );							//debug pass
        //to test void OpenFileL( const TDesC& aFileName, const TDesC8& aMimeType );
        TInt TestOpenFileLFNMime( CStifItemParser& aItem );									//debug pass
        //to test TID3Version ID3Version();
        TInt TestID3Version( CStifItemParser& aItem );										//debug pass
        //to test  void ResetL();
        TInt TestResetL( CStifItemParser& aItem );											//debug pass
 
        TInt TestInitChunkData( CStifItemParser& aItem );	
        TInt TestProcessChunkData( CStifItemParser& aItem );
        
        //Test "not in chunk data mode" at this point since we have not done the Init() yet.
        TInt TestIsNotChunkDataMode( CStifItemParser& aItem );
        //Test do the init and the result should be "in chunk data mode"
        TInt TestIsChunkDataMode( CStifItemParser& aItem );       
        TInt TestCloseChunkData( CStifItemParser& aItem );
        //Add for create new object for CChunkDataAPITester
        TInt CreateChunkDataTester( CStifItemParser& aItem );
        //Add for delete the tester object
        TInt DestructChunkDataTester( CStifItemParser& aItem ); 
        
        
        /**************************************************************************
         *            Test functions for CMetaDataFieldContainer
         * ***********************************************************************/
        TInt GetContainerViaMetaDataUtility( CStifItemParser& aItem );						//debug pass
        //to test IMPORT_C TInt Count() const;
        TInt ContainerCount( CStifItemParser& aItem );										//debug pass
        //IMPORT_C TPtrC Field( TMetaDataFieldId  aFieldId ) const;
        TInt ContainerField( CStifItemParser& aItem );										//debug pass
        //to test IMPORT_C TPtrC8 Field8( TMetaDataFieldId  aFieldId ) const;
        TInt ContainerField8( CStifItemParser& aItem );										//debug pass
        //to test IMPORT_C void AppendL( TMetaDataFieldId  aFieldId, const TDesC& aData );
        TInt ContainerAppendLTDesCL( CStifItemParser& aItem );								//debug pass
        //to test IMPORT_C void AppendL( TMetaDataFieldId  aFieldId, const TDesC8& aData );
        TInt ContainerAppendLTDesC8L( CStifItemParser& aItem );								//debug pass
        //to test IMPORT_C TPtrC At( TInt aPosition, TMetaDataFieldId& aFieldId ) const;
        TInt ContainerAt( CStifItemParser& aItem );											//debug pass
        //to test IMPORT_C void FieldIdAt( TInt aPosition, TMetaDataFieldId& aFieldId ) const;
        TInt ContainerFieldIdAt( CStifItemParser& aItem );									//debug pass
        

        /**
         * Method used to log version of test class
         */
        void SendTestClassVersion();
        
        HBufC8* TestGetDescriptor8LC(const TFileName& aFileName);
        HBufC8* GetDescriptor8LC( const TFileName& aFileName,TBool aIsMimeUsed,TInt aSize = 0 );
        void CheckID3Version();
        

        //ADD NEW METHOD DEC HERE
        //[TestMethods] - Do not remove

    public:     // Data
        // ?one_line_short_description_of_data
        //?data_declaration;
    	
        
    protected:  // Data
        // ?one_line_short_description_of_data
        //?data_declaration;

    private:    // Data
        
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
    	CMetaDataUtility* iMetaData;
    	//Create new object
    	CChunkDataAPITester* iChunkDataTester;

    	const CMetaDataFieldContainer* iContainer;//not owned
    	
        HBufC*   iMetaDataField[KNumMetaDataField];
        TBuf<50> iMetaDataFieldName[KNumMetaDataField];
        TMetaDataFieldId iMetaDataFieldID[KNumMetaDataField];

		// For X-pcs Metadata
		HBufC* iArtistXpcs;
		HBufC* iTitleXpcs;
		HBufC* iAlbumXpcs;
		HBufC* iInfoXpcs;
    };

#endif      // METADATADISPLAY_H

// End of File
