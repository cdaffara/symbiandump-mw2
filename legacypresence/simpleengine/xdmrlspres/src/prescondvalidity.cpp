/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:    RLS and Presence XDM, This class represents validity part of 
*                 a Presence Condition
*
*/




// Includes
#include <XdmDocumentNode.h>
#include <XdmNodeAttribute.h>
#include <xcapappusagedef.h>
#include <utf.h>

#include "prescondvalidity.h"
#include "cpresencexdm.h"
#include "rlspresxdmlogger.h"
#include "rlspresxdmconstsint.h"


// ---------------------------------------------------------------------------
// TPresCondValidity::TPresCondValidity()
// ---------------------------------------------------------------------------
//	
EXPORT_C TPresCondValidity::TPresCondValidity(): iPresXDM(NULL)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondValidity::TPresCondValidity()" ));
    }

// ---------------------------------------------------------------------------
// TPresCondValidity::Init()
// ---------------------------------------------------------------------------
//    
EXPORT_C void TPresCondValidity::Init(CPresenceXDM* const aPresXDM)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondValidity::Init(%d)" ), aPresXDM);
    iPresXDM = aPresXDM;
    }


// ---------------------------------------------------------------------------
// TPresCondValidity::GetValidity()
// ---------------------------------------------------------------------------
//	
EXPORT_C TInt TPresCondValidity::GetValidityL(const TDesC& aRuleID, 
                                        RArray<TTimePeriod>& aValidTimes)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondValidity::GetValidity()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S"),&aRuleID);

	CXdmDocumentNode* validityNode = GetValidityNodeL(aRuleID, EFalse);
	if(validityNode==NULL)
	    return KErrNotFound;
	
	TInt nodeCount = validityNode->NodeCount();
    OPENG_DP(D_OPENG_LIT( "     validityNode->NodeCount() = %d"),nodeCount);
	if(!nodeCount)
	    return KErrNotFound;
	
	RPointerArray<CXdmDocumentNode> fromNodes;
	RPointerArray<CXdmDocumentNode> untilNodes;
	
	HBufC* fromPtr(NULL);
	HBufC* untilPtr(NULL); 
	
	TTime fromTime(0);
	TTime untilTime(0);
	
	
	TTimePeriod myTimePeriod;
	
    // Get all the <from> nodes
  	validityNode->Find(KXdmFrom, fromNodes);
    validityNode->Find(KXdmUntil, untilNodes);
    
    if((fromNodes.Count()) != (untilNodes.Count()))
        {
        fromNodes.Close();
        untilNodes.Close();
        return KErrGeneral;
        }
        
    
    nodeCount = fromNodes.Count();
    OPENG_DP(D_OPENG_LIT( "     nodesCount = %d" ),nodeCount);

	for(TInt i=0; i<nodeCount; i++)
	    {
        aValidTimes.Append(myTimePeriod);

	    // processing from
	    (fromNodes[i])->SetLeafNode(ETrue);
	    fromPtr = CnvUtfConverter::ConvertToUnicodeFromUtf8L((fromNodes[i])->LeafNodeContent());
	    CleanupStack::PushL(fromPtr);
	    OPENG_DP(D_OPENG_LIT( "     from string at i(%d): %S" ),i,&(*fromPtr));
	    ParseFromXMLTimeString(fromPtr->Des(), fromTime);
	    CleanupStack::PopAndDestroy(fromPtr);
	    ConvertToLocal(fromTime);
	    ((aValidTimes[i]).from).Set(fromTime.DateTime().Year(),fromTime.DateTime().Month(),
	        fromTime.DateTime().Day(),fromTime.DateTime().Hour(),fromTime.DateTime().Minute(),
	        fromTime.DateTime().Second(), fromTime.DateTime().MicroSecond());
	    OPENG_DP(D_OPENG_LIT( "     from at i = %d" ),i);
	    LogDateTime((aValidTimes[i]).from);
	    
	    
	    // processing until
	    (untilNodes[i])->SetLeafNode(ETrue);
	    untilPtr = CnvUtfConverter::ConvertToUnicodeFromUtf8L((untilNodes[i])->LeafNodeContent());
	    CleanupStack::PushL(untilPtr);
	    OPENG_DP(D_OPENG_LIT( "     until string at i(%d): %S" ),i,&(*untilPtr));
	    ParseFromXMLTimeString(untilPtr->Des(), untilTime);
	    CleanupStack::PopAndDestroy(untilPtr);
	    ConvertToLocal(untilTime);
	    (aValidTimes[i]).until.Set(untilTime.DateTime().Year(),untilTime.DateTime().Month(),
	        untilTime.DateTime().Day(),untilTime.DateTime().Hour(),untilTime.DateTime().Minute(),
	        untilTime.DateTime().Second(),untilTime.DateTime().MicroSecond());
	    OPENG_DP(D_OPENG_LIT( "     until at i = %d" ),i);
	    LogDateTime((aValidTimes[i]).until);

	    }
	    
    fromNodes.Close();
    untilNodes.Close();
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// TPresCondValidity::DeleteAllValidity()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondValidity::DeleteAllValidityL(const TDesC& aRuleID)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondValidity::DeleteAllValidity()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S"),&aRuleID);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));
    
	CXdmDocumentNode* validityNode = GetValidityNodeL(aRuleID, EFalse);
	if(validityNode==NULL)
	    return KErrNotFound;
	
	iPresXDM->RemoveFromModelL(validityNode);
	
	OPENG_DP(D_OPENG_LIT( "     removed"));
	return KErrNone;
	}

