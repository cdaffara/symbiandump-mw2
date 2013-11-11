/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This file contains the base class from which specific audio
*                play controllers are derived. This class encapsulates common
*                behavior for all audio play controllers.
*
*/


// INCLUDE FILES
#include "AdvancedAudioPlayController.h"
#include "AdvancedAudioResource.h"
#include <AudioOutput.h>
#include <MetaDataFieldContainer.h>
#include <MetaDataUtility.h>
#include <mmfformatimplementationuids.hrh>
#include <mmfmeta.h>
#include <MultimediaDataSourceEvents.h>
#include <MultimediaDataSourceFactory.h>
#include <oma2dcf.h>

// CONSTANTS

// ============================= LOCAL FUNCTIONS ===============================

// ============================ MEMBER FUNCTIONS ===============================


// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::GetNumberOfMetaDataEntriesL
// Returns the number of metadata entries found in source, if supported.
// -----------------------------------------------------------------------------
//
EXPORT_C void CAdvancedAudioPlayController::GetNumberOfMetaDataEntriesL(
	TInt& aNumberOfEntries)
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioPlayController::GetNumberOfMetaDataEntriesL"));
#endif

	if (!iMetaDataSupport)
		{
		User::Leave(KErrNotSupported);
		}

    if (!iMetaDataRead)
        {
		if (!iDataSource)
			{
			User::Leave(KErrNotReady);
			}

        CMetaDataUtility* metaDataUtility = CMetaDataUtility::NewL();
        CleanupStack::PushL(metaDataUtility);

		RArray<TMetaDataFieldId> wantedFields;
		CleanupClosePushL(wantedFields);
		wantedFields.Append(EMetaDataSongTitle);
		wantedFields.Append(EMetaDataArtist);
		wantedFields.Append(EMetaDataAlbum);
		wantedFields.Append(EMetaDataYear);
		wantedFields.Append(EMetaDataComment);
		wantedFields.Append(EMetaDataAlbumTrack);
		wantedFields.Append(EMetaDataGenre);
		wantedFields.Append(EMetaDataComposer);
		wantedFields.Append(EMetaDataCopyright);
		wantedFields.Append(EMetaDataOriginalArtist);
		wantedFields.Append(EMetaDataUrl);
		wantedFields.Append(EMetaDataJpeg);
		wantedFields.Append(EMetaDataUserUrl);

		if (iSourceType == KUidMmfFileSource ||
			 iSourceType == TUid::Uid(0x10207A7C)	) // progressive download
			{
			CMMFFile* file = static_cast<CMMFFile*>(iDataSource);
       	    file->SourcePrimeL();
			metaDataUtility->OpenFileL(file->FileL(), wantedFields);
			}
		else if(iSourceType == KUidMmfDescriptorSource)
			{
			CMMFClip* clip = static_cast<CMMFClip*>(iDataSource);
            CMMFDataBuffer* buf = CreateSourceBufferOfSizeLC(clip->Size());
            clip->SourcePrimeL();
            clip->ReadBufferL(buf, 0);
			metaDataUtility->OpenDesL(buf->Data(), wantedFields);
			CleanupStack::PopAndDestroy(buf);	// buf
			}
		else
			{
			CleanupStack::PopAndDestroy(&wantedFields);
			CleanupStack::PopAndDestroy(metaDataUtility);
			User::Leave(KErrNotSupported);
			}

        TInt count = metaDataUtility->MetaDataCount();

        if (count > 0)
			{
			const CMetaDataFieldContainer& container = metaDataUtility->MetaDataFieldsL();
			TMetaDataFieldId id;
			for (TInt i = 0; i < count; i++)
				{
				TPtrC content = container.At(i, id);
				CMMFMetaDataEntry* metaData = NULL;
				switch (id)
					{
					case EMetaDataSongTitle:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntrySongTitle, content);
						break;
					case EMetaDataArtist:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryArtist, content);
						break;
					case EMetaDataAlbum:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryAlbum, content);
						break;
					case EMetaDataYear:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryYear, content);
						break;
					case EMetaDataComment:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryComment, content);
						break;
					case EMetaDataAlbumTrack:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryAlbumTrack, content);
						break;
					case EMetaDataGenre:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryGenre, content);
						break;
					case EMetaDataComposer:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryComposer, content);
						break;
					case EMetaDataCopyright:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryCopyright, content);
						break;
					case EMetaDataOriginalArtist:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryOriginalArtist, content);
						break;
					case EMetaDataUrl:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryWOAF, content);
						break;
					case EMetaDataJpeg:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryAPIC, content);
						break;
					case EMetaDataUserUrl:
						metaData = CMMFMetaDataEntry::NewL(KMMFMetaEntryWXXX, content);
						break;
					default:	// Should never get here really...
						break;
					}
				if (metaData)
					{
					CleanupStack::PushL(metaData);
					User::LeaveIfError(iMetaDataEntries.Append(metaData));
					CleanupStack::Pop(metaData);	// metaData
					}
				}
			}
		iMetaDataRead = ETrue;
		CleanupStack::PopAndDestroy(&wantedFields);	// wantedFields
		CleanupStack::PopAndDestroy(metaDataUtility);	//metaDataUtility
		}

    aNumberOfEntries = iMetaDataEntries.Count();
    }

// -----------------------------------------------------------------------------
// CAdvancedAudioPlayController::GetMetaDataEntryL
// Returns the metadata in the specified index.
// -----------------------------------------------------------------------------
//
EXPORT_C CMMFMetaDataEntry* CAdvancedAudioPlayController::GetMetaDataEntryL(
	TInt aIndex)
    {
#ifdef _DEBUG
	RDebug::Print(_L("CAdvancedAudioPlayController::GetMetaDataEntryL [%d]"), aIndex);
#endif

	if (!iMetaDataSupport)
		{
		User::Leave(KErrNotSupported);
		}

    if (aIndex > iMetaDataEntries.Count() - 1)
    	{
        User::Leave(KErrArgument);
		}

    return CMMFMetaDataEntry::NewL(*iMetaDataEntries[aIndex]);
    }
    
// End of file
