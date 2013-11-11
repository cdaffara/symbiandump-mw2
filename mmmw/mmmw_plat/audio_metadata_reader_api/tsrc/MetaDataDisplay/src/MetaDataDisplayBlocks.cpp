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
* Description:  parsered informantion to display for STIF test
*
*/



// [INCLUDE FILES] - do not remove
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include "MetaDataDisplay.h"
#include <mmfFormatImplementationUIDs.hrh>

#include <utf.h>//added on 17,12,2008
class CnvUtfConverter;


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
// CMetaDataDisplay::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CMetaDataDisplay::Delete() 
    {

    }

// -----------------------------------------------------------------------------
// CMetaDataDisplay::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::RunMethodL( 
    CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function.        
        //ADD NEW ENTRY HERE
        // [test cases entries] - Do not remove
        // Add test case for the header "MetaDataUtility.h"  
        ENTRY( "TestCreateUtility", CMetaDataDisplay::TestCreateUtility ),
        ENTRY( "TestDeleteUtility", CMetaDataDisplay::TestDeleteUtility ),
        ENTRY( "TestOpenFile", CMetaDataDisplay::TestOpenFile ),
        ENTRY( "TestOpenFileWithFieldL", CMetaDataDisplay::TestOpenFileWithFieldL ),
        ENTRY( "TestOpenRFile", CMetaDataDisplay::TestOpenRFile ),
        ENTRY( "TestOpenRFileWithFieldL", CMetaDataDisplay::TestOpenRFileWithFieldL ),
        ENTRY( "TestOpenDesL", CMetaDataDisplay::TestOpenDesL ),
        ENTRY( "TestOpenDesWithFieldL", CMetaDataDisplay::TestOpenDesWithFieldL ), 
        ENTRY( "TestMetaDataCountL", CMetaDataDisplay::TestMetaDataCountL ),
        ENTRY( "TestMetaDataFieldsL", CMetaDataDisplay::TestMetaDataFieldsL ),
        ENTRY( "TestOpenDesLWith3paras", CMetaDataDisplay::TestOpenDesLWith3paras ),
        ENTRY( "TestOpenDesLWith2paras", CMetaDataDisplay::TestOpenDesLWith2paras ),
        ENTRY( "TestOpenFileLWith3paras", CMetaDataDisplay::TestOpenFileLWith3paras ),
        ENTRY( "TestOpenFileLWith2paras", CMetaDataDisplay::TestOpenFileLWith2paras ),
        ENTRY( "TestOpenFileLFNFeildsMime", CMetaDataDisplay::TestOpenFileLFNFeildsMime ),
        ENTRY( "TestOpenFileLFNMime", CMetaDataDisplay::TestOpenFileLFNMime ),
        ENTRY( "TestID3Version", CMetaDataDisplay::TestID3Version ),
        ENTRY( "TestResetL", CMetaDataDisplay::TestResetL ),
        ENTRY( "TestInitChunkData", CMetaDataDisplay::TestInitChunkData ),
        ENTRY( "TestProcessChunkData", CMetaDataDisplay::TestProcessChunkData ),
        ENTRY( "TestIsNotChunkDataMode", CMetaDataDisplay::TestIsNotChunkDataMode ),
        ENTRY( "TestIsChunkDataMode", CMetaDataDisplay::TestIsChunkDataMode ),
        ENTRY( "TestCloseChunkData", CMetaDataDisplay::TestCloseChunkData ),
        ENTRY( "CreateChunkDataTester", CMetaDataDisplay::CreateChunkDataTester ),
        ENTRY( "DestructChunkDataTester", CMetaDataDisplay::DestructChunkDataTester ),
        
        /*----------------------------------------------------------------------
         * Test functions for CMetaDataFieldContainer
         *----------------------------------------------------------------------
         */
        ENTRY( "GetContainerViaMetaDataUtility", CMetaDataDisplay::GetContainerViaMetaDataUtility ),
        ENTRY( "ContainerCount", CMetaDataDisplay::ContainerCount ),
        ENTRY( "ContainerField", CMetaDataDisplay::ContainerField ),
        ENTRY( "ContainerField8", CMetaDataDisplay::ContainerField8 ),
        ENTRY( "ContainerAppendLTDesCL", CMetaDataDisplay::ContainerAppendLTDesCL ),
        ENTRY( "ContainerAppendLTDesC8L", CMetaDataDisplay::ContainerAppendLTDesC8L ),
        ENTRY( "ContainerAt", CMetaDataDisplay::ContainerAt ),
        ENTRY( "ContainerFieldIdAt", CMetaDataDisplay::ContainerFieldIdAt ),
        };

    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }


// test MetaDataFieldsL test method function
TInt CMetaDataDisplay::DisplayFields(TInt aEntryCount, CMetaDataUtility* iMetaData, TBool aDisplay)
	{
	TInt i;
	TInt j=0;

	for( i = 0; i < aEntryCount; i++)
	{
		TMetaDataFieldId fieldId;
		TPtrC field = iMetaData->MetaDataFieldsL().At(i, fieldId);

		if(field != KNullDesC)
		{
		    // in case file has duplicate field ID
		    if (iMetaDataField[fieldId])
		        delete iMetaDataField[fieldId];
		
			iMetaDataField[fieldId] = field.AllocL();
			if (aDisplay)
			{
			    TBuf<50> fieldName = iMetaDataFieldName[fieldId];
			    fieldName.Append(_L("..."));
			    iLog->Log(fieldName);
			    // check for jpeg image
			    if (fieldId == EMetaDataJpeg)
			    	iLog->Log(_L("(image)"));
			    else
			    	iLog->Log(*iMetaDataField[fieldId]);
			}
			j++;
		} // end of if(field != KNullDesC)
	} // end of for

	if (j==aEntryCount)
		return KErrNone;
	else
		return KErrExpectedValueDifferent;

	}


