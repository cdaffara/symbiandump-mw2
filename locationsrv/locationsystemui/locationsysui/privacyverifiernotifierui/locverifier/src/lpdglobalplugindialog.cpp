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
* Description:  List query dialog suitable for verifier plug-in use.
*
*/


// INCLUDE FILES
#include "lpdglobalplugindialog.h"
#include "lpddlgobserver.h"
#include "lpdrequestorprocessor.h"
#include "locverifierdlgdebug.h"

#include <lbs/EPos_CPosContactRequestor.h>
#include <lbs/EPos_CPosServiceRequestor.h>
#include <lbs/EPos_CPosRequestor.h>
#include <hbsymbianvariant.h>
#include <hbtextresolversymbian.h>
#include <systemtoneservice.h>
#include <e32des16.h>

_LIT(KNotifReqSeperator,",");
_LIT(KHeadingKey, "heading");
_LIT(KQueryTextKey, "queryText");
_LIT(KQueryText2Key, "queryText2");
_LIT(KRequestorKey, "requestor");
_LIT(KVerificationDlgType, "hb.posverifiction.dialog/1.0");
_LIT(KLocTsFileName, "lilocationmw_");
_LIT(KLocTsFilePath, "z://resource/qt/translations/");
_LIT(KLocTsAFileAltPath, "c://resource/");
//Translation String Ids
//Verification Dialog
_LIT(KRequestHeading, "txt_loe_title_location_request");
_LIT(KSingleshotDecisionRejected, "txt_loe_info_your_location_will_not_be_shared_by_d");
_LIT(KSingleshotDecisionAccepted, "txt_loe_info_your_location_will_be_shared_by_defau");
_LIT(KSingleshotDecisionNotAvailable, "txt_loe_info_your_location_will_be_shared_with");
_LIT(KPeriodicDecisionRejected, "txt_loe_info_your_location_will_not_be_shared_peri");
_LIT(KPeriodicDecisionAccepted, "txt_loe_info_your_location_will_be_shared_periodic");
_LIT(KPeriodicDecisionNotAvailable, "txt_loe_info_you_location_will_be_shared_periodica");
_LIT(KRequestorText, "txt_loe_info_location_was_requested_by");
_LIT(KUnknownRequestor, "txt_loe_info_unknown_requestor");
//Notification
_LIT(KNotificationNotSent, "txt_loe_dpophead_your_location_not_sent_to");
_LIT(KNotificationSent, "txt_loe_dpophead_your_location_sent_to");

const TInt KNotificationTimeout = 15000;
const TInt KMaxSize = 1024;
const TInt KVariantMaxSize = 256;
// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::CLpdGlobalPluginDialog
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CLpdGlobalPluginDialog::CLpdGlobalPluginDialog(                    
    MLpdDlgObserver& aCallBack
    )
    : iCallBack( aCallBack ),
      iMsgBox(NULL),
      iNotification(NULL),
      iRequestInfo(NULL)
    {
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::ConstructL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
void CLpdGlobalPluginDialog::ConstructL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::ConstructL" );
    iMsgBox = CHbDeviceDialogSymbian::NewL();
    iNotification = CHbDeviceNotificationDialogSymbian::NewL(this);
    TBool result =
            HbTextResolverSymbian::Init(KLocTsFileName, KLocTsFilePath);
    if (!result)
        {
        HbTextResolverSymbian::Init(KLocTsFileName, KLocTsAFileAltPath);
        }
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::ConstructL" );
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::NewL
// Two-phased constructor.
// -----------------------------------------------------------------------------
//
CLpdGlobalPluginDialog* CLpdGlobalPluginDialog::NewL(                    
    MLpdDlgObserver& aCallBack)
    {
    CLpdGlobalPluginDialog* self =
        new( ELeave ) CLpdGlobalPluginDialog( aCallBack);
	CleanupStack::PushL(self);
	self->ConstructL();
	CleanupStack::Pop(self);  
    return self;
    }

// Destructor
CLpdGlobalPluginDialog::~CLpdGlobalPluginDialog()
	{
   	 LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::~CLpdGlobalPluginDialog" );
   	delete iMsgBox;
	iMsgBox = NULL;
	
	delete iNotification;
	iNotification = NULL;
	
	 delete iRequestInfo;
	 iRequestInfo=NULL;
	 LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::~CLpdGlobalPluginDialog" );
	}


// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::RunLD
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdGlobalPluginDialog::RunVerificationQueryL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::RunVerificationQueryL" );
    mSymbianVariantMap = CHbSymbianVariantMap::NewL();
    CleanupStack::PushL(mSymbianVariantMap);
    
    AddVerDlgHeadingParamL();
    AddQueryTextParamL();
    AddRequestorsParamL();
    CSystemToneService* sts = CSystemToneService::Create();
    iMsgBox->Show(KVerificationDlgType, *mSymbianVariantMap, this);
    if( sts )
        {
        sts->PlayTone(CSystemToneService::ELocationRequest);
        }
    CSystemToneService::Delete(sts);
    CleanupStack::Pop(mSymbianVariantMap);
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::RunVerificationQueryL" );
    }

