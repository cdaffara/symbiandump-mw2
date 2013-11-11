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
* Description: MPXPluginHandlerBase testing implementation (mpxpluginhandlerbase.h)
*
*/


#include "commontestclass.h"
#include <mpxpluginhandlerbase.h>
#include <mpxplugininfo.h>
#include <mpxpluginhandlerobserver.h>


// Begin MPXPluginHandlerBase testing implementation (mpxpluginhandlerbase.h)============================
const TUid KMPXPPluginHandlerBaseUid = {0x12345};
const TUid KMPXPlaybackInterfaceUid = {0x101FFC05};
class CDummyMpxPluginHandler : public CMPXPluginHandlerBase,
                               public MMPXPluginHandlerObserver
    {
public:  // Constructors and destructor
    CDummyMpxPluginHandler();  
    ~CDummyMpxPluginHandler();
    
    //void ResolvePluginL();
    
    void BasePluginsChangedL();
    
    TBool BaseIsPluginLoaded(const TUid&  aPluginUid );
    
    void BaseLoadPluginL(const TUid& aPluginUid);
    
    void BaseHandlePluginUnload(const TUid& aPluginUid);
    
    /*
    * From MMPXPluginHandlerObserver
    * @see MMPXPluginHandlerObserver
    */
    void HandlePluginHandlerEvent(
            TPluginHandlerEvents aEvent, 
            const TUid& aPluginUid,
            TInt aLoaded, TInt aData);
    
    };
    
// -----------------------------------------------------------------------------
// CDummyMpxPluginHandler::CDummyMpxPluginHandler()
// -----------------------------------------------------------------------------

CDummyMpxPluginHandler::CDummyMpxPluginHandler():
    CMPXPluginHandlerBase (
            KMPXPlaybackInterfaceUid,
            CMPXPluginHandlerBase::ESelectionUid, 
            0,*this,
            CMPXPluginMonitor::NewL((TUid)KMPXPlaybackInterfaceUid/*, *this*/))
    {
        TInt err = KErrNone;
        TRAP(err,iDataType = HBufC8::NewL(11));//NULL;
        TRAP(err,iScheme = HBufC8::NewL(9));//NULL;
        TRAP(err,iExt = HBufC8::NewL(6));//NULL;
        
        TRAP(err,BaseConstructL()); //if protected export functions should be stifed, then this function
        							//must be took away and be tested separately.
    }



// -----------------------------------------------------------------------------
// CDummyMpxPluginHandler::~CDummyMpxPluginHandler()
// -----------------------------------------------------------------------------
CDummyMpxPluginHandler::~CDummyMpxPluginHandler()
    {
    }
// -----------------------------------------------------------------------------
// CDummyMpxPluginHandler::BasePluginsChangedL()
// -----------------------------------------------------------------------------
void CDummyMpxPluginHandler::BasePluginsChangedL()
    {
    PluginsChangedL();
    }

// -----------------------------------------------------------------------------
// CDummyMpxPluginHandler::BaseLoadPluginL()
// -----------------------------------------------------------------------------
void CDummyMpxPluginHandler::BaseLoadPluginL(const TUid& aPluginUid)
    {
    LoadPluginL( aPluginUid );
    }

// -----------------------------------------------------------------------------
// CDummyMpxPluginHandler::BaseHandlePluginUnload()
// -----------------------------------------------------------------------------
void CDummyMpxPluginHandler::BaseHandlePluginUnload(const TUid& aPluginUid)
    {
    HandlePluginUnload( aPluginUid );
    }

// -----------------------------------------------------------------------------
// CDummyMpxPluginHandler::BaseIsPluginLoaded()
// -----------------------------------------------------------------------------
TBool CDummyMpxPluginHandler::BaseIsPluginLoaded(const TUid&  aPluginUid )
    {
    return IsPluginLoaded( aPluginUid );
    }

