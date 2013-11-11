/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Positioning settings advanced operation class.
*
*/

#ifndef POSSETTINGSENGADVANCEDOPERATION_H
#define POSSETTINGSENGADVANCEDOPERATION_H


#include "possettingsglobal.h"
#include <qobject.h>
#include <hbview.h>
#include <hbdataform.h>
#include <hbdataformmodel.h>
#include <hbdataformmodelitem.h>

class POSSETTINGS_EXPORT PosSettingsAdvOp : public QObject
    {
    Q_OBJECT
    
public:
    /**
     * Constructor
     */
    PosSettingsAdvOp(HbView* view);
    
    /**
     * Destructor
     */
    ~PosSettingsAdvOp();
    
    /**
     * Appends dataform model item to the dataform model.
     */
    void appendDataFormItem( HbDataFormModelItem *data,
                             HbDataFormModelItem *parent = 0 );
    
    /***
     * Makes connections for the data form model items
     */
    void addConnection( HbDataFormModelItem *item, const char* signal, 
                        QObject *receiver, const char* slot );
    
    /**
     * Used to add menue items & actions
     */
    void addAction( HbAction *action, HbView::ActionContainer 
                    preferedActionContainer );
    /**
     * Gets the index of a particular data form model item
     * @param[in] item - data form model item whose index is required
     * @return - index of the data form model item within the data form model
     */
    QModelIndex indexFromItem(const HbDataFormModelItem *  item  );
    /**
     * Removes connection that were already made
     */
    void removeConnection(HbDataFormModelItem *item, const char *signal, QObject *receiver, const char *slot);
    /**
     * Sets the model item to be expanded/collapsed
     */
    void setExpanded(const QModelIndex &index, bool expanded);
    /**
     * Checks if a particualr data model item is expanded
     */
    bool isExpanded (const QModelIndex &index) const;
    /**
     * Removes a particualr data form model item from the model
     */
    bool removeItem(HbDataFormModelItem* item);
    /**
     * set prototype for data form
     */
    void setItemPrototypes(QList<HbAbstractViewItem *> &protoTypeList);

    /**
     * gets prototype list for the data form
     */
    void itemPrototypes(QList<HbAbstractViewItem *> &protoTypeList);
private:

    /**
     * Pointer to view.
     * Not own.
     */
    HbView* mView;
    
    /**
     * Pointer to data form
     * Not own.
     */
    HbDataForm* mDataForm;
    
    /**
     * Pointer to data form model.
     * Not own.
     */
    HbDataFormModel* mDataFormModel;
    };


#endif /* POSSETTINGSENGADVANCEDOPERATION_H_ */
