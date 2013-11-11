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
* Description: Provides information about remote party information of ongoing calls.
*
*/

#ifndef XQCALLREMOTEPARTYINFO_H
#define XQCALLREMOTEPARTYINFO_H

#include <QObject>
#include <QString>
#include <QGlobal.h>
#include <ccpdefs.h>
#include <mcallremotepartyinfo.h>

// forward declarations 
class XQCallRemotePartyInfoPrivate;
class CallRemotePartyInfoPrivate;

/*!
    CallRemotePartyInfo
    Holds calls remote party information.
*/
NONSHARABLE_CLASS( CallRemotePartyInfo )
    {
public:
    /*!
       Constructors and destructor
    */
    CallRemotePartyInfo();
    CallRemotePartyInfo(const CallRemotePartyInfo&);
    CallRemotePartyInfo& operator=(const CallRemotePartyInfo&);
    ~CallRemotePartyInfo();

public:
    void setCallIndex(int index); 
    void setAddress(QString address); 
    void setMatchedName(QString matchedName); 
    void setNameFromNetwork(QString nameFromNetwork); 
    void setCompanyName(QString companyName); 
    void setSATCall(bool SATCall); 
    void setRemoteIdentity(MCallRemotePartyInfo::TRemoteIdentityStatus remoteIdentity); 
    
    int callIndex() const;  
    QString address() const;
    QString matchedName() const ; 
    QString nameFromNetwork() const; 
    QString companyName() const; 
    bool SATCall() const; 
    MCallRemotePartyInfo::TRemoteIdentityStatus remoteIdentity() const;
    
protected:  
    CallRemotePartyInfoPrivate* d_ptr;  
     
    };
 

#ifdef BUILD_XQCALLREMOTEPARTYINFO
#define XQCALLREMOTEPARTYINFO_EXPORT Q_DECL_EXPORT
#else
#define XQCALLREMOTEPARTYINFO_EXPORT Q_DECL_IMPORT
#endif
/*!
    XQCallRemotePartyInfo
    Class to be used for receiving information about ongoing call remote party informations.
    
    Usage example:
    
    QList<CallRemotePartyInfo> infos;
    CallRemotePartyInfo callRemotePartyInfoFirst;
    CallRemotePartyInfo callRemotePartyInfoLast;
    XQCallRemotePartyInfo xqCallRemotePartyInfo;
    xqCallRemotePartyInfo.getCallRemotePartyInfos(infos);
    callRemotePartyInfoFirst = infos.first();
    callRemotePartyInfoLast = infos.last();
     
*/


class XQCALLREMOTEPARTYINFO_EXPORT XQCallRemotePartyInfo: public QObject
{
    Q_OBJECT

public: 
    /*!
       Constructor and destructor
    */
    XQCallRemotePartyInfo ();
    ~XQCallRemotePartyInfo ();
        
    /*!
        \fn void getCallRemotePartyInfos(QList<CallRemotePartyInfo> &infos)
        
        Gets the snapshot of remote party information of ongoing calls.
    */
    void getCallRemotePartyInfos(QList<CallRemotePartyInfo> &infos);
 
signals:
    
    /*!
        \fn void callRemotePartyInformationChanged()
        
        Signals that there are changes in remote party informations of ongoing calls. 
        Client should fetch the call infomations after receiving the notification.
    */
    void callRemotePartyInformationChanged();

private slots:       
   
/*!
        \fn void sendCallRemotePartyInfoChangedSignal()
        
        Sends signal call remote party info changed signal to client's.
    */
    void sendCallRemotePartyInfoChangedSignal();
    
    
protected:  

     XQCallRemotePartyInfoPrivate* const d_ptr;  

     Q_DECLARE_PRIVATE(XQCallRemotePartyInfo);  
 
};

#endif // XQCALLREMOTEPARTYINFO_H
