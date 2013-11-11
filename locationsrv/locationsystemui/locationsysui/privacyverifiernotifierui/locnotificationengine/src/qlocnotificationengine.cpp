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

#include "qlocnotificationengine.h"

#include <QDebug>
#include <QLocale>

#include <qcontactrequests.h>
#include <qcontactfilters.h>
#include <qcontact.h>
#include <qcontactname.h>
#include <qcontactemailaddress.h>
#include <qcontactphonenumber.h>
#include <qcontacturl.h>
#include <qcontactmanager.h>
#include <qcontactdisplaylabel.h>

#include <hbnumbergrouping.h>



//---------------------------------------------------------------------
// QLocNotificationEngine::QLocNotificationEngine()
// (other items were commented in a header).
//---------------------------------------------------------------------
QLocNotificationEngine::QLocNotificationEngine()
    {
    qDebug()
            << "+QLocNotificationEngine::QLocNotificationEngine()";
    //"symbian" is the string used to load the contact manager corresponding to the phonebook database
    mContactManager = new QtMobility::QContactManager("symbian");
   qDebug()
            << "-QLocNotificationEngine::QLocNotificationEngine()";	
    }

//------QLocNotificationEngine-----------------------------------------
// QLocNotificationEngine::~QLocNotificationEngine()
// (other items were commented in a header).
//---------------------------------------------------------------------
QLocNotificationEngine::~QLocNotificationEngine()
    {
    delete mContactManager;
    }

//------QLocNotificationEngine-----------------------------------------
// QLocNotificationEngine::processRequestors
// Used to resolve requestors
// (other items were commented in a header).
//---------------------------------------------------------------------
void QLocNotificationEngine::processRequestors(QPosRequestorData& posRequestorData)
    {        
    qDebug()
            << "+QLocNotificationEngine::processRequestors()"; 
    QtMobility::QContactDetailFilter filter;

    QString idString;
    QPosRequestorData::IdFormat idFormat =  QPosRequestorData::EIdFormatUnknown;
    posRequestorData.getPosRequestorData(idFormat,idString);
    
    switch(idFormat) // set definition name based on the idformat to be searched for
        {
        case QPosRequestorData::EIdFormatPhoneNumber:      
            {
            filter.setDetailDefinitionName(QtMobility::QContactPhoneNumber::DefinitionName, QtMobility::QContactPhoneNumber::FieldNumber);            
            }
            break;
        
        case QPosRequestorData::EIdFormatEmail:
            {
            filter.setDetailDefinitionName(QtMobility::QContactEmailAddress::DefinitionName, QtMobility::QContactEmailAddress::FieldEmailAddress);
            }
            break;
        case QPosRequestorData::EIdFormatUrl:
            {
            filter.setDetailDefinitionName(QtMobility::QContactUrl::DefinitionName,QtMobility::QContactUrl::ContextHome);
            }
            break;
        
        //other cases are not supported by the phonebook
        default: //this case wont arise as other values will already have been filtered out 
            
            break;
        }
    filter.setMatchFlags(QtMobility::QContactFilter::MatchExactly);
    filter.setValue(idString);
    
    QList<QtMobility::QContactLocalId> matchingContacts = mContactManager->contactIds(filter);
    
    if (matchingContacts.size() > 0) //if any contacts were found
        {
               
        QtMobility::QContact match = mContactManager->contact(matchingContacts.at(0)); //return the first contact among the ones found
          
        QString labelDetail = match.displayLabel();
        posRequestorData.setPosRequestorData(idFormat,labelDetail); //set back result
        }
    else
    	{
    	// if no contacts were found
    	
    	if( QPosRequestorData::EIdFormatPhoneNumber == idFormat )
    		{
    		// Group the phone number according to current system locale
    		/*
    	    QString isStringPh = HbNumberGrouping::formatPhoneNumber(idString,QLocale::system().country());
            posRequestorData.setPosRequestorData(idFormat,isStringPh); //set back result
    		*/
            posRequestorData.setPosRequestorData(idFormat,idString); //set back result
    		}
    	} 
    
    qDebug()
            << "-QLocNotificationEngine::processRequestors()"; 
    }

//---------------------------------------------------------------------
// QPosRequestorData::QPosRequestorData()
// Constructor for QPosRequestorData
// (other items were commented in a header).
//---------------------------------------------------------------------
QPosRequestorData::QPosRequestorData()
    {
    
    }
   
//---------------------------------------------------------------------
// QPosRequestorData::~QPosRequestorData()
// Destructor for QPosRequestorData
// (other items were commented in a header).
//---------------------------------------------------------------------
QPosRequestorData::~QPosRequestorData()
    {
    
    }

//---------------------------------------------------------------------
// QPosRequestorData::setPosRequestorData()
// Settermethod for QPosRequestorData
// (other items were commented in a header).
//---------------------------------------------------------------------
void QPosRequestorData::setPosRequestorData(const QPosRequestorData::IdFormat& idFormat, const QString& idString)
    {
    mIdFormat = idFormat;
    mIdString = idString;
    }
    
//---------------------------------------------------------------------
// QPosRequestorData::getPosRequestorData()
// Gettermethod for QPosRequestorData
// (other items were commented in a header).
//---------------------------------------------------------------------
void QPosRequestorData::getPosRequestorData(QPosRequestorData::IdFormat& idFormat, QString& idString) const
    {
    idFormat = mIdFormat;
    idString = mIdString;
    }
    
// End of file.
