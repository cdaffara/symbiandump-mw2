// Copyright (c) 1998-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
//

// Standard includes
#include <coemain.h>	// CCoeEnv
#include <bautils.h>	// BaflUtils
#include <fbs.h>
#include "MSVUTILS.H"
 
// Specific includes
#include "MTUDCBAS.H"
#include "MTUDPAN.H"
#include "MTUD.HRH"

// Constants
const TInt KBitmapArrayGranularity=		18;
const TInt KFunctionArrayGranularity=	18;


//
// TMtmUiFunction - Function description class //
//

EXPORT_C CBaseMtmUiData::TMtmUiFunction::TMtmUiFunction(const TMtmUiFunction& aFunc)
	:	iCaption(aFunc.iCaption), 
		iPreferredHotKeyKeyCode(aFunc.iPreferredHotKeyKeyCode), 
		iPreferredHotKeyModifiers(aFunc.iPreferredHotKeyModifiers), 
		iFunctionId(aFunc.iFunctionId), 
		iFunctionType(aFunc.iFunctionType),
		iFlags(aFunc.iFlags)
	/** Copy constructor.
	
	Creates a TMtmUiFunction by copying another TMtmUiFunction. Its action is the same as the 
	default copy constructor, except that it will panic in debug builds if multiple 
	location flags are set in aFunc.
	@param aFunc TMtmUiFunction to copy 
	*/
	{
	__ASSERT_DEBUG( // check that only one location flag is set
		 (!!(iFlags&EMtudCommandTransferSend)
		+ !!(iFlags&EMtudCommandTransferReceive)
		+ !!(iFlags&EMtudCommandSendAs))==1,
		Panic(EMtudMoreThanOneLocationInFuntionFlags));
	}

EXPORT_C CBaseMtmUiData::TMtmUiFunction::TMtmUiFunction(TResourceReader& aResourceReader)
/** Creates a TMtmUiFunction with its values read from the resource file location 
specified by TResourceReader. This position should be at a MTUD_FUNCTION resource.

@param aResourceReader Position in UI Data MTM resource file to read operation 
*/
	{
	iCaption=aResourceReader.ReadTPtrC();
	iFunctionId=aResourceReader.ReadInt32();
	TChar chr(aResourceReader.ReadUint32());
	chr.LowerCase();
	iPreferredHotKeyKeyCode=chr;
	SetModifiersFromResourceDefinition(aResourceReader.ReadInt8());
	iFlags=aResourceReader.ReadInt32();
	iFunctionType=(TUint8)aResourceReader.ReadUint8();
	}

void CBaseMtmUiData::TMtmUiFunction::SetModifiersFromResourceDefinition(TInt aResourceFileModifiers)
	{
	iPreferredHotKeyModifiers=0;
	if (aResourceFileModifiers & EMtudShift)
		iPreferredHotKeyModifiers |= EModifierShift;
	if (aResourceFileModifiers & EMtudCtrl)
		iPreferredHotKeyModifiers |= EModifierCtrl;
	}

//
// CBaseMtmUiCustomisation - MTM Ui customisation base API   //
//

EXPORT_C CBaseMtmUiData::~CBaseMtmUiData()
/** Destructor.

This cleans up the base class. CBaseMtmUiData-derived objects must be deleted by 
client applications when they are no longer required. Clean up includes:

1. cleaning up the iIconArrays and iMtmSpecificFunctions

2. unloading the UI Data MTM resource file

Derived classes can implement a destructor to do any additional clean up tasks 
that they require. */
	{
	if (iIconArrays != NULL)
		{
		for (TInt bb=0; bb < iIconArrays->Count(); ++bb)
			iIconArrays->At(bb)->ResetAndDestroy();
		iIconArrays->ResetAndDestroy();
		delete iIconArrays;
		}

	delete iMtmSpecificFunctions;
	iCoeEnv->DeleteResourceFile(iResourceFileOffset);

	iRegisteredMtmDll.ReleaseLibrary();
	}

EXPORT_C TUid CBaseMtmUiData::Type() const
/** Gets the Type UID of the message type associated with the UI Data MTM.

@return UID of the message type associated with the MTM */
	{ 
	return iRegisteredMtmDll.MtmTypeUid();
	}

