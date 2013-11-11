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
* Description:  This class provides an interface for controlling the presets.
*                Currently, only FM preset is supported. This API may be extended
*                in future to support other presets also.
*
*
*/



#ifndef M_RADIOPRESETUTILITY_H
#define M_RADIOPRESETUTILITY_H

#include <e32std.h>
#include <presetutilityobserver.h>


#warning This file is deprecated and will be removed in future releases. Please use the new API

/** Preset name, 32 characters */
typedef TBuf<32> TFmPresetName;
/** Preset Url, 64 characters, the maximum number of characters in RDS RT string */
typedef TBuf<64> TFmPresetUrl;


/**
 *  This class provides the interface class to be implemented by object receiving events
 *  from preset control.
 *
 *  @lib RadioPresetUtility.lib
 *  @since S60 3.2
 */
class MRadioPresetObserver
    {
    public:

    /**  Preset event */
    enum TPresetChangeEvent
        {
        EPresetAdded = MPresetUtilityObserver::EPresetAdded,
        EPresetDeleted = MPresetUtilityObserver::EPresetDeleted,
        EPresetUpdated = MPresetUtilityObserver::EPresetUpdated
        };

    /**
     * Called when a preset changes.
     *
     * NOTE: EPresetDeleted with aIndex == -1, indicates that all presets have been deleted.
     *
     * @since S60 3.2
     * @param aChange Change event type
     * @param aIndex Index to the preset that has changed. Zero means all presets.
     */
    virtual void MrpeoPresetChanged( TPresetChangeEvent aChange, TInt aIndex ) = 0;
	};


/**
 *  This class provides an interface for controlling the preset settings.
 *  This is an abstract interface that must be implemented by all preset utilities.
 *
 *  @lib RadioPresetUtility.lib
 *  @since S60 3.2
 */
class MRadioPreset
    {
public:

    /**
     * Get the maximum number of presets supported by this utility.
     * The numbering of presets starts from 1 and the largest preset number equals the value
     * returned from this function.
     *
     * @since S60 3.2
     * @param aMaxNumOfPresets On return contains the maximum number of presets.
     * @return A standard system error code.
     */
    virtual TInt GetMaxNumberOfPresets( TInt& aMaxNumOfPresets ) const = 0;

    /**
     * Get the number of presets already set by the client (i.e. contains data).
     *
     * @since S60 3.2
     * @param aNumOfPresets On return contains the current number of presets.
     * @return A standard system error code.
     */
    virtual TInt GetNumberOfPresets( TInt& aNumOfPresets ) const = 0;

    /**
     * Get the first preset index that has been set.
     * Leaves with KErrNone if no preset exists.
     *
     * @since S60 3.2
     * @param aIndex On return contains the index of the first preset.
     */
    virtual void GetFirstPresetL( TInt& aIndex ) const = 0;

    /**
     * Get the next preset index that has been set, given the current index.
     * Leaves with KErrNone if no other preset exists.
     *
     * @since S60 3.2
     * @param aIndex Current preset index.
     * @param aNextIndex On return contains the index of next preset.
     */
    virtual void GetNextPresetL( TInt aIndex, TInt& aNextIndex ) const = 0;

    /**
     * Delete the specified preset.
     * Has not effect if specified index contains no preset.
     *
     * NOTE: Specifying aIndex == 0, will delete all presets.
     *
     * @since S60 3.2
     * @param aIndex A preset index.
     */
    virtual void DeletePresetL( TInt aIndex ) = 0;
    };


/**
 *  This class provides an interface for controlling the preset for an FM tuner.
 *  This is an abstract interface that must be implemented by all preset utilities supporting
 *  FM tuner preset.
 *
 *  @lib RadioPresetUtility.lib
 *  @since S60 3.2
 */
class MRadioFmPreset
    {
public:

    /**
     * Set the preset with specified frequency and name. Previous content in the specified index
     * will be overwritten with the new information.
     *
     * The maximum length for the preset name is 32 characters. Names longer than 32 characters
     * will be truncated.
     *
     * @since S60 3.2
     * @param aIndex A preset index.
     * @param aName Preset name.
     * @param aFrequency Frequency in Hz.
     */
    virtual void SetPresetL( TInt aIndex, const TFmPresetName& aName, TInt aFrequency ) = 0;

    /**
     * Get the preset name and frequency at the specified index.
     * Leaves with KErrNotFound if invalid index is specified.
     *
     * Preset name is maximum of 32 characters. Client must allocate buffer big
     * enough to hold the 32 characters. If not, characters will be truncated to fit the
     * buffer.
     *
     * @since S60 3.2
     * @param aIndex A preset index.
     * @param aFrequency On return, contains the preset name.
     * @param Frequency On return, contains frequency in Hz.
     */
    virtual void GetPresetL( TInt aIndex, TFmPresetName& aName, TInt& aFrequency ) = 0;
 
    /**
     * Get the preset name at the specified index.
     * Leaves with KErrNotFound if invalid index is specified.
     *
     * Preset name is maximum of 32 characters. Client must allocate buffer big
     * enough to hold the 32 characters. If not, characters will be truncated to fit the
     * buffer.
     *
     * @since S60 5.1
     * @param aIndex A preset index.
     * @param aName On return, contains the preset name.
     */
    virtual void GetPresetNameL( TInt aIndex, TFmPresetName& aName ) = 0; 
    
    /**
     * Get the preset frequency at the specified index.
     * Leaves with KErrNotFound if invalid index is specified.
     *
     * @since S60 5.1
     * @param aIndex A preset index.
     * @param aFrequency On return, contains the preset name.
     */
    virtual void GetPresetFrequencyL( TInt aIndex, TInt& aFrequency ) = 0;      
    };

#endif // M_RADIOPRESETUTILITY_H
