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

#ifndef DIALPADSYMBIANWRAPPER_H
#define DIALPADSYMBIANWRAPPER_H

#include <QObject>
#include <QString>
#include <qglobal.h>

class DialpadSymbianWrapperPrivate;

/*!
    DialpadSymbianWrapper
*/
class  DialpadSymbianWrapper : public QObject
{

Q_OBJECT

public:
    explicit DialpadSymbianWrapper(QObject *parent = 0);
    ~DialpadSymbianWrapper();
    
    int getMailboxNumber(QString &vmbxNumber);
    int getVideoMailboxNumber(QString &vmbxNumber);
        
    int defineMailboxNumber(QString &vmbxNumber);
    int defineVideoMailboxNumber(QString &vmbxNumber);
    
    bool changeSilentModeState();

protected:
    DialpadSymbianWrapperPrivate *const d_ptr;
   
    Q_DECLARE_PRIVATE(DialpadSymbianWrapper)
};

#endif // DIALPADSYMBIANWRAPPER_H
