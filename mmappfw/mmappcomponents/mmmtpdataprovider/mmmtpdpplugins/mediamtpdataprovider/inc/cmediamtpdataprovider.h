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
* Description:  media mtp data provider
*
*/


#ifndef CMEDIAMTPDATAPROVIDER_H
#define CMEDIAMTPDATAPROVIDER_H

#include <mtp/cmtpdataproviderplugin.h>

#include "mediamtpdataproviderprocessor.h"
#include "mmmtpenumerationcallback.h"
#include "mmmtpdpconfig.h"

// Forward declaration
class MMmRequestProcessor;
class CMediaMtpDataProviderEnumerator;
class CMTPObjectMetaData;
class CRenameObject;

/**
* Defines media mtp data provider plugin
*/
class CMediaMtpDataProvider : public CMTPDataProviderPlugin,
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
    ~CMediaMtpDataProvider();

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
    * @return the reference of CMmMtpDpMetadataAccessWrapper to enumerator
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

    /**
    * Notify the data provider that the object has been added
    * @param aObjectHandle    the added object handle
    */
    void ObjectAddedL( TUint32 aObjectHandle );

    void StartObjectEnumerationL( TUint32 aStorageId );

    void StartStorageEnumerationL();

    /**
    * Defines the supported operations and formats of the data provider
    * @param aCategory Defines what MTP is quering the DP about
    * @param aArray Supported() edits array to append supported features
    */
    void Supported( TMTPSupportCategory aCategory, RArray<TUint>& aArray ) const;

    void SupportedL( TMTPSupportCategory aCategory, CDesCArray& aStrings ) const;

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
    CMediaMtpDataProvider( TAny* aParams );

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

    void InsertL( RArray<TUint>& aArray, const TUint aProperCode ) const;

    void GetSupportedFormatL();

    void GetSupportedPropL();

    void GetAllSupportedPropL();

private:
    // data
    RPointerArray<MMmRequestProcessor> iActiveProcessors;
    CMediaMtpDataProviderEnumerator* iMediaEnumerator;
    RArray<TUint32> iPendingEnumerations;

    TInt iActiveProcessor;
    TBool iActiveProcessorRemoved;
    TBool iIsSessionOpen;

    CRenameObject *iRenameObject;

    RArray<TUint> iSupportedFormat;

    RArray<TUint> iSupportedPropAudio;
    RArray<TUint> iSupportedPropVideo;
    RArray<TUint> iSupportedPropAll;

    CPropertySettingUtility* iPropSettingUtility;
    CDescriptionUtility* iDescriptionUtility;

    };

#endif // CMEDIAMTPDATAPROVIDER_H