// ---------------------------------------------------------------------------
// TPresCondValidity::AddValidityL()
// ---------------------------------------------------------------------------
//
EXPORT_C TInt TPresCondValidity::AddValidityL(const TDesC& aRuleID, 
                                                const TTimePeriod aValidTime)
	{
    OPENG_DP(D_OPENG_LIT( "TPresCondValidity::AddValidityL()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S"),&aRuleID);
    OPENG_DP(D_OPENG_LIT( "     from:"));
    LogDateTime(aValidTime.from);
    OPENG_DP(D_OPENG_LIT( "     until:"));
    LogDateTime(aValidTime.until);

	CXdmDocumentNode* validityNode = GetValidityNodeL(aRuleID, ETrue);
	if(validityNode==NULL)
	    return KErrNotFound;
	
	TBuf<KPresDateTimeBufLength> fromString;
	TBuf<KPresDateTimeBufLength> untilString;
	
	
	TTime fromTTime(aValidTime.from);
	TTime untilTTime(aValidTime.until);
	
	// convert to universal times
	ConvertToUniversal(fromTTime);
	ConvertToUniversal(untilTTime);
	
	// get the xml strings
	FormatToXMLTimeStringL(fromString, fromTTime);
    FormatToXMLTimeStringL(untilString, untilTTime);
    
    OPENG_DP(D_OPENG_LIT( "     from: %S"), &fromString);
    OPENG_DP(D_OPENG_LIT( "     until: %S"), &untilString);
    
    // make the nodes
	CXdmDocumentNode* fromNode = validityNode->CreateChileNodeL(KXdmFrom);
	CXdmDocumentNode* untilNode = validityNode->CreateChileNodeL(KXdmUntil);
	
	// make the leaf and put contents
	fromNode->SetLeafNode(ETrue);
	fromNode->SetLeafNodeContentL(fromString);
	
	untilNode->SetLeafNode(ETrue);
	untilNode->SetLeafNodeContentL(untilString);
	
	return KErrNone;		
	}

// ---------------------------------------------------------------------------
// TPresCondValidity::IsTimeValidL()
// ---------------------------------------------------------------------------
//	
EXPORT_C TBool TPresCondValidity::IsTimeValidL(const TDesC& aRuleID, 
                                                const TTimePeriod aTime)
    {
    OPENG_DP(D_OPENG_LIT( "TPresCondValidity::IsTimeValid()" ) );
    OPENG_DP(D_OPENG_LIT( "     aRuleID = %S"),&aRuleID);
    OPENG_DP(D_OPENG_LIT( "     from:"));
    LogDateTime(aTime.from);
    OPENG_DP(D_OPENG_LIT( "     until:"));
    LogDateTime(aTime.until);
    
    
	CXdmDocumentNode* validityNode = GetValidityNodeL(aRuleID, EFalse);
	if (validityNode==NULL)
	    return ETrue;   // because all times are valid if no validity node
	    
	TBool result(EFalse);
	    
	RArray<TTimePeriod> validTimes;
	GetValidityL(aRuleID, validTimes);
	
	TTime givenfromTime(aTime.from);
	TTime givenuntilTime(aTime.until);
	
	TInt validTimeCounts = validTimes.Count();
	for(TInt i=0; i<validTimeCounts; i++)
	    {
	    TTime fromTime((validTimes[i]).from);
	    TTime untilTime((validTimes[i]).until);
	    
	    if( (givenfromTime>=fromTime) && (givenuntilTime<=untilTime) )
	        {
            result = ETrue;
	        break;
	        }
	    }
	    
	validTimes.Close();
	OPENG_DP(D_OPENG_LIT( "     result = %d"),result);
	return result;   
    }

