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

#ifndef ACTIONCUSTOMITEM_H
#define ACTIONCUSTOMITEM_H


// System includes
#include <qobject.h>
#include <hbdataformviewitem.h>
#include <hbdataformmodelitem.h>

// Forward declarations
class CpItemDataHelper;

// Class declaration
class ActionCustomItem :  public HbDataFormViewItem
{
    Q_OBJECT
public:
    /*!
        Constructor
    */
    ActionCustomItem(QGraphicsItem *parent = 0);
    
    /*!
        Destructor
    */

    ~ActionCustomItem();
    /*!
        From base class CpPluginInterface
        Form more information please check the base class.
    */
     virtual HbAbstractViewItem* createItem();

     virtual bool canSetModelIndex(const QModelIndex &index) const;

     enum { VmbxEditItem = HbDataFormModelItem::CustomItemBase + 1  };
    
protected:

    virtual HbWidget* createCustomWidget();
        
};

#endif // ACTIONCUSTOMITEM_H
