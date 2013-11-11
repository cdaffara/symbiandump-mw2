// Copyright (c) 2004-2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include <e32wins.h>

#include "TMsvServerStoreManager.h"
#include "MSVUTILS.H"
#include "MSVSERV.H"
#include "MSVPANIC.H"
#include "MSVAPI.H"
#include "msvindexadapter.h"
#include "msvpreferreddrivelist.h"

TMsvServerStoreManager::TMsvServerStoreManager() : iMsvServer(NULL),iFs(NULL)
	{
	}

void TMsvServerStoreManager::SetMsvServerAndFileSession(CMsvServer& aMsvServer,RFs& aFs)
	{
	iMsvServer = &aMsvServer;
	iFs = &aFs;
	}

/** Gets the filepath for the storage of binary files associated with the context

If the directory specified in the context does not exist, it is created.

@param aEntryId index id of the entry whose filepath is requested
@param aDirectory On return, contains the binary file directory path
@return KErrNone - success;
@return The server was unable to create the directory.- failure */
TInt TMsvServerStoreManager::GetFilePath(TMsvId aEntryId, TFileName& aDirectory) const
	{
	__ASSERT_ALWAYS(iMsvServer != NULL, PanicServer(EMsvServerNotValid));

	TMsvId owningService = KMsvNullIndexEntryId;
	GetOwningServiceAndFilePath(aEntryId,aDirectory,owningService);

	MsvUtils::ConstructEntryName(owningService, aEntryId, aDirectory, MsvUtils::EFolder);
	TInt error = iMsvServer->FileSession().MkDirAll(aDirectory);
	return (error==KErrAlreadyExists ? KErrNone : error);
	}

/** Gets the filestore name associated with the context

@param aEntryId Index id of the entry whose store filename is requested
@param aFileName On return, contains the store file name for the context
*/
void TMsvServerStoreManager::GetStoreFileName(TMsvId aEntryId, TFileName& aFileName) const
	{
	__ASSERT_ALWAYS(iMsvServer != NULL, PanicServer(EMsvServerNotValid));

	TMsvId owningService = KMsvNullIndexEntryId;
	GetOwningServiceAndFilePath(aEntryId,aFileName,owningService);

	MsvUtils::ConstructEntryName(owningService, aEntryId, aFileName, MsvUtils::EStore);
	}

void TMsvServerStoreManager::GetOwningServiceAndFilePath(TMsvId aEntryId,TDes& aFilePath,TMsvId& aOwningService) const
	{
	__ASSERT_ALWAYS(iMsvServer != NULL, PanicServer(EMsvServerNotValid));

#if (defined SYMBIAN_MSGS_ENHANCED_REMOVABLE_MEDIA_SUPPORT)
	//TUint driveId = MsvUtils::GetDriveId(aEntryId);
	TUint driveId = GetDriveId(aEntryId);
	TDriveNumber driveNum = EDriveC;
	CMsvPreferredDriveList::GetDriveList()->GetDriveNumber(driveId, driveNum);
	aFilePath.Zero();
	aFilePath.Append(TDriveUnit(driveNum).Name());
	TParse parse;
	parse.Set(iMsvServer->Context().MessageFolder(), NULL, NULL);
	aFilePath.Append(parse.Path());
#else
	aFilePath = iMsvServer->Context().MessageFolder();
#endif

	iMsvServer->IndexAdapter().OwningService(aEntryId, aOwningService);
	}

/** Checks if the filename length is greater than KMaxFileName(256)

If the filename is greater than KMaxFileName then it truncates the filename

@param aFilePath file path where the file is being created.
@param aFileName name of the file whose length if being checked.
@return ETrue if the filename is changed.
*/
TBool TMsvServerStoreManager::CheckFileNameLengthAndTrimL(const TDesC& aFilePath,TDes& aFileName)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

 	TParsePtrC filenamePPtr(aFileName);
 	TPtrC fileExtension(filenamePPtr.Ext());

#ifdef __WINS__
	TFileName winsFileName;
	TFileName mailStoreDrive;
	TDriveUnit drive(MessageServer::CurrentDriveL(*iFs));
	mailStoreDrive.Append(drive.Name());
	mailStoreDrive.Append(KPathDelimiter);
	MapEmulatedFileName(winsFileName, mailStoreDrive); // Ignoring return value
	TInt KEpocBasePathLen = winsFileName.Length();
	TInt maxFilenameLength = KMaxFileName-(aFilePath).Length()-fileExtension.Length()-KEpocBasePathLen;
