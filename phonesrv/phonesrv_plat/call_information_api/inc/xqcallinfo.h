/*!
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
* Description: Provides information about ongoing calls.
*
*/

#ifndef XQCALLINFO_H
#define XQCALLINFO_H

#include <QObject>
#include <QString>
#include <ccpdefs.h>


// forward declarations 
class XQCallInfoImpl;

/*!
    CallInfo
    Holds calls information.
*/
NONSHARABLE_CLASS( CallInfo )
    {
public:
    int callIndex() const { return  m_Index; }
    CCPCall::TCallType callType() const { return  m_Type; }
    CCPCall::TCallState callState() const { return  m_State; }
    unsigned int serviceId() const { return  m_ServiceId; }
    CCPCall::TCallDirection callDirection() const { return  m_Direction; }
    
    CCPCall::TCallState m_State;
    CCPCall::TCallType  m_Type;
    CCPCall::TCallDirection  m_Direction;
    int  m_Index;
    unsigned int  m_ServiceId;
    };


#ifdef BUILD_XQCALLINFO
#define XQCALLINFO_EXPORT Q_DECL_EXPORT
#else
#define XQCALLINFO_EXPORT Q_DECL_IMPORT
#endif
/*!
    XQCallInfo
    Class to be used for receiving information about ongoing calls. 

    @code
    XQCallInfo *callInfo = XQCallInfo::create(); 
    @endcode
    
*/
class XQCALLINFO_EXPORT XQCallInfo: public QObject
{
    Q_OBJECT
    
public:
    
    /*!
        \fn XQCallInfo *create
        
        Creates XQCallInfo instance.
    */
    static XQCallInfo *create();
    
    /*!
        \fn void getCallsL(QList<CallInfo> &calls)
        
        Gets the snapshot of ongoing calls.
    */
    virtual void getCalls(QList<CallInfo> &calls) = 0;
    
    /*!
        \fn bool isCallInState( CCPCall::TCallState state ) const
        
        Returns information whether call in given state exist.
    */
    virtual bool isCallInState(CCPCall::TCallState state) const = 0;

signals:
    
    /*!
        \fn void CallInformationChanged()
        
        Signals that there are changes in ongoing calls. Client should fetch 
        the call infomations after receiving the notification.
    */
    void callInformationChanged();
    
protected:
    
    /*!
        \fn void sendSignal()
        
        Sends signal call info changed signal to client's.
    */
    void sendCallInfoChangedSignal();
    
    /*!
       Constructor
    */
    XQCallInfo();
};

#endif // XQCALLINFO_H
