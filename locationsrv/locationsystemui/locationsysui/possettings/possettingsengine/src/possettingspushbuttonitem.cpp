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
* Description:  This class provides custom list item for the
* DataForm
*
*/

#include <possettingspushbuttonitem.h>
#include <hbdataformmodelitem.h>
#include <hbpushbutton.h>
#include <hbabstractitemview.h>
#include <QMetaProperty>
#include <hbdataformmodel.h>

// ---------------------------------------------------------------------------
// PosSettingsPushButtonItem:::PosSettingsPushButtonItem
// ---------------------------------------------------------------------------
//
PosSettingsPushButtonItem::PosSettingsPushButtonItem(QGraphicsItem *parent) :
HbDataFormViewItem(parent)
{
}

// ---------------------------------------------------------------------------
// PosSettingsPushButtonItem:::~PosSettingsPushButtonItem
// ---------------------------------------------------------------------------
//
PosSettingsPushButtonItem::~PosSettingsPushButtonItem()
{
}

// ---------------------------------------------------------------------------
// PosSettingsPushButtonItem:::createItem
// ---------------------------------------------------------------------------
//
HbAbstractViewItem* PosSettingsPushButtonItem::createItem()
{
    return new PosSettingsPushButtonItem(*this);
}

// ---------------------------------------------------------------------------
// PosSettingsPushButtonItem:::canSetModelIndex
// ---------------------------------------------------------------------------

bool PosSettingsPushButtonItem::canSetModelIndex(const QModelIndex &index) const
{
        int type = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
        if (type == PushButtonItem ) {
                return true;
        }
        else {
                return false;
        }
}

// ---------------------------------------------------------------------------
// PosSettingsPushButtonItem:::createCustomWidget
// ---------------------------------------------------------------------------
//
HbWidget* PosSettingsPushButtonItem::createCustomWidget()
{
    HbDataFormModelItem::DataItemType itemType =
    static_cast<HbDataFormModelItem::DataItemType> 
    ( modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt() );
    
    switch (itemType) {
      case PushButtonItem:
            { 
            //custom button
            QString str =
                    modelIndex().data(HbDataFormModelItem::PropertyRole).toString();
            HbPushButton *button = new HbPushButton(str);
            mWidget = button;
            return mWidget;
            }
        default:
            return 0;
    }
}

// ---------------------------------------------------------------------------
// PosSettingsPushButtonItem:::restore
// ---------------------------------------------------------------------------
//
void PosSettingsPushButtonItem::restore()
{
    HbDataFormViewItem::restore();
    if (mWidget) {
        HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
            modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

        if(itemType == PushButtonItem) {

            QModelIndex itemIndex = modelIndex();
            HbDataFormModel *model = static_cast<HbDataFormModel*>(itemView()->model());;
            HbDataFormModelItem *modelItem = static_cast<HbDataFormModelItem*>(
                model->itemFromIndex(itemIndex));

            const QMetaObject *metaObj = mWidget->metaObject();
            int count = metaObj->propertyCount();
            for (int i = 0; i < count; i++) {
                QMetaProperty metaProperty = metaObj->property(i);
                if (metaProperty.isValid() && metaProperty.isWritable()) {
                    metaProperty.write(mWidget,modelItem->contentWidgetData(metaProperty.name()));
                }
          }
      }
  }
}
   
//EOF
