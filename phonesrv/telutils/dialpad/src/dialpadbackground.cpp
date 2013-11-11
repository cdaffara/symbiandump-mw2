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
* Description: Background item to close dialpad when tapping outside.
*
*/

#include <QGraphicsSceneMouseEvent>
#include "dialpadbackground.h"
#include "dialpad.h"

DialpadBackground::DialpadBackground(Dialpad& dialpad) :
    mDialpad(dialpad), mPressed(false)
{
    setFlag(QGraphicsItem::ItemIsFocusable,true);
}

DialpadBackground::~DialpadBackground()
{

}

void DialpadBackground::setRect(QRectF rect)
{
    mRect = rect;
}

QRectF DialpadBackground::boundingRect() const
{
    return mRect;
}

void DialpadBackground::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget)
{
    Q_UNUSED(painter);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void DialpadBackground::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    mPressed = true;
    event->accept();
}

void DialpadBackground::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ( !boundingRect().contains(event->pos()) && mPressed ) {
        ungrabMouse();
        mPressed = false;
    }
}

void DialpadBackground::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    if (mPressed) {
        mPressed = false;
        event->accept();
        mDialpad.startCloseAnimation();
    }
}



