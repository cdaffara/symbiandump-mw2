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

#include <QList>
#include <QGraphicsGridLayout>

#include <hbaction.h>
#include <hbcommonnote.h>
#include <hbinstance.h>
#include <hblabel.h>
#include <hblineedit.h>
#include <hbmainwindow.h>
#include <hbmenu.h>
#include <hbpushbutton.h>
#include <hbtoolbar.h>

#include "toolbarview.h"

ToolbarView::ToolbarView() :
    HbView(),
    portraitOrientation(true),
    mirrored(false),
    orientationChangeAction(0),
    mirroringChangeAction(0)
{
    //setIcon(HbIcon(":/toolbardemo/toolbar"));
    setTitle("Toolbar demo view");

    QGraphicsGridLayout *layout = new QGraphicsGridLayout();

    QSizePolicy policy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    HbLabel *textLabel = new HbLabel("Text:", this);
    textLabel->setSizePolicy(policy);
    textLabel->setObjectName("textLabel");
    layout->addItem(textLabel, 0, 0);

    textLineEdit = new HbLineEdit("button text", this);
    textLineEdit->setSizePolicy(policy);
    textLineEdit->setObjectName("textLineEdit");
    layout->addItem(textLineEdit, 0, 1);

    HbLabel *tooltipLabel = new HbLabel("Tooltip:", this);
    tooltipLabel->setSizePolicy(policy);
    tooltipLabel->setObjectName("tooltipLabel");
    layout->addItem(tooltipLabel, 1, 0);

    tooltipLineEdit = new HbLineEdit("tooltip text", this);
    tooltipLineEdit->setSizePolicy(policy);
    tooltipLineEdit->setObjectName("tooltipLineEdit");
    layout->addItem(tooltipLineEdit, 1, 1);

    HbPushButton *addButton = new HbPushButton("Add", this);
    addButton->setSizePolicy(policy);
    addButton->setObjectName("add");
    layout->addItem(addButton, 2, 0, 1, 2);
    connect(addButton, SIGNAL(clicked()), this, SLOT(toolbarDemoSlot()));

    HbPushButton *clearButton = new HbPushButton("Clear", this);
    clearButton->setSizePolicy(policy);
    clearButton->setObjectName("clear");
    layout->addItem(clearButton, 3, 0, 1, 2);
    connect(clearButton, SIGNAL(clicked()), this, SLOT(toolbarDemoSlot()));

    HbPushButton *showButton = new HbPushButton("Show", this);
    showButton->setSizePolicy(policy);
    showButton->setObjectName("show");
    layout->addItem(showButton, 4, 0, 1, 2);
    connect(showButton, SIGNAL(clicked()), this, SLOT(toolbarDemoSlot()));

    HbPushButton *hideButton = new HbPushButton("Hide", this);
    hideButton->setSizePolicy(policy);
    hideButton->setObjectName("hide");
    layout->addItem(hideButton, 5, 0, 1, 2);
    connect(hideButton, SIGNAL(clicked()), this, SLOT(toolbarDemoSlot()));

    HbLabel *indexLabel = new HbLabel("Index:", this);
    indexLabel->setSizePolicy(policy);
    indexLabel->setObjectName("indexLabel");
    layout->addItem(indexLabel, 6, 0);

    indexLineEdit = new HbLineEdit("0", this);
    indexLineEdit->setSizePolicy(policy);
    indexLineEdit->setObjectName("indexLineEdit");
    layout->addItem(indexLineEdit, 6, 1);

    HbPushButton *insertButton = new HbPushButton("Insert", this);
    insertButton->setSizePolicy(policy);
    insertButton->setObjectName("insert");
    layout->addItem(insertButton, 7, 0, 1, 2);
    connect(insertButton, SIGNAL(clicked()), this, SLOT(toolbarDemoSlot()));

    HbPushButton *removeButton = new HbPushButton("Remove", this);
    removeButton->setSizePolicy(policy);
    removeButton->setObjectName("remove");
    layout->addItem(removeButton, 8, 0, 1, 2);
    connect(removeButton, SIGNAL(clicked()), this, SLOT(toolbarDemoSlot()));

    HbPushButton *iconButton = new HbPushButton("Icon", this);
    iconButton->setSizePolicy(policy);
    iconButton->setObjectName("icon");
    layout->addItem(iconButton, 9, 0, 1, 2);
    connect(iconButton, SIGNAL(clicked()), this, SLOT(toolbarDemoSlot()));

    setLayout(layout);

    if (orientationChangeAction) {
        menu()->addAction(orientationChangeAction);
    } else {
        orientationChangeAction = menu()->addAction( "Change to landscape (auto)" );
        connect(orientationChangeAction, SIGNAL(triggered()), this, SLOT(changeOrientation()));
    }

    if (mirroringChangeAction) {
        menu()->addAction(mirroringChangeAction);
    } else {
        mirroringChangeAction = menu()->addAction( "Turn mirroring on" );
        connect(mirroringChangeAction, SIGNAL(triggered()), this, SLOT(changeMirroring()));
    }

    HbMenu *prtOrMenu = menu()->addMenu( "Set orientation" );
    prtOrMenu->addAction( "Vertical" );
    prtOrMenu->addAction( "Horizontal" );

    connect(prtOrMenu, SIGNAL(triggered(HbAction*)), this, SLOT(setOrientation(HbAction*)));

    HbAction* unsetor = menu()->addAction( "Unset orientation" );
    connect(unsetor, SIGNAL(triggered()), this, SLOT(unsetOrientation()));
}

