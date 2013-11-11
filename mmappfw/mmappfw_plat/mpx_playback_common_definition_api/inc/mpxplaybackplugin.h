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
* Description:  Playback plugin interface
*
*/


#ifndef CMPXPLAYBACKPLUGIN_H
#define CMPXPLAYBACKPLUGIN_H

// INCLUDES
#include <ecom/ecom.h>
#include <mpxplaybackframeworkdefs.h>
#include <mpxattribute.h>
#include <mpxattributespecs.h>
#include <mpxcommand.h>

// FORWARD DECLARATIONS

class MMPXPlaybackPluginObserver;

// CONSTANTS
const TUid KMPXPlaybackInterfaceUid={0x101FFC05};

// CLASS DECLARATION

/**
*  Interface to play media.
*
*  @lib mpxcommon.lib
*/
NONSHARABLE_CLASS(CMPXPlaybackPlugin): public CBase
    {

public: // Constructors and destructor

    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @param aUid uid of interface implementation
    * @param aObs observer
    * @return object of constructed
    */
    inline static CMPXPlaybackPlugin* NewL(TUid aUid,
                                      MMPXPlaybackPluginObserver& aObs);

    /**
    * Two-phased constructor.
    *
    * @since S60 3.2.3
    * @param aUid uid of interface implementation
    * @return object of constructed
    */
    inline static CMPXPlaybackPlugin* NewL(TUid aUid);

    /**
    * Destructor.
    *
    * @since S60 3.2.3
    */
    inline virtual ~CMPXPlaybackPlugin();

public:

    /**
     * Implementation uid of plugin.
     *
     * @since S60 3.2.3
     */
    inline const TUid& Uid();

    /**
    * Set observer.
    *
    * @since S60 3.2.3
    * @param aObs observer
    */
    virtual inline void SetObserver(MMPXPlaybackPluginObserver& aObs);

    /**
    * Initializes a song for playback.
    *
    * @since S60 3.2.3
    * @param aSong the song path
    */
    virtual void InitialiseL(const TDesC& aSong)=0;

    /**
    * Initializes a song for playback.
    *
    * @since S60 3.2.3
    * @param aFile file handle of a song
    */
    virtual void InitialiseL(RFile& aSong)=0;

    /** 
    * Executes a command on the selected song.
    *
    * @since S60 3.2.3
    * @param aCmd a command
    * @param aData data (optional)
    * @deprecated Use method CMPXPlaybackPlugin::CommandL(
    *     CMPXCommand& aCmd) instead.
    */
    virtual void CommandL(TMPXPlaybackCommand aCmd, TInt aData=0)=0;

    /**
     * Executes a command.
     *
     * @since S60 3.2.3
     * @param aCmd a command
     */
     virtual void CommandL(CMPXCommand& /*aCmd*/)
         {/*Note: All clients need to implement this */}

    /**
    * Sets a property of the plugin.
    *
    * @since S60 3.2.3
    * @param aProperty a playback property
    * @param aValue the value of the property
    */
    virtual void SetL(TMPXPlaybackProperty aProperty, TInt aValue)=0;

    /** 
     * Gets a property of the plugin (async).
     *
     * Plugin has to implement either ValueL or PropertyL. But the plugin can not
     * implement both of the functions. Otherwise, there will be circular function call.
     *
     * @since S60 3.2.3
     * @param aProperty a property
     * @deprecated Use method CMPXPlaybackPlugin::PropertyL(
     *        TMPXPlaybackProperty aProperty) instead.
     * Note: please call PropertyL in the downloadproxy plugin.
     */
    virtual void ValueL(TMPXPlaybackProperty aProperty) const
         {
         // Call the propertyL of the plugin which does not implement the ValueL
         PropertyL(aProperty);
         }

     /**
      * Gets a property of the plugin (async).
      * Result will be called back in HandleProperty.
      *
      * @since S60 3.2.3
      * @param aProperty a property
      */
    virtual void PropertyL(TMPXPlaybackProperty aProperty) const
         {
         //Note: change into pure virtual when all plugins implement the api
         //Default map to ValueL of the plugin which does not implement the PropertyL
         ValueL(aProperty);
         }

    /**
    * Gets a list of sub players.
    *
    * @since S60 3.2.3
    * @return a list of names of sub players
    */
    virtual void SubPlayerNamesL()=0;

    /**
    * Select a sub player.
    *
    * @since S60 3.2.3
    * @param aIndex index to the sub player
    */
    virtual void SelectSubPlayerL(TInt aIndex)=0;

    /**
    * Returns current sub player name.
    *
    * @since S60 3.2.3
    * @return friendly name of the current the sub player
    */
    virtual const TDesC& SubPlayerName()=0;

    /**
    * Current sub player index.
    *
    * @since S60 3.2.3
    * @return index to the sub player
    */
    virtual TInt SubPlayerIndex() const=0;

    /**
    * Implement the version with CMPXAttributeSpecs instead.
    * This will be removed in a future release.
    *
    * Extended properties of the current file (async).
    *
    * @since S60 3.2.3
    * @param aAttrs attributes requested
    * @deprecated Use method CMPXPlaybackPlugin::MediaL(
    *       const TArray<TMPXAttribute>& aAttrs,
    *       CMPXAttributeSpecs* aSpecs ) instead.
    */
    virtual void MediaL(const TArray<TMPXAttribute>& aAttrs)=0;

    /**
    * Should make this pure virtual in the future when
    * the previous API is removed and all client has implement
    * this API.
    *
    * Extended properties of the current file (async).
    *
    * @since S60 3.2.3
    * @param aAttrs attributes requested
    * @param aSpecs attribute spec, ownership not passed
    */
    virtual void MediaL(
        const TArray<TMPXAttribute>& aAttrs,
        CMPXAttributeSpecs* /*aSpecs*/ )
        {
        MediaL( aAttrs );
        };

    /**
    * Cancel outstanding request.
    *
    * @since S60 3.2.3
    */
    virtual void CancelRequest()=0;

protected:
    MMPXPlaybackPluginObserver*  iObs; // Not owned

private: // Data

    // identifies the actual plugin instance
    TUid iDtorKey;
    TUid iUid; //Implementation uid of the plugin
    };

#include "mpxplaybackplugin.inl"

#endif      // CMPXPLAYBACKPLUGIN_H


