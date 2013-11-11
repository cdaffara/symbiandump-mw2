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

// System includes
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QStringList>
#include <cpitemdatahelper.h>
#include <hblineedit.h>
#include <hbdataformmodel.h>
#include "actioncustomitem.h"

// User includes
#include "vmbxuiengine.h"
#include "vmbxcpgroup.h"
#include "loggerutil.h"

/*!
    Constructor
*/
VmbxCpGroup::VmbxCpGroup(
    CpItemDataHelper &itemDataHelper) :
        CpSettingFormItemData(HbDataFormModelItem::GroupItem,
        hbTrId("txt_phone_subhead_call_mbx"), NULL),
        mUiEngine(NULL),
        mDefaultMailboxEditor(NULL),
        mCsVoice1Editor(NULL),
        mCsVideo1Editor(NULL),
        mItemHelper(itemDataHelper)
{ 
    _DBGLOG("VmbxCpGroup::VmbxCpGroup >")
    // Localization file loading
    QTranslator translator; 
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";

    bool translatorLoaded = translator.load(path + "telephone_cp_" + lang);
    _DBGLOG("VmbxCpGroup::VmbxCpGroup translator loaded")
    if (translatorLoaded && qApp) {
        qApp->installTranslator(&translator);
        _DBGLOG("VmbxCpGroup::VmbxCpGroup translator installed")
    }

    mUiEngine = new VmbxUiEngine();

    loadingPreparation();
    
    // Request notify when VMBX number changed
    mUiEngine->notifyVmbxNumberChange(true);
    bool ret = connect(mUiEngine,
            SIGNAL(voiceMailboxEngineEntriesUpdated(const TVmbxType)), 
            this, SLOT(updateVmbxNumber(const TVmbxType)));
    _DBGLOG2("VmbxCpGroup::VmbxCpGroup connect ret=", ret)
    _DBGLOG("VmbxCpGroup::VmbxCpGroup <")
}

/*!
    Destructor
*/
VmbxCpGroup::~VmbxCpGroup()
{
    _DBGLOG("VmbxCpGroup::~VmbxCpGroup")
    if (mUiEngine) {
        delete mUiEngine;
        mUiEngine = NULL;
    }
}

/*!
    Preparation before loading
*/
void VmbxCpGroup::loadingPreparation()
{
    _DBGLOG("VmbxCpGroup::loadingPreparation >")
    
    ActionCustomItem *viewItem = new ActionCustomItem();
    mItemHelper.addItemPrototype(viewItem);
    mItemHelper.connectToForm(
        SIGNAL(itemShown(QModelIndex)), this, SLOT(itemShown(QModelIndex)));
  
    // Create cs voice primary editor
    mCsVoice1Editor= createDataFormModelItem(EVmbxVoice);       
    if (mUiEngine->isVideoSupport()){
        _DBGLOG("VmbxCpGroup::loadingPreparation create video1 mailbox")
        mCsVideo1Editor= createDataFormModelItem(EVmbxVideo);
    }
    _DBGLOG("VmbxCpGroup::loadingPreparation <")
}

HbDataFormModelItem* VmbxCpGroup::createDataFormModelItem(
    const TVmbxType vmbxType)
{
    _DBGLOG("VmbxCpGroup::createDataFormModelItem >")
    QString number("");
    HbDataFormModelItem *item = 0;
    QString name("");
    bool create(false);
    if (EVmbxVoice == vmbxType) {
        _DBGLOG("VmbxCpGroup::createDataFormModelItem voice")
        name = hbTrId("txt_phone_setlabel_default_mbx_val_voice");
        create = true;
    } else if (EVmbxVideo == vmbxType) {
        _DBGLOG("VmbxCpGroup::createDataFormModelItem video")
        name = hbTrId("txt_phone_setlabel_default_mbx_val_video");
        create = true;
    }
    if (create) {
        item = new HbDataFormModelItem(
            static_cast<HbDataFormModelItem::DataItemType>(
            ActionCustomItem::VmbxEditItem),name);

        appendChild(item);
        
        if (EVmbxVoice == vmbxType) {
            if (mUiEngine->isVoiceWritable()) {
                _DBGLOG("VmbxCpGroup::createDataFormModelItem add voice")
                mItemHelper.addConnection(item, SIGNAL(editItemClicked()),
                this, SLOT(queryCsVoiceNumber()));
            }else {
                _DBGLOG("VmbxCpGroup::createDataFormModelItem voice no read")
                item->setEnabled(false);   
            }                
        } else if (EVmbxVideo == vmbxType) {
            if (mUiEngine->isVideoWritable()) {
                _DBGLOG("VmbxCpGroup::createDataFormModelItem add video")
                mItemHelper.addConnection(item, SIGNAL(editItemClicked()),
                    this, SLOT(queryCsVideoNumber()));
            }else {
                _DBGLOG("VmbxCpGroup::createDataFormModelItem video no read")
                item->setEnabled(false);   
            }                
        }
    }
    _DBGLOG("VmbxCpGroup::createDataFormModelItem <")
    return item;
}

