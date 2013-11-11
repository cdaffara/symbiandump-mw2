/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Definition of the 3GPAudioMetaDataComposer.
*
*/


#ifndef _3GPAUDIOMETADATACOMPOSER_H
#define _3GPAUDIOMETADATACOMPOSER_H

// INCLUDES
#include <e32base.h>
#include <mmfcontrollerframeworkbase.h>
#include <mmfmeta.h>


// CLASS DECLARATION

/**
*  This class implements 3GPAudioMetaDataComposer
*
*  @lib 3GPAudioRecordControllerPlugin.lib
*  @since 3.0
*/
class C3GPAudioMetaDataComposer : public CBase
    {
    public:

        /**
		* Two-phased constructor.
        *
        */
		IMPORT_C static C3GPAudioMetaDataComposer* NewL();

		/**
		* Destructor
		*/
		IMPORT_C ~C3GPAudioMetaDataComposer();

        /**
		* Add and remember meta data entry to be written later
		* @param aNewEntry details of the new entry to be added
		* @return void
		*/
		IMPORT_C void AddMetaDataEntryL(const CMMFMetaDataEntry& aNewEntry);

        /**
		* Return a buffer that contain UDTA atom
		* @return HBufC8* new buffer that contains utf-8 format of value, or null if none
		*/
		IMPORT_C HBufC8* GetUdtaAtomL();

    private:

        /**
        * 2nd phase constructor
        * @return void
        */
        void ConstructL();

        /**
        * Convert a meta data value to utf-8 format buffer
        * @param aBuffer contain utf-8 format of the value on return
        * @param aValue value to be added
        * @return void
        */
        void DoAddMetaDataL(HBufC8*& aBuffer, const TDesC& aValue);

        /**
        * Append a size to a given buffer
        * @param aBuffer buffer to be appended to
        * @param aSize size to be appended
        * @return void
        */
        void AppendSize(TDes8& aBuffer, TInt aSize);

        /**
        * Makeup a meta data box and append it to a given buffer
        * @param aBuffer buffer to be appended to
        * @param aName name to be appended
        * @param aValue value to be appended
        * @return void
        */
        void AppendMetaDataL(TDes8& aBuffer, const TDesC8& aName, const TDesC8& aValue);

    private:
        HBufC8* iSongTitle;
        HBufC8* iArtist;
        HBufC8* iAlbum;
        HBufC8* iGenre;
        HBufC8* iComposer;
    };

#endif // _3GPAUDIOMETADATACOMPOSER_H

// End of File