#else
	TInt maxFilenameLength = KMaxFileName-(aFilePath).Length()-fileExtension.Length();
#endif
	TBool fileNameChanged = EFalse;
	// Truncate filename if too long.
	if (maxFilenameLength < aFileName.Length())
		{
		aFileName.SetLength(maxFilenameLength);
		aFileName.Append(fileExtension);

		fileNameChanged = ETrue;
		}
	return fileNameChanged;
	}

TBool TMsvServerStoreManager::GenerateFileNameIfExistsL(TDes& aFileName)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	TParse fileParser;
	User::LeaveIfError(fileParser.Set(aFileName, NULL, NULL));
	TInt fileNameGenerator = 1;
	TBool fileNameChanged = EFalse;
	CDir* dir;
	TInt error =  iFs->GetDir(fileParser.DriveAndPath(), KEntryAttNormal, ESortNone, dir);
	if (error==KErrNone)
		{
		CleanupStack::PushL(dir);
		TInt fileCount=dir->Count();
		while(fileCount--)
			{
			TBool open;
			TFileName filePath(aFileName);
			TInt error = iFs->IsFileOpen(aFileName, open);
			// file exists
			if (error==KErrNone)
				{
				TFileName tempFileName(fileParser.DriveAndPath());
				tempFileName.Append(fileParser.Name());
				tempFileName.AppendNum(fileNameGenerator++);
				tempFileName.Append(fileParser.Ext());
				aFileName = tempFileName;
				fileNameChanged = ETrue;
				}
			else if (error==KErrNotFound)
				{
				break;
				}
			else
				{
				User::Leave(error);
				}
			}
		CleanupStack::PopAndDestroy(dir);
		}
	return fileNameChanged;
	}

// from MMsvStoreManager
TBool TMsvServerStoreManager::CreateAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	return CreateAttachmentL(aEntryId, aFilename, aFile, *iFs);
	}

TBool TMsvServerStoreManager::CreateShareProtectedAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile)
	{
	RFs fs;
	User::LeaveIfError(fs.Connect());
	User::LeaveIfError(fs.ShareProtected());

	return CreateAttachmentL(aEntryId, aFilename, aFile, fs, ETrue);
	}

TBool TMsvServerStoreManager::CreateAttachmentL(TMsvId aEntryId, TDes& aFilename, RFile& aFile, RFs& aFs,TBool aSharedAccess)
	{
	TFileName filePath;
	GetFilePath(aEntryId,filePath);
	TBool fileNameChanged = EFalse;

	if(CheckFileNameLengthAndTrimL(filePath,(TDes&)aFilename))
		{
		fileNameChanged = ETrue;
		}
		
	//for handling TBuf desc overflow
	RBuf newFilePath;
	TInt error = KErrNone;
 	newFilePath.CreateL(KMaxPath);
	newFilePath.CleanupClosePushL(); 
	newFilePath.Copy(filePath);
	if(aFilename.Length()+filePath.Length()>KMaxFileName)
		{
	newFilePath.ReAllocL(aFilename.Length()+filePath.Length());	
		}
	newFilePath.Append(aFilename);
	if (aSharedAccess)
		{
		//Shared access for reading and writing
		error = aFile.Create(aFs,newFilePath,EFileShareAny|EFileWrite);		
		}
	else
		{
		error = aFile.Create(aFs,newFilePath,EFileWrite);	
		}
	
	if(error == KErrAlreadyExists)
		{
		if(GenerateFileNameIfExistsL(newFilePath))
			{			
			fileNameChanged = ETrue;	
			}
			if (aSharedAccess)
				{
				//Shared access for reading and writing
				User::LeaveIfError(aFile.Create(aFs,newFilePath, EFileShareAny|EFileWrite)); 
				}
			else
				{
			 	User::LeaveIfError(aFile.Create(aFs,newFilePath,EFileWrite));		
				}
			
			aFilename.Copy(newFilePath);
				
		}
	else 
	   	{
		User::LeaveIfError(error);
		// return if error = KErrNone
		aFilename.Copy(newFilePath);
		}	
	CleanupStack::PopAndDestroy(&newFilePath);//newFilePath destroy 
	return fileNameChanged;
	}

