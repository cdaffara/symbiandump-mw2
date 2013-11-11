/*
 * Copyright (c) 2009 - 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef VMBXCPGROUP_H
#define VMBXCPGROUP_H

// System includes
#include <cpsettingformitemdata.h>
#include <voicemailboxdefs.h>
#include <QModelIndex>


// Forward declarations
class VmbxUiEngine;
class CpItemDataHelper;
class HbDataFormModelItem;


// Class declaration
class VmbxCpGroup : public CpSettingFormItemData
{
    Q_OBJECT

public:
    
    /*!
        Constructor
    */
    VmbxCpGroup( 
        CpItemDataHelper &itemDataHelper);

    /*!
        Destructor
    */
    ~VmbxCpGroup();

private slots:

    /*!
        Query cs voice number when user click line edit
    */
    void queryCsVoiceNumber();

    /*!
        Query cs video number when user click line edit
    */
    void queryCsVideoNumber();

    /*!
        Update number when vmbx number changed by OTA,OMA etc.
    */
    void updateVmbxNumber(const TVmbxType vmbxType);

    /*!
         Slot which handles item updating when it is needed.
     */
    void itemShown(const QModelIndex &item);    
    
private:
    
    /*!
        Preparation before loading
    */
    void loadingPreparation();    
    
    /*!
         Create the data form item by type
     */
    HbDataFormModelItem* createDataFormModelItem(const TVmbxType vmbxType);
        
    /*!
      Update the vmbx number to the UI
     */
    void UpdateWidgetContent(const QModelIndex &item, const QString &string);


private:
    
    // Own.
    VmbxUiEngine *mUiEngine;

    // Not own.
    CpSettingFormItemData *mDefaultMailboxEditor;    

    // Not own.
    HbDataFormModelItem *mCsVoice1Editor;

    // Not own.
    HbDataFormModelItem *mCsVideo1Editor;
    
    // Index
    QModelIndex mCsVoice1Index;

    // Index
    QModelIndex mCsVideo1Index;
    
    // Not own
    CpItemDataHelper &mItemHelper;
};

#endif // VMBXCPGROUP_H
