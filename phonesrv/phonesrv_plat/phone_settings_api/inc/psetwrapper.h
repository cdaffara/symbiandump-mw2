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
 * Description:  
 *
 */
#ifndef PSETWRAPPER_H
#define PSETWRAPPER_H

#include <QObject>

class CPsetContainer; 
class PSetCliWrapper;
class PSetCallWaitingWrapper;
class PSetCallDivertingWrapper;
class PSetNetworkWrapper;
class PSetCallBarringWrapper;

#ifdef BUILD_PSETWRAPPER
#define PSETWRAPPER_EXPORT Q_DECL_EXPORT
#else
#define PSETWRAPPER_EXPORT Q_DECL_IMPORT
#endif

class PSETWRAPPER_EXPORT PSetWrapper: public QObject
{
    Q_OBJECT

public:
    
    explicit PSetWrapper(QObject *parent = NULL);
    
    ~PSetWrapper();
    
public: // Functions:  
    /**
     Returns reference to call line identification settings.
     @exception bad alloc if creations fails
     */
    PSetCliWrapper& cliWrapper(); 
    
    /**
     Returns reference to call waiting settings.
     @exception bad alloc, if creations fails.
     */
    PSetCallWaitingWrapper& callWaitingWrapper();
    
    /**
     Returns reference to call diverting settings.
     @exception bad alloc, if creations fails.
     */
    PSetCallDivertingWrapper& callDivertingWrapper();
    
    /**
     Returns reference to network settings.
     @exception bad alloc, if creations fails.
     */
    PSetNetworkWrapper& networkWrapper();

    /**
     Returns reference to call barring settings.
     @exception bad alloc, if creations fails.
     */
    PSetCallBarringWrapper& callBarringWrapper();
    
private: // Data: 
    // Own
    CPsetContainer *m_psetContainer; 
        
    // Phone setting handlers 
    PSetCliWrapper *m_wrapperCli; 
    
    PSetCallWaitingWrapper *m_callWaitingWrapper;
    
    PSetCallDivertingWrapper *m_callDivertingWrapper;
    
    PSetNetworkWrapper *m_networkWrapper;
    
    PSetCallBarringWrapper *m_callBarringWrapper;
    
};
#endif // PSETWRAPPER

