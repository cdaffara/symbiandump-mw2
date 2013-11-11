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
// RefTestAgentImport.cpp
// 
//

#include <f32file.h>
#include <caf/caferr.h>
#include "rtaImport.h"
#include "drmrights.h"
#include "drmrightsparser.h"
#include "rtarights.h"
#include "rightsobject.h"
#include "reftestagentconstants.h"
#include "rtapanic.h"

using namespace ReferenceTestAgent;

EXPORT_C CRefTestAgentImport* CRefTestAgentImport::NewL(const TDesC& aClientSuggestedFileName)
	{
	CRefTestAgentImport* self = new (ELeave) CRefTestAgentImport();
	CleanupStack::PushL(self);
	self->ConstructL(aClientSuggestedFileName);
	CleanupStack::Pop(self);
	return self;
	}

CRefTestAgentImport::CRefTestAgentImport() : iOutputFileOpen(EFalse),
	iRightsOffsetPckg(iRightsOffset), iRightsSizePckg(iRightsSize),
	iDrmFilesOffsetPckg(iDrmFilesOffset), iDrmFilesSizePckg(iDrmFilesSize)
	{
	// Set the length of the buffers to receive integers to be zero initially
	iRightsOffsetPckg.Zero();
	iRightsSizePckg.Zero();
	iDrmFilesOffsetPckg.Zero();
	iDrmFilesSizePckg.Zero();
	}

void CRefTestAgentImport::ConstructL(const TDesC& /*aClientSuggestedFileName*/)
	{
	iDrmRightsParser = CDrmRightsParser::NewL();

	}

CRefTestAgentImport::~CRefTestAgentImport()
	{
	delete iPreviousWriteBuffer;
	if(iOutputFileOpen)
		{
		iFile.Close();
		iOutputFileOpen = EFalse;
		}
	delete iDrmRightsParser;
	}

EXPORT_C void CRefTestAgentImport::WriteL(const TDesC8& aData)
	{
	TPtrC8 rightsOffsetFragment;
	TPtrC8 drmFilesOffsetFragment;
	TPtrC8 remainingContentFragment;
	TPtrC8 drmFilesSizeFragment;
	TPtrC8 remainingDrmFilesFragment;
	TPtrC8 rightsSizeFragment;
	TPtrC8 inputData(aData);
	
	if(aData.Length() > 0)
		{
		switch(iImportStage)
			{
		case EReceivingRightsOffset:
			//ASSERT_ALWAYS(iBytesReceived < iRightsOffsetPckg.MaxLength());
		
			// Grab the remaining bytes of the rights offset
			rightsOffsetFragment.Set(inputData.Left(iRightsOffsetPckg.MaxLength() - iRightsOffsetPckg.Length()));
			iRightsOffsetPckg.Append(rightsOffsetFragment);
			iBytesReceived += rightsOffsetFragment.Length();
			
			// See if we have received the entire iRightsOffset
			if(iRightsOffsetPckg.Length() == iRightsOffsetPckg.MaxLength())
				{
				// Continue processing any remaining data in the next stage
				// by recursively calling WriteData()
				inputData.Set(aData.Right(aData.Length() - rightsOffsetFragment.Length()));
				iImportStage = EReceivingDrmFilesOffset;
				WriteL(inputData);
				}
			break;

		case EReceivingDrmFilesOffset:
			//ASSERT_ALWAYS(iBytesReceived >= iRightsOffsetPckg.MaxLength());
			//ASSERT_ALWAYS(iBytesReceived < iRightsOffsetPckg.MaxLength() + iDrmFilesOffsetPckg.MaxLength());
		
			// Grab the remaining bytes of the rights offset
			drmFilesOffsetFragment.Set(inputData.Left(iDrmFilesOffsetPckg.MaxLength() - iDrmFilesOffsetPckg.Length()));
			iDrmFilesOffsetPckg.Append(drmFilesOffsetFragment);
			iBytesReceived += drmFilesOffsetFragment.Length();

			// See if we have received the entire iDrmFilesOffset
			if(iDrmFilesOffsetPckg.Length() == iDrmFilesOffsetPckg.MaxLength())
				{
				// Continue processing any remaining data in the next stage
				// by recursively calling WriteData()
				inputData.Set(aData.Right(aData.Length() - drmFilesOffsetFragment.Length()));
			
				if(iDrmFilesOffset == 0)
					{
					// the source only contains rights
					iImportStage = EReceivingRightsSize;
					}

				WriteL(inputData);
				}
			break;
		case EReceivingRightsSize:
			// Grab the remaining bytes of the rights offset
			rightsSizeFragment.Set(inputData.Left(iRightsSizePckg.MaxLength() - iRightsSizePckg.Length()));
			iRightsSizePckg.Append(rightsSizeFragment);
			iBytesReceived += rightsSizeFragment.Length();

			// See if we have received the entire iDrmFilesOffset
			if(iRightsSizePckg.Length() == iRightsSizePckg.MaxLength())
				{
				// Don't write the rights size to the output file
				// Rights are parsed not written to the file
			
				// Continue processing any remaining data in the next stage
				// by recursively calling WriteL()
				inputData.Set(aData.Right(aData.Length() - rightsSizeFragment.Length()));
			
				iImportStage = EReceivingRights;
				WriteL(inputData);
				}
			break;
		case EReceivingRights:
			if(iBytesReceived + inputData.Length() <= iRightsOffset + iRightsSize + iRightsSizePckg.MaxLength())
				{
				// Parse the Xml data as it's received
				iDrmRightsParser->ParseL(inputData);
				iBytesReceived += inputData.Length();
				}
			else
				{
				// unexpected data after the rights
				User::Leave(KErrCorrupt);		
				}
			
			// See if all the rights have been received
			if(iBytesReceived == iRightsOffset + iRightsSize + iRightsSizePckg.MaxLength())
				{
				iImportStage = EFinished;
				}
			break;
		
		case EFinished:
			// not expecting any more data
			User::Leave(KErrCorrupt);		
			break;
		default:
			User::Panic(KRtaPanicString(), ERtaPanicImportInvalidState);
			break;
			}
		}
	}


