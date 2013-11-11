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


#ifndef CABSTRACTMEDIAMTPDATAPROVIDERENUMERATOR_H
#define CABSTRACTMEDIAMTPDATAPROVIDERENUMERATOR_H

#include <e32base.h>

#include "mmmtpenumerationcallback.h"
#include "cmmmtpdpperflog.h"

// Forward declarations
class MMTPDataProviderFramework;
class MMTPObjectMgr;
class CAbstractMediaMtpDataProvider;
class CMPXMediaArray;

/**
* Defines file enumerator.  Enumerates all files/directories under a
* specified path or storage
*/
class CAbstractMediaMtpDataProviderEnumerator: public CActive
    {
public:
    /**
    * Two phase constructor
    * @param aFramework Reference to MMTPDataProviderFramework
    * @param aDataProvider Reference to Abstract Media DataProvider
    * @return A pointer to a new instance of the object
    */
    static CAbstractMediaMtpDataProviderEnumerator* NewL( MMTPDataProviderFramework& aFramework,
        CAbstractMediaMtpDataProvider& aDataProvider );

    /**
    * destructor
    */
    ~CAbstractMediaMtpDataProviderEnumerator();

    /**
    * Kick off the enumeration on the specified storage
    * @param aStorageId storage to be enumerated
    */
    void StartL( TUint32 aStorageId );

protected: // from CActive
    /**
    * Cancel the enumeration process
    */
    void DoCancel();

    void RunL();

    /**
    * Ignore the error, continue with the next one
    */
    TInt RunError( TInt aError );

private:

    enum TEnumState
    {
        EEnumNone,
        EEnumPlaylist,
        EEnumAbstractAlbum,
        EEnumCount
    };

    /**
    * Standard C++ Constructor
    * @param aFramework Reference to MMTPDataProviderFramework
    * @param aDataProvider Reference to Abstract Media DataProvider
    */
    CAbstractMediaMtpDataProviderEnumerator( MMTPDataProviderFramework& aFramework,
        CAbstractMediaMtpDataProvider& aDataProvider );

    void ConstructL();

    /**
    * Called when the enumeration is completed
    */
    void SignalCompleteL( MMTPEnumerationCallback& aCallback,
        TInt aError = KErrNone );

    void InitStorageL( );
    void ScanNextL();

    /**
    * Add a file entry to the object store
    * @param aPath current scanned file path
    */
    void AddEntryL( const TDesC& aSuid );
    
    /**
    * reset Object's format sub code in object manager
    */
    void ResetObjectFormatSubCodeL( TUint32 aStorageId );


private:
    // Owned
    /** MMTPDataProviderFramework used to access MTP components*/
    MMTPDataProviderFramework& iFramework;

    /** MTP Object Manager */
    MMTPObjectMgr& iObjectMgr;

    /** MTP Data Provider ID */
    TUint iDataProviderId;

    /** Provides Callback mechanism for completion of enumeration*/
    CAbstractMediaMtpDataProvider& iDataProvider;

    RArray<TUint> iStorages;
    TUint32 iStorageId;

    CDesCArray* iAbstractMedias;
    TInt iCount;
    TInt iCurrentIndex;
    TInt iEnumState;
    
    TInt iResetCount;
    
#if defined(_DEBUG) || defined(MMMTPDP_PERFLOG)
    CMmMtpDpPerfLog* iPerfLog;
#endif

    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDERENUMERATOR_H
