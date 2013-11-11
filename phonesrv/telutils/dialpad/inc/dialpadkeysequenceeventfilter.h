/*!
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
* Description: Implements key sequence recognition filter for Dialpad.
*
*/

#ifndef DIALPADKEYSEQUENCEEVENTFILTER_H
#define DIALPADKEYSEQUENCEEVENTFILTER_H

#include <QObject>
#include <xqappmgr.h>
#include <xqaiwinterfacedescriptor.h>

class Dialpad;

/*!
    DialpadKeySequenceEventFilter
    Class provides key sequence recognition and handling.
*/
class DialpadKeySequenceEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit DialpadKeySequenceEventFilter(
        Dialpad* dialpad, 
        QObject* parent = 0);
    virtual ~DialpadKeySequenceEventFilter();

public:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void constructKeySequenceToHandlerMappings();
    XQAiwInterfaceDescriptor findKeySequenceHandler(
        const QString &keySequenceCandidate);
    
private:
    /*! Dialpad. Not own. */
    Dialpad* mDialpad;
    
    /*! Application manager. */
    XQApplicationManager mAiwMgr;
    
    /*! Key sequence validators and associated sequence handlers. */
    QMap<QString, XQAiwInterfaceDescriptor> mValidators;
};

#endif // DIALPADKEYSEQUENCEEVENTFILTER_H
