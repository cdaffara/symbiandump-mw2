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
* Description: Provides and maintains SAT main view (selection list)
*
*/

// qt
#include <QObject>
#include <QTimer>
#include <QPixmap>
#include <QtDebug>
// hb
#include <hbmainwindow.h>
#include <hbdocumentloader.h>
#include <hbview.h>
#include <hbmenu.h>
#include <hblabel.h>
#include <hbaction.h>
#include <hblistwidget.h>
// satapp
#include "satappmenuprovider.h"
#include "satappaction.h"
#include "satappconstant.h"

// constants
const char *SATAPP_DOCML = ":/xml/satapp.docml";
const char *SATAPP_SETUPMENU_VIEW = "setupmenu_view";
const char *SATAPP_SELECTITEM_VIEW = "selectitem_view";
const char *SATAPP_MENUITEM = "s:ListWidget";
const char *SATAPP_SELECTITEM = "t:ListWidget";
const char *SATAPP_SUBTITLE = "t:label";


// ----------------------------------------------------------------------------
// SatAppMenuProvider::SatAppMenuProvider
// ----------------------------------------------------------------------------
//
SatAppMenuProvider::SatAppMenuProvider(HbMainWindow *window,QObject *parent)
    : QObject(parent), mMainWindow(window),mSetupMenuView(0),
      mSelectItemView(0),mSoftKeyQuitAction(0),mSoftKeyBackAction(0),
      mListWidget(0),mSelectListWidget(0),mSubTitle(0),mCurrentAction(0)
{
    qDebug("SATAPP: SatAppMenuProvider::SatAppMenuProvider");
}

// ----------------------------------------------------------------------------
// SatAppMenuProvider::~SatAppMenuProvider
// ----------------------------------------------------------------------------
//
SatAppMenuProvider::~SatAppMenuProvider()
{
    qDebug("SATAPP: SatAppMenuProvider::~SatAppMenuProvider >");
    resetState();
    // delete all objects created from DOCML.
    while (!mObjects.isEmpty()) {
        delete mObjects.takeFirst();
    }
    // delete actions
    delete mSoftKeyQuitAction;
    delete mSoftKeyBackAction;
    qDebug("SATAPP: SatAppMenuProvider::~SatAppMenuProvider <");
}

// ----------------------------------------------------------------------------
// SatAppMenuProvider::takeScreenShot
// ----------------------------------------------------------------------------
//
QPixmap SatAppMenuProvider::takeScreenShot()
{
    qDebug("SATAPP: SatAppMenuProvider::takeScreenShot");
    return QPixmap::grabWidget(mMainWindow, mMainWindow->rect());
}

// ----------------------------------------------------------------------------
// SatAppMenuProvider::loadMainView
// ----------------------------------------------------------------------------
//
void SatAppMenuProvider::loadMainView()
{
    qDebug("SATAPP: SatAppMenuProvider::loadMainView >");

    // create actions
    mSoftKeyQuitAction = new HbAction(Hb::QuitNaviAction,this);
    mSoftKeyBackAction = new HbAction(Hb::BackNaviAction,this);

    bool docmlLoad = false;
    HbDocumentLoader loader;
    // ownership of the objects are transferred to caller
    mObjects = loader.load(SATAPP_DOCML,&docmlLoad);
    Q_ASSERT(docmlLoad);
    qDebug("SATAPP: SatAppMenuProvider main view found");
    // load setupmenu view
    mSetupMenuView = qobject_cast<HbView *>(loader.findWidget
            (SATAPP_SETUPMENU_VIEW));
    // find setupmenu view items
    mListWidget = qobject_cast<HbListWidget *>
        ( loader.findWidget(SATAPP_MENUITEM ));
    // connect setupmenu view menu
    HbAction *menuAction = mSetupMenuView->menu()->addAction(
        hbTrId("txt_common_menu_exit"));
    SAT_ASSERT(connect(menuAction, SIGNAL(triggered()),
        mMainWindow, SLOT(close())));
    SAT_ASSERT(connect(mSoftKeyQuitAction, SIGNAL(triggered()),
        mMainWindow, SLOT(close())));
    // set this view as current view
    mMainWindow->addView(mSetupMenuView);
    mMainWindow->setCurrentView(mSetupMenuView);

    // load selectitem view
    mSelectItemView = qobject_cast<HbView *>(loader.findWidget
            (SATAPP_SELECTITEM_VIEW));
    // find selectitem view items
    mSubTitle = qobject_cast<HbLabel *>
        ( loader.findWidget(SATAPP_SUBTITLE) );
    mSelectListWidget = qobject_cast<HbListWidget *>
        ( loader.findWidget(SATAPP_SELECTITEM ));
    // connect selectitem view menu
    HbAction *menuBack = mSelectItemView->menu()->addAction(
        hbTrId("txt_common_opt_back"));
    SAT_ASSERT(connect(menuBack, SIGNAL(triggered()),
        mSoftKeyBackAction, SIGNAL(triggered())));
    HbAction *menuQuit = mSelectItemView->menu()->addAction(
        hbTrId("txt_common_menu_exit"));

    SAT_ASSERT(connect(menuQuit, SIGNAL(triggered()),
        mMainWindow, SLOT(close())));
    qDebug("SATAPP: SatAppMenuProvider::loadMainView <");

}

