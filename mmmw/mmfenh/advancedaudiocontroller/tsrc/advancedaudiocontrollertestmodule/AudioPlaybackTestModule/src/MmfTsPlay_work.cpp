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
* Description:  mmftsplay test component
*
*/

#include <StifTestInterface.h>
#include "MmfTsPlay.h"
#include "SimpleSoundPlayer.h"
#include <badesca.h>
//#include "RecordFormats.h"

// Binh
TInt CMmfTsPlay::TestCasePlayFileMetaInfoL(CStifSectionParser *section , TTestResult &aResult, TBool aFileHandler)
	{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
		{
		CSimpleSoundPlayer *sndPlayer;
		TFileName FileName = FileNamePtr;
		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);

        if (aFileHandler)
        {
          	RFs	  fs;
	        RFile file;

        	User::LeaveIfError(fs.Connect());
	        fs.ShareProtected();

			User::LeaveIfError(file.Open( fs, FileName, EFileRead));
    		sndPlayer = CSimpleSoundPlayer::NewLC( file, TestModuleIf() , *iLogger);

    		fs.Close();
        }
        else
        {
    		sndPlayer = CSimpleSoundPlayer::NewLC( FileName, TestModuleIf() , *iLogger);
        }

        // Binh
    	CStifItemParser *item1 = section->GetItemLineL(KTagDontPlayAudioClip);
    	if (item1)
    	{
    	    sndPlayer->iDontPlayAudioClip = true;
            delete item1;
    	}

		// * ************************************************

		sndPlayer->metaInfo = true;
	/*	TInt NumMetaDatas;
		TInt MetaDataError = sndPlayer->GetNumberOfMetaDataEntries(NumMetaDatas);
		if ( MetaDataError )
			{	//Error getting meta data info
			}
		else
			{	//No error yet
			TInt currMetaDataIndex;
			for ( currMetaDataIndex=0 ; currMetaDataIndex < NumMetaDatas ; currMetaDataIndex++)
				{
				CMMFMetaDataEntry *currMetaData = sndPlayer->GetMetaDataEntryL(currMetaDataIndex);
				iLogger->Log(_L("MetaData[%d]: Name: [%S], Value: [%S]") , currMetaDataIndex , &(currMetaData->Name()) , &(currMetaData->Value()) );
				delete currMetaData;
				}
			} */

		// * ************************************************
		iLogger->Log(_L("Starting scheduler"));
		CActiveScheduler::Start();

		iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		aResult.iResult = sndPlayer->iFinalError;
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;
		}
	else
		{
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
		}
	}