// ---------------------------------------------------------------------------
// TPresCondValidity::GetValidityNode()
// ---------------------------------------------------------------------------
//    
CXdmDocumentNode* TPresCondValidity::GetValidityNodeL(const TDesC& aRuleID, 
                                                        TBool aCreate)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondValidity::GetValidityNodeL()" ) );
    OPENG_DP(D_OPENG_LIT( "     GetValidityNodeL aRuleID = %S, aCreate = %d"),
                                                            &aRuleID, aCreate);
    
    __ASSERT_ALWAYS(iPresXDM, User::Leave(KErrNotReady));                                                        
    
    CXdmDocumentNode* conditionNode = iPresXDM->GetRuleChildNodeL(aRuleID, 
                                                    KXdmConditions, aCreate);
    if (conditionNode==NULL)
        return conditionNode;

    RPointerArray<CXdmDocumentNode> nodes;
    CXdmDocumentNode* validityNode(NULL);

    // Finding sphere node
	conditionNode->Find(KXdmValidity, nodes);
    OPENG_DP(D_OPENG_LIT( "     GetValidityNodeL nodeCount = %d"),nodes.Count());
	if(nodes.Count()) // if identity node exist
	    {
	    validityNode = nodes[0]; // only one sphere node can exist
	    }
	else if (aCreate) // if asked to create
	    {
	    validityNode = conditionNode->CreateChileNodeL(KXdmValidity);
	    }

    nodes.Close();        
    return validityNode;   
    }


// ---------------------------------------------------------------------------
// TPresCondValidity::ParseFromXMLTimeString()
// ---------------------------------------------------------------------------
//    
TInt TPresCondValidity::ParseFromXMLTimeString(const TDesC& aXMLTimeString, 
                                                         TTime& aUTCDateTime)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondValidity::ParseFromXMLTimeString()" ) );
    OPENG_DP(D_OPENG_LIT( "     ParseFromXMLTimeString aXMLTimeString = %S"), 
                                                                &aXMLTimeString);

    TInt err(KErrNone);
    
    // Initializing the locale
    TLocale myLocale, systemLocale;
    myLocale.Refresh();
    systemLocale.Refresh();
    myLocale.SetDateFormat(EDateJapanese);
    myLocale.SetDateSeparator('-',1);
    myLocale.SetDateSeparator('-',2);
    myLocale.SetDateSeparator('T',3);
    
    myLocale.SetTimeFormat(ETime24);
    myLocale.SetTimeSeparator(':',1);
    myLocale.SetTimeSeparator(':',2);
    myLocale.SetTimeSeparator(' ',3);
    myLocale.Set();

    TTime myTime;
    TTime myUTCtime;
    TChar uTCtimeSign(aXMLTimeString[23]); // 23 is ascii T
    
   
    // parsing main time
    TBuf<KPresDateTimeBufLength> dateTimeBuffer(aXMLTimeString);
    dateTimeBuffer.Delete(KPresDTWithoutUTCLength-1,6);
    dateTimeBuffer.Replace(KPresDTSeperatorPos,1,KPresSpaceString);
    myTime.Parse(dateTimeBuffer);
    
    // parsing utc time
    TBuf<KPresDateTimeBufLength> uTCbuffer(aXMLTimeString);
    uTCbuffer.Delete(0,KPresDTWithoutUTCLength);
    myUTCtime.Parse(uTCbuffer);
    TDateTime uTCDateTime = myUTCtime.DateTime();
    
    // adding or substracting utc from main time
    if(uTCtimeSign=='+') // if diff is positive
        {
        aUTCDateTime = myTime - TTimeIntervalHours(uTCDateTime.Hour());
        aUTCDateTime = aUTCDateTime - TTimeIntervalMinutes(uTCDateTime.Minute());            
        }
    else if(uTCtimeSign=='-')
        {
        aUTCDateTime = myTime + TTimeIntervalHours(uTCDateTime.Hour());
        aUTCDateTime = aUTCDateTime + TTimeIntervalMinutes(uTCDateTime.Minute());            
        }
    else
        err = KErrArgument;
    
    // putting the system locale back
    systemLocale.Set();
    return err;    
    }
                                                            
