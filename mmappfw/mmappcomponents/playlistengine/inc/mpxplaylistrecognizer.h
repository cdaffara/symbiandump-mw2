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
* Description:  Recognizer for playlist files.
*
*/



#ifndef MPXPLAYLISTRECOGNIZER_H
#define MPXPLAYLISTRECOGNIZER_H

// INCLUDES
#include <e32base.h>
#include <apmrec.h>
#include <apmstd.h>

// CLASS DECLARATION
class CMPXPluginHandlerBase;

/**
*  Deprecated. replaced by playlist recognizer in mw/mmmw.
*
*  Concrete implementation of the recognizer API used to recognize playlist files.
*
*  This is used by the application framework, as well as CMPXPlaylistEngine to
*  determine if a given media is a supported playlist.
*
*  @lib mpxplaylisrecognizer.lib
*/
class CMPXPlaylistRecognizer :
    public CApaDataRecognizerType,
    public MMPXPluginHandlerObserver
	{
    public:  // Constructors and destructor

        /**
        * Two-phased constructor.
        *
        * @return constructed object
        */
        static CMPXPlaylistRecognizer* NewL();

        /**
        * Destructor.
        */
        virtual ~CMPXPlaylistRecognizer();

    private:

        /**
        * C++ default constructor.
        */
        CMPXPlaylistRecognizer();

        /**
        * By default Symbian 2nd phase constructor is private.
        */
        void ConstructL();

    private: // Functions from base class

        /**
        * From CApaDataRecognizerType
        * Returns one of the SupportedDataType
        * @param aIndex - depending on the file extension
        * @return Supported DataType
        */
        TDataType SupportedDataTypeL( TInt aIndex ) const;

        /**
        * From CApaDataRecognizerType
        * Returns the buffersize needed to scan
        * @return BufferSize if needed
        */
        TUint PreferredBufSize();

        /**
        * From CApaDataRecognizerType
        * Do the recognition of the file using the name of the
        * the file or the buffer to scan
        * @param aName    Name of the file
        * @param aBuffer  Buffer that shall be scanned for recognition
        */
        void DoRecognizeL( const TDesC& aName, const TDesC8& aBuffer );

        /*
        * @see MMPXPluginHandlerObserver
        */
        void HandlePluginHandlerEvent(TPluginHandlerEvents aEvent,
            const TUid& aPluginUid, TBool aLoaded, TInt aData);

    private:	// Data

        CMPXPluginHandlerBase* iPluginHandler;

	};


#endif	// MPXPLAYLISTRECOGNIZER_H
