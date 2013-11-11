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

#ifndef PSETNETWORKWRAPPERPRIVATE_H
#define PSETNETWORKWRAPPERPRIVATE_H

#include <mpsetnetworkinfoobs.h>
#include <mpsetnetworkmodeobs.h>
#include <mpsetnetworkselect.h>
#include <QList>
#include <MSSSettingsRefreshObserver.h>
#include "psetnetworkwrapper.h"

class PSetNetworkWrapper;
class CPsetContainer;
class CPsetNetwork;
class CPsetCustomerServiceProfile;
class CPSetRefreshHandler;

class PSetNetworkWrapperPrivate
    :
    public MPsetNetworkInfoObserver, 
    public MPsetNetworkModeObserver,
    public MSSSettingsRefreshObserver
{
public:
    
    PSetNetworkWrapperPrivate(
        PSetNetworkWrapper &owner,
        CPsetContainer &psetContainer);
    
    virtual ~PSetNetworkWrapperPrivate();

public:
    
    void getAvailableNetworks();

    PSetNetworkWrapper::ErrorCode getNetworkSelectionMode(
        PSetNetworkWrapper::NetworkSelectionMode& mode) const;
    
    void selectNetwork( 
        const PSetNetworkWrapper::NetworkInfo& info);
    
    void cancelRequest();
    
public:

    void getNetworkAccessMode() const;

    void setNetworkAccessMode(PSetNetworkWrapper::NetworkAccessMode mode);
    
    bool isManualNetworkSelectionSupported() const;

public: // From MPsetNetworkInfoObserver
    
    void HandleNetworkInfoReceivedL( 
        const CNetworkInfoArray* aInfoArray, 
        const TInt aResult );
    
    void HandleCurrentNetworkInfoL( 
        const MPsetNetworkSelect::TCurrentNetworkInfo& aCurrentInfo, 
        const TInt aResult );

    void HandleNetworkChangedL( 
        const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
        const MPsetNetworkSelect::TCurrentNetworkStatus aStatus, 
        const TInt aResult );    

    void HandleNetworkChangedL( 
        const MPsetNetworkSelect::TNetworkInfo& aCurrentInfo,
        const RMobilePhone::TMobilePhoneRegistrationStatus& aStatus, 
        const TInt aResult );    

    void HandleSearchingNetworksL( 
        MPsetNetworkInfoObserver::TServiceRequest aRequest );

    void HandleRequestingSelectedNetworkL( 
        TBool aOngoing );

    void HandleCallActivatedL();

    void HandleNetworkErrorL( 
        const MPsetNetworkInfoObserver::TServiceRequest aRequest,
        const TInt aError );

public: // From MPsetNetworkModeObserver

    void HandleNetworkSystemModeEventsL( 
        const MPsetNetworkModeObserver::TServiceRequest aRequest,
        const TUint32 aNetworkModeCaps );

    void HandleNetworkErrorL(
        const MPsetNetworkModeObserver::TServiceRequest aRequest,
        const TInt aError );
    
public: // From MSSSettingsRefreshObserver

        TBool AllowRefresh(
            const TSatRefreshType aType,
            const TSatElementaryFiles aFiles );

        void Refresh(
            const TSatRefreshType aType,
            const TSatElementaryFiles aFiles );

private:
    
    PSetNetworkWrapper::ErrorCode ConvertToQtErrorCode(
        TInt symbianErrorCode) const;
    
private:
    
    /*! Owner. */
    PSetNetworkWrapper &m_owner;
    
    // TODO: better comments
    /*! Own. Network object used to observe network information. */
    QScopedPointer<CPsetNetwork> m_psetNetwork;
    
    /*! Own. Network object used to observe network mode changes. */
    QScopedPointer<CPsetNetwork> m_psetNetworkMode;
    
    QList<PSetNetworkWrapper::NetworkInfo*> m_networkInfoList;
    
    /*! Own. */
    QScopedPointer<CPsetCustomerServiceProfile> m_csp;
    
    /*! Own. */
    QScopedPointer<CPSetRefreshHandler> m_refreshHandler;
    
};

#endif // PSETNETWORKWRAPPERPRIVATE_H
