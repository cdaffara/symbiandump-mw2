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
* for the groupbox on the DataForm
*/

#ifndef POSSETTINGS_GROUPBOXITEM_H
#define POSSETTINGS_GROUPBOXITEM_H

#include <QObject>

#include <hbdataformviewitem.h>
#include <hbdataformmodelitem.h>

const int GroupboxItem = HbDataFormModelItem::CustomItemBase+12; 
 
class PosSettingsGroupboxItem : public HbDataFormViewItem
{
Q_OBJECT

public: //constructor and destructor
    PosSettingsGroupboxItem(QGraphicsItem *parent);
    ~PosSettingsGroupboxItem();
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

#endif // POSSETTINGS_GROUPBOXITEM_H
