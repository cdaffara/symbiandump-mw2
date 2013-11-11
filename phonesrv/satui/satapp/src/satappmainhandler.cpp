/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/
//Qt
#include <HbApplication>
#include <hbapplication.h>
#include <hbmessagebox.h>
#include <afactivitystorage.h>
#include <afstorageglobals.h>

// symbian
#include <centralrepository.h>
#include <coreapplicationuissdkcrkeys.h>

#include "satappmainhandler.h"
#include "satappserverdispatcher.h"
#include "satappmenuprovider.h"
#include "satappinputprovider.h"
#include "satapptoneprovider.h"
#include "satapppopupprovider.h"
#include "satappconstant.h"
#include "satappconfirmprovider.h"

// Activity ID for Sat Application
const char *SATAPP_ACTIVITY_ID = "SIMServicesList";

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// SatAppMainHandler::SatAppMainHandler
// (Construtor).
// ----------------------------------------------------------------------------
//
SatAppMainHandler::SatAppMainHandler(HbMainWindow &window, 
    QObject */*parent*/):
mOfflineWarningDlg(NULL)
{
    qDebug("SATAPP: SatAppMainHandler::SatAppMainHandler >");

    if(!isOffline()){
        mServer = new SatAppServerDispatcher(this);
        mMenu = new SatAppMenuProvider(&window, this);
        mInput = new SatAppInputProvider(this);
        mTone = new SatAppToneProvider(this);
        mPopup = new SatAppPopupProvider(this);
        mConfirm = new SatAppConfirmProvider(this);
        initConnections();
    }
    else{
        showOfflineWarning();
    }
    
    mActivityStorage = new AfActivityStorage(this);
    removeActivity();
    
    qDebug("SATAPP: SatAppMainHandler::SatAppMainHandler <");
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::~SatAppMainHandler
// (Destructor).
// ----------------------------------------------------------------------------
//
SatAppMainHandler::~SatAppMainHandler()
{
    qDebug("SATAPP: SatAppMainHandler::~SatAppMainHandler >");
    delete mOfflineWarningDlg;
    qDebug("SATAPP: SatAppMainHandler::~SatAppMainHandler <");
}

// ----------------------------------------------------------------------------
// Local override for connect-function
// this method ENSURES that the connection is succesful.
// ----------------------------------------------------------------------------
//
void doConnect(
    const QObject *sender,
    const char *signal,
    const QObject *receiver,
    const char *member)
{
    bool ret = QObject::connect(sender, signal,
                receiver, member, Qt::DirectConnection);
    Q_ASSERT_X(ret, "doConnect: connection failed for: ", signal);
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::initConnections
// 
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::initConnections()
{
    qDebug("SATAPP: SatAppMainHandler::initConnections >");

    // --------------------------------------
    // MenuProvider
    // --------------------------------------

    // SetupMenu command from server
    doConnect(
        mServer, SIGNAL( setUpMenu( SatAppAction &) ),
        mMenu, SLOT( setUpMenu( SatAppAction &) ) );
    
    // SelectItem command from server
    doConnect(
        mServer, SIGNAL( selectItem( SatAppAction &) ),
        mMenu, SLOT( selectItem( SatAppAction &) ) );

    // --------------------------------------
    // InputProvider
    // --------------------------------------

    // GetInkey command    
    doConnect(
        mServer, SIGNAL( getInkey( SatAppAction &) ),
        mInput, SLOT( getInkey( SatAppAction &) ) );
    // GetInput command
    doConnect(
        mServer, SIGNAL( getInput( SatAppAction &) ),
        mInput, SLOT( getInput( SatAppAction &) ) );

    // clearScreen in InputProvider
    doConnect(
        mServer, SIGNAL( clearScreen() ),
        mInput, SLOT( resetState() ));
 
    // --------------------------------------
    // Play tone
    // --------------------------------------
    // Play tone 
    doConnect(
        mServer, SIGNAL( playTone( SatAppAction &) ),
        mTone, SLOT( playTone( SatAppAction &) ) );

    // clearScreen in tone provider
    doConnect(
        mServer, SIGNAL( clearScreen() ),
        mTone, SLOT( clearScreen() ) );

    // --------------------------------------
    // Show Popups
    // --------------------------------------

    // display text
    doConnect(
        mServer, SIGNAL(displayText( SatAppAction & )),
        mPopup, SLOT(displayText( SatAppAction & )));

    // show notification
    doConnect(
        mServer, SIGNAL( notification( SatAppAction & ) ),
        mPopup, SLOT( notification( SatAppAction & ) ) );

    // hide wait note
    doConnect(
        mServer, SIGNAL( stopShowWaitNote() ),
        mPopup, SLOT( stopShowWaitNote() ) );

    // clearScreen in popup note provider
    doConnect(
        mServer, SIGNAL(clearScreen()),
        mPopup, SLOT(clearScreen()));
    
    // clearScreen in popup note provider
    doConnect(
        mServer, SIGNAL(showSsErrorNote()),
        mPopup, SLOT(showSsErrorNote()));    

    // --------------------------------------
    // Show Confirm
    // --------------------------------------

    // show confirmCommand
    doConnect(
        mServer, SIGNAL( confirmCommand( SatAppAction & ) ),
        mConfirm, SLOT( confirmCommand( SatAppAction & ) ) );

    // clearScreen
    doConnect(
        mServer, SIGNAL( clearScreen() ),
        mConfirm, SLOT( clearScreen() ) );

    // Task switcher
    doConnect(
        mServer, SIGNAL( setUpMenu( SatAppAction & ) ),
        this, SLOT( updateActivity() ) );

    doConnect(
        qApp, SIGNAL( aboutToQuit() ),
        this, SLOT( saveActivity() ) );

    qDebug("SATAPP: SatAppMainHandler::initConnections <");
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::updateActivity
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::updateActivity()
{
    qDebug("SATAPP: SatAppMainHandler::updateActivity >");
    mActivity.insert(ActivityScreenshotKeyword, 
        mMenu->takeScreenShot());
    qDebug("SATAPP: SatAppMainHandler::updateActivity <");
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::saveActivity
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::saveActivity()
{
    qDebug("SATAPP: SatAppMainHandler::saveActivity >");

    // Add the activity to the activity manager
    const bool ok = mActivityStorage->saveActivity(
        SATAPP_ACTIVITY_ID, QVariant(), mActivity);
    
    qDebug("SATAPP: SatAppMainHandler::saveActivity < %d", ok);
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::removeActivity
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::removeActivity()
{
    qDebug("SATAPP: SatAppMainHandler::removeActivity >");
        
   
    QVariantHash activity = mActivityStorage->activityMetaData(SATAPP_ACTIVITY_ID);
    if(activity.keys().contains(SATAPP_ACTIVITY_ID)) {
        mActivity = activity;
        qDebug("SATAPP: SatAppMenuProvider::removeActivity store");
    }
    const bool ok = mActivityStorage->removeActivity(SATAPP_ACTIVITY_ID);
    
    qDebug("SATAPP: SatAppMainHandler::removeActivity < %d", ok);
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::isOffline
// ----------------------------------------------------------------------------
//
bool SatAppMainHandler::isOffline()
{
    //If current active profile is offline, show a warning and quit
    qDebug("SATAPP: SatAppMainHandler::isOffline >");
    TInt profileId(0);
    CRepository* cr (NULL);
    TRAPD(err, cr = CRepository::NewL(KCRUidCoreApplicationUIs));
    if ( KErrNone == err )
    {
        // Get the ID of the currently active profile:
        const TInt error = 
            cr->Get(KCoreAppUIsNetworkConnectionAllowed, profileId);
        qDebug("SATAPP: SatAppMainHandler::isOffline get active \
                profile error=%d",error);
        delete cr;
    }
    qDebug("SATAPP: SatAppMainHandler::isOffline< profileId = %d",profileId);
    return ( ECoreAppUIsNetworkConnectionNotAllowed == profileId );
}

// ----------------------------------------------------------------------------
// SatAppMainHandler::showOfflineWarning
// ----------------------------------------------------------------------------
//
void SatAppMainHandler::showOfflineWarning()
{
    qDebug("SATAPP: SatAppMainHandler::showOfflineWarning >");
    mOfflineWarningDlg = 
        new HbMessageBox(HbMessageBox::MessageTypeWarning);
    mOfflineWarningDlg->setText(
        hbTrId("txt_simatk_dpopinfo_sim_services_not_available"));
    mOfflineWarningDlg->clearActions();
    mOfflineWarningDlg->setDismissPolicy(HbDialog::TapOutside);
    mOfflineWarningDlg->setTimeout(KDisplayTxtUserClearTimeout);
    SAT_ASSERT(connect(mOfflineWarningDlg, SIGNAL(aboutToClose()),
                qApp, SLOT(quit())));
    mOfflineWarningDlg->open();
    qDebug("SATAPP: SatAppMainHandler::showOfflineWarning <");
}

//End of file
