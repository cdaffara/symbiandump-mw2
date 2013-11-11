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
* Description:  Implementation of location notification engine class. 
*
*/


// INCLUDE FILES


#include "locrequestorutilsresolver.h"
#include "qlocnotificationengine.h"

#include <e32base.h>
#include <barsread.h>


#include <lbs/epos_cposrequestor.h>
#include <lbs/epos_cposservicerequestor.h>
#include <lbs/epos_cposcontactrequestor.h>




// ============= CLocRequestorUtilsResolver MEMBER FUNCTIONS =================

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::NewL
// 2 Phase Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CLocRequestorUtilsResolver* CLocRequestorUtilsResolver::NewL()
	{
	CLocRequestorUtilsResolver* self = 
							new(ELeave) CLocRequestorUtilsResolver();
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);
    return self;
	}

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CLocRequestorUtilsResolver::~CLocRequestorUtilsResolver()
	{
	delete iEngine;
	iEngine = NULL;
	}

// ---------------------------------------------------------------------------
// The method resolves requestors by delegating requests to QPosRequestorData
// ---------------------------------------------------------------------------
//
EXPORT_C void CLocRequestorUtilsResolver::ResolveRequestorsL( 
	RPointerArray<CPosRequestor>& aRequestors )
	{
    //resolve requestors one by one by extracting the idFormat and idString 
    //from the requestor array
    TInt count = aRequestors.Count();
    for(TInt iter = 0; iter < count; ++iter)
        {        
        CPosRequestor::TRequestorIdFormat idFormat = aRequestors[iter]->RequestorIdFormat();        
        QPosRequestorData::IdFormat qposidformat;
         switch(idFormat)
        {

        case CPosRequestor::EIdFormatGenericName:
             qposidformat=QPosRequestorData::EIdFormatGenericName;
             break;	
             
        case CPosRequestor::EIdFormatPhoneNumber:
            qposidformat=QPosRequestorData::EIdFormatPhoneNumber;
             break;
        case CPosRequestor::EIdFormatUrl:
            qposidformat=QPosRequestorData::EIdFormatUrl;
              break;
        case CPosRequestor::EIdFormatEmail:
            qposidformat=QPosRequestorData::EIdFormatEmail;
             break;
        case CPosRequestor::EIdFormatSIPUrl:
            qposidformat=QPosRequestorData::EIdFormatSIPUrl;
             break;
        case CPosRequestor::EIdFormatIMSPublicIdentity:
            qposidformat=QPosRequestorData::EIdFormatIMSPublicIdentity;
             break;
        case CPosRequestor::EIdFormatMIN:
            qposidformat=QPosRequestorData::EIdFormatMIN;
             break;
        case CPosRequestor::EIdFormatMDN:
            qposidformat=QPosRequestorData::EIdFormatMDN;
             break;
        case  CPosRequestor::EIdFormatUnknown:
        default:
            qposidformat=QPosRequestorData::EIdFormatUnknown; 
             break;
            }    
        //right now the phonebook has fields corresponding to  email,phone number, URL only
        //other cases are not supported by the phonebook
        if(qposidformat == QPosRequestorData::EIdFormatEmail ||
                qposidformat == QPosRequestorData::EIdFormatPhoneNumber ||
                qposidformat == QPosRequestorData::EIdFormatUrl ) 
            {
            //convert TDes to QString
            QString qReqString((QChar*)aRequestors[iter]->RequestorIdString().Ptr(),aRequestors[iter]->RequestorIdString().Length());
            
            QPosRequestorData posRequestor;
            posRequestor.setPosRequestorData(qposidformat,qReqString);
            
            iEngine->processRequestors(posRequestor); 

            QString retString;        
            posRequestor.getPosRequestorData(qposidformat,retString);
            
            //convert QString to TDes
            TPtrC16 textPtr(reinterpret_cast<const TUint16*>(retString.utf16()));

            aRequestors[iter]->SetRequestorIdL(idFormat,textPtr);
            }
        }
	}
	

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::CLocRequestorUtilsResolver
// C++ default constructor can NOT contain any code, that
// might leave.
// ---------------------------------------------------------------------------
//
CLocRequestorUtilsResolver::CLocRequestorUtilsResolver()
	{
	// Nothing to do.
	}

// ---------------------------------------------------------------------------
// CLocRequestorUtilsResolver::ConstructL
// Symbian Second Phase COnstructor
// ---------------------------------------------------------------------------
//
void CLocRequestorUtilsResolver::ConstructL()
	{
    try
	    {
	    iEngine = new QLocNotificationEngine();
		}
	catch( int error )
		{
		// Memory alloc failure
		User::Leave(KErrNoMemory);
		}	
	}

//  End of File

