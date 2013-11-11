// Copyright (c) 2004-2010 Nokia Corporation and/or its subsidiary(-ies).
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
#include <http.h>
#include <http/rhttpheaders.h>
#include <stringpool.h>

#include "Reftestagentmanager.h"
#include "rtaArchive.h"
#include "drmfilecontent.h"
#include "drmfiles.h"
#include "reftestagentconstants.h"
#include "rtarights.h"

using namespace ContentAccess;
using namespace ReferenceTestAgent;


CRefTestAgentManager* CRefTestAgentManager::NewL()
	{
	CRefTestAgentManager* self = NewLC();
	CleanupStack::Pop(self);
	return self;
	}

CRefTestAgentManager* CRefTestAgentManager::NewLC()
	{
	CRefTestAgentManager* self=new(ELeave) CRefTestAgentManager();
	CleanupStack::PushL(self);
	self->ConstructL();
	return self;
	}

CRefTestAgentManager::CRefTestAgentManager()
	{
	}

CRefTestAgentManager::~CRefTestAgentManager()
	{
	iManagerSession.Close();
	}
  
void CRefTestAgentManager::ConstructL()
	{
	User::LeaveIfError(iManagerSession.Connect());
	}

// DeleteFile API is for deleting files in RTA's private data cage.
// Willl return KErrArgument if call upon to delete a public file.

// A client uses this API as hint for RTA to delete rights
// objects. Parameter "aFileName" is the suggested output file
// given by the client at rights import time.
//
// Hence return KErrNone if:
// 1. File is a RTA private file and delete is successful.
// 2. There are rights associated with the filename.
TInt CRefTestAgentManager::DeleteFile(const TDesC& aFileName)	
	{
	TRAPD(err,DeleteFileL(aFileName));
	// Delete file.
	TInt ret = iManagerSession.DeleteFile(aFileName);

	// Merge return codes
	if ( (ret == KErrNotFound || ret == KErrArgument) && (err == KErrNone) )
		{
		ret = KErrNone;
		}
	return ret;
	}
	
void CRefTestAgentManager::DeleteFileL(const TDesC& aFileName)
	{
	// Delete rights.
    HBufC8* hbuf8 = HBufC8::NewLC( aFileName.Length() );
	hbuf8->Des().Copy(aFileName);

	RRtaRights rightsServer;
	User::LeaveIfError(rightsServer.Connect());
	CleanupClosePushL(rightsServer);
	rightsServer.DeleteRightsByKeyL(*hbuf8);
	CleanupStack::PopAndDestroy(&rightsServer);
	CleanupStack::PopAndDestroy(hbuf8);
	}

TInt CRefTestAgentManager::CopyFile(const TDesC& aSource, const TDesC& aDestination)
	{
	return iManagerSession.CopyFile(aSource, aDestination);
	}

TInt CRefTestAgentManager::CopyFile(RFile& aSource, const TDesC& aDestination)
	{
	return iManagerSession.CopyFile(aSource, aDestination);
	}

TInt CRefTestAgentManager::RenameFile(const TDesC& aSource, const TDesC& aDestination)
	{
	return iManagerSession.RenameFile(aSource, aDestination);
	}

TInt CRefTestAgentManager::MkDir(const TDesC& aPath)
	{
	return iManagerSession.MkDir(aPath);
	}

TInt CRefTestAgentManager::MkDirAll(const TDesC& aPath)
	{
	return iManagerSession.MkDirAll(aPath);
	}	

TInt CRefTestAgentManager::RmDir(const TDesC& aPath)
	{
	return iManagerSession.RmDir(aPath);
	}	

TInt CRefTestAgentManager::GetDir(const TDesC& aName,TUint aEntryAttMask,TUint aEntrySortKey, CDir*& aEntryList) const 
	{
	TInt ret=KErrNone;
	TRAPD(err,ret=iManagerSession.GetDirL(aName, aEntryAttMask, aEntrySortKey, aEntryList));
	if(err==KErrNone)
		return ret; 
	else
		return err;
	}

TInt CRefTestAgentManager::GetDir(const TDesC& aName,TUint aEntryAttMask,TUint aEntrySortKey, CDir*& aEntryList,CDir*& aDirList) const
	{
	TInt ret=KErrNone;
	TRAPD(err,ret=iManagerSession.GetDirL(aName, aEntryAttMask, aEntrySortKey, aEntryList, aDirList));
	if(err==KErrNone)
		return ret; 
	else
		return err;
	}

TInt CRefTestAgentManager::GetDir(const TDesC& aName, const TUidType& aEntryUid,TUint aEntrySortKey, CDir*& aFileList) const
	{
	TInt ret=KErrNone;
	TRAPD(err,ret=iManagerSession.GetDirL(aName, aEntryUid, aEntrySortKey, aFileList));
	if(err==KErrNone)
		return ret; 
	else
		return err;
	}

TInt CRefTestAgentManager::GetAttribute(TInt aAttribute, TInt& aValue, const TVirtualPathPtr& aVirtualPath)
	{
	return iManagerSession.GetAttribute(aVirtualPath, aAttribute, aValue);
	}

TInt CRefTestAgentManager::GetAttributeSet(RAttributeSet& aAttributeSet, const TVirtualPathPtr& aVirtualPath)
	{
	TInt ret=KErrNone;
	TRAPD(err,ret=iManagerSession.GetAttributeSetL(aVirtualPath, aAttributeSet));
	if(err==KErrNone)
		return ret; 
	else
		return err;
	}

TInt CRefTestAgentManager::GetStringAttribute(TInt aAttribute, TDes& aValue, const TVirtualPathPtr& aVirtualPath)
	{
	return iManagerSession.GetStringAttribute(aVirtualPath, aAttribute, aValue);
	}

