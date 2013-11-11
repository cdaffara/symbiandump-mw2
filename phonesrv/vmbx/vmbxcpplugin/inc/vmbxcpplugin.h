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

#ifndef VMBXCPPLUGIN_H
#define VMBXCPPLUGIN_H

// System includes
#include <qobject.h>
#include <cpplugininterface.h>

// Forward declarations
class CpItemDataHelper;

// Class declaration
class VmbxCpPlugin : public QObject, public CpPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(CpPluginInterface)
    
public:
    /*!
        Constructor
    */
    VmbxCpPlugin();
    
    /*!
        Destructor
    */
    ~VmbxCpPlugin();

    /*!
        From base class CpPluginInterface
        Form more information please check the base class.
    */
    virtual QList<CpSettingFormItemData *> createSettingFormItemData(
        CpItemDataHelper &itemDataHelper) const;
};

#endif // VMBXCPPLUGIN_H
