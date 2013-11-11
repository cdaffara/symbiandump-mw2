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
 * Description:Declaration of the Positioning Settings Advanced View class 
 *
 */

#ifndef POSSETTINGSENG_ADVANCEDVIEW_H
#define POSSETTINGSENG_ADVANCEDVIEW_H

#include "possettingsadvop.h"

#include <QObject>
#include <cpbasesettingview.h>

//class PosSettingsAdvancedDataForm;
class HbDataForm;
class HbDataFormModel;
class HbDataFormModelItem;
class PosSettingsAdvOp;
class PosSettingsAdvInterface;
class PosSettingsPluginsLoader;
class QFileSystemWatcher;

class PosSettingsAdvView : public CpBaseSettingView
    {
Q_OBJECT

public:
    /*
     * Constructor
     */
    PosSettingsAdvView(QGraphicsItem *parent = 0);
    /*
     * Destructor
     */
    ~PosSettingsAdvView();

private slots:
    /**
     * Updates the advanced view in case of change in any of the plugins,
     * this slot needs to be called
     */
    void updateAdvView();

private:

    /**
      * Handle to Heading groupbox
      * Not Own
      */
     HbDataFormModelItem *mHeading;
     
     
    /*
     * Pointer to the HbDataForm
     * Owns
     */
    HbDataForm* mAdvancedDataForm;
    /*
     * Pointer to the PosSettingsAdvOp
     * owns
     */
    PosSettingsAdvOp* mAdvancedOperation;
    /**
     * List containing the plugins
     */
    QList<PosSettingsAdvInterface*> mPluginsList;
    /**
     * Pointer to the PosSettingsPluginsLoader
     */
    PosSettingsPluginsLoader* mPluginsLoader;
    /**
     * Pointer to the QFileSystemWatcher
     * Owns
     */
    QFileSystemWatcher* mWatcher;

    };

#endif //POSSETTINGSENG_ADVANCEDVIEW_H