void TMsvServerStoreManager::ReplaceAttachmentForWriteL(TMsvId aEntryId, TDes& aFilename, RFile& aFile)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	TFileName filePath;
	GetFilePath(aEntryId, filePath);
	TParsePtrC fileName(aFilename);
	filePath.Append(fileName.NameAndExt());

	User::LeaveIfError(aFile.Replace(*iFs, filePath, EFileWrite));

	aFilename = filePath;
	}

void TMsvServerStoreManager::OpenAttachmentL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile)
	{
	__ASSERT_ALWAYS(iFs != NULL,PanicServer(EMsvFileSessionNotValid));

	TFileName filePath;
	GetFilePath(aEntryId,filePath);
	TParsePtrC fileName(aFilePath);
	filePath.Append(fileName.NameAndExt());

	User::LeaveIfError(aFile.Open(*iFs,filePath,EFileRead|EFileShareReadersOnly));
	}

void TMsvServerStoreManager::OpenAttachmentForWriteL(TMsvId aEntryId, const TDesC& aFilePath, RFile& aFile)
	{
	__ASSERT_ALWAYS(iFs != NULL,PanicServer(EMsvFileSessionNotValid));

	TFileName filePath;
	GetFilePath(aEntryId,filePath);
	TParsePtrC fileName(aFilePath);
	filePath.Append(fileName.NameAndExt());

	User::LeaveIfError(aFile.Open(*iFs,filePath,EFileWrite));
	}


TInt TMsvServerStoreManager::DeleteAttachment(TMsvId aEntryId, const TDesC& aFilePath)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	TFileName filePath;
	GetFilePath(aEntryId,filePath);
	TParsePtrC fileName(aFilePath);
	filePath.Append(fileName.NameAndExt());

	TInt error = iFs->Delete(filePath);

	if(error == KErrNotFound || error == KErrPathNotFound)
		return KErrNotFound;

	return error;
	}

TInt TMsvServerStoreManager::RenameAttachment(TMsvId aEntryId, const TDesC& aOldFilePath, const TDesC& aNewName)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	TFileName filePath;
	TInt err = GetFilePath(aEntryId, filePath);

	TParsePtrC fileToRename(aOldFilePath);
	if(err==KErrNone)
		{
		filePath.Append(fileToRename.NameAndExt());
		TParse newPath;
		err = newPath.Set(aNewName, &fileToRename.FullName(), NULL);
		if( err==KErrNone )
			{
			err = iFs->Rename(filePath, newPath.FullName());
			}
		}

	return err;
	}

TBool TMsvServerStoreManager::FileExistsL(const TDesC& aFilePath)
	{
	// Use the Att method to check if file exists as it does change the
	// file and is a light-weight call
	TUint attValue = 0;
	TInt err = iFs->Att(aFilePath, attValue);

	TBool fileExists = EFalse;
	if (err == KErrNotFound || err == KErrPathNotFound)
		{
		fileExists = EFalse;
		}
	else if (err == KErrNone)
		{
		fileExists = ETrue;
		}
	else
		{
		User::Leave(err);
		}

	return fileExists;
	}

void TMsvServerStoreManager::AttachmentFilePathL(TMsvId aEntryId, TDes& aFilePath)
	{
	TFileName filePath;
	User::LeaveIfError(GetFilePath(aEntryId, filePath));

	aFilePath = filePath;
	}

TInt TMsvServerStoreManager::OpenFileStoreForRead(TMsvId aEntryId, RFile& aFile)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	TFileName filePathAndName;
	GetStoreFileName(aEntryId,filePathAndName);

	TInt error = aFile.Open(*iFs,filePathAndName,EFileRead|EFileShareAny);
	if(error == KErrNotFound || error == KErrPathNotFound)
		return KErrNotFound;

	return error;
	}

void TMsvServerStoreManager::OpenTempStoreFileL(TMsvId aEntryId, RFile& aFile)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	TFileName tempFilePathAndName;
	GetStoreFileName(aEntryId,tempFilePathAndName);

	tempFilePathAndName.Append(KMsvUtilsNewExtension);
	
	TInt error= aFile.Replace(*iFs,tempFilePathAndName,EFileShareExclusive|EFileWrite);
	if (error==KErrPathNotFound)
		{
        //only create directory if not found
        error=iFs->MkDirAll(tempFilePathAndName);
        if (error==KErrNone)
            error=aFile.Replace(*iFs,tempFilePathAndName,EFileShareExclusive|EFileWrite);
		}
	User::LeaveIfError(error);
	
	}

