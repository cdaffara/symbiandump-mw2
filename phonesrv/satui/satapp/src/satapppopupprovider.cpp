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
#include <QTimer>
#include <QtDebug>
// hb
#include <hbmessagebox.h>
#include <hbprogressdialog.h>
#include <hbdevicemessagebox.h>
#include <hblabel.h>
#include <hbaction.h>
// satapp
#include "satapppopupprovider.h"
#include "satappconstant.h"
#include "satappaction.h"


// ----------------------------------------------------------------------------
// SatAppPopupProvider::SatAppPopupProvider
// ----------------------------------------------------------------------------
//
SatAppPopupProvider::SatAppPopupProvider( QObject *parent)
    : QObject(parent), mDisplayText(0), mWaitDialog(0),
      mWaitDeviceDialog(0), mAction(0)
{
    qDebug("SATAPP: SatAppPopupProvider::SatAppPopupProvider");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::~SatAppPopupProvider
// ----------------------------------------------------------------------------
//
SatAppPopupProvider::~SatAppPopupProvider()
{
    qDebug("SATAPP: SatAppPopupProvider::~SatAppPopupProvider >");
    
    if (mWaitDialog) {
        qDebug("SATAPP: SatAppPopupProvider::~SatAppPopupProvider wait note");
        mWaitDialog->close();
        delete mWaitDialog;
        mWaitDialog = NULL;
    }
    
    if (mWaitDeviceDialog) {
        qDebug("SATAPP: SatAppPopupProvider::~SatAppPopupProvider device note");
        mWaitDeviceDialog->close();
        delete mWaitDeviceDialog;
        mWaitDeviceDialog = NULL;            
    }
    
    if (mDisplayText){
        qDebug( "SatAppPopupProvider::~SatAppPopupProvider DisplayText" );
        delete mDisplayText;
        mDisplayText = 0;
    }
    qDebug("SATAPP: SatAppPopupProvider::~SatAppPopupProvider <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::displayText
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::displayText(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::displayText >");
    mAction = &action;
    delete mDisplayText;
    mDisplayText = 0;
    QString heading = action.value(KeyApplicationName).toString();
    if (heading.isEmpty()) {
        heading = hbTrId("txt_simatk_dialog_sim_services");
    }
    qDebug() << "SATAPP: displayText: heading=" << heading;

    // text
    QString text = action.value(KeyText).toString();

    // some flags
    bool sustainedText = action.value(KeySustainedText).toBool();
    bool waitUserToClear = action.value(KeyWaitUserToClear).toBool();

    // duration
    int duration = action.value(KeyDuration).toInt();
    if (duration==0) {
        duration = KDisplayTxtDefaultduration;
        if (waitUserToClear) {
            duration = KDisplayTxtUserClearTimeout;
        }
    }
    if ( !(!sustainedText || 
           action.value(KeyDuration).toInt() || 
           !waitUserToClear) ) {
        duration = 0;
    }
    qDebug("SATAPP: displayText: duration=%d",duration);

    // Show DisplayText dialog
    mDisplayText = new HbMessageBox(HbMessageBox::MessageTypeInformation);

    // Set the label as heading widget
    HbLabel *label = new HbLabel(heading, mDisplayText);
    mDisplayText->setHeadingWidget(label);
    mDisplayText->setText(text);
    mDisplayText->setIconVisible(false);
    mDisplayText->setStandardButtons(HbMessageBox::Ok | HbMessageBox::Cancel);
    // ok pressed -> complete action with KSatSuccess
    SAT_ASSERT(connect(mDisplayText->actions().at(0), SIGNAL(triggered()),
        &action, SLOT(completeWithSuccess())));
    // cancel pressed -> complete action with ESatBackwardModeRequestedByUser
    SAT_ASSERT(connect(mDisplayText->actions().at(1), SIGNAL(triggered()),
        &action, SLOT(completeWithBackRequested())));
    SAT_ASSERT(connect(mAction, SIGNAL(actionCompleted(SatAppAction *)),
        this, SLOT(resetState())));
    if (duration) {
        if (waitUserToClear && !sustainedText) {
           // in timeout, complete action with ESatNoResponseFromUser
            QTimer::singleShot(duration,
                &action, SLOT(completeWithNoResponse()));
        } else {
            QTimer::singleShot(duration,
                &action, SLOT(completeWithSuccess()));
        }
        mDisplayText->setTimeout(duration); 
    }

    // open the popup.
    mDisplayText->open();
    
    qDebug("SATAPP: SatAppPopupProvider::displayText <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::notification
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::notification(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::notification >");
    // some flags
    int commandId = action.value(KeyCommandId).toInt();
    int alphaIdStatus = action.value(KeyAlphaIdStatus).toInt();
    // reset mAction
    mAction = 0;
    qDebug("SATAPP: notification id=%d alphaSt=%d",
        commandId, alphaIdStatus);

    switch (commandId)
    {
        case ESatSSendDataNotify:
        case ESatSReceiveDataNotify:
        {
            qDebug("SATAPP: Notifying BIP Send/Receive");
            // need to complete action with success when use don't prees cancel
            mAction = &action;
            showBIPWaitNote(action);
            break;
        }
        case ESatSCloseChannelNotify:
        {
            qDebug("SATAPP: Notifying BIP CloseChannel");
            showCloseChannelWaitNote(action);
            break;
        }
        case ESatSMoSmControlNotify:
        {
            qDebug("SATAPP: Notifying MoSmControl");
            if (ESatAlphaIdNotNull == alphaIdStatus) {
                showMoSmControlNote(action);
            } else {
                showSatInfoNote(action);
            }
            break;
        }
        case ESatSCallControlNotify:
        {
            qDebug("SATAPP: Notifying CallControl");
            showCallControlNote(action);
            break;
        }
        case ESatSSendUssdNotify:   // fall through
        case ESatSSendSsNotify:
        {
            qDebug("SATAPP: Notifying SendSs / SendUssd");
            showSsWaitNote(action);
            break;
        }
        case ESatSSendDtmfNotify:
        {
            qDebug("SATAPP: Notifying SendDtmf");
            // need to complete action with success when use don't prees cancel
            mAction = &action;
            showDtmfWaitNote(action);
            break;
        }
        case ESatSSendSmsNotify:
        {
            qDebug("SATAPP: Notifying SendSms");
            showSmsWaitNote(action);
            break;
        }
        default:
        {
            qDebug("SATAPP: Unkown notification");
            action.completeWithFailure();
            break;
        }
    }
    qDebug("SATAPP: SatAppPopupProvider::notification <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::stopShowWaitNote
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::stopShowWaitNote()
{
    qDebug("SATAPP:SatAppPopupProvider::stopShowWaitNote: >mWaitDialog = %x", 
        mWaitDialog );
    if (mWaitDialog) {
        mWaitDialog->close();
        delete mWaitDialog;
        mWaitDialog = NULL;
        if (mAction) {
            qDebug("SATAPP: stopShowWaitNote: mAction");
            int commandId = mAction->value(KeyCommandId).toInt();
            if (ESatSSendDataNotify == commandId 
                || ESatSReceiveDataNotify == commandId 
                || ESatSSendDtmfNotify == commandId) {
                 mAction->completeWithSuccess();
            }
        }
    }

    qDebug("SATAPP: stopShowWaitNote: mWaitDeviceDialog=%x",
        mWaitDeviceDialog);
    if (mWaitDeviceDialog) {
        mWaitDeviceDialog->close();
        delete mWaitDeviceDialog;
        mWaitDeviceDialog = NULL;            
    }

    qDebug("SATAPP:SatAppPopupProvider::stopShowWaitNote: <" );
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::defaultAlphaId
// provides a default text in case it is empty
// ----------------------------------------------------------------------------
//
QString SatAppPopupProvider::alphaId(SatAppAction& action)
{
    QString alpha = action.value(KeyText).toString();
    qDebug() << "SATAPP: SatAppPopupProvider::alphaId" << alpha;

    int commandId = action.value(KeyCommandId).toInt();
    int controlResult = action.value(KeyControlResult).toInt();

    if (!alpha.isEmpty()) {
        qDebug("SATAPP:SatAppPopupProvider::alphaId not empty<");
        return alpha;
    }
    switch (commandId)
    {
    case ESatSSendDataNotify: // SendData
        {
        alpha = hbTrId("txt_simatk_dialog_sendingdata");
        break;
        }
    case ESatSReceiveDataNotify: // ReceiveData
        {
        alpha = hbTrId("txt_simatk_dialog_receivingdata");
        break;
        }
    case ESatSCloseChannelNotify: // CloseChannel
        {
        alpha = hbTrId("txt_simatk_dialog_connectionclosed");
        break;
        }
    case ESatSMoSmControlNotify: // MoSmControl
        {
        if (ESatNotAllowed == controlResult) {
        alpha = hbTrId("txt_simatk_dpopinfo_request_not_allowed");
        } else if (ESatAllowedWithModifications == controlResult) {
            alpha = hbTrId("txt_simatk_dpopinfo_request_modified");
        } else {
            alpha =  hbTrId(""); // Allowed, default alpha -> no info
        }
        break;
        }
    case ESatSCallControlNotify: // CallControl
       {
        if (ESatNotAllowed == controlResult) {
            alpha= hbTrId("txt_simatk_dpopinfo_request_not_allowed");
        } else if (ESatAllowedWithModifications == controlResult) {
            alpha = hbTrId("txt_simatk_dpopinfo_request_modified");
        } else {
            alpha = hbTrId(""); // Allowed, default alpha -> no info
        }
        break;
        }
    case ESatSSendSmsNotify: // fall through
    case ESatSSendDtmfNotify: // fall through
    case ESatSSendUssdNotify: // fall through
    case ESatSSendSsNotify: // fall through
        {
        // alpha id is empty, set a default string
        alpha = hbTrId("txt_common_info_processing");
        break;
        }
    default:
        break;
    }
    qDebug("SATAPP:SatAppPopupProvider::alphaId <");
    return alpha;
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::showBIPWaitNote
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showBIPWaitNote(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::showBIPWaitNote >");

    QString text = alphaId(action);
    // this is a new SEND DATA action
    delete mWaitDialog;
    mWaitDialog = 0;
    HbProgressDialog *pd = new HbProgressDialog(HbProgressDialog::WaitDialog);
    pd->setText(text);
    // cancel -> complete with ESatBackwardModeRequestedByUser
    SAT_ASSERT(connect(pd->actions().at(0),SIGNAL(triggered()),
        &action,SLOT(completeWithBackRequested())));

    // open dialog
    pd->open();
    mWaitDialog = pd;

    qDebug("SATAPP: SatAppPopupProvider::showBIPWaitNote <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::showCloseChannelWaitNote
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showCloseChannelWaitNote(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::showCloseChannelWaitNote >");
    HbProgressDialog *pd = new HbProgressDialog(HbProgressDialog::WaitDialog);
    pd->clearActions();
    pd->setText(alphaId(action));
    pd->open();
    mWaitDialog = pd;
    qDebug("SATAPP: SatAppPopupProvider::showCloseChannelWaitNote <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::showMoSmControlNote
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showMoSmControlNote(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::showMoSmControlNote >");
    HbMessageBox *mb = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    mb->clearActions();
    mb->setText(alphaId(action));
    mb->open();
    mWaitDialog = mb;
    qDebug("SATAPP: SatAppPopupProvider::showMoSmControlNote <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::showSatInfoNote
//
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showSatInfoNote(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::showSatInfoNote >");
    HbMessageBox *mb = new HbMessageBox(HbMessageBox::MessageTypeInformation);
    mb->setText(alphaId(action));
    mb->open();
    mWaitDialog = mb;
    qDebug("SATAPP: SatAppPopupProvider::showSatInfoNote <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::showCallControlNote
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showCallControlNote(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::showCallControlNote >");
    HbDeviceMessageBox *dmb = new HbDeviceMessageBox(HbMessageBox::MessageTypeInformation);
    // No cancel key
    dmb->setText(alphaId(action));
    dmb->show();
    mWaitDeviceDialog = dmb;

    qDebug("SATAPP: SatAppPopupProvider::showCallControlNote <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::showSsWaitNote
// Displays a wait note to indicate SS sending.
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showSsWaitNote(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::showSsWaitNote >");
    HbProgressDialog *pd = new HbProgressDialog(HbProgressDialog::WaitDialog);
    pd->clearActions();
    pd->setText(alphaId(action));
    pd->show();
    mWaitDialog = pd;
    qDebug("SATAPP: SatAppPopupProvider::showSsWaitNote <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::showDtmfWaitNote
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showDtmfWaitNote(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::showDtmfWaitNote >");
    HbProgressDialog *pd = new HbProgressDialog(HbProgressDialog::WaitDialog);
    // cancel -> complete action with ESatBackwardModeRequestedByUser
    SAT_ASSERT(connect(pd->actions().at(0), SIGNAL(triggered()),
        &action, SLOT(completeWithBackRequested())));
    pd->setText(alphaId(action));
    pd->open();
    mWaitDialog = pd;
    qDebug("SATAPP: SatAppPopupProvider::showDtmfWaitNote <");
 }


// ----------------------------------------------------------------------------
// SatAppPopupProvider::showDtmfWaitNote
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showSmsWaitNote(SatAppAction& action)
{
    qDebug("SATAPP: SatAppPopupProvider::showSmsWaitNote >");
    HbProgressDialog *pd = new HbProgressDialog(HbProgressDialog::WaitDialog);
    //remove the default cancel softkey
    pd->clearActions();
    pd->setText(alphaId(action));
    pd->open();
    mWaitDialog = pd;
    qDebug("SATAPP: SatAppPopupProvider::showSmsWaitNote <");
 }

// ----------------------------------------------------------------------------
// SatAppPopupProvider::stopShowWaitNote
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::showSsErrorNote()
{
    qDebug("SATAPP: SatAppPopupProvider::showSsErrorNote >");
    HbMessageBox *mb = new HbMessageBox(HbMessageBox::MessageTypeWarning);
    mb->setText(hbTrId("txt_sat_sendss_error_note"));
    mb->open();
    mWaitDialog = mb;
    qDebug("SATAPP: SatAppPopupProvider::showSsErrorNote <");
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::clearScreen
// terminates all ongoing UI actions
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::clearScreen()
{
    qDebug( "SATAPP: SatAppPopupProvider::clearScreen >" );
    stopShowWaitNote();
    if (mDisplayText){
        qDebug( "SatAppPopupProvider::clearScreen DisplayText" );
        delete mDisplayText;
        mDisplayText = 0;
        if (mAction) {
            qDebug( "SatAppPopupProvider::clearScreen mAction" );
            SAT_ASSERT(disconnect(mAction, SIGNAL(actionCompleted(SatAppAction *)),
                this, SLOT(resetState())));
            mAction->completeWithNoResponse();
            mAction = 0;
        }
        qDebug( "SatAppPopupProvider::clearScreen DisplayText <" );
    }
    qDebug( "SATAPP: SatAppToneProvider::clearScreen <" );
}

// ----------------------------------------------------------------------------
// SatAppPopupProvider::resetState
// reset
// ----------------------------------------------------------------------------
//
void SatAppPopupProvider::resetState()
{
    qDebug( "SATAPP: SatAppPopupProvider::resetState >" );
    mAction = 0;
    qDebug( "SATAPP: SatAppPopupProvider::resetState <" );
}

//End of file