/*!
    Query voice number
*/
void VmbxCpGroup::queryCsVoiceNumber()
{
    _DBGLOG("VmbxCpGroup::queryCsVoiceNumber >")
    mCsVoice1Editor->setEnabled(false);
    QString number;
    int result = mUiEngine->queryVoiceNumber(number);
    _DBGLOG2("VmbxCpGroup::queryCsVoiceNumber result=", result)
    if (KErrNone == result) {
        _DBGLOG2("VmbxCpGroup::queryCsVoiceNumber >, number=", number) 
        mUiEngine->setCsVoiceNumber(number);
    }
    mCsVoice1Editor->setEnabled(true);
    _DBGLOG("VmbxCpGroup::queryCsVoiceNumber <")
}

/*!
    Query video number
*/
void VmbxCpGroup::queryCsVideoNumber()
{
    _DBGLOG("VmbxCpGroup::queryCsVideoNumber >")
    mCsVideo1Editor->setEnabled(false);
    QString number;
    int result = mUiEngine->queryVideoNumber(number);
    _DBGLOG2("VmbxCpGroup::queryCsVideoNumber  result=", result)
    if (KErrNone == result) {
        _DBGLOG2("VmbxCpGroup::queryCsVideoNumber >, number=", number)
        mUiEngine->setCsVideoNumber(number);
    }
    mCsVideo1Editor->setEnabled(true);
    _DBGLOG("VmbxCpGroup::queryCsVideoNumber <")
}

/*!
    Update number when vmbx number changed by OTA,OMA etc.
*/
void VmbxCpGroup::updateVmbxNumber(const TVmbxType vmbxType)
{
    _DBGLOG("VmbxCpGroup::updateVmbxNumber >")

    QString number("");
    if (EVmbxVoice == vmbxType) {
        if (mCsVoice1Editor && mUiEngine){
            mUiEngine->getCsVoiceNumber(number);
            _DBGLOG2("VmbxCpGroup::updateVmbxNumber\
                 number=", number)
            UpdateWidgetContent(mCsVoice1Index, number); 
        }
    } else if (EVmbxVideo == vmbxType) {
        if (mCsVideo1Editor && mUiEngine){
            mUiEngine->getCsVideoNumber(number);
            _DBGLOG2("VmbxCpGroup::updateVmbxNumber\
                 newUiValue=", number)
            UpdateWidgetContent(mCsVideo1Index, number);  
        }
    }  
    _DBGLOG("VmbxCpGroup::updateVmbxNumber <")
}

/*!
  slot for item has been shown on the UI
 */
void VmbxCpGroup::itemShown(const QModelIndex &item)
{
    _DBGLOG("VmbxCpGroup::itemShown >")

    HbDataFormModelItem* modelItem = mItemHelper.modelItemFromModelIndex(item);

    if (modelItem == mCsVoice1Editor ) {
        _DBGLOG("VmbxCpGroup::itemShown voice")
        mCsVoice1Index = item;
        updateVmbxNumber(EVmbxVoice);
    }else if (modelItem == mCsVideo1Editor ) {
        _DBGLOG("VmbxCpGroup::itemShown video")
        mCsVideo1Index = item;
        updateVmbxNumber(EVmbxVideo);
    }
    _DBGLOG("VmbxCpGroup::itemShown <")
}

/*!
  Update the vmbx number to the UI
 */
void VmbxCpGroup::UpdateWidgetContent(const QModelIndex &item,
        const QString &string)
{
    _DBGLOG("VmbxCpGroup::UpdateWidgetContent >")
    HbLineEdit* widget = static_cast<HbLineEdit *>(
            mItemHelper.widgetFromModelIndex(item));
    if (widget) {
        _DBGLOG("VmbxCpGroup::UpdateWidgetContent widegt found")
        widget->setText(string);
    }
    if (item == mCsVoice1Index) {
        mCsVoice1Editor->setContentWidgetData("setText", string);
    } else if (item == mCsVideo1Index) {
        mCsVideo1Editor->setContentWidgetData("setText", string);
    }
    _DBGLOG("VmbxCpGroup::UpdateWidgetContent <")

}

//End of file