void TMsvServerStoreManager::ReplaceFileStoreL(TMsvId aEntryId)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	TFileName filePathAndName;
	
	GetStoreFileName(aEntryId,filePathAndName);
	TFileName tempFilePathAndName(filePathAndName);
	
	tempFilePathAndName.Append(KMsvUtilsNewExtension);

	TInt error = iFs->Replace(tempFilePathAndName,filePathAndName);
	

	if(error !=KErrNone)
		{
		iFs->Delete(tempFilePathAndName);
		User::Leave(error);
		}
	}

void TMsvServerStoreManager::DeleteFileStoreL(TMsvId aEntryId)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));

	TFileName filePathAndName;
	GetStoreFileName(aEntryId,filePathAndName);

	TInt error = iFs->Delete(filePathAndName);
	if(error != KErrNone && error != KErrNotFound && error != KErrPathNotFound)
		User::Leave(error);

	// Delete the temporary store file if it exists
	filePathAndName.Append(KMsvUtilsNewExtension);
	error = iFs->Delete(filePathAndName);
	if(error != KErrNone && error != KErrNotFound && error != KErrPathNotFound)
		User::Leave(error);
	}

TBool TMsvServerStoreManager::FileStoreExistsL(TMsvId aEntryId) const
	{
	__ASSERT_ALWAYS(iMsvServer != NULL, PanicServer(EMsvServerNotValid));

	TMsvId owningService = KMsvNullIndexEntryId;
	TFileName filePathAndName;
	GetOwningServiceAndFilePath(aEntryId,filePathAndName,owningService);

	return User::LeaveIfError(MsvUtils::HasStore(iMsvServer->FileSession(),filePathAndName,owningService, aEntryId));
	}


/**
Gets the path for the file in message store for which the message Id is passed.
@param aBodyTextId The Id of the body text for which the path is required.
@param aFilepath The output parameter which contains filepath.
@return none.
*/
void TMsvServerStoreManager::BodyTextFilePathL(TMsvId aBodyTextId, TDes& aFilePath)
	{
	TFileName filePath;
	User::LeaveIfError(GetFilePath(aBodyTextId, filePath));
	aFilePath = filePath;
	}

/**
Opens the RFile for which the aFilepath is passed in read mode.
@param RFile   		Reference to RFile that needs to be opened.
@param aBodyTextId 	The Id of the bodytext.
@param aFilepath 	The path of the file to be opened.
@return none.
*/
void TMsvServerStoreManager::OpenBodyTextFileForReadL(RFile& aFile, TMsvId /*aBodyTextId*/, const TDesC& aFilePath)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));
	
	User::LeaveIfError(aFile.Open(*iFs, aFilePath, EFileRead));
	}

/**
Creates the RFile in the aFilepath is passed.
@param RFile     	Reference to RFile that will be created.
@param aBodyTextId 	The Id of the bodytext.
@return none.
*/
void TMsvServerStoreManager::CreatePlainTextFileL(RFile& aFile, TMsvId aBodyTextId)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));
	
	TFileName filePathAndName;
	GetFilePath(aBodyTextId, filePathAndName);
	
	TBuf<KFileNameFixedWidth> fileName;
	
	fileName.NumFixedWidth(aBodyTextId, EHex, KFileNameFixedWidth);
	filePathAndName.Append(fileName);
	
	TFileName tempFilePathAndName((filePathAndName));
	tempFilePathAndName.Append(KMsvUtilsNewExtension);
	User::LeaveIfError(aFile.Replace(*iFs, tempFilePathAndName, EFileShareExclusive|EFileWrite));
	}

