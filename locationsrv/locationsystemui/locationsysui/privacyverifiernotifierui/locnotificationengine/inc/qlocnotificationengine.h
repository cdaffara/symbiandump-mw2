/*
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
* Description:  Implementation of positioning settings engine class. 
*
*/

#ifndef QLOCNOTIFICATIONENGINE_H
#define QLOCNOTIFICATIONENGINE_H

#include <qcontactrequests.h>

class QContactManager;

/**
 * This class is used as a container for Pos requestor data which needs to be resolved.
 * After resolution the result is returned in the same class. QPosRequestorData is used
 * so as to avoid exposing CPosRequestorData to 
 */

class QPosRequestorData
    {
    
public :
    enum IdFormat
        {
        /** The requestor ID format is unknown. */
        EIdFormatUnknown,
        /** The requestor is identified by name. */
        EIdFormatGenericName,
        /** The requestor is identified by phone number. */
        EIdFormatPhoneNumber,
        /** The requestor is identified by URL. */
        EIdFormatUrl,
        /** The requestor is identified by email address. */
        EIdFormatEmail,
        /** The requestor is identified by the URL of the SIP session. */
        EIdFormatSIPUrl,
        /** The requestor is identified by the IMS public identity. */
        EIdFormatIMSPublicIdentity,
        /** The requestor is identified by Mobile identification Number. */
        EIdFormatMIN,
        /** The requestor is identified by Mobile Directory Number. */
        EIdFormatMDN
        };
public:
    /*
     * Default construtor
     */
    QPosRequestorData();
    
    /*
     * Destructor for QPosRequestorData
     */
    ~QPosRequestorData();
    
public:
    /*
     * Setter method for QPosRequestorData
     * @param idFormat -  Field in which string needs to be searched for resolution
     * @param idString -  String which needs to be resolved
     */
    void setPosRequestorData(const QPosRequestorData::IdFormat& idFormat, const QString& idString);
    
    /*
     * Getter method for QPosRequestorData
     * @param idFormat -  Field in which string needs to be searched for resolution
     * @param idString -  String which needs to be resolved \ after resolution the result is stored in this
     */
    void getPosRequestorData(QPosRequestorData::IdFormat& idFormat, QString& idString) const;
    
private:
    
    // contains string to be resolve -  string after resolution is stored in this
    QString mIdString;
    //idformat of the string to be searched - indicates in what field the string needs to be searched in
    IdFormat mIdFormat; 
    };

/**
 * QLocNotificationEngine class provides a Qt interface to perform operations such as
 * requestor resolution, activating indicators for missed location notfications etc.
 * 
 */

class QLocNotificationEngine
    {
    
public:
    /**
     * Default constructor
     * This throws an error if construction fails.
     */
    QLocNotificationEngine();
    
    /**
     * Destructor 
     */
    ~QLocNotificationEngine();
    
public:
    /*
     * Resolves requestor from phoebook
     * @param[IN,OUT] posRequestorData resolved requestors
     */
    void processRequestors(QPosRequestorData& posRequestorData);

private: // data member
    // Contact manager instance
    QtMobility::QContactManager* mContactManager;

    };

#endif // QLOCNOTIFICATIONENGINE_H 
