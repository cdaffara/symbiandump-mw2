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

#ifndef SIMPLEVIEW_H_
#define SIMPLEVIEW_H_

#include <hbview.h>
#include <Hbview.h>

class HbLabel;
class HbPushButton;
class QSignalMapper;

class SimpleView : public HbView
    {
    
    Q_OBJECT
    
public:
    
    SimpleView(const QString &name = "Welcome!");

    virtual ~SimpleView();

public slots:

    void setLabelText(int commandId);
        
private:
    
    bool connectButton(HbPushButton *button, int commandId);
    
    HbLabel *label;
    QSignalMapper *mapper;
    
    };

#endif /* SimpleView_H_ */
