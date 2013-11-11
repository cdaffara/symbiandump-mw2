/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  MPXPluginMonitor testing implementation (mpxpluginmonitor.h)
*
*/


#include <mpxplaybackplugin.h>

#include "commontestclass.h"

// Begin CMPXPluginMonitor testing implementation (mpxpluginmonitor.h)============
// -----------------------------------------------------------------------------
// CCommonTestClass::CreatePluginMonitor()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::CreatePluginMonitorL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::CreatePluginMonitorL testing CMPXPluginMonitor::NewL begin")));
    iLog->Log(_L("CCommonTestClass::CreatePluginMonitorL testing CMPXPluginMonitor::NewL begin"));
    TInt err = KErrNone;
    if ( iPluginMonitor != NULL)
        {
        delete iPluginMonitor;
        }
    iPluginMonitor = NULL;
    iPluginMonitor = CMPXPluginMonitor::NewL((TUid)KMPXPlaybackInterfaceUid/*, *this*/);//edit
//    iPluginMonitor = CMPXPluginMonitor::NewL((TUid)KMPXPlaybackInterfaceUid, *this);
    if ( iPluginMonitor == NULL)
        {
        err = KErrNotFound;
        }
	FTRACE(FPrint(_L("CCommonTestClass::CreatePluginMonitorL testing CMPXPluginMonitor::NewL end=%d"), err));
    iLog->Log(_L("CCommonTestClass::CreatePluginMonitorL testing CMPXPluginMonitor::NewL end=%d"), err);
	return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::DeletePluginMonitorL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::DeletePluginMonitorL()
    {
	FTRACE(FPrint(_L("CCommonTestClass::DeletePluginMonitorL testing CMPXPluginMonitor::~ begin")));
    iLog->Log(_L("CCommonTestClass::DeletePluginMonitorL testing CMPXPluginMonitor::~ begin"));
    TInt err = KErrNone;
    delete iPluginMonitor;
    iPluginMonitor = NULL;
	FTRACE(FPrint(_L("CCommonTestClass::DeletePluginMonitorL testing CMPXPluginMonitor::~ end=%d"), err));
    iLog->Log(_L("CCommonTestClass::DeletePluginMonitorL testing CMPXPluginMonitor::~ end=%d"), err);
	return err;
    }

void CCommonTestClass::PluginsChangedL()
    {
    }
void CCommonTestClass::HandlePluginUnload(const TUid& /*aPluginUid*/)//add
	{
	}
	
// end of CMPXPluginMonitor (mpxpluginmonitor.h) =============================================
