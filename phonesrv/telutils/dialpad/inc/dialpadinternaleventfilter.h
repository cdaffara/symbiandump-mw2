/*!
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
* Description: Dialpad keypad
*
*/

#ifndef DIALPADINTERNALEVENTFILTER_H
#define DIALPADINTERNALEVENTFILTER_H

#include <QObject>
#include <QTimer>

/**
* Event filter for internal development purposes.
* PhoneUi.exe can be shut down with long press 9 and
* restarted with long press 8 in Dialpad
*/
class DialpadInternalEventFilter : public QObject
{
    Q_OBJECT
    
public:
    DialpadInternalEventFilter();
    ~DialpadInternalEventFilter(); 
protected:
    bool eventFilter(QObject *watched, QEvent *event);
private slots:
    void togglePhoneUi();
private:    
    void shutdown();
    void start();

private:
    QTimer *mLongPressTimer;
    int mKey;
};

#endif // DIALPADINTERNALEVENTFILTER_H