#define USE_FILE_FORMATS
TInt CMmfTsPlay::TestCasePlayFileFormatsL(CStifSectionParser * section , TTestResult & aResult)
	{
#ifndef USE_FILE_FORMATS
	return KErrNotSupported;
#endif
#ifdef USE_FILE_FORMATS

	//=/TInt KErrRecFormats = -1130;
	//[*]TPtrC FileNamePtr;
	TInt Error=KErrNone;
	//[*]if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
	//[*]	{
	//[*]	TFileName FileName = FileNamePtr;
	//[*]	iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);
	//[*]	CSimpleSoundPlayer *sndPlayer = CSimpleSoundPlayer::NewLC( FileName, TestModuleIf() , *iLogger);

		// * ************************************************






		REComSession session;
		TRAPD(err,session = REComSession::OpenL());
		if (err != KErrNone)
			{
			iLogger->Log(_L("Error (%d) opening REComSession"),err);
			aResult.iResult = err;
			aResult.iResultDes.Copy( KTestCaseResultFail() );
			return KErrExecuted;
			}

		//-/for (TInt i=0; i<1; i++)
			//-/{
			//TUid id;
			//id.iUid = KUidInterfaceMMFController; //KMmfUidPluginInterfaceController; //KMmfUidPluginInterfaceFormatDecode; //KMmfUidPluginInterfaceFormatEncode
			//TUid id = KUidInterfaceMMFController;
			TBool UsingDefault;
			TUid id;
			id.iUid = GetIntL(section, KTagInterfaceId, UsingDefault, KUidInterfaceMMFController.iUid);
			RImplInfoPtrArray aArray;

			iLogger->Log(_L("Calling REComSession::ListImplementationsL()") );
			TRAPD(err2,REComSession::ListImplementationsL(id,aArray));
			if (err2 != KErrNone)
				{
				iLogger->Log(_L("Error (%d) getting implementations (ListImplementationsL)"),err2);
				aResult.iResult = err;
				aResult.iResultDes.Copy( KTestCaseResultFail() );
				return KErrExecuted;
				//return KErrRecFormats;
				}

			for (TInt j=0; j<aArray.Count() ; j++)
				{
				CMMFControllerImplementationInformation* controller = NULL;

				iLogger->Log(_L("Calling CMMFControllerImplementationInformation::NewL()"));
				TRAPD(err3, controller=CMMFControllerImplementationInformation::NewL(id));

				//TRAPD(err3,controller = CMMFControllerImplementationInformation::NewL( *(aArray[j]) )  );

				if (err3 != KErrNone)
					{
					iLogger->Log(_L("Error %d openning controller"),err3);
					aResult.iResult = Error = err3;
					break;
					//continue;
					}
				CleanupStack::PushL(controller);

				iLogger->Log(_L("Calling CMMFControllerImplementationInformation::PlayFormats()"));
				RMMFFormatImplInfoArray formats = controller->PlayFormats();
				for (TInt k = 0; k < formats.Count(); k++)
					{
					CMMFFormatImplementationInformation* info = formats[k];
					CleanupStack::PushL(info);

					for (TInt l = 0; l < (info->SupportedFileExtensions()).Count(); l++)
						{
						iLogger->Log(_L("Calling CMMFFormatImplementationInformation::SupportedFileExtensions()"));
						iLogger->Log(_L("Format supported: %S"),(info->SupportedFileExtensions())[l]);
						}
					CleanupStack::PopAndDestroy();
					}
				formats.Close();
				CleanupStack::PopAndDestroy(controller);
				CleanupStack::PopAndDestroy();
				}
			//-/}
		aArray.Close();

		session.Close();

		//return KErrNone;

		// * ************************************************
		//[*]iLogger->Log(_L("Starting scheduler"));
		//[*]CActiveScheduler::Start();

		//[*]iLogger->Log(_L("Returned from player, errorcode: %d"), sndPlayer->iFinalError );
		//[*]aResult.iResult = sndPlayer->iFinalError ;
		if (aResult.iResult)
			{
			iLogger->Log(_L("Test failed, error code %d"), aResult.iResult);
			aResult.iResultDes.Copy(KTestCaseResultFail());
			}
		else
			{
			iLogger->Log(_L("Test was successful"));
			aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
		//[*]CleanupStack::PopAndDestroy(sndPlayer);
		return KErrExecuted;
	//[*]	}
	//[*]else
	//[*]	{
	//[*]	//Falta el nombre del archivo a reproducir
	//[*]	aResult.iResultDes.Copy(KConfigInvalid());
	//[*]	aResult.iResult = KErrNotExecuted;
	//[*]	return KErrConfigInvalid;
	//[*]	}
#endif
	}

TInt CMmfTsPlay::GetIntL(CStifSectionParser *aSection, const TDesC &aTag, TBool &aUsingDefault, const TInt &aDefaultInt, TBool aContinue)
	{
	TInt ParseError=KErrNotFound;
	TInt TempDelay=0;
	TInt ReturnValue=aDefaultInt;

	CStifItemParser *item;
	if (aContinue)
		{
		item = aSection->GetNextItemLineL(aTag);
		}
	else
		{
		 item = aSection->GetItemLineL(aTag);
		}
	if (item)
		{
		CleanupStack::PushL(item);
		ParseError = item->GetInt(aTag, TempDelay) ;
		CleanupStack::PopAndDestroy(item);
		}

	if (ParseError)
		{	//Integer not found, using defaults
		aUsingDefault=ETrue;
		}
	else
		{	//Integer found.
		aUsingDefault=EFalse;
		ReturnValue=TempDelay;
		}

	if (ParseError) {iLogger->Log(_L("Error retrieving integer, [%S] not found"), &aTag);}
	iLogger->Log(_L("Integer read, value (%d)") , ReturnValue);
	return ReturnValue;
	}

// Binh
//  This test will compare the MetaData in the audio clip with the value read in from the cfg file.
TInt CMmfTsPlay::TestMetaDataDataL(CStifSectionParser* section ,
                                   TTestResult &aResult,
                                   TBool aFileHandler)
{
    TInt err = KErrNone;
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
	{
		TFileName FileName = FileNamePtr;
        CSimpleSoundPlayer *sndPlayer;

		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);

        if (aFileHandler)
        {
          	RFs	  fs;
	        RFile file;

        	User::LeaveIfError(fs.Connect());
	        fs.ShareProtected();

			User::LeaveIfError(file.Open( fs, FileName, EFileRead));
    		sndPlayer = CSimpleSoundPlayer::NewLC( file, TestModuleIf() , *iLogger);

    		fs.Close();
        }
        else
        {
    		sndPlayer = CSimpleSoundPlayer::NewLC( FileName, TestModuleIf() , *iLogger);
        }

		aResult.iResult = err;

    	CStifItemParser *item1 = section->GetItemLineL(KTagDontPlayAudioClip);
    	if (item1)
    	{
    	    sndPlayer->iDontPlayAudioClip = true;
            delete item1;
    	}

   		iLogger->Log(_L("Starting scheduler"));
    	CActiveScheduler::Start();

	    TInt count = 0;
	    sndPlayer->GetNumberOfMetaDataEntries(count);
	    // Populate MetaData fields from audio file.
   		DisplayFields(count, sndPlayer , EFalse);
    	CStifItemParser *item = section->GetItemLineL(KTagMetaDataName);
    	while (item)
    	{
            // read MetaData Tag Name from cfg file
      		TPtrC namePtr;
		    CleanupStack::PushL(item);
		    err = item->GetString(KTagMetaDataName, namePtr);
		    CleanupStack::PopAndDestroy(item);

		    if (err)
		    {
   		        // Metadata Tag name is missing
   				aResult.iResultDes.Copy(KConfigInvalid());
    			aResult.iResult = KErrNotExecuted;
   				err = KErrConfigInvalid;
   				break;
		    }

   		    TInt fieldId = GetFieldId(namePtr);

   		    // read MetaData Tag data from cfg file
    	    TPtrC dataPtr;
	        item = section->GetNextItemLineL(KTagMetaDataData);
	        if (!item)
   		    {
   		        // Metadata Tag data is missing
   				aResult.iResultDes.Copy(KConfigInvalid());
    			aResult.iResult = KErrNotExecuted;
   				err = KErrConfigInvalid;
   				break;
   		    }

   		    CleanupStack::PushL(item);
   		    TInt err1 = KErrNone;
    	    err1 = item->GetString(KTagMetaDataData, dataPtr);

		    if (err1)
		    {
   		        // Error reading Metadata Tag data
   				aResult.iResultDes.Copy(KConfigInvalid());
    			aResult.iResult = KErrNotExecuted;
   				err = KErrConfigInvalid;
   		        CleanupStack::PopAndDestroy(item);
   				break;
		    }

            // Build whole data string with spaces.
		    TBuf<250> dataString(dataPtr);
		    while (!err1)
		    {
		        err1 = item->GetNextString(dataPtr);
		        if (!err1)
		        {
		            dataString.Append(_L(" "));
		            dataString.Append(dataPtr);
		        }
		    }
	        CleanupStack::PopAndDestroy(item);

            // Compare data
            if (!iMetaDataField[fieldId])
            {
                // Tag does not exist in file
                iLogger->Log(_L("Tag %S does not exist in the file."), &iMetaDataFieldName[fieldId]);
    	    	aResult.iResultDes.Copy(KTestCaseResultFail());
    		    aResult.iResult = KErrExpectedValueDifferent;
    		    err = KErrExecuted;
    		    break;
            }
            else if (iMetaDataField[fieldId]->Compare(dataString))
            {
                // Test Case Failed: No match
                iLogger->Log(_L("Actual   %S = %S"), &iMetaDataFieldName[fieldId],
                                                     iMetaDataField[fieldId]);
                iLogger->Log(_L("Expected %S = %S"), &iMetaDataFieldName[fieldId],
                                                     &dataString);
		    	aResult.iResultDes.Copy(KTestCaseResultFail());
   			    aResult.iResult = KErrExpectedValueDifferent;
   			    err = KErrExecuted;
   			    break;
            }
            else
            {
                // Test Case Passed:  Match
			    iLogger->Log(_L("Test was successful"));
   				aResult.iResultDes.Copy(KTestCaseResultSuccess());
            }

	        item = section->GetNextItemLineL(KTagMetaDataName);
    	}

        // Clear metadata fields
        DeleteMetaDataFields();

		CleanupStack::PopAndDestroy(sndPlayer);

		return err;
	}
	else
	{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
	}
}

