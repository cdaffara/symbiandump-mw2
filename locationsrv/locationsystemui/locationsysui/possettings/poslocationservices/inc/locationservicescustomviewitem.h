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
 * Description:  This class to be used to create group items for location services
 *
 */
#ifndef LOCATIONSERVICES_CUSTOMVIEWITEM_H
#define LOCATIONSERVICES_CUSTOMVIEWITEM_H

#include <hbdataformviewitem.h>
#include <hbdataformmodelitem.h>
#include <HbWidget>


//dataItemType for Location services custom widget 
const int LocationServicesCustomItem = HbDataFormModelItem::CustomItemBase
        + 15;

class LocationServicesCustomViewItem : public HbDataFormViewItem
    {
Q_OBJECT
public:
    /**
     * Constructor
     */
    explicit LocationServicesCustomViewItem(QGraphicsItem *parent = 0);
    
    /**
     * Destructor
     */
    ~LocationServicesCustomViewItem();

protected:
    
    /**
     * This method is used to create the Custom widget for location services
     * @returns - HbAbstractButton, instance of the custom widget
     */
    virtual HbWidget* createCustomWidget();
    
    /**
     * 
     */
    virtual HbAbstractViewItem* createItem();
    
    /**
     * 
     */
    virtual bool canSetModelIndex(const QModelIndex &index) const;
public slots:
    virtual void restore();
    //virtual void save();
private:
    /**
     * instance of HbAbstractButton
     */
    HbWidget *mPushWidget;
    };
#endif
