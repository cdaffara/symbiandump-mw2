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
* Description:  Definitions for key sequence recognition component.
*
*/

#ifndef KEYSEQUENCERECOGNITIONSERVICELOG_H
#define KEYSEQUENCERECOGNITIONSERVICELOG_H

#include <QDebug>

class MethodEntryExitDebugPrint
{
public:
    MethodEntryExitDebugPrint(const char* methodDescription)
        :
        m_methodDescription(0),
        m_stackFrame(0)
    {
#ifdef __WINS__
        // Workaround for exception detection as MSL runtime has a bug in
        // std::uncaught_exception() implementation.
        int stackFrame = 0;
        _asm( mov stackFrame, ebp );
        m_stackFrame = stackFrame;
#endif
        m_methodDescription = methodDescription;
        qDebug() << m_methodDescription << "IN";
    };
    
    ~MethodEntryExitDebugPrint() 
    {
#ifdef __WINS__
        // Workaround for exception detection as MSL runtime has a bug in
        // std::uncaught_exception() implementation.
        int stackFrame = 0;
        int returnValue = -1;
        _asm( mov stackFrame, ebp );
        _asm( mov returnValue, ebx );
        if ((stackFrame + sizeof(int) != m_stackFrame) && (0 == returnValue)) {
           qDebug() << m_methodDescription << "EXCEPTION!";
        } else {
           qDebug() << m_methodDescription << "OUT";
        }
#else
        if (std::uncaught_exception()) {
           qDebug() << m_methodDescription << "EXCEPTION!";
        } else {
           qDebug() << m_methodDescription << "OUT";
        }
#endif
    };

private:
    const char* m_methodDescription;
    int m_stackFrame;
};

#define DPRINT_METHODENTRYEXIT \
    MethodEntryExitDebugPrint __entryExitDebugPrint(__PRETTY_FUNCTION__)

#endif // KEYSEQUENCERECOGNITIONSERVICELOG_H
