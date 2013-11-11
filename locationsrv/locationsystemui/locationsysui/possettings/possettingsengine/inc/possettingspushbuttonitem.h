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
* Description:  This class provides custom list item 
* for the push button on the DataForm
*/

#ifndef POSSETTINGS_PUSHBUTTONITEM_H
#define POSSETTINGS_PUSHBUTTONITEM_H

#include <QObject>

#include <hbdataformviewitem.h>
#include <hbdataformmodelitem.h>

const int PushButtonItem = HbDataFormModelItem::CustomItemBase+11; 
 
class PosSettingsPushButtonItem : public HbDataFormViewItem
{
Q_OBJECT

public: //constructor and destructor
    PosSettingsPushButtonItem(QGraphicsItem *parent);
    ~PosSettingsPushButtonItem();
public: //from HbDataFormViewItem
    /*
     * It will return custom control item
     */
    virtual HbAbstractViewItem* createItem();
    virtual bool canSetModelIndex(const QModelIndex &index) const;
    virtual void restore();
protected:
    /*
     * It will define the custom control item
     */
    virtual HbWidget* createCustomWidget();
    
private:
    /*
     * Does not own
     */
    HbWidget* mWidget;
};

#endif // POSSETTINGS_PUSHBUTTONITEM_H
