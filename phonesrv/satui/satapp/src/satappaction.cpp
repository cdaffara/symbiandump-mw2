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

#include <QEventLoop>
#include <QCoreApplication>
#include <QVariant>
#include <QStringList>
#include <QtDebug>

#include "satappaction.h"

// ----------------------------------------------------------------------------
// global des2str
// trasmit from symbian descripter to QString
// ----------------------------------------------------------------------------
//
QString des2str(const TDesC& desc)
{
    return QString::fromUtf16(desc.Ptr(), desc.Length());
}

// ======== MEMBER FUNCTIONS ==================================================

// ----------------------------------------------------------------------------
// SatAppAction::SatAppAction
// ----------------------------------------------------------------------------
//
SatAppAction::SatAppAction(int action, QObject *parent)
: QObject(parent)
{
    qDebug("SATAPP: SatAppAction::SatAppAction");
    mAction = action;
    mResponse = ESatNoResponseFromUser;
	mState = Active;
    
    //monitor application
    SAT_ASSERT(connect(
        qApp,SIGNAL(aboutToQuit()),
        this,SLOT(completeWithSessionTerminated())));
}

// ----------------------------------------------------------------------------
// SatAppAction::~SatAppAction
// ----------------------------------------------------------------------------
//
SatAppAction::~SatAppAction()
{
    if (mState==Waiting)
        complete(); // release waiting thread
}

// ----------------------------------------------------------------------------
// SatAppAction::waitUntilCompleted
// sync method that returns when this action is completed
// ----------------------------------------------------------------------------
//
void SatAppAction::waitUntilCompleted()
{
    qDebug("SATAPP: SatAppAction::waitUntilCompleted");
    if (mState==Completed) return; // no need to wait!
    QEventLoop loop;
    SAT_ASSERT(connect(
        this,SIGNAL(actionCompleted(SatAppAction*)),
        &loop,SLOT(quit())));
    mState=Waiting;
    qDebug("SATAPP: waitUntilCompleted: waiting...");
    loop.exec();
    // When use the FSW complete the satapp, the signal aboutToExit
    // can not be recieved. temp solution
    if(Waiting == mState){
        mState=Completed;
        mResponse=ESatSessionTerminatedByUser;
    }
    qDebug("SATAPP: waitUntilCompleted: ...released");
}

// ----------------------------------------------------------------------------
// SatAppAction::complete
// completes the action
// ----------------------------------------------------------------------------
//
void SatAppAction::complete(TSatUiResponse resp)
{
    qDebug("SATAPP: SatAppAction::complete resp= %x in", resp);
    if (mState!=Completed) {
        qDebug("SATAPP: SatAppAction::completed");
        setResponse(resp);
        mState = Completed;
        emit actionCompleted(this);
    }
    qDebug("SATAPP: SatAppAction::complete resp= %x out", resp);
}

// ----------------------------------------------------------------------------
// SatAppAction::set
// setter for QVariant
// ----------------------------------------------------------------------------
//
void SatAppAction::set(const QString& key, QVariant value)
{
    qDebug() << "SATAPP: SatAppAction::set(" <<
        key << "=" << value << ")";
    mParams[key] = value;
}

// ----------------------------------------------------------------------------
// SatAppAction::set
// setter for Symbian Descripter
// ----------------------------------------------------------------------------
//
void SatAppAction::set(const QString& key, const TDesC& value)
{
    set(key,des2str(value));
}

// ----------------------------------------------------------------------------
// SatAppAction::set
// setter for Symbian MDesCArray - Menu list
// ----------------------------------------------------------------------------
//
void SatAppAction::set(const QString& key, const MDesCArray& value)
{
    QList<QVariant> list;
    for(int i=0; i<value.MdcaCount(); ++i)
        list.append(des2str(value.MdcaPoint(i)));
    set(key,list);
}

// ----------------------------------------------------------------------------
// SatAppAction::set
// setter for CArrayFixFlat<TInt> - mene icon
// ----------------------------------------------------------------------------
//
void SatAppAction::set(const QString& key, CArrayFixFlat<TInt>& value)
{
    QList<QVariant> list;
    for(int i=0; i<value.Count(); ++i)
        list.append(value[i]);
    set(key,list);
}

// ----------------------------------------------------------------------------
// SatAppAction::set
// setter for TTimeIntervalSeconds - duration
// ----------------------------------------------------------------------------
//
void SatAppAction::set(const QString& key, TTimeIntervalSeconds& value)
{
    int value_millisec = value.Int() * 1000; // sec->millisec
    set(key,value_millisec);
}

// ----------------------------------------------------------------------------
// SatAppAction::set
// setter for TTimeIntervalMicroSeconds - duration
// ----------------------------------------------------------------------------
//
void SatAppAction::set(const QString& key, TTimeIntervalMicroSeconds& value)
{
    int value_millisec = value.Int64() / 1000; // microsec->millisec
    set(key,value_millisec);
}

// ----------------------------------------------------------------------------
// SatAppAction::value
// getter by key
// ----------------------------------------------------------------------------
//
QVariant SatAppAction::value(const QString& key)
{
    if (!mParams.contains(key))
    {
        // lazy-fetch params implementation goes here.
        // for instance if Icon is requested, request for the icons here.
        // however at this moment, all parameters should be present.
        qFatal("SATAPP: SatAppAction::value - param missing: %s",
            key.toLatin1().data());
    }
    return mParams[key];
}

// ----------------------------------------------------------------------------
// SatAppAction::hasValue
// ----------------------------------------------------------------------------
//
bool SatAppAction::hasValue(const QString& key)
{
    return mParams.contains(key);
}

//End of file