// Binh
TInt CMmfTsPlay::DisplayFields(TInt aEntryCount, CSimpleSoundPlayer* aSndPlayer, TBool aDisplay)
{
	TInt i;
	TInt j=0;
    CMMFMetaDataEntry*	metaDataPtr;

	for( i = 0; i < aEntryCount; i++)
	{
		TRAPD(err, metaDataPtr = aSndPlayer->GetMetaDataEntryL(i));

		if(err == KErrNone)
		{
    		TInt fieldId = GetFieldId(metaDataPtr->Name());
		    // in case file has duplicate field ID
		    if (iMetaDataField[fieldId])
		        delete iMetaDataField[fieldId];

			iMetaDataField[fieldId] = metaDataPtr->Value().AllocL();
			if (aDisplay)
			{
			    TBuf<50> fieldName = iMetaDataFieldName[fieldId];
			    fieldName.Append(_L("..."));
			    iLogger->Log(fieldName);
            	iLogger->Log(_L("%S"), iMetaDataField[fieldId]);
			}
			j++;
		}
		delete metaDataPtr;
		metaDataPtr = NULL;
	}

	if (j==aEntryCount)
		return KErrNone;
	else
		return KErrExpectedValueDifferent;
}

// Binh
TInt CMmfTsPlay::GetFieldId(const TDesC& aString)
{
    for(TInt i=1; i < KNumMetaDataField; i++)
    {
        if (!aString.Compare(iMetaDataFieldName[i]))
            return i;
    }

    return 0;
}

