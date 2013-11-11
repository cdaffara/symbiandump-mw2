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
 * Description: This file implements the plugin interface for positioning verification dialogs
 *
 */
#include "posverificationdialogplugin.h"
//User Includes
#include "customdocumentloader.h"
#include "posverificationcustomdialog.h"
#include "apilogger.h"

#include <QDebug>
#include <QTCore>

//Carbide Application docml file
const char *POSVERFICATIONDIALOG_DOCML =
        ":/xml/posverificationcustomdialog.docml";

// This plugin implements one custom dialog type
enum DeviceDlgType
    {
    PosVerificationDialogType
    };
static const struct
    {
    const char *mTypeString;
    DeviceDlgType mType;
    } verdlgtypes[] =
    {
        {
        "hb.posverifiction.dialog/1.0", PosVerificationDialogType
        }
    };

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::PosVerificationDialogPlugin()    
// Constructor
//-----------------------------------------------------------------------    
PosVerificationDialogPlugin::PosVerificationDialogPlugin() :
    mTranslator(NULL)
    {
    qDebug() << "+PosVerificationDialogPlugin::PosVerificationDialogPlugin()";
    //#ifndef QT_NO_DEBUG_OUTPUT
    //    ApiLogger::OpenLogFile();
    //    qInstallMsgHandler(ApiLogger::MyOutputHandler);
    //#endif
    qDebug() << "-PosVerificationDialogPlugin::PosVerificationDialogPlugin()";
    }

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::~PosVerificationDialogPlugin()    
// Destructor
//----------------------------------------------------------------------- 
PosVerificationDialogPlugin::~PosVerificationDialogPlugin()
    {
    qDebug()
            << "+PosVerificationDialogPlugin::~PosVerificationDialogPlugin()";
    if (mTranslator)
        {
        if (mTranslator->isEmpty() == false)
            qApp->removeTranslator(mTranslator);
        delete mTranslator;
        }
    qDebug()
            << "-PosVerificationDialogPlugin::~PosVerificationDialogPlugin()";
    //#ifndef QT_NO_DEBUG_OUTPUT
    //    ApiLogger::CloseLogFile();
    //#endif
    }

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::accessAllowed 
// Check if client is allowed to use device dialog widget
//----------------------------------------------------------------------- 
bool PosVerificationDialogPlugin::accessAllowed(
        const QString &deviceDialogType, const QVariantMap &parameters,
        const QVariantMap &securityInfo) const
    {
    qDebug() << "+PosVerificationDialogPlugin::accessAllowed()";
    Q_UNUSED(deviceDialogType)
    Q_UNUSED(parameters)
    Q_UNUSED(securityInfo)

    // This plugin doesn't perform operations that may compromise security. All clients
    // are allowed to use.
    qDebug() << "-PosVerificationDialogPlugin::accessAllowed()";
    return true;
    }

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::createDeviceDialog 
// Create device dialog widget
//-----------------------------------------------------------------------
HbDeviceDialogInterface * PosVerificationDialogPlugin::createDeviceDialog(
        const QString &deviceDialogType, const QVariantMap &parameters)
    {
    qDebug() << "+PosVerificationDialogPlugin::createDeviceDialog(): Type"
            << deviceDialogType;
    //install the translator. Fix for Bug id: ou1cimx1#462081
    installTranslator();

    int i;
    PosVerificationCustomDialog * dialog = 0;
    const int numTypes = sizeof(verdlgtypes) / sizeof(verdlgtypes[0]);
    for (i = 0; i < numTypes; ++i)
        {
        if (verdlgtypes[i].mTypeString == deviceDialogType)
            {
            break;
            }
        }
    if (i < numTypes)
        {
        switch (verdlgtypes[i].mType)
            {
            case PosVerificationDialogType:
                {
                CustomDocumentLoader loader;
                bool ok = false;
                loader.load(POSVERFICATIONDIALOG_DOCML, &ok);
                Q_ASSERT_X(ok, "PosVerificationCustomDialog",
                        "invalid DocML file");

                if (ok)
                    {
                    dialog = qobject_cast<PosVerificationCustomDialog *> (
                            loader.findWidget("PosVerificationCustomDialog"));
                    if (dialog)
                        {
                        dialog->getWidgets(loader);
                        dialog->setDeviceDialogParameters(parameters);
                        qDebug() << "createDeviceDialog(): success";
                        }
                    else
                        {
                        qDebug()
                                << "createDeviceDialog(): creation of device dialog failed";
                        }
                    }
                else
                    {
                    qDebug() << "createDeviceDialog(): loading docml failed";
                    }
                break;
                }
            default:
                {
                qDebug() << "createDeviceDialog(): default case";
                Q_ASSERT(false);
                break;
                }
            }
        }
    else
        {
        qDebug()
                << "createDeviceDialog(): Dialog type not supported by this plugin";
        }
    qDebug() << "-PosVerificationDialogPlugin::createDeviceDialog()";
    return dialog;
    }

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::deviceDialogInfo 
// Return information of device dialog the plugin creates
//-----------------------------------------------------------------------
bool PosVerificationDialogPlugin::deviceDialogInfo(
        const QString &deviceDialogType, const QVariantMap &parameters,
        DeviceDialogInfo *info) const
    {
    qDebug() << "+PosVerificationDialogPlugin::deviceDialogInfo()";
    Q_UNUSED(parameters)
    Q_UNUSED(deviceDialogType)

    info->group = GenericDeviceDialogGroup;
    info->flags = NoDeviceDialogFlags;
    info->priority = DefaultPriority;
    qDebug() << "-PosVerificationDialogPlugin::deviceDialogInfo()";
    return true;
    }

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::deviceDialogTypes 
// Return device dialog types this plugin implements
//-----------------------------------------------------------------------
QStringList PosVerificationDialogPlugin::deviceDialogTypes() const
    {
    qDebug() << "+PosVerificationDialogPlugin::deviceDialogTypes()";
    QStringList types;
    const int numTypes = sizeof(verdlgtypes) / sizeof(verdlgtypes[0]);
    for (int i = 0; i < numTypes; i++)
        {
        types.append(verdlgtypes[i].mTypeString);
        }
    qDebug() << "-PosVerificationDialogPlugin::deviceDialogTypes()";
    return types;
    }

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::pluginFlags 
// Return plugin flags
//-----------------------------------------------------------------------
HbDeviceDialogPlugin::PluginFlags PosVerificationDialogPlugin::pluginFlags() const
    {
    qDebug() << "PosVerificationDialogPlugin::pluginFlags()";
    return NoPluginFlags;
    }

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::error 
// Return last error
//-----------------------------------------------------------------------
int PosVerificationDialogPlugin::error() const
    {
    qDebug() << "PosVerificationDialogPlugin::error()";
    return 0;
    }

//-----------------------------------------------------------------------
// PosVerificationDialogPlugin::installTranslator 
// loads the qt message file and installs the translator
//-----------------------------------------------------------------------
void PosVerificationDialogPlugin::installTranslator()
    {
    if (!mTranslator)
        {
        mTranslator = new QTranslator();
        }
    else
        {
        return;
        }
    QString lang = QLocale::system().name();
    QString path = "z:/resource/qt/translations/";
    QString altpath = "c://resource/";
    bool loaded = false;
    loaded = mTranslator->load(path + QString("lilocationmw_") + lang);
    if (loaded == false)
        {
        loaded = mTranslator->load(altpath + QString("lilocationmw_") + lang);
        }
    if (loaded)
        {
        qApp->installTranslator(mTranslator);
        }
    }

Q_EXPORT_PLUGIN2(posverificationdialogplugin, PosVerificationDialogPlugin)

