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
#ifndef CHECK_CUSTOMVIEWITEM_H
#define CHECK_CUSTOMVIEWITEM_H

#include <hbdataformviewitem.h>
#include <hbdataformmodelitem.h>

class HbWidget;

class CheckCustomViewItem: public HbDataFormViewItem
{
	Q_OBJECT
	
public:
    explicit CheckCustomViewItem(QGraphicsItem *parent = 0);
    ~CheckCustomViewItem();
    virtual HbAbstractViewItem* createItem();
        virtual bool canSetModelIndex(const QModelIndex &index) const;
};
#endif // CHECK_CUSTOMVIEWITEM_H
