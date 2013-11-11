/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CCommonTestClass block implemetation for STIF Test Framework TestScripter.
*
*/



// INCLUDE FILES
#include <e32svr.h>
#include <StifParser.h>
#include <Stiftestinterface.h>
#include <mpxmediaarray.h>
#include <mpxcollectionpath.h>
#include "commontestclass.h"
#include "mpxpluginhandlerbase.h"//add

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CCommonTestClass::Delete
// Delete here all resources allocated and opened from test methods. 
// Called from destructor. 
// -----------------------------------------------------------------------------
//
void CCommonTestClass::Delete() 
    {
    FTRACE(FPrint(_L("CCommonTestClass::Delete")));
    
    iExpectedEvents.Close();
    iOcurredEvents.Close();

    // mpxpluginmonitor
    if ( iPluginMonitor != NULL)
        {
        delete iPluginMonitor;
        }    
    // mpxcentrpwatcher
    if ( iCenRepWatcher != NULL)
        {
        delete iCenRepWatcher;
        }    
    
    // mpxparameter
    if ( iParameter != NULL)
        {
        delete iParameter;
        }    
    
    // mpxcollectiontype
    if ( iType != NULL)
        {
        delete iType;
        }    

    // CMPXPSKeyWatcher
    if ( iPSWatcher != NULL)
        {
        delete iPSWatcher;
        }    

    // MPXDrmMediaUtility
    if ( iDrmMediaUtility != NULL)
        {
        delete iDrmMediaUtility;
        }
    
    // CMPXMessageQueue
    if ( iMessageQueue != NULL)
        {
        delete iMessageQueue;
        }
   
    // MPX Attribute array
    iMPXAttArray.Close();
    // MPXItemId array
    iMPXItemIdArray.Close();
    
    // MPXMediaArray
    if ( iMPXMediaArray )
        {
        iMPXMediaArray->Reset();
        delete iMPXMediaArray;
        }
    // MPXCollectionPath
    if ( iMPXCollectionPath )
        {
        iMPXCollectionPath->Reset();
        delete iMPXCollectionPath;
        }
    
    // MPXClientList
    if ( iClientList )
        {
        delete iClientList;
        }

    // CMPXMessageMonitor
    if ( iMsgMonitor )
        {
        delete iMsgMonitor;
        iMessageSession.Close();
        }

    // CMPXViewPlugin
    if ( iViewPlugin )
        {
        delete iViewPlugin;
        REComSession::FinalClose();
        }

    iLog->Log(_L("Deleting test class..."));
	iLog->Log(_L(""));
	iLog->Log(_L(""));
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CCommonTestClass::RunMethodL( CStifItemParser& aItem ) 
    {
    TStifFunctionInfo const KFunctions[] =
        {  
        // Copy this line for every implemented function.
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        ENTRY( "CreatePluginMonitorL", CCommonTestClass::CreatePluginMonitorL ),
        ENTRY( "DeletePluginMonitorL", CCommonTestClass::DeletePluginMonitorL ),
        // test cases for the header "testcenrepwatcher.h"
        ENTRY( "CreateCenRepWatcherL", CCommonTestClass::CreateCenRepWatcherL ),
        ENTRY( "DeleteCenRepWatcherL", CCommonTestClass::DeleteCenRepWatcherL ),
        ENTRY( "CenRepCurrentValueL", CCommonTestClass::CenRepCurrentValueL ),
        ENTRY( "CenRepSetValueL", CCommonTestClass::CenRepSetValueL ),
        // test cases for the header "testmpxparameter.h"
        ENTRY( "CreateMPXParameter", CCommonTestClass::CreateMPXParameter ),
        ENTRY( "DeleteMPXParameter", CCommonTestClass::DeleteMPXParameter ),
        ENTRY( "InternalizeMPXParameterL", CCommonTestClass::InternalizeMPXParameterL ),
        ENTRY( "ExternalizeMPXParameterL", CCommonTestClass::ExternalizeMPXParameterL ),
        // test cases for the header "testmpxcollectiontype.h"
        ENTRY( "CreateMPXCollectionTypeWOParam", CCommonTestClass::CreateMPXCollectionTypeWOParam ),
        ENTRY( "CreateMPXCollectionType", CCommonTestClass::CreateMPXCollectionType ),
        ENTRY( "InternalizeMPXCollectionTypeL", CCommonTestClass::InternalizeMPXCollectionTypeL ),
        ENTRY( "ExternalizeMPXCollectionTypeL", CCommonTestClass::ExternalizeMPXCollectionTypeL ),
        // test cases for the header "testmpxpskeywatcher.h"
        ENTRY( "CreateCMPXPSKeyWatcherL", CCommonTestClass::CreateCMPXPSKeyWatcherL ),
        ENTRY( "DeleteCMPXPSKeyWatcherL", CCommonTestClass::DeleteCMPXPSKeyWatcherL ),
        ENTRY( "GetTntValue", CCommonTestClass::GetTntValue ),
        ENTRY( "GetDes8Value", CCommonTestClass::GetDes8Value ),
        ENTRY( "GetDes16Value", CCommonTestClass::GetDes16Value ),
        ENTRY( "SetTntValue", CCommonTestClass::SetTntValue ),
        ENTRY( "SetDes8Value", CCommonTestClass::SetDes8Value ),
        ENTRY( "SetDes16Value", CCommonTestClass::SetDes16Value ),
        // test cases for the header "testmpxdrmmediautility.h" 
        ENTRY( "CreateCMPXDrmMediaUtilityL", CCommonTestClass::CreateCMPXDrmMediaUtilityL ),
        ENTRY( "CreateCMPXDrmMediaUtilityLC", CCommonTestClass::CreateCMPXDrmMediaUtilityLC ),
        ENTRY( "DeleteCMPXDrmMediaUtilityL", CCommonTestClass::DeleteCMPXDrmMediaUtilityL ),
        ENTRY( "InitFileNameL", CCommonTestClass::InitFileNameL ),
        ENTRY( "InitRFileL", CCommonTestClass::InitRFileL ),
        ENTRY( "Close", CCommonTestClass::Close ),
        ENTRY( "GetMediaL", CCommonTestClass::GetMediaL ),
        ENTRY( "ConsumeL", CCommonTestClass::ConsumeL ),
        // test cases for the header "testmpxmessage.h" 
        ENTRY( "TMPXMessage3L", CCommonTestClass::TMPXMessage3L ),
        ENTRY( "TMPXMessageL", CCommonTestClass::TMPXMessageL ),
        ENTRY( "TMPXMessage1L", CCommonTestClass::TMPXMessage1L ),
        ENTRY( "EqualTMPXMessage", CCommonTestClass::EqualTMPXMessage ),
        ENTRY( "Event", CCommonTestClass::Event ),
        ENTRY( "Type", CCommonTestClass::Type ),
        ENTRY( "Data", CCommonTestClass::Data ),
        // test cases for the header "testmpxmessagequeue.h" 
        ENTRY( "CreateCMPXMessageQueueL", CCommonTestClass::CreateCMPXMessageQueueL ),
        ENTRY( "ResetCMPXMessageQueue", CCommonTestClass::ResetCMPXMessageQueue ),
        ENTRY( "SendNextCMPXMessageQueue", CCommonTestClass::SendNextCMPXMessageQueue ),
        ENTRY( "AddCMPXMessageQueue", CCommonTestClass::AddCMPXMessageQueue ),
        ENTRY( "DeleteCMPXMessageQueue", CCommonTestClass::DeleteCMPXMessageQueue ),
        // test cases for the header "testmpxsession.h" 
        
        ENTRY( "CreateRMPXSession", CCommonTestClass::CreateRMPXSession ),
        ENTRY( "CloseAndDestructRMPXSession", CCommonTestClass::CloseAndDestructRMPXSession ),
        
        ENTRY( "VersionRMPXSession", CCommonTestClass::VersionRMPXSession ),
        ENTRY( "ConnectRMPXSessionL", CCommonTestClass::ConnectRMPXSessionL ),
        ENTRY( "ReconnectRMPXSessionL", CCommonTestClass::ReconnectRMPXSessionL ),
        ENTRY( "SendReceiveLRMPXSessionL", CCommonTestClass::SendReceiveLRMPXSessionL ),
        ENTRY( "SendReceiveStatusRMPXSessionL", CCommonTestClass::SendReceiveStatusRMPXSessionL ),
        ENTRY( "SendReceiveStatus3RMPXSessionL", CCommonTestClass::SendReceiveStatus3RMPXSessionL ),
        // test cases for the header "testmpxmessagemonitor.h"
        ENTRY( "CreateCMPXMessageMonitorL", CCommonTestClass::CreateCMPXMessageMonitorL ),
        ENTRY( "DeleteCMPXMessageMonitorL", CCommonTestClass::DeleteCMPXMessageMonitorL ),
        ENTRY( "RunLCMPXMessageMonitorL", CCommonTestClass::RunLCMPXMessageMonitorL ),
        // test cases for the header "testmpxactivetaskqueue.h"
        ENTRY( "CreateCMPXActiveTaskQueueL", CCommonTestClass::CreateCMPXActiveTaskQueueL ),
        ENTRY( "CancelRequestsCMPXActiveTaskQueueL", CCommonTestClass::CancelRequestsCMPXActiveTaskQueueL ),
        ENTRY( "AddTaskCMPXActiveTaskQueueL", CCommonTestClass::AddTaskCMPXActiveTaskQueueL ),
        ENTRY( "CompleteTaskCMPXActiveTaskQueueL", CCommonTestClass::CompleteTaskCMPXActiveTaskQueueL ),
        
        //added on 2009,2,11
        ENTRY( "CompleteAllTasksCMPXActiveTaskQueueL", CCommonTestClass::CompleteAllTasksCMPXActiveTaskQueueL ),
        
        ENTRY( "IsEmptyCMPXTaskQueueL", CCommonTestClass::IsEmptyCMPXTaskQueueL ),
        ENTRY( "AddTaskCMPXTaskQueueL", CCommonTestClass::AddTaskCMPXTaskQueueL ),
        ENTRY( "RemoveTaskCMPXTaskQueueL", CCommonTestClass::RemoveTaskCMPXTaskQueueL ),
        ENTRY( "RemoveTask2CMPXTaskQueueL", CCommonTestClass::RemoveTask2CMPXTaskQueueL ),
        ENTRY( "CallbackCMPXTaskQueueL", CCommonTestClass::CallbackCMPXTaskQueueL ),
        ENTRY( "ParamCMPXTaskQueueL", CCommonTestClass::ParamCMPXTaskQueueL ),
        ENTRY( "PtrDataCMPXTaskQueueL", CCommonTestClass::PtrDataCMPXTaskQueueL ),
        ENTRY( "BufDataCMPXTaskQueueL", CCommonTestClass::BufDataCMPXTaskQueueL ),
        ENTRY( "TaskCMPXTaskQueueL", CCommonTestClass::TaskCMPXTaskQueueL ),
        // test cases for the header "testmpxuser.h" 
        ENTRY( "CompareUids", CCommonTestClass::CompareUids ),
        ENTRY( "IsCallOngoing", CCommonTestClass::IsCallOngoing ),
        ENTRY( "CompleteWithDllPath", CCommonTestClass::CompleteWithDllPath ),
        ENTRY( "Alloc8L", CCommonTestClass::Alloc8L ),
        ENTRY( "AllocL", CCommonTestClass::AllocL ),
        ENTRY( "Ptr", CCommonTestClass::Ptr ),
        ENTRY( "Ptr8", CCommonTestClass::Ptr8 ),
        ENTRY( "CreateBufferLC", CCommonTestClass::CreateBufferLC ),
        ENTRY( "CreateBufferDesCL", CCommonTestClass::CreateBufferDesCL ),////two
        ENTRY( "CreateBufferDesCL", CCommonTestClass::CreateBufferDesCL ),
        ENTRY( "CreateBufferDesCArrayL", CCommonTestClass::CreateBufferDesCArrayL ),
        
        ENTRY( "CreateBufferMsgL", CCommonTestClass::CreateBufferMsgL ),//added by 2009,2,10
               
        ENTRY( "ExternalizeL", CCommonTestClass::ExternalizeL ),
        ENTRY( "ExternalizeArrayL", CCommonTestClass::ExternalizeArrayL ),
        ENTRY( "InternalizeArrayL", CCommonTestClass::InternalizeArrayL ),
        
        ENTRY( "InternalizeDesCArrayL", CCommonTestClass::InternalizeDesCArrayL ),//added by 2009,2,11
        
        ENTRY( "CopyArrayL", CCommonTestClass::CopyArrayL ),
        ENTRY( "MergeArray", CCommonTestClass::MergeArray ),
        ENTRY( "MergeAttributeL", CCommonTestClass::MergeAttributeL ),
        ENTRY( "ProcessIdL", CCommonTestClass::ProcessIdL ),
        ENTRY( "CompareOrderedUidArrays", CCommonTestClass::CompareOrderedUidArrays ),       
        ENTRY( "CreateFromBufferL", CCommonTestClass::CreateFromBufferL ),
        ENTRY( "CreateCMPXViewPlugin", CCommonTestClass::CreateCMPXViewPlugin ),
        ENTRY( "DeleteCMPXViewPlugin", CCommonTestClass::DeleteCMPXViewPlugin ),
        // test cases for the header "mpxattribute.h" 
        ENTRY( "MpxAttDefaultConst", CCommonTestClass::MpxAttDefaultConst ),
        ENTRY( "MpxAttConst", CCommonTestClass::MpxAttConst ),
        ENTRY( "MpxAttCopyConstAtt", CCommonTestClass::MpxAttCopyConstAtt ),
        ENTRY( "MpxAttCopyConstAttData", CCommonTestClass::MpxAttCopyConstAttData ),
        ENTRY( "MpxAttOperatorAssignment", CCommonTestClass::MpxAttOperatorAssignment ),
        ENTRY( "MpxAttOperatorEqual", CCommonTestClass::MpxAttOperatorEqual ),
        ENTRY( "MpxAttStreamingL", CCommonTestClass::MpxAttStreamingL ),
        ENTRY( "MpxAttContentId", CCommonTestClass::MpxAttContentId ),
        ENTRY( "MpxAttAttributeId", CCommonTestClass::MpxAttAttributeId ),
        ENTRY( "MpxAttMatch", CCommonTestClass::MpxAttMatch ),
        ENTRY( "MpxAttMatchContentId", CCommonTestClass::MpxAttMatchContentId ),
        ENTRY( "MpxAttOperatorAndAttData", CCommonTestClass::MpxAttOperatorAndAttData ),
        ENTRY( "MpxAttOperatorAndData", CCommonTestClass::MpxAttOperatorAndData ),
        ENTRY( "MpxAttOperatorOr", CCommonTestClass::MpxAttOperatorOr ),
        // test cases for the header "mpxmedia.h"  
        ENTRY( "MpxMediaNewL", CCommonTestClass::MpxMediaNewL ),
        ENTRY( "MpxMediaNewLSupportedIdsL", CCommonTestClass::MpxMediaNewLSupportedIdsL ),
        ENTRY( "MpxMediaNewLMediaL", CCommonTestClass::MpxMediaNewLMediaL ),
        ENTRY( "MpxMediaNewLDataHandleL", CCommonTestClass::MpxMediaNewLDataHandleL ),
        ENTRY( "MpxMediaNewLStreamL", CCommonTestClass::MpxMediaNewLStreamL ),
        ENTRY( "MpxMediaOperatorAssignment", CCommonTestClass::MpxMediaOperatorAssignment ),
        ENTRY( "MpxMediaSupportedIds", CCommonTestClass::MpxMediaSupportedIds ),
        ENTRY( "MpxMediaAttributes", CCommonTestClass::MpxMediaAttributes ),
        ENTRY( "MpxMediaSetTObjectValueL", CCommonTestClass::MpxMediaSetTObjectValueL ),
        ENTRY( "MpxMediaSetCObjectValueL", CCommonTestClass::MpxMediaSetCObjectValueL ),
        ENTRY( "MpxMediaSetNoNewLCObjectL", CCommonTestClass::MpxMediaSetNoNewLCObjectL ),
        ENTRY( "MpxMediaSetTextValueL", CCommonTestClass::MpxMediaSetTextValueL ),
        ENTRY( "MpxMediaIsSupported", CCommonTestClass::MpxMediaIsSupported ),
        ENTRY( "MpxMediaCount", CCommonTestClass::MpxMediaCount ),
        ENTRY( "MpxMediaAttribute", CCommonTestClass::MpxMediaAttribute ),
        ENTRY( "MpxMediaAttributesSet", CCommonTestClass::MpxMediaAttributesSet ),
        ENTRY( "MpxMediaIndex", CCommonTestClass::MpxMediaIndex ),
        ENTRY( "MpxMediaTypeIndex", CCommonTestClass::MpxMediaTypeIndex ),
        ENTRY( "MpxMediaTypeAttribute", CCommonTestClass::MpxMediaTypeAttribute ),
        ENTRY( "MpxMediaReset", CCommonTestClass::MpxMediaReset ),
        ENTRY( "MpxMediaDeleteAttribute", CCommonTestClass::MpxMediaDeleteAttribute ),
        ENTRY( "MpxMediaDeleteIndex", CCommonTestClass::MpxMediaDeleteIndex ),
        ENTRY( "MpxMediaValueText", CCommonTestClass::MpxMediaValueText ),
        ENTRY( "MpxMediaValue", CCommonTestClass::MpxMediaValue ),
        ENTRY( "MpxMediaValueTObjectL", CCommonTestClass::MpxMediaValueTObjectL ),
        ENTRY( "MpxMediaValueCObjectL", CCommonTestClass::MpxMediaValueCObjectL ),
        ENTRY( "MpxMediaValueNoNewLCObjectL", CCommonTestClass::MpxMediaValueNoNewLCObjectL ),
        ENTRY( "MpxMediaStreamingL", CCommonTestClass::MpxMediaStreamingL ),
        // test cases for the header "mpxmediaarray.h"
        ENTRY( "MpxMediaArrayNewL", CCommonTestClass::MpxMediaArrayNewL ),
        ENTRY( "MpxMediaArrayNewLMediaArrayL", CCommonTestClass::MpxMediaArrayNewLMediaArrayL ),
        ENTRY( "MpxMediaArrayCount", CCommonTestClass::MpxMediaArrayCount ),
        
        ENTRY( "MpxMediaArrayAtLByIndexL", CCommonTestClass::MpxMediaArrayAtLByIndexL ),//added by on 2009,2,11
        
        ENTRY( "MpxMediaArrayAppendLByPtrL", CCommonTestClass::MpxMediaArrayAppendLByPtrL ),
        ENTRY( "MpxMediaArrayAppendLByRefL", CCommonTestClass::MpxMediaArrayAppendLByRefL ),
        ENTRY( "MpxMediaArrayReset", CCommonTestClass::MpxMediaArrayReset ),
        ENTRY( "MpxMediaArrayRemove", CCommonTestClass::MpxMediaArrayRemove ),
        ENTRY( "MpxMediaArrayInsertByPtr", CCommonTestClass::MpxMediaArrayInsertByPtr ),
        ENTRY( "MpxMediaArrayInsertByRef", CCommonTestClass::MpxMediaArrayInsertByRef ),
        ENTRY( "MpxMediaArrayInsertLByPtrL", CCommonTestClass::MpxMediaArrayInsertLByPtrL ),
        ENTRY( "MpxMediaArrayInsertLByRefL", CCommonTestClass::MpxMediaArrayInsertLByRefL ),
        ENTRY( "MpxMediaArraySet", CCommonTestClass::MpxMediaArraySet ),
        ENTRY( "MpxMediaArrayStreamingL", CCommonTestClass::MpxMediaArrayStreamingL ),
        // test cases for the header "mpxcollectionpath.h"
        ENTRY( "MpxItemIdConst", CCommonTestClass::MpxItemIdConst ),
        ENTRY( "MpxItemIdReset", CCommonTestClass::MpxItemIdReset ),
        ENTRY( "MpxCollPathNewL", CCommonTestClass::MpxCollPathNewL ),
        ENTRY( "MpxCollPathNewLPathL", CCommonTestClass::MpxCollPathNewLPathL ),
        ENTRY( "MpxCollPathNewLStreamL", CCommonTestClass::MpxCollPathNewLStreamL ),
        ENTRY( "MpxCollPathAppendLIdL", CCommonTestClass::MpxCollPathAppendLIdL ),
        ENTRY( "MpxCollPathAppendLIdsL", CCommonTestClass::MpxCollPathAppendLIdsL ),
        ENTRY( "MpxCollPathLevels", CCommonTestClass::MpxCollPathLevels ),
        ENTRY( "MpxCollPathBack", CCommonTestClass::MpxCollPathBack ),
        ENTRY( "MpxCollPathReset", CCommonTestClass::MpxCollPathReset ),
        ENTRY( "MpxCollPathIndex", CCommonTestClass::MpxCollPathIndex ),
        ENTRY( "MpxCollPathOperatorPlus", CCommonTestClass::MpxCollPathOperatorPlus ),
        ENTRY( "MpxCollPathOperatorMinus", CCommonTestClass::MpxCollPathOperatorMinus ),
        ENTRY( "MpxCollPathSetToFirst", CCommonTestClass::MpxCollPathSetToFirst ),
        ENTRY( "MpxCollPathSetToLast", CCommonTestClass::MpxCollPathSetToLast ),
        ENTRY( "MpxCollPathSetIndex", CCommonTestClass::MpxCollPathSetIndex ),
        ENTRY( "MpxCollPathSetId", CCommonTestClass::MpxCollPathSetId ),
        ENTRY( "MpxCollPathSetMode", CCommonTestClass::MpxCollPathSetMode ),
        ENTRY( "MpxCollPathSetLAttrsL", CCommonTestClass::MpxCollPathSetLAttrsL ),
        ENTRY( "MpxCollPathOpenNextMode", CCommonTestClass::MpxCollPathOpenNextMode ),
        ENTRY( "MpxCollPathOpenPreviousMode", CCommonTestClass::MpxCollPathOpenPreviousMode ),
        ENTRY( "MpxCollPathSelectLIdL", CCommonTestClass::MpxCollPathSelectLIdL ),
        ENTRY( "MpxCollPathSelectLIndexL", CCommonTestClass::MpxCollPathSelectLIndexL ),
        ENTRY( "MpxCollPathSelectAllL", CCommonTestClass::MpxCollPathSelectAllL ),
        ENTRY( "MpxCollPathDeselectId", CCommonTestClass::MpxCollPathDeselectId ),
        ENTRY( "MpxCollPathDeselectIndex", CCommonTestClass::MpxCollPathDeselectIndex ),
        ENTRY( "MpxCollPathDeselectAll", CCommonTestClass::MpxCollPathDeselectAll ),
        ENTRY( "MpxCollPathRemoveId", CCommonTestClass::MpxCollPathRemoveId ),
        ENTRY( "MpxCollPathRemoveIndex", CCommonTestClass::MpxCollPathRemoveIndex ),
        ENTRY( "MpxCollPathIsSelectedId", CCommonTestClass::MpxCollPathIsSelectedId ),
        ENTRY( "MpxCollPathIsSelectedIndex", CCommonTestClass::MpxCollPathIsSelectedIndex ),
        ENTRY( "MpxCollPathClearSelection", CCommonTestClass::MpxCollPathClearSelection ),
        ENTRY( "MpxCollPathSelection", CCommonTestClass::MpxCollPathSelection ),
        ENTRY( "MpxCollPathSelectionL", CCommonTestClass::MpxCollPathSelectionL ),
        ENTRY( "MpxCollPathId", CCommonTestClass::MpxCollPathId ),
        ENTRY( "MpxCollPathCount", CCommonTestClass::MpxCollPathCount ),
        ENTRY( "MpxCollPathIndexOfId", CCommonTestClass::MpxCollPathIndexOfId ),
        ENTRY( "MpxCollPathIdOfIndex", CCommonTestClass::MpxCollPathIdOfIndex ),
        ENTRY( "MpxCollPathOpenAttributes", CCommonTestClass::MpxCollPathOpenAttributes ),
        ENTRY( "MpxCollPathIndexLevel", CCommonTestClass::MpxCollPathIndexLevel ),
        ENTRY( "MpxCollPathIdLevel", CCommonTestClass::MpxCollPathIdLevel ),
        ENTRY( "MpxCollPathStreamingL", CCommonTestClass::MpxCollPathStreamingL ),
        ENTRY( "MpxCollPathHandleChange", CCommonTestClass::MpxCollPathHandleChange ),
        //ENTRY( "ContainerPathL", CCommonTestClass::ContainerPathL ),
        // test cases for the header "mpxclientlist.h"
        //there are so many absent functions
        ENTRY( "MpxClientListNewL", CCommonTestClass::MpxClientListNewL ),
        ENTRY( "MpxClientListClientCount", CCommonTestClass::MpxClientListClientCount ),
        ENTRY( "MpxClientListIsClient", CCommonTestClass::MpxClientListIsClient ),
        ENTRY( "MpxClientListFindByMsg", CCommonTestClass::MpxClientListFindByMsg ),
        ENTRY( "MpxClientListFindById", CCommonTestClass::MpxClientListFindById ), //pass
        //add function to testmpxclientlist.cpp 16,9,2008
        ENTRY( "MpxClientListDestruction", CCommonTestClass::MpxClientListDestruction ),
        ENTRY( "MpxClientListAddClientL", CCommonTestClass::MpxClientListAddClientL ),                     //pass added  16,9,2008
        ENTRY( "MpxClientListAddClientLUsingMode", CCommonTestClass::MpxClientListAddClientLUsingMode ),   //pass added 16,9,2008
        ENTRY( "MpxClientListRemoveClient", CCommonTestClass::MpxClientListRemoveClient ),                 //pass added 16,9,2008
        ENTRY( "MpxClientListClientProcessList", CCommonTestClass::MpxClientListClientProcessList ),       //pass added 16,9,2008
        ENTRY( "DeleteMpxClientList", CCommonTestClass::DeleteMpxClientList ),                             //pass added 16,9,2008
        ENTRY( "MpxClientListClientMode", CCommonTestClass::MpxClientListClientMode ),                     //pass added 17.9 2008
        ENTRY( "MpxClientListSendMsg", CCommonTestClass::MpxClientListSendMsg ),                           //Pass added 22.9 2008
        ENTRY( "MpxClientListSendMsgaIndex", CCommonTestClass::MpxClientListSendMsgaIndex ),               //pass modified 7,10 2008
        ENTRY( "SendMsgLByMsgCMPXClientList", CCommonTestClass::SendMsgLByMsgCMPXClientList ),             //pass
        ENTRY( "MPXClientListConvertMsgLC", CCommonTestClass::MPXClientListConvertMsgLC ), 
        ENTRY( "SendMsgLIndexAndTMPMsgCMPXClientList", CCommonTestClass::SendMsgLIndexAndTMPMsgCMPXClientList ),//pass
        ENTRY( "MpxClientListAddSubscriptionL", CCommonTestClass::MpxClientListAddSubscriptionL ),  //added 20.10 2008
        ENTRY( "MpxClientListRemoveSubscriptionL", CCommonTestClass::MpxClientListRemoveSubscriptionL ),  //added 20.10 2008
        ENTRY( "MpxClientListRemoveAllSubscriptionsL", CCommonTestClass::MpxClientListRemoveAllSubscriptionsL ),  //added 20.10 2008
        // test cases for the header "mpxpluginhandlerbase.h"
        //There is absent.
        ENTRY( "MpxPluginHandlerBaseSelectPluginLIdL", CCommonTestClass::MpxPluginHandlerBaseSelectPluginLIdL ),
        ENTRY( "MpxPluginHandlerBaseSelectPluginLNameL", CCommonTestClass::MpxPluginHandlerBaseSelectPluginLNameL ),
        ENTRY( "MpxPluginHandlerBaseSelectPluginLUriL", CCommonTestClass::MpxPluginHandlerBaseSelectPluginLUriL ),
        ENTRY( "MpxPluginHandlerBaseSelectPluginLTypeL", CCommonTestClass::MpxPluginHandlerBaseSelectPluginLTypeL ),
        ENTRY( "MpxPluginHandlerBaseClearSelectionCriteria", CCommonTestClass::MpxPluginHandlerBaseClearSelectionCriteria ),
        ENTRY( "MpxPluginHandlerBasePluginNameType", CCommonTestClass::MpxPluginHandlerBasePluginNameType ),
        ENTRY( "MpxPluginHandlerBasePluginNamesL", CCommonTestClass::MpxPluginHandlerBasePluginNamesL ),
        ENTRY( "MpxPluginHandlerBaseSupportedMimeTypesL", CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesL ),
        ENTRY( "MpxPluginHandlerBaseSupportedExtensionsL", CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsL ),
        ENTRY( "MpxPluginHandlerBaseSupportedSchemasL", CCommonTestClass::MpxPluginHandlerBaseSupportedSchemasL ),
        ENTRY( "MpxPluginHandlerBaseSupportedMimeTypesLUidL", CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesLUidL ),
        ENTRY( "MpxPluginHandlerBaseSupportedExtensionsLUidL", CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsLUidL ),
        ENTRY( "MpxPluginHandlerBaseGetPluginTypes", CCommonTestClass::MpxPluginHandlerBaseGetPluginTypes ),
        ENTRY( "MpxPluginHandlerBaseGetPluginUids", CCommonTestClass::MpxPluginHandlerBaseGetPluginUids ),
        ENTRY( "MpxPluginHandlerBaseGetPluginUidsL", CCommonTestClass::MpxPluginHandlerBaseGetPluginUidsL ),
        ENTRY( "MpxPluginHandlerBasePluginUid", CCommonTestClass::MpxPluginHandlerBasePluginUid ),
        ENTRY( "MpxPluginHandlerBaseIndexOf", CCommonTestClass::MpxPluginHandlerBaseIndexOf ),
        ENTRY( "MpxPluginHandlerBaseCreatePluginListL", CCommonTestClass::MpxPluginHandlerBaseCreatePluginListL ),
        ENTRY( "MpxPluginHandlerBaseFindPlugin", CCommonTestClass::MpxPluginHandlerBaseFindPlugin ),
        //added tested functions for protected export functions of CMPXPluginHandlerBase
        ENTRY( "MpxPluginHBBasePluginsChangedL", CCommonTestClass::MpxPluginHBBasePluginsChangedL), 
        ENTRY( "MpxPluginHBBaseIsPluginLoaded", CCommonTestClass::MpxPluginHBBaseIsPluginLoaded), 
        ENTRY( "MpxPluginHBBaseLoadPluginL", CCommonTestClass::MpxPluginHBBaseLoadPluginL), 
        ENTRY( "MpxPluginHBBaseHandlePluginUnload", CCommonTestClass::MpxPluginHBBaseHandlePluginUnload), 
        
        
        //add function to testmpxuser.cpp 
        ENTRY( "RecreateBufferL", CCommonTestClass::RecreateBufferL),              //pass  12,9,2008
        ENTRY( "CompleteWithDllPathOL", CCommonTestClass::CompleteWithDllPathOL),  //pass  12,9,2008
        ENTRY( "FindWindowGroupIdL", CCommonTestClass::FindWindowGroupIdL),        //pass  12,9,2008
        ENTRY( "Alloc8ZL", CCommonTestClass::Alloc8ZL),                            //pass  12,9,2008
        //add function to testmpxmessagemonitor.cpp 16,9,2008
        ENTRY( "GetNextMessageCMPXMessageMonitorL", CCommonTestClass::GetNextMessageCMPXMessageMonitorL ), //pass
        ENTRY( "DoCancelCMPXMessageMonitorL", CCommonTestClass::DoCancelCMPXMessageMonitorL ),             //pass
        ENTRY( "RunErrorCMPXMessageMonitorL", CCommonTestClass::RunErrorCMPXMessageMonitorL ),             //pass    
        //add function to testmpxcollectionpath.cpp 17,9,2008
        ENTRY( "MpxCollectionpathContainerPathL", CCommonTestClass::MpxCollectionpathContainerPathL ),   //pass  17,9,2008
        ENTRY( "MpxCollectionpathInsertL", CCommonTestClass::MpxCollectionpathInsertL ),                 //pass  17,9,2008
        ENTRY( "MpxCollectionpathItems", CCommonTestClass::MpxCollectionpathItems ),                     //pass  17,9,2008
        ENTRY( "MpxCollectionpathUpdate", CCommonTestClass::MpxCollectionpathUpdate ),                   //pass  17,9,2008
        ENTRY( "MpxCollectionpathDelete", CCommonTestClass::MpxCollectionpathDelete ),                   //pass  18,9,2008
        ENTRY( "MpxCollectionpathCollPathSetAttrsL", CCommonTestClass::MpxCollectionpathCollPathSetAttrsL ),       //pass 18,9,2008
        //add function to testmpxcollectiontype.cpp 17,9,2008 
        ENTRY( "TUidMPXCollectionType", CCommonTestClass::TUidMPXCollectionType ),     //pass 17,9,2008                                                                          
        //add function to testmpxsession.cpp 17,9,2008 
        ENTRY( "SendReceiveLTIpcArgsRMPXSessionL", CCommonTestClass::SendReceiveLTIpcArgsRMPXSessionL ),  //pass 17,9,2008
        ENTRY( "CMPXCollectionTypeDelete", CCommonTestClass::CMPXCollectionTypeDelete ),                  //pass(debug )  9/19/2008 1:32                                                                            
        //add function to testmpxpluginhandlerbase.cpp  18,9,2008 
        ENTRY( "MpxPluginHandlerBaseSupportUids", CCommonTestClass::MpxPluginHandlerBaseSupportUids ),                   //faile modified 23,9,2008  
        ENTRY( "MpxPluginHandlerBasePluginNameByUid", CCommonTestClass::MpxPluginHandlerBasePluginNameByUid ),           //crash added 19,9,2008
        ENTRY( "MpxPluginHandlerBasePluginFlagsL", CCommonTestClass::MpxPluginHandlerBasePluginFlagsL ),                 //crash added 19,9,2008  
        ENTRY( "MpxPluginHandlerBasePluginType", CCommonTestClass::MpxPluginHandlerBasePluginType ),                     //pass(debug) 19,9,2008
	    ENTRY( "MpxPluginHandlerBaseDestruction", CCommonTestClass::MpxPluginHandlerBaseDestruction ),                           //pass modified 23,9,2008
	    ENTRY( "MpxPluginHandlerBaseSelectPluginLRFile", CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile ),             //pass(debug) modified  24,9,2008
	    ENTRY( "MpxPluginHandlerBaseGetSelectionCriteria", CCommonTestClass::MpxPluginHandlerBaseGetSelectionCriteria ),         //pass debug 26,9,2008
	    ENTRY( "MpxPluginHandlerBaseUsePluginTUid", CCommonTestClass::MpxPluginHandlerBaseUsePluginTUid ),                       //pass debug 19,9,2008
	    ENTRY( "MpxPluginHandlerBaseReleasePluginTUid", CCommonTestClass::MpxPluginHandlerBaseReleasePluginTUid ),               //pass debug 19,9,2008
	    ENTRY( "MpxPluginHandlerBaseCreate", CCommonTestClass::MpxPluginHandlerBaseCreate ),                                     // //pass debug 19,9,2008
	    //add MpxMediaCopyL to testmpxmedia.cpp  9/18/2008 10:47
        ENTRY( "MpxMediaCopyL", CCommonTestClass::MpxMediaCopyL ),                       //pass modified 23,9,2008
        ENTRY( "MpxMediaDeleteL", CCommonTestClass::MpxMediaDeleteL ),                   //pass  18,9,2008
        ENTRY( "MpxMediaHeapMemoryInfoL", CCommonTestClass::MpxMediaHeapMemoryInfoL ),   //pass  18,9,2008
        ENTRY( "MpxMediaMergeMediaL", CCommonTestClass::MpxMediaMergeMediaL ),           //pass  18,9,2008
        ENTRY( "MpxMediaMatchL", CCommonTestClass::MpxMediaMatchL ),                     //modified  24,9,2008
        ENTRY( "MpxMediaSetErrorL", CCommonTestClass::MpxMediaSetErrorL ),               //pass  6,10,2008
        ENTRY( "MpxMediaErrorL", CCommonTestClass::MpxMediaErrorL ),                     //pass  6,10,2008
        ENTRY( "MpxMediaExternalizeL", CCommonTestClass::MpxMediaExternalizeL ),         //pass  18,9,2008
        ENTRY( "MpxMediaInternalizeL", CCommonTestClass::MpxMediaInternalizeL ),         //pass  18,9,2008   
        ENTRY( "CreateCMPXTaskQueue", CCommonTestClass::CreateCMPXTaskQueue),            //Pass  22,9,2008
        ENTRY( "DeleteCMPXTaskQueue", CCommonTestClass::DeleteCMPXTaskQueue),            //Pass  22,9,2008
        ENTRY( "ScheduleNextTaskCMPXTaskQueue",CCommonTestClass::ScheduleNextTaskCMPXTaskQueue ),//Pass 22,9,2008
        ENTRY( "ResetCMPXTaskQueue", CCommonTestClass::ResetCMPXTaskQueue ),                     //Pass 22,9,2008
        ENTRY( "DeleteCMPXActiveTaskQueue",CCommonTestClass::DeleteCMPXActiveTaskQueue ),        //Pass 22,9,2008
        //test in mpxplugininfo.h  added 10/6/2008 
        ENTRY( "CMPXPluginInfoDestructor", CCommonTestClass::CMPXPluginInfoDestructor ),//Pass debug
        ENTRY( "CMPXPluginInfoImplementationUid", CCommonTestClass::CMPXPluginInfoImplementationUid ),//Pass debug
        ENTRY( "CMPXPluginInfoFlags", CCommonTestClass::CMPXPluginInfoFlags ),//Crash debug
        //added test functions to test protected export functions
        ENTRY( "CMPXPluginInfoProcessTaggedDataExL", CCommonTestClass::CMPXPluginInfoProcessTaggedDataExL ),
        ENTRY( "CMPXPluginInfoExtractIntoArrayL", CCommonTestClass::CMPXPluginInfoExtractIntoArrayL ),
        
        
        //add test functions for CMPXSubscription of mpxsubscription.h 
        ENTRY( "CMPXSubscriptionNewL", CCommonTestClass::CMPXSubscriptionNewL ),
        ENTRY( "CMPXSubscriptionNewLWithMediaArrayL", CCommonTestClass::CMPXSubscriptionNewLWithMediaArrayL ),
        ENTRY( "CMPXSubscriptionAddItemL", CCommonTestClass::CMPXSubscriptionAddItemL ),
        ENTRY( "CMPXSubscriptionItemsL", CCommonTestClass::CMPXSubscriptionItemsL ),
        ENTRY( "CMPXSubscriptionIsEqualOperator", CCommonTestClass::CMPXSubscriptionIsEqualOperator ),
        ENTRY( "MpxPluginHandlerBaseSelectPluginLRFile64", CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile64 ),
        };
    const TInt count = sizeof( KFunctions ) / 
                        sizeof( TStifFunctionInfo );
    return RunInternalL( KFunctions, count, aItem );
    }
//  End of File
