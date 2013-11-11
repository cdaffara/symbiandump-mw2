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
* Description: dialog waiter
*
*
*/

#ifndef DIALOGWAITER_H
#define DIALOGWAITER_H

// INCLUDES
#include <QObject>
#include <QEventLoop>

// FORWARD DECLARATION
class HbAction;

/**
 * This utility synchronises access to dialogs, popups, menus and
 * other ORBIT resources. How to use:
 * <code>
 * HbDialog* dialog = new HbDialog();
 * DialogWaiter waiter();
 * dialog->open(&waiter, SLOT(done(HbAction*)));
 * HbAction* result = waiter.wait();
 * delete dialog;
 * if (result==...) ...
 * </code>
 */
class DialogWaiter : public QObject
{
    Q_OBJECT

    public:
    
        /**
         * waits until dialog exits
         * returns dialog exit action
         */
        HbAction* wait();

    private slots:
        void done(HbAction* result);

    private:
        QEventLoop mLoop;
        HbAction* mResult;

};

#endif  // DIALOGWAITER_H