TInt CRefTestAgentManager::GetStringAttributeSet(RStringAttributeSet& aAttributeSet, const TVirtualPathPtr& aVirtualPath)
	{
	TInt ret=KErrNone;
	TRAPD(err,ret=iManagerSession.GetStringAttributeSetL(aVirtualPath, aAttributeSet));
	if(err==KErrNone)
		return ret; 
	else
		return err;
	}

void CRefTestAgentManager::NotifyStatusChange(const TDesC& /*aURI*/, TEventMask /*aMask*/, TRequestStatus& aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	User::RequestComplete(ptr, KErrCANotSupported);
	}

TInt CRefTestAgentManager::CancelNotifyStatusChange(const TDesC& /*aURI*/, TRequestStatus& /*aStatus*/)
	{
	return KErrCANotSupported;
	}

TInt CRefTestAgentManager::SetProperty(TAgentProperty /*aProperty*/, TInt /*aValue*/)
	{
	return KErrNone;
	}

void CRefTestAgentManager::DisplayInfoL(TDisplayInfo /*aInfo*/, const TVirtualPathPtr& /*aVirtualPath*/)
	{
	User::Leave(KErrCANotSupported);
	}	

TBool CRefTestAgentManager::IsRecognizedL(const TDesC& aUri, TContentShareMode /*aShareMode*/) const
	{
	TBool result = EFalse;
	
	// Check that the file has content only, otherwise it should
	// be put through the supplier API before it can be used
	TPtrC extension(aUri.Right(KRtaExtensionContent().Length()));
	if(extension.CompareF(KRtaExtensionContent) == 0)
		{
		result = ETrue;
		}
	return result;
	}

TBool CRefTestAgentManager::IsRecognizedL(RFile& aFile) const
	{
	TBool result = EFalse;
	
	// Get the name of the file
	TFileName fileName;
	aFile.Name(fileName);

	// Check that the file has content only, otherwise it should
	// be put through the supplier API before it can be used
	TPtrC extension(fileName.Right(KRtaExtensionContent().Length()));
	if(extension.CompareF(KRtaExtensionContent) == 0)
		{
		result = ETrue;
		}
	return result;
	}

TBool CRefTestAgentManager::RecognizeFileL(const TDesC& aFileName, const TDesC8& aBuffer, TDes8& aFileMimeType, TDes8& aContentMimeType) const
	{
	TBool result = EFalse;
	
	// Check filename extension
	TPtrC extension(aFileName.Right(KRtaExtensionContent().Length()));
	if(extension.CompareF(KRtaExtensionContent) == 0)
		{
		// It's a content file ready for applications to read
		aFileMimeType.Copy(KRtaMimeContent());
		CRefTestAgentArchive::GetDefaultMimeTypeFromHeaderL(aBuffer, aContentMimeType);
		result = ETrue;	
		}
	else
		{
		extension.Set(aFileName.Right(KRtaExtensionContentRights().Length()));
		if(extension.CompareF(KRtaExtensionContentRights) == 0)
			{
			// It's a content and rights file ready for the supplier API
			aFileMimeType.Copy(KRtaMimeContentRights());
			result = ETrue;
			}
		else
			{
			extension.Set(aFileName.Right(KRtaExtensionRights().Length()));
			if(extension.CompareF(KRtaExtensionRights) == 0)
				{
				// It's a content and rights file ready for the supplier API
				aFileMimeType.Copy(KXmlRORights());
				result = ETrue;
				}
			}
		}
	return result;
	}

TInt CRefTestAgentManager::AgentSpecificCommand(TInt /*aCommand*/, const TDesC8& /*aInputBuffer*/, TDes8& /*aOutputBuffer*/)
	{
	return KErrCANotSupported;
	}

void CRefTestAgentManager::AgentSpecificCommand(TInt aCommand, const TDesC8& aInputBuffer, TDes8& aOutputBuffer, TRequestStatus& aStatus)
	{
	TRequestStatus *ptr = &aStatus;
	User::RequestComplete(ptr, AgentSpecificCommand(aCommand, aInputBuffer, aOutputBuffer));
	}

void CRefTestAgentManager::DisplayManagementInfoL()
	{
	User::Leave(KErrCANotSupported);
	}

void CRefTestAgentManager::PrepareHTTPRequestHeaders(RStringPool& aStringPool, RHTTPHeaders& aRequestHeaders) const
	{
	TRAP_IGNORE(PrepareHTTPRequestHeadersL(aStringPool,aRequestHeaders));
	}

void CRefTestAgentManager::PrepareHTTPRequestHeadersL(RStringPool& aStringPool, RHTTPHeaders& aRequestHeaders) const
	{
	// Add the accept header for the reference test agent
	TBuf8 <KMaxDataTypeLength> mimeType;

	// Add accept header for Content only files
	mimeType.Copy(KRtaMimeContent());
	RStringF valStr = aStringPool.OpenFStringL(mimeType);
	CleanupClosePushL(valStr);
	THTTPHdrVal val1(valStr);
	aRequestHeaders.SetFieldL(aStringPool.StringF(HTTP::EAccept, RHTTPSession::GetTable()), val1);
	CleanupStack::PopAndDestroy(&valStr);
	
	// Add accept header for content and rights files
	mimeType.Copy(KRtaMimeContentRights());
	valStr = aStringPool.OpenFStringL(mimeType);
	CleanupClosePushL(valStr);
	THTTPHdrVal val2(valStr);
	aRequestHeaders.SetFieldL(aStringPool.StringF(HTTP::EAccept, RHTTPSession::GetTable()), val2);
	CleanupStack::PopAndDestroy(&valStr);
	}