void CLpdGlobalPluginDialog::RunNotificationL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::RunNotificationL" );
    if( EPosDecisionByRequestSource == iRequestInfo->iNotifReason ||
        EPosVerificationTimeout == iRequestInfo->iNotifReason    )
        {
        if ( EPosDecisionAccepted == iRequestInfo->iRequestDecision )
            {
            HBufC* title = HbTextResolverSymbian::LoadL(KNotificationSent);
            CleanupStack::PushL(title);
            iNotification->SetTitleL(title->Des());
            CleanupStack::PopAndDestroy(title);
            }
        if( EPosDecisionRejected == iRequestInfo->iRequestDecision )
            {
            HBufC* title = HbTextResolverSymbian::LoadL(KNotificationNotSent);
            CleanupStack::PushL(title);
            iNotification->SetTitleL(title->Des());
            CleanupStack::PopAndDestroy(title);
            }
        }
    else
        {
        // Not supported   
        User::Leave(KErrNotSupported);
        }
    TBuf<KMaxSize> buf;
    
    TInt count= iRequestInfo->iRequestors.Count();
    if( count == 0 )
        {
        HBufC* requestor = HbTextResolverSymbian::LoadL(KUnknownRequestor);
        CleanupStack::PushL(requestor);
        buf.Append(requestor->Des());
        CleanupStack::PopAndDestroy(requestor);
        }
    else
        {
        for(TInt i=0;i< count;i++ )
            {
            if(i!=0)
                {
                buf.Append(KNotifReqSeperator);
                }
            buf.Append(iRequestInfo->iRequestors[i]->RequestorIdString());
            }
        }
    iNotification->SetTextL(buf);
    iNotification->SetTimeout(KNotificationTimeout);
    iNotification->EnableTouchActivation(ETrue);
    iNotification->ShowL();
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::RunNotificationL" );
    }

// -----------------------------------------------------------------------------
// CLpdGlobalPluginDialog::SetMessageTextL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CLpdGlobalPluginDialog::SetRequestInfoL( CLpdRequestorProcessor* aRequestInfo )
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::SetRequestInfoL" );
    if(iRequestInfo)
        {
        // clear and create again if already has been set before
        delete iRequestInfo;
        iRequestInfo=NULL;
        }
    iRequestInfo=CLpdRequestorProcessor::NewL();
    
    iRequestInfo->iDialogType=aRequestInfo->iDialogType;
    iRequestInfo->iNotifReason=aRequestInfo->iNotifReason;
    iRequestInfo->iRequestDecision=aRequestInfo->iRequestDecision;
    iRequestInfo->iRequestType=aRequestInfo->iRequestType;
    
    const RPointerArray<CPosRequestor>& requestors = aRequestInfo->iRequestors;
    
    TInt count = requestors.Count();
    for(TInt i=0; i< count; i++ )
        {
        CPosRequestor::TRequestorIdFormat idFormat;
        TPtrC idString;
        idFormat=requestors[i]->RequestorIdFormat();
        idString.Set(requestors[i]->RequestorIdString());
        
        if( CPosRequestor::ERequestorService==requestors[i]->RequestType())
            {
            CPosServiceRequestor* service = CPosServiceRequestor::NewL(idFormat,idString);
            CleanupStack::PushL(service);
            // Ownership transferred
            iRequestInfo->iRequestors.Append(service);
            CleanupStack::Pop(service);
            }
        else
            {
            CPosContactRequestor* contact = CPosContactRequestor::NewL(idFormat,idString);
            CleanupStack::PushL(contact);
            // Ownership transferred
            iRequestInfo->iRequestors.Append(contact);
            CleanupStack::Pop(contact);
            }
        
        }
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::SetRequestInfoL" );
    }



