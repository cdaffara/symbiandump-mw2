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
* Description:  Implementation of testvideoplaybackplugin interface
*
*/

#ifndef _CTESTVIDEOPLAYBACKPLUGIN_H_
#define _CTESTVIDEOPLAYBACKPLUGIN_H_

//
//  INCLUDES
//
#include <e32base.h>

#include <mpxplaybackplugin.h>
#include <mpxplaybackpluginversion2.h>
#include <mpxplaybackengineobserver.h>
#include <mpxvideoplaybackdefs.h>

#include "mpxplaybackutilitytest.h"




#include <e32svr.h>
#include <stifparser.h>
#include <stiftestinterface.h>

#include <mpxmediageneraldefs.h>
#include <mpxcommandgeneraldefs.h>
#include <mmf/common/mmferrors.h>
#include <mpxplaybackpluginversion2.h>
#include <mpxmessagegeneraldefs.h>

#include <accpolnamevaluerecord.h>
#include <accconfigfileparser.h>
#include <acccongenericid.h>
#include <accpolnamevaluerecord.h>

#include <mpxvideoplaybackdefs.h>
#include "mpxmediavideodefs.h"

typedef struct
{
    TInt               iEvent;
    TInt               iData;
    TInt               iError;
} TTestVideoPlaybackCallbackEvent;

typedef CArrayPtrFlat<TTestVideoPlaybackCallbackEvent> CCallbackArray;

//
//  CLASS DECLARATION
//

/*
 *  CTestVideoPlaybackPlugin class
 *
 */

NONSHARABLE_CLASS( CTestVideoPlaybackPlugin ) : public CMPXPlaybackPluginVersion2
                                                
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
        static CTestVideoPlaybackPlugin* NewL(TAny* aInitParams);

        /*
         *  Destructor
         *  Destroy the object and release all memory objects
         */
        ~CTestVideoPlaybackPlugin();
        
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
        void InitStreamingL(const TDesC& aUri, const TDesC8& aType, TInt aAccessPoint, TInt aPosition);

        /**
        * Initializes a file handle for playback.
        *
        * @since S60 9.2
        * @param aFile file handle of a file
        * @param aAccessPoint the access point
        */
        void InitStreamingL(RFile& aFile, TInt aAccessPoint, TInt aPosition);
                  
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    * Initializes a file handle for playback.
    *
    * @since S60 9.2
    * @param aFile 64 bit file handle of a file
    * @param aAccessPoint the access point
    */
    void InitStreaming64L(RFile64& aFile, TInt aAccessPoint, TInt aPosition);

    /**
    * Initializes a song for playback.
    *
    * @since S60 9.2
    * @param aFile 64 bit file handle of a song
    */
    void Initialise64L(RFile64& aFile, TInt aPosition);
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API

    /**
    * Initializes a song for playback.
    *
    * @since S60 9.2
    * @param aSong the song path
    * @param aPosition the starting position
    */
    virtual void InitialiseWithPositionL(const TDesC& aSong, TInt aPosition = 0 );
    
    /**
    * Initializes a song for playback.
    *
    * @since S60 9.2
    * @param aFile file handle of a song
    * @param aPosition the starting position
    */
    virtual void InitialiseWithPositionL(RFile& aSong, TInt aPosition = 0);
    
    //runl for active object
    void RunL();
    
    void DoCancel();
    
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
        CTestVideoPlaybackPlugin();

        /*
         *  the second phase constructor ConstructL to safely construct things
         *  that can leave
         */
        void ConstructL();
        
        void AddCallbackEvent( TTestVideoPlaybackCallbackEvent* event );
        static TInt SendEvent( TAny* aPtr );        
        void DoSendEvent();

    private:

        HBufC*                        iClipName;
        RFs                           iFs;
        RFile                         iFile; 
        CIdle*                        iCallback; //active object
        CCallbackArray*               iEventArray;
};

#endif
