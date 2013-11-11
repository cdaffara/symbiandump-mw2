// Copyright (c) 2003-2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <caf/cafplatform.h>
#include <caf/dirstreamable.h>
#include "testagentmanager.h"
#include "testagentattributes.h"
#include "TestAgentFile.h"
#include "TestAgentDrmContent.h"
#include <stringpool.h>
#include <http.h>
#include <http/rhttpheaders.h>
#include <stringpool.h>
using namespace ContentAccess;

_LIT(KDrmContentExtension,".DRM");
_LIT(KFakePrivateDirectory, "C:\\tcaf\\testdata\\TestAgentPrivateDir\\");
_LIT(KAgentPrivateDirectory, "c:\\private\\12345678\\");
_LIT(KDrmMimeType, "application/testagent.drm");

CTestAgentManager* CTestAgentManager::NewL()
	{
	CTestAgentManager* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CTestAgentManager* CTestAgentManager::NewLC()
	{
	CTestAgentManager* self=new(ELeave) CTestAgentManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CTestAgentManager::CTestAgentManager()
	{
	}

CTestAgentManager::~CTestAgentManager()
	{
	delete iFileMan;
	iFs.Close();
	}
  
void CTestAgentManager::ConstructL()
	{
	User::LeaveIfError(iFs.Connect());
	iFileMan = CFileMan::NewL(iFs);
	}


TInt CTestAgentManager::DeleteFile(const TDesC &aFileName)	
	{
	// Most likely would have some kind of rights check here to determine if it can be
	// deleted
	return iFs.Delete(aFileName);
	}

TInt CTestAgentManager::CopyFile(const TDesC& /*aSource*/, const TDesC& /*aDestination*/)
	{
	return KErrCANotSupported;
	}

TInt CTestAgentManager::RenameFile(const TDesC& /*aSource*/, const TDesC& /*aDestination*/)
	{
	return KErrCANotSupported;
	}

TInt CTestAgentManager::MkDir(const TDesC& /*aPath*/)
	{
	return KErrCANotSupported;
	}

TInt CTestAgentManager::MkDirAll(const TDesC& /*aPath*/)
	{
	return KErrCANotSupported;
	}	

TInt CTestAgentManager::RmDir(const TDesC& /*aPath*/)
	{
	return KErrCANotSupported;
	}	

TInt CTestAgentManager::GetDir(const TDesC& aName,TUint anEntryAttMask,TUint anEntrySortKey, CDir*& anEntryList) const 
	{
	// Setting opaque_data="12345678" in the Test Agent resource file
	// indicates to CAF that test agent looks after this directory
	// So check that CAF is asking for
	// "c:\\private\\12345678\\"
	if(aName != KAgentPrivateDirectory())
		{
		return KErrPathNotFound;
		}

	// We will instead look at another directory just so the test works	
	return iFs.GetDir(KFakePrivateDirectory(), anEntryAttMask, anEntrySortKey, anEntryList);
	}

TInt CTestAgentManager::GetDir(const TDesC& aName,TUint anEntryAttMask,TUint anEntrySortKey, CDir*& anEntryList,CDir*& aDirList) const
	{
	// Setting opaque_data="12345678" in the Test Agent resource file
	// indicates to CAF that test agent looks after this directory
	// So check that CAF is asking for
	// "c:\\private\\12345678\\"
	if(aName != KAgentPrivateDirectory())
		{
		return KErrPathNotFound;
		}

	// We will instead look at another directory just so the test works
	return iFs.GetDir(KFakePrivateDirectory(), anEntryAttMask, anEntrySortKey, anEntryList, aDirList);
	}

TInt CTestAgentManager::GetDir(const TDesC& aName,const TUidType& anEntryUid,TUint anEntrySortKey, CDir*& aFileList) const
	{
	// Setting opaque_data="12345678" in the Test Agent resource file
	// indicates to CAF that test agent looks after this directory
	// So check that CAF is asking for
	// "c:\\private\\12345678\\"
	if(aName != KAgentPrivateDirectory())
		{
		return KErrPathNotFound;
		}
	
	// We will instead look at another directory just so the test works	
	return iFs.GetDir( KFakePrivateDirectory(), anEntryUid, anEntrySortKey, aFileList);
	}

TInt CTestAgentManager::GetAttribute(TInt aAttribute, TInt& aValue, const TVirtualPathPtr& aVirtualPath)
	{
	TRAPD(leaveValue, DoGetAttributeL(aAttribute, aValue, aVirtualPath));
	return leaveValue;
	}

TInt CTestAgentManager::DoGetAttributeL(TInt aAttribute, TInt& aValue, const TVirtualPathPtr& aVirtualPath)
	{
	TInt err = KErrNone;
	RFile file;
	CTestAgentFileInterface *fileObject = NULL;

	// check virtual path here to see it's ok
	if(TTestAgentAttributes::CheckVirtualPath(aVirtualPath) != KErrNone)
		{
		return KErrNotFound;	
		}

	// open file in order to retrieve an attribute from it
	err = file.Open(iFs, aVirtualPath.URI(), EFileStream | EFileRead | EFileShareReadersOnly);
	if(err == KErrNone)
		{
		CleanupClosePushL(file);

		if(aVirtualPath.UniqueId().Length() == 0)
			{
			fileObject = CTestAgentFile::NewL(file);
			}
		else
			{
			fileObject = CTestAgentDrmContent::NewL(file);
			}
		CleanupStack::PushL(fileObject);

		err = fileObject->GetAttribute(aAttribute, aValue);
		CleanupStack::PopAndDestroy(2, &file);
		}
	return err;	
	}
	
TInt CTestAgentManager::GetAttributeSet(RAttributeSet& aAttributeSet, const TVirtualPathPtr& aVirtualPath)
	{
	TRAPD(leaveValue, DoGetAttributeSetL(aAttributeSet, aVirtualPath));
	return leaveValue;
	}

TInt CTestAgentManager::DoGetAttributeSetL(RAttributeSet& aAttributeSet, const TVirtualPathPtr& aVirtualPath)
	{
	TInt err = KErrNone;
	RFile file;
	CTestAgentFileInterface *fileObject = NULL;	

	// check virtual path here to see it's ok
	if(TTestAgentAttributes::CheckVirtualPath(aVirtualPath) != KErrNone)
		{
		return KErrNotFound;	
		}

	// open file in order to retrieve an attribute from it
	err = file.Open(iFs, aVirtualPath.URI(), EFileStream | EFileRead | EFileShareReadersOnly);
	if(err == KErrNone)
		{
		CleanupClosePushL(file);

		if(aVirtualPath.UniqueId().Length() == 0)
			{
			fileObject = CTestAgentFile::NewL(file);
			}
		else
			{
			fileObject = CTestAgentDrmContent::NewL(file);
			}
		CleanupStack::PushL(fileObject);
		err = TTestAgentAttributes::GetAttributeSet(*fileObject, aAttributeSet);
		CleanupStack::PopAndDestroy(2, &file);
		}
	return err;	
	}
	
TInt CTestAgentManager::GetStringAttributeSet(RStringAttributeSet& aAttributeSet, const TVirtualPathPtr& aVirtualPath)
	{
	TRAPD(leaveValue, DoGetStringAttributeSetL(aAttributeSet, aVirtualPath));
	return leaveValue;
	}
	
TInt CTestAgentManager::DoGetStringAttributeSetL(RStringAttributeSet& aAttributeSet, const TVirtualPathPtr& aVirtualPath)
	{
	TInt err = KErrNone;
	RFile file;
	CTestAgentFileInterface *fileObject = NULL;
	
	// check virtual path here to see it's ok
	if(TTestAgentAttributes::CheckVirtualPath(aVirtualPath) != KErrNone)
		{
		return KErrNotFound;	
		}

	// open file in order to retrieve an attribute from it
	err = file.Open(iFs, aVirtualPath.URI(), EFileStream | EFileRead | EFileShareReadersOnly);
	if(err == KErrNone)
		{
		CleanupClosePushL(file);

		if(aVirtualPath.UniqueId().Length() == 0)
			{
			fileObject = CTestAgentFile::NewL(file);
			}
		else
			{
			fileObject = CTestAgentDrmContent::NewL(file);
			}
		CleanupStack::PushL(fileObject);
		err = TTestAgentAttributes::GetStringAttributeSet(*fileObject, aAttributeSet);
		CleanupStack::PopAndDestroy(2, &file);
		}
	return err;	
	}

TInt CTestAgentManager::GetStringAttribute(TInt aAttribute, TDes& aValue, const TVirtualPathPtr& aVirtualPath)
	{
	TRAPD(leaveValue, DoGetStringAttributeL(aAttribute, aValue, aVirtualPath));
	return leaveValue;
	}
	
TInt CTestAgentManager::DoGetStringAttributeL(TInt aAttribute, TDes& aValue, const TVirtualPathPtr& aVirtualPath)
	{
	TInt err = KErrNone;
	RFile file;
	CTestAgentFileInterface *fileObject = NULL;
	
	// check virtual path here to see it's ok
	if(TTestAgentAttributes::CheckVirtualPath(aVirtualPath) != KErrNone)
		{
		return KErrNotFound;	
		}

	// open file in order to retrieve an attribute from it
	err = file.Open(iFs, aVirtualPath.URI(), EFileStream | EFileRead | EFileShareReadersOnly);
	if(err == KErrNone)
		{
		CleanupClosePushL(file);

		if(aVirtualPath.UniqueId().Length() == 0)
			{
			fileObject = CTestAgentFile::NewL(file);
			}
		else
			{
			fileObject = CTestAgentDrmContent::NewL(file);
			}
		CleanupStack::PushL(fileObject);

		err = fileObject->GetStringAttribute(aAttribute, aValue);
		CleanupStack::PopAndDestroy(2, &file);
		}
	return err;	
	}

void CTestAgentManager::NotifyStatusChange(const TDesC& /*aURI*/, TEventMask /*aMask*/, TRequestStatus& aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	User::RequestComplete(ptr, KErrCANotSupported);
	}

TInt CTestAgentManager::CancelNotifyStatusChange(const TDesC& /*aURI*/, TRequestStatus& /*aStatus*/)
	{
	return KErrCANotSupported;
	}

TInt CTestAgentManager::SetProperty(TAgentProperty /*aProperty*/, TInt /*aValue*/)
	{
	return KErrNone;
	}

void CTestAgentManager::DisplayInfoL(TDisplayInfo /*aInfo*/, const TVirtualPathPtr& /*aVirtualPath*/)
	{
	User::Leave(KErrCANotSupported);
	}	

TBool CTestAgentManager::IsRecognizedL(const TDesC& aUri, TContentShareMode /*aShareMode*/) const
	{
	// This is a test implementation of IsRecognizedL
	// For testing purposes it only recognizes DCF content files

	TInt result;
	
	// Convert URI to uppercase
	HBufC *uriUpper = aUri.AllocLC();
	TPtr uriPtr(uriUpper->Des());
	uriPtr.UpperCase();

	// Check that the file extension is .DRM
	TPtrC extension = uriPtr.Right(KDrmContentExtension().Length());
	result = extension.Compare(KDrmContentExtension);
	CleanupStack::PopAndDestroy(uriUpper);
	
	if(result == 0)
		{
		return ETrue;
		}
	else 
		{
		return EFalse;
		}
	}

TBool CTestAgentManager::IsRecognizedL(RFile& aFile) const
	{
	// This is a test implementation of IsRecognizedL
	// For testing purposes it only recognizes DCF content files

	TInt result;
	
	// Convert URI to uppercase
	HBufC *uriUpper = HBufC::NewLC(KMaxFileName);
	TPtr uriPtr(uriUpper->Des());
	aFile.Name(uriPtr);
	uriPtr.UpperCase();

	// Check that the file extension is .DRM
	TPtrC extension = uriPtr.Right(KDrmContentExtension().Length());
	result = extension.Compare(KDrmContentExtension);
	CleanupStack::PopAndDestroy(uriUpper);
	
	if(result == 0)
		{
		return ETrue;
		}
	else 
		{
		return EFalse;
		}
	}

TBool CTestAgentManager::RecognizeFileL(const TDesC& aFileName, const TDesC8& aBuffer, TDes8& aFileMimeType, TDes8& aContentMimeType) const
	{
	// This is a test implementation of DoRecognizeL
	// For testing purposes it only recognizes DCF content files
	TInt result;

	// Convert filename to uppercase
	HBufC *FileNameUpper = aFileName.AllocLC();
	TPtr FileNamePtr(FileNameUpper->Des());
	FileNamePtr.UpperCase();

	// Check filename extension
	TPtrC extension = FileNamePtr.Right(KDrmContentExtension().Length());
	result = extension.Compare(KDrmContentExtension);
	CleanupStack::PopAndDestroy(); // FileNameUpper

	if(result == 0)
		{
		// It's a .DRM file
		aFileMimeType.Copy(KDrmMimeType);
	
		if(aBuffer.Length() > 3)
			{
			// Find content type 
			// content-type length is a single byte, stored at offset 0 in the DRM file
			TUint8 length = aBuffer[0];

			// find out if the file has been processed through supplier
			// interface yet
			if(aBuffer[length + 1] == (TUint8)'T')
				{
				// content type always starts at offset 1 in the DCF file
				aContentMimeType.Copy(aBuffer.Mid(1,length));
				}
			}
		return ETrue;
		}
	else
		{
		return EFalse;
		}
	}


_LIT(KAgentSpecificCommandResponse, "output");

TInt CTestAgentManager::AgentSpecificCommand(TInt aCommand, const TDesC8& /*aInputBuffer*/, TDes8& aOutputBuffer)
	{
	if(aCommand == 1)
		{
		aOutputBuffer.Copy(KAgentSpecificCommandResponse());
		return KErrNone;
		}
	return KErrCANotSupported;
	}

void CTestAgentManager::AgentSpecificCommand(TInt aCommand, const TDesC8& aInputBuffer, TDes8& aOutputBuffer, TRequestStatus& aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	User::RequestComplete(ptr, AgentSpecificCommand(aCommand, aInputBuffer, aOutputBuffer));
	}

void CTestAgentManager::DisplayManagementInfoL()
	{
	User::Leave(KErrCANotSupported);
	}

void CTestAgentManager::PrepareHTTPRequestHeaders(RStringPool& aStringPool, RHTTPHeaders& aRequestHeaders) const
	{
	TRAP_IGNORE(DoPrepareHTTPRequestHeadersL(aStringPool, aRequestHeaders));
	}

void CTestAgentManager::DoPrepareHTTPRequestHeadersL(RStringPool& aStringPool, RHTTPHeaders& aRequestHeaders) const
	{
	TBuf8 <KMaxDataTypeLength> mimeType;
	mimeType.Copy(KDrmMimeType());
	RStringF valStr = aStringPool.OpenFStringL(mimeType);
	CleanupClosePushL(valStr);
	THTTPHdrVal val(valStr);
	aRequestHeaders.SetFieldL(aStringPool.StringF(HTTP::EAccept, RHTTPSession::GetTable()), val);
	CleanupStack::PopAndDestroy(&valStr);	
	}

