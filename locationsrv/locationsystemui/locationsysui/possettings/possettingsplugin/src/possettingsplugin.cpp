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
 * Description: This is for Positioning Settings plugin in ControlPanel
 *
 */

#include "possettingsplugin.h"
#include "possettingsview.h"
#include "possettingspluginhelper.h"
#include "apilogger.h"

#include <QtCore>
#include <QDebug>
#include <QTranslator>
#include <cpsettingformentryitemdataimpl.h>



// ---------------------------------------------------------------------------
// PosSettingsPlugin::PosSettingsPlugin
// ---------------------------------------------------------------------------
//
PosSettingsPlugin::PosSettingsPlugin():mTranslator(0)
    {
#ifndef QT_NO_DEBUG_OUTPUT
    ApiLogger::OpenLogFile();
    qInstallMsgHandler(ApiLogger::MyOutputHandler);
#endif
    
    qDebug() << "+ PosSettingsPluginHelper::PosSettingsPlugin()";
   mTranslator = new QTranslator();
    
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    mTranslator->load(path + QString("lilocationmw_") + lang);
    qApp->installTranslator(mTranslator);
    
    qDebug() << "- PosSettingsPluginHelper::PosSettingsPlugin()";
    }

// ---------------------------------------------------------------------------
// PosSettingsPlugin::~PosSettingsPlugin
// ---------------------------------------------------------------------------
//
PosSettingsPlugin::~PosSettingsPlugin()
    {
    qDebug() << "+ PosSettingsPluginHelper::~PosSettingsPlugin()";
    if (mTranslator)
        {
        if (mTranslator->isEmpty() == false)
            qApp->removeTranslator(mTranslator);
        delete mTranslator;
        }
    qDebug() << "- PosSettingsPluginHelper::~PosSettingsPlugin()";
    #ifndef QT_NO_DEBUG_OUTPUT
    qInstallMsgHandler(0);
    ApiLogger::CloseLogFile();
    #endif
    }

// ---------------------------------------------------------------------------
// PosSettingsPlugin::createSettingFormItemData
// ---------------------------------------------------------------------------
//
QList<CpSettingFormItemData*> PosSettingsPlugin::createSettingFormItemData(
        CpItemDataHelper &itemDataHelper) const
    {    
    qDebug() << "+ PosSettingsPluginHelper::createSettingFormItemData()";
    PosSettingsPluginHelper helper;
    QList<CpSettingFormItemData*> formItemDataList;
    if (!helper.isFeatureFlagEnabled() && helper.isPositionPsyAvailable())
        {
        CpSettingFormItemData *itemData = new CpSettingFormEntryItemDataImpl<
                PosSettingsView> (CpSettingFormEntryItemData::ListEntryItem, itemDataHelper, hbTrId(
                "txt_cp_dblist_positioning"), hbTrId(
                "txt_cp_dblist_positioning_val_change_positioning_s"), "qtg_large_maps");
        formItemDataList.append(itemData);
        }
        qDebug() << "- PosSettingsPluginHelper::createSettingFormItemData()";
        return formItemDataList;
    }

Q_EXPORT_PLUGIN2(possettingsplugin, PosSettingsPlugin)