// Clear metadata fields
void CMetaDataDisplay::DeleteMetaDataFields()
	{	
    for (TInt i=0; i < KNumMetaDataField; i++)
    if (iMetaDataField[i])
        {
        delete iMetaDataField[i];
        iMetaDataField[i] = NULL;               
        }
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestCreateUtility
// NewL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestCreateUtility( CStifItemParser& /*aItem*/ )
    {
    iLog->Log(_L("CMetaDataDisplay::TestCreateUtility"));
    TInt err = KErrNone;
	TRAP(err, iMetaData = CMetaDataUtility::NewL());
    if ( err != KErrNone )
    	{
    	iLog->Log(_L("CMetaDataDisplay::TestCreateUtility returned: %d"), err);
    	}
	else
		{
    	iLog->Log(_L("CMetaDataDisplay::TestCreateUtility returned no error"));
    	}
    for (TInt i=0; i < KNumMetaDataField; i++)
        {
        iMetaDataFieldID[i] = TMetaDataFieldId(i);
        }
    return err;
    }

// -----------------------------------------------------------------------------
//  CMetaDataDisplay::TestDeleteUtility
// (other items were commented in a header).
// -----------------------------------------------------------------------------
TInt CMetaDataDisplay::TestDeleteUtility(CStifItemParser& /*aItem*/ )
    {
    TInt err = KErrNone;
    delete iMetaData;
    iMetaData = NULL;
	iLog->Log(_L("CMetaDataDisplay::TestDeleteUtility testing ~CMetaDataUtility end err=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenFile
// OpenFileL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenFile( CStifItemParser& aItem )
	{
    TInt err = KErrNone;
	TPtrC FileNamePtr;	
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	    TBuf<120> KFrom;
   		KFrom.Append(KMetaDataDisplay_testPath);
   		KFrom.Append(FileNamePtr);
		TFileName FileName = KFrom;
		iLog->Log(_L("Input file [%S]"), &FileNamePtr);
		
		TRAP(err, iMetaData->OpenFileL(FileName));

		if ( err != KErrNone )
	    	{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenFile returned: %d"), err);
	    	}
		else
			{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenFile returned no error"));
	    	}		    
	}
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenFileWithFieldL
// OpenFileL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenFileWithFieldL( CStifItemParser& aItem )
	{
	TInt err = KErrNone;
	TPtrC FileNamePtr;
	RArray<TMetaDataFieldId> aWantedFields;
	TInt wantedField;	
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	    TBuf<120> KFrom;
   		KFrom.Append(KMetaDataDisplay_testPath);
   		KFrom.Append(FileNamePtr);
		TFileName FileName = KFrom;
		iLog->Log(_L("Input file [%S]"), &FileNamePtr);
		while( aItem.GetNextInt( wantedField ) == KErrNone )			
			{
			aWantedFields.Append(iMetaDataFieldID[wantedField]);					    		    
			}
		TRAP(err, iMetaData->OpenFileL( FileName, aWantedFields ));
		aWantedFields.Close();
		if ( err != KErrNone )
	    	{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenFileWithFieldL returned: %d"), err);
	    	}
		else
			{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenFileWithFieldL returned no error"));
	    	}		
		}
	
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenRFile
// OpenFileL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenRFile( CStifItemParser& aItem )
	{
    TInt err = KErrNone;
	TPtrC FileNamePtr;	
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	    TBuf<120> KFrom;
   		KFrom.Append(KMetaDataDisplay_testPath);
   		KFrom.Append(FileNamePtr);
		TFileName FileName = KFrom;
		iLog->Log(_L("Input file [%S]"), &FileNamePtr);
		RFs	  fs;
        RFile file;        
    	User::LeaveIfError(fs.Connect());
        fs.ShareProtected();
		User::LeaveIfError(file.Open( fs, FileName, EFileRead));
		TRAP(err, iMetaData->OpenFileL(file));
		fs.Close();
		if ( err != KErrNone )
	    	{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenRFile returned: %d"), err);
	    	}
		else
			{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenRFile returned no error"));
	    	}		    
	}
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenRFileWithFieldL
// OpenFileL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenRFileWithFieldL( CStifItemParser& aItem )
	{
	TInt err = KErrNone;
	TPtrC FileNamePtr;
	RArray<TMetaDataFieldId> aWantedFields;
	TInt wantedField;

	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	    TBuf<120> KFrom;
   		KFrom.Append(KMetaDataDisplay_testPath);
   		KFrom.Append(FileNamePtr);
		TFileName FileName = KFrom;
		iLog->Log(_L("Input file [%S]"), &FileNamePtr);
		RFs	  fs;
        RFile file;        
    	User::LeaveIfError(fs.Connect());
        fs.ShareProtected();
		User::LeaveIfError(file.Open( fs, FileName, EFileRead));
		while( aItem.GetNextInt( wantedField ) == KErrNone )			
			{
			aWantedFields.Append(iMetaDataFieldID[wantedField]);					    		    
			}
	    TRAP(err, iMetaData->OpenFileL( file, aWantedFields ));
	    fs.Close();
	    aWantedFields.Close();
	    if ( err != KErrNone )
	    	{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenRFileWithFieldL returned: %d"), err);
	    	}
		else
			{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenRFileWithFieldL returned no error"));
	    	}	
	   }
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenDesL
// OpenDesL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenDesL( CStifItemParser& aItem )
	{
    TInt err = KErrNone;
	TPtrC FileNamePtr;

	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	    TBuf<120> KFrom;
   		KFrom.Append(KMetaDataDisplay_testPath);
   		KFrom.Append(FileNamePtr);
		TFileName FileName = KFrom;
		iLog->Log(_L("Input file [%S]"), &FileNamePtr);
		
		TInt FileError, SoundSize=0;
		RFile file;
		RFs FsSession;
		iLog->Log(_L("Connecting to File Server Session") );
		FileError = FsSession.Connect();
		if (FileError)
			{
			iLog->Log(_L("File error FsSession.Connect() (%d)") , FileError);
			User::LeaveIfError(FileError);
			}

		iLog->Log(_L("Opening file to get size") );
		FileError = file.Open(FsSession, FileName, EFileStream);
		if (FileError)
			{
			iLog->Log(_L("File error file.Open() (%d)") , FileError);
			User::LeaveIfError(FileError);
			}
		FileError = file.Size(SoundSize);
		iLog->Log(_L("File Size (%d)") , SoundSize);
		if (FileError)
			{
			iLog->Log(_L("Error getting size") );
			User::LeaveIfError(FileError);
			}
		
		HBufC8 *SoundFile;
		SoundFile = HBufC8::NewLC(SoundSize);
		TPtr8 helper = SoundFile->Des();
		helper.Append (KFrom);
		FileError = file.Read( helper );
		if (FileError)
			{
			iLog->Log(_L("Error getting file Descriptor") );
			User::LeaveIfError(FileError);
			}
		
		TRAP(err, iMetaData->OpenDesL(*SoundFile));		
		CleanupStack::PopAndDestroy(SoundFile);
		file.Close();
		FsSession.Close();
		if ( err != KErrNone )
	    	{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenDesL returned: %d"), err);
	    	}
		else
			{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenDesL returned no error"));
	    	}		    
	}
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenDesWithFieldL
// OpenDesL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenDesWithFieldL( CStifItemParser& aItem )
	{
    TInt err = KErrNone;
	TPtrC FileNamePtr;	
	TInt wantedField;
	RArray<TMetaDataFieldId> aWantedFields;			   	
   	
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	    TBuf<120> KFrom;
   		KFrom.Append(KMetaDataDisplay_testPath);
   		KFrom.Append(FileNamePtr);
		TFileName FileName = KFrom;
		iLog->Log(_L("Input file [%S]"), &FileNamePtr);
		TInt FileError, SoundSize=0;
		RFile file;
		RFs FsSession;
		iLog->Log(_L("Connecting to File Server Session") );
		FileError = FsSession.Connect();
		if (FileError)
			{
			iLog->Log(_L("File error FsSession.Connect() (%d)") , FileError);
			User::LeaveIfError(FileError);
			}

		iLog->Log(_L("Opening file to get size") );
		FileError = file.Open(FsSession, FileName, EFileStream);
		if (FileError)
			{
			iLog->Log(_L("File error file.Open() (%d)") , FileError);
			User::LeaveIfError(FileError);
			}
		FileError = file.Size(SoundSize);
		iLog->Log(_L("File Size (%d)") , SoundSize);
		if (FileError)
			{
			iLog->Log(_L("Error getting size") );
			User::LeaveIfError(FileError);
			}
		
		HBufC8 *SoundFile;
		SoundFile = HBufC8::NewLC(SoundSize);
		TPtr8 helper = SoundFile->Des();
		helper.Append (KFrom);
		FileError = file.Read( helper );
		if (FileError)
			{
			iLog->Log(_L("Error getting file Descriptor") );
			User::LeaveIfError(FileError);
			}
		// Get aWantedFields
		while( aItem.GetNextInt( wantedField ) == KErrNone )			
			{
			aWantedFields.Append(iMetaDataFieldID[wantedField]);					    		    
			}
		TRAP(err, iMetaData->OpenDesL(*SoundFile, aWantedFields));
		CleanupStack::PopAndDestroy(SoundFile);		
		aWantedFields.Close();
		file.Close();
		FsSession.Close();
		if ( err != KErrNone )
	    	{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenDesL returned: %d"), err);
	    	}
		else
			{
	    	iLog->Log(_L("CMetaDataDisplay::TestOpenDesL returned no error"));
	    	}			
	   }
	return err;
	}


// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestMetaDataCountL
// MetaDataCount test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestMetaDataCountL(CStifItemParser& aItem )
	{
	TInt err = KErrNone;
	TPtrC FileNamePtr;	
	TInt count;
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	    TBuf<120> KFrom;
  		KFrom.Append(KMetaDataDisplay_testPath);
  		KFrom.Append(FileNamePtr);
		TFileName FileName = KFrom;
		iLog->Log(_L("Input file [%S]"), &FileNamePtr);
		
		TRAP(err, iMetaData->OpenFileL(FileName));
		if (!err)
		   {
			count = iMetaData->MetaDataCount();
			TInt expectedEntryCount; 
			aItem.GetNextInt(expectedEntryCount);
			if ( count != expectedEntryCount )
		    	{
		    	iLog->Log(_L("CMetaDataDisplay::TestMetaDataCountL returned: %d"), count);
		    	}
			else
				{
		    	iLog->Log(_L("CMetaDataDisplay::TestMetaDataCountL returned no error"));
		    	}	
		   }
	   }
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestMetaDataFieldsL
// MetaDataFieldsL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestMetaDataFieldsL( CStifItemParser& aItem )
	{
	TInt err = KErrNone;
	TPtrC FileNamePtr;	
	TInt count;
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	    TBuf<120> KFrom;
  		KFrom.Append(KMetaDataDisplay_testPath);
  		KFrom.Append(FileNamePtr);
		TFileName FileName = KFrom;
		iLog->Log(_L("Input file [%S]"), &FileNamePtr);
		
		TRAP(err, iMetaData->OpenFileL(FileName));
		if (!err)
		   {
			count = iMetaData->MetaDataCount();
			TInt expectedEntryCount; 
			aItem.GetNextInt(expectedEntryCount);
			if ( count == expectedEntryCount )
		    	{
		    	TInt error = DisplayFields(count, iMetaData);
                DeleteMetaDataFields();
                if (!error)
				    {
					iLog->Log(_L("Test was successful"));						
					}
					else
					{
					iLog->Log(_L("Display metadata fields error [%d]"), error);					
					}                
                iLog->Log(_L("CMetaDataDisplay::TestMetaDataFieldsL returned no error"));
		    	}
			else
				{
				iLog->Log(_L("CMetaDataDisplay::TestMetaDataFieldsL returned: %d"), err);
		    	
		    	}	
		   }
	   }
	return err;
	}


HBufC8* CMetaDataDisplay::TestGetDescriptor8LC(const TFileName& aFileName)
		{
		TInt FileError, SoundSize=0;
		RFile file;
		RFs FsSession;
		HBufC8 *SoundFileDes;
	
		iLog->Log(_L("Connecting to File Server Session") );
		FileError = FsSession.Connect();
		if (FileError)
			{
			iLog->Log(_L("File error FsSession.Connect() (%d)") , FileError);
			User::LeaveIfError(FileError);
			}
	
		iLog->Log(_L("Opening file to get size") );
		FileError = file.Open(FsSession, aFileName, EFileStream);
		if (FileError)
			{
			iLog->Log(_L("File error file.Open() (%d)") , FileError);
			User::LeaveIfError(FileError);
			}
		FileError = file.Size(SoundSize);
		iLog->Log(_L("File Size (%d)") , SoundSize);
		if (FileError)
			{
			iLog->Log(_L("Error getting size") );
			User::LeaveIfError(FileError);
			}
		
	//	TPtr8 helper;
	
		SoundFileDes = HBufC8::NewLC(SoundSize);
	
		TPtr8 helper = SoundFileDes->Des();
		FileError = file.Read( helper );
		
		if (FileError)
		{
		iLog->Log(_L("Error getting file Descriptor") );
		User::LeaveIfError(FileError);
		
		}
	
		iLog->Log(_L("Descriptor length [%d]") , SoundFileDes->Size() );
		file.Close();
		FsSession.Close();
		return SoundFileDes;
		}

HBufC8* CMetaDataDisplay::GetDescriptor8LC( const TFileName& aFileName,TBool aIsMimeUsed,TInt aSize )
		{
		TInt FileError, SoundSize = aSize;
		HBufC8 *SoundFile;	
		RFile file;
		RFs FsSession;
		if( SoundSize == 0 )
			{
			iLog->Log(_L("Connecting to File Server Session") );
			FileError = FsSession.Connect();
			if (FileError)
				{
				iLog->Log(_L("File error FsSession.Connect() (%d)") , FileError);
				User::LeaveIfError(FileError);
				}
	
			iLog->Log(_L("Opening file to get size") );
			FileError = file.Open(FsSession, aFileName, EFileStream);
			if (FileError)
				{
				iLog->Log(_L("File error file.Open() (%d)") , FileError);
				User::LeaveIfError(FileError);
				}
			FileError = file.Size(SoundSize);
			iLog->Log(_L("File Size (%d)") , SoundSize);
			if (FileError)
				{
				iLog->Log(_L("Error getting size") );
				User::LeaveIfError(FileError);
				}
			file.Close();
			FsSession.Close();
			}
		//	TPtr8 helper;
		if ( aIsMimeUsed )
			{
			_LIT(MP3MIME,"audio/mpeg");
			SoundFile = HBufC8::NewLC( SoundSize );
	
			TPtr8 helper(SoundFile->Des());
			helper.Append(MP3MIME);
			}
		else
			{
			SoundFile = HBufC8::NewLC(SoundSize);
			TPtr8 helper = SoundFile->Des();
			FileError = file.Read( helper );
			if (FileError)
			{
			iLog->Log(_L("Error getting file Descriptor") );
			User::LeaveIfError(FileError);
			}
			iLog->Log(_L("Descriptor length [%d]") , SoundFile->Size() );
			}
		return SoundFile;
		}

void CMetaDataDisplay::CheckID3Version()
	{
	iLog->Log( _L("Checking ID3 version.") );
	TID3Version ID3Version = iMetaData->ID3Version();
	switch( ID3Version )
		{
		case ENonID3:
			iLog->Log( _L("ID3 version is ENonID3.") );
			break;
		case EID3Version1:
			iLog->Log( _L("ID3 version is EID3Version1.") );
			break;
		case EID3Version2:
			iLog->Log( _L("ID3 version is EID3Version2.") );
			break;
		}
	iLog->Log( _L("Checking ID3 version end.") );
	}


// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenDesLWith3paras
// TestOpenDesLWith3paras test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenDesLWith3paras(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestOpenDesLWith3paras.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;
    RArray<TMetaDataFieldId> aWantedFields;
    TInt wantedField;
    HBufC8 *SoundFile;
    HBufC8 *SoundFileDes;
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
  	   KFrom.Append(KMetaDataDisplay_testPath);
  	   KFrom.Append(FileNamePtr);
	   TFileName FileName = KFrom;

	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );
	   SoundFileDes = TestGetDescriptor8LC( FileName );
	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }

	   while( !aItem.GetNextInt( wantedField ) )
		   {
		   aWantedFields.Append( iMetaDataFieldID[wantedField] );
		   }
	   if( SoundFileDes && SoundFile )
		   {
		   //iMetaData->OpenDesL( *SoundFileDes,aWantedFields,*SoundFile );
		   TRAP( err, iMetaData->OpenDesL( *SoundFileDes,aWantedFields,*SoundFile ) );
		   }
	   else
		   {
		   err = KBadParameter;
		   iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		   User::Leave( err );
		   }
	   if ( err != KErrNone )
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenDesLWith3paras returned: %d"), err );
		   }
	   else
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenDesLWith3paras returned no error") );
		   }
	   CleanupStack::PopAndDestroy( 2 );//SoundFileDes first ,SoundFile second
	}
	iLog->Log( _L("End calling TestOpenDesLWith3paras.") );
	return err;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenDesLWith2paras
