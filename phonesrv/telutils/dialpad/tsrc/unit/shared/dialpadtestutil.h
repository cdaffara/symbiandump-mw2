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
* Description: Dialpad test utility
*
*/

#ifndef DIALPADTESTUTIL_H
#define DIALPADTESTUTIL_H

#include <QObject>
#include <QMap>
#include <QGraphicsWidget>
#include <QSwipeGesture>

class HbMainWindow;

class DialpadTestUtil : public QObject
{

public:
    enum MouseEventType
    {
        Press,
        Release,
        Click
    };

public:
    explicit DialpadTestUtil(HbMainWindow& mainWindow, QObject* parent=0);
    ~DialpadTestUtil();

    void mouseClickDialpad(int key, MouseEventType type=Click, bool pause=true);

private:
    QGraphicsWidget* getWidgetByName(const QString& name);
    QPointF keyPosition(QGraphicsWidget* widget, int key);

private:
    QMap<int,QString> mKeyNames;
    HbMainWindow& mMainWindow;
};

#endif // DIALPADTESTUTIL_H
