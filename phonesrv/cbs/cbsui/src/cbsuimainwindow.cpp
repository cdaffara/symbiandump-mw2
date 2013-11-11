/*
* Copyright (c) 2010  Nokia Corporation and/or its subsidiary(-ies).
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

#include "rcbsui.h"
#include "cbsuiviewmanager.h"
#include "cbslog.h"

#include "cbsuimainwindow.h"

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// CbsUiMainWindow::CbsUiMainWindow
// ----------------------------------------------------------------------------
//
CbsUiMainWindow::CbsUiMainWindow()
{
    LOG(NORMAL,"CBSUI: CbsUiMainWindow::CbsUiMainWindow IN");    
    // Create connection to the server. SYMBIAN/S60 CODE
    mServer = new RCbsUi;    
    if ( mServer ) {
        // Connect Cbs Server
        int result = mServer->Connect();
        LOG2(NORMAL,"CBSUI: CbsUiMainWindow::CbsUiMainWindow \
                connect server %d", result);
        // Construct the view manager.
        mViewManager = new CbsUiViewManager(this, *mServer);        
    }  
    LOG(NORMAL,"CBSUI: CbsUiMainWindow::CbsUiMainWindow OUT");
}

// ----------------------------------------------------------------------------
// CbsUiMainWindow::~CbsUiMainWindow
// ----------------------------------------------------------------------------
//
CbsUiMainWindow::~CbsUiMainWindow()
{
    LOG(NORMAL,"CBSUI: CbsUiMainWindow::~CbsUiMainWindow IN");
    mServer->Close();
    delete mServer;
    LOG(NORMAL,"CBSUI: CbsUiMainWindow::~CbsUiMainWindow OUT");
}

// End of file
