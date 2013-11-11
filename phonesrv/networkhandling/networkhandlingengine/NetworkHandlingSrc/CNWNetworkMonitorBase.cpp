/*
* Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  This module contains the implementation of CNWNetworkMonitorBase class 
*                member functions. 
*
*/


//  INCLUDE FILES
#include    "CNWMessageHandler.h"
#include    "CNWNetworkMonitorBase.h"
#include    "NWLogger.h"

// ================= MEMBER FUNCTIONS =========================================

// ----------------------------------------------------------------------------
// CNWNetworkMonitorBase::CNWNetworkMonitorBase
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CNWNetworkMonitorBase::CNWNetworkMonitorBase
    ( 
    CNWMessageHandler& aOwner, 
    RMobilePhone& aPhone,
    TNWInfo& aNetworkInfo, 
    RMmCustomAPI& aCustomAPI )
    : CActive( EPriorityStandard ), 
    iOwner( aOwner ),
    iPhone( aPhone ),
    iNWNetworkInfo( aNetworkInfo ),
    iCustomAPI( aCustomAPI ),
    iIsInitialised( EFalse )
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkMonitorBase::CNWNetworkMonitorBase() Called" );
    }

// Destructor
CNWNetworkMonitorBase::~CNWNetworkMonitorBase()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkMonitorBase::~CNWNetworkMonitorBase() Called" );
    }

// ----------------------------------------------------------------------------
// CNWNetworkMonitorBase::IsInitialised
// Returns is derived monitor class initialised.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
TBool CNWNetworkMonitorBase::IsInitialised() const
    {
    NWLOGSTRING2( KNWOBJECT,
        "NW: CNWNetworkMonitorBase::IsInitialised() Called,\
        iIsInitialised = %d", iIsInitialised );
    return iIsInitialised;
    }

// ----------------------------------------------------------------------------
// CNWNetworkMonitorBase::Initialise
// Function to that starts monitoring.
// Base class version of Iiinitialise. Calls pure virtual IssueRequest.
// (other items were commented in a header).
// ----------------------------------------------------------------------------
//
void CNWNetworkMonitorBase::Initialise()
    {
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkMonitorBase::Initialise() Begin" );
    
    IssueRequest();
    
    NWLOGSTRING( KNWOBJECT,
        "NW: CNWNetworkMonitorBase::Initialise() End" );
    }


//  End of File 