// TestOpenDesLWith2paras test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenDesLWith2paras(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestOpenDesLWith2paras.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;
    HBufC8 *SoundFile;
    HBufC8 *SoundFileDes;
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
 	   KFrom.Append(KMetaDataDisplay_testPath);
 	   KFrom.Append(FileNamePtr);
   	   TFileName FileName = KFrom;
	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );
	   SoundFileDes = TestGetDescriptor8LC( FileName );
	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }
	   
	   if( SoundFileDes && SoundFile )
		   {
		   //iMetaData->OpenDesL( *SoundFileDes,*SoundFile );
		   TRAP( err, iMetaData->OpenDesL( *SoundFileDes,*SoundFile ) );
		   }
	   else
		   {
		   err = KBadParameter;
		   iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		   User::Leave( err );
		   }
	   if ( err != KErrNone )
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenDesLWith2paras returned: %d"), err );
		   }
	   else
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenDesLWith2paras returned no error") );
		   }
	   CleanupStack::PopAndDestroy( 2 );//SoundFileDes first ,SoundFile second
	}
	iLog->Log( _L("End calling TestOpenDesLWith2paras.") );
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenFileLWith3paras
// TestOpenFileLWith3paras test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenFileLWith3paras(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestOpenFileLWith3paras.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;
	TInt isCheckID3Version( 0 );	
    RArray<TMetaDataFieldId> aWantedFields;
    TInt wantedField;
    HBufC8 *SoundFile;
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
 	   KFrom.Append(KMetaDataDisplay_testPath);
 	   KFrom.Append(FileNamePtr);
   	   TFileName FileName = KFrom;
	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );
	   
	   RFs fs;
       RFile file;
       User::LeaveIfError(fs.Connect());
       fs.ShareProtected();
       User::LeaveIfError(file.Open( fs, FileName, EFileRead));
       TInt SoundSize;
       TInt FileError = file.Size(SoundSize);
       iLog->Log(_L("File Size (%d)") , SoundSize);
       if( FileError )
    	   {
    	   iLog->Log( _L("Error getting size") );
    	   User::LeaveIfError( FileError );
    	   }
	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue,SoundSize );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse,SoundSize );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }
	   
	   aItem.GetNextInt( isCheckID3Version );
	   
	   while( !aItem.GetNextInt( wantedField ) )
		   {
		   aWantedFields.Append( iMetaDataFieldID[wantedField] );
		   }
	   if( SoundFile )
		   {
		   //iMetaData->OpenFileL( file,aWantedFields,*SoundFile );
		   TRAP( err, iMetaData->OpenFileL( file,aWantedFields,*SoundFile ) );
		   if( isCheckID3Version )
			   {//TInt isCheckID3Version( 0 );	
			   CheckID3Version();
			   }
		   }
	   else
		   {
		   err = KBadParameter;
		   iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		   User::Leave( err );
		   }
	   if ( err != KErrNone )
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenFileLWith3paras returned: %d"), err );
		   }
	   else
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenFileLWith3paras returned no error") );
		   }
	   CleanupStack::PopAndDestroy( SoundFile );
	   file.Close();
	   fs.Close();
	}
	iLog->Log( _L("End calling TestOpenFileLWith3paras.") );
	return err;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenFileLWith2paras
