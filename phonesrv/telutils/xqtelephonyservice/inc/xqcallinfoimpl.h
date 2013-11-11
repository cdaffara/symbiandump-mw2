/*
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
* Description:  Provides implementation class for XQCallInfo -class.
*
*/

#ifndef XQCALLINFOIMPL_H
#define XQCALLINFOIMPL_H

#include <QObject>
#include <QString>
#include <mcallinformationobserver.h>

#include "xqcallinfo.h"

// forward declarations
class CCallInformation;
class CCallInfoIter;

/*!
    XQCallInfoImpl
    This class provides implementation for XQCallInfo -class.
*/
class XQCallInfoImpl : 
    public XQCallInfo, 
    public MCallInformationObserver
    {
    Q_OBJECT
public:
    
    /*!
       Constructor
    */
    explicit XQCallInfoImpl();

    /*!
       Destructor
    */
    virtual ~XQCallInfoImpl();
    
    /*!
        \fn void getCalls( QList<CallInfo> &calls )
        
       Gets the snapshot of ongoing calls.
    */
    void getCalls(QList<CallInfo> &calls);
    
    /*!
        \fn bool isCallInState( CCPCall::TCallState state ) const
        
        Returns information whether call in given state exist.
    */
    bool isCallInState(CCPCall::TCallState state) const;
    
public: // from MCallInformationObserver.    
    
    /*!
        \fn void CallInformationChanged()
        
        Informs XQCallinfo that there are changes in ongoing calls.
    */
    void CallInformationChanged();
    
private:
    
    /*!
        \fn void setCallInformationObserver()
        
        Sets the observer for changes in current calls
    */
    void setCallInformationObserver();
    
private:
    
    CCallInformation *m_CallInformation;
    
    CCallInfoIter *m_Iter;
    };


#endif // XQCALLINFOIMPL_H