// -----------------------------------------------------------------------------
// CDummyMpxPluginHandler::HandlePluginHandlerEvent()
// -----------------------------------------------------------------------------
void CDummyMpxPluginHandler::HandlePluginHandlerEvent(
        TPluginHandlerEvents /*aEvent*/, const TUid& /*aPluginUid*/,
        TBool /*aLoaded*/, TInt /*aData*/ )
    {
    }

// -----------------------------------------------------------------------------
// CTestMpxPluginHandler::ResolvePluginL()
// -----------------------------------------------------------------------------
//void CDummyMpxPluginHandler::ResolvePluginL()
//    {
//    
//    }


// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSelectPluginLIdL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSelectPluginLIdL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLIdL testing CMPXPluginHandlerBase::SelectPluginL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLIdL testing CMPXPluginHandlerBase::SelectPluginL() begin"));
    TInt err=KErrNone;
    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.SelectPluginL(KMPXPPluginHandlerBaseUid);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSelectPluginLNameL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSelectPluginLNameL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLNameL testing CMPXPluginHandlerBase::SelectPluginL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLNameL testing CMPXPluginHandlerBase::SelectPluginL() begin"));
    TInt err=KErrNone;
    TBufC<10> name(_L("TestName"));

    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.SelectPluginL(name);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSelectPluginLUriL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSelectPluginLUriL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLUriL testing CMPXPluginHandlerBase::SelectPluginL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLUriL testing CMPXPluginHandlerBase::SelectPluginL() begin"));
    TInt err=KErrNone;
    TBufC<20> uri(_L("c:\\data\\test.mp3"));

    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.SelectPluginL(uri, KNullDesC8);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSelectPluginLTypeL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSelectPluginLTypeL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLTypeL testing CMPXPluginHandlerBase::SelectPluginL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLTypeL testing CMPXPluginHandlerBase::SelectPluginL() begin"));
    TInt err=KErrNone;
    TInt type=1;

    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.SelectPluginL(type);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseClearSelectionCriteria()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseClearSelectionCriteria(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseClearSelectionCriteria testing CMPXPluginHandlerBase::ClearSelectionCriteria() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseClearSelectionCriteria testing CMPXPluginHandlerBase::ClearSelectionCriteria() begin"));
    TInt err=KErrNone;
    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.ClearSelectionCriteria();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBasePluginNameType()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBasePluginNameType(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBasePluginNameType testing CMPXPluginHandlerBase::PluginName() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBasePluginNameType testing CMPXPluginHandlerBase::PluginName() begin"));
    TInt err=KErrNone;
    TBufC<20> name;

    CDummyMpxPluginHandler pluginHandler;
    name = pluginHandler.PluginName(0);
    iLog->Log(_L("Plugin Name=%S"), &name);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBasePluginNamesL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBasePluginNamesL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBasePluginNamesL testing CMPXPluginHandlerBase::PluginNamesL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBasePluginNamesL testing CMPXPluginHandlerBase::PluginNamesL() begin"));
    TInt err=KErrNone;
    CDesCArray* names;

    CDummyMpxPluginHandler pluginHandler;
    names = pluginHandler.PluginNamesL();
    iLog->Log(_L("Number of plugin names=%d"), names->Count());
    delete names;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesL testing CMPXPluginHandlerBase::SupportedMimeTypesL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesL testing CMPXPluginHandlerBase::SupportedMimeTypesL() begin"));
    TInt err=KErrNone;
    CDesCArray* mimeType;

    CDummyMpxPluginHandler pluginHandler;
    mimeType = pluginHandler.SupportedMimeTypesL();
    iLog->Log(_L("Number of plugin mimeType=%d"), mimeType->Count());
    delete mimeType;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsL testing CMPXPluginHandlerBase::SupportedExtensionsL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsL testing CMPXPluginHandlerBase::SupportedExtensionsL() begin"));
    TInt err=KErrNone;
    CDesCArray* ext;

    CDummyMpxPluginHandler pluginHandler;
    ext = pluginHandler.SupportedExtensionsL();
    iLog->Log(_L("Number of plugin extensions=%d"), ext->Count());
    delete ext;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSupportedSchemasL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSupportedSchemasL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedSchemasL testing CMPXPluginHandlerBase::SupportedSchemasL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedSchemasL testing CMPXPluginHandlerBase::SupportedSchemasL() begin"));
    TInt err=KErrNone;
    CDesCArray* schemas;

    CDummyMpxPluginHandler pluginHandler;
    schemas = pluginHandler.SupportedSchemasL();
    iLog->Log(_L("Number of plugin schemas=%d"), schemas->Count());
    delete schemas;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesLUidL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesLUidL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesLUidL testing CMPXPluginHandlerBase::SupportedMimeTypesL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedMimeTypesLUidL testing CMPXPluginHandlerBase::SupportedMimeTypesL() begin"));
    TInt err=KErrNone;
    CDesCArray* mimeType;

    CDummyMpxPluginHandler pluginHandler;
    mimeType = pluginHandler.SupportedMimeTypesL(KMPXPPluginHandlerBaseUid);
    iLog->Log(_L("Number of plugin mimeType=%d"), mimeType->Count());
    delete mimeType;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsLUidL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsLUidL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsLUidL testing CMPXPluginHandlerBase::SupportedExtensionsL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSupportedExtensionsLUidL testing CMPXPluginHandlerBase::SupportedExtensionsL() begin"));
    TInt err=KErrNone;
    CDesCArray* ext;

    CDummyMpxPluginHandler pluginHandler;
    ext = pluginHandler.SupportedExtensionsL(KMPXPPluginHandlerBaseUid);
    iLog->Log(_L("Number of plugin extensions=%d"), ext->Count());
    delete ext;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseGetPluginTypes()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseGetPluginTypes(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseGetPluginTypes testing CMPXPluginHandlerBase::GetPluginTypes() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseGetPluginTypes testing CMPXPluginHandlerBase::GetPluginTypes() begin"));
    TInt err=KErrNone;
    RArray<TInt> types;

    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.GetPluginTypes(types);
    iLog->Log(_L("Number of plugin types=%d"), types.Count());
    types.Reset();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseGetPluginUids()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseGetPluginUids(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseGetPluginUids testing CMPXPluginHandlerBase::GetPluginUids() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseGetPluginUids testing CMPXPluginHandlerBase::GetPluginUids() begin"));
    TInt err=KErrNone;
    RArray<TUid> uids;

    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.GetPluginUids(uids);
    iLog->Log(_L("Number of plugin uids=%d"), uids.Count());
    uids.Reset();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseGetPluginUidsL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseGetPluginUidsL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseGetPluginUidsL testing CMPXPluginHandlerBase::GetPluginUidsL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseGetPluginUidsL testing CMPXPluginHandlerBase::GetPluginUidsL() begin"));
    TInt err=KErrNone;
    RArray<TUid> uids;

    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.GetPluginUidsL(uids, 1);
    iLog->Log(_L("Number of plugin uids=%d"), uids.Count());
    uids.Reset();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBasePluginUid()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBasePluginUid(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBasePluginUid testing CMPXPluginHandlerBase::PluginUid() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBasePluginUid testing CMPXPluginHandlerBase::PluginUid() begin"));
    TInt err=KErrNone;
    TUid uid;
    TBufC<10> name = _L("dummyName");

    CDummyMpxPluginHandler pluginHandler;
    uid = pluginHandler.PluginUid(name);
    iLog->Log(_L("Plugin uid=%d"), uid.iUid);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseIndexOf()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseIndexOf(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseIndexOf testing CMPXPluginHandlerBase::IndexOf() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseIndexOf testing CMPXPluginHandlerBase::IndexOf() begin"));
    TInt err=KErrNone;

    CDummyMpxPluginHandler pluginHandler;
    TInt index = pluginHandler.IndexOf(KMPXPPluginHandlerBaseUid);
    iLog->Log(_L("Plugin index=%d"), index);
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseCreatePluginListL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseCreatePluginListL(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseCreatePluginListL testing CMPXPluginHandlerBase::CreatePluginListL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseCreatePluginListL testing CMPXPluginHandlerBase::CreatePluginListL() begin"));
    TInt err=KErrNone;

    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.CreatePluginListL();
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseFindPlugin()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseFindPlugin(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseFindPlugin testing CMPXPluginHandlerBase::FindPlugin() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseFindPlugin testing CMPXPluginHandlerBase::FindPlugin() begin"));
    TInt err=KErrNone;
    RArray<TUid> uids;

    uids.Reset();
    CDummyMpxPluginHandler pluginHandler;
    TUid uid = pluginHandler.FindPlugin(uids.Array());
    iLog->Log(_L("Plugin uid=%d"), uid.iUid);
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::CreateMpxPluginHandlerBase()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseSupportUids(CStifItemParser& /*aItem*/)
    {//TODO need to update
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSupportUids testing CMPXPluginHandlerBase::FindPlugin() SupportUids")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSupportUids testing CMPXPluginHandlerBase::FindPlugin() SupportUids"));
    
    TInt err=KErrNone;
  
    const TUid KMPXCollectionServerUid={0x101FFC09};//KMPXCollectionServerUid={0x101FFC09};/KMPXCollectionServerUid={0x12345};
    RArray<TUid> uids;
    CleanupClosePushL( uids );
    TRAP(err,uids.AppendL(KMPXCollectionServerUid));  

    CDummyMpxPluginHandler* pluginHandler =new CDummyMpxPluginHandler();
    
    //pluginHandler->CreatePluginListL();
    //pluginHandler->ResolvePluginL();
    pluginHandler->GetPluginUids( uids );
    //----------------------------------------------
    
    TUid uid = pluginHandler->FindPlugin(uids.Array());
    pluginHandler->SupportUids(uid, uids.Array());
    //pluginHandler->SupportUids(uid,uids.Array());
    CleanupStack::PopAndDestroy( &uids );
    delete pluginHandler;
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseDestruction()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseDestruction(CStifItemParser& /*aItem*/)
    {
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseDestruction testing CMPXPluginHandlerBase::~CMPXPluginHandlerBase() begin")));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseDestruction testing CMPXPluginHandlerBase::~CMPXPluginHandlerBase() begin"));
     
     TInt err = KErrNone;
     CDummyMpxPluginHandler* pluginHandler =new CDummyMpxPluginHandler();
     delete pluginHandler;
     pluginHandler = NULL;
	 
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseDestruction testing CMPXPluginHandlerBase::~CMPXPluginHandlerBase() end err=%d"),err));
	 iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseDestruction testing CMPXPluginHandlerBase::~CMPXPluginHandlerBase() end err=%d"),err);
     return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------

TInt CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile testing CMPXPluginHandlerBase::SelectPluginL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile testing CMPXPluginHandlerBase::SelectPluginL() begin"));
    TPtrC string;
    TInt err = KErrNone;
    RFile file;
    
    if( aItem.GetNextString( string ) == KErrNone )
       {
        TBuf<120> KFrom;
        KFrom.Append(KCommonTestFilePath);
        KFrom.Append(string);
	    RFs fs;
	    fs.Connect();
	    file.Open(fs, KFrom, EFileShareReadersOrWriters);
	    
	    CDummyMpxPluginHandler pluginHandler;
	    //pluginHandler.SelectPluginL( file );
	    TRAP(err,pluginHandler.SelectPluginL(file));
	    if( err != KErrNone )
	    	{
	    	iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile testing CMPXPluginHandlerBase::SelectPluginL() end=%d"),err);
	    	}
	    file.Close();//added on April 10,2009
	    fs.Close();
	    
       } 
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile64()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------

TInt CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile64(CStifItemParser& aItem)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile64 testing CMPXPluginHandlerBase::SelectPlugin64L() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile64 testing CMPXPluginHandlerBase::SelectPlugin64L() begin"));
    TPtrC string;
    TInt err = KErrNone;
#ifndef SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    err = KErrNotSupported;
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile64 testing CMPXPluginHandlerBase::SelectPlugin64L() end=%d"),err);
#else  // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    RFile64 file;
    
    if( aItem.GetNextString( string ) == KErrNone )
       {
        TBuf<120> KFrom;
        KFrom.Append(KCommonTestFilePath);
        KFrom.Append(string);
        RFs fs;
        fs.Connect();
        file.Open(fs, KFrom, EFileShareReadersOrWriters);
        
        CDummyMpxPluginHandler pluginHandler;
        //pluginHandler.SelectPluginL( file );
        TRAP(err,pluginHandler.SelectPlugin64L(file));
        if( err != KErrNone )
            {
            iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseSelectPluginLRFile64 testing CMPXPluginHandlerBase::SelectPlugin64L() end=%d"),err);
            }
        file.Close();
        fs.Close();
        
       } 
#endif // SYMBIAN_ENABLE_64_BIT_FILE_SERVER_API
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBasePluginNameByUid()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBasePluginNameByUid(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBasePluginNameByUid testing CMPXPluginHandlerBase::PluginName() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBasePluginNameByUid testing CMPXPluginHandlerBase::PluginName() begin"));
   
    TInt err=KErrNone;
    return err;
    }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBasePluginFlagsL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBasePluginFlagsL(CStifItemParser& /*aItem*/)
    {//TODO need to update
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBasePluginFlagsL testing CMPXPluginHandlerBase::PluginFlagsL() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBasePluginFlagsL testing CMPXPluginHandlerBase::PluginFlagsL() begin"));

    CDummyMpxPluginHandler pluginHandler;
    
    //TInt index = pluginHandler.IndexOf(KMPXPPluginHandlerBaseUid);
    TRAPD( err, pluginHandler.PluginFlagsL(KMPXPPluginHandlerBaseUid));
    // Using the invalid id (KMPXPPluginHandlerBaseUid), so it is right if the 
    // return err is KErrNotFound.
    if ( err == KErrNotFound )
        {
        err = KErrNone;
        }
    
    iLog->Log(_L("CCommonTestClass::MpxPluginType testing CMPXPluginHandlerBase::MpxPluginFlagsL() end"),err);
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBasePluginType()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBasePluginType(CStifItemParser& /*aItem*/)
    {
    FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBasePluginType testing CMPXPluginHandlerBase::PluginType() begin")));
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBasePluginType testing CMPXPluginHandlerBase::PluginType() begin"));
    TInt err=KErrNone;
    
    CDummyMpxPluginHandler pluginHandler;
    pluginHandler.PluginType(KMPXPPluginHandlerBaseUid);
    iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBasePluginType testing CMPXPluginHandlerBase::PluginType() end"),err);
    
    return err;
    }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseGetSelectionCriteria()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseGetSelectionCriteria(CStifItemParser& /*aItem*/)
     {
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseGetSelectionCriteria testing CMPXPluginHandlerBase::GetSelectionCriteria() begin")));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseGetSelectionCriteria testing CMPXPluginHandlerBase::GetSelectionCriteria() begin"));
     
     TInt err = KErrNone;         
     CDummyMpxPluginHandler pluginHandler; 
     CMPXPluginHandlerBase::TPluginSelection iSelectionP;
     TInt iPluginType ;
     TUid iTUid ;     
     TPtrC8 iDataTypeP;
     TPtrC8 iSchemeP;
     TPtrC8 iExtensionP;
     
     pluginHandler.GetSelectionCriteria(iSelectionP,iPluginType,iTUid,iDataTypeP,iSchemeP,iExtensionP);
     
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseGetSelectionCriteria testing CMPXPluginHandlerBase::GetSelectionCriteria() end err=%d"),err));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseGetSelectionCriteria testing CMPXPluginHandlerBase::GetSelectionCriteria() end err=%d"),err);
     return err;  
     }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseUsePluginTUid()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseUsePluginTUid(CStifItemParser& /*aItem*/)
     {
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseUsePluginTUid testing CMPXPluginHandlerBase::UsePlugin() begin")));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseUsePluginTUid testing CMPXPluginHandlerBase::UsePlugin() begin"));
     
     CDummyMpxPluginHandler pluginHandler;
     TRAPD(err, pluginHandler.UsePlugin(KMPXPPluginHandlerBaseUid));
     
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseUsePluginTUid testing CMPXPluginHandlerBase::UsePlugin() end err=%d"),err));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseUsePluginTUid testing CMPXPluginHandlerBase::UsePlugin() end err=%d"),err);
     return err;
     }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseReleasePluginTUid()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHandlerBaseReleasePluginTUid(CStifItemParser& /*aItem*/)
     {
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseReleasePluginTUid testing CMPXPluginHandlerBase::ReleasePlugin() begin")));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseReleasePluginTUid testing CMPXPluginHandlerBase::ReleasePlugin() begin"));
     
     TInt err = KErrNone;
     
     CDummyMpxPluginHandler pluginHandler;
     TBool check_unload = pluginHandler.ReleasePlugin(KMPXPPluginHandlerBaseUid);
     
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseReleasePluginTUid testing CMPXPluginHandlerBase::ReleasePlugin() end")));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseReleasePluginTUid testing CMPXPluginHandlerBase::ReleasePlugin() end"));
          
     return err;
     }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHandlerBaseCreate()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------