_LIT(KReceiptFileExtension, ".TXT");

EXPORT_C TInt CRefTestAgentImport::GetSuggestedOutputFileExtension(TDes& aFileExtension)
	{
	if(iImportStage != EFinished)
		{
		aFileExtension.Copy(KRtaExtensionContent());
		}
	else
		{
		aFileExtension.Copy(KReceiptFileExtension());
		}
	return KErrNone;
	}

EXPORT_C TInt CRefTestAgentImport::GetSuggestedOutputFileName(TDes& aFileName)
	{
	TInt err = KErrNone;
	CDrmRights* drmRights=NULL;
	TRAP(err, drmRights=&iDrmRightsParser->DrmRightsL());
	if(iImportStage != EFinished)
		{
		_LIT(KSampleName,"OUTPUT");
		aFileName.Copy(KSampleName());
		aFileName.Append(KRtaExtensionContent());
		}
	else if(err == KErrNone && drmRights->RightsCount() > 0)
		{
		aFileName.Copy(drmRights->Rights(0).Cid());
		aFileName.Append(KReceiptFileExtension());
		}
	else
		{
		err = KErrNotReady;
		}
	return err;
	}

EXPORT_C void CRefTestAgentImport::WriteCompleteL()
	{
	if(iImportStage != EFinished)
		{
		User::Leave(KErrCorrupt);
		}
	else if(iDrmRightsParser->DrmRightsL().RightsCount() > 0)
		{
		// Erase the client suggested filename from db before adding
		// the imported rights. This way the client who uses the
		// feature of DeleteFile(suggestedFileName) will not
		// touch other's rights. And clients who do not know
		// about this feature are not affected. They can just
		// keep using the default suggested filename and tests
		// continue to pass.
		RRtaRights rightsServer;
		User::LeaveIfError(rightsServer.Connect());
		CleanupClosePushL(rightsServer);
		CleanupStack::PopAndDestroy(&rightsServer);

		// This will start a loop where all the rights are processed
		// Through Continue with new output fileL
		User::Leave(KErrCANewFileHandleRequired);
		}
	}

EXPORT_C void CRefTestAgentImport::ProcessRightsL()
	{
	RRtaRights rightsServer;

	// Just process one rights object at a time
	if(iDrmRightsParser->DrmRightsL().RightsCount() > 0)
		{
		User::LeaveIfError(rightsServer.Connect());
		CleanupClosePushL(rightsServer);

		CRightsObject& ro = iDrmRightsParser->DrmRightsL().Rights(0);
		rightsServer.AddRightsL(ro);

		// Remove the rights object from our array
		iDrmRightsParser->DrmRightsL().Remove(0);
		CleanupStack::PopAndDestroy(&rightsServer);
		}
	else
		{
		User::Leave(KErrNotFound);
		}
	}

