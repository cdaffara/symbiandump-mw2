/*!
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
* Description: Dialpad test utility
*
*/

#include <QGraphicsWidget>
#include <QtTest/QtTest>
#include <hbmainwindow.h>

#include "dialpadtestutil.h"

const int WAIT_TIME = 300;

DialpadTestUtil::DialpadTestUtil(HbMainWindow& mainWindow, QObject* parent) :
    QObject(parent), mMainWindow(mainWindow)
{
    mKeyNames.insert(Qt::Key_1,"numericButtons");
    mKeyNames.insert(Qt::Key_2,"numericButtons");
    mKeyNames.insert(Qt::Key_3,"numericButtons");
    mKeyNames.insert(Qt::Key_4,"numericButtons");
    mKeyNames.insert(Qt::Key_5,"numericButtons");
    mKeyNames.insert(Qt::Key_6,"numericButtons");
    mKeyNames.insert(Qt::Key_7,"numericButtons");
    mKeyNames.insert(Qt::Key_8,"numericButtons");
    mKeyNames.insert(Qt::Key_9,"numericButtons");
    mKeyNames.insert(Qt::Key_Asterisk,"numericButtons");
    mKeyNames.insert(Qt::Key_0,"numericButtons");
    mKeyNames.insert(Qt::Key_NumberSign,"numericButtons");
    mKeyNames.insert(Qt::Key_Backspace,"backspaceButton");
    mKeyNames.insert(Qt::Key_Yes,"callButton");
}

DialpadTestUtil::~DialpadTestUtil()
{      
}

QGraphicsWidget* DialpadTestUtil::getWidgetByName(const QString& name)
{
    QGraphicsWidget* widget = 0;

    QList<QGraphicsItem*> items = mMainWindow.scene()->items();
    foreach (QGraphicsItem* item, items) {
        if (item->isWidget()) {
            QGraphicsWidget *w = static_cast<QGraphicsWidget*>(item);
            if (w->objectName()==name) {
                widget = w;
            }
        }
    }

    return widget;
}

QPointF DialpadTestUtil::keyPosition(QGraphicsWidget* widget, int key)
{
    QPointF keyPos;
    QRectF rect = widget->rect();

    // 3 x 4 grid
    int cellWidth = rect.width() / 3;
    int cellHeight = rect.height() / 4;

    if (key==Qt::Key_Asterisk) {
        keyPos = QPointF( 0.5 * cellWidth, 3.5 * cellHeight );
    } else if (key==Qt::Key_NumberSign) {
        keyPos = QPointF( 2.5 * cellWidth, 3.5 * cellHeight );
    } else if (key==Qt::Key_0) {
        keyPos = QPointF( 1.5 * cellWidth, 3.5 * cellHeight );
    } else {
        int normalized = key - 49;
        // qDebug() << normalized;
        int column = normalized % 3;
        int row = (normalized / 3);
        // qDebug() << column << row;

        keyPos = QPointF((column+0.5)*cellWidth,(row+0.5)*cellHeight);
    }

    return keyPos;
}

void DialpadTestUtil::mouseClickDialpad(int key, MouseEventType type, bool pause)
{
    QString name = mKeyNames.value(key);

    QGraphicsWidget* widget = getWidgetByName(name);

    if ( widget ) {
        QPointF widgetPos;

        if (name=="numericButtons") {
            widgetPos = widget->scenePos() +
                        keyPosition(widget, key);
        } else {
            widgetPos = widget->scenePos() +
                        widget->rect().center();
        }

        QPoint windowPos = mMainWindow.mapFromScene( widgetPos );

        if (type==Press) {
            QTest::mousePress( mMainWindow.viewport(), Qt::LeftButton,
                               0, windowPos );
        } else if (type==Release) {
            QTest::mouseRelease( mMainWindow.viewport(), Qt::LeftButton,
                                 0, windowPos );
        } else {
            QTest::mouseClick( mMainWindow.viewport(), Qt::LeftButton,
                               0, windowPos );
        }

        if (pause) {
            QTest::qWait( WAIT_TIME );
        }
    } else {
        QFAIL( "Button could not be accessed!" );
    }
}


