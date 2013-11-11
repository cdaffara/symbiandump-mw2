// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// Implementation of the CROCHCAFInterface class.
// 
//


#include "ROCHCAFInterface.h"

_LIT8(KXHyphen,"X-");
_LIT8(KContentHyphen,"Content-");
const TUint8 KImcvColon	= ':';

#if  _DEBUG
_LIT(KErrInvalidCafState,	"Invalid CAF State");
#endif

/**
Construct with callers file session handle.
c'tor
*/
EXPORT_C CROCHCAFInterface::CROCHCAFInterface(RFs& aFs) : iFsSession(aFs)
	{}

/**
d'tor
*/
EXPORT_C  CROCHCAFInterface::~CROCHCAFInterface()
	{
	delete iSupplier;
	delete iContentType;
	delete iMetaData;
	delete iImportFile;
	iAttachmentFile.Close();
	}
	

/**
Prior to StartProcessing() the CAF agent could provide an initial filename for the CAF session.
@param aAttachmentFileName Descriptor for writing CAF agent suggested filename.
@return KErrNone success or system error code.
@pre PrepareProcessingL() must be called prior to this call.
@panic imut 36
*/
EXPORT_C TInt CROCHCAFInterface::GetSuggestedAttachmentFileName(TDes& aAttachmentFileName) const
	{
	__ASSERT_DEBUG(iImportFile != NULL, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));
	return iImportFile->GetSuggestedOutputFileName(aAttachmentFileName);
	}


/**
Write a buffer of data to the CAF Agent.
@param aData Data to write to the CAF agent.
@return KErrNone success or system error code.
@pre StartProcessingL() must be called prior to this call
@post Generates 0-N receipt files under the current message attachment entry.
@panic imut 36
*/
EXPORT_C void CROCHCAFInterface::WriteDataL(const TDesC8& aData)
	{
	__ASSERT_DEBUG(iImportFile != NULL, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));
	User::LeaveIfError(iImportFile->WriteData(aData));
	}

/**
Add Mime header metadata to CAF class in preparation for PrepareProcessingL()
Metadata comprises a field and data pair. For example:
aField - "X-Oma-Drm-Separate-Delivery" aData - "12"
If field does not contain "X-" or "Content-" then the method returns without
adding them to the class.
@param aField - Mime header field
@param aData - Mime field's corresponding data
@panic imut 36
*/
EXPORT_C void CROCHCAFInterface::AddToMetaDataL(const TDesC8& aField, const TDesC8& aData)
	{
	if(aField.FindF(KContentHyphen) == KErrNone && aField.FindF(KXHyphen) == KErrNone)
		{		
		return;
		}
	if(!iMetaData)
		{		
		iMetaData = CMetaDataArray::NewL();
		}
	// Check we're not overwriting an existing one. Could show up a logic error.
	#if (defined _DEBUG)
	TPtrC8 des(iMetaData->SearchL(aField));
	__ASSERT_DEBUG(des.Length() == 0, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));	
	#endif
	iMetaData->AddL(aField,aData);
	}

/**
Add Mime header metadata to CAF class in preparation for PrepareProcessingL()
Parses a complete unfolded mime header line to extract fields and values
Format expected:
Fieldxxx: Valuexxx;ParamField=ParamValue
If line does not contain "X-" or "Content-" then the method returns without
adding data to the class.
@param aMimeLine - Mime header data line minus CRLF
@panic imut 36
*/
EXPORT_C void CROCHCAFInterface::AddToMetaDataL(const TDesC8& aMimeLine)
	{
	// Don't bother adding if it's neither of these
	// This means we won't add envelope stuff
	if(aMimeLine.FindF(KContentHyphen) == KErrNone && aMimeLine.FindF(KXHyphen) == KErrNone)
		{
		return;
		}
	if(!iMetaData)
		{		
		iMetaData = CMetaDataArray::NewL();
		}
	TInt offset;
	TPtrC8 field;
	TPtrC8 data;
	// Isolate the field and data pair separated by ':'	
	if((offset = aMimeLine.Locate(KImcvColon)) != KErrNotFound)
		{
		field.Set(aMimeLine.Left(++offset));
		// Remove leading whitespace
		while(aMimeLine[offset] == ' ')
			{
			offset++;
			}
		data.Set(aMimeLine.Mid(offset));
		}
	else
		{
		// No colon should never happen if mail is not corrupt
		__ASSERT_DEBUG(offset != KErrNotFound, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));
		return;
		}
	// Check we're not overwriting an existing one. Could show up a logic error.
	#if (defined _DEBUG)
	TPtrC8 des(iMetaData->SearchL(field));
	__ASSERT_DEBUG(des.Length() == 0, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));				
	#endif
	iMetaData->AddL(field,data);
	}