/**
Deletes the RFile in the aFilepath passed.
@param aBodyTextId  The Id of the bodytext.
@return none.
*/	
void TMsvServerStoreManager::DeletePlainTextFileL(TMsvId aBodyTextId)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));
	TFileName filePathAndName;
	GetFilePath(aBodyTextId, filePathAndName);
	
	TBuf<KFileNameFixedWidth> fileName;
	
	fileName.NumFixedWidth(aBodyTextId, EHex, KFileNameFixedWidth);
	filePathAndName.Append(fileName);
	
	TFileName tempFilePathAndName((filePathAndName));
	tempFilePathAndName.Append(KMsvUtilsNewExtension);
	// Remove the temporary file if it is there.
	TInt err = iFs->Delete(tempFilePathAndName);
	
	if (err != KErrNone && err != KErrNotFound)
		{
		User::Leave(err);
		}
	// There is no temporary file, delete the body text file.
	if (err == KErrNotFound)
		{
		TInt error = iFs->Delete(filePathAndName);
		if (error != KErrNone && error != KErrNotFound && error != KErrPathNotFound)
			{
			User::Leave(error);
			}		
		}
	}

/**
Replaces a temporary RFile with a new RFile.
@param aBodyTextId  The Id of the bodytext.
@return none.
*/
void TMsvServerStoreManager::ReplacePlainTextFileL(TMsvId aBodyTextId)
	{
	__ASSERT_ALWAYS(iFs != NULL, PanicServer(EMsvFileSessionNotValid));
	TFileName filePathAndName;
	GetFilePath(aBodyTextId, filePathAndName);
	
	TBuf<KFileNameFixedWidth> fileName;
	
	fileName.NumFixedWidth(aBodyTextId, EHex, KFileNameFixedWidth);
	filePathAndName.Append(fileName);
	
	TFileName tempFilePathAndName((filePathAndName));
	tempFilePathAndName.Append(KMsvUtilsNewExtension);
	TInt err = iFs->Replace(tempFilePathAndName,filePathAndName);
	if(err != KErrNone)
		{
		iFs->Delete(tempFilePathAndName);
		User::Leave(err);
		}
	}





#if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
/**
 * LoadHeaderL()
 * 
 * @param aMtmId : a MTM id.
 * @param aEntryId: Metadata entry Id of the message body.
 * @param iFieldPairList: Array of header fields.
 */

void TMsvServerStoreManager::LoadHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, RPointerArray<CHeaderFields>& aFieldPairList)
	{
	iMsvServer->MessageDBAdapter().LoadHeaderEntryL(aMtmId,aEntryId,aFieldPairList);
	}
	
/**
 * DeleteHeaderL()
 *
 * 
 * @param aMtmId : a MTM id.
 * @param aEntryId: Metadata entry Id of the message body.
 */

void TMsvServerStoreManager::DeleteHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId)
	{
	iMsvServer->MessageDBAdapter().DeleteHeaderEntryL(aMtmId,aEntryId);	
	}


/**
 * UpdateHeaderL()
 *
 * 
 * @param aMtmId : a MTM id.
 * @param aEntryId: Metadata entry Id of the message body.
 * @param iFieldPairList: Array of header fields.
 */

void TMsvServerStoreManager::UpdateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList)
	{
	iMsvServer->MessageDBAdapter().UpdateHeaderEntryL(aMtmId,aEntryId,aFieldPairList);
	}

/**
 * CreateHeaderEntryL()
 *
 * 
 * @param aMtmId : a MTM id.
 * @param aEntryId: Metadata entry Id of the message body.
 * @param iFieldPairList: Array of header fields.
 */

void TMsvServerStoreManager::CreateHeaderEntryL(const TUid& aMtmId, TMsvId aEntryId, const RPointerArray<CHeaderFields>& aFieldPairList)
	{
	iMsvServer->MessageDBAdapter().CreateHeaderEntryL(aMtmId,aEntryId,aFieldPairList);
	}



TBool TMsvServerStoreManager::DoesAnyStoreExists(TMsvId aId, TUid aMtmId)
	{
	TBool isStoreExists = EFalse;
	TRAPD(err, isStoreExists = iMsvServer->MessageDBAdapter().DoesAnyStoreExistsL(aId, aMtmId));
	if(err)
		{
		return EFalse;
		}
	else
		{
		return isStoreExists;
		}

	}

TBool TMsvServerStoreManager::DoesHeaderTableExist(const TUid& aMtmId)
	{
	TBool doesTableExists = EFalse;
	TRAPD(err, doesTableExists = iMsvServer->MessageDBAdapter().IsHeaderTableExistsL(aMtmId));
	if(err)
		{
		return EFalse;
		}
	else
		{
		return doesTableExists;
		}

	}






#endif			// #if (defined SYMBIAN_MESSAGESTORE_HEADER_BODY_USING_SQLDB)
