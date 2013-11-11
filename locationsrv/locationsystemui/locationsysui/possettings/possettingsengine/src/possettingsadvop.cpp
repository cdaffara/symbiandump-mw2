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
 * Description: Implementation of Positioning settings advanced 
 *              operation class.
 *
 */

#include "possettingsadvop.h"
#include "possettingsadvview.h"
#include <QDebug>
#include <hbdataformviewitem.h>

//---------------------------------------------------------------------
// PosSettingsAdvOp::PosSettingsAdvOp()
// Constructor
//---------------------------------------------------------------------
PosSettingsAdvOp::PosSettingsAdvOp(HbView* view) : mView(view)
    {
    qDebug() << "+ PosSettingsAdvOp::PosSettingsAdvOp()";
    mDataForm = qobject_cast<HbDataForm*>(mView->widget());
    mDataFormModel = static_cast<HbDataFormModel*>(mDataForm->model());
    qDebug() << "- PosSettingsAdvOp::PosSettingsAdvOp()";
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::~PosSettingsAdvOp()
// Destructor
//---------------------------------------------------------------------
PosSettingsAdvOp::~PosSettingsAdvOp()
    {
    qDebug() << "+ PosSettingsAdvOp::~PosSettingsAdvOp()";
    qDebug() << "- PosSettingsAdvOp::~PosSettingsAdvOp()";
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::appendDataFormItem
// 
//---------------------------------------------------------------------
void PosSettingsAdvOp::appendDataFormItem(HbDataFormModelItem *data,
        HbDataFormModelItem *parent)
    {
    qDebug() << "+ PosSettingsAdvOp::appendDataFormItem()";
    mDataFormModel->appendDataFormItem(data, parent);
    qDebug() << "- PosSettingsAdvOp::appendDataFormItem()";
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::addConnection
// 
//---------------------------------------------------------------------
void PosSettingsAdvOp::addConnection(HbDataFormModelItem *item,
        const char* signal, QObject *receiver, const char* slot)
    {
    qDebug() << "+ PosSettingsAdvOp::addConnection()";
    mDataForm->addConnection(item, signal, receiver, slot);
    qDebug() << "- PosSettingsAdvOp::addConnection()";
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::addAction
// 
//---------------------------------------------------------------------
void PosSettingsAdvOp::addAction(HbAction *action,
        HbView::ActionContainer preferedActionContainer)
    {
    qDebug() << "+ PosSettingsAdvOp::addAction()";
    mView->addAction(action, preferedActionContainer);
    qDebug() << "- PosSettingsAdvOp::addAction()";
    }


//---------------------------------------------------------------------
// PosSettingsAdvOp::indexFromItem
// 
//---------------------------------------------------------------------
QModelIndex PosSettingsAdvOp::indexFromItem(const HbDataFormModelItem * item)
    {
    qDebug() << "+ PosSettingsAdvOp::indexFromItem()";
    qDebug() << "- PosSettingsAdvOp::indexFromItem()";
    return mDataFormModel->indexFromItem(item);
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::removeConnection
// 
//---------------------------------------------------------------------
void PosSettingsAdvOp::removeConnection(HbDataFormModelItem *item,
        const char* signal, QObject *receiver, const char* slot)
    {
    qDebug() << "+ PosSettingsAdvOp::removeConnection()";
    mDataForm->removeConnection(item, signal, receiver, slot);
    qDebug() << "- PosSettingsAdvOp::removeConnection()";    
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::setExpanded
// 
//---------------------------------------------------------------------
void PosSettingsAdvOp::setExpanded(const QModelIndex &index, bool expanded)
    {
    qDebug() << "+ PosSettingsAdvOp::setExpanded()";
    mDataForm->setExpanded( index,expanded);
    qDebug() << "- PosSettingsAdvOp::setExpanded()";
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::isExpanded
// 
//---------------------------------------------------------------------
bool PosSettingsAdvOp::isExpanded(const QModelIndex &index)const
    {
    qDebug() << "+ PosSettingsAdvOp::isExpanded()";
    qDebug() << "- PosSettingsAdvOp::isExpanded()";
    return mDataForm->isExpanded(index);
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::removeItem
// 
//---------------------------------------------------------------------
bool PosSettingsAdvOp::removeItem(HbDataFormModelItem* item)
    {
    qDebug() << "+ PosSettingsAdvOp::removeItem()";
    qDebug() << "- PosSettingsAdvOp::removeItem()";
    return mDataFormModel->removeItem(item);
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::removeItem
// 
//---------------------------------------------------------------------
void PosSettingsAdvOp::setItemPrototypes(
        QList<HbAbstractViewItem *> &protoTypeList)
    {
    qDebug() << "+ PosSettingsAdvOp::setItemPrototypes()";
    QList<HbAbstractViewItem *> oldProtoTypeList = mDataForm->itemPrototypes();
    oldProtoTypeList.append(protoTypeList);
    mDataForm->setItemPrototypes(oldProtoTypeList);
    qDebug() << "- PosSettingsAdvOp::setItemPrototypes()";
    }

//---------------------------------------------------------------------
// PosSettingsAdvOp::itemPrototypes
// 
//---------------------------------------------------------------------
void PosSettingsAdvOp::itemPrototypes(
        QList<HbAbstractViewItem *> &protoTypeList)
    {
    qDebug() << "+ PosSettingsAdvOp::itemPrototypes()";
    protoTypeList = mDataForm->itemPrototypes();
    qDebug() << "- PosSettingsAdvOp::itemPrototypes()";
    }

