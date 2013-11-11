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
 * Description: This is for Positioning Supl Settings plugin 
 *							for Positioning Settings's Advanced view
 *
 */
#include "possuplsettings.h"
#include "possuplsettingsengine.h"
#include "possettingsadvop.h"

#include <QtCore>
#include <QDebug>
#include <hbcheckbox.h>
#include <hbmenu.h>
#include <hbaction.h>
#include <hbradiobuttonlist.h>
#include <hblabel.h>
#include <hbdialog.h>


// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::PosSuplSettingsPlugin
// ---------------------------------------------------------------------------
//
PosSuplSettingsPlugin::PosSuplSettingsPlugin() :
    mSuplSettingsEngine(0), mGroup(0), mOperation(0), mSignalMapper(0),
            mMainMenu(0), mPrimaryAction(0), mRadioButtonList(0),
            mDeleteQuery(0)
    {
    qDebug() << "+ PosSuplSettingsPlugin::PosSuplSettingsPlugin()";
    mSuplSettingsEngine = new PosSuplSettingsEngine();
    qDebug() << "- PosSuplSettingsPlugin::PosSuplSettingsPlugin()";
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::~PosSuplSettingsPlugin
// ---------------------------------------------------------------------------
//
PosSuplSettingsPlugin::~PosSuplSettingsPlugin()
    {
    qDebug() << "+ PosSuplSettingsPlugin::~PosSuplSettingsPlugin()";

    if (mSuplSettingsEngine)
        {
        delete mSuplSettingsEngine;
        mSuplSettingsEngine = 0;
        }

    if (mGroup && mOperation)
        {
        mOperation->removeItem(mGroup);
        mGroup = 0;
        }

    if (mMainMenu)
        {
        delete mMainMenu;
        mMainMenu = 0;
        }
  ///// 
        QObject::disconnect(mSignalMapper, SIGNAL(mapped(int)), this,
                SLOT(changeServerUsage(int))); 

    if (mSignalMapper)
        {
        delete mSignalMapper;
        mSignalMapper = NULL;
        }

    if (mPrimaryAction)
        {
        delete mPrimaryAction;
        mPrimaryAction = 0;
        }

    if (mRadioButtonList)
        {
        mRadioButtonList->reset();
        delete mRadioButtonList;
        }

    if (mDeleteQuery)
        {
        delete mDeleteQuery;
        }
        ///

    // delete the signal mapper instances 
    int cnt = mSignalMapperList.count();
    for (int i = 0; i < cnt ; ++i)
        {
        QObject::disconnect(mSignalMapperList[i], SIGNAL(signalmapped()),
                mSignalMapper, SLOT(map()));
        delete mSignalMapperList[i];
        mSignalMapperList[i] = NULL;
        }
        
        ///

    mServerList.clear();
    mSignalMapperList.clear();
    mDeleteableServerList.clear();
    qDebug() << "- PosSuplSettingsPlugin::~PosSuplSettingsPlugin()";
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::initialise
// ---------------------------------------------------------------------------
//
int PosSuplSettingsPlugin::initialise(PosSettingsAdvOp* operation)
    {
    qDebug() << "+ PosSuplSettingsPlugin::initialise()";

    int result = 0;
    // Assign to the member variable
    mOperation = operation;

    // create the signal mapper
    mSignalMapper = new QSignalMapper();

    //get the list of server present for positioning
    if (mSuplSettingsEngine)
        {
        result = mSuplSettingsEngine->suplServerList(mServerList);
        }

    // if no error while getting the server list then add the group
    // for Positioning servers
    if (result == 0)
        {
        mGroup = new HbDataFormModelItem(HbDataFormModelItem::GroupItem,
                hbTrId("txt_loe_subtitle_positioning_servers"), 0);
        //Add the group to the data form
        operation->appendDataFormItem(mGroup);
        //list all the server inside the group
        listServers();
        }
    QObject::connect(mSuplSettingsEngine, SIGNAL(serverListUpdated()), this,
            SLOT(updateList()));

    qDebug() << "- PosSuplSettingsPlugin::initialise()" << "result = "
            << result;

    return result;
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::ChangeServerUsage
// ---------------------------------------------------------------------------
//
void PosSuplSettingsPlugin::changeServerUsage(int value)
    {
    qDebug() << "+ PosSuplSettingsPlugin::changeServerUsage()";
    //toggle the value based on the slpid
    if (!mServerList[value].serverEnabled)
        {
        mSuplSettingsEngine->enableServer(mServerList[value].slpId, true);
        }
    else
        {
        mSuplSettingsEngine->enableServer(mServerList[value].slpId, false);
        }
    qDebug() << "- PosSuplSettingsPlugin::changeServerUsage()";
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::listServers
// ---------------------------------------------------------------------------
//
void PosSuplSettingsPlugin::listServers()
    {
    qDebug() << "+ PosSuplSettingsPlugin::listServers()";

    qDebug() << "mServerList.count = " << mServerList.count();

    //create a serverlist iterator
    QList<PosServerParams>::const_iterator serveriterator;

    // Get the server names and display
    for (serveriterator = mServerList.begin(); serveriterator
            != mServerList.end(); ++serveriterator)
        {
        qDebug() << "server name = " << serveriterator->serverAdd;
        if (serveriterator->serverAdd.length() > 0)
            {
            //for each server create new checkbox item 
            HbDataFormModelItem* checkBox = new HbDataFormModelItem(
                    HbDataFormModelItem::CheckBoxItem, QString(), 0);

            //set the widget data to the server name
            checkBox->setContentWidgetData("text", QString(hbTrId(
                    "txt_loe_list_use_1")).arg(serveriterator->serverAdd));

            //if the server is enabled set the initial state of the checkbox
            if (serveriterator->serverEnabled)
                {
                checkBox->setContentWidgetData("checkState", Qt::Checked);
                }
            else
                {
                checkBox->setContentWidgetData("checkState", Qt::Unchecked);
                }

            qDebug() << "Adding checkbox in the group";
            
            //this part of code is called when the servers are added and 
            //Advanced view is in the background. Since there is no group
            // a new group has to be created to add the servers
            mOperation->appendDataFormItem(checkBox, mGroup);

            //create an object of dummy signal mapper class
            //this is done to send the index info to the slots
            //since checkbox is not of QObject type and dummy class is 
            // created.
            PosSettingsSignalMapper* signalMapper =
                    new PosSettingsSignalMapper();
            mSignalMapperList.append(signalMapper);

            //add connection for statechanged of the checkbox, depending on which the
            // add connections on change of check box state
            mOperation->addConnection(checkBox, SIGNAL(stateChanged(int)),
                    mSignalMapperList[serveriterator - mServerList.begin()],
                    SLOT(signalmap()));

            QObject::connect(mSignalMapperList[serveriterator
                    - mServerList.begin()], SIGNAL(signalmapped()),
                    mSignalMapper, SLOT(map()));

            // set mapping of each check box item to its index
            if (mSignalMapper)
                {
                mSignalMapper->setMapping(mSignalMapperList[serveriterator
                        - mServerList.begin()], (serveriterator
                        - mServerList.begin()));
                }
            }
        }

    //use signal mapper to connect
    QObject::connect(mSignalMapper, SIGNAL(mapped(int)), this,
            SLOT(changeServerUsage(int)));

    qDebug() << "mDeleteableServerList.count before reset = "
            << mDeleteableServerList.count();

    //refresh the list of deleteable servers in case any deleteable server is added
    mDeleteableServerList.clear();

    if (mSuplSettingsEngine)
        {
        mSuplSettingsEngine->deletableServerList(mDeleteableServerList);
        }

    qDebug() << "mDeleteableServerList.count after fetch = "
            << mDeleteableServerList.count();

    //if the server list is <=1 while updating the list the menu is destroyed
    if (mDeleteableServerList.count() == 0)
        {
        if (mMainMenu)
            {
            delete mMainMenu;
            mMainMenu = 0;
            }
        }
    else 
        {
        delete mMainMenu;
        mMainMenu = 0;
        createMenu();
        }
    qDebug() << "-  PosSuplSettingsPlugin::listServers()";
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::updateList
// ---------------------------------------------------------------------------
//
void PosSuplSettingsPlugin::updateList()
    {
    qDebug() << "+ PosSuplSettingsPlugin::updateList()";
    int result = 0;
    int oldChildCount = 0;

    if (mGroup)
        {
        qDebug() << "Store the group info";

        oldChildCount = mGroup->childCount();

        // remove all connections
        int count = mSignalMapperList.count();
        qDebug() << "mSignalMapperList.count() = " << count;

        for (int i = 0; i < count; ++i)
            {
            QObject::disconnect(mSignalMapperList[i], SIGNAL(signalmapped()),
                    mSignalMapper, SLOT(map()));
            }
        QObject::disconnect(mSignalMapper, SIGNAL(mapped(int)), this,
                SLOT(changeServerUsage(int)));
        
        qDebug() << "oldChildCount = " << oldChildCount;

        //remove the children in the group
        if (oldChildCount > 0)
            {
            qDebug() << "removing the children";
            //mGroup->removeChildren(0, oldChildCount);
            //ToDo: Hack from HbDataForm team        
            for (int i = 0; i < oldChildCount; ++i)
                {
                mGroup->removeChild(0);
                }
            }        
        }
    else
        {
        qDebug() << "Creating new group ";

        mGroup = new HbDataFormModelItem(HbDataFormModelItem::GroupItem,
                hbTrId("txt_loe_subtitle_positioning_servers"), 0);
        mOperation->appendDataFormItem(mGroup);
        }

    qDebug() << "mServerList.count() before clean = " << mServerList.count();

    //clear the list and get the list of server present for positioning
    mServerList.clear();
    if (mSuplSettingsEngine)
        {
        result = mSuplSettingsEngine->suplServerList(mServerList);
        }
    qDebug() << "result = " << result;

    //update the new list to the group
    if (result == 0)
        {
        listServers();
        }
    if (result == -1)
        {
        qDebug() << "Removing the group from form";
        //remove the group
        mOperation->removeItem(mGroup);
        mGroup = 0;
        // delete the menu
        if (mMainMenu)
            {
            delete mMainMenu;
            mMainMenu = 0;
            }
        }
    qDebug() << "- PosSuplSettingsPlugin::updateList()";
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::onDeleteServer
// ---------------------------------------------------------------------------
//
void PosSuplSettingsPlugin::onDeleteServer()
    {
    qDebug() << "+ PosSuplSettingsPlugin::onDeleteServer()";
    //list that will contain the deleteable servers names
    QStringList contentList;

    //get the deleteable server list from the engine
    QList<PosServerParams>::const_iterator iterator;

    qDebug() << "mDeleteableServerList.count = "
            << mDeleteableServerList.count();

    //add the server names to the string list
    for (iterator = mDeleteableServerList.constBegin(); iterator
            != mDeleteableServerList.constEnd(); ++iterator)
        {
        //Copy the address to Qstring object
        qDebug() << "deletable server = " << iterator->serverAdd;
        contentList << iterator->serverAdd;
        }

    // delete the mRadioButtonList before creating it,each time the delete
    // query needs to be popped up
    if (mRadioButtonList != 0)
        {
        delete mRadioButtonList;
        mRadioButtonList = 0;
        }
    //create a radion button list of the deletable servers
    mRadioButtonList = new HbRadioButtonList();
    mRadioButtonList->setItems(contentList);
    QObject::connect(mRadioButtonList, SIGNAL(itemSelected(int)), this,
            SLOT(setPrimaryAction(int)));

    // delete the mDeleteQuery before creating it,each time the delete
    // query needs to be popped up
    
    if (mDeleteQuery != 0)
        {
        delete mDeleteQuery;
        mDeleteQuery = 0;
        }
    //create the query dialog and attach the radio button to the dialog
    mDeleteQuery = new HbDialog();

    mDeleteQuery->setTimeout(HbDialog::NoTimeout);

    mDeleteQuery->setHeadingText(QString(hbTrId(
            "txt_loe_info_select_server_to_delete")));

    // delete the mPrimaryAction before creating it,each time the delete
    // query needs to be popped up
    if (mPrimaryAction != 0)
        {
        delete mPrimaryAction;
        mPrimaryAction = 0;
        }
    //the primary action that will come in the SK1 of the Dialog box
    mPrimaryAction
            = new HbAction(QString(hbTrId("txt_common_button_delete")));

    mDeleteQuery->addAction(mPrimaryAction);
    mDeleteQuery->addAction(new HbAction(QString(hbTrId(
            "txt_common_button_cancel"))));
    QObject::connect(mPrimaryAction, SIGNAL(triggered()), this,
            SLOT(deleteServer()));

    //set the content Widget
    mDeleteQuery->setContentWidget(mRadioButtonList);

    //the Primary action is disabled since no server from the list is selected
    mPrimaryAction->setDisabled(true);
    
    mDeleteQuery->show();

    qDebug() << "- PosSuplSettingsPlugin::onDeleteServer()";
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::createMenu
// ---------------------------------------------------------------------------
//
void PosSuplSettingsPlugin::createMenu()
    {
    qDebug() << "+ PosSuplSettingsPlugin::createMenu()";
    mMainMenu = new HbMenu();
    HbAction* action = mMainMenu->addAction(hbTrId(
            "txt_loe_opt_delete_server"));

    QObject::connect(action, SIGNAL(triggered()), this,
            SLOT(onDeleteServer()));

    mOperation->addAction(action, HbView::OptionsMenu);
    qDebug() << "- PosSuplSettingsPlugin::createMenu()";
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::setPrimaryAction
// ---------------------------------------------------------------------------
//
void PosSuplSettingsPlugin::setPrimaryAction(int /*itemSelected*/)
    {
    //enable the primary Action
    if (mPrimaryAction)
        {
        mPrimaryAction->setEnabled(true);
        }
    }

// ---------------------------------------------------------------------------
// PosSuplSettingsPlugin::deleteServer
// ---------------------------------------------------------------------------
//
void PosSuplSettingsPlugin::deleteServer()
    {
    //get the index of the selected servers
    int serverIndex = mRadioButtonList->selected();

    if( serverIndex != -1 )
    	{
    		if (mSuplSettingsEngine)
        {
        //delete the server
        mSuplSettingsEngine->deleteServer(
                mDeleteableServerList[serverIndex].slpId);
        }
    	}
    }
Q_EXPORT_PLUGIN2(possuplsettings, PosSuplSettingsPlugin)
;

