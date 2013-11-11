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

#include <possettingsgroupboxitem.h>
#include <hbdataformmodelitem.h>
#include <hbgroupbox.h>
#include <hbabstractitemview.h>
#include <QMetaProperty>
#include <hbdataformmodel.h>

// ---------------------------------------------------------------------------
// PosSettingsGroupboxItem:::PosSettingsGroupboxItem
// ---------------------------------------------------------------------------
//
PosSettingsGroupboxItem::PosSettingsGroupboxItem(QGraphicsItem *parent) :
HbDataFormViewItem(parent)
{
}

// ---------------------------------------------------------------------------
// PosSettingsGroupboxItem:::~PosSettingsGroupboxItem
// ---------------------------------------------------------------------------
//
PosSettingsGroupboxItem::~PosSettingsGroupboxItem()
{
}

// ---------------------------------------------------------------------------
// PosSettingsGroupboxItem:::createItem
// ---------------------------------------------------------------------------
//
HbAbstractViewItem* PosSettingsGroupboxItem::createItem()
{
    return new PosSettingsGroupboxItem(*this);
}

// ---------------------------------------------------------------------------
// PosSettingsGroupboxItem:::canSetModelIndex
// ---------------------------------------------------------------------------

bool PosSettingsGroupboxItem::canSetModelIndex(const QModelIndex &index) const
{
        int type = index.data(HbDataFormModelItem::ItemTypeRole).toInt();
        if (type == GroupboxItem ) {
                return true;
        }
        else {
                return false;
        }
}

// ---------------------------------------------------------------------------
// PosSettingsGroupboxItem:::createCustomWidget
// ---------------------------------------------------------------------------
//
HbWidget* PosSettingsGroupboxItem::createCustomWidget()
{
    HbDataFormModelItem::DataItemType itemType =
    static_cast<HbDataFormModelItem::DataItemType> 
    ( modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt() );
    
    switch (itemType) {
      case GroupboxItem:
            { 
            //custom button
            QString str =
                    modelIndex().data(HbDataFormModelItem::PropertyRole).toString();
           HbGroupBox* groupbox = new HbGroupBox(this);
					  // Use the Groupbox widget for heading
					  groupbox->setHeading(str);
            mWidget = groupbox;
            return mWidget;
            }
        default:
            return 0;
    }
}

// ---------------------------------------------------------------------------
// PosSettingsGroupboxItem:::restore
// ---------------------------------------------------------------------------
//
void PosSettingsGroupboxItem::restore()
{
    HbDataFormViewItem::restore();
    if (mWidget) {
        HbDataFormModelItem::DataItemType itemType = static_cast<HbDataFormModelItem::DataItemType>(
            modelIndex().data(HbDataFormModelItem::ItemTypeRole).toInt());

        if(itemType == GroupboxItem) {

            QModelIndex itemIndex = modelIndex();
            HbDataFormModel *model = static_cast<HbDataFormModel*>(itemView()->model());;
            HbDataFormModelItem *modelItem = static_cast<HbDataFormModelItem*>(
                model->itemFromIndex(itemIndex));

            const QMetaObject *metaObj = mWidget->metaObject();
            int count = metaObj->propertyCount();
            for (int i = 0; i < count; ++i) {
                QMetaProperty metaProperty = metaObj->property(i);
                if (metaProperty.isValid() && metaProperty.isWritable()) {
                    metaProperty.write(mWidget,modelItem->contentWidgetData(metaProperty.name()));
                }
          }
      }
  }
}
   
//EOF
