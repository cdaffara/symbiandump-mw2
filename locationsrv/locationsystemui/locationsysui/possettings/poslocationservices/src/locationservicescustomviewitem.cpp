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
 * Description: This is extended as HbDataFormViewItem to be used to create 
 *              group items for location services
 *
 */


#include "locationservicescustomviewitem.h"
#include "locationserviceswidget.h"
#include <hbdataformmodel.h>
#include <hbabstractitemview.h>
#include <qmetaobject.h>


// ---------------------------------------------------------------------------
// LocationServicesCustomViewItem::LocationServicesCustomViewItem
// ---------------------------------------------------------------------------
//
LocationServicesCustomViewItem::LocationServicesCustomViewItem(
        QGraphicsItem *parent) :
    HbDataFormViewItem(parent)
    {    	
    }

// ---------------------------------------------------------------------------
// LocationServicesCustomViewItem::~LocationServicesCustomViewItem
// ---------------------------------------------------------------------------
//
LocationServicesCustomViewItem::~LocationServicesCustomViewItem()
    {
    }

// ---------------------------------------------------------------------------
// LocationServicesCustomViewItem::createItem
// ---------------------------------------------------------------------------
//
HbAbstractViewItem* LocationServicesCustomViewItem::createItem()
{
    return new LocationServicesCustomViewItem(*this);
}

// ---------------------------------------------------------------------------
// LocationServicesCustomViewItem::canSetModelIndex
// ---------------------------------------------------------------------------
//
bool LocationServicesCustomViewItem::canSetModelIndex(const QModelIndex &index) const
{
    int type = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
    if (type == LocationServicesCustomItem) {
        return true;
    }
    else {
        return false;
    }
}


// ---------------------------------------------------------------------------
// LocationServicesCustomViewItem::createCustomWidget
// ---------------------------------------------------------------------------
//
HbWidget * LocationServicesCustomViewItem::createCustomWidget()
    {
    //get the type of the model
    int type = modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt();
    if (type == LocationServicesCustomItem)
        {
        //create the instance of location widget
        LocationServicesWidget* locationservicesWidget =
                new LocationServicesWidget();
        mPushWidget = locationservicesWidget;
        return mPushWidget;
        }
    else
        {
        //else return 0
        return 0;
        }
    }

// ---------------------------------------------------------------------------
// LocationServicesCustomViewItem::restore
// ---------------------------------------------------------------------------
//
void LocationServicesCustomViewItem::restore()
{
    HbDataFormViewItem::restore();
    if (mPushWidget) {
        HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
            modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

        if(itemType == LocationServicesCustomItem) {

            QModelIndex itemIndex = modelIndex();
            HbDataFormModel *model = static_cast<HbDataFormModel*>(itemView()->model());;
            HbDataFormModelItem *modelItem = static_cast<HbDataFormModelItem*>(
                model->itemFromIndex(itemIndex));

            const QMetaObject *metaObj = mPushWidget->metaObject();
            int count = metaObj->propertyCount();
            for (int i = 0; i < count; ++i) {
                QMetaProperty metaProperty = metaObj->property(i);
                if (metaProperty.isValid() && metaProperty.isWritable()) {
                    metaProperty.write(mPushWidget,modelItem->contentWidgetData(metaProperty.name()));
                }

            }
        }
    }
}

// ---------------------------------------------------------------------------
// LocationServicesCustomViewItem::save
// ---------------------------------------------------------------------------
//
/*void LocationServicesCustomViewItem::save()
{
    HbDataFormViewItem::save();
    if (mPushWidget) {
        HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
            modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

        if(itemType == LocationServicesCustomItem) {

            QModelIndex itemIndex = modelIndex();
            HbDataFormModel *model = static_cast<HbDataFormModel*>(itemView()->model());;
            HbDataFormModelItem *modelItem = static_cast<HbDataFormModelItem*>(
            model->itemFromIndex(itemIndex));
            if (LocationServicesWidget *locationservicesWidget = qobject_cast<LocationServicesWidget *>(mPushWidget)) {
                modelItem->setContentWidgetData("primarytext",locationservicesWidget->primaryText());     
                modelItem->setContentWidgetData("sectext",locationservicesWidget->secText());
                modelItem->setContentWidgetData("sec2text",locationservicesWidget->sec2Text());
            }
        }
    }
}*/

//-------------End of File--------------------------
