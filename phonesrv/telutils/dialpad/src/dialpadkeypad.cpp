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

#include <QSignalMapper>

#include <hbapplication.h>
#include <hblineedit.h>
#include <hbevent.h>
#include <hbmainwindow.h>

#include "dialpadnumericbutton.h"
#include "dialpadkeypad.h"
#include "dialpadbutton.h"
#include "dialpadinputfield.h"
#include "dialpadnumericbuttongrid.h"

static const int DialpadKeypadRowCount = 5;
static const QLatin1String handsetIcon("qtg_mono_call");

DialpadKeypad::DialpadKeypad(
    const HbMainWindow& mainWindow,
    DialpadInputField& inputField,
    QGraphicsItem* parent) :
    HbWidget(parent),
    mMainWindow(mainWindow),
    mInputField(inputField)
{
    // create clicked signal mapper
    mKeyClickedSignalMapper = new QSignalMapper(this);
    connect(mKeyClickedSignalMapper,SIGNAL(mapped(int)),
            SLOT(handleKeyClicked(int)));

    // connect backspace signals
    connect(&mInputField.backspaceButton(),SIGNAL(clicked()),
            mKeyClickedSignalMapper,SLOT(map()));
    mKeyClickedSignalMapper->setMapping(&mInputField.backspaceButton(),
                                        Qt::Key_Backspace);

    // create call button
    mCallButton = new DialpadButton(this);
    mCallButton->setObjectName("callButton");
    mCallButton->setButtonType(DialpadButton::CallButton);
    mCallButton->setIcon(HbIcon(handsetIcon));
    connect(mCallButton,SIGNAL(clicked()),
            mKeyClickedSignalMapper,SLOT(map()));
    connect(mCallButton,SIGNAL(longPress(QPointF)),
            mKeyClickedSignalMapper,SLOT(map()));
    mKeyClickedSignalMapper->setMapping(mCallButton,
                                        Qt::Key_Yes);

    // create numeric button grid
    mNumericButtonGrid = new DialpadNumericButtonGrid(mMainWindow,this);
    QObject::connect(mNumericButtonGrid,
                     SIGNAL(buttonPressed(const QKeyEvent&)),
                     this, SLOT(sendKeyPressEvent(const QKeyEvent&)));
    QObject::connect(mNumericButtonGrid,
                     SIGNAL(buttonReleased(const QKeyEvent&)),
                     this, SLOT(sendKeyReleaseEvent(const QKeyEvent&)));
    QObject::connect(mNumericButtonGrid,
                     SIGNAL(buttonLongPressed(const QKeyEvent&)),
                     this, SLOT(sendLongPressEvent(const QKeyEvent&)));
    QObject::connect(mNumericButtonGrid,
                     SIGNAL(pressedButtonChanged(const QKeyEvent&,
                                                 const QKeyEvent&)),
                     this, SLOT(handleKeyChangeEvent(const QKeyEvent&,
                                                     const QKeyEvent&)));
}

DialpadKeypad::~DialpadKeypad()
{
}

bool DialpadKeypad::isCallButtonEnabled() const
{
    return mCallButton->isEnabled();
}

void DialpadKeypad::setCallButtonEnabled(bool enabled)
{
    mCallButton->setEnabled(enabled);
}

void DialpadKeypad::resetButtons()
{
    mNumericButtonGrid->resetButtons();
    mCallButton->setDown(false);
}

void DialpadKeypad::handleKeyClicked(int key)
{
    // concerns only yes and backspace keys
    postKeyEvent(QEvent::KeyPress, key);
    postKeyEvent(QEvent::KeyRelease, key);
}

void DialpadKeypad::postKeyEvent(QEvent::Type type, int key)
{
    // send simulated key to application
    QKeyEvent *keyEvent = new QKeyEvent(type, key, Qt::NoModifier);
    HbApplication::postEvent(const_cast<HbMainWindow*>(&mMainWindow),keyEvent);
}

void DialpadKeypad::sendKeyEventToEditor(QEvent::Type type, int key)
{
    // send key event to editor
    QKeyEvent keyEvent(type, key, Qt::NoModifier,
                       mNumericButtonGrid->inputCharacter(key));
    HbApplication::sendEvent(&mInputField.editor(), &keyEvent);
}

void DialpadKeypad::sendKeyPressEvent(const QKeyEvent& event)
{
    mPressedNumericKey = event.key();
    postKeyEvent(QEvent::KeyPress, event.key());
}

void DialpadKeypad::sendKeyReleaseEvent(const QKeyEvent& event)
{
    if (mPressedNumericKey) {
        // short press, update editor here
        sendKeyEventToEditor(QEvent::KeyPress, event.key());
    }

    postKeyEvent(QEvent::KeyRelease, event.key());    
}

void DialpadKeypad::sendLongPressEvent(const QKeyEvent& event)
{
    sendKeyEventToEditor(QEvent::KeyPress, event.key());
    mPressedNumericKey = 0;
}

void DialpadKeypad::handleKeyChangeEvent(
    const QKeyEvent& releaseEvent,
    const QKeyEvent& pressEvent)
{
    Q_UNUSED(pressEvent)

    postKeyEvent(QEvent::KeyRelease, releaseEvent.key());
    mNumericButtonGrid->resetButtons();
}

void DialpadKeypad::setGeometry(const QRectF &rect)
{
    HbWidget::setGeometry(rect);

    // make button heights uniform
    qreal buttonHeight = rect.height()/DialpadKeypadRowCount;

    mNumericButtonGrid->setPos(QPointF(0,0));
    QSizeF gridSize(rect.size());
    gridSize.setHeight(gridSize.height()-buttonHeight);
    mNumericButtonGrid->resize(gridSize);

    mCallButton->setPos(QPointF(0,gridSize.height()));
    mCallButton->resize(rect.width(),buttonHeight);
}