// TestOpenFileLWith2paras test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenFileLWith2paras(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestOpenFileLWith2paras.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;
	TInt isCheckID3Version( 0 );	

    HBufC8 *SoundFile;
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
 	   KFrom.Append(KMetaDataDisplay_testPath);
 	   KFrom.Append(FileNamePtr);
   	   TFileName FileName = KFrom;
	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );
	   
	   RFs fs;
       RFile file;
       User::LeaveIfError(fs.Connect());
       //fs.ShareProtected();
       User::LeaveIfError(file.Open( fs, FileName, EFileRead) );
       TInt SoundSize;
       TInt FileError = file.Size(SoundSize);
       iLog->Log(_L("File Size (%d)") , SoundSize);
       if( FileError )
    	   {
    	   iLog->Log( _L("Error getting size") );
    	   User::LeaveIfError( FileError );
    	   }
	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue,SoundSize );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse,SoundSize );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }
	   
	   aItem.GetNextInt( isCheckID3Version );
	   
	   if( SoundFile )
		   {
		   //iMetaData->OpenFileL( file,*SoundFile );
		   TRAP( err, iMetaData->OpenFileL( file,*SoundFile ) );
		   if( isCheckID3Version )
			   {//TInt isCheckID3Version( 0 );	
			   CheckID3Version();
			   }
		   }
	   else
		   {
		   err = KBadParameter;
		   iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		   User::Leave( err );
		   }
	   if ( err != KErrNone )
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenFileLWith2paras returned: %d"), err );
		   }
	   else
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenFileLWith2paras returned no error") );
		   }
	   CleanupStack::PopAndDestroy( SoundFile );
	   file.Close();
	   fs.Close();
	   
	}
	iLog->Log( _L("End calling TestOpenFileLWith2paras.") );
	return err;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenFileLFNFeildsMime
// TestOpenFileLFNFeildsMime test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenFileLFNFeildsMime(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestOpenFileLFNFeildsMime.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;
	TInt isCheckID3Version( 0 );	
    RArray<TMetaDataFieldId> aWantedFields;
    TInt wantedField;
    HBufC8 *SoundFile;

	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
 	   KFrom.Append(KMetaDataDisplay_testPath);
 	   KFrom.Append(FileNamePtr);
   	   TFileName FileName = KFrom;
	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );

	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }
	   
	   aItem.GetNextInt( isCheckID3Version );
	   
	   while( !aItem.GetNextInt( wantedField ) )
		   {
		   aWantedFields.Append( iMetaDataFieldID[wantedField] );
		   }
	   if( SoundFile )
		   {
		   //iMetaData->OpenFileL( FileName,aWantedFields,*SoundFile );
		   TRAP( err, iMetaData->OpenFileL( FileName,aWantedFields,*SoundFile ) );
		   if( isCheckID3Version )
			   {//TInt isCheckID3Version( 0 );	
			   CheckID3Version();
			   }
		   }
	   else
		   {
		   err = KBadParameter;
		   iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		   User::Leave( err );
		   }
	   if ( err != KErrNone )
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenFileLFNFeildsMime returned: %d"), err );
		   }
	   else
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenFileLFNFeildsMime returned no error") );
		   }
	   CleanupStack::PopAndDestroy( SoundFile );//SoundFile
	}
	iLog->Log( _L("End calling TestOpenFileLFNFeildsMime.") );
	return err;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestOpenFileLFNMime
