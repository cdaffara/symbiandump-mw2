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
* Description:  Enumerator objects
*
*/


#ifndef CMEDIAMTPDATAPROVIDERENUMERATOR_H
#define CMEDIAMTPDATAPROVIDERENUMERATOR_H

#include <e32base.h>
#include "mmmtpenumerationcallback.h"
#include "cmmmtpdpperflog.h"

// Forward declarations
class MMTPDataProviderFramework;
class CMediaMtpDataProvider;

/**
* Defines file enumerator.  Enumerates all files/directories under a
* specified path or storage
*/
class CMediaMtpDataProviderEnumerator: public CActive
    {
public:
    /**
    * Two phase constructor
    * @param aFramework Reference to MMTPDataProviderFramework
    * @param aDataProvider Reference to CMediaMtpDataProvider
    * @return A pointer to a new instance of the object
    */
    static CMediaMtpDataProviderEnumerator* NewL( MMTPDataProviderFramework& aFramework,
        CMediaMtpDataProvider& aDataProvider );

    /**
    * destructor
    */
    ~CMediaMtpDataProviderEnumerator();

    /**
    * Kick off the enumeration on the specified storage
    * @param aStorageId storage to be enumerated
    */
    void StartL( TUint32 aStorageId );

    // introduce to cleanup db at close session
    void SessionClosedL();

protected:
    // from CActive
    /**
    * Cancel the enumeration process
    */
    void DoCancel();

    /**
    * Update the mpx db flag for round trip
    */
    void RunL();

    /**
    * Ignore the error, continue with the next one
    */
    TInt RunError( TInt aError );

private:
    /**
    * Standard C++ Constructor
    * @param aFramework Reference to MMTPDataProviderFramework
    * @param aDataProvider Reference to CMediaMtpDataProvider
    */
    CMediaMtpDataProviderEnumerator( MMTPDataProviderFramework& aFramework,
        CMediaMtpDataProvider& aDataProvider );

    void ConstructL();

    /**
    * Called when the enumeration is completed
    */
    void SignalCompleteL( MMTPEnumerationCallback& aCallback,
        TInt aError = KErrNone );

    /**
    * Get modified content and report to Symbian MTP Framework
    */
    void GetModifiedContentL( const TDesC& aStorageRoot );

private:
    // Owned
    /** MMTPDataProviderFramework used to access MTP components*/
    MMTPDataProviderFramework& iFramework;

    CMediaMtpDataProvider& iDataProvider;
    RArray<TUint> iStorages;
    TUint32 iStorageId;

#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    CMmMtpDpPerfLog* iPerfLog;
#endif

    };

#endif // CMEDIAMTPDATAPROVIDERENUMERATOR_H
