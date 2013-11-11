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
* Description: 
*
*/

#ifndef DIALPADSYMBIANWRAPPERPRIVATE_H
#define DIALPADSYMBIANWRAPPERPRIVATE_H

#include <QString>
#include <qglobal.h>

class DialpadSymbianWrapper;


/*!
    DialpadSymbianWrapperPrivate
    
*/
class  DialpadSymbianWrapperPrivate
{

public:
    
    DialpadSymbianWrapperPrivate(DialpadSymbianWrapper *parent);
    ~DialpadSymbianWrapperPrivate();
    
    int getMailboxNumber(QString &vmbxNumber);
    int getVideoMailboxNumber(QString &vmbxNumber);
    
    int defineMailboxNumber(QString &vmbxNumber);
    int defineVideoMailboxNumber(QString &vmbxNumber);

private:

    DialpadSymbianWrapper *const q_ptr;
    Q_DECLARE_PUBLIC(DialpadSymbianWrapper)

    int mTimes;
};

#endif // DIALPADSYMBIANWRAPPERPRIVATE_H
