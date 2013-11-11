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
* Description:  CMnrpAppServer class implementation
*
*/


#include <mnappserviceuids.hrh>
#include "debug.h"

#include <lbsposition.h>
#include <epos_cposlandmark.h>

#include "mnrpnavigationservice.h"
#include "mnrpmapviewservice.h"

#include <MnRefProvider.rsg>
#include "mnrpappui.h"
#include "mnrpengine.h"
#include "mnrpappserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppServer::~CMnrpAppServer()
    {
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C CApaAppServiceBase* CMnrpAppServer::CreateServiceL(TUid aServiceType) const
    {  
    LOG1("MnRefProvider::CreateServiceL, svcUid = %X", aServiceType.iUid);
    switch ( aServiceType.iUid )
        {
        case KMnAppMapViewService:
            return CreateMapViewServiceL();

        case KMnAppNavigationService:
            return CreateNavigationServiceL();
            
        default:
            return CAknAppServer::CreateServiceL( aServiceType );
        }
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
EXPORT_C void CMnrpAppServer::HandleAllClientsClosed()
    {
    LOG( "MnRefProvider: HandleAllClientsClosed" );
    CMnrpAppUi* appUi = AppUi();
    if ( appUi )
    	{
    	if ( appUi->IsChainedMode() ) 
    		{
    		// close
    		CAknAppServer::HandleAllClientsClosed();
    		}
    	else // standalone mode
    		{
    		if ( !appUi->IsAnyUserViewActive() )
    			{
	    		// close
	    		CAknAppServer::HandleAllClientsClosed();
    			}
    		else
    			{	
    			// "remain active until user closes me"
    			}
    		}	
    	}
	else
        {
        CAknAppServer::HandleAllClientsClosed();
        }
    }
    
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpNavigationService* CMnrpAppServer::CreateNavigationServiceL() const
    {
    LOG( "MnRefProvider: CreateNavigationServiceL" );
    CMnrpNavigationService* svc = CMnrpNavigationService::NewL();
    return svc;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpMapViewService* CMnrpAppServer::CreateMapViewServiceL() const
    {
    LOG( "MnRefProvider: CreateMapViewServiceL" );
    CMnrpMapViewService* svc = CMnrpMapViewService::NewL();
    return svc;
    }

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
//
CMnrpAppUi* CMnrpAppServer::AppUi()
    {
    CMnrpAppUi* ui = (CMnrpAppUi*) CEikonEnv::Static()->AppUi();
    if ( !ui )
        {
        LOG("MnRefProvider: UI not found!");
        }
    return ui;
    }
