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
 * Description:
 *
 */

#ifndef PSETNETWORKWRAPPER_H
#define PSETNETWORKWRAPPER_H

#include <QObject>

class PSetNetworkWrapperPrivate;
class CPsetContainer;
class HbDialog;

#ifdef BUILD_PSETWRAPPER
#define PSETWRAPPER_EXPORT Q_DECL_EXPORT
#else
#define PSETWRAPPER_EXPORT Q_DECL_IMPORT
#endif

class PSETWRAPPER_EXPORT PSetNetworkWrapper : public QObject
{
    Q_OBJECT

public:
    
    enum ErrorCode {
        ErrNoError  = 0,
        ErrGeneral,
        ErrCauseCallActive,
        ErrNoNetworkService,
        ErrOfflineOpNotAllowed,
        ErrNoNetworkAccess
    };
    
    enum RequestType {
        RequestNone,
        RequestEnumerateNetworks,
        RequestGetNetworkInfo,
        RequestGetCurrentNetworkInfo,
        RequestGetNetworkSearchMode,
        RequestSetNetwork,
        RequestSetNetworkMode,
        RequestSetNetworkAutomatic
    };
    
    enum RegistrationStatus {
        RegisteredOnHomeNetwork,
        RegisteredRoaming
    };
    
    enum NetworkAccessMode {
        AccessModeDual,
        AccessModeUmts,
        AccessModeGsm
    };
    
    struct NetworkId{
        /*! Country code. Maximum length is 4. */
        QString m_countryCode;
        /*! Network code. Maximum length is 8. */
        QString m_networkCode;
    };

    enum NetworkSelectionMode {
        SelectionModeManual,
        SelectionModeAutomatic 
    };

    enum NetworkStatus {
        StatusUnknown,
        StatusAvailable,
        StatusCurrent,
        StatusForbidden
    };
    
    enum NetworkAccessType {
        AccessTypeGsm, 
        AccessTypeWcdma,
        AccessTypeGsmAndWcdma
    };

    struct NetworkInfo {
        NetworkId m_id;
        NetworkStatus m_status;
        NetworkSelectionMode m_mode;
        NetworkAccessType m_access;
        /*! Short name. Maximum length is 20. */
        QString m_shortName;
        /*! Long name. Maximum length is 30. */
        QString m_longName;
    };        

public:

    explicit PSetNetworkWrapper(CPsetContainer &psetContainer,
        QObject *parent = NULL);

    virtual ~PSetNetworkWrapper();

public:
    
    void getAvailableNetworks();
    
    ErrorCode getNetworkSelectionMode(
        PSetNetworkWrapper::NetworkSelectionMode& mode) const;
    
    void selectNetwork( 
        const PSetNetworkWrapper::NetworkInfo& info);
    
    void cancelRequest();

public:

    void getNetworkAccessMode() const;

    void setNetworkAccessMode(PSetNetworkWrapper::NetworkAccessMode mode);
    
    bool isManualNetworkSelectionSupported() const;

signals:

    // Real type of parameter mode is enum PSetNetworkWrapper::NetworkAccessMode.
    void networkAccessModeGot(int mode);
    
    void availableNetworksGot(QList<PSetNetworkWrapper::NetworkInfo*> &networkInfoList);
     
    void networkReqestFailed(
        PSetNetworkWrapper::ErrorCode error, 
        PSetNetworkWrapper::RequestType type);
    
    void searchingNetworks(PSetNetworkWrapper::RequestType &type);
    
    void requestingSelectedNetwork(bool ongoing);
    
    void networkChanged(
        PSetNetworkWrapper::NetworkInfo& currentInfo,
        PSetNetworkWrapper::RegistrationStatus& status);

    void chageVisbilityOfManualNetworkSelection(bool visible);
    
private:
    
    PSetNetworkWrapper(const PSetNetworkWrapper &);
    PSetNetworkWrapper &operator=(const PSetNetworkWrapper &);
    PSetNetworkWrapperPrivate* m_privateImpl;
    friend class PSetNetworkWrapperPrivate;
};

#endif // PSETNETWORKWRAPPER_H
