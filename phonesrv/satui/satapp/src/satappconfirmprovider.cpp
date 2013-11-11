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
*/

// qt
#include <QAction>
// hb
#include <hbmessagebox.h>
#include <hbaction.h>
// satapp
#include "satappconfirmprovider.h"
#include "satappconstant.h"
#include "satappaction.h"

// ----------------------------------------------------------------------------
// SatAppConfirmProvider::SatAppConfirmProvider
// ----------------------------------------------------------------------------
//
SatAppConfirmProvider::SatAppConfirmProvider(QObject *parent)
    : QObject(parent), mConfirmQuery(0), mAction(0)
{
    qDebug("SATAPP: SatAppConfirmProvider::SatAppConfirmProvider");
}

// ----------------------------------------------------------------------------
// SatAppConfirmProvider::~SatAppConfirmProvider
// ----------------------------------------------------------------------------
//
SatAppConfirmProvider::~SatAppConfirmProvider()
{
    qDebug("SATAPP: SatAppConfirmProvider::~SatAppConfirmProvider >");
    delete mConfirmQuery;
    mConfirmQuery = 0;
    qDebug("SATAPP: SatAppConfirmProvider::~SatAppConfirmProvider <");
}

// ----------------------------------------------------------------------------
// SatAppConfirmProvider::showOpenChannelConfirm
// ----------------------------------------------------------------------------
//
void SatAppConfirmProvider::confirmCommand(SatAppAction &action)
{
    qDebug("SATAPP: SatAppConfirmProvider::confirmCommand >");
    mAction = &action;
    delete mConfirmQuery;
    mConfirmQuery = 0;
    TSatSQueryCommand aCommandId = 
        static_cast<TSatSQueryCommand>(action.value(KeyQueryCommandId).toInt());

    qDebug("SATAPP: SatAppConfirmProvider::ConfirmCommand aCommandId: %d",
         aCommandId);
    // Check quering command
    switch (aCommandId)
        {
        case ESatOpenChannelQuery:
            {
            qDebug("SATAPP: SatAppConfirmProvider::ConfirmCommand:\
                Quering OpenChannel");
            showOpenChannelConfirm(action);
            break;
            }
        case ESatSSetUpCallQuery:
            {
            qDebug("SATAPP: SatAppConfirmProvider::ConfirmCommand:\
                Quering SetUpCall");
            showSetUpCallConfirm(action);
            break;
            }
        // TODO: implemente later   
        case ESatSRefreshQuery:
        // TODO: implemente later
        case ESatSLaunchBrowserQuery:
            {
            action.completeWithNoResponse();
            break;
            }
        default:
            {
            qDebug("SATAPP: SatAppConfirmProvider::ConfirmCommand:\
                Unknown quering command");
            action.completeWithNoResponse();
            }
            break;
        }
    qDebug("SATAPP: SatAppConfirmProvider::ConfirmCommand <");
}

// ----------------------------------------------------------------------------
// SatAppConfirmProvider::showOpenChannelConfirm
// ----------------------------------------------------------------------------
//
void SatAppConfirmProvider::showOpenChannelConfirm(SatAppAction &action)
{
    qDebug("SATAPP: SatAppConfirmProvider::showOpenChannelConfirm >");
    QString content = action.value(KeyText).toString();
    if (!content.length()){
        content = hbTrId("txt_simatk_info_open_connection");
    }
    mConfirmQuery = new HbMessageBox(HbMessageBox::MessageTypeQuestion);
    // Sets the "Yes"-action/button
    mConfirmQuery->setText(content);
    mConfirmQuery->setStandardButtons(HbMessageBox::Ok | HbMessageBox::Cancel);
    SAT_ASSERT(connect(mConfirmQuery->actions().at(0), SIGNAL(triggered()),
        &action, SLOT(completeWithSuccess())));
    SAT_ASSERT(connect(mConfirmQuery->actions().at(1), SIGNAL(triggered()),
        &action, SLOT(completeWithBackRequested())));
    SAT_ASSERT(connect(mAction, SIGNAL(actionCompleted(SatAppAction *)),
        this, SLOT(resetState())));
    qDebug("SATAPP: SatAppUiProvider::showOpenChannelConfirm before open");
    mConfirmQuery->open();
    qDebug("SATAPP: SatAppConfirmProvider::showOpenChannelConfirm <");
}

// ----------------------------------------------------------------------------
// SatAppConfirmProvider::showSetUpCallConfirm
// ----------------------------------------------------------------------------
//
void SatAppConfirmProvider::showSetUpCallConfirm(SatAppAction &action)
{
    qDebug("SATAPP: SatAppConfirmProvider::showSetUpCallConfirm >");
    QString text = action.value(KeyText).toString();
    QString additionText = action.value(KeyAdditionalText).toString();
    QString alphaId;
    if (text.length() == 0) {
        qDebug("SATAPP: SatAppConfirmProvider::showSetUpCallConfirm length 0");
        alphaId = hbTrId("txt_simatk_info_1_about_to_call");
        if (additionText.length() > 0) {
            alphaId = alphaId.arg(additionText);
        } else {
            alphaId = alphaId.arg(hbTrId("txt_simatk_title_sim_services"));
        }
    } else {
        qDebug("SATAPP: SatAppConfirmProvider::showSetUpCallConfirm text");
        alphaId.append(text);
    }
    qDebug("SATAPP: SatAppConfirmProvider::showSetUpCallConfirm new");
    mConfirmQuery = new HbMessageBox(HbMessageBox::MessageTypeQuestion);
    mConfirmQuery->setStandardButtons(HbMessageBox::Ok | HbMessageBox::Cancel);
    mConfirmQuery->setText(alphaId);
    SAT_ASSERT(connect(mConfirmQuery->actions().at(0), SIGNAL(triggered()),
        &action, SLOT(completeWithSuccess())));
    SAT_ASSERT(connect(mConfirmQuery->actions().at(1), SIGNAL(triggered()),
        &action, SLOT(completeWithBackRequested())));
    SAT_ASSERT(connect(mAction, SIGNAL(actionCompleted(SatAppAction *)),
        this, SLOT(resetState())));
    qDebug("SATAPP: SatAppConfirmProvider::showSetUpCallConfirm before open");
    mConfirmQuery->open();
    qDebug("SATAPP: SatAppConfirmProvider::showSetUpCallConfirm <");
}

// ----------------------------------------------------------------------------
// SatAppConfirmProvider::clearScreen
// ----------------------------------------------------------------------------
//
void SatAppConfirmProvider::clearScreen()
{
    qDebug("SATAPP: SatAppConfirmProvider::clearScreen >");
    if (mAction) {
        SAT_ASSERT(disconnect(mAction, SIGNAL(actionCompleted(SatAppAction *)),
            this, SLOT(resetState())));
        qDebug("SATAPP: SatAppConfirmProvider::clearScreen quit event loop");
        mAction->completeWithNoResponse();
        mAction = 0;
    }
    delete mConfirmQuery;
    mConfirmQuery = 0;
    qDebug("SATAPP: SatAppConfirmProvider::clearScreen <");
}

// ----------------------------------------------------------------------------
// SatAppConfirmProvider::resetState
// reset
// ----------------------------------------------------------------------------
//
void SatAppConfirmProvider::resetState()
{
    qDebug( "SATAPP: SatAppConfirmProvider::resetState >" );
    mAction = 0;
    qDebug( "SATAPP: SatAppConfirmProvider::resetState <" );
}

//End of file
