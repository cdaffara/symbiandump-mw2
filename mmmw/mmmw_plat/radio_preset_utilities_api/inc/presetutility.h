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
* Description:  This class provides an interface for monitoring radio
*               presets.
*
*
*/

#ifndef CPRESETUTILITY_H_
#define CPRESETUTILITY_H_

#include <e32base.h>

class MPresetUtilityObserver;
class TPreset;

/**
 *  This class provides an interface for controlling the presets for a radio tuner.
 *  The maximum number of presets supported by this utility is pre-defined and the
 *  client must first query it.
 *
 *  NOTE! The preset indexes start from 1
 */
NONSHARABLE_CLASS( CPresetUtility ) : public CBase
    {
public:

    /**
     * Factory function to create a new instance of the Preset Utility.
     *
     * @since S60 10.1
     * @return A new Preset Utility object.
     */
    IMPORT_C static CPresetUtility* NewL();

    IMPORT_C ~CPresetUtility();

    /**
     * Set the Radio Preset's observer pointer.
     *
     * @since S60 10.1
     */
    virtual void SetObserver( MPresetUtilityObserver* aObserver ) = 0;

    /**
     * Get the maximum number of presets supported by this utility.
     * The numbering of presets starts from 1 and the largest preset number equals the value
     * returned from this function.
     *
     * @since S60 10.1
     * @return The maximum number of presets.
     */
    virtual TInt MaxNumberOfPresets() const = 0;

    /**
     * Get the number of presets already set by the client (i.e. contains data).
     *
     * @since S60 10.1
     * @return The current number of presets, possible values from 1 to maximum
     * number of presets.
     */
    virtual TInt PresetCount() const = 0;

    /**
     * Get the first preset index that has been set.
     * Leaves with KErrNone if no preset exists.
     *
     * @since S60 10.1
     * @return The index of first preset, possible values from 1 to maximum
     * number of presets.
     */
    virtual TInt FirstPreset() const = 0;

    /**
     * Get the next preset index that has been set, given the current index.
     * Returns -1 if no other preset exists.
     *
     * @since S60 10.1
     * @param aFromIndex Current preset index.
     * @return The index of next preset. -1 if no other preset exists.
     * Otherwise possible values from 1 to maximum number of presets.
     */
    virtual TInt NextPreset( TInt aFromIndex ) const = 0;

    /**
     * Delete the specified preset.
     * Has no effect if specified index contains no preset.
     *
     * NOTE: Specifying aIndex <= 0, will delete all presets.
     *
     * @since S60 10.1
     * @param aIndex A preset index. If aIndex == -1, will delete all. 
     * Otherwise legal values all used indexes from 0 to (maximum - 1)
     * number of presets.
     * @return A standard system error code.
     */
    virtual TInt DeletePreset( TInt aIndex ) = 0;

    /**
     * Save the given preset. Previous content in the specified index
     * will be overwritten with the new information.
     *
     * @since S60 10.1
     * @param aPreset Object containing all information of the preset.
     * @param aIndex A preset index. Default value -1 means that a free index
     * is allocated for the preset. Otherwise legal values from 0 to 
     * (maximum - 1)number of presets.
     * @return Given/Allocated index of the preset. Possible values from 1 to
     * maximum number of presets.
     */
    virtual TInt SavePresetL( const TPreset& aPreset, TInt aIndex = -1 ) = 0;

    /**
     * Read the preset at the specified index.
     * Leaves with KErrNotFound if invalid index is specified.
     *
     * @since S60 10.1
     * @param aIndex A preset index. Legal values from 0 to (maximum - 1) number of presets.
     * @param aPreset On return, this object is updated with given preset's information.
     */
    virtual void ReadPresetL( TInt aIndex, TPreset& aPreset ) = 0;

    };

#endif // CPRESETUTILITY_H_