// TestOpenFileLFNMime test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestOpenFileLFNMime(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestOpenFileLFNMime.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;
	TInt isCheckID3Version( 0 );	
    HBufC8 *SoundFile;

	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
 	   KFrom.Append(KMetaDataDisplay_testPath);
 	   KFrom.Append(FileNamePtr);
   	   TFileName FileName = KFrom;
	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );

	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }
	   aItem.GetNextInt( isCheckID3Version );
	   if( SoundFile )
		   {
		   //iMetaData->OpenFileL( FileName,*SoundFile );
		   TRAP( err, iMetaData->OpenFileL( FileName,*SoundFile ) );
		   if( isCheckID3Version )
			   {//TInt isCheckID3Version( 0 );	
			   CheckID3Version();
			   }
		   }
	   else
		   {
		   err = KBadParameter;
		   iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		   User::Leave( err );
		   }
	   if ( err != KErrNone )
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenFileLFNMime returned: %d"), err );
		   }
	   else
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestOpenFileLFNMime returned no error") );
		   }
	   CleanupStack::PopAndDestroy( SoundFile );//SoundFile
	}
	iLog->Log( _L("End calling TestOpenFileLFNMime.") );
	return err;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestID3Version
// TestID3Version test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestID3Version(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestID3Version.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;
	TInt isCheckID3Version( 0 );
    HBufC8 *SoundFile;

	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
 	   KFrom.Append(KMetaDataDisplay_testPath);
 	   KFrom.Append(FileNamePtr);
   	   TFileName FileName = KFrom;
	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );

	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }
	   aItem.GetNextInt( isCheckID3Version );
	   if( SoundFile )
		   {
		   iMetaData->OpenFileL( FileName,*SoundFile );
		   if( isCheckID3Version )
			   {//TInt isCheckID3Version( 0 );	
			   CheckID3Version();
			   }
		   }
	   else
		   {
		   err = KBadParameter;
		   iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		   User::Leave( err );
		   }
	   if ( err != KErrNone )
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestID3Version returned: %d"), err );
		   }
	   else
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestID3Version returned no error") );
		   }
	   CleanupStack::PopAndDestroy( SoundFile );//SoundFile
	}
	iLog->Log( _L("End calling TestID3Version.") );
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestResetL
// ResetL test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestResetL(CStifItemParser& /*aItem*/ )
	{
	iLog->Log( _L("Entering TestResetL.") );
	TInt err = KErrNone;
	TRAP( err,iMetaData->ResetL() );
	if( err == KErrNone )
		{
		iLog->Log( _L("CMetaDataDisplay::TestResetL returned no error") );
		}
	else
		{
		iLog->Log( _L("CMetaDataDisplay::TestResetL returned: %d"), err );
		}
	iLog->Log( _L("End calling TestResetL.") );
	return err;
	}


