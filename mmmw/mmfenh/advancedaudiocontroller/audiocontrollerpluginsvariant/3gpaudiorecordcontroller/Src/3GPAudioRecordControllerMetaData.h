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
* Description:  Definition of the 3GPAudioRecordControllerMetaData.
*
*/


#ifndef C3GPAUDIORECORDCONTROLLERMETADATA_H
#define C3GPAUDIORECORDCONTROLLERMETADATA_H

// INCLUDES
#include <e32base.h>
#include <mmfcontrollerframeworkbase.h>
#include <mmfcontroller.h>

// FORWARD DECLARATION
class C3GPAudioMetaDataComposer;
class C3GPAudioRecordControllerPlugin;

// CLASS DECLARATION

/**
*  This class implements 3GPAudioRecordControllerMetaData
*
*  @lib 3GPAudioRecordControllerPlugin.lib
*  @since 3.0
*/
class C3GPAudioRecordControllerMetaData : public CBase
    {
    public:

        /**
		* Two-phased constructor.
        */
		static C3GPAudioRecordControllerMetaData* NewL();

		/**
		* Destructor
		*/
		~C3GPAudioRecordControllerMetaData();

        /**
		* Add a MetaDataWrite Parser to the Parent
        * @param aParent C3GPAudioRecordControllerPlugin that own this object
		* @return void
		*/
        void AddMetaDataWriteParserL(C3GPAudioRecordControllerPlugin* aParent);

        /**
		* Add and remember meta data entry to be written later
		* @param aNewEntry details of the new entry to be added
		* @return void
		*/
		void AddMetaDataEntryL(const CMMFMetaDataEntry& aNewEntry);

        /**
		* Return a buffer that contain UDTA atom
		* @return HBufC8* new buffer that contains utf-8 format of value, or null if none
		*/
		HBufC8* GetUdtaAtomL();

    private:

        /**
        * 2nd phase constructor
        * @return void
        */
        void ConstructL();

    private:

        C3GPAudioMetaDataComposer* iMetaDataComposer;        
    };

#endif // C3GPAUDIORECORDCONTROLLERMETADATA_H

// End of File