// ---------------------------------------------------------------------------
// TPresCondValidity::FormatToXMLTimeStringL()
// ---------------------------------------------------------------------------
//
TInt TPresCondValidity::FormatToXMLTimeStringL(TDes& aXMLTimeString, 
                                                    const TTime aUTCDateTime)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondValidity::FormatToXMLTimeString()" ) );
    OPENG_DP(D_OPENG_LIT( "     FormatToXMLTimeString aUTCDateTime:"));
    LogDateTime(aUTCDateTime.DateTime());

    // Initializing the locale
    TLocale myLocale, systemLocale;
    myLocale.Refresh();
    systemLocale.Refresh();
    myLocale.SetDateFormat(EDateJapanese);
    myLocale.SetDateSeparator('-',1);
    myLocale.SetDateSeparator('-',2);
    myLocale.SetDateSeparator('T',3);
    
    myLocale.SetTimeFormat(ETime24);
    myLocale.SetTimeSeparator(':',1);
    myLocale.SetTimeSeparator(':',2);
    myLocale.SetTimeSeparator(' ',3);
    myLocale.Set();
    
    // getting UTC difference    
    TTimeIntervalSeconds uTCseconds = systemLocale.UniversalTimeOffset();
    
    // processing main time and date component
    TTime mainTTime = aUTCDateTime + uTCseconds;            
    mainTTime.FormatL(aXMLTimeString, KPresDateTimeFormatString);
    
    // Processing for time difference
    TChar uTCtimeSign('+');
    TDateTime myUTCtime(0,EJanuary,0,0,0,0,0);
    TTime uTCTTime(myUTCtime);
    if(uTCseconds.Int()<0)
        {
        uTCtimeSign = '-';
        uTCseconds = (uTCseconds.Int())*(-1);        
        }
    uTCTTime = uTCTTime + uTCseconds;
    TBuf<KPresDateTimeBufLength> dateTimeUTCBuffer;
    uTCTTime.FormatL(dateTimeUTCBuffer, KPresUTCFormatString);


    // Appending the time difference to main string
    aXMLTimeString.Append(dateTimeUTCBuffer);
    
    // put time difference sign to main string
    aXMLTimeString[23] = uTCtimeSign;
    
    // putting the system locale back
    systemLocale.Set();
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// TPresCondValidity::ConvertToUniversal()
// ---------------------------------------------------------------------------
//    
void TPresCondValidity::ConvertToUniversal(TTime& aDateTime)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondValidity::ConvertToUniversal()" ) );
    OPENG_DP(D_OPENG_LIT( "     ConvertToUniversal aDateTime:"));
    LogDateTime(aDateTime.DateTime());

    TLocale myLocale;
    myLocale.Refresh();
    
    // getting UTC difference    
    TTimeIntervalSeconds uTCseconds = myLocale.UniversalTimeOffset();
    aDateTime = aDateTime - uTCseconds;
    }
        
// ---------------------------------------------------------------------------
// TPresCondValidity::ConvertToLocal()
// ---------------------------------------------------------------------------
//        
void TPresCondValidity::ConvertToLocal(TTime& aDateTime)
    {
    OPENG_DP(D_OPENG_LIT( " TPresCondValidity::ConvertToLocal()" ) );
    OPENG_DP(D_OPENG_LIT( "     ConvertToLocal aDateTime:"));
    LogDateTime(aDateTime.DateTime());

    TLocale myLocale;
    myLocale.Refresh();
    
    // getting UTC difference    
    TTimeIntervalSeconds uTCseconds = myLocale.UniversalTimeOffset();
    aDateTime = aDateTime + uTCseconds;
    }

// ---------------------------------------------------------------------------
// TPresCondValidity::LogDateTime()
// ---------------------------------------------------------------------------
//
void TPresCondValidity::LogDateTime(TDateTime aDateTime)
    {
    OPENG_DP(D_OPENG_LIT( "         %d, %d, %d, %d, %d, %d, %d"),
                    aDateTime.Year(), aDateTime.Month()+1, aDateTime.Day()+1,
                    aDateTime.Hour(), aDateTime.Minute(), aDateTime.Second(),
                                                    aDateTime.MicroSecond() );
    }

// end of file

