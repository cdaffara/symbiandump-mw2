// Copyright (c) 2002-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// This file contains the declaration of the CPushAppHandlerEx which is an example 
// WAP Push plugin
// 
//

/**
 @file PushAppHandlerEx.h
*/

#ifndef __PUSHAPPHANDEREX_H__
#define __PUSHAPPHANDEREX_H__


/** All Push Messages have an Application-ID header.
*   Registered values are on the Wapforum web site: http://www.wapforum.com/wina/
*   It can be found on that site under 'WAG' 'Push Application ID'
*
*   This example uses a non-registered value as an example.  Change this for your
*   real push plugin.
*
*   This plugin is registered for this via the ECOM resource mechanism
*   Look in the XXXXX.rss file 
*	The values in both the text and numeric form is in the default_data section.
*   
*   This file conatains the Example WAP Push Application Handler Plugin
*	
*/


#include <e32base.h>
#include <push/cpushhandlerbase.h>


/** 
*	The CPushAppHandlerEx is a WAP Push Plugin.  It registers for a nonsense push
*   push message. It is invoked to do its work in when HandleMessage() API is 
*   called by the Push Watcher framework.
*	As a WAP Push Plugin, it must destroy itself when it is complete. The Watcher
*   does not hold a pointer to it, it is invoked and responsible for destoying 
*   itself.
*   
*	When invoked, the Push Message will contain a CPushMessage.  It can do anything
*   with the message such as parsing, storing, passing it to another application, etc.
*
*	In this example, it will parse the message, then destroy it.
*
*/ 
class CPushAppHandlerEx : public CPushHandlerBase
	{
public:	// Methods

	/**
	 * Intended Usage : Static factory constructor called by the Wap Push framework
	 * @return			Pointer to a fully instantiated CPushAppHandlerEx class
	 */
	static CPushAppHandlerEx* NewL();
	
	/**
	 * Intended Usage : Default Destructor
	 */
	virtual ~CPushAppHandlerEx();

public:	// Methods from CPushHandlerBase

	/**
	 * Intended Usage :Called by the Wap Push Framework to handle the message.
	 *					Asynchronous version
	 * @param aPushMsg The push message delivered from the framework containing
	 *	               the Push Message.  Ownership is taken over.
	 * @param aStatus  when this function is complete, this status is returned 
	 *				   with acompletion code.
	 */
	void HandleMessageL(CPushMessage* aPushMsg, TRequestStatus& aStatus);

	/**
	 * Intended Usage : Called by the Wap Push Framework to handle the message.
	 *					Synchronous version
	 * @param aPushMsg The push message delivered from the framework containing
	 *	               the Push Message. Ownership is taken over.
	 */
	void HandleMessageL(CPushMessage* aPushMsg);

	/**
	 * Intended Usage :Called by the Wap Push Framework to cancel outstanding 
	 *	               events.
	 */
	void CancelHandleMessage();

	/**
	 * Intended Usage :Reserved for ECOM for future expansion
	 */
	virtual void CPushHandlerBase_Reserved1();

	/**
	 * Intended Usage :Reserved for ECOM for future expansion
	 */
	virtual void CPushHandlerBase_Reserved2();

private:	// Methods from CActive

	/**
	 * Intended Usage :Inherited from CActive - called to cancel outanding events
	 */
	void DoCancel();

	/**
	 * Intended Usage :Inherited from CActive - called when object is active
	 */
	void RunL();


private:	// Methods

	/**
	 * Intended Usage :Default constructor
	 */
	CPushAppHandlerEx();

	/**
	 * Intended Usage :2nd phase constuctor for instantiating member variables
	 */
	void ConstructL();


	/**
	 * Intended Usage :Completes the iRequestStatus with KErrNone 
	 */
	void IdleComplete();

	/**
	 * @param			aError - indicate to function an error
	 * Intended Usage :Kill Self 
	 */
	void Done(TInt aError);

	/**
	 * Intended Usage :Demonstrate parsing a push message 
	 */
	void  ParsePushMsgL();
	
	/**
	 * Intended Usage :Demonstrate doing something with a push message 
	 */
	void  ProcessPushMsgL();

private:	// Attributes
	/**
	 * State machine to demonstate plugin
	 */
	typedef enum TState { EParsing, EProcessing, EDone };  
	TState iState;

	/**
	 * Push Message passed in
	 */
	CPushMessage*	iMessage;

	/**
	* Flag to indicate if this was called asynchonously
	*/
	TBool			iAsyncHandling;

	};

#endif	// __PUSHAPPHANDEREX_H__