// ----------------------------------------------------------------------------
// switchView
// ----------------------------------------------------------------------------
//
void SatAppMenuProvider::switchView()
{
    qDebug("SATAPP: SatAppMenuProvider::switchView >");
    if (mCurrentAction && ESatSelectItemAction == mCurrentAction->action()) {
        // switch to select item view
        mMainWindow->removeView(mMainWindow->currentView());
        mMainWindow->addView(mSelectItemView);
        mMainWindow->setCurrentView(mSelectItemView);
    } else {
        // switch to main menu view
        mMainWindow->removeView(mMainWindow->currentView());
        mMainWindow->addView(mSetupMenuView);
        mMainWindow->setCurrentView(mSetupMenuView);
    }
    qDebug("SATAPP: SatAppMenuProvider::switchView <");
}


// ----------------------------------------------------------------------------
// SatAppMenuProvider::setUpMenu
// constructs a menu in the main SAT application view
// ----------------------------------------------------------------------------
//
void SatAppMenuProvider::setUpMenu(SatAppAction &action)
{
    qDebug("SATAPP: SatAppMenuProvider::setUpMenu >");
    if (!mSetupMenuView) {
        loadMainView();
    }
    resetState();
    mCurrentAction = &action;
    switchView();
    action.setResponse(ESatSuccess);

    // The text is the application name
    // should be shown on the menu area always
    QString text = action.value(KeyText).toString();
    if (!text.isEmpty()) {
        mSetupMenuView->setTitle(text);
        mSelectItemView->setTitle(text);
    }
    qDebug() << "SATAPP: SetUpMenu title=" << text;

    // build the menu
    QList<QVariant> menu = action.value(KeyMenu).toList();
    mListWidget->clear();
    for(int i = 0; i < menu.count(); i++ ) {
        mListWidget->addItem(menu.at(i).toString());
    }
    qDebug() << "SATAPP: SetUpMenu menu=" << menu;

    // back-key quits the application
    mSetupMenuView->setNavigationAction(mSoftKeyQuitAction);
    // completes action with ESatSuccess and selected item
    connectItem();

    qDebug("SATAPP: SatAppMenuProvider::setUpMenu <");
}


// ----------------------------------------------------------------------------
// SatAppMenuProvider::selectItem
// Constructs a SETECT ITEM view
// ----------------------------------------------------------------------------
//
void SatAppMenuProvider::selectItem( SatAppAction &action)
{
    qDebug("SATAPP: SatAppMenuProvider::selectItem >");
    
    if (!mSetupMenuView) {
        loadMainView();
    }
    
    resetState();
    mCurrentAction = &action;
    switchView();
    action.setResponse(ESatSuccess);

    // Set sub title
    QString text = action.value(KeyText).toString();
    if (!text.isEmpty()) {
        mSubTitle->setPlainText(text);
    } else {
        mSubTitle->setPlainText(hbTrId("txt_long_caption_sim_services"));
    }
    qDebug() << "SATAPP: selectItem text=" << text;

    // Set List widget
    QList<QVariant> menu = action.value(KeyMenu).toList();
    mSelectListWidget->clear();
    for(int i = 0; i < menu.count(); i++ ) {
        mSelectListWidget->addItem(menu.at( i ).toString());
    }
    qDebug() << "SATAPP: selectItem menu=" << menu;

    // timeout completes action with ESatNoResponseFromUser
    QTimer::singleShot(KDefaultSelectionTimeoutMseconds,
        mCurrentAction,SLOT(completeWithNoResponse()));
    // back-key completes action with ESatBackwardModeRequestedByUser
    mSelectItemView->setNavigationAction(mSoftKeyBackAction);
    SAT_ASSERT(connect(mSoftKeyBackAction, SIGNAL(triggered()),
        mCurrentAction, SLOT(completeWithBackRequested())));
    SAT_ASSERT(connect(mSelectItemView->menu()->actions().at(1), 
        SIGNAL(triggered()),
        mCurrentAction, SLOT(completeWithSessionTerminated())));
    
    // completes action with ESatSuccess and selected item
    connectItem();

    qDebug("SATAPP: SatAppMenuProvider::selectItem <");
}