//Binh
void CMmfTsPlay::DeleteMetaDataFields()
{
    // Clear metadata fields
    for (TInt i=0; i < KNumMetaDataField; i++)
        if (iMetaDataField[i])
        {
            delete iMetaDataField[i];
            iMetaDataField[i] = NULL;
        }
}


// Binh
TInt CMmfTsPlay::TestMetaDataCountL(CStifSectionParser *section , TTestResult &aResult, TBool aFileHandler)
{
	TPtrC FileNamePtr;
	if ( !section->GetLine(KTagSoundFile, FileNamePtr, ENoTag) )
	{
		TFileName FileName = FileNamePtr;
        CSimpleSoundPlayer *sndPlayer;

		iLogger->Log(_L("Creating simple player with file [%S]"), &FileNamePtr);

        if (aFileHandler)
        {
          	RFs	  fs;
	        RFile file;

        	User::LeaveIfError(fs.Connect());
	        fs.ShareProtected();

			User::LeaveIfError(file.Open( fs, FileName, EFileRead));
    		sndPlayer = CSimpleSoundPlayer::NewLC( file, TestModuleIf() , *iLogger);

    		fs.Close();
        }
        else
        {
    		sndPlayer = CSimpleSoundPlayer::NewLC( FileName, TestModuleIf() , *iLogger);
        }

    	CStifItemParser *item1 = section->GetItemLineL(KTagDontPlayAudioClip);
    	if (item1)
    	{
    	    sndPlayer->iDontPlayAudioClip = true;
            delete item1;
    	}

   		iLogger->Log(_L("Starting scheduler"));
    	CActiveScheduler::Start();

		TInt entryCount = 0;
		TInt expectedEntryCount = 0;

		sndPlayer->GetNumberOfMetaDataEntries(entryCount);

		CStifItemParser *item = section->GetItemLineL(KTagEntriesCount);

		if (item)
		{
			CleanupStack::PushL(item);
			item->GetInt(KTagEntriesCount, expectedEntryCount);
			CleanupStack::PopAndDestroy(item);

			if (entryCount==expectedEntryCount)
			{
				iLogger->Log(_L("Test was successful"));
        		aResult.iResult = KErrNone ;
				aResult.iResultDes.Copy(KTestCaseResultSuccess());
			}
			else
			{
				iLogger->Log(_L("EntryCount is %d"), entryCount);
				iLogger->Log(_L("ExpectedEntryCount is %d"), expectedEntryCount);
				iLogger->Log(_L("ExpectedEntryCount is different from real metadata count"));
				aResult.iResultDes.Copy(KTestCaseResultFail());
				aResult.iResult = KErrExpectedValueDifferent;
			}
		}
		else
		{
			//The metaData count is missing
			aResult.iResultDes.Copy(KConfigInvalid());
			aResult.iResult = KErrNotExecuted;

    		CleanupStack::PopAndDestroy(sndPlayer);

			return KErrConfigInvalid;
		}

		CleanupStack::PopAndDestroy(sndPlayer);

		return KErrExecuted;
	}
	else
	{
		//The file name of the clip to play is missing
		aResult.iResultDes.Copy(KConfigInvalid());
		aResult.iResult = KErrNotExecuted;
		return KErrConfigInvalid;
	}
}




