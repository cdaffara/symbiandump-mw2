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
* Description: Provides information about ongoing calls.
*
*/

#include "dialpadsymbianwrapper.h"
#include "dialpadsymbianwrapper_p.h"


DialpadSymbianWrapper::DialpadSymbianWrapper(QObject *parent) : 
    QObject(parent),
    d_ptr(new DialpadSymbianWrapperPrivate(this))
{
}

DialpadSymbianWrapper::~DialpadSymbianWrapper()
{
    delete d_ptr;
}

int DialpadSymbianWrapper::getMailboxNumber(QString &vmbxNumber)
{
    //Q_D(DialpadSymbianWrapper);
    return d_ptr->getMailboxNumber(vmbxNumber);
}

int DialpadSymbianWrapper::getVideoMailboxNumber(QString &vmbxNumber)
{
    return d_ptr->getVideoMailboxNumber(vmbxNumber);
}

int DialpadSymbianWrapper::defineMailboxNumber(QString &vmbxNumber)
{
    //Q_D(DialpadSymbianWrapper);
    return d_ptr->defineMailboxNumber(vmbxNumber);
}

int DialpadSymbianWrapper::defineVideoMailboxNumber(QString &vmbxNumber)
{
    return d_ptr->defineVideoMailboxNumber(vmbxNumber);
}

bool DialpadSymbianWrapper::changeSilentModeState()
{
    return d_ptr->changeSilentModeState();
}
