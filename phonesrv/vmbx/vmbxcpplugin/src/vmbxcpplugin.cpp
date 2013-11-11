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
#include <QDebug>
#include <cpitemdatahelper.h>

// User includes
#include "vmbxcpplugin.h"
#include "vmbxcpgroup.h"
#include "loggerutil.h"

/*!
    Constructor
*/
VmbxCpPlugin::VmbxCpPlugin()
{
    _DBGLOG("VmbxCpPlugin::VmbxCpPlugin()")
}

/*!
    Destructor
*/
VmbxCpPlugin::~VmbxCpPlugin()
{
    _DBGLOG("VmbxCpPlugin::~VmbxCpPlugin()")
}

/*!
    From base class CpPluginInterface
*/
QList<CpSettingFormItemData *> VmbxCpPlugin::createSettingFormItemData(
    CpItemDataHelper &itemDataHelper) const
{
    _DBGLOG("VmbxCpPlugin::createSettingFormItemData")
    QList<CpSettingFormItemData *> ret;
    ret.append(new VmbxCpGroup(itemDataHelper));
    _DBGLOG("VmbxCpPlugin::createSettingFormItemData created")
    return ret;
}
Q_EXPORT_PLUGIN2(VmbxCpPlugin, VmbxCpPlugin);

//End of file
