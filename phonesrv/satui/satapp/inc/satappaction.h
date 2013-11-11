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
*
*/

#ifndef SATAPPACTION_H
#define SATAPPACTION_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMap>
#include <e32base.h> // for symbian-to-qt conversions
#include "MSatUiObserver.h" // for the param type definitions
#include "satappconstant.h"

// class declaration
class SatAppAction : public QObject
{
    Q_OBJECT

public:
    /** creates an action with given identity (TSatAction) */
    SatAppAction(int action, QObject *parent = 0);
    ~SatAppAction();

    /** synchronous wait until the action has been completed */
    void waitUntilCompleted();
    /** completes the action with guiven response */
    void complete(TSatUiResponse resp);

public slots:

    /** completes the action */
    void complete() {complete(response());}

    // comfy methods for completing with certain response
    void completeWithSuccess() {complete(ESatSuccess);}
    void completeWithFailure() {complete(ESatFailure);}
    void completeWithNoResponse() {complete(ESatNoResponseFromUser);}
    void completeWithBackRequested() {complete(ESatBackwardModeRequestedByUser);}
    void completeWithSessionTerminated() {complete(ESatSessionTerminatedByUser);}

signals:

    /** signalled when the action has been completed */
    void actionCompleted(SatAppAction *action);

public: // param access

    /** returns the identity of the action (TSatAction) */
    int action() {return mAction;}

    /** sets the action response */
    void setResponse(TSatUiResponse resp) {mResponse=resp;}
    /** the action response (TSatUiResponse) */
    TSatUiResponse response() {return mResponse;}

    /** sets a value (arbitary type) */
    void set(const QString& key, QVariant value);

    /** sets QString value after doing symbian-to-qt conversion */
    void set(const QString& key, const TDesC& value);
    /** sets QStringList value after doing symbian-to-qt conversion */
    void set(const QString& key, const MDesCArray& value);
    /** sets QList<int> value after doing symbian-to-qt conversion */
    void set(const QString& key, CArrayFixFlat<TInt>& value);
    /** sets int(millisec) value after doing symbian-to-qt conversion */
    void set(const QString& key, TTimeIntervalSeconds& value);
    /** sets int(millisec) value after doing symbian-to-qt conversion */
    void set(const QString& key, TTimeIntervalMicroSeconds& value);

    /** returns a value (arbitary type) */
    QVariant value(const QString &key);
    /** whether a value is present */
    bool hasValue(const QString &key);

private: // data

    // identity of this action
    int mAction;

    // response
    TSatUiResponse mResponse;
    
    // parameter map
    QMap<QString, QVariant> mParams;

    // internal action state
    enum State {Active,Waiting,Completed};
    State mState;
};

#endif // SATAPPACTION_H