TInt CCommonTestClass::MpxPluginHandlerBaseCreate()
    {
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseCreate testing CMPXPluginHandlerBase::CMPXPluginHandlerBase() begin")));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseCreate testing CMPXPluginHandlerBase::CMPXPluginHandlerBase() begin"));
     TInt err = KErrNone;
     
     CDummyMpxPluginHandler* iPluginHandler=new CDummyMpxPluginHandler();  
     
     if ( iPluginHandler == NULL)
             {
             err = KErrNotFound;
             }
     
     delete iPluginHandler;
     FTRACE(FPrint(_L("CCommonTestClass::MpxPluginHandlerBaseCreate testing CMPXPluginHandlerBase::CMPXPluginHandlerBase() end err=%d"),err));
     iLog->Log(_L("CCommonTestClass::MpxPluginHandlerBaseCreate testing CMPXPluginHandlerBase::CMPXPluginHandlerBase() end err=%d"),err);
     return err;
     }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHBBasePluginsChangedL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHBBasePluginsChangedL(CStifItemParser& /*aItem*/)
     {
     FTRACE( FPrint( _L("Test CMPXPluginHandlerBase::PluginsChangedL(),begins...") ) );
     iLog->Log( _L("Test CMPXPluginHandlerBase::PluginsChangedL(),begins...") );
     
     TInt err = KErrNone;
     
     CDummyMpxPluginHandler pluginHandler;
     //pluginHandler.BasePluginsChangedL();
     TRAP( err,pluginHandler.BasePluginsChangedL() );
     if( err )
    	 {
    	 iLog->Log( _L("CMPXPluginHandlerBase::PluginsChangedL() returned error %d."),err );
    	 }
     else
    	 {
    	 iLog->Log( _L("CMPXPluginHandlerBase::PluginsChangedL() returned no error.") );
    	 }
     
     FTRACE( FPrint( _L("Test CMPXPluginHandlerBase::PluginsChangedL(),end.") ) );
     iLog->Log( _L("Test CMPXPluginHandlerBase::PluginsChangedL(),end.") );
          
     return err;
     }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHBBaseIsPluginLoaded()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHBBaseIsPluginLoaded(CStifItemParser& /*aItem*/)
     {
     FTRACE( FPrint( _L("Test CMPXPluginHandlerBase::IsPluginLoaded(),begins...") ) );
     iLog->Log( _L("Test CMPXPluginHandlerBase::IsPluginLoaded(),begins...") );
     
     TInt err = KErrNone;
     TUid pluginUid( KNullUid );
     
     CDummyMpxPluginHandler pluginHandler;
     TBool isLoaded = pluginHandler.BaseIsPluginLoaded( pluginUid );
     if( isLoaded )
    	 {
    	 iLog->Log( _L("The relative plugin is loaded.") );
    	 }
     else
    	 {
    	 iLog->Log( _L("The relative plugin is not loaded.") );
    	 }
     
     FTRACE( FPrint( _L("Test CMPXPluginHandlerBase::IsPluginLoaded(),end.") ) );
     iLog->Log( _L("Test CMPXPluginHandlerBase::IsPluginLoaded(),end.") );
          
     return err;
     }
// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHBBaseLoadPluginL()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHBBaseLoadPluginL(CStifItemParser& /*aItem*/)
     {
     FTRACE( FPrint( _L("Test CMPXPluginHandlerBase::LoadPluginL(),begins...") ) );
     iLog->Log( _L("Test CMPXPluginHandlerBase::LoadPluginL(),begins...") );
     
     TInt err = KErrNone;
     TUid pluginUid( KNullUid );
     
     CDummyMpxPluginHandler pluginHandler;
     //pluginHandler.BasePluginsChangedL( pluginUid );
     TRAP( err,pluginHandler.BaseLoadPluginL( pluginUid ) );
     if( err )
    	 {
    	 iLog->Log( _L("CMPXPluginHandlerBase::LoadPluginL() returned error %d."),err );
    	 }
     else
    	 {
    	 iLog->Log( _L("CMPXPluginHandlerBase::LoadPluginL() returned no error.") );
    	 }
     
     FTRACE( FPrint( _L("Test CMPXPluginHandlerBase::LoadPluginL(),end.") ) );
     iLog->Log( _L("Test CMPXPluginHandlerBase::LoadPluginL(),end.") );
          
     return err;
     }

// -----------------------------------------------------------------------------
// CCommonTestClass::MpxPluginHBBaseHandlePluginUnload()
// Returns: Symbian OS errors.
// -----------------------------------------------------------------------------
TInt CCommonTestClass::MpxPluginHBBaseHandlePluginUnload(CStifItemParser& /*aItem*/)
     {
     FTRACE( FPrint( _L("Test CMPXPluginHandlerBase::HandlePluginUnload(),begins...") ) );
     iLog->Log( _L("Test CMPXPluginHandlerBase::HandlePluginUnload(),begins...") );
     
     TInt err = KErrNone;
     TUid pluginUid( KNullUid );
     
     CDummyMpxPluginHandler pluginHandler;
     //pluginHandler.BaseHandlePluginUnload( pluginUid );
     TRAP( err,pluginHandler.BaseHandlePluginUnload( pluginUid ) );
     if( err )
    	 {
    	 iLog->Log( _L("CMPXPluginHandlerBase::HandlePluginUnload() returned error %d."),err );
    	 }
     else
    	 {
    	 iLog->Log( _L("CMPXPluginHandlerBase::HandlePluginUnload() returned no error.") );
    	 }
     
     FTRACE( FPrint( _L("Test CMPXPluginHandlerBase::HandlePluginUnload(),end.") ) );
     iLog->Log( _L("Test CMPXPluginHandlerBase::HandlePluginUnload(),end.") );
          
     return err;
     }





//end of MPXPluginHandlerBase testing implementation (mpxpluginhandlerbase.h)===========================

