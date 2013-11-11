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
* Description:  Playback Init Access Point interface
*
*/

 
#ifndef CMPXPLAYBACKPLUGINVERSION2_H
#define CMPXPLAYBACKPLUGINVERSION2_H

// INCLUDES

#include <mpxplaybackplugin.h>

/**
*  Interface for receiving property values asynchronously.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(CMPXPlaybackPluginVersion2): public CMPXPlaybackPlugin
    {
public:
    
    /**
    * Initializes a file for playback.
    *
    * @since S60 9.2
    * @param aUri URI of the item
    * @param aType the mime type of the item
    * @param aAccessPoint the access point
    * @param aPosition the starting position
    */
    virtual void InitStreamingL(const TDesC& aUri, const TDesC8& aType, TInt aAccessPoint, TInt aPosition = 0) = 0; 

    /**
    * Initializes a file handle for playback.
    *
    * @since S60 9.2
    * @param aFile file handle of a file
    * @param aAccessPoint the access point
    * @param aPosition the starting position
    */
    virtual void InitStreamingL(RFile& aFile, TInt aAccessPoint, TInt aPosition = 0) = 0; 
    
#ifdef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    /**
    * Initializes a file handle for playback.
    *
    * @since S60 9.2
    * @param aFile 64 bit file handle of a file
    * @param aAccessPoint the access point
    * @param aPosition the starting position
    */
    virtual void InitStreaming64L(RFile64& aFile, TInt aAccessPoint, TInt aPosition = 0) = 0; 

    /**
    * Initializes a song for playback.
    *
    * @since S60 9.2
    * @param aFile 64 bit file handle of a song
    * @param aPosition the starting position
    */
    virtual void Initialise64L(RFile64& aSong, TInt aPosition = 0) = 0; 
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    

    /**
    * Initializes a song for playback.
    *
    * @since S60 9.2
    * @param aSong the song path
    * @param aPosition the starting position
    */
    virtual void InitialiseWithPositionL(const TDesC& aSong, TInt aPosition = 0) = 0;
    
    /**
    * Initializes a song for playback.
    *
    * @since S60 9.2
    * @param aFile file handle of a song
    * @param aPosition the starting position
    */
    virtual void InitialiseWithPositionL(RFile& aSong, TInt aPosition = 0) = 0;
    
    };
    
#endif // CMPXPLAYBACKPLUGINVERSION2_H
