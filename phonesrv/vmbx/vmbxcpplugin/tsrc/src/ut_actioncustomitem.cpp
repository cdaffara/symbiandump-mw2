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
* Description:  Implementation of the Ut_vmbxCpPlugin class
*
*/

#include <HbDataFormModelItem>
#include <QFocusEvent>
#include <HbMainWindow>
#include <QEvent>
#include "ut_actioncustomitem.h"
#include "actioncustomitem.h"
#include "customedit.h"

void Ut_ActionCustomItem::testNewAndDelete()
{
    QGraphicsWidget *qGraphicsWidget = new QGraphicsWidget();
    QVERIFY(qGraphicsWidget != 0);

    ActionCustomItem *item = new ActionCustomItem(qGraphicsWidget);

    qDebug("item=0x%x", item);
    QVERIFY(item);    
    delete item;
    item = NULL;   
    qDebug("delete item=0x%x", item);
   
    QVERIFY(!item);
    
    delete qGraphicsWidget;
    qGraphicsWidget = NULL;
}

void Ut_ActionCustomItem::testCreateItem()
{
    QGraphicsWidget *qGraphicsWidget = new QGraphicsWidget();
    QVERIFY(qGraphicsWidget != 0);

    ActionCustomItem *item = new ActionCustomItem(qGraphicsWidget);
    qDebug("item=0x%x", item);
    QVERIFY(item);    
    
    HbAbstractViewItem *viewItem = item->createItem();

    qDebug("viewItem=0x%x", viewItem);
    QVERIFY(viewItem);
    
    delete viewItem;
    viewItem = NULL;

    delete item;
    item = NULL;   
    qDebug("delete item=0x%x", item);
   
    QVERIFY(!item);
    
    delete qGraphicsWidget;
    qGraphicsWidget = NULL;
}

void Ut_ActionCustomItem::testCanSetModelIndex()
{
    QGraphicsWidget *qGraphicsWidget = new QGraphicsWidget();
    QVERIFY(qGraphicsWidget != 0);

    ActionCustomItem *item = new ActionCustomItem(qGraphicsWidget);
    qDebug("item=0x%x", item);
    QVERIFY(item);    
    
    QModelIndex index;
    item->canSetModelIndex(index);
    
    delete item;
    item = NULL;   
    qDebug("delete item=0x%x", item);
   
    QVERIFY(!item);
    
    delete qGraphicsWidget;
    qGraphicsWidget = NULL;
}