// ----------------------------------------------------------------------------
// SatAppMenuProvider::resetState
// terminates ongoing events and clears the screen
// ----------------------------------------------------------------------------
//
void SatAppMenuProvider::resetState()
{
    qDebug("SATAPP: SatAppMenuProvider::resetState >");
    mCurrentAction = 0;
    disconnectItem();
    qDebug("SATAPP: SatAppMenuProvider::resetState <");
}

// ----------------------------------------------------------------------------
// SatAppMenuProvider::menuItemSelected
// (Menu item selected).
// ----------------------------------------------------------------------------
//
void SatAppMenuProvider::menuItemSelected(HbListWidgetItem *item)
{
    qDebug("SATAPP: SatAppMenuProvider::menuItemSelected >");
    Q_ASSERT(mCurrentAction);
    if (ESatSetUpMenuAction == mCurrentAction->action()) {
        int index = mListWidget->row(item);
        mCurrentAction->set(KeySelection,index);
        mCurrentAction->set(KeyHelpRequested,false);
        mCurrentAction->completeWithSuccess();
    } else if(ESatSelectItemAction == mCurrentAction->action()) {
        int index = mSelectListWidget->row(item);
        mCurrentAction->set(KeySelection,index);
        mCurrentAction->completeWithSuccess();
    }
    qDebug("SATAPP: SatAppMenuProvider::menuItemSelected <");
}


// ----------------------------------------------------------------------------
// SatAppMenuProvider::connectItem
// (Connect listwidget item).
// ----------------------------------------------------------------------------
//
void SatAppMenuProvider::connectItem()
{
    qDebug("SATAPP: SatAppMenuProvider::connectItem >");
    Q_ASSERT(mCurrentAction);
    if (ESatSetUpMenuAction == mCurrentAction->action()) {
        SAT_ASSERT(connect(
            mListWidget, SIGNAL(activated(HbListWidgetItem *)), 
            this, SLOT(menuItemSelected(HbListWidgetItem *))));
        SAT_ASSERT(connect(
            mListWidget, 
            SIGNAL(longPressed(HbListWidgetItem *, const QPointF &)), 
            this, SLOT(menuItemSelected(HbListWidgetItem *))));
    } else if(mCurrentAction->action()== ESatSelectItemAction) {
        SAT_ASSERT(connect(
            mSelectListWidget, SIGNAL(activated(HbListWidgetItem *)), 
            this, SLOT(menuItemSelected(HbListWidgetItem *))));
        SAT_ASSERT(connect(
            mSelectListWidget, 
            SIGNAL(longPressed(HbListWidgetItem*, const QPointF &)), 
            this, SLOT(menuItemSelected(HbListWidgetItem *))));
    }
    qDebug("SATAPP: SatAppMenuProvider::connectItem <");
}

// ----------------------------------------------------------------------------
// SatAppMenuProvider::disconnectItem
// (Disconnect listwidget item).
// ----------------------------------------------------------------------------
//
void SatAppMenuProvider::disconnectItem()
{
    qDebug("SATAPP: SatAppMenuProvider::disconnectItem >");
    // setup menu view
    disconnect(mListWidget, SIGNAL( activated(HbListWidgetItem *)),
          this, SLOT( menuItemSelected( HbListWidgetItem *)));        
    disconnect(
        mListWidget, SIGNAL(longPressed(HbListWidgetItem*, const QPointF &)), 
        this, SLOT(menuItemSelected(HbListWidgetItem *)));
    // select item view
    disconnect(mSelectListWidget, SIGNAL( activated(HbListWidgetItem *)),
          this, SLOT( menuItemSelected( HbListWidgetItem *)));
    disconnect(
        mSelectListWidget, 
        SIGNAL(longPressed(HbListWidgetItem*, const QPointF &)), 
        this, SLOT(menuItemSelected(HbListWidgetItem *)));
    qDebug("SATAPP: SatAppMenuProvider::disconnectItem <");
}

//End of file
