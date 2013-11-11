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
* Description:
*
*/

#ifndef CBSUIMAINWINDOW_H
#define CBSUIMAINWINDOW_H

#include <hbmainwindow.h>

class HbMainWindow;
class MCbs;
class CbsUiViewManager;

class CbsUiMainWindow : public HbMainWindow 
{
public:
    CbsUiMainWindow();
    virtual ~CbsUiMainWindow();

private:
    /*
     * Own
     */
    MCbs *mServer;
    /*
     * Own
     */    
    CbsUiViewManager *mViewManager;
};

#endif /* CBSUIMAINWINDOW_H */
