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
 * Description:Declaration of the Positioning Settings PLugins loader 
 *
 */

#ifndef PosSettingsPluginsLoader_H
#define PosSettingsPluginsLoader_H

#include "possettingsadvinterface.h"

#include <QObject>
#include <e32cmn.h>

class PosSettingsPluginsLoader 
    {
    
public:
    /**
     * Constructor
     */
    PosSettingsPluginsLoader();
    /**
     * Destructor
     */
    ~PosSettingsPluginsLoader();
    /*
     * Detects & loads the plugins that implement the 
     * PosSettingsAdvInterface class
     */
     QList<PosSettingsAdvInterface*> loadPlugins();
private:
     /**
      * Reads the cenrep key value to obtain the default order in which 
      * the positioning settings plugins need to be loaded.
      */
     void ParseCenRepKey();

private:
     /**
      * Contains list of plugin's dll name in the order in which it
      * is supposed to be displayed in the advanced view
      */
     QList<QString> mDllNameList;
    

    };

#endif //PosSettingsPluginsLoader_H
