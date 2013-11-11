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
* Description:  xadebugtraceutility header
*
*/

#ifndef XADEBUGTRACEUTILITY_H
#define XADEBUGTRACEUTILITY_H

/* Functions to be used by c files */
#ifdef _cplusplus
extern "C" {
#endif

void xa_print(const char* formatString);
void xa_print_1(const char* formatString, int a1);
void xa_print_1_str(const char* formatString, const char* a1);
void xa_print_2(const char* formatString, int a1, int a2);
void xa_print_2_str(const char* formatString, const char* a1, const char* a2);
void xa_print_3(const char* formatString, int a1, int a2, int a3);
void xa_print_4(const char* formatString, int a1, int a2, int a3, int a4);
void xa_heap_total_alloc_print(const char* formatString);
#ifdef _cplusplus
}
#endif

/* Functions to be used by cpp files */
void xa_cppprint(const char* formatString);
void xa_cppprint_1(const char* formatString, int a1);
void xa_cppprint_2(const char* formatString, int a1, int a2);

#endif /* XADEBUGTRACEUTILITY_H */
