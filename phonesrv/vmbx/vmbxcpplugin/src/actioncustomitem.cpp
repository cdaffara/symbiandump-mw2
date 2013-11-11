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
 * Description:
 *
 */

// System includes
#include <QDebug>
#include <QVariant>

#include <hbdataformmodelitem.h>
#include <cpitemdatahelper.h>
#include "customedit.h"

// User includes
#include "actioncustomitem.h"
#include "loggerutil.h"

/*!
    Constructor
*/
ActionCustomItem::ActionCustomItem(QGraphicsItem *parent)
    : HbDataFormViewItem(parent)
{
    _DBGLOG("ActionCustomItem::ActionCustomItem()")
}

/*!
    Destructor
*/
ActionCustomItem::~ActionCustomItem()
{
    _DBGLOG("VmbxCpPlugin::~ActionCustomItem()")
}

/*!
    From base class HbDataFormViewItem
*/
HbAbstractViewItem* ActionCustomItem::createItem()
{
    _DBGLOG("ActionCustomItem::createItem <>")
    return new ActionCustomItem(*this);
}

/*!
    From base class HbDataFormViewItem
*/
HbWidget* ActionCustomItem::createCustomWidget()
{
    _DBGLOG("ActionCustomItem::createCustomWidget >")
    int type = modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt();

    CustomEdit *edit = 0;
    if (type == VmbxEditItem) {
        edit = new CustomEdit(this);
        edit->setEchoMode(HbLineEdit::Normal );
        edit->setReadOnly(true);        
        _DBGLOG("ActionCustomItem::createCustomWidget customItem editor")
    }
    _DBGLOG("ActionCustomItem::createCustomWidget default<")
    return edit;
}

bool ActionCustomItem::canSetModelIndex(const QModelIndex &index) const
{
    _DBGLOG("ActionCustomItem::canSetModelIndex >")
    int type = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
    return type == VmbxEditItem;
}


//End of file
