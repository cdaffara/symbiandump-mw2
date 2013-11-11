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
* Description:  Definition of the S60 Audio Client Stream Source
*
*/

#ifndef __CS60AUDIOSTREAMSOURCE_H__
#define __CS60AUDIOSTREAMSOURCE_H__

#include <e32std.h>
#include <e32base.h>
#include "MS60AudioStreamSourceObserver.h"
#include <mmfbase.h>
#include "S60AudioSource.h"
#include "S60ClientAudioBuffer.h"

//FORWARD DECLARATION
class CS60SourceEventDispatcher;
class CWriteRequest;

/**
*  This class defines the S60 audio stream source functions.
*
*  @lib S60AudioStreamSource.lib
*  @since 3.1
*/
class CS60AudioStreamSource: public CS60AudioBaseSource
	{

	public: // Constructors and destructor

	    /**
        * Factory function for creating the Stream Source object.
        * @since 3.1
        * @return pointer to CS60AudioStreamSource object
        */
		IMPORT_C static CS60AudioStreamSource* NewL( MS60AudioStreamSourceObserver& aObserver,
													 const TDesC8& aMimeType);

		/**
        *	Destructor
        */
		virtual ~CS60AudioStreamSource();

    public: // New functions

		/**
        * Write the specified buffer to source
        * @param aBuffer Buffer to be processed.
        * @since 3.1
        * @return One of the Standard Return Error Codes
        */
		IMPORT_C virtual TInt WriteAudioData(CClientAudioBuffer& aBuffer);

		/**
        * Sets the Size of the Source
        * @param aSize size to be set.
        * @return One of the Standard Return Error Codes
        * @since 3.1
        */
		IMPORT_C virtual TInt SetSize(TUint aSize);

		/**
        * Get the minimum size of buffer that can be processed.
        * @return One of the Standard Return Error Codes
        * @since 3.1
        */
		IMPORT_C virtual TInt GetMinimumBufferSize();

		/**
        * Gets the Size of the Source
        * @return The Size of the Source
        * @since 3.1
        */
		IMPORT_C virtual TUint GetSize();


		/**
        * Gets the Mimetype of the Source
        * @param aMimeType mimetype of the Source.
        * @since 3.1
        */
		IMPORT_C virtual void GetMimeType(TDes8& aMimeType);

		/**
        * Gets the Position in the Source
        * @return The Position in the Source
        * @since 3.1
        */
		IMPORT_C virtual TInt64 GetPosition();

		/**
        * Sets the Position in the Source
        * @param aPosition position to be set in the Source.
        * @return One of the Standard Return Error Codes
        * @since 3.1
        */
		IMPORT_C virtual TInt SetPosition(TInt64 aPosition);

	private:

		/**
        * Gets the RandomSeek Capability from the Source
        * @return ETrue/EFalse depending on the Support capability
        * @since 3.1
        */
		virtual TBool IsRandomSeekingSupported();

		/**
        * Gets the Seeking Capability from the Source
        * @return ETrue/EFalse depending on the Support capability
        * @since 3.1
        */
		virtual TBool IsSeekingSupported();


	private:

		/**
		* Private C++ constructor for this class.
        * @since 3.1
        * @return -
        */
		CS60AudioStreamSource();

		void ConstructL(MS60AudioStreamSourceObserver& aObserver, const TDesC8& aMimeType );

	private:

		void FindControllersL();

		TBool GetFreeRequestSlot(TInt& index);

	private: // Data

		// Array to hold list of controllers suitable to handle the requested format
		RMMFControllerImplInfoArray iControllers;
		HBufC8* iMimeType;
		TInt64 iPosition;
		CS60SourceEventDispatcher* iDispatcher;
		RPointerArray<CWriteRequest> iWriteRequests;
		TUint iSize;

	};

#endif // __CS60AUDIOSTREAMSOURCE_H__

//  End of File
