/*
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
* Description:
*
*/

#include "ut_psuinotes.h"
#include "qtestmains60ui.h"
#include <hbmainwindow.h>
#include <HbDeviceMessageBox.h>
#include <HbMessageBox.h>
#include <hbnotificationdialog.h>
#define private public
#include "psuinotes.h"

/*!
  UT_PsUiNotes::UT_PsUiNotes
 */
UT_PsUiNotes::UT_PsUiNotes() 
{
    PsUiNotes::instance();
}

/*!
  UT_PsUiNotes::~UT_PsUiNotes
 */
UT_PsUiNotes::~UT_PsUiNotes()
{
    //delete m_psui;
}

/*!
  UT_PsUiNotes::timerEvent
 */
void UT_PsUiNotes::timerEvent( QTimerEvent* event )
{
    QString currentTest(QTest::currentTestFunction());
    qDebug() << "timerEvent:" << currentTest;
    
    if (currentTest == "t_showGlobalErrorNote") {
        verifyGlobalNote(wantedNoteText);
    }
    
    if (m_noteid1) {
        PsUiNotes::instance()->cancelNote(m_noteid1);
        m_noteid1 = 0;
    }
    if (m_noteid2) {
        PsUiNotes::instance()->cancelNote(m_noteid2);
        m_noteid2 = 0;
    }
    if (m_noteid3) {
        PsUiNotes::instance()->cancelNote(m_noteid3);
        m_noteid3 = 0;
    }
    
    if (visibleDialog()) {
        visibleDialog()->close();
        QTest::qWait(1);
    }
	
}

/*!
  UT_PsUiNotes::visibleDialog
 */
HbDialog *UT_PsUiNotes::visibleDialog()
{
    QList<QGraphicsItem*> items = mainWindow->scene()->items();
    foreach (QGraphicsItem* item, items) {
        HbDialog *w = qobject_cast<HbDialog*>(item->parentWidget());
        if (w && w->isVisible()) {
            qDebug() << "visibleDialog: " << w->getStaticMetaObject().className() <<
                    "contentWidget: " << w->contentWidget()->getStaticMetaObject().className();
            return w;
        }
    }
    
    return 0;
}

/*!
  UT_PsUiNotes::verifyGlobalNote
 */
void UT_PsUiNotes::verifyGlobalNote( const QString& noteText )
{
    qDebug() << "verifyGlobalNote:" << noteText;
    HbDeviceMessageBox *note=0;
    QObject* o=0;
    QVERIFY(PsUiNotes::instance()->m_notesQueue->count());
    QVERIFY(o = PsUiNotes::instance()->m_notesQueue->at(0));
    QVERIFY(note = qobject_cast<HbDeviceMessageBox *>(o));
    QCOMPARE(note->text(), noteText);
    note->close();
    QTest::qWait(1);
}

/*!
  UT_PsUiNotes::init
 */
void UT_PsUiNotes::init()
{
    initialize();
    timerid = startTimer(3000);
    m_noteid1 = 0;
    m_noteid2 = 0;
    m_noteid3 = 0;
}

/*!
  UT_PsUiNotes::cleanup
 */
void UT_PsUiNotes::cleanup()
{
    reset();
    if (timerid) {
        killTimer(timerid);
        timerid = 0;
    }
}

/*!
  UT_PsUiNotes::t_memleak
 */
void UT_PsUiNotes::t_memleak()
{
    
}

/*!
  UT_PsUiNotes::t_showNotificationDialog
 */
void UT_PsUiNotes::t_showNotificationDialog()
{
    PsUiNotes::instance()->showNotificationDialog("NotificationDialog");
}

/*!
  UT_PsUiNotes::t_showProgressNote
 */
void UT_PsUiNotes::t_showProgressNote()
{
    PsUiNotes::instance()->showGlobalProgressNote(m_noteid1, "testshowProgressNote1");
    PsUiNotes::instance()->showGlobalProgressNote(m_noteid2, "testshowProgressNote2");
    QTest::qWait(5000);
}

/*!
  UT_PsUiNotes::t_showGlobalProgressNote
 */
void UT_PsUiNotes::t_showGlobalProgressNote()
{
    PsUiNotes::instance()->showGlobalProgressNote(m_noteid1, "testshowGPN");
    QTest::qWait(5000);
    PsUiNotes::instance()->showGlobalProgressNote(m_noteid1,
            hbTrId("txt_common_info_requesting"));
    QTest::qWait(5000);
}

/*!
  UT_PsUiNotes::t_showGlobalNote
 */
void UT_PsUiNotes::t_showGlobalNote()
{
    PsUiNotes::instance()->showGlobalNote(m_noteid1, "MessageTypeQuestion",HbMessageBox::MessageTypeQuestion );
    PsUiNotes::instance()->showGlobalNote(m_noteid2, "MessageTypeInformation",HbMessageBox::MessageTypeInformation);
    PsUiNotes::instance()->showGlobalNote(m_noteid3, "MessageTypeWarning",HbMessageBox::MessageTypeWarning );
    
}

/*!
  UT_PsUiNotes::t_showGlobalErrorNote
 */
void UT_PsUiNotes::t_showGlobalErrorNote()
{
    int errorcode = -1;
    wantedNoteText = "";
    expect("PsUiUtils::errorCodeTextMapping").with(errorcode,wantedNoteText).returns(true);
    PsUiNotes::instance()->showGlobalErrorNote(m_noteid1, errorcode );
    QVERIFY( verify() );

}


/*!
  UT_PsUiNotes::t_showCallDivertDetails
 */
void UT_PsUiNotes::t_showCallDivertDetails()
{
    expect("PsUiSettingsWrapper::numberGroupingSupported").returns(QVariant(true));
    PSCallDivertingStatus status;
    status.iStatus = DivertingStatusActive;
    status.iServiceGroup = ServiceGroupVoice;
    status.iTimeout = 30;
    QList<PSCallDivertingStatus*> statusList;
    statusList.append(&status);
    PsUiNotes::instance()->showCallDivertDetails(statusList);
    QVERIFY( verify() );

    expect("PsUiSettingsWrapper::numberGroupingSupported").returns(QVariant(false));
    PsUiNotes::instance()->showCallDivertDetails(statusList);
    QVERIFY( verify() );
}

/*!
  UT_PsUiNotes::t_showPasswordQueryDialog
 */
void UT_PsUiNotes::t_showPasswordQueryDialog()
{
    QString queryTitle("Password query title");
    QString password = "";
    bool okPressed = false;
    QRegExp regExpression("\\d{4}");
    QRegExpValidator validator(regExpression, this);
    PsUiNotes::instance()->showPasswordQueryDialog(
        queryTitle, validator, 5);

}

/*!
  UT_PsUiNotes::t_cancelNote
 */
void UT_PsUiNotes::t_cancelNote()
{
    PsUiNotes::instance()->cancelNote(m_noteid1);
    PsUiNotes::instance()->showGlobalProgressNote(m_noteid1, "testcancelNote");
    PsUiNotes::instance()->noteShowing();
    PsUiNotes::instance()->cancelNote(m_noteid1);
        
    QTest::qWait(5000);
}

QTEST_MAIN_S60UI(UT_PsUiNotes)
