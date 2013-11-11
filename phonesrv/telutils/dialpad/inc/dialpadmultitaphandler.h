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
* Description: Multitap handler
*
*/

#ifndef DIALPADMULTITAPHANDLER_H_
#define DIALPADMULTITAPHANDLER_H_

#include <qobject.h>
#include <QTime>
#include <QMap>

class HbLineEdit;

class DialpadMultitapHandler : public QObject
    {
public:
    explicit DialpadMultitapHandler(HbLineEdit& editor, QObject* parent=0);
    virtual ~DialpadMultitapHandler();

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    inline bool isNumericKey(int key);

private:
    HbLineEdit& mEditor;
    QTime mMultitapTimer;
    int mAsteriskMultitapIndex;
    bool mGeneratedAsterisk;
    QMap<int,QChar> mExtraChar;
    };

#endif /* DIALPADMULTITAPHANDLER_H_ */
