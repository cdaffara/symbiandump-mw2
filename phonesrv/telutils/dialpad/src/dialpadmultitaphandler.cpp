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
* Description: Dialpad multitap handler
*
*/

#include <QKeyEvent>
#include <hbapplication.h>
#include <hblineedit.h>
#include "dialpadmultitaphandler.h"

static const int DialpadMaxMultitapTime = 800; // ms
static const int DialpadAsteriskMultitapCount = 4;
static const int DialpadAsteriskMultitapChars[DialpadAsteriskMultitapCount] =
    {Qt::Key_Asterisk, Qt::Key_Plus, Qt::Key_P, Qt::Key_W};


DialpadMultitapHandler::DialpadMultitapHandler(
    HbLineEdit& editor,
    QObject* parent) :
    QObject(parent),
    mEditor(editor),
    mAsteriskMultitapIndex(0),
    mGeneratedAsterisk(false)
{
    // extra characters
    mExtraChar.insert(Qt::Key_Asterisk, '*');
    mExtraChar.insert(Qt::Key_Plus, '+');
    mExtraChar.insert(Qt::Key_P, 'p');
    mExtraChar.insert(Qt::Key_W, 'w');
}

DialpadMultitapHandler::~DialpadMultitapHandler()
{
}

bool DialpadMultitapHandler::eventFilter(QObject *watched, QEvent *event)
{
    if (watched!=&mEditor) {
        return false;
    }

    int type = event->type();

    if (type==QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (isNumericKey(keyEvent->key()) &&
            keyEvent->text().isEmpty() ) {
            return true;
        }
    }

    bool keyEventEaten = false;

    if (type==QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);

        if (keyEvent->key()==Qt::Key_Asterisk) {
            if ((mMultitapTimer.elapsed() <= DialpadMaxMultitapTime) &&
                !mGeneratedAsterisk) {
                // remove the last char by generated backspace
                QKeyEvent backspaceEvent(QEvent::KeyPress,
                                         Qt::Key_Backspace,
                                         Qt::NoModifier, "");
                HbApplication::sendEvent(&mEditor,&backspaceEvent);

                // generate +,p,w,* character
                mAsteriskMultitapIndex =
                    ++mAsteriskMultitapIndex % DialpadAsteriskMultitapCount;

                int key = DialpadAsteriskMultitapChars[mAsteriskMultitapIndex];

                // Allow + character only as a first char in editor
                int cursorPosition = mEditor.cursorPosition();
                if(cursorPosition != 0 && key == Qt::Key_Plus) {
                    mAsteriskMultitapIndex = ++mAsteriskMultitapIndex;
                    Q_ASSERT(mAsteriskMultitapIndex < DialpadAsteriskMultitapCount);
                    key = DialpadAsteriskMultitapChars[mAsteriskMultitapIndex];
                }
                
                QKeyEvent generatedEvent(QEvent::KeyPress, key,
                                         Qt::KeypadModifier,
                                         mExtraChar.value(key));
                if (key==Qt::Key_Asterisk) {
                    mGeneratedAsterisk = true;
                }

                HbApplication::sendEvent(&mEditor,&generatedEvent);
                keyEventEaten = true;
                mMultitapTimer.start();
            } else {
                mMultitapTimer.start();
                mAsteriskMultitapIndex = 0;
            }

            mGeneratedAsterisk = false;
        }
    }

    return keyEventEaten;
}

bool DialpadMultitapHandler::isNumericKey(int key)
{
    if (key==Qt::Key_Yes || key==Qt::Key_Backspace) {
        return false;
    } else {
        return true;
    }
}
