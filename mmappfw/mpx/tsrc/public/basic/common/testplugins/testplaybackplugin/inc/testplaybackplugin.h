/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of testplaybackplugin interface
*
*/

#ifndef _CTESTPLAYBACKPLUGIN_H_
#define _CTESTPLAYBACKPLUGIN_H_

//
//  INCLUDES
//
#include <mpxplaybackplugin.h>
#include <mpxplaybackpluginversion2.h>


//
//  CLASS DECLARATION
//

/*
 *  CTestPlaybackPlugin class
 *
 */

NONSHARABLE_CLASS( CTestPlaybackPlugin ) : public CMPXPlaybackPluginVersion2
{
    public:
        //
        //  Constructors and destructor
        //

        /*
         *  Two-phased constructor.
         *  @param aInitParams, initialization parameter
         *  @return a pointer to the created instance
         */
        static CTestPlaybackPlugin* NewL(TAny* aInitParams);

        /*
         *  Destructor
         *  Destroy the object and release all memory objects
         */
        ~CTestPlaybackPlugin();
        
        /*
         *  Returns the current file handle iFile
         */        
        RFile GetFileHandle();

        /**
        * Initializes a file for playback.
        *
        * @since S60 9.2
        * @param aUri URI of the item
        * @param aType the mime type of the item
        * @param aAccessPoint the access point
        */
        void InitStreamingL(const TDesC& aUri, const TDesC8& aType, TInt aAccessPoint);

        /**
        * Initializes a file handle for playback.
        *
        * @since S60 9.2
        * @param aFile file handle of a file
        * @param aAccessPoint the access point
        */
        void InitStreamingL(RFile& aFile, TInt aAccessPoint);
                  
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    * Initializes a file handle for playback.
    *
    * @since S60 9.2
    * @param aFile 64 bit file handle of a file
    * @param aAccessPoint the access point
    */
    void InitStreaming64L(RFile64& aFile, TInt aAccessPoint);

    /**
    * Initializes a song for playback.
    *
    * @since S60 9.2
    * @param aFile 64 bit file handle of a song
    */
    void Initialise64L(RFile64& aSong);
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

    private:
        //
        //  CMPXPlaybackPlugin Implementation
        //

        /*
         *  Set observer
         *
         *  @param aObs observer
         */
        void SetObserver( MMPXPlaybackPluginObserver& aObs );

        /*
         *  Initializes a clip for playback
         *  @param aSong the song path
         */
        void InitialiseL( const TDesC& aSong );

        /*
         *  Initializes a song for playback
         *  @param aFile file handle of a song
         */
        void InitialiseL( RFile& aFile );

        /*
         *  Executes a command on the selected song
         * @param aCmd a command
         * @param aData, data
         */
        void CommandL( CMPXCommand& aCmd );  
        
        // Supposedly being deprecated
        // We're forced to implement this as its declared as a 
        // pure virtual function by the MPX framework        
        void CommandL(TMPXPlaybackCommand aCmd, TInt aData=0);
        
        /*
         *  Sets a property of the plugin
         *  @param aProperty a property
         *  @param aValue the value of the setting
         */
        void SetL( TMPXPlaybackProperty aProperty , TInt aValue );

        /*
         *  Gets a property of the plugin (async)
         *  @param aProperty a property
         */
        void PropertyL( TMPXPlaybackProperty aProperty ) const;

        /*
         *  Gets a list of sub players
         *  @return a list of names of sub players
         */
        void SubPlayerNamesL();

        /*
         *  Select a sub player
         *  @param aIndex index to the sub player
         */
        void SelectSubPlayerL( TInt aIndex );

        /*
         *  Returns current sub player name
         *  @return friendly name of the current the sub player
         */
        const TDesC& SubPlayerName();

        /*
         *  Current sub player index
         *  @return index to the sub player
         */
        TInt SubPlayerIndex() const;

        /*
         *  Media properties of the current file (async)
         *  @param aAttrs attributes requested
         */
        void MediaL( const TArray<TMPXAttribute>& aAttrs );

        /*
         * Cancel async request
         */
        void CancelRequest();                          

    private:
        /*
         *  C++ default constructor
         */
        CTestPlaybackPlugin();

        /*
         *  the second phase constructor ConstructL to safely construct things
         *  that can leave
         */
        void ConstructL();

    private:

        HBufC*                        iClipName;
        RFs                           iFs;
        RFile                         iFile;         
};

#endif
