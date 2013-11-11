/*
* Copyright (c) 2002-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Factory for event monitors
*
*/


// INCLUDE FILES
#include    "TSatEventMonitorFactory.h"
#include    "MSatEventMonitor.h"
#include    "CSatSimRemovalMonitor.h"
#include    "CSatUserActivityMonitor.h"
#include    "CSatIdleScreenMonitor.h"
#include    "CSatLanguageSelectionMonitor.h"
#include    "CSatBrowserTerminationMonitor.h"
#include    "CSatBIPDataAvailableMonitor.h"
#include    "CSatBIPChannelStatusMonitor.h"
#include    "SatLog.h"

// ============================ LOCAL FUNCTIONS ================================

// -----------------------------------------------------------------------------
// CleanupEventMonitors
// Remove and destroy items from the specified array.
// @param aArray An array whose items need to be removed and destroyed.
// -----------------------------------------------------------------------------
//
LOCAL_C void CleanupEventMonitors( TAny* aArray )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: CleanupEventMonitors calling" )
    
    CArrayPtrFlat<MSatEventMonitor>* array = 
        static_cast< CArrayPtrFlat< MSatEventMonitor >* >(aArray);
    array->ResetAndDestroy();
    
    LOG( SIMPLE, "SATEVENTMONITORS: CleanupEventMonitors exiting" )
    }

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// TSatEventMonitorFactory::CreateEventMonitorsL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C CArrayPtrFlat<MSatEventMonitor>* TSatEventMonitorFactory::
    CreateEventMonitorsL( MSatEventMonitorUtils& aUtils )
    {
    LOG( SIMPLE, "SATEVENTMONITORS: TSatEventMonitorFactory::\
        CreateEventMonitorsL calling" )

    CArrayPtrFlat<MSatEventMonitor>* array =
        new( ELeave ) CArrayPtrFlat<MSatEventMonitor>( 1 );
    CleanupStack::PushL( TCleanupItem(CleanupEventMonitors, array) );
    
    MSatEventMonitor* eventMonitor = NULL;
    // Add event monitors
    eventMonitor = 
        CSatSimRemovalMonitor::NewL( aUtils );
    CleanupStack::PushL( eventMonitor );
    array->AppendL( eventMonitor );
    CleanupStack::Pop( eventMonitor );
    
    eventMonitor = 
        CSatUserActivityMonitor::NewL( aUtils );
    CleanupStack::PushL( eventMonitor );
    array->AppendL( eventMonitor );
    CleanupStack::Pop( eventMonitor );
    
    eventMonitor = 
        CSatIdleScreenMonitor::NewL( aUtils );
    CleanupStack::PushL( eventMonitor );
    array->AppendL( eventMonitor );
    CleanupStack::Pop( eventMonitor );    
        
    eventMonitor = 
        CSatLanguageSelectionMonitor::NewL( aUtils );
    CleanupStack::PushL( eventMonitor );
    array->AppendL( eventMonitor );
    CleanupStack::Pop( eventMonitor ); 
    
    eventMonitor = 
        CSatBrowserTerminationMonitor::NewL( aUtils );
    CleanupStack::PushL( eventMonitor );
    array->AppendL( eventMonitor );
    CleanupStack::Pop( eventMonitor ); 
    
    eventMonitor = 
        CSatBIPDataAvailableMonitor::NewL( aUtils );
    CleanupStack::PushL( eventMonitor );
    array->AppendL( eventMonitor );
    CleanupStack::Pop( eventMonitor );
    
    eventMonitor = 
        CSatBIPChannelStatusMonitor::NewL( aUtils );
    CleanupStack::PushL( eventMonitor );
    array->AppendL( eventMonitor );
    CleanupStack::Pop( eventMonitor );     

    CleanupStack::Pop( array );

    LOG( SIMPLE, "SATEVENTMONITORS: TSatEventMonitorFactory::\
        CreateEventMonitorsL exiting" )
    return array;
    }

//  End of File
