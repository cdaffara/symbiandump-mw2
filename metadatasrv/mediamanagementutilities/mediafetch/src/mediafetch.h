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
* Description:  Media Fetch implementation plug-in loader
*
*/


#ifndef MEDIAFETCH_H
#define MEDIAFETCH_H

#include <e32base.h>
#include <badesca.h>
#include <MediaFileTypes.hrh>

class MMGFetchVerifier;
class MMGFetchCanceler;
class CMediaFetchImplementation;
class MDesCArray;

/**
 *  Media Fetch ECom plug-in loader
 *
 *  Loads the ECom plug-in with the highest version number that supports the
 *  media type(s) given at construction parameter.
 *
 *  @lib MGFetch.lib
 *  @since S60 v3.2
 */
NONSHARABLE_CLASS( CMediaFetch ) : public CBase
    {

public:

    static CMediaFetch* NewL( TInt aMediaType );

    static CMediaFetch* NewLC( TInt aMediaType );

    virtual ~CMediaFetch();

    /**
     *
     * @since S60 v3.2
     * @param aMultiSelect if ETrue then multiselection fetcher is enabled
     */
    void SetMultiSelectionL( TBool aMultiSelect );

    /**
     *
     * @since S60 v3.2
     * @param aMimeTypes plug-in should show only files with given Mime types
     */
    void SetMimeTypesL( const MDesCArray& aMimeTypes );

    /**
     *
     * @since S60 v3.2
     * @param aSelectionSoftkey Custom selection softkey label
     */
    void SetSelectionSoftkeyL( const TDesC& aSelectionSoftkey );

    /**
     *
     * @since S60 v3.2
     * @param aHeading Custom heading text for fetcher
     */
    void SetHeadingL( const TDesC& aHeading );

    /**
     *
     * @since S60 v3.2
     * @param aVerifier See MMGFetchVerifier.h
     */
    void SetVerifierL( MMGFetchVerifier& aVerifier );

    /**
     *
     * @since S60 v3.2
     * @param aCanceler See MMGFetchCanceler.h
     */
    void GetCancelerL( MMGFetchCanceler*& aCanceler );

    /**
     *
     *
     * @since S60 v3.2
     * @param aSelectedFiles array to hold selected items
     * @param aMediaType type of fetcher to use
     * @return ETrue if user selected files, EFalse if user canceled the fetcher
     */
    TBool LaunchL( CDesCArray& aSelectedFiles,
                   TMediaFileType aMediaType );

private:

    inline CMediaFetch();

    inline void ConstructL( TInt aMediaType );

private: // data

    /**
     * Fetcher implementation
     * Own.
     */
    CMediaFetchImplementation* iFetcher;

    };

#endif // MEDIAFETCH_H
