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

#ifndef WELCOMEVIEW_H_
#define WELCOMEVIEW_H_

#include <hbview.h>

class HbLabel;
class HbPushButton;
class QSignalMapper;

class WelcomeView : public HbView
    {
    
    Q_OBJECT
    
public:
    
    WelcomeView(const QString &name = "Welcome!");

    virtual ~WelcomeView();

public slots:

    void setLabelText(int commandId);
        
private:
    
    bool connectButton(HbPushButton *button, int commandId);
    
    HbLabel *label;
    QSignalMapper *mapper;
    
    };

#endif /* WELCOMEVIEW_H_ */