void CMetaDataDisplay::ProcessMetaDataL( CMetaDataUtility *metaDataUtility, 
								        /*CMetaDataUtilityTestAppFileDetails* aFileDetails,*/
								        TInt error )
	{//TODO need to implement the code here
	if ( error != KErrNone ) 
        { 
        // there's a problem    
	    iLog->Log( _L("Pass in bad parameter(s): %d"), error );
	    User::Leave( error );
        } 

    TInt count = metaDataUtility->MetaDataCount(); 
    
    for(TInt i = 0; i < count; i++) 
       { 
        TMetaDataFieldId fieldId; 
        
        TPtrC field = metaDataUtility->MetaDataFieldsL().At( i, fieldId ); 
        if( field != KNullDesC ) 
            { 
            switch( fieldId ) 
                { 
            	case EMetaDataSongTitle:
                    {
                   // SetFileDetailL( aFileDetails->iTitle, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataArtist:
                    {
                  //  SetFileDetailL( aFileDetails->iArtist, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataAlbum:
                    {
                  //  SetFileDetailL( aFileDetails->iAlbum, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataYear:
                    {
                  //  SetFileDetailL( aFileDetails->iYear, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataComment:
                    {
                  //  SetFileDetailL( aFileDetails->iComment, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataAlbumTrack:
                    {
                  //  SetFileDetailL( aFileDetails->iAlbumTrack, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataGenre:
                    {
                  //  SetFileDetailL( aFileDetails->iGenre, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataComposer:
                    {
                  //  SetFileDetailL( aFileDetails->iComposer, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataCopyright:
                    {
                 //   SetFileDetailL( aFileDetails->iCopyright, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataOriginalArtist:
                    {
                //    SetFileDetailL( aFileDetails->iOriginalArtist, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
                case EMetaDataUserUrl:
                case EMetaDataUrl:     
                    {
                //    SetFileDetailL( aFileDetails->iAudioFileWebPage, field );
                    iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;
                    }
               	case EMetaDataJpeg:
	               	{
	                //SetFileDetailL( aFileDetails->iAlbumArt, field );
	                iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
	               	break;	
	               	}
				case EMetaDataVendor:
					{
				//	SetFileDetailL( aFileDetails->iVendor, field );
					iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;	
					}
                case EMetaDataRating:
                  	{
                //  SetFileDetailL( aFileDetails->iRating, field );
                   	iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
					break;	
                   	}
                case EMetaDataDuration:
	                {
	            //    SetFileDetailL( aFileDetails->iComposer, field );
	                iLog->Log( _L("CMetaDataDisplay::ProcessMetaDataL returned: %d"), error );
                    break;	
	                }
	            default:
                    {
                    // nothing to do
                    break;
                    }
                } 
            }
        }
	}


// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestInitChunkData
// InitChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestInitChunkData(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestInitChunkData.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;   
    HBufC8 *SoundFile;
    HBufC8 *SoundFileDes;
    
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
  	   KFrom.Append(KMetaDataDisplay_testPath);
  	   KFrom.Append(FileNamePtr);
	   TFileName FileName = KFrom;

	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );
	   SoundFileDes = TestGetDescriptor8LC( FileName );
	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }

	   if( SoundFileDes && SoundFile )
		   {		  
		   err = iChunkDataTester->TestInitChunkDataAPI( *SoundFile );
		   
           if( err == KBadParameter )
               {
		       iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		       User::Leave( err );
		       }
		   else if ( err != KErrNone )
		       {
		       iLog->Log( _L("CMetaDataDisplay::TestInitChunkData returned: %d"), err );
		       User::Leave( err );
		       }
	       else
		       {
		       iLog->Log( _L("CMetaDataDisplay::TestInitChunkData returned no error") );
		       }
		       
		   //[Arnel and Flo]
		   //close the metadata utility and delete the tester object          
           //Add CMetaDataDisplay::DestructChunkDataTester to delete the tester object
           //and call the close and delete function in cfg file
		   }
		   
	   CleanupStack::PopAndDestroy( 2 );
	   
	}
	iLog->Log( _L("End calling TestInitChunkData.") );
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestProcessChunkData
// ProcessChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestProcessChunkData(CStifItemParser& aItem )
	{
	iLog->Log( _L("Entering TestProcessChunkData.") );
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	TInt isMimeUsed;   
    HBufC8 *SoundFile;
    HBufC8 *SoundFileDes;
	if ( aItem.GetNextString( FileNamePtr ) == KErrNone )
	   {
	   TBuf<120> KFrom;
  	   KFrom.Append(KMetaDataDisplay_testPath);
  	   KFrom.Append(FileNamePtr);
	   TFileName FileName = KFrom;

	   iLog->Log( _L("Input file [%S]"), &FileNamePtr );
	   SoundFileDes = TestGetDescriptor8LC( FileName );
	   if( ( err = aItem.GetNextInt( isMimeUsed ) ) == KErrNone )
		   {
		   if( isMimeUsed )
			   {
			   SoundFile = GetDescriptor8LC( FileName,ETrue );
			   }
		   else
			   {
			   SoundFile = GetDescriptor8LC( FileName,EFalse );
			   }	   
		   }
	   else
		   {
		   iLog->Log( _L("Bad parameter is passed in.") );
		   User::Leave( err );
		   }

	   if( SoundFileDes && SoundFile )
		   {
		   err = iChunkDataTester->TestInitChunkDataAPI( *SoundFile );
		   if ( err == KErrNone ) 
	           { 
	           // error can be KErrInvalidArg (e.g., chunk size too large) 
	           //[Arnel and Flo]
	           // This method will also test parsing when the metadatautility reaches a
	           // state where the data is ready to be parsed.
	           err = iChunkDataTester->TestProcessChunkDataAPI( FileName ); 
	           } 
        
	       // setup for a callback from the tester 
		   iChunkDataTester->WaitToProcessMetaData( this ); 	
		   
		   //[Arnel and Flo]
		   //In the callback, maybe you should close the metadata utility and delete the tester object?     
		   }
	   
	   if ( err == KBadParameter )
		   {		  
		   iLog->Log( _L("Pass in bad parameter(s): %d"), err );
		   User::Leave( err );
		   }
	   else if ( err != KErrNone )
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestProcessChunkData returned: %d"), err );
		   User::Leave( err );
		   }
	   else
		   {
		   iLog->Log( _L("CMetaDataDisplay::TestProcessChunkData returned no error") );
		   }
		   
	   CleanupStack::PopAndDestroy( 2 );
	   
		   //[Arnel and Flo]
		   //close the metadata utility and delete the tester object 	   
   		   //Add CMetaDataDisplay::DestructChunkDataTester to delete the tester object
           //and call the close and delete function in cfg file
	}
	iLog->Log( _L("End calling TestProcessChunkData.") );
	return err;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestParseChunkData
// ParseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
//[Arnel and Flo]
// This can be removed since parsing is also tested by TestProcessChunkData().


// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestIsNotChunkDataMode
// TestIsChunkDataModeAPI "not in chunk data mode" test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestIsNotChunkDataMode(CStifItemParser& /*aItem*/ )
	{
	iLog->Log( _L("---------->Entering TestIsNotChunkDataMode") );
	TBool isChunkDataMode = iChunkDataTester->TestIsChunkDataModeAPI();		
	//[Flo and Arnel]
	// The result should be "not in chunk data mode" at this point since we have not
	// done the Init() yet.
	
	// The Init() must be called before calling TestIsChunkDataModeAPI()
	// so we separate two test cases to verify whether it is in Chunk Data mode
	if( isChunkDataMode )
		{
		iLog->Log( _L("It is in Chunk Data mode") );
		User::Leave(-1);
		}
	else
		{
		iLog->Log( _L("It is not in Chunk Data mode") );
		} 		
	iLog->Log( _L("<----------End of TestIsNotChunkDataMode") );
	
		   //[Arnel and Flo]
		   //close the metadata utility and delete the tester object 
		   //Add CMetaDataDisplay::DestructChunkDataTester to delete the tester object
           //and call the close and delete function in cfg file
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestIsChunkDataMode
// TestIsChunkDataModeAPI "in chunk data mode" test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestIsChunkDataMode(CStifItemParser& /*aItem*/ )
	{
	iLog->Log( _L("---------->Entering TestIsChunkDataMode") );
	TBool isChunkDataMode = iChunkDataTester->TestIsChunkDataModeAPI();
	
    //[Flo and Arnel]
    // Now, let's do the init and the result should be "in chunk data mode".
    // InitChunkData(); // TODO: implement this
	
	// The Init() must be called before calling TestIsChunkDataModeAPI()
	// so we separate two test cases to verify whether it is in Chunk Data mode
	// and the funcation init() is called in cfg file
	if( isChunkDataMode )
		{
		iLog->Log( _L("It is in Chunk Data mode") );
		}
	else
		{
		iLog->Log( _L("It is not in Chunk Data mode") );
		User::Leave(-1);
		}        
    		
	iLog->Log( _L("<----------End of TestIsChunkDataMode") );
	
		   //[Arnel and Flo]
		   //close the metadata utility and delete the tester object 
		   //Add CMetaDataDisplay::DestructChunkDataTester to delete the tester object
           //and call the close and delete function in cfg file
	return KErrNone;
	}


// -----------------------------------------------------------------------------
// CMetaDataDisplay::TestCloseChunkData
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::TestCloseChunkData(CStifItemParser& /*aItem*/ )
	{
	iLog->Log( _L("---------->Entering TestCloseChunkData") );
	iChunkDataTester->TestCloseChunkDataAPI();
	iLog->Log( _L("<----------End of TestCloseChunkData") );
	return KErrNone;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::CreateChunkDataTester
// CreateChunkDataTester test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::CreateChunkDataTester( CStifItemParser& /*aItem*/ )
	{
	iLog->Log( _L("-------->Create ChunkDataAPITester...") );
	TInt result = KErrNone;	
	TRAP( result,iChunkDataTester = CChunkDataAPITester::NewL() );
	if( result == KErrNone )
		{
		iLog->Log( _L("Successfully create ChunkDataAPITester.") );
		}
	else
		{
		iLog->Log( _L("Fail to create ChunkDataAPITester.Error %d return."),result );
		}
	iLog->Log( _L("<--------The end of creation of ChunkDataAPITester.") );
	return result;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::DestructChunkDataTester
// Delete the tester object test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::DestructChunkDataTester( CStifItemParser& /*aItem*/ )
	{
	TInt err = KErrNone;
	delete iChunkDataTester;
	iChunkDataTester = NULL;
	iLog->Log( _L("CMetaDataDisplay::DestructChunkDataTester testing ~CChunkDataAPITester end err=%d"), err);
	return err;
	}


/******************************************************************************************
*
*                      Test functions for CMetaDataFieldContainer
*
******************************************************************************************/
// -----------------------------------------------------------------------------
// CMetaDataDisplay::GetContainerViaMetaDataUtility
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::GetContainerViaMetaDataUtility( CStifItemParser& /*aItem*/ )
	{
	iLog->Log( _L("") );//A blank line makes the log file look better
	iLog->Log( _L("Entering GetContainerViaMetaDataUtility-->") );
	TInt result( 0 );
	//iContainer  = &iMetaData->MetaDataFieldsL();
	TRAP( result,iContainer  = &iMetaData->MetaDataFieldsL() );
	if( result == KErrNone )
		{
		iLog->Log( _L("Succeeded to get CMetaDataFieldContainer.") );
		}
	else
		{
		iLog->Log( _L("Failed to create CMetaDataFieldContainer.Error %d returns."),result );
		}
	iLog->Log( _L("<--Out of GetContainerViaMetaDataUtility.") );
	return result;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::ContainerCount
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::ContainerCount( CStifItemParser& /*aItem*/ )
	{
	iLog->Log( _L("") );//A blank line makes the log file look better
	iLog->Log( _L("Entering ContainerCount-->") );
	TInt result( 0 );
	TInt count = iContainer->Count();
	iLog->Log( _L("The number of meta data fields is %d."),count );
	iLog->Log( _L("<--Out of ContainerCount.") );
	return result;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::ContainerField
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::ContainerField( CStifItemParser& aItem )
	{
	iLog->Log( _L("") );//A blank line makes the log file look better
	iLog->Log( _L("Entering ContainerField-->") );
	TInt result( 0 );
	TInt fieldId;
	while( aItem.GetNextInt( fieldId ) == KErrNone )
		{
		if( ( 0 <= fieldId ) && ( fieldId <= 19) )
			{
			TMetaDataFieldId tempFieldId = (TMetaDataFieldId)fieldId;
			TPtrC field = iContainer->Field( tempFieldId );
			switch( tempFieldId )
				{//There are 19 cases to be processed,I am not gonna to process all of them
				case EUnknownMetaDataField:
					{
					if( field == KNullDesC() )
						{
						iLog->Log( _L("The content of meta data field EUnknownMetaDataField is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EUnknownMetaDataField is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataSongTitle:
					{
					if( field == KNullDesC() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataSongTitle is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataSongTitle is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataArtist:
					{
					if( field == KNullDesC() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataArtist is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataArtist is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataAlbum:
					{
					if( field == KNullDesC() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataAlbum is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataAlbum is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataDuration:
					{
					if( field == KNullDesC() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataDuration is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataDuration is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataDate:
					{
					if( field == KNullDesC() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataDate is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataDate is [%S]."),field.Alloc() );
						}
					break;
					}
					
				default:
					{
					if( field == KNullDesC() )
						{
						iLog->Log( _L("The content of meta data field [%d] is NULL."),tempFieldId );
						}
					else
						{
						//iLog->Log( _L("The content of meta data field [%d] is [%S]."),field.Alloc() );
					    iLog->Log( _L("Unknown meta data field"));
						}
					break;
					}
				}
			}
		else
			{
			result= KBadParameter;
			iLog->Log( _L("<--The meta data field Id you input is out of bound( 0 <= Id <= 19 ).User leaves here.") );
			User::Leave( KBadParameter );
			}
		}
	iLog->Log( _L("<--Out of ContainerField.") );
	return result;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::ContainerField8
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::ContainerField8( CStifItemParser& aItem )
	{
	iLog->Log( _L("") );//A blank line makes the log file look better
	iLog->Log( _L("Entering ContainerField8-->") );
	TInt result( 0 );
	TInt fieldId;
	while( aItem.GetNextInt( fieldId ) == KErrNone )
		{
		if( ( 0 <= fieldId ) && ( fieldId <= 19 ) )
			{
			TMetaDataFieldId tempFieldId = (TMetaDataFieldId)fieldId;
			TPtrC8 field = iContainer->Field8( tempFieldId );
			switch( tempFieldId )
				{//There are 19 cases to be processed,I am not gonna to process all of them
				case EUnknownMetaDataField:
					{
					if( field == KNullDesC8() )
						{
						iLog->Log( _L("The content of meta data field EUnknownMetaDataField is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EUnknownMetaDataField is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataSongTitle:
					{
					if( field == KNullDesC8() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataSongTitle is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataSongTitle is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataArtist:
					{
					if( field == KNullDesC8() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataArtist is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataArtist is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataAlbum:
					{
					if( field == KNullDesC8() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataAlbum is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataAlbum is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataDuration:
					{
					if( field == KNullDesC8() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataDuration is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataDuration is [%S]."),field.Alloc() );
						}
					break;
					}
					
				case EMetaDataDate:
					{
					if( field == KNullDesC8() )
						{
						iLog->Log( _L("The content of meta data field EMetaDataDate is NULL.") );
						}
					else
						{
						iLog->Log( _L("The content of meta data field EMetaDataDate is [%S]."),field.Alloc() );
						}
					break;
					}
					
				default:
					{
					if( field == KNullDesC8() )
						{
						iLog->Log( _L("The content of meta data field [%d] is NULL."),tempFieldId );
						}
					else
						{
						iLog->Log( _L("The content of meta data field [%d] is [%S]."),field.Alloc() );
						}
					break;
					}
				}
			}
		else
			{
			result= KBadParameter;
			iLog->Log( _L("<--The meta data field Id you input is out of bound( 0 <= Id <= 19 ).User leaves here.") );
			User::Leave( KBadParameter );
			}
		}
	iLog->Log( _L("<--Out of ContainerField8.") );
	return result;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::ContainerAppendLTDesCL
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::ContainerAppendLTDesCL( CStifItemParser& aItem )
	{
	iLog->Log( _L("") );//A blank line makes the log file look better
	iLog->Log( _L("Entering ContainerAppendLTDesCL-->") );
	TInt result( 0 );
	TInt fieldId;
	TInt NumOfFieldAppended( 0 );
	if( aItem.GetNextInt( NumOfFieldAppended ) == KErrNone )
		{
		if( ( 0 < NumOfFieldAppended ) && ( NumOfFieldAppended <= 19 ) )
			{
			for( TInt num = NumOfFieldAppended ; (num--)>0 ; )
				{
				TInt fieldTemp;
				TPtrC fieldContent;
				if( aItem.GetNextInt( fieldTemp ) == KErrNone )
					{
					if( ( 0 <= fieldTemp ) && ( fieldTemp <= 19 ) )
						{
						fieldId = fieldTemp;
						if( aItem.GetNextString( fieldContent ) == KErrNone )
							{
							iLog->Log( _L("Meta data field [%d] is being appended."),fieldId );
							iLog->Log( _L("Content of the just appended meta data field is [%S]."),fieldContent.Alloc() );
							TMetaDataFieldId tempFieldId = (TMetaDataFieldId)fieldId;
							//const_cast<CMetaDataFieldContainer*>(iContainer)->AppendL( tempFieldId, fieldContent );	//TODO
							TRAP( result,const_cast<CMetaDataFieldContainer*>(iContainer)->AppendL( tempFieldId, fieldContent ) );
							if( result == KErrNone )
								{
								iLog->Log( _L("Succeeded to append meta data field [%d]."),fieldId );
								}
							else
								{
								iLog->Log( _L("Failed to append meta data field [%d].Error [%d] returns."),fieldId,result );
								}
							}
						else
							{
							result = KBadParameter;
							iLog->Log( _L("Pass in bad parameter.User Leave here.") );
							User::Leave( KBadParameter );
							}
						}
					else
						{
						result= KBadParameter;
						iLog->Log( _L("<--The meta data field Id [%d] you input is out of bound( 0 <= Id <= 19 ).User leaves here."),fieldTemp );
						User::Leave( KBadParameter );
						}

					}
				else
					{
					result = KBadParameter;
					iLog->Log( _L("Pass in bad parameter.User Leave here.") );
					User::Leave( KBadParameter );
					}
				}
			}
		else if( NumOfFieldAppended == 0 )
			{
			result = KErrNone;
			iLog->Log( _L("No meta data field is appended.") );
			}
		else
			{
			result= KBadParameter;
			iLog->Log( _L("<--The meta data field number you want to input is out of bound( 0 <= Id <= 19 ).User leaves here.") );
			User::Leave( KBadParameter );
			}
		}
	else
		{
		result = KBadParameter;
		iLog->Log( _L("Pass in bad parameter.User Leave here.") );
		User::Leave( KBadParameter );
		}
	iLog->Log( _L("<--Out of ContainerAppendLTDesCL.") );
	return result;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::ContainerAppendLTDesC8L
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::ContainerAppendLTDesC8L( CStifItemParser& aItem )
	{
	iLog->Log( _L("") );//A blank line makes the log file look better
	iLog->Log( _L("Entering ContainerAppendLTDesC8L-->") );
	TInt result( 0 );
	TInt fieldId;
	TInt NumOfFieldAppended( 0 );
	if( aItem.GetNextInt( NumOfFieldAppended ) == KErrNone )
		{
		if( ( 0 < NumOfFieldAppended ) && ( NumOfFieldAppended <= 19 ) )
			{
			for( TInt num = NumOfFieldAppended ; (num--)>0 ; )
				{
				TInt fieldTemp;
				TPtrC fieldContent;
				if( aItem.GetNextInt( fieldTemp ) == KErrNone )
					{
					if( ( 0 <= fieldTemp ) && ( fieldTemp <= 19 ) )
						{
						fieldId = fieldTemp;
						if( aItem.GetNextString( fieldContent ) == KErrNone )
							{
							iLog->Log( _L("Meta data field [%d] is being appended."),fieldId );
							iLog->Log( _L("Content of the just appended meta data field is [%S]."),fieldContent.Alloc() );
							TMetaDataFieldId tempFieldId = (TMetaDataFieldId)fieldId;							
							TBuf<20> fieldBuf16;
							fieldBuf16.Copy( fieldContent );
							TBuf8<40> fieldBuf8;
							CnvUtfConverter::ConvertFromUnicodeToUtf8( fieldBuf8,fieldBuf16 );
							//const_cast<CMetaDataFieldContainer*>(iContainer)->AppendL( tempFieldId,fieldBuf8  );	//TODO 
							TRAP( result,const_cast<CMetaDataFieldContainer*>(iContainer)->AppendL( tempFieldId,fieldBuf8  ) );
							if( result == KErrNone )
								{
								iLog->Log( _L("Succeeded to append meta data field [%d]."),fieldId );
								}
							else
								{
								iLog->Log( _L("Failed to append meta data field [%d].Error [%d] returns."),fieldId,result );
								}
							}
						else
							{
							result = KBadParameter;
							iLog->Log( _L("Pass in bad parameter.User Leaves here.") );
							User::Leave( KBadParameter );
							}
						}
					else
						{
						result= KBadParameter;
						iLog->Log( _L("<--The meta data field Id [%d] you input is out of bound( 0 <= Id <= 19 ).User leaves here."),fieldTemp );
						User::Leave( KBadParameter );
						}
					}
				else
					{
					result = KBadParameter;
					iLog->Log( _L("Pass in bad parameter.User Leaves here.") );
					User::Leave( KBadParameter );
					}
				}
			}
		else if( NumOfFieldAppended == 0 )
			{
			result = KErrNone;
			iLog->Log( _L("No meta data field is appended.") );
			}
		else
			{
			result= KBadParameter;
			iLog->Log( _L("<--The meta data field number you want to input is out of bound( 0 <= Id <= 19 ).User leaves here.") );
			User::Leave( KBadParameter );
			}
		}
	else
		{
		result = KBadParameter;
		iLog->Log( _L("Pass in bad parameter.User Leaves here.") );
		User::Leave( KBadParameter );
		}
	iLog->Log( _L("<--Out of ContainerAppendLTDesC8L.") );
	return result;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::ContainerAt
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::ContainerAt( CStifItemParser& aItem )
	{
	iLog->Log( _L("") );//A blank line makes the log file look better
	iLog->Log( _L("Entering ContainerAt-->") );
	TInt result( 0 );
	TInt position;
	TInt paraError( aItem.GetNextInt( position ) );	
	if( paraError == KErrNone )
		{
		while( paraError == KErrNone )
			{
			if( ( 0 <= position ) && ( position <= 19 )  )
				{
				TMetaDataFieldId tempFieldId;
				TPtrC content = iContainer->At( position,tempFieldId );
				if( content == KNullDesC() )
					{
					iLog->Log( _L("The content of the meta data field [%d] is NULL."),tempFieldId );
					}
				else
					{
					iLog->Log( _L("The content of the meta data field [%d] is [%S]."),tempFieldId,content.Alloc() );
					}
				result = KErrNone;
				paraError= aItem.GetNextInt( position );
				if( paraError != KErrNone )
					{
					iLog->Log( _L("Pass in no parameter.Get out of the while-loop-clause here.") );
					break;
					//User::Leave( KBadParameter );
					}
				}
			else
				{
				result = KBadParameter;
				iLog->Log( _L("Position must be between 0 and 19( 0<= position <= 19 ).User Leaves here.") );
				User::Leave( KBadParameter );
				}
			}
		}
	else
		{
		result = KBadParameter;
		iLog->Log( _L("Pass in no parameter.User Leaves here.") );
		User::Leave( KBadParameter );
		}
	iLog->Log( _L("<--Out of ContainerAt.") );
	return result;
	}
// -----------------------------------------------------------------------------
// CMetaDataDisplay::ContainerFieldIdAt
// CloseChunkData test method function.
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CMetaDataDisplay::ContainerFieldIdAt( CStifItemParser& aItem )
	{
	iLog->Log( _L("") );//A blank line makes the log file look better
	iLog->Log( _L("Entering ContainerFieldIdAt-->") );
	TInt result( 0 );
	TInt position;
	TInt paraError( aItem.GetNextInt( position ) );	
	if( paraError == KErrNone )
		{
		while( paraError == KErrNone )
			{
			if( ( 0 <= position ) && ( position <= 19 )  )
				{
				TMetaDataFieldId tempFieldId;
				iContainer->FieldIdAt( position,tempFieldId );
				iLog->Log( _L("The meta data field that the position [%d] asociates with is [%d]."),position,tempFieldId );
				result = KErrNone;
				paraError= aItem.GetNextInt( position );
				if( paraError != KErrNone )
					{
					iLog->Log( _L("Pass in no parameter.Get out of the while-loop-clause here.") );
					break;
					//User::Leave( KBadParameter );
					}
				}
			else
				{
				result = KBadParameter;
				iLog->Log( _L("Position must be between 0 and 19( 0<= position <= 19 ).User Leaves here.") );
				User::Leave( KBadParameter );
				}
			}
		}
	else
		{
		result = KBadParameter;
		iLog->Log( _L("Pass in no parameter.User Leaves here.") );
		User::Leave( KBadParameter );
		}
	iLog->Log( _L("<--Out of ContainerFieldIdAt.") );
	return result;
	}

// -----------------------------------------------------------------------------
// CMetaDataDisplay::?member_function
// ?implementation_description
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
/*
TInt CMetaDataDisplay::?member_function(
   CItemParser& aItem )
   {

   ?code

   }
*/

// ========================== OTHER EXPORTED FUNCTIONS =========================
// None

//  [End of File] - Do not remove
