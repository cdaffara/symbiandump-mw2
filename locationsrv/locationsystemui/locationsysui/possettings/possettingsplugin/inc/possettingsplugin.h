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

#ifndef POSSETTINGS_PLUGIN_H
#define POSSETTINGS_PLUGIN_H

class QTranslator;
#include <cpplugininterface.h>

class PosSettingsPlugin : public QObject, public CpPluginInterface
{
Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
public: //constructor and destructor
    PosSettingsPlugin();
    virtual ~PosSettingsPlugin();
public:
    /*
     * Derived from CpPluginPlatInterface
     */
    virtual QList<CpSettingFormItemData*> createSettingFormItemData(CpItemDataHelper &itemDataHelper) const;    
private:
	/**
	* Holds localization translator handle
	*/
    QTranslator* mTranslator;
};
#endif // POSSETTINGS_PLUGIN_H
