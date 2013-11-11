/*!
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Dialpad keypad
*
*/
#include "dialpadinternaleventfilter.h"
#include "qtphonesrvlog.h"

#include <hbdevicenotificationdialog.h>
#include <QDebug>
#include <QKeyEvent>

#include <e32base.h>
#include <APGTASK.H>
#include <coemain.h>
#include <apgcli.h>
#include <apacmdln.h>

const int DialpadLongKeyPressTimeOut(1000);

DialpadInternalEventFilter::DialpadInternalEventFilter() :
    QObject(),
    mKey(0)
{
    PHONE_TRACE;
    mLongPressTimer = new QTimer(this);
    mLongPressTimer->setSingleShot(true);
    connect(mLongPressTimer,SIGNAL(timeout()), this, SLOT(togglePhoneUi()));
}

DialpadInternalEventFilter::~DialpadInternalEventFilter()
{
}

bool DialpadInternalEventFilter::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(watched)
            
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_8 ||  keyEvent->key() == Qt::Key_9) {
            mKey = keyEvent->key();
            mLongPressTimer->stop();
            mLongPressTimer->start(DialpadLongKeyPressTimeOut);
        }
    } else if (event->type() == QEvent::KeyRelease) {
            mLongPressTimer->stop();
    }

    // Don't consume the key
    return false;
}

void DialpadInternalEventFilter::togglePhoneUi()
{
    if(mKey == Qt::Key_8) {
        start();
    }
    if(mKey == Qt::Key_9) {
        shutdown();
    } 
}

void DialpadInternalEventFilter::shutdown()
{
    HbDeviceNotificationDialog notificationDialog;
    QString text = "Shutting down phoneui.exe";
    notificationDialog.setText(text);
    notificationDialog.show();

    
    qDebug() << "Shutting down phoneui.exe";
    
    const TUid KPhoneAppUid = {0x100058B3};
    TApaTaskList taskList(CCoeEnv::Static()->WsSession());
    TApaTask task = taskList.FindApp(KPhoneAppUid);
    
    if (task.Exists()){
        task.EndTask(); // Ends the task
    }    
}

void DialpadInternalEventFilter::start()
{

    HbDeviceNotificationDialog notificationDialog;
    QString text = "Starting phoneui.exe";
    notificationDialog.setText(text);
    notificationDialog.show();
    
    TThreadId app_threadid;
    CApaCommandLine* cmdLine; 
    cmdLine=CApaCommandLine::NewLC();
    cmdLine->SetExecutableNameL(_L("phoneui.exe"));
    cmdLine->SetCommandL( EApaCommandRun );
    RApaLsSession ls;
    User::LeaveIfError(ls.Connect());
    TInt err=ls.StartApp(*cmdLine,app_threadid);
    ls.Close();
    CleanupStack::PopAndDestroy(); // cmdLine
}
