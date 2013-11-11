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
#include "dialpadsymbianwrapper_p_stub.h"

DialpadSymbianWrapperPrivate::DialpadSymbianWrapperPrivate(DialpadSymbianWrapper *parent) : 
    q_ptr(parent),
    mTimes(0)
{
}

DialpadSymbianWrapperPrivate::~DialpadSymbianWrapperPrivate()
{
}

int DialpadSymbianWrapperPrivate::getMailboxNumber(QString &vmbxNumber)
{
    int errValue(0);
    switch( mTimes ) {
    case 0:
    case 1:{
       errValue = -1;
       break;
    }
    case 2:{
        errValue = 0;
        break;
    }
    default:
        break;
    }
    
    return errValue;
}

int DialpadSymbianWrapperPrivate::getVideoMailboxNumber(QString &vmbxNumber)
{
    return getMailboxNumber(vmbxNumber);
}

int DialpadSymbianWrapperPrivate::defineMailboxNumber(QString &vmbxNumber)
{
    int errValue(0);
    
    switch( mTimes ) {
    case 0:{
        errValue = 0;
        vmbxNumber.append("12345678");
        break;
    }
    case 1:{
        errValue = -3;
        vmbxNumber.append("");
        break;
    }
    default:
       break;
    }
    mTimes++;
    return errValue;
}

int DialpadSymbianWrapperPrivate::defineVideoMailboxNumber(QString &vmbxNumber)
{
    return defineMailboxNumber(vmbxNumber);	
}
