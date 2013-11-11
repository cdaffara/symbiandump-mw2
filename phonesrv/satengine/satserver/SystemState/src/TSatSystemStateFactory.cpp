/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Provides SAT SystemState objects outside of this DLL
*
*/

#include    <startupdomainpskeys.h>
#include    <PSVariables.h>
#define Q_OS_SYMBIAN // needed to activate homescreendomainpskeys.h
#include    <homescreendomainpskeys.h>
#include    <CommonEngineDomainCRKeys.h>
#include    <BTSapDomainPSKeys.h>
#include    <coreapplicationuissdkcrkeys.h>
#include    "TSatSystemStateFactory.h"
#include    "CSatSystemState.h"
#include    "msatmultimodeapi.h"
#include    "CSatPSChangeNotifier.h"
#include    "CSatCenRepChangeNotifier.h"
#include    "SatLog.h"

// ======== MEMBER FUNCTIONS ========

// -----------------------------------------------------------------------------
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C MSatSystemState* TSatSystemStateFactory::CreateSystemStateL(
    MSatMultiModeApi& aPhone )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::CreateSystemStateL \
        calling" )

    MSatSystemState* systemState = CSatSystemState::NewL( aPhone );

    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::CreateSystemStateL \
        exiting" )
    return systemState;
    }

// -----------------------------------------------------------------------------
// TSatSystemStateFactory::CreateIdleModeChangeNotifierL
// -----------------------------------------------------------------------------
//
EXPORT_C MSatSystemStateChangeNotifier*
    TSatSystemStateFactory::CreateIdleModeChangeNotifierL(
    MSatSystemStateObserver& aObserver )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateIdleModeChangeNotifierL calling" )

    MSatSystemStateChangeNotifier* ret = CSatPSChangeNotifier::NewL(
        KHsCategoryUid,
        KHsCategoryStateKey,
        aObserver,
        EHomeScreenWidgetViewForeground );

    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateIdleModeChangeNotifierL exiting" )
    return ret;
    }

// -----------------------------------------------------------------------------
// TSatSystemStateFactory::CreateSimStatusChangeNotifierL
// -----------------------------------------------------------------------------
//
EXPORT_C MSatSystemStateChangeNotifier*
    TSatSystemStateFactory::CreateSimStatusChangeNotifierL(
    MSatSystemStateObserver& aObserver )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateSimStatusChangeNotifierL calling" )

    MSatSystemStateChangeNotifier* ret = CSatPSChangeNotifier::NewL(
        KPSUidStartup,
        KPSSimStatus,
        aObserver,
        ESimNotPresent );

    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateSimStatusChangeNotifierL exiting" )
    return ret;
    }

// -----------------------------------------------------------------------------
// TSatSystemStateFactory::CreateSIMAccessProfileChangeNotifierL
// -----------------------------------------------------------------------------
//
EXPORT_C MSatSystemStateChangeNotifier*
    TSatSystemStateFactory::CreateSIMAccessProfileChangeNotifierL(
    MSatSystemStateObserver& aObserver )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateSIMAccessProfileChangeNotifier calling" )

    MSatSystemStateChangeNotifier* ret = CSatPSChangeNotifier::NewL(
        KPSUidBluetoothSapConnectionState,
        KBTSapConnectionState,
        aObserver,
        KNotifyValueNotSet ); // No notify value specified

    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateSIMAccessProfileChangeNotifier exiting" )
    return ret;
    }

// -----------------------------------------------------------------------------
// TSatSystemStateFactory::CreateStartupChangeChangeNotifierL
// -----------------------------------------------------------------------------
//
EXPORT_C MSatSystemStateChangeNotifier*
    TSatSystemStateFactory::CreateStartupChangeNotifierL(
    MSatSystemStateObserver& aObserver )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateStartupChangeNotifierL calling" )

    MSatSystemStateChangeNotifier* ret = CSatPSChangeNotifier::NewL(
        KPSUidStartup,
        KPSGlobalSystemState,
        aObserver,
        KNotifyValueNotSet );

    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateStartupChangeNotifierL exiting" )
    return ret;
    }

// -----------------------------------------------------------------------------
// TSatSystemStateFactory::CreateLanguageSelectionChangeNotifierL
// -----------------------------------------------------------------------------
//
EXPORT_C MSatSystemStateChangeNotifier*
    TSatSystemStateFactory::CreateLanguageSelectionChangeNotifierL(
    MSatSystemStateObserver& aObserver )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateLanguageSelectionChangeNotifier calling" )

    MSatSystemStateChangeNotifier* ret = CSatCenRepChangeNotifier::NewL(
        KCRUidCommonEngineKeys,
        KGSDisplayTxtLang,
        aObserver,
        CSatCenRepChangeNotifier::ECRTypeInteger );

    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateLanguageSelectionChangeNotifier exiting" )
    return ret;
    }

// -----------------------------------------------------------------------------
// TSatSystemStateFactory::CreateProfileChangeNotifierL
// -----------------------------------------------------------------------------
//
EXPORT_C MSatSystemStateChangeNotifier*
    TSatSystemStateFactory::CreateProfileChangeNotifierL(
    MSatSystemStateObserver& aObserver )
    {
    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateProfileChangeNotifierL calling" )

    MSatSystemStateChangeNotifier* ret = CSatCenRepChangeNotifier::NewL(
        KCRUidCoreApplicationUIs,
        KCoreAppUIsNetworkConnectionAllowed,
        aObserver,
        CSatCenRepChangeNotifier::ECRTypeInteger );

    LOG( SIMPLE, "SATSYSTEMSTATE: TSatSystemStateFactory::\
        CreateProfileChangeNotifierL exiting" )
    return ret;
    }