void CLpdGlobalPluginDialog::Cancel()
    {
    // Close any running dialogs if any.Close() doesnt have any effect if dialog is not running
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::Cancel" );
    isCancelled = ETrue;
    iMsgBox->Cancel();
    iNotification->Close();
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::Cancel" );
    }

void CLpdGlobalPluginDialog::NotificationDialogActivated(
        const CHbDeviceNotificationDialogSymbian */*aDialog*/)        
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::NotificationDialogActivated" );
    // Complete with KErrNone now 
    TRAP_IGNORE(iCallBack.HandleDlgDismissedL( KErrNone ));
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::NotificationDialogActivated" );
    }

void CLpdGlobalPluginDialog::NotificationDialogClosed(
        const CHbDeviceNotificationDialogSymbian */*aDialog*/,
        TInt aCompletionCode)
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::NotificationDialogClosed" );
    TRAP_IGNORE(iCallBack.HandleDlgDismissedL( aCompletionCode ));
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::NotificationDialogClosed" );
    }

void CLpdGlobalPluginDialog::DataReceived(CHbSymbianVariantMap& /*aData*/)
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::DataReceived" );
    mDissmissed = ETrue;
    TRAP_IGNORE(iCallBack.HandleDlgDismissedL( KErrAccessDenied ));
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::DataReceived" );
    }

void CLpdGlobalPluginDialog::DeviceDialogClosed(TInt /*aCompletionCode*/)
    {
     LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::DeviceDialogClosed" );
    if (!mDissmissed && !isCancelled)
        {
        TRAP_IGNORE(iCallBack.HandleDlgDismissedL( KErrNone ));
        }
     LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::DeviceDialogClosed" );
    }

void CLpdGlobalPluginDialog::AddVerDlgHeadingParamL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::AddVerDlgHeadingParamL" );
    HBufC* headingText = HbTextResolverSymbian::LoadL(KRequestHeading);
    CleanupStack::PushL(headingText);
    CHbSymbianVariant* heading = CHbSymbianVariant::NewL(headingText,
            CHbSymbianVariant::EDes);
    CleanupStack::PushL(heading);
    mSymbianVariantMap->Add(KHeadingKey, heading );
    CleanupStack::Pop(heading);
    CleanupStack::PopAndDestroy(headingText);
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::AddVerDlgHeadingParamL" );
    }

