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
* Description:  Implementation of the Ut_vmbxCpPlugin class
*
*/

// System includes
#include <e32base.h>
#include <cpitemdatahelper.h>

// User includes
#include "vmbxcpplugin.h"
#include "vmbxcpgroup.h"
#include "ut_vmbxcpplugin.h"

/*!
    Ut_vmbxCpPlugin::createSettingFormItemData
*/
void Ut_vmbxCpPlugin::testCreateSettingFormItemData()
{
    CpItemDataHelper itemDataHelper;
    VmbxCpPlugin *plugin = new VmbxCpPlugin();
    QVERIFY(plugin);
    QList<CpSettingFormItemData *> list;
    list = plugin->createSettingFormItemData(itemDataHelper);
    QVERIFY2(0 != list[0], "createSettingFormItemData failed");
    delete plugin;
    plugin = NULL;
}

/*!
    VmbxCpGroup construtor and destroctor
*/
void Ut_vmbxCpPlugin::testCreateVmbxCpGroup()
{
    CpItemDataHelper itemDataHelper;
    VmbxCpGroup *group = new VmbxCpGroup(itemDataHelper);
    QVERIFY(group);
    delete group;
    group = NULL;
}
//End of file
