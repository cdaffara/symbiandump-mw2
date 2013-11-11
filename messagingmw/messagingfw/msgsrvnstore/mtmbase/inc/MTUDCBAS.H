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

#if !defined(__MTUDBAS_H__)
#define __MTUDBAS_H__

#if !defined(__MSVSTD_H__)
#include <msvstd.h>
#endif

#if !defined(__MSVREG_H__)
#include <msvreg.h>
#endif

#if !defined(__MTCLREG_H__)
#include <mtclreg.h>
#endif

#if !defined(__BARSREAD_H__)
#include <barsread.h>
#endif

// Forward references
class CFbsBitmap;
class CCoeEnv;

//
// CBaseMtmUiData - MTM Ui data layer base API   //
//
class CBaseMtmUiData : public CBase
/** Base class for UI Data MTM components. 

CBaseMtmUiData-derived classes allow message client applications to customise 
dynamically their user interfaces for a particular message type. For example, 
if a message client application currently had a fax entry selected, then it 
could use the fax MTM's concrete class to get information on the menus and 
icons that it should display. 

Specifically, functionality falls into the following three areas:

1. supplying MTM-specific icons for Message Server entries

2. supplying user interface text, e.g. for menus, for MTM-specific operations

3. providing checks that MTM functions are suitable for use on an entry. It is 
intended that client applications will use these functions to select dynamically 
the user interface features available dependent on the entry currently selected 
within the application. For example, menu items that do not apply to particular 
entries could be greyed out. Note that OperationSupportedL() performs a similar 
function for MTM-specific functions. Implementations should determine if the 
operation is supported by checking the properties of the entry, such as its 
type. If the operation is not appropriate on the entry, then the aReasonResourceId 
argument is set to the ID of a string in the UI Data MTM's resource file 
that explains why not. Otherwise, aReasonResourceId is 0.

Message client applications use the class to access such functionality polymorphically. 
MTM implementers implement this class to provide such functionality for their 
message protocol. 
@publishedAll
@released
*/
	{
public:
	// --- Types ---
	/** Defines an array of icon bitmaps for the UI Data MTM. 
	
	It is used in the definition of CBaseMtmUiData::iIconArrays. */
	typedef CArrayPtr<CFbsBitmap> CBitmapArray;
	//
	// --- Inline class ---
	class TMtmUiFunction
	/** Provides the interface for clients to access MTM-specific functions, and present 
	these functions to the user. A message client application can use this information to customise the UI.

	The class encapsulates information relating to an MTM-specific operation:

	1. human-readable name: for display on the UI

	2. ID: used to invoke the operation through the CBaseMtmUi::InvokeSyncFunctionL()/InvokeAsyncFunctionL() functions on the associated User Interface MTM 

	3. a suggested hot-key that the UI should support for the operation

	4. flags that describe further operation characteristics. 
@publishedAll
@released
*/
		{
		friend class CBaseMtmUiData;// For access to constructor
	public:
		// --- Constants ---
		/** Enum to define UI Data base class constants. */
		enum 
			{
			/** Maximum length for operation caption. Used in defining iCaption. */
			ECaptionMaxLen=40
			};

	public:
		// --- Construction ---
		IMPORT_C TMtmUiFunction(TResourceReader& aResourceReader);		
		IMPORT_C TMtmUiFunction(const TMtmUiFunction& aFunc);		

	private:
		// --- Private functions ---
		void SetModifiersFromResourceDefinition(TInt aResourceFileModifiers);		

	public:
		// --- Data ---
		/** String, suitable for a menu, describing the operation. */
		TBuf<ECaptionMaxLen>		iCaption;
		/** Character code for a suggested hotkey for the operation. */
		TInt						iPreferredHotKeyKeyCode;
		/** Suggested key modifiers for the hotkey.  */
		TInt						iPreferredHotKeyModifiers;
		/** The ID that can be used to invoke the operation. */
		TInt						iFunctionId;
		/** Flags setting location characteristics of the operation. */
		TUint8						iFunctionType;	// see MTUD.HRH
		/** Flags setting characteristics of the operation. */
		TInt						iFlags;			// see MTUD.HRH
		};
	//
	// --- Destruction ---
	IMPORT_C virtual ~CBaseMtmUiData();
	//
	// --- Accessor functions ---
	IMPORT_C TUid Type() const;
	//
	// --- Context icons ---
	/** Gets an array of bitmaps relevant to the passed context entry. 
	
	The bitmaps within the array should be representations of the same bitmap at various sizes. 
	
	The aStateFlags argument indicates the user interface state, for example, 
	"open", of the entry. 
	
	Requirements:
	
	Implementations should check the type of the context, using TMsvEntry::iType::iUid, 
	and the passed state, and return an appropriate bitmap array.
	
	@param aContext Context entry to return the icons for. 
	@param aStateFlags Flasgs indicating the user interface state of the entry. 
	@return Bitmap array for the given context.. */
	virtual const CBitmapArray& ContextIcon(const TMsvEntry& aContext, TInt aStateFlags) const = 0;
	//
	// --- MTM-Specific function information (e.g. for dynamically-added menu items) ---
	IMPORT_C const CArrayFix<TMtmUiFunction>& MtmSpecificFunctions() const;
	IMPORT_C TBool FunctionRecognised(TInt aFunctionId) const;
	//
	// --- Provide context-specific function information ---
	/** Tests if an entry can be created as a child.
	
	@param aParent The entry under which aNewEntry should be created. 
	@param aNewEntry The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanCreateEntryL(const TMsvEntry& aParent, TMsvEntry& aNewEntry, TInt& aReasonResourceId) const = 0;
	/** Tests if a delete operation is supported.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanDeleteFromEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests if the entry can be undeleted.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	IMPORT_C virtual TBool CanUnDeleteFromEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const;
	/** Tests if the service entry can be deleted.
	
	@param aService The service entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanDeleteServiceL(const TMsvEntry& aService, TInt& aReasonResourceId) const = 0;
	/** Tests if an entry can be replied to.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanReplyToEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests if an entry can be forwarded.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanForwardEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests if the entry can be edited.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanEditEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests if an entry can be viewed.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanViewEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests if an entry can be opened.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanOpenEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests if a close operation is supported.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID, or 0 if operation 
	is supported. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanCloseEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests if a copy from or move to operation is supported.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanCopyMoveToEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests whether a copy from or move from operation is supported.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID or 0. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanCopyMoveFromEntryL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	/** Tests if the current operation on the entry can be cancelled.
	
	@param aContext The entry to which the operation applies. 
	@param aReasonResourceId On return, a resource string ID, or 0 if operation 
	is supported. 
	@return ETrue, if the operation is appropriate on the entry; 
	EFalse, if the operation is NOT appropriate on the entry. */
	virtual TBool CanCancelL(const TMsvEntry& aContext, TInt& aReasonResourceId) const = 0;
	//
	// --- RTTI ---
	/** Tests if an MTM-specific operation is appropriate to the specified entry. The 
	operation specified must have been obtained from a call to MtmSpecificFunctions(). 
		
	It is intended that client applications will use this function to select dynamically 
	the MTM-specific user interface features available dependent on the entry 
	currently selected within the application.
	
	Requirements:
	
	Implementations should check the type of the context, using TMsvEntry::iType::iUid, 
	and use any other information required available from the context, to determine 
	if the operation is supported.
	
	@param aOperationId ID of operation to query. 
	@param aContext The entry to which the operation applies. 
	@return KErrNone: operation is appropriate to User Interface MTM and entry. 
	Other: if the operation is not appropriate on the entry, then this is the 
	ID of a string in the UI Data MTM's resource file that explains why not. */
	virtual TInt OperationSupportedL(TInt aOperationId, const TMsvEntry& aContext) const = 0; // returns 0 if operation *IS* supported, otherwise a resource ID for the reason text
	/** Queries if the MTM supports a particular capability, specified by a UID. 
	
	Requirements:
	
	Implementations should check aCapability for the standard capability values 
	and return a suitable response in aResponse. If aFunctionId is unknown, return 
	KErrNotSupported.
	
	@param aFunctionId UID of capability to be queried 
	@param aResponse Response value. The format of the response varies according 
	to the capability. 
	@return KErrNone: aFunctionId is a recognised value and a response is returned 
	KErrNotSupported: aFunctionId is not a recognised value */
	virtual TInt QueryCapability(TUid aFunctionId, TInt& aResponse) const = 0;
	/** Provides status text for the sending state of the entry.
	
	@param aContext Entry to query
	@return Descriptor containing status text. */
	virtual HBufC* StatusTextL(const TMsvEntry& aContext) const = 0;

protected:
	// --- Construction ---
	IMPORT_C CBaseMtmUiData(CRegisteredMtmDll& aRegisteredMtmDll);
	IMPORT_C void ConstructL();
	/** Initialises bitmaps and function information. 
	
	Client applications do not use this function. It is relevant only to implementers of derived classes.
	
	Requirements:
	
	This function is called by the base class ConstructL() function. Implementations 
	should provide this function to:
	
	1. insert MTM-specific icon bitmap arrays into the iIconArrays array data member 
	
	2. insert MTM-specific operation information into the iMtmSpecificFunctions array 
	data member */
	virtual void PopulateArraysL() = 0; // Should populate iCapabilities, iMtmSpecificFunctions and iIconArrays
	/** Gets the resource file name. 
	
	Client applications do not use this function. It is relevant only to implementers of derived classes.
	
	Requirements:
	
	Each UI Data MTM can have a resource file associated with it. Implementations 
	should set aFilename to the full path and name of this resource file. The 
	function is called by CBaseMtmUiData::ConstructL(), which then loads the resource 
	file.
	
	@param aFileName Filename buffer to be filled with the resource file path 
	and name */
	virtual void GetResourceFileName(TFileName& aFileName) const = 0;
	//
	// --- Utility ---
	IMPORT_C void ReadFunctionsFromResourceFileL(TInt aResourceId);// Up to clients to call this if necessary
	IMPORT_C void CreateBitmapsL(TInt aNumZoomStates, const TDesC& aBitmapFile, TInt aStartBitmap, TInt aEndBitmap);
	CFbsBitmap* CreateBitmapL(const TDesC& aFileName, TInt aId) const;

	// Method used for extension: called by non virtual methods that need 
	// to have a polymorphic behaviour.
	IMPORT_C virtual TAny* GetInterface(TUid aUid);

protected:
	// --- Data ---
	/** Control environment object. It is set by the constructor. */
	CCoeEnv*									iCoeEnv;
	/** The array that stores the arrays of icon bitmaps for the UI Data MTM. Items 
	are added to the array by PopulateArraysL(). */
	CArrayPtr<CBitmapArray>*					iIconArrays;
	/** The array that stores MTM-specific function information for the UI Data MTM. 
	Items are added to the array by ReadFunctionsFromResourceFileL(). */
	CArrayFix<TMtmUiFunction>*					iMtmSpecificFunctions;

private:
	// --- Data ---
	TInt										iResourceFileOffset;
	CRegisteredMtmDll&							iRegisteredMtmDll;  // Required in order to call ReleaseLibrary()

	// Extra data member to allow for future extensions
	TAny* iExtensionData;
	};


#endif // __MTUDBAS_H__
