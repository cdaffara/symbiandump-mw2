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
* Description: Number grid and dial key.
*
*/

#ifndef DIALPADKEYPAD_H
#define DIALPADKEYPAD_H

#include <hbwidget.h>

class DialpadInputField;
class DialpadButton;
class DialpadNumericButtonGrid;
class QSignalMapper;

class DialpadKeypad : public HbWidget
{
    Q_OBJECT

public:
    explicit DialpadKeypad(
        const HbMainWindow& mainWindow,
        DialpadInputField& inputField,
        QGraphicsItem* parent=0);

    ~DialpadKeypad();

public:
    bool isCallButtonEnabled() const;

    void setCallButtonEnabled(bool enabled);

    void resetButtons();

protected slots:
    void handleKeyClicked(int key);
    void sendKeyPressEvent(const QKeyEvent& event);
    void sendKeyReleaseEvent(const QKeyEvent& event);
    void sendLongPressEvent(const QKeyEvent& event);
    void handleKeyChangeEvent(const QKeyEvent& releaseEvent,
                              const QKeyEvent& pressEvent);

protected:
    void postKeyEvent(QEvent::Type type, int key);
    void sendKeyEventToEditor(QEvent::Type type, int key);

protected:
    void setGeometry(const QRectF &rect);

private:
    const HbMainWindow& mMainWindow;
    DialpadInputField& mInputField;
    QSignalMapper* mKeyClickedSignalMapper;
    int mPressedNumericKey;
    DialpadButton* mCallButton;
    DialpadNumericButtonGrid* mNumericButtonGrid;
};

#endif // DIALPADKEYPAD_H