/**
Prepare the CROCHCAFInterface class for a CAF session.
@pre RegisterL() must be called prior to this call and success checked with a Registered() call.
@panic imut 36
*/
EXPORT_C void CROCHCAFInterface::PrepareProcessingL()
	{
	__ASSERT_DEBUG(iSupplier != NULL && iContentType != NULL && iMetaData != NULL && iImportFile == NULL, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));
	iImportFile = iSupplier->ImportFileL(iContentType->Des(),*iMetaData);// Prepare agent for writing data.
	Deregister();
	}
	
/**
Set the CAF class variables required to Start the CAF write session.
@param aDefaultAttachmentFileName Localised default attachment name.
@param aAttachmentFilePath If CAF agent requires extra files, this is the folder/attachment entry path.
@param aStartFile An open File handle for attachment write. Caller can close following this call.
@pre PrepareProcessingL() must be called prior to this call
@panic imut 36
*/
EXPORT_C void CROCHCAFInterface::StartProcessing(const TDesC& aDefaultAttachmentFileName,const TDesC& aAttachmentFilePath,RFile& aStartFile)
	{
	__ASSERT_DEBUG(iSupplier != NULL && iImportFile != NULL && iContentType == NULL && iMetaData == NULL && aStartFile.SubSessionHandle() != 0, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));
	iDefaultAttachmentFileName.Set(aDefaultAttachmentFileName);
	iAttachmentFilePath = aAttachmentFilePath;
	iAttachmentFile.Duplicate(aStartFile); // Caller can close their copy
	}
	
/**
Terminate CAF session.
Set the mime type to a unique CAF one if there are.
@panic imut 36
*/
EXPORT_C void CROCHCAFInterface::EndProcessingL()
	{
	__ASSERT_DEBUG(iImportFile != NULL, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));
	WriteDataCompleteL();

	delete iImportFile;
	iImportFile = NULL;
	}

/**
Attempt to register a mime content-type with a CAF agent.
Success can be checked by a subsequent call to Registered().
@param aMimeLine MIME Content-Type for possible interest by a CAF agent. For example - application/vnd.oma.drm.rights+xml
@panic imut 36
*/
EXPORT_C void CROCHCAFInterface::RegisterL(const TDesC8& aMimeLine)
	{
	__ASSERT_DEBUG(iContentType == NULL, User::Panic(KErrInvalidCafState, EPanicInvalidCafState));
	if(!iSupplier)
		{		
		iSupplier = CSupplier::NewL();
		}
	if(iSupplier->IsImportSupported(aMimeLine))
		{
		// CAF agent interested in the content-type
		// Use this buffer for storing the content-type.
		// We could extract it from the caf metadata later but this will be quicker
		iContentType = aMimeLine.AllocL();
		// REMOVE when CAF fixed
		iContentType->Des().LowerCase();
		// Construct the metadata array here but add to it later
		// Could contain stuff like "X-Oma-Drm-Separate-Delivery: 12"
		if(!iMetaData)
			{		
			iMetaData = CMetaDataArray::NewL();
			}
		}
	}

/**
Free resources allocated during CAF session
@pre RegisterL() must be called prior to this call.
@panic imut 36
*/
EXPORT_C void CROCHCAFInterface::Deregister()
	{
	__ASSERT_DEBUG(Registered(), User::Panic(KErrInvalidCafState, EPanicInvalidCafState));
	// Don't delete iSupplier. This is kept open to prevent reloading of DLLs
	delete iContentType;
	iContentType = NULL;
	delete iMetaData;
	iMetaData = NULL;
	}
		
/**
Check whether RegisterL() succeeded.
@return Registered status of the CAF instance.
*/
EXPORT_C TBool CROCHCAFInterface::Registered() const
	{
	return iContentType != NULL;
	}
	
/**
Retrieve processing status for the CAF session.
@return Processing status of the CAF instance
*/
EXPORT_C TBool CROCHCAFInterface::Processing() const
	{
	return iImportFile != NULL;
	}

//Tell the CAF agent this that the last data for the CAF session has been written.
void CROCHCAFInterface::WriteDataCompleteL()
	{
	User::LeaveIfError(iImportFile->WriteDataComplete());
	}

