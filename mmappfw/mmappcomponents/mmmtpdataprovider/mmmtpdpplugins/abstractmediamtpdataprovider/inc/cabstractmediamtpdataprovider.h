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
* Description:  Abstract media data provider plug in
*
*/


#ifndef CABSTRACTMEDIAMTPDATAPROVIDER_H
#define CABSTRACTMEDIAMTPDATAPROVIDER_H

#include <mtp/cmtpdataproviderplugin.h>

#include "abstractmediamtpdataproviderprocessor.h"
#include "mmmtpenumerationcallback.h"
#include "mmmtpdpconfig.h"
#include "mmmtpdp_variant.hrh"

// Forward declaration
class MMmRequestProcessor;
class CAbstractMediaMtpDataProviderEnumerator;
class CMTPObjectMetaData;
class CAbstractMediaMtpDataProviderRenameObject;

/**
* Defines abstract media data provider plugin
*/
class CAbstractMediaMtpDataProvider : public CMTPDataProviderPlugin,
    public MMTPEnumerationCallback,
    public MMmMtpDpConfig
    {
public:
    /**
    * Two Phase Construction
    * @param aParams pointer to MMTPDataProviderFramework
    */
    static TAny* NewL( TAny* aParams );

    /**
    * Destructor.
    */
    ~CAbstractMediaMtpDataProvider();

public:
    // from MMmMtpDpConfig
    const RArray<TUint>* GetSupportedFormat() const;

    const RArray<TUint>* GetSupportedPropertiesL( TUint32 aFormatCode ) const;

    const RArray<TUint>* GetAllSupportedProperties() const;

    /**
    * Get default storage Id from resource file, if not specify a storage Id
    * @return TUint32 Default storage Id which read from resource file
    */
    TUint32 GetDefaultStorageIdL() const;

    /**
    * Get db handler wrapper
    * @return wrapper references
    */
    CMmMtpDpMetadataAccessWrapper& GetWrapperL();

    /**
    * @return The utility to setting properties
    */
    CPropertySettingUtility* PropSettingUtility();

    /**
    * @return The utiltiy to setting descriptions
    */
    CDescriptionUtility* DescriptionUtility();

protected:
    // from CMTPDataProviderPlugin
    void Cancel();

    void ProcessEventL( const TMTPTypeEvent& aEvent,
        MMTPConnection& aConnection );

    void ProcessNotificationL( TMTPNotification aNotification,
        const TAny* aParams );

    /**
    * Process the request from initiator
    * @param aPhase    The request transaction phase
    * @param aRequest    The request to be processed
    * @param aConnection    The connection from which the request comes
    */
    void ProcessRequestPhaseL( TMTPTransactionPhase aPhase,
        const TMTPTypeRequest& aRequest,
        MMTPConnection& aConnection );

    /**
    * Notify the data provider that the session has been open
    */
    void SessionOpenedL( const TMTPNotificationParamsSessionChange& aSession );

    /**
    * Notify the data provider that the session has been closed
    * @param aSession    The connection of the sesssion
    */
    void SessionClosedL( const TMTPNotificationParamsSessionChange& aSession );

    /**
    * Notify the data provider that the object has been changed
    * @param aObject    the changed object
    */
    void RenameObjectL( const TMTPNotificationParamsHandle& aObject );

    void StartObjectEnumerationL( TUint32 aStorageId );

    void StartStorageEnumerationL();

    /**
    * Defines the supported operations and formats of the data provider
    * @param aCategory Defines what MTP is quering the DP about
    * @param aArray Supported() edits array to append supported features
    */
    void Supported( TMTPSupportCategory aCategory, RArray<TUint>& aArray ) const;

    void SupportedL( TMTPSupportCategory aCategory, CDesCArray& aStrings ) const;

    TAny* GetExtendedInterface( TUid aInterfaceUid );

protected:
    // From MMTPEnumerationCallback
    /**
    * enumeration completed
    */
    void NotifyEnumerationCompleteL( TUint32 aStorageId, TInt aError );

private:
    /**
    * Standard C++ constructor
    * @param aParams    pointer to MMTPDataProviderFramework
    */
    CAbstractMediaMtpDataProvider( TAny* aParams );

    /**
    * Second-phase construction
    */
    void ConstructL();

    /**
    * Find or create a request processor that can process the request
    * @param aRequest    The request to be processed
    * @param aConnection The connection from which the request comes
    * @return the index of the found/created request processor
    */
    TInt LocateRequestProcessorL( const TMTPTypeRequest& aRequest,
        MMTPConnection& aConnection );

    /**
    * Find or create a request processor that can process the event
    * @param aEvent    The event to be processed
    * @param aConnection The connection from which the request comes
    * @return the index of the found/created request processor
    */
    TInt LocateRequestProcessorL( const TMTPTypeEvent& aEvent,
        MMTPConnection& aConnection );

    /**
    * helper function
    */
    void InsertL( RArray<TUint>& aArray, const TUint aProperCode ) const;

    void GetSupportedFormatL();

    void GetSupportedPropertiesL();

    void GetAllSupportedPropL();

private:
    // data
    RPointerArray<MMmRequestProcessor> iActiveProcessors;
    CAbstractMediaMtpDataProviderEnumerator* iAbstractMediaEnumerator;
    RArray<TUint32> iPendingEnumerations;

    TInt iActiveProcessor;
    TBool iActiveProcessorRemoved;

    CAbstractMediaMtpDataProviderRenameObject *iRenameObject;
    TBool iIsSessionOpen;

    RArray<TUint> iSupportedFormat;

#ifdef MMMTPDP_ABSTRACTAUDIOALBUM_SUPPORT
    RArray<TUint> iSupportedPropAbstractAlbum;
#endif
    RArray<TUint> iSupportedPropPlaylist;
    RArray<TUint> iSupportedPropAll;

    CPropertySettingUtility* iPropSettingUtility;
    CDescriptionUtility* iDescriptionUtility;

    };

#endif // CABSTRACTMEDIAMTPDATAPROVIDER_H
