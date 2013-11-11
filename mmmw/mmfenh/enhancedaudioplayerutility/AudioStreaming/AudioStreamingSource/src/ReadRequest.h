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
* Description:  Definition of the stream source reader active object class
*
*/


#ifndef CREADREQUEST_H
#define CREADREQUEST_H

// INCLUDES
#include <e32base.h>
#include "S60StreamingSource.h"

class CMMFBuffer;
class MDataSink;

/**
*  Reader class to provide an asynchronous jump to avoid re-entrant code.
*
*  @lib S60StreamingSource.lib
*  @since 3.1
*/
class CReadRequest : public CActive
    {
    public: // Constructors and destructor

        /**
        * Two-phased constructor.
        */
        static CReadRequest* NewL( CS60StreamingSource& aCallback, CMMFBuffer* aBuffer, MDataSink* aConsumer);

        /**
        * Destructor.
        */
        virtual ~CReadRequest();

        void HandleRequest();

    private:

        /**
        * C++ default constructor.
        */
        CReadRequest(CS60StreamingSource& aCallback, CMMFBuffer* aBuffer, MDataSink* aConsumer);

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

        void RunL();

        void DoCancel();

    public:
    	// next item
        TSglQueLink* iLink;

    private:
		// Object to handle request
		CS60StreamingSource& iCallback;
		// Buffer to be sent to server-side
		CMMFBuffer* iBuffer;
		// Object to callback when buffer is filled
		MDataSink* iConsumer;
	};

#endif 		// CREADREQUEST_H

//  End of File
