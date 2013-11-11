/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: Helper class for unzipping KMZ file.
*
*
*/


#include "EPos_PosLmUnzipUtil.h"


const TInt KMaxDirName = 255;
_LIT(KExtractZipPath, "extracts\\");



void PosLmUnzipUtil::DoUnzipL(const TDesC& aPath) 
{
	RFs fs; 
	User::LeaveIfError(fs.Connect()); //Connect to file session
	CleanupClosePushL(fs);
	TFileName dirFileName(aPath);
	UnzipFileL(fs, &dirFileName);
	CleanupStack::PopAndDestroy(&fs);



}

void PosLmUnzipUtil::UnzipFileL(RFs& aFs, TFileName* aFileName) 
{
	CZipFile* zipFile = NULL;
	CZipFileMember* member = NULL;
	CZipFileMemberIterator* fileMembers = NULL;
	zipFile = CZipFile::NewL(aFs,*aFileName);
	CleanupStack::PushL(zipFile);
	User::LeaveIfError(zipFile->OpenL());
	fileMembers = zipFile->GetMembersL();
	CleanupStack::PushL(fileMembers);
	member = fileMembers->NextL();
	while (member) 
	{
	//You must take ownership of the member object created for you by NextL()!
	CleanupStack::PushL(member);
	ExtractFileL(aFs, member, zipFile, aFileName);
	
	CleanupStack::PopAndDestroy(); //member
	member = fileMembers->NextL();
	}
	zipFile->Close();
	CleanupStack::PopAndDestroy(2); //fileMembers, zipFile
}

void PosLmUnzipUtil::ExtractFileL(RFs& aFs, CZipFileMember* aMember, CZipFile* aZipFile, TFileName* aFileName) 
{
	TInt loop=0;
	HBufC* name = aMember->Name()->AllocLC();
	// Change any instances of '/' to '\' in zipped file paths
	while (loop < name->Length()) 
	{
		if ((*name)[loop] == '/') 
		{
			name->Des()[loop] = '\\';
		}
		loop++;
	}
	//set target path
	TBuf<KMaxDirName> privatePath;
	aFs.PrivatePath(privatePath);
	TFileName fn;
	fn.Append(privatePath);
	fn.Append(KExtractZipPath);
	fn.Append(*name);
	
	//create target path if not exist.
	TInt err = aFs.MkDirAll(fn);
	if (err != KErrNone && err != KErrAlreadyExists) 
	{
		User::Leave(err);
	}
	RFile expandedMember;
	User::LeaveIfError(expandedMember.Replace(aFs, fn, EFileShareAny|EFileWrite));
	CleanupClosePushL(expandedMember);
	RZipFileMemberReaderStream* fileStream; 
	
	// KCompressionMethodNotSupported is possible in decompressing file here
	User::LeaveIfError(aZipFile->GetInputStreamL(aMember, fileStream));
	CleanupStack::PushL(fileStream); 
	
	// Assume file contents are 8-bit data
	TUint32 size = aMember->UncompressedSize();
	HBufC8* bytes = HBufC8::NewLC(size);
	TPtr8 ptr = bytes->Des(); //Obtain a modifiable descriptor
	fileStream->Read(ptr, size); 
	// save the unzipped contents to file
	User::LeaveIfError(expandedMember.Write(ptr));
	expandedMember.Close();
	CleanupStack::PopAndDestroy(4); //bytes, fileStream, expandedMember, name
}

