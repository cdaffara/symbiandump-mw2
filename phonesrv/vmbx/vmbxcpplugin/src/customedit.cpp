/*
* Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
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

#include "customedit.h"
#include "loggerutil.h"

/*!
    Constructor
*/
CustomEdit::CustomEdit(QGraphicsItem *parent)
    : HbLineEdit(parent)
{
    _DBGLOG("CustomEdit::CustomEdit() >")
    const bool ret = connect(&mItemClickedTimer, SIGNAL(timeout()),
        this, SLOT(emitEditItemClicked()));
    _DBGLOG2("CustomEdit::CustomEdit() <", ret)
}

/*!
    Distructor
*/
CustomEdit::~CustomEdit()
{
    _DBGLOG("CustomEdit::CustomEdit()")
    disconnect(&mItemClickedTimer, SIGNAL(timeout()),
        this, SLOT(emitEditItemClicked()));
}

/*!
    focusInEvent
*/
void CustomEdit::focusInEvent(QFocusEvent *event)
{
    _DBGLOG("CustomEdit::focusInEvent() >")
    if (!event->gotFocus()) {
        _DBGLOG("CustomEdit::focusInEvent() ignore")
        event->ignore();
    } else {
        _DBGLOG("CustomEdit::focusInEvent() emit clicked")
        // When focus comes to the controller, a 100 ms timer is started.
        // a signal is emitted after this 100 ms, and focus transfer to other controller,
        // after the other controller finished,to avoid focus gained again.
        mItemClickedTimer.start(100);
        event->accept();
    }
    _DBGLOG("CustomEdit::focusInEvent() <")
}

/*!
    emitEditItemClicked
*/
void CustomEdit::emitEditItemClicked()
{
    _DBGLOG("CustomEdit::emitEditItemClicked() >")
    emit editItemClicked();
    mItemClickedTimer.stop();
    _DBGLOG("CustomEdit::emitEditItemClicked() <")
}

// End of file
