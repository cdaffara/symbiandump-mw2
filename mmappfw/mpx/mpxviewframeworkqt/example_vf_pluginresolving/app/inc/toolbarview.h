/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TOOLBARVIEW_H
#define TOOLBARVIEW_H

#include <QObject>

#include <hbview.h>

class HbAction;
class HbLineEdit;
class HbMenu;
class HbMenuItem;
class HbMainWindow;

class ToolbarView : public HbView
{
    Q_OBJECT

public:
    ToolbarView();
    ~ToolbarView();

private:
    int selectedIndex(bool& ok);

private slots:
    void toolbarDemoSlot();
    void toolButtonTriggered();
    void changeOrientation();
    void changeMirroring();
    void setOrientation( HbAction* action );
    void unsetOrientation();

private:
    bool portraitOrientation;
    bool mirrored;

    HbAction *orientationChangeAction;
    HbAction *mirroringChangeAction;
    HbLineEdit *tooltipLineEdit;
    HbLineEdit *textLineEdit;
    HbLineEdit *indexLineEdit;
};

#endif // TOOLBARVIEW_H
