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

#include "welcomeview.h"

#include <hbview.h>
#include <hblabel.h>
#include <hbmenu.h>
#include <hbaction.h>
#include <hbpusHbutton.h>

#include <QSizePolicy>
#include <QSignalMapper>
#include <QGraphicsGridLayout>

WelcomeView::WelcomeView(const QString &name) 
: 
HbView()
    {
    setObjectName(name);
    setTitle(name);
    mapper = new QSignalMapper(this);
    mapper->setParent(this);
    
    setTitle(title());
    QSizePolicy policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    
    QGraphicsGridLayout *layout = new QGraphicsGridLayout();
    
    label = new HbLabel();
    label->setSizePolicy(policy);
//    label->setAlignment(Qt::AlignCenter);
    layout->addItem(label, 7, 1, 3, 1);
    
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setLabelText(int)));
    
    bool result = false;

/*
    HbPushButton *buttonNextSib = new HbPushButton("Right", this);
    buttonNextSib->setSizePolicy(policy);
    layout->addItem(buttonNextSib, 1, 1, 1, 1);
//    connectButton(buttonNextSib, HbViewCommandGoNextSibiling);

    HbPushButton *buttonPreviousSib = new HbPushButton("Left", this);
    buttonPreviousSib->setSizePolicy(policy);
    layout->addItem(buttonPreviousSib, 2, 1, 1, 1);
//    connectButton(buttonPreviousSib, HbViewCommandGoPreviousSibiling);

    
    HbPushButton *buttonParent = new HbPushButton("Parent", this);
    buttonParent->setSizePolicy(policy);
    layout->addItem(buttonParent, 3, 1, 1, 1);
//    connectButton(buttonParent, HbViewCommandGoParent);
    
    HbPushButton *buttonChild = new HbPushButton("Child", this);
    buttonChild->setSizePolicy(policy);
    layout->addItem(buttonChild, 4, 1, 1, 1);
//    connectButton(buttonChild, HbViewCommandGoDefaultChild);
    
    HbPushButton *buttonForward = new HbPushButton("Forward", this);
    buttonForward->setSizePolicy(policy);
    layout->addItem(buttonForward, 5, 1, 1, 1);
//    connectButton(buttonForward, HbViewCommandGoForward);

    HbPushButton *buttonBack = new HbPushButton("Back", this);
    buttonBack->setSizePolicy(policy);
    layout->addItem(buttonBack, 6, 1, 1, 1);
//    connectButton(buttonBack, HbViewCommandGoBack);
    
    this->setLayout(layout);
*/
    }

WelcomeView::~WelcomeView()
    {
    }

bool WelcomeView::connectButton(HbPushButton *button, int commandId)
    {
    bool result = connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    if (result) {
        mapper->setMapping(button, commandId);
    }
    return result;
    }

void WelcomeView::setLabelText(int commandId) 
    {
    QString ltext("command id: %1");
    label->setText(ltext.arg(commandId));
    }

