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
* Description:  This is a wrapper class of Central Repository, responsible
*               for the operations that need to access the central respository.
*
*
*/

#ifndef CCENREPHANDLER_H
#define CCENREPHANDLER_H

// System includes
#include <e32base.h>

// Constants
const TInt KBufferLength = 20;
const TInt KStreamBufferSize = 100;

// Forward declarations
class CRepository;
class TPreset;
class CCenrepBookKeeping;

// Class declaration

/**
 *  it is a wrapper of Central Repository, responsible for the operations that need to access
 *  the central respository.
 */
class CCenrepHandler : public CBase
    {
public:

    /**
    * Two-phased class constructor.
    */
    static CCenrepHandler* NewL();
    
    /**
    * Destructor of CCenrepHandler class.
    */
    virtual ~CCenrepHandler();

    /**
    * Get the maximum number of presets.
    * @return The maximum number of presets.
    */
    TInt MaxPresetCount() const;

    /**
    * Get the current number of presets.
    * @return The number of presets.
    */
    TInt PresetCount() const;

    /**
    * Get the index of first preset.
    * @return The first preset index.
    */        
    TInt FirstPreset() const;

    /**
    * Get the index of the next preset.
    * @param aFromIndex -  Start from index after this.
    * @return the next preset index.
    */           
    TInt NextPreset( TInt aFromIndex ) const;
    
    /**
    * Delete the specified preset.
    * Specifying aIndex < 0, will delete all presets.
    * Has no effect if specified index contains no preset.
    */
    TInt DeletePreset( TInt aIndex );
    
    /**
    * Save the preset.
    * @param aPreset Preset object.
    * @param aIndex -  Preset index.
    * @return given/allocated(in case given value was -1) index.
    */           
    TInt SavePresetL( const TPreset& aPreset, TInt aIndex = -1 );
    
    
    /**
    * Read a preset.
    * @param aIndex -  Preset index.
    * @param aPreset Preset object to where to update the data.
    * @return none
    */           
    void ReadPresetL( TInt aIndex, TPreset& aPreset );

private:

    /**
    * Default class constructor.
    */
    CCenrepHandler();

    /**
    * Second phase class constructor.
    */
    void ConstructL();

    /**
    * Read bookkeeping information from the permanent storage.
    */
    void ReadBookKeepingL();

    /**
    * Save bookkeeping information to the permanent storage.
    */
    void SaveBookKeeping();
        
    /**
    * Read preconfigured presets from the permanent storage.
    */
    void ReadPreDefinedPresetsL();

private: // data

    // Central Repository
    CRepository*        iCentralRepository;

    // Bookkeeping of existing presets
    CCenrepBookKeeping* iBookKeeping;

    };

#endif  // CCENREPHANDLER_H
