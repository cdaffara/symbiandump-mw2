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
* Description:  Media Fetch Implementation API
*
*/


#ifndef MEDIAFETCHIMPLEMENTATION_H
#define MEDIAFETCHIMPLEMENTATION_H

#include <e32base.h>
#include <MediaFileTypes.hrh>

class MMGFetchVerifier;
class MMGFetchCanceler;

/**
 *  ECom interface for Media Fetch plug-in implementation.
 *
 *  Plug-in implementation used for Media Fetch API functionality is selected
 *  based on plug-ins' implementation information.
 *
 *  Media types supported by the plug-in are defined in plug-in resource's
 *  default_data. The value must be in hexadecimal format:
 *
 *  LTEXT8  default_data;
 *
 *  Example: default_data = "00000003" if both image (0x00000001) and video
 *  (0x00000002) fetchers are supported by the plug-in.
 *
 *  See MediaFileTypes.hrh for supported media types.
 *
 *  If multiple plug-ins support the same media type(s) then the plug-in with
 *  the highest version number is selected.
 *
 *  BYTE    version_no;
 *
 *  See registryinfov2.rh for ECom resource structure information.
 *
 *  @lib n/a
 *  @since S60 v3.2
 */
class CMediaFetchImplementation : public CBase
    {

public:

    inline static CMediaFetchImplementation* NewL(
                        const TUid& aImplementationUid );

    inline virtual ~CMediaFetchImplementation();

    /**
     * Optional feature.
     * By default fetcher is single selection fetcher.
     * If multi-selection is enabled then plug-in should display a
     * multi-selection fetcher and allow user to select several items.
     * @since S60 v3.2
     * @param aMultiSelect if ETrue then multiselection fetcher is enabled
     */
    virtual void SetMultiSelectionL( TBool aMultiSelect ) = 0;

    /**
     * Optional feature.
     * @since S60 v3.2
     * @param aMimeTypes plug-in should show only files with given Mime types
     */
    virtual void SetMimeTypesL( const MDesCArray& aMimeTypes ) = 0;

    /**
     * Optional feature.
     * Plug-in should use given text as selection key (SK1) label
     * @since S60 v3.2
     * @param aSelectionSoftkey Custom selection softkey label
     */
    virtual void SetSelectionSoftkeyL( const TDesC& aSelectionSoftkey ) = 0;

    /**
     * Optional feature
     * Plug-in should use given text as heading
     * @since S60 v3.2
     * @param aHeading Custom heading text for fetcher
     */
    virtual void SetHeadingL( const TDesC& aHeading ) = 0;

    /**
     * Optional feature.
     * Plug-in should call this interface before selection is done.
     * if MMGFetchVerifier::VerifySelectionL() returns ETrue
     * then the user's selection should be accepted and the fetcher
     * closed. If EFalse is returned then user should change the current
     * selection before it can be accepted and the fetcher closed.
     * @since S60 v3.2
     * @param aVerifier See MMGFetchVerifier.h
     */
    virtual void SetVerifierL( MMGFetchVerifier& aVerifier ) = 0;

    /**
     * Optional feature.
     * Plug-in should return canceler interface.
     * @since S60 v3.2
     * @param aCanceler See MMGFetchCanceler.h
     */
    virtual void GetCancelerL( MMGFetchCanceler*& aCanceler ) = 0;

    /**
     * Plug-in should launch fetcher popup for given media type.
     * @since S60 v3.2
     * @param aSelectedFiles plug-in appends selected files to this array
     * @param aMediaType plug-in should display only files with this media type
     * return ETrue if user selected file(s), EFalse if use cancelled.
     */
    virtual TBool LaunchL( CDesCArray& aSelectedFiles,
                           TMediaFileType aMediaType ) = 0;

private: // data

    // Own: ID key
    TUid iDtor_ID_Key;

    };

#include "mediafetchimplementation.inl"

#endif // MEDIAFETCHIMPLEMENTATION_H
