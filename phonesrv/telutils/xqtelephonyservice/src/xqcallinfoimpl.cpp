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
* Description:  Provides implementation for XQCallInfo -class.
*
*/

#include "xqcallinfoimpl.h"
#include <ccallinformation.h>
#include <ccallinfoiter.h>
#include <mcall.h>

XQCallInfoImpl::XQCallInfoImpl(): XQCallInfo(), m_CallInformation(0)
{
    TRAPD( error, m_CallInformation = CCallInformation::NewL());
    qt_symbian_throwIfError(error);
    setCallInformationObserver();
}

XQCallInfoImpl::~XQCallInfoImpl()
{
    delete m_CallInformation;
}

void XQCallInfoImpl::setCallInformationObserver()
{
    m_CallInformation->NotifyCallInformationChanges(*this);
}

void XQCallInfoImpl::getCalls(QList<CallInfo> &calls)
{
    TRAPD( error, m_Iter = &m_CallInformation->GetCallsL() );
    qt_symbian_throwIfError(error);
    
    for (m_Iter->First(); !m_Iter->IsDone(); m_Iter->Next()) {
        const MCall& call = m_Iter->Current();
        CallInfo callInfo;
        callInfo.m_Direction = call.CallDirection();
        callInfo.m_Index = call.CallIndex();
        callInfo.m_ServiceId = call.ServiceId();
        callInfo.m_Type = call.CallType();
        callInfo.m_State = call.CallState();
        calls.append(callInfo);
     }
}

bool XQCallInfoImpl::isCallInState(CCPCall::TCallState state) const
{
    return m_CallInformation->IsCallInState(state);
}

void XQCallInfoImpl::CallInformationChanged()
{
    XQCallInfo::sendCallInfoChangedSignal();
}