EXPORT_C const CArrayFix<CBaseMtmUiData::TMtmUiFunction>& CBaseMtmUiData::MtmSpecificFunctions() const
/** Gets an array of MTM-specific operations that can be carried out through the 
associated User Interface MTM's CBaseMtmUi::InvokeSyncFunctionL()/InvokeAsyncFunctionL() 
functions.

It is intended that message client applications will use this function to 
add dynamically user interface features, such as menu items, dependent on 
the MTM type of the entry currently selected within the application. Message 
client applications can also use OperationSupportedL() to check dynamically 
if a particular operation is appropriate to a particular entry.

Implementers should note that this function returns a pointer to the iMtmSpecificFunctions 
data member.

@return Array of operations relevant to the MTM and context. */
	{ 
	return *iMtmSpecificFunctions; 
	}

EXPORT_C TBool CBaseMtmUiData::FunctionRecognised(TInt aFunctionId) const
/** Tests if an operation with the specified ID is recognised by the UI Data MTM.

Implementers should note that this function searches the iMtmSpecificFunctions 
array for the aFunctionId.

@param aFunctionId The operation to check 
@return ETrue: operation aFunctionId is supported by this UI Data MTM. EFalse: 
operation aFunctionId is not supported by this UI Data MTM. */
	{
	TInt count=iMtmSpecificFunctions->Count();
	for (TInt cc=0; cc < count ; ++cc)
		{
		if (iMtmSpecificFunctions->At(cc).iFunctionId==aFunctionId)
			return ETrue;
		}
	return EFalse;
	}

//
// CBaseMtmUiCustomisation - (protected methods)   //
//

EXPORT_C CBaseMtmUiData::CBaseMtmUiData(CRegisteredMtmDll& aRegisteredMtmDll)
	:	iCoeEnv(CCoeEnv::Static()), iRegisteredMtmDll(aRegisteredMtmDll)
/** Constructor.

This creates a CBaseMtmUiData and initialises its private member variables. Client 
applications do not use this function. It is relevant only to implementers 
of derived classes.

Derived classes can implement a constructor to perform any additional MTM-specific 
setup that can be safely carried out in a constructor. Such constructors must 
call the base class constructor function.

@param aRegisteredMtmDll Registration data for the DLL. */
	{
	__DECLARE_NAME(_S("CBaseMtmUiData"));
	}

EXPORT_C void CBaseMtmUiData::ConstructL()
/** Second-phase constructor.

This implements two-phase construction functions (NewL(), ConstructL()) to create 
a new instance of the object. Client applications do not use this function. 
It is relevant only to implementers of derived classes.

The function should perform any necessary dynamic allocation. ConstructL() 
should be called from the NewL() function of the derived class.

The default implementation of this function initiates the loading of required 
resources: 

1. instantiates objects for the iIconArrays and iMtmSpecificFunctions protected 
members

2. loads the UI Data MTM's resource file, as specified by GetResourceFileName()

3. calls PopulateArraysL() so that the derived class can load resources

Derived classes can implement this function to perform any additional MTM-specific 
second stage construction tasks that they require. Implementations must call 
the base class ConstructL() function.

Concrete MTMs must also implement a factory function by which a MTM registry 
can request an instance of the class: 

@see MtmUiDataFactoryFunctionL */
	{
	iIconArrays = 
		new(ELeave) CArrayPtrSeg<CBitmapArray>(KBitmapArrayGranularity);
	iMtmSpecificFunctions = 
		new(ELeave) CArrayFixFlat<TMtmUiFunction>(KFunctionArrayGranularity);

	// Get resource file
	TFileName resourceFileName;
 	GetResourceFileName(resourceFileName);
	
	MsvUtils::AddPathAndExtensionToResFileL(resourceFileName);
 
	//Now look for the nearest filename--will search the system drive if specified
	BaflUtils::NearestLanguageFile(iCoeEnv->FsSession(), resourceFileName);

	TRAPD(err, iResourceFileOffset=iCoeEnv->AddResourceFileL(resourceFileName));
	User::LeaveIfError(err);
  
	PopulateArraysL();
	}

/** Used by TCleaupItem in CreateBitmapsL.

We check for NULL in case any funcs leave before the cleanup item
is invoked.

*/
LOCAL_C void DeleteCBitMapArray(TAny* aPtr)
	{
	if (aPtr) 
		{
        CBaseMtmUiData::CBitmapArray* array=reinterpret_cast<CBaseMtmUiData::CBitmapArray*>(aPtr);	
		array->ResetAndDestroy();
		delete array;
		}
	}

