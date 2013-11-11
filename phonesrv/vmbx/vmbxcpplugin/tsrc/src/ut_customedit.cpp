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

#include <QFocusEvent>
#include <HbMainWindow>
#include <QEvent>
#include <QEventLoop>
#include "ut_customedit.h"
#include "customedit.h"



void Ut_CustomEdit::testFocusInEvent()
{
    HbMainWindow *window = new HbMainWindow;
    QVERIFY(window);
    
    QGraphicsWidget *qGraphicsWidget = new QGraphicsWidget();
    QVERIFY(qGraphicsWidget != 0);
    CustomEdit *edit = new CustomEdit(qGraphicsWidget);
    QVERIFY(edit);
    
    QFocusEvent *event = new QFocusEvent(QEvent::FocusIn, 
        Qt::MouseFocusReason);  
    QVERIFY(event);
    edit->focusInEvent(event);
    delete event;
    event  = NULL;
    
    event = new QFocusEvent(QEvent::FocusOut, 
        Qt::MouseFocusReason);
    QVERIFY(event);
    edit->focusInEvent(event);
    // test for emitEditItemClicked    
    QEventLoop loop;
    QTimer timer;
    timer.start(150);
    connect( &timer,SIGNAL(timeout()),
        &loop,SLOT(quit()));
    loop.exec();
    delete event;
    event = NULL;
    delete edit;
    edit = NULL;
    QVERIFY(!edit);
    
    delete qGraphicsWidget;
    qGraphicsWidget = NULL;
}