void CLpdGlobalPluginDialog::AddQueryTextParamL()
    {
     LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::AddQueryTextParamL" );
    HBufC * value = HBufC::NewL(KVariantMaxSize);
    CleanupStack::PushL(value);
    HBufC * value2 = HBufC::NewL(KVariantMaxSize);
    CleanupStack::PushL(value2);
    HBufC* requestorText = HbTextResolverSymbian::LoadL(KRequestorText);
    CleanupStack::PushL(requestorText);

    if (CPosRequestor::ERequestSingleShot == iRequestInfo->iRequestType)
        {
        if (EPosDecisionNotAvailable == iRequestInfo->iRequestDecision)
            {
            HBufC* queryText = HbTextResolverSymbian::LoadL(
                    KSingleshotDecisionNotAvailable);
            CleanupStack::PushL(queryText);
            value->Des().Copy(queryText->Des());
            CleanupStack::PopAndDestroy(queryText);
            }
        else if (EPosDecisionAccepted == iRequestInfo->iRequestDecision)
            {
            HBufC* queryText = HbTextResolverSymbian::LoadL(
                    KSingleshotDecisionAccepted);
            CleanupStack::PushL(queryText);
            value->Des().Copy(queryText->Des());
            value2->Des().Copy(requestorText->Des());
            CleanupStack::PopAndDestroy(queryText);
            }
        else if (EPosDecisionRejected == iRequestInfo->iRequestDecision)
            {
            HBufC* queryText = HbTextResolverSymbian::LoadL(
                    KSingleshotDecisionRejected);
            CleanupStack::PushL(queryText);
            value->Des().Copy(queryText->Des());
            value2->Des().Copy(requestorText->Des());
            CleanupStack::PopAndDestroy(queryText);
            }
        }

    else if (CPosRequestor::ERequestPeriodic == iRequestInfo->iRequestType)
        {
        if (EPosDecisionNotAvailable == iRequestInfo->iRequestDecision)
            {
            HBufC* queryText = HbTextResolverSymbian::LoadL(
                    KPeriodicDecisionNotAvailable);
            CleanupStack::PushL(queryText);
            value->Des().Copy(queryText->Des());
            CleanupStack::PopAndDestroy(queryText);
            }
        else if (EPosDecisionAccepted == iRequestInfo->iRequestDecision)
            {
            HBufC* queryText = HbTextResolverSymbian::LoadL(
                    KPeriodicDecisionAccepted);
            CleanupStack::PushL(queryText);
            value->Des().Copy(queryText->Des());
            value2->Des().Copy(requestorText->Des());
            CleanupStack::PopAndDestroy(queryText);
            }
        else if (EPosDecisionRejected == iRequestInfo->iRequestDecision)
            {
            HBufC* queryText = HbTextResolverSymbian::LoadL(
                    KPeriodicDecisionRejected);
            CleanupStack::PushL(queryText);
            value->Des().Copy(queryText->Des());
            value2->Des().Copy(requestorText->Des());
            CleanupStack::PopAndDestroy(queryText);
            }
        }
    else
        {
        // Not supported   
        User::Leave(KErrNotSupported);
        }

    CHbSymbianVariant* queryText = CHbSymbianVariant::NewL(value,
            CHbSymbianVariant::EDes);
    CleanupStack::PushL(queryText);

    CHbSymbianVariant* queryText2 = CHbSymbianVariant::NewL(value2,
            CHbSymbianVariant::EDes);
    CleanupStack::PushL(queryText2);

    if (value2->Length()!= 0)
        {
        mSymbianVariantMap->Add(KQueryText2Key, queryText2);
        }
    mSymbianVariantMap->Add(KQueryTextKey, queryText);
    CleanupStack::Pop(queryText2);
    CleanupStack::Pop(queryText);
    CleanupStack::PopAndDestroy(3);
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::AddQueryTextParamL" );
    }

void CLpdGlobalPluginDialog::AddRequestorsParamL()
    {
    LOCVERIFIERDLGDEBUG( "+CLpdGlobalPluginDialog::AddRequestorsParamL" );
    CDesC16ArrayFlat* array = new CDesC16ArrayFlat(8);
    TInt reqCount = iRequestInfo->iRequestors.Count();
    if (!reqCount)
        {
        HBufC* requestor = HbTextResolverSymbian::LoadL(KUnknownRequestor);
        CleanupStack::PushL(requestor);
        array->AppendL(requestor->Des());
        CleanupStack::PopAndDestroy(requestor);
        }
    else
    	{
    	for (TInt i = 0; i < reqCount; ++i)
	        {
	        array->AppendL(iRequestInfo->iRequestors[i]->RequestorIdString());
	        }
	    }

    TInt count = array->MdcaCount();
    MDesCArray* marray = static_cast<MDesCArray*> (array);
    CHbSymbianVariant* requestors = CHbSymbianVariant::NewL(marray,
            CHbSymbianVariant::EDesArray);
    mSymbianVariantMap->Add(KRequestorKey, requestors);
    LOCVERIFIERDLGDEBUG( "-CLpdGlobalPluginDialog::AddRequestorsParamL" );
    }

// End of file