EXPORT_C void CBaseMtmUiData::CreateBitmapsL(TInt aNumZoomStates, const TDesC& aBitmapFile, TInt aStartBitmap, TInt aEndBitmap)
/** Fills the iIconArrays array with icon bitmaps from an mbm file. 

Client applications do not use this function. It is relevant only to implementers of derived classes.

The bitmaps to load must be contiguous within the file between the positions 
indicated by aStartBitmap and aEndBitmap.

The function is typically called from PopulateArraysL().

@param aNumZoomStates Number of zoom states for each icon. This is used as 
the array size of each element in iIconArrays. 
@param aBitmapFile Filename of .mbm format bitmap array file. 
@param aStartBitmap First bitmap within the file for this MTM. 
@param aEndBitmap Final bitmap within the file for this MTM. */
	{
	__ASSERT_DEBUG((aStartBitmap+aEndBitmap+1)%aNumZoomStates==0, Panic(EMtudIncorrectBitmapNumbers));
	TFileName fileName(aBitmapFile);
	_LIT(KMtmUiDataFileLocationV2, "z:\\resource\\messaging\\");
	TFindFile finder(iCoeEnv->FsSession());
	// V2
	User::LeaveIfError(finder.FindByDir(fileName, KMtmUiDataFileLocationV2));
	fileName = finder.File();

	BaflUtils::NearestLanguageFile(iCoeEnv->FsSession(), fileName);

	CBitmapArray* array=NULL;
	for (TInt bb=aStartBitmap; bb <= aEndBitmap; bb+=aNumZoomStates)
		{
		array=new(ELeave) CArrayPtrFlat<CFbsBitmap>(aNumZoomStates);
		CleanupStack::PushL(TCleanupItem(DeleteCBitMapArray, array));
		TInt cc=0;
		for (; cc < aNumZoomStates; ++cc)
			{
			CFbsBitmap* bmp=CreateBitmapL(fileName, bb+cc);
			CleanupStack::PushL(bmp);
			// warning: bmp is deleted by the array CleanupItem. Immediately Pop or risk double deletion upon a Leave.
			array->AppendL(bmp);
			CleanupStack::Pop(bmp); 
			}
		iIconArrays->AppendL(array);
		CleanupStack::Pop(array);
		}
	}

CFbsBitmap* CBaseMtmUiData::CreateBitmapL(const TDesC& aFileName, TInt aId) const
	{
    CWsBitmap* bitmap=new(ELeave) CWsBitmap(iCoeEnv->WsSession());
	TInt err=bitmap->Load(aFileName,aId);
	if (err)
		{
		delete(bitmap);
		User::Leave(err);
		}
	bitmap->SetSizeInTwips(iCoeEnv->ScreenDevice());
	return bitmap;
	}

EXPORT_C void CBaseMtmUiData::ReadFunctionsFromResourceFileL(TInt aResourceId)
/** Fills the iMtmSpecificFunctions array with MTM-specific operation definitions 
from the UI Data MTM's resource file. 

Client applications do not use this function. It is relevant only to 
implementers of derived classes.

For each MTM-specific operation definition in the resource file, it adds a 
corresponding TMtmUiFunction object to iMtmSpecificFunctions. 

The function is typically called from PopulateArraysL().

@param aResourceId Resource ID of MTM-specific operation definition array */
	{
	TResourceReader reader;
	iCoeEnv->CreateResourceReaderLC(reader, aResourceId);
	const TInt functionCount=reader.ReadInt16();
	for (TInt cc=0; cc < functionCount; ++cc)
		{
		TMtmUiFunction func(reader);
		iMtmSpecificFunctions->AppendL(func);
		}
	CleanupStack::PopAndDestroy();// reader
	}

EXPORT_C TBool CBaseMtmUiData::CanUnDeleteFromEntryL(const TMsvEntry& /*aContext*/, TInt& aReasonResourceId) const
/** Tests if the entry can be undeleted.

@return	KErrNone: the operation is appropriate on the entry. KErrNotSupported: 
the operation is not appropriate on the entry. 		
@param aContext The entry to which the operation applies. 
@param aReasonResourceId On return, a resource string ID or 0. 
*/
	{
	aReasonResourceId=0;
	return EFalse;
	}

/**
Returns a pointer to the interface with the specified Uid.

This method is the first part of an extension pattern to allow for 
more functionality to be supported without adding virtual methods 
to this base class.

The default implementation returns a NULL pointer.
 
@param	aUid  
Uid of the extension interface
@return
Pointer to the extension interface
*/
EXPORT_C TAny* CBaseMtmUiData::GetInterface(TUid /*aUid*/)
 	{
	return NULL;
 	}