ToolbarView::~ToolbarView()
{
}

int ToolbarView::selectedIndex(bool& ok)
{
    ok = true;
    bool valid = false;
    QString str = indexLineEdit->text();
    int value = str.toInt(&valid, 10);
    if (!valid && !value) {
        ok = false;
    }
    return value;
}

void ToolbarView::toolbarDemoSlot()
{
    QString objectName = QObject::sender()->objectName();
    if (objectName == "add") {
        toolBar()->show();
        HbAction *action = new HbAction();
        connect(action, SIGNAL(triggered()), this, SLOT(toolButtonTriggered()));
        action->setText(textLineEdit->text());
        action->setToolTip(tooltipLineEdit->text());
        action->setIcon(HbIcon(":/toolbardemo/homer.jpg"));
        toolBar()->addAction(action);
    } else if (objectName == "clear") {
        toolBar()->clearActions();
    } else if (objectName == "show") {
        toolBar()->show();
    } else if (objectName == "hide") {
        toolBar()->hide();
    } else if (objectName == "insert") {
        bool ok;
        int index = selectedIndex(ok);

        if (ok) {
            toolBar()->show();
            HbAction *action = new HbAction();
            connect(action, SIGNAL(triggered()), this, SLOT(toolButtonTriggered()));
            action->setText(textLineEdit->text());
            action->setToolTip(tooltipLineEdit->text());
            action->setIcon(HbIcon(":/toolbardemo/homer.jpg"));

            HbAction* before = NULL;
            if ( index >= 0 && index < toolBar()->actions().count() ) {
                before = (HbAction*)toolBar()->actions()[index];
            }
            toolBar()->insertAction(before,action);
        }
    } else if (objectName == "remove") {
        bool ok;
        int index = selectedIndex(ok);
        if (ok && index >= 0 && index < toolBar()->actions().count()) {
            toolBar()->removeAction(toolBar()->actions()[index]);
        }
    } else if (objectName == "icon") {
        bool ok;
        int index = selectedIndex(ok);
        if (ok && index >= 0 && index < toolBar()->actions().count()) {
            HbAction* action = (HbAction*)toolBar()->actions()[index];
            action->setIcon(HbIcon(":/toolbardemo/barney.gif"));
        }
    }
}

void ToolbarView::toolButtonTriggered()
{
    HbAction *triggeredAction = static_cast<HbAction*>(sender());
    HbCommonNote *note = new HbCommonNote(triggeredAction->text(), HbCommonNote::InformationNoteType);
    note->setAttribute(Qt::WA_DeleteOnClose);
    //note->setUnfadedItems(Hb::ToolBarItem);
    note->setUnfadedItems(Hb::AllItems);
}

void ToolbarView::changeOrientation()
{
    portraitOrientation = !portraitOrientation;

    if (portraitOrientation) {
        orientationChangeAction->setText("Change to landscape (auto)");
    } else {
        orientationChangeAction->setText("Change to portrait (auto)");
    }

    // Change application's orientation
    hbInstance->setOrientation(portraitOrientation? Qt::Vertical : Qt::Horizontal);
}

void ToolbarView::changeMirroring()
{
    mirrored = !mirrored;

    if (mirrored) {
        mirroringChangeAction->setText("Turn mirroring off");
    } else {
        mirroringChangeAction->setText("Turn mirroring on");
    }

    if (mirrored) {
        hbInstance->primaryWindow()->setLayoutDirection(Qt::RightToLeft);
    } else {
        hbInstance->primaryWindow()->setLayoutDirection(Qt::LeftToRight);
    }    
}

void ToolbarView::setOrientation( HbAction* action )
{        
    if ( action->text() == "Vertical" ){
        toolBar()->setOrientation( Qt::Vertical );
    } else if ( action->text() == "Horizontal" ) {
        toolBar()->setOrientation( Qt::Horizontal );
    }
}

void ToolbarView::unsetOrientation()
{
    toolBar()->unsetOrientation();
}
