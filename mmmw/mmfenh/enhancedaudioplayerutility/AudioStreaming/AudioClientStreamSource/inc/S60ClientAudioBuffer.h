/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Enhanced Audio Player Utiliy
*
*/



#ifndef CCLIENTAUDIOBUFFER_H
#define CCLIENTAUDIOBUFFER_H

// INCLUDES
#include <e32base.h>


// CLASS DECLARATION


class CClientAudioBuffer : public CBase
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        IMPORT_C static CClientAudioBuffer* NewL(TInt aBufferSize);

        /**
        * Destructor.
        */
        virtual ~CClientAudioBuffer();

        /**
        * Get the Pointer to the Buffer associated with the CClientAudioBuffer.
        * @since Series 60 3.1
        * @return Pointer to the Buffer Descriptor
        */
		IMPORT_C TPtr8& GetBufferPtr();

        /**
        * Gets the Last Buffer Status associated with the Buffer
        * @since Series 60 3.1
        * @return return the Last Buffer Status of the Buffer
        */
		IMPORT_C TBool IsLastBuffer();

        /**
        * Sets the Last Buffer status associated with the Client Buffer
        * @since Series 60 3.1
        * @param aStatus status of the Buffer
        */
		IMPORT_C void SetLastBuffer(TBool aStatus);
    private:

        /**
        * C++ default constructor.
        */
        CClientAudioBuffer();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL(TInt aBufferSize);

    private:

		HBufC8* iAudioBuffer;
		TPtr8 iAudioBufferDes;
		TBool iLastBuffer;
	};

#endif 		// CCLIENTAUDIOBUFFER_H

//  End of File