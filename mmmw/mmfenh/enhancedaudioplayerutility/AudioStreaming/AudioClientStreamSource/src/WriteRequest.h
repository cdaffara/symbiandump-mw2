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
* Description:  Definition of the audio buffer write request active object class
*
*/



#ifndef CWRITEREQUEST_H
#define CWRITEREQUEST_H

// INCLUDES
#include <e32base.h>
#include "S60AudioClientStreamSource.h"


// CLASS DECLARATION

/**
*  S60 Audio Buffer class encapsulating a client audio buffer write request to the server-side data source.
*
*  @lib S60AudioClientStreamSource.lib
*  @since 3.1
*/
class CWriteRequest : public CActive
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CWriteRequest* NewL(TMMFMessageDestinationPckg aMessageHandler, RMMFController& aController,
        							 CS60SourceEventDispatcher& aCallback);

        /**
        * Destructor.
        */
        virtual ~CWriteRequest();

    public: // New functions

		/**
        * Write the specified buffer to source
        * @param aBuffer Buffer to be processed.
        * @since 3.1
        * @return One of the Standard Return Error Codes
        */
        void WriteAudioData(CClientAudioBuffer& aBuffer);

        /**
        * Returns if the WriteRequest is Free
        * @since Series 60 3.1
        * @return return the status of the WriteRequest
        */
        TBool IsFree();


    private:

        /**
        * C++ default constructor.
        */
        CWriteRequest(TMMFMessageDestinationPckg aMessageHandler, RMMFController& aController,
        							 CS60SourceEventDispatcher& aCallback);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        void RunL();

        void DoCancel();

    private:

		// Pointer to controller with custom command facility
		RMMFController& iController;
		// Message handler handle
		TMMFMessageDestinationPckg iMessageHandler;
		// Object to notify upon request completion
		CS60SourceEventDispatcher& iCallback;
		// Last buffer indicator to be sent to server-side
		TPckgBuf<TInt> iLastBufferPckg;
		// Buffer to be sent to server-side
		//TDesC8* iBuffer;
		CClientAudioBuffer* iClientBuffer;
		// Indicates whether the buffer has been processed
		TBool iProcessed;
		// Indicates whether this object is free to reuse
		TBool iFree;
	};

#endif 		// CWRITEREQUEST_H

//  End of File
