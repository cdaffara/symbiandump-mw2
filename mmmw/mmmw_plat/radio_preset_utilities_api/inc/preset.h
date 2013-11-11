/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Preset base class.
*
*
*/


#ifndef TPRESET_H
#define TPRESET_H

#include <e32std.h>
#include <babitflags.h>         // For TBitFlags

// Forward declarations
class RWriteStream;
class RReadStream;

const int KPresetNameLength = 32;
typedef TBuf<KPresetNameLength> TPresetName;
const int KUrlMaxLength = 50;
typedef TBuf<KUrlMaxLength> TRadioUrl;

/**
 *  define preset structure.
 */
class TPreset
    {
public:

    /**
    * Default class constructor.
    */
    IMPORT_C TPreset();

    /**
    * Gives the preset name.
    * @since Series 60 10.1
    * @return the name of the preset
    */
    IMPORT_C TPtrC Name() const;

    /**
    * Gives the preset frequency
    * @since Series 60 10.1
    * @return the frequency
    */
    IMPORT_C TUint Frequency() const;

    /**
    * Set name.
    * @since Series 60 10.1
    * @param aStationName The name to be set.
    * @return none
    */
    IMPORT_C void SetName( const TPresetName& aStationName );

    /**
    * Set the frequency value.
    * @since Series 60 10.1
    * @param aFrequency  The frequency to be set
    */
    IMPORT_C void SetFrequency( const TUint aFrequency );

    /**
    * Set URL.
    * @since Series 60 10.1
    * @param aStationName The URL to be set.
    * @return none
    */
    IMPORT_C void SetUrl( const TRadioUrl& aUrl );

    /**
    * Get the URL.
    * @since Series 60 10.1
    * @return the URL
    */
    IMPORT_C TPtrC Url() const;

    /**
    * Set the Favorite status.
    * @since Series 60 10.1
    * @param aFavorite -  Favorite status.
    * @return none
    */
    IMPORT_C void SetFavorite( TBool aFavorite );

    /**
    * Get the Favorite status.
    * @since Series 60 10.1
    * @return TBool ETrue if favorite, otherwise EFalse
    */
    IMPORT_C TBool Favorite() const;

    /**
    * Set the Local Station status.
    * @since Series 60 10.1
    * @param aLocalStation -  Local Station status.
    * @return none
    */
    IMPORT_C void SetLocalStation( TBool aLocalStation );

    /**
    * Get the local station status.
    * @since Series 60 10.1
    * @return TBool ETrue if local station, otherwise EFalse
    */
    IMPORT_C TBool LocalStation() const;

    /**
    * Set the Set By User status.
    * @since Series 60 10.1
    * @param aRenamed -  Set By User status.
    * @return none
    */
    IMPORT_C void SetRenamedByUser( TBool aRenamed );

    /**
    * Get the renamed by user status.
    * @since Series 60 10.1
    * @return TBool ETrue if renamed by user, otherwise EFalse
    */
    IMPORT_C TBool RenamedByUser() const;

    /**
    * Set PTY.
    * @since Series 60 10.1
    * @param aGenre The PTY to be set.
    * @return none
    */
    IMPORT_C void SetGenre( TInt aGenre );

    /**
    * Get the PTY.
    * @since Series 60 10.1
    * @return the PTY
    */
    IMPORT_C TInt Genre() const;

    /**
    * Set PI.
    * @since Series 60 10.1
    * @param aGenre The PI to be set.
    * @return none
    */
    IMPORT_C void SetPiCode( TInt aPiCode );

    /**
    * Get the PI.
    * @since Series 60 10.1
    * @return the PI
    */
    IMPORT_C TInt PiCode() const;

    /**
    * Externalize (ie. serialize) data.
    * @param aStream -  Stream to where to serialize.
    * @return none
    */           
    IMPORT_C virtual void ExternalizeL( RWriteStream & aStream ) const;

    /**
    * Internalize data.
    * @param aStream -  Stream from where to internalize.
    * @return none
    */           
    IMPORT_C virtual void InternalizeL( RReadStream& aStream );

    /**
    * Get the size (in bytes) of my externalized data.
    * @since Series 60 10.1
    * @return The size (in bytes) of my externalized data.
    */
   IMPORT_C virtual TUint MyExternalizedDataSize() const; 
    
private: // data

    // Preset's name
    TPresetName     iPresetName;

    // Preset's frequency
    TUint           iPresetFrequency;

    // Preset's URL
    TRadioUrl       iUrl;

    // Program Type
    TInt                iGenre;
    // Program Identification
    TInt                iPiCode;
    // Bitwise flags
    TBitFlags           iFlags;

    };

#endif // TPRESET_H
