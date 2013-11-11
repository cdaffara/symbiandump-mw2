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

#include "simpleview.h"

#include <hbview.h>
#include <hblabel.h>
#include <hbmenu.h>
#include <hbaction.h>
#include <hbpusHbutton.h>

#include <QSizePolicy>
#include <QSignalMapper>
#include <QGraphicsGridLayout>

SimpleView::SimpleView(const QString &name) 
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
    layout->addItem(label, 7, 1, 3, 1);
    
    connect(mapper, SIGNAL(mapped(int)), this, SLOT(setLabelText(int)));
    
    bool result = false;

    }

SimpleView::~SimpleView()
    {
    }

bool SimpleView::connectButton(HbPushButton *button, int commandId)
    {
    bool result = connect(button, SIGNAL(clicked()), mapper, SLOT(map()));
    if (result) {
        mapper->setMapping(button, commandId);
    }
    return result;
    }

void SimpleView::setLabelText(int commandId) 
    {
    QString ltext("command id: %1");
    label->setText(ltext.arg(commandId));
    }

