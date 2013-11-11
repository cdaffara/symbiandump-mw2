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

#ifndef DIALPADBACKGROUND_H
#define DIALPADBACKGROUND_H

#include "qgraphicsitem.h"

class Dialpad;

class DialpadBackground : public QGraphicsItem
{
public:
    DialpadBackground(Dialpad& dialpad);
    virtual ~DialpadBackground();
    
    void setRect(QRectF rect);
    QRectF boundingRect() const;    

protected:
    void paint(
        QPainter *painter,
        const QStyleOptionGraphicsItem *option,
        QWidget *widget = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);

private:
    Dialpad& mDialpad;
    QRectF mRect;
    bool mPressed;
};

#endif // DIALPADBACKGROUND_H
