/*****************************************************************************

  The following code is derived, directly or indirectly, from the SystemC
  source code Copyright (c) 1996-2006 by all Contributors.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.4 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_unsigned.h -- Arbitrary precision unsigned arithmetic.
 
    This file includes the definitions of sc_unsigned_bitref,
    sc_unsigned_subref, and sc_unsigned classes. The first two classes
    are proxy classes to reference one bit and a range of bits of a
    sc_unsigned number, respectively.

    An sc_signed number has the sign-magnitude representation
    internally. However, its interface guarantees a 2's-complement
    representation. The sign-magnitude representation is chosen
    because of its efficiency: The sc_signed and sc_unsigned types are
    optimized for arithmetic rather than bitwise operations. For
    arithmetic operations, the sign-magnitude representation performs
    better.

    It is also important to note that an sc_unsigned number with n
    bits is equivalent to an sc_signed non-negative number with n + 1
    bits.

    The implementations of sc_signed and sc_unsigned classes are
    almost identical: Most of the member and friend functions are
    defined in sc_nbcommon.cpp and sc_nbfriends.cpp so that they can
    be shared by both of these classes. These functions are chosed by
    defining a few macros before including them such as IF_SC_SIGNED
    and CLASS_TYPE. Our implementation choices are mostly dictated by
    performance considerations in that we tried to provide the most
    efficient sc_signed and sc_unsigned types without compromising
    their interface.

    For the behavior of operators, we have two semantics: the old and
    new. The most important difference between these two semantics is
    that the old semantics is closer to C/C++ semantics in that the
    result type of a binary operator on unsigned and signed arguments
    is unsigned; the new semantics, on the other hand, requires the
    result type be signed. The new semantics is required by the VSIA
    C/C++ data types standard. We have implemented the new semantics.

  Original Author: Ali Dasdan, Synopsys, Inc.
  
 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

  $Id: //dvt/mti/rel/10.0b/src/systemc/sc_unsigned.h#1 $

 *****************************************************************************/

#ifndef SC_UNSIGNED_H
#define SC_UNSIGNED_H


#include "sc_object.h"
#include "sc_value_base.h"
#include "sc_iostream.h"
#include "sc_length_param.h"
#include "sc_nbdefs.h"
#include "sc_nbutils.h"
#include "sc_nbexterns.h"
#include "sc_temporary.h"

// BEGIN MODELSIM ADDITIONS
#include "sc_mti_access.h"
// END MODELSIM ADDITIONS


namespace sc_dt
{

// classes defined in this module
class sc_unsigned_bitref_r;
class sc_unsigned_bitref;
class sc_unsigned_subref_r;
class sc_unsigned_subref;
class sc_concatref; 
class sc_unsigned;

// forward class declarations
class sc_bv_base;
class sc_lv_base;
class sc_int_base;
class sc_uint_base;
class sc_int_subref_r;
class sc_uint_subref_r;
class sc_signed;
class sc_signed_subref_r;
class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;

// Helper function declarions
int compare_unsigned(small_type us,
                              int unb,
                              int und,
                              const sc_digit *ud,
                              small_type vs,
                              int vnb,
                              int vnd,
                              const sc_digit *vd,
                              small_type if_u_signed,
                              small_type if_v_signed);

sc_unsigned add_unsigned_friend(small_type us,
                                         int unb,
                                         int und,
                                         const sc_digit *ud,
                                         small_type vs,
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);

sc_unsigned sub_unsigned_friend(small_type us,
                                         int unb,
                                         int und,
                                         const sc_digit *ud,
                                         small_type vs,
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);

sc_unsigned mul_unsigned_friend(small_type s,
                                         int unb,
                                         int und,
                                         const sc_digit *ud,
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);

sc_unsigned div_unsigned_friend(small_type s,
                                         int unb,
                                         int und,
                                         const sc_digit *ud,
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);

sc_unsigned mod_unsigned_friend(small_type us,
                                         int unb,
                                         int und,
                                         const sc_digit *ud,
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);

sc_unsigned and_unsigned_friend(small_type us,
                                         int unb,
                                         int und,
                                         const sc_digit *ud,
                                         small_type vs,
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);


sc_unsigned or_unsigned_friend(small_type us,
                                        int unb,
                                        int und,
                                        const sc_digit *ud,
                                        small_type vs,
                                        int vnb,
                                        int vnd,
                                        const sc_digit *vd);

sc_unsigned xor_unsigned_friend(small_type us,
                                         int unb,
                                         int und,
                                         const sc_digit *ud,
                                         small_type vs,
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);

// friend operator declarations
  // ARITHMETIC OPERATORS:

  // ADDition operators:

    sc_signed operator + (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator + (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator + (const sc_unsigned&  u, const sc_unsigned&  v);
    sc_signed operator + (const sc_unsigned&  u, int64               v);
  sc_unsigned operator + (const sc_unsigned&  u, uint64              v);
    sc_signed operator + (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator + (const sc_unsigned&  u, mtiUlongT       v);
    sc_signed operator + (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator + (const sc_unsigned&  u, unsigned int        v);

    sc_signed operator + (int64               u, const sc_unsigned&  v);
  sc_unsigned operator + (uint64              u, const sc_unsigned&  v);
    sc_signed operator + (mtiLongT u, const sc_unsigned&  v);
  sc_unsigned operator + (mtiUlongT u, const sc_unsigned&  v);
    sc_signed operator + (int                 u, const sc_unsigned&  v);
  inline sc_unsigned operator + (unsigned int        u, const sc_unsigned&  v);

  sc_unsigned operator + (const sc_unsigned&  u, const sc_uint_base& v);
    sc_signed operator + (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned operator + (const sc_uint_base& u, const sc_unsigned&  v);
    sc_signed operator + (const sc_int_base&  u, const sc_unsigned&  v);

  // SUBtraction operators:

    sc_signed operator - (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator - (const sc_signed&    u, const sc_unsigned&  v);

    sc_signed operator - (const sc_unsigned&  u, const sc_unsigned&  v);
    sc_signed operator - (const sc_unsigned&  u, int64               v);
    sc_signed operator - (const sc_unsigned&  u, uint64              v);
    sc_signed operator - (const sc_unsigned&  u, mtiLongT                v);
    sc_signed operator - (const sc_unsigned&  u, mtiUlongT       v);
    sc_signed operator - (const sc_unsigned&  u, int                 v);
    sc_signed operator - (const sc_unsigned&  u, unsigned int        v);

    sc_signed operator - (int64               u, const sc_unsigned&  v);
    sc_signed operator - (uint64              u, const sc_unsigned&  v);
    sc_signed operator - (mtiLongT u, const sc_unsigned&  v);
    sc_signed operator - (mtiUlongT u, const sc_unsigned&  v);
    sc_signed operator - (int                 u, const sc_unsigned&  v);
    sc_signed operator - (unsigned int        u, const sc_unsigned&  v);

    sc_signed operator - (const sc_unsigned&  u, const sc_uint_base& v);
    sc_signed operator - (const sc_unsigned&  u, const sc_int_base&  v);
    sc_signed operator - (const sc_uint_base& u, const sc_unsigned&  v);
    sc_signed operator - (const sc_int_base&  u, const sc_unsigned&  v);

  // MULtiplication operators:

    sc_signed operator * (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator * (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator * (const sc_unsigned&  u, const sc_unsigned&  v);
    sc_signed operator * (const sc_unsigned&  u, int64               v);
  sc_unsigned operator * (const sc_unsigned&  u, uint64              v);
    sc_signed operator * (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator * (const sc_unsigned&  u, mtiUlongT       v);
    sc_signed operator * (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator * (const sc_unsigned&  u, unsigned int        v);

    sc_signed operator * (int64               u, const sc_unsigned&  v);
  sc_unsigned operator * (uint64              u, const sc_unsigned&  v);
    sc_signed operator * (mtiLongT u, const sc_unsigned&  v);
  sc_unsigned operator * (mtiUlongT u, const sc_unsigned&  v);
    sc_signed operator * (int                 u, const sc_unsigned&  v);
  inline sc_unsigned operator * (unsigned int        u, const sc_unsigned&  v);

  sc_unsigned operator * (const sc_unsigned&  u, const sc_uint_base& v);
    sc_signed operator * (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned operator * (const sc_uint_base& u, const sc_unsigned&  v);
    sc_signed operator * (const sc_int_base&  u, const sc_unsigned&  v);

  // DIVision operators:

    sc_signed operator / (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator / (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator / (const sc_unsigned&  u, const sc_unsigned&  v);
    sc_signed operator / (const sc_unsigned&  u, int64               v);
  sc_unsigned operator / (const sc_unsigned&  u, uint64              v);
    sc_signed operator / (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator / (const sc_unsigned&  u, mtiUlongT       v);
    sc_signed operator / (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator / (const sc_unsigned&  u, unsigned int        v);

    sc_signed operator / (int64               u, const sc_unsigned&  v);
  sc_unsigned operator / (uint64              u, const sc_unsigned&  v);
    sc_signed operator / (mtiLongT u, const sc_unsigned&  v);
  sc_unsigned operator / (mtiUlongT u, const sc_unsigned&  v);
    sc_signed operator / (int                 u, const sc_unsigned&  v);
  inline sc_unsigned operator / (unsigned int        u, const sc_unsigned&  v);

  sc_unsigned operator / (const sc_unsigned&  u, const sc_uint_base& v);
    sc_signed operator / (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned operator / (const sc_uint_base& u, const sc_unsigned&  v);
    sc_signed operator / (const sc_int_base&  u, const sc_unsigned&  v);

  // MODulo operators:

    sc_signed operator % (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator % (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator % (const sc_unsigned&  u, const sc_unsigned&  v);
    sc_signed operator % (const sc_unsigned&  u, int64               v);
  sc_unsigned operator % (const sc_unsigned&  u, uint64              v);
    sc_signed operator % (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator % (const sc_unsigned&  u, mtiUlongT       v);
    sc_signed operator % (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator % (const sc_unsigned&  u, unsigned int        v);

    sc_signed operator % (int64               u, const sc_unsigned&  v);
  sc_unsigned operator % (uint64              u, const sc_unsigned&  v);
    sc_signed operator % (mtiLongT u, const sc_unsigned&  v);
  sc_unsigned operator % (mtiUlongT u, const sc_unsigned&  v);
    sc_signed operator % (int                 u, const sc_unsigned&  v);
  inline sc_unsigned operator % (unsigned int        u, const sc_unsigned&  v);

  sc_unsigned operator % (const sc_unsigned&  u, const sc_uint_base& v);
    sc_signed operator % (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned operator % (const sc_uint_base& u, const sc_unsigned&  v);
    sc_signed operator % (const sc_int_base&  u, const sc_unsigned&  v);

  // BITWISE OPERATORS:

  // Bitwise AND operators:

    sc_signed operator & (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator & (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator & (const sc_unsigned&  u, const sc_unsigned&  v);
    sc_signed operator & (const sc_unsigned&  u, int64               v);
  sc_unsigned operator & (const sc_unsigned&  u, uint64              v);
    sc_signed operator & (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator & (const sc_unsigned&  u, mtiUlongT       v);
    sc_signed operator & (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator & (const sc_unsigned&  u, unsigned int        v);

    sc_signed operator & (int64               u, const sc_unsigned&  v);
  sc_unsigned operator & (uint64              u, const sc_unsigned&  v);
    sc_signed operator & (mtiLongT u, const sc_unsigned&  v);
  sc_unsigned operator & (mtiUlongT u, const sc_unsigned&  v);
    sc_signed operator & (int                 u, const sc_unsigned&  v);
  inline sc_unsigned operator & (unsigned int        u, const sc_unsigned&  v);

  sc_unsigned operator & (const sc_unsigned&  u, const sc_uint_base& v);
    sc_signed operator & (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned operator & (const sc_uint_base& u, const sc_unsigned&  v);
    sc_signed operator & (const sc_int_base&  u, const sc_unsigned&  v);

  // Bitwise OR operators:

    sc_signed operator | (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator | (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator | (const sc_unsigned&  u, const sc_unsigned&  v);
    sc_signed operator | (const sc_unsigned&  u, int64               v);
  sc_unsigned operator | (const sc_unsigned&  u, uint64              v);
    sc_signed operator | (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator | (const sc_unsigned&  u, mtiUlongT       v);
    sc_signed operator | (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator | (const sc_unsigned&  u, unsigned int        v);

    sc_signed operator | (int64               u, const sc_unsigned&  v);
  sc_unsigned operator | (uint64              u, const sc_unsigned&  v);
    sc_signed operator | (mtiLongT u, const sc_unsigned&  v);
  sc_unsigned operator | (mtiUlongT u, const sc_unsigned&  v);
    sc_signed operator | (int                 u, const sc_unsigned&  v);
  inline sc_unsigned operator | (unsigned int        u, const sc_unsigned&  v);

  sc_unsigned operator | (const sc_unsigned&  u, const sc_uint_base& v);
    sc_signed operator | (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned operator | (const sc_uint_base& u, const sc_unsigned&  v);
    sc_signed operator | (const sc_int_base&  u, const sc_unsigned&  v);

  // Bitwise XOR operators:

    sc_signed operator ^ (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator ^ (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator ^ (const sc_unsigned&  u, const sc_unsigned&  v);
    sc_signed operator ^ (const sc_unsigned&  u, int64               v);
  sc_unsigned operator ^ (const sc_unsigned&  u, uint64              v);
    sc_signed operator ^ (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator ^ (const sc_unsigned&  u, mtiUlongT       v);
    sc_signed operator ^ (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator ^ (const sc_unsigned&  u, unsigned int        v);

    sc_signed operator ^ (int64               u, const sc_unsigned&  v);
  sc_unsigned operator ^ (uint64              u, const sc_unsigned&  v);
    sc_signed operator ^ (mtiLongT u, const sc_unsigned&  v);
  sc_unsigned operator ^ (mtiUlongT u, const sc_unsigned&  v);
    sc_signed operator ^ (int                 u, const sc_unsigned&  v);
  inline sc_unsigned operator ^ (unsigned int        u, const sc_unsigned&  v);

  sc_unsigned operator ^ (const sc_unsigned&  u, const sc_uint_base& v);
    sc_signed operator ^ (const sc_unsigned&  u, const sc_int_base&  v);
  sc_unsigned operator ^ (const sc_uint_base& u, const sc_unsigned&  v);
    sc_signed operator ^ (const sc_int_base&  u, const sc_unsigned&  v);

  // SHIFT OPERATORS:

  // LEFT SHIFT operators:

  sc_unsigned operator << (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator << (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator << (const sc_unsigned&  u, const sc_unsigned&  v);
  sc_unsigned operator << (const sc_unsigned&  u, int64               v);
  sc_unsigned operator << (const sc_unsigned&  u, uint64              v);
  sc_unsigned operator << (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator << (const sc_unsigned&  u, mtiUlongT       v);
  inline sc_unsigned operator << (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator << (const sc_unsigned&  u, unsigned int        v);

  sc_unsigned operator << (const sc_unsigned&  u, const sc_uint_base& v);
  sc_unsigned operator << (const sc_unsigned&  u, const sc_int_base&  v);

  // RIGHT SHIFT operators:

  sc_unsigned operator >> (const sc_unsigned&  u, const sc_signed&    v);
    sc_signed operator >> (const sc_signed&    u, const sc_unsigned&  v);

  sc_unsigned operator >> (const sc_unsigned&  u, const sc_unsigned&  v);
  sc_unsigned operator >> (const sc_unsigned&  u, int64               v);
  sc_unsigned operator >> (const sc_unsigned&  u, uint64              v);
  sc_unsigned operator >> (const sc_unsigned&  u, mtiLongT                v);
  sc_unsigned operator >> (const sc_unsigned&  u, mtiUlongT       v);
  inline sc_unsigned operator >> (const sc_unsigned&  u, int                 v);
  inline sc_unsigned operator >> (const sc_unsigned&  u, unsigned int        v);

  sc_unsigned operator >> ( const sc_unsigned& , const sc_uint_base& );
  sc_unsigned operator >> ( const sc_unsigned&, const sc_int_base& );

  // Unary arithmetic operators
  sc_unsigned operator + (const sc_unsigned& u);
  sc_signed operator - (const sc_unsigned& u);

  // LOGICAL OPERATORS:

  // Logical EQUAL operators:

  bool operator == (const sc_unsigned&  u, const sc_signed&    v);
  bool operator == (const sc_signed&    u, const sc_unsigned&  v);

  bool operator == (const sc_unsigned&  u, const sc_unsigned&  v);
  bool operator == (const sc_unsigned&  u, int64               v);
  bool operator == (const sc_unsigned&  u, uint64              v);
  bool operator == (const sc_unsigned&  u, mtiLongT                v);
  bool operator == (const sc_unsigned&  u, mtiUlongT       v);
  inline bool operator == (const sc_unsigned&  u, int                 v);
  inline bool operator == (const sc_unsigned&  u, unsigned int        v);

  bool operator == (int64               u, const sc_unsigned&  v);
  bool operator == (uint64              u, const sc_unsigned&  v);
  bool operator == (mtiLongT u, const sc_unsigned&  v);
  bool operator == (mtiUlongT u, const sc_unsigned&  v);
  inline bool operator == (int                 u, const sc_unsigned&  v);
  inline bool operator == (unsigned int        u, const sc_unsigned&  v) ;

  bool operator == (const sc_unsigned&  u, const sc_uint_base& v);
  bool operator == (const sc_unsigned&  u, const sc_int_base&  v);
  bool operator == (const sc_uint_base& u, const sc_unsigned&  v);
  bool operator == (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical NOT_EQUAL operators:

  bool operator != (const sc_unsigned&  u, const sc_signed&    v);
  bool operator != (const sc_signed&    u, const sc_unsigned&  v);

  bool operator != (const sc_unsigned&  u, const sc_unsigned&  v);
  bool operator != (const sc_unsigned&  u, int64               v);
  bool operator != (const sc_unsigned&  u, uint64              v);
  bool operator != (const sc_unsigned&  u, mtiLongT                v);
  bool operator != (const sc_unsigned&  u, mtiUlongT       v);
  inline bool operator != (const sc_unsigned&  u, int                 v);
  inline bool operator != (const sc_unsigned&  u, unsigned int        v);

  bool operator != (int64               u, const sc_unsigned&  v);
  bool operator != (uint64              u, const sc_unsigned&  v);
  bool operator != (mtiLongT u, const sc_unsigned&  v);
  bool operator != (mtiUlongT u, const sc_unsigned&  v);
  inline bool operator != (int                 u, const sc_unsigned&  v);
  inline bool operator != (unsigned int        u, const sc_unsigned&  v);

  bool operator != (const sc_unsigned&  u, const sc_uint_base& v);
  bool operator != (const sc_unsigned&  u, const sc_int_base&  v);
  bool operator != (const sc_uint_base& u, const sc_unsigned&  v);
  bool operator != (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical LESS_THAN operators:

  bool operator < (const sc_unsigned&  u, const sc_signed&    v);
  bool operator < (const sc_signed&    u, const sc_unsigned&  v);

  bool operator < (const sc_unsigned&  u, const sc_unsigned&  v);
  bool operator < (const sc_unsigned&  u, int64               v);
  bool operator < (const sc_unsigned&  u, uint64              v);
  bool operator < (const sc_unsigned&  u, mtiLongT                v);
  bool operator < (const sc_unsigned&  u, mtiUlongT       v);
  inline bool operator < (const sc_unsigned&  u, int                 v);
  inline bool operator < (const sc_unsigned&  u, unsigned int        v);

  bool operator < (int64               u, const sc_unsigned&  v);
  bool operator < (uint64              u, const sc_unsigned&  v);
  bool operator < (mtiLongT u, const sc_unsigned&  v);
  bool operator < (mtiUlongT u, const sc_unsigned&  v);
  inline bool operator < (int                 u, const sc_unsigned&  v);
  inline bool operator < (unsigned int        u, const sc_unsigned&  v);

  bool operator < (const sc_unsigned&  u, const sc_uint_base& v);
  bool operator < (const sc_unsigned&  u, const sc_int_base&  v);
  bool operator < (const sc_uint_base& u, const sc_unsigned&  v);
  bool operator < (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical LESS_THAN_AND_EQUAL operators:

  bool operator <= (const sc_unsigned&  u, const sc_signed&    v);
  bool operator <= (const sc_signed&    u, const sc_unsigned&  v);

  bool operator <= (const sc_unsigned&  u, const sc_unsigned&  v);
  bool operator <= (const sc_unsigned&  u, int64               v);
  bool operator <= (const sc_unsigned&  u, uint64              v);
  bool operator <= (const sc_unsigned&  u, mtiLongT                v);
  bool operator <= (const sc_unsigned&  u, mtiUlongT       v);
  inline bool operator <= (const sc_unsigned&  u, int                 v);
  inline bool operator <= (const sc_unsigned&  u, unsigned int        v);

  bool operator <= (int64               u, const sc_unsigned&  v);
  bool operator <= (uint64              u, const sc_unsigned&  v);
  bool operator <= (mtiLongT u, const sc_unsigned&  v);
  bool operator <= (mtiUlongT u, const sc_unsigned&  v);
  inline bool operator <= (int                 u, const sc_unsigned&  v);
  inline bool operator <= (unsigned int        u, const sc_unsigned&  v);

  bool operator <= (const sc_unsigned&  u, const sc_uint_base& v);
  bool operator <= (const sc_unsigned&  u, const sc_int_base&  v);
  bool operator <= (const sc_uint_base& u, const sc_unsigned&  v);
  bool operator <= (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical GREATER_THAN operators:

  bool operator > (const sc_unsigned&  u, const sc_signed&    v);
  bool operator > (const sc_signed&    u, const sc_unsigned&  v);

  bool operator > (const sc_unsigned&  u, const sc_unsigned&  v);
  bool operator > (const sc_unsigned&  u, int64               v);
  bool operator > (const sc_unsigned&  u, uint64              v);
  bool operator > (const sc_unsigned&  u, mtiLongT                v);
  bool operator > (const sc_unsigned&  u, mtiUlongT       v);
  inline bool operator > (const sc_unsigned&  u, int                 v);
  inline bool operator > (const sc_unsigned&  u, unsigned int        v);

  bool operator > (int64               u, const sc_unsigned&  v);
  bool operator > (uint64              u, const sc_unsigned&  v);
  bool operator > (mtiLongT u, const sc_unsigned&  v);
  bool operator > (mtiUlongT u, const sc_unsigned&  v);
  inline bool operator > (int                 u, const sc_unsigned&  v);
  inline bool operator > (unsigned int        u, const sc_unsigned&  v);

  bool operator > (const sc_unsigned&  u, const sc_uint_base& v);
  bool operator > (const sc_unsigned&  u, const sc_int_base&  v);
  bool operator > (const sc_uint_base& u, const sc_unsigned&  v);
  bool operator > (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical GREATER_THAN_AND_EQUAL operators:

  bool operator >= (const sc_unsigned&  u, const sc_signed&    v);
  bool operator >= (const sc_signed&    u, const sc_unsigned&  v);

  bool operator >= (const sc_unsigned&  u, const sc_unsigned&  v);
  bool operator >= (const sc_unsigned&  u, int64               v);
  bool operator >= (const sc_unsigned&  u, uint64              v);
  bool operator >= (const sc_unsigned&  u, mtiLongT                v);
  bool operator >= (const sc_unsigned&  u, mtiUlongT       v);
  inline bool operator >= (const sc_unsigned&  u, int                 v);
  inline bool operator >= (const sc_unsigned&  u, unsigned int        v);

  bool operator >= (int64               u, const sc_unsigned&  v);
  bool operator >= (uint64              u, const sc_unsigned&  v);
  bool operator >= (mtiLongT u, const sc_unsigned&  v);
  bool operator >= (mtiUlongT u, const sc_unsigned&  v);
  inline bool operator >= (int                 u, const sc_unsigned&  v);
  inline bool operator >= (unsigned int        u, const sc_unsigned&  v);

  bool operator >= (const sc_unsigned&  u, const sc_uint_base& v);
  bool operator >= (const sc_unsigned&  u, const sc_int_base&  v);
  bool operator >= (const sc_uint_base& u, const sc_unsigned&  v);
  bool operator >= (const sc_int_base&  u, const sc_unsigned&  v);

  // Bitwise NOT operator (unary).
  sc_unsigned operator ~ (const sc_unsigned& u);


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref_r
//
//  Proxy class for sc_unsigned bit selection (r-value only).
// ----------------------------------------------------------------------------

class sc_unsigned_bitref_r : public sc_value_base
{
    friend class sc_unsigned;

protected:

    // constructor and initialization:

    sc_unsigned_bitref_r()
	: m_index(0), m_obj_p(0)
		{}

	void initialize( const sc_unsigned* obj_p, int index_ )
	{
		m_obj_p = CCAST<sc_unsigned*>( obj_p );
		m_index = index_;
	}

public:

    // destructor

    virtual ~sc_unsigned_bitref_r()
	{}

    // copy constructor

    sc_unsigned_bitref_r( const sc_unsigned_bitref_r& a )
	:  m_index( a.m_index ), m_obj_p( a.m_obj_p )
	{}

    // capacity

    int length() const
	{ return 1; }


    // implicit conversion to uint64

    operator uint64 () const;
    bool operator ! () const;
    bool operator ~ () const;


    // explicit conversions

    uint64 value() const
	{ return operator uint64(); }

    bool to_bool() const
	{ return operator uint64(); }


    // concatenation support

    virtual int concat_length(bool* xz_present_p) const
        { if ( xz_present_p ) *xz_present_p = false; return 1; }
    virtual uint64 concat_get_uint64() const 
        { return (uint64)operator uint64(); }
    virtual bool concat_get_ctrl( sc_digit* dst_p, int low_i ) const 
        {
            int  bit_mask = 1 << (low_i % BITS_PER_DIGIT);  
            int  word_i = low_i / BITS_PER_DIGIT;
	    dst_p[word_i] &= ~bit_mask;
	    return false;
        }
    virtual bool concat_get_data( sc_digit* dst_p, int low_i ) const 
        {
            int  bit_mask = 1 << (low_i % BITS_PER_DIGIT);  
	    bool result;	// True if non-zero.
            int  word_i = low_i / BITS_PER_DIGIT;
            if ( operator uint64() )
	    {
                dst_p[word_i] |= bit_mask;
		result = true;
	    }
            else
	    {
                dst_p[word_i] &= ~bit_mask;
		result = false;
	    }
	    return result;
        }

    // other methods

    void print( ::std::ostream& os = ::std::cout ) const
	{ os << to_bool(); }

protected:

    int          m_index;
    sc_unsigned* m_obj_p;

private:

    // disabled
    const sc_unsigned_bitref_r& operator = ( const sc_unsigned_bitref_r& );
};



inline
::std::ostream&
operator << ( ::std::ostream&, const sc_unsigned_bitref_r& );


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref
//
//  Proxy class for sc_unsigned bit selection (r-value and l-value).
// ----------------------------------------------------------------------------

class sc_unsigned_bitref
    : public sc_unsigned_bitref_r
{
    friend class sc_unsigned;
	friend class sc_core::sc_vpool<sc_unsigned_bitref>;


protected: // construction

    sc_unsigned_bitref()
		{}

public:

    // copy constructor

    sc_unsigned_bitref( const sc_unsigned_bitref& a )
	: sc_unsigned_bitref_r( a )
	{}

    // assignment operators

    const sc_unsigned_bitref& operator = ( const sc_unsigned_bitref_r& );
    const sc_unsigned_bitref& operator = ( const sc_unsigned_bitref& );
    const sc_unsigned_bitref& operator = ( bool );

    const sc_unsigned_bitref& operator &= ( bool );
    const sc_unsigned_bitref& operator |= ( bool );
    const sc_unsigned_bitref& operator ^= ( bool );

    // concatenation methods

    virtual void concat_set(int64 src, int low_i);
    virtual void concat_set(const sc_signed& src, int low_i);
    virtual void concat_set(const sc_unsigned& src, int low_i);
    virtual void concat_set(uint64 src, int low_i);


    // other methods

    void scan( ::std::istream& is = ::std::cin );

protected:
	static sc_core::sc_vpool<sc_unsigned_bitref> m_pool;

};



inline
::std::istream&
operator >> ( ::std::istream&, sc_unsigned_bitref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref_r
//
//  Proxy class for sc_unsigned part selection (r-value only).
// ----------------------------------------------------------------------------

class sc_unsigned_subref_r : public sc_value_base
{
	friend class sc_signed;
    friend class sc_unsigned;
    friend class sc_unsigned_signal;

protected:

    // constructor

    sc_unsigned_subref_r()
	: m_left(0), m_obj_p(0), m_right(0)
	{}

	void initialize( const sc_unsigned* obj_p, int left_, int right_ )
	{
		m_obj_p = CCAST<sc_unsigned*>( obj_p );
		m_left = left_;
		m_right = right_;
	}

  
public:

    // destructor

    virtual ~sc_unsigned_subref_r()
	{}


    // copy constructor

    sc_unsigned_subref_r( const sc_unsigned_subref_r& a )
	: m_left( a.m_left ), m_obj_p( a.m_obj_p ), m_right( a.m_right )
	{}

    // capacity

    int length() const
	{ return m_left >= m_right ? (m_left-m_right+1) : (m_right-m_left+1 ); }


    // implicit conversion to sc_unsigned

    operator sc_unsigned () const;


    // explicit conversions

    int           to_int() const;
    unsigned int  to_uint() const;
    mtiLongT          to_long() const;
    mtiUlongT to_ulong() const;
    int64         to_int64() const;
    uint64        to_uint64() const;
    double        to_double() const;


    // explicit conversion to character string

    const std::string to_string( sc_numrep numrep = SC_DEC ) const;
    const std::string to_string( sc_numrep numrep, bool w_prefix ) const;


    // concatenation support

    virtual int concat_length(bool* xz_present_p) const
	{ 
	    if ( xz_present_p ) *xz_present_p = false; 
	    return m_left - m_right + 1; 
	}
    virtual uint64 concat_get_uint64() const;
    virtual bool concat_get_ctrl( sc_digit* dst_p, int low_i ) const;
    virtual bool concat_get_data( sc_digit* dst_p, int low_i ) const;

    // reduce methods

    bool and_reduce() const;
    bool nand_reduce() const;
    bool or_reduce() const;
    bool nor_reduce() const;
    bool xor_reduce() const ;
    bool xnor_reduce() const;

    // other methods

    void print( ::std::ostream& os = ::std::cout ) const
	{ os << to_string(sc_io_base(os, SC_DEC),sc_io_show_base(os)); }

protected:

    int          m_left;   // Left-most bit in this part selection.
    sc_unsigned* m_obj_p;  // Target of this part selection. 
    int          m_right;  // Right-most bit in this part selection.

private:

    // disabled
    const sc_unsigned_subref_r& operator = ( const sc_unsigned_subref_r& );
};



inline
::std::ostream&
operator << ( ::std::ostream&, const sc_unsigned_subref_r& );


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref
//
//  Proxy class for sc_unsigned part selection (r-value and l-value).
// ----------------------------------------------------------------------------

class sc_unsigned_subref
    : public sc_unsigned_subref_r
{
    friend class sc_unsigned;
	friend class sc_core::sc_vpool<sc_unsigned_subref>;


    // constructor

protected:
    sc_unsigned_subref()
	{}
  
public:

    // copy constructor

    sc_unsigned_subref( const sc_unsigned_subref& a )
	: sc_unsigned_subref_r( a )
	{}


    // assignment operators

    const sc_unsigned_subref& operator = ( const sc_unsigned_subref_r& a );
    const sc_unsigned_subref& operator = ( const sc_unsigned_subref& a );
    const sc_unsigned_subref& operator = ( const sc_unsigned& a );

	// START MODELSIM MODIFICATIONS

	//
	// Put this function definition outside the sc_signed_subref
	// class definition for HP aCC compiler.
	//
    template<class T>
    const sc_unsigned_subref& operator = ( const sc_generic_base<T>& a );

	// END MODELSIM MODIFICATIONS

    const sc_unsigned_subref& operator = ( const sc_signed_subref_r& a );
    const sc_unsigned_subref& operator = ( const sc_signed& a );

    const sc_unsigned_subref& operator = ( const char* a );
    const sc_unsigned_subref& operator = (mtiUlongT a );
    const sc_unsigned_subref& operator = (mtiLongT a );

    const sc_unsigned_subref& operator = ( unsigned int a )
	{ return operator = ( (mtiUlongT) a ); }

    const sc_unsigned_subref& operator = ( int a )
	{ return operator = ( (mtiLongT) a ); }

    const sc_unsigned_subref& operator = ( uint64 a );
    const sc_unsigned_subref& operator = ( int64 a );
    const sc_unsigned_subref& operator = ( double a );  
    const sc_unsigned_subref& operator = ( const sc_int_base& a );
    const sc_unsigned_subref& operator = ( const sc_uint_base& a );

    // concatenation methods

    virtual void concat_set(int64 src, int low_i);
    virtual void concat_set(const sc_signed& src, int low_i);
    virtual void concat_set(const sc_unsigned& src, int low_i);
    virtual void concat_set(uint64 src, int low_i);

    // other methods

    void scan( ::std::istream& is = ::std::cin );

protected:
	static sc_core::sc_vpool<sc_unsigned_subref> m_pool;

};



inline
::std::istream&
operator >> ( ::std::istream&, sc_unsigned_subref& );


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned
//
//  Arbitrary precision unsigned number.
// ----------------------------------------------------------------------------

class sc_unsigned : 
	public sc_mti_access,  // modelsim addition
	public sc_value_base
{
    friend class sc_concatref;
    friend class sc_unsigned_bitref_r;
    friend class sc_unsigned_bitref;
    friend class sc_unsigned_subref_r;
    friend class sc_unsigned_subref;
    friend class sc_signed;
    friend class sc_signed_subref;
	friend class sc_signed_subref_r;

  // Needed for types using sc_unsigned.
  typedef bool elemtype;

public:

    // constructors

    explicit sc_unsigned( int nb = sc_length_param().len() );
    sc_unsigned( const sc_unsigned& v );
    sc_unsigned( const sc_signed&   v );
	template<class T>
	explicit sc_unsigned( const sc_generic_base<T>& v );
	explicit sc_unsigned( const sc_bv_base& v );
	explicit sc_unsigned( const sc_lv_base& v );
	explicit sc_unsigned( const sc_int_subref_r& v );
	explicit sc_unsigned( const sc_uint_subref_r& v );
	explicit sc_unsigned( const sc_signed_subref_r& v );
	explicit sc_unsigned( const sc_unsigned_subref_r& v );


    // assignment operators

    const sc_unsigned& operator = (const sc_unsigned&        v);
    const sc_unsigned& operator = (const sc_unsigned_subref_r& a );

    template<class T>
    const sc_unsigned& operator = ( const sc_generic_base<T>& a )
        { a->to_sc_unsigned(*this); return *this; }

    const sc_unsigned& operator = (const sc_signed&          v);
    const sc_unsigned& operator = (const sc_signed_subref_r& a );

    const sc_unsigned& operator = ( const char*               v);
    const sc_unsigned& operator = ( int64                     v);
    const sc_unsigned& operator = ( uint64                    v);
    const sc_unsigned& operator = (mtiLongT v);
    const sc_unsigned& operator = (mtiUlongT v);

    const sc_unsigned& operator = ( int                       v) 
	{ return operator=((mtiLongT) v); }

    const sc_unsigned& operator = ( unsigned int              v) 
	{ return operator=((mtiUlongT) v); }

    const sc_unsigned& operator = ( double                    v);
    const sc_unsigned& operator = ( const sc_int_base&        v);
    const sc_unsigned& operator = ( const sc_uint_base&       v);

    const sc_unsigned& operator = ( const sc_bv_base& );
    const sc_unsigned& operator = ( const sc_lv_base& );

#ifdef SC_INCLUDE_FX
    const sc_unsigned& operator = ( const sc_fxval& );
    const sc_unsigned& operator = ( const sc_fxval_fast& );
    const sc_unsigned& operator = ( const sc_fxnum& );
    const sc_unsigned& operator = ( const sc_fxnum_fast& );
#endif


    // destructor

    virtual ~sc_unsigned()
	{
#           ifndef SC_MAX_NBITS
			if (!is_using_pool)
				delete [] digit;
#           endif

		// BEGIN MODELSIM ADDITIONS

		if (m_mti_value != NULL)
			delete[] m_mti_value;

		// END MODELSIM ADDITIONS
	}

    // Concatenation support:

	sc_digit* get_raw() const { return digit; }
    virtual int concat_length(bool* xz_present_p) const
       { if ( xz_present_p ) *xz_present_p = false; return nbits-1; }
    virtual bool concat_get_ctrl( sc_digit* dst_p, int low_i ) const;
    virtual bool concat_get_data( sc_digit* dst_p, int low_i ) const;
    virtual uint64 concat_get_uint64() const;
    virtual void concat_set(int64 src, int low_i);
    virtual void concat_set(const sc_signed& src, int low_i);
    virtual void concat_set(const sc_unsigned& src, int low_i);
    virtual void concat_set(uint64 src, int low_i);

    // Increment operators.

    sc_unsigned& operator ++ ();
    const sc_unsigned operator ++ (int);

    // Decrement operators.

    sc_unsigned& operator -- ();
    const sc_unsigned operator -- (int);


    // bit selection
	
	inline void check_index( int i ) const
		{ if ( (i < 0) || (i >= nbits-1) ) invalid_index(i); }

	void invalid_index( int i ) const;

    sc_unsigned_bitref& operator [] ( int i )
	{
		check_index(i);
		sc_unsigned_bitref* result_p =
			sc_unsigned_bitref::m_pool.allocate();
		result_p->initialize( this, i );
		return *result_p;
	}

		

    const sc_unsigned_bitref_r& operator [] ( int i ) const
	{
		check_index(i);
		sc_unsigned_bitref* result_p =
			sc_unsigned_bitref::m_pool.allocate();
		result_p->initialize( this, i );
		return *result_p;
	}

    sc_unsigned_bitref& bit( int i )
	{
		check_index(i);
		sc_unsigned_bitref* result_p =
			sc_unsigned_bitref::m_pool.allocate();
		result_p->initialize( this, i );
		return *result_p;
	}

    const sc_unsigned_bitref_r& bit( int i ) const
	{
		check_index(i);
		sc_unsigned_bitref* result_p =
			sc_unsigned_bitref::m_pool.allocate();
		result_p->initialize( this, i );
		return *result_p;
	}


    // part selection

    // Subref operators. Help access the range of bits from the ith to
    // jth. These indices have arbitrary precedence with respect to each
    // other, i.e., we can have i <= j or i > j. Note the equivalence
    // between range(i, j) and operator (i, j). Also note that
    // operator (i, i) returns an unsigned number that corresponds to the
    // bit operator [i], so these two forms are not the same.
	
	inline void check_range( int l, int r ) const
	{
		if ( l < r )
		{
			if ( (l < 0) || (r >= nbits-1) ) invalid_range(l,r);
		}
		else
		{
			if ( (r < 0) || (l >= nbits-1) ) invalid_range(l,r);
		}
	}

	void invalid_range( int l, int r ) const;


    sc_unsigned_subref& range( int i, int j )
	{
		check_range(i,j);
		sc_unsigned_subref* result_p =
			sc_unsigned_subref::m_pool.allocate();
		result_p->initialize( this, i, j );
		return *result_p;
	}


    const sc_unsigned_subref_r& range( int i, int j ) const
	{
		check_range(i,j);
		sc_unsigned_subref* result_p =
			sc_unsigned_subref::m_pool.allocate();
		result_p->initialize( this, i, j );
		return *result_p;
	}

    sc_unsigned_subref& operator () ( int i, int j ) 
	{
		check_range(i,j);
		sc_unsigned_subref* result_p =
			sc_unsigned_subref::m_pool.allocate();
		result_p->initialize( this, i, j );
		return *result_p;
	}

    const sc_unsigned_subref_r& operator () ( int i, int j ) const
	{
		check_range(i,j);
		sc_unsigned_subref* result_p =
			sc_unsigned_subref::m_pool.allocate();
		result_p->initialize( this, i, j );
		return *result_p;
	}


    // explicit conversions

    int           to_int() const;
    unsigned int  to_uint() const;
    mtiLongT          to_long() const;
    mtiUlongT to_ulong() const;
    int64         to_int64() const;
    uint64        to_uint64() const;
    double        to_double() const;

#ifdef SC_DT_DEPRECATED
    int to_signed() const
	{ return to_int(); }

    unsigned int to_unsigned() const
	{ return to_uint(); }
#endif

    // explicit conversion to character string

    const std::string to_string( sc_numrep numrep = SC_DEC ) const;
    const std::string to_string( sc_numrep numrep, bool w_prefix ) const;


    // Print functions. dump prints the internals of the class.

    void print( ::std::ostream& os = ::std::cout ) const
	{ os << to_string(sc_io_base(os,SC_DEC),sc_io_show_base(os)); }

    void scan( ::std::istream& is = ::std::cin );

    void dump( ::std::ostream& os = ::std::cout ) const;


  // Functions to find various properties.    
  int  length() const { return nbits - 1; }  // Bit width.
  bool iszero() const;                       // Is the number zero?
  bool sign() const { return 0; }            // Sign.

    // reduce methods

    bool and_reduce() const;

    bool nand_reduce() const
        { return ( ! and_reduce() ); }

    bool or_reduce() const;

    bool nor_reduce() const
        { return ( ! or_reduce() ); }

    bool xor_reduce() const;

    bool xnor_reduce() const  
        { return ( ! xor_reduce() ); }


  // Functions to access individual bits.  
  bool test(int i) const;      // Is the ith bit 0 or 1?
  void set(int i);             // Set the ith bit to 1.
  void clear(int i);           // Set the ith bit to 0.
  void set(int i, bool v)      // Set the ith bit to v.
    { if (v) set(i); else clear(i);  }
  void invert(int i)           // Negate the ith bit.
    { if (test(i)) clear(i); else set(i);  }

  // Make the number equal to its mirror image.
  void reverse();

  // Get/set a packed bit representation of the number.
  void get_packed_rep(sc_digit *buf) const;
  void set_packed_rep(sc_digit *buf);

  /*
    The comparison of the old and new semantics are as follows:

    Let s = sc_signed, 
        u = sc_unsigned,
        un = { uint64, mtiUlongT, unsigned int },
        sn = { int64, mtiLongT, int, char* }, and
        OP = { +, -, *, /, % }.

    Old semantics:                     New semantics:
      u OP u -> u                        u OP u -> u
      s OP u -> u                        s OP u -> s
      u OP s -> u                        u OP s -> s
      s OP s -> s                        s OP s -> s

      u OP un = un OP u -> u             u OP un = un OP u -> u
      u OP sn = sn OP u -> u             u OP sn = sn OP u -> s

      s OP un = un OP s -> s             s OP un = un OP s -> s
      s OP sn = sn OP s -> s             s OP sn = sn OP s -> s

    In the new semantics, the result is u if both operands are u; the
    result is s otherwise. The only exception is subtraction. The result
    of a subtraction is always s.

    The old semantics is like C/C++ semantics on integer types; the
    new semantics is due to the VSIA C/C++ data types standard.  
   */

  // ARITHMETIC OPERATORS:

  // ADDition operators:
   
  friend   sc_signed operator + (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator + (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator + (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator + (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator + (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator + (const sc_unsigned&  u, mtiUlongT       v);
  friend   sc_signed operator + (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator + (const sc_unsigned&  u, unsigned int        v) 
    { return operator+(u, (mtiUlongT) v); }

  friend   sc_signed operator + (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator + (mtiLongT u, const sc_unsigned&  v); 
  friend sc_unsigned operator + (mtiUlongT u, const sc_unsigned&  v);
  friend   sc_signed operator + (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator + (unsigned int        u, const sc_unsigned&  v)  
    { return operator+((mtiUlongT) u,  v); } 

  const sc_unsigned& operator += (const sc_signed&    v); 
  const sc_unsigned& operator += (const sc_unsigned&  v); 
  const sc_unsigned& operator += (int64               v); 
  const sc_unsigned& operator += (uint64              v); 
  const sc_unsigned& operator += (mtiLongT v); 
  const sc_unsigned& operator += (mtiUlongT v); 
  const sc_unsigned& operator += (int                 v) 
    { return operator+=((mtiLongT) v); }
  const sc_unsigned& operator += (unsigned int        v) 
    { return operator+=((mtiUlongT) v); }

  friend sc_unsigned operator + (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator + (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator + (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator + (const sc_int_base&  u, const sc_unsigned&  v);
  const sc_unsigned& operator += (const sc_int_base&  v);
  const sc_unsigned& operator += (const sc_uint_base& v);

  // SUBtraction operators:
   
  friend   sc_signed operator - (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator - (const sc_signed&    u, const sc_unsigned&  v); 

  friend   sc_signed operator - (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator - (const sc_unsigned&  u, int64               v); 
  friend   sc_signed operator - (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator - (const sc_unsigned&  u, mtiLongT                v); 
  friend   sc_signed operator - (const sc_unsigned&  u, mtiUlongT       v);
  friend   sc_signed operator - (const sc_unsigned&  u, int                 v); 
  friend   sc_signed operator - (const sc_unsigned&  u, unsigned int        v);

  friend   sc_signed operator - (int64               u, const sc_unsigned&  v); 
  friend   sc_signed operator - (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator - (mtiLongT u, const sc_unsigned&  v); 
  friend   sc_signed operator - (mtiUlongT u, const sc_unsigned&  v);
  friend   sc_signed operator - (int                 u, const sc_unsigned&  v);
  friend   sc_signed operator - (unsigned int        u, const sc_unsigned&  v); 

  const sc_unsigned& operator -= (const sc_signed&    v); 
  const sc_unsigned& operator -= (const sc_unsigned&  v); 
  const sc_unsigned& operator -= (int64               v); 
  const sc_unsigned& operator -= (uint64              v); 
  const sc_unsigned& operator -= (mtiLongT v); 
  const sc_unsigned& operator -= (mtiUlongT v); 
  const sc_unsigned& operator -= (int                 v) 
    { return operator-=((mtiLongT) v); }
  const sc_unsigned& operator -= (unsigned int        v) 
    { return operator-=((mtiUlongT) v); }

  friend   sc_signed operator - (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator - (const sc_unsigned&  u, const sc_int_base&  v);
  friend   sc_signed operator - (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator - (const sc_int_base&  u, const sc_unsigned&  v);
  const sc_unsigned& operator -= (const sc_int_base&  v);
  const sc_unsigned& operator -= (const sc_uint_base& v);

  // MULtiplication operators:
   
  friend   sc_signed operator * (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator * (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator * (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator * (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator * (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator * (const sc_unsigned&  u, mtiUlongT       v);
  friend   sc_signed operator * (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator * (const sc_unsigned&  u, unsigned int        v) 
    { return operator*(u, (mtiUlongT) v); }

  friend   sc_signed operator * (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator * (mtiLongT u, const sc_unsigned&  v); 
  friend sc_unsigned operator * (mtiUlongT u, const sc_unsigned&  v);
  friend   sc_signed operator * (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator * (unsigned int        u, const sc_unsigned&  v)  
    { return operator*((mtiUlongT) u,  v); } 

  const sc_unsigned& operator *= (const sc_signed&    v); 
  const sc_unsigned& operator *= (const sc_unsigned&  v); 
  const sc_unsigned& operator *= (int64               v); 
  const sc_unsigned& operator *= (uint64              v); 
  const sc_unsigned& operator *= (mtiLongT v); 
  const sc_unsigned& operator *= (mtiUlongT v); 
  const sc_unsigned& operator *= (int                 v) 
    { return operator*=((mtiLongT) v); }
  const sc_unsigned& operator *= (unsigned int        v) 
    { return operator*=((mtiUlongT) v); }

  friend sc_unsigned operator * (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator * (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator * (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator * (const sc_int_base&  u, const sc_unsigned&  v);
  const sc_unsigned& operator *= (const sc_int_base&  v);
  const sc_unsigned& operator *= (const sc_uint_base& v);

  // DIVision operators:
   
  friend   sc_signed operator / (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator / (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator / (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator / (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator / (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator / (const sc_unsigned&  u, mtiUlongT       v);
  friend   sc_signed operator / (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator / (const sc_unsigned&  u, unsigned int        v) 
    { return operator/(u, (mtiUlongT) v); }

  friend   sc_signed operator / (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator / (mtiLongT u, const sc_unsigned&  v); 
  friend sc_unsigned operator / (mtiUlongT u, const sc_unsigned&  v);
  friend   sc_signed operator / (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator / (unsigned int        u, const sc_unsigned&  v)  
    { return operator/((mtiUlongT) u,  v); } 

  const sc_unsigned& operator /= (const sc_signed&    v); 
  const sc_unsigned& operator /= (const sc_unsigned&  v); 
  const sc_unsigned& operator /= (int64               v); 
  const sc_unsigned& operator /= (uint64              v); 
  const sc_unsigned& operator /= (mtiLongT v); 
  const sc_unsigned& operator /= (mtiUlongT v); 
  const sc_unsigned& operator /= (int                 v) 
    { return operator/=((mtiLongT) v); }
  const sc_unsigned& operator /= (unsigned int        v) 
    { return operator/=((mtiUlongT) v); }

  friend sc_unsigned operator / (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator / (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator / (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator / (const sc_int_base&  u, const sc_unsigned&  v);
  const sc_unsigned& operator /= (const sc_int_base&  v);
  const sc_unsigned& operator /= (const sc_uint_base& v);

  // MODulo operators:
   
  friend   sc_signed operator % (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator % (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator % (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator % (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator % (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator % (const sc_unsigned&  u, mtiUlongT       v);
  friend   sc_signed operator % (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator % (const sc_unsigned&  u, unsigned int        v) 
    { return operator%(u, (mtiUlongT) v); }

  friend   sc_signed operator % (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator % (mtiLongT u, const sc_unsigned&  v); 
  friend sc_unsigned operator % (mtiUlongT u, const sc_unsigned&  v);
  friend   sc_signed operator % (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator % (unsigned int        u, const sc_unsigned&  v)  
    { return operator%((mtiUlongT) u,  v); } 

  const sc_unsigned& operator %= (const sc_signed&    v); 
  const sc_unsigned& operator %= (const sc_unsigned&  v); 
  const sc_unsigned& operator %= (int64               v); 
  const sc_unsigned& operator %= (uint64              v); 
  const sc_unsigned& operator %= (mtiLongT v); 
  const sc_unsigned& operator %= (mtiUlongT v); 
  const sc_unsigned& operator %= (int                 v) 
    { return operator%=((mtiLongT) v); }
  const sc_unsigned& operator %= (unsigned int        v) 
    { return operator%=((mtiUlongT) v); }

  friend sc_unsigned operator % (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator % (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator % (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator % (const sc_int_base&  u, const sc_unsigned&  v);
  const sc_unsigned& operator %= (const sc_int_base&  v);
  const sc_unsigned& operator %= (const sc_uint_base& v);

  // BITWISE OPERATORS:

  // Bitwise AND operators:
   
  friend   sc_signed operator & (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator & (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator & (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator & (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator & (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator & (const sc_unsigned&  u, mtiUlongT       v);
  friend   sc_signed operator & (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator & (const sc_unsigned&  u, unsigned int        v) 
    { return operator&(u, (mtiUlongT) v); }

  friend   sc_signed operator & (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator & (mtiLongT u, const sc_unsigned&  v); 
  friend sc_unsigned operator & (mtiUlongT u, const sc_unsigned&  v);
  friend   sc_signed operator & (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator & (unsigned int        u, const sc_unsigned&  v)  
    { return operator&((mtiUlongT) u,  v); } 

  const sc_unsigned& operator &= (const sc_signed&    v); 
  const sc_unsigned& operator &= (const sc_unsigned&  v); 
  const sc_unsigned& operator &= (int64               v); 
  const sc_unsigned& operator &= (uint64              v); 
  const sc_unsigned& operator &= (mtiLongT v); 
  const sc_unsigned& operator &= (mtiUlongT v); 
  const sc_unsigned& operator &= (int                 v) 
    { return operator&=((mtiLongT) v); }
  const sc_unsigned& operator &= (unsigned int        v) 
    { return operator&=((mtiUlongT) v); }

  friend sc_unsigned operator & (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator & (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator & (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator & (const sc_int_base&  u, const sc_unsigned&  v);
  const sc_unsigned& operator &= (const sc_int_base&  v);
  const sc_unsigned& operator &= (const sc_uint_base& v);

  // Bitwise OR operators:
   
  friend   sc_signed operator | (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator | (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator | (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator | (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator | (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator | (const sc_unsigned&  u, mtiUlongT       v);
  friend   sc_signed operator | (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator | (const sc_unsigned&  u, unsigned int        v) 
    { return operator|(u, (mtiUlongT) v); }

  friend   sc_signed operator | (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator | (mtiLongT u, const sc_unsigned&  v); 
  friend sc_unsigned operator | (mtiUlongT u, const sc_unsigned&  v);
  friend   sc_signed operator | (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator | (unsigned int        u, const sc_unsigned&  v)  
    { return operator|((mtiUlongT) u,  v); } 

  const sc_unsigned& operator |= (const sc_signed&    v); 
  const sc_unsigned& operator |= (const sc_unsigned&  v); 
  const sc_unsigned& operator |= (int64               v); 
  const sc_unsigned& operator |= (uint64              v); 
  const sc_unsigned& operator |= (mtiLongT v); 
  const sc_unsigned& operator |= (mtiUlongT v); 
  const sc_unsigned& operator |= (int                 v) 
    { return operator|=((mtiLongT) v); }
  const sc_unsigned& operator |= (unsigned int        v) 
    { return operator|=((mtiUlongT) v); }

  friend sc_unsigned operator | (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator | (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator | (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator | (const sc_int_base&  u, const sc_unsigned&  v);
  const sc_unsigned& operator |= (const sc_int_base&  v);
  const sc_unsigned& operator |= (const sc_uint_base& v);

  // Bitwise XOR operators:
   
  friend   sc_signed operator ^ (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator ^ (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator ^ (const sc_unsigned&  u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, uint64              v); 
  friend   sc_signed operator ^ (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator ^ (const sc_unsigned&  u, mtiUlongT       v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, int                 v);
  friend sc_unsigned operator ^ (const sc_unsigned&  u, unsigned int        v) 
    { return operator^(u, (mtiUlongT) v); }

  friend   sc_signed operator ^ (int64               u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (uint64              u, const sc_unsigned&  v); 
  friend   sc_signed operator ^ (mtiLongT u, const sc_unsigned&  v); 
  friend sc_unsigned operator ^ (mtiUlongT u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (int                 u, const sc_unsigned&  v);
  friend sc_unsigned operator ^ (unsigned int        u, const sc_unsigned&  v)  
    { return operator^((mtiUlongT) u,  v); } 

  const sc_unsigned& operator ^= (const sc_signed&    v); 
  const sc_unsigned& operator ^= (const sc_unsigned&  v); 
  const sc_unsigned& operator ^= (int64               v); 
  const sc_unsigned& operator ^= (uint64              v); 
  const sc_unsigned& operator ^= (mtiLongT v); 
  const sc_unsigned& operator ^= (mtiUlongT v); 
  const sc_unsigned& operator ^= (int                 v) 
    { return operator^=((mtiLongT) v); }
  const sc_unsigned& operator ^= (unsigned int        v) 
    { return operator^=((mtiUlongT) v); }

  friend sc_unsigned operator ^ (const sc_unsigned&  u, const sc_uint_base& v);
  friend   sc_signed operator ^ (const sc_unsigned&  u, const sc_int_base&  v);
  friend sc_unsigned operator ^ (const sc_uint_base& u, const sc_unsigned&  v);
  friend   sc_signed operator ^ (const sc_int_base&  u, const sc_unsigned&  v);
  const sc_unsigned& operator ^= (const sc_int_base&  v);
  const sc_unsigned& operator ^= (const sc_uint_base& v);

  // SHIFT OPERATORS:

  // LEFT SHIFT operators:
   
  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator << (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator << (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator << (const sc_unsigned&  u, mtiUlongT       v);
  friend sc_unsigned operator << (const sc_unsigned&  u, int                 v) 
    { return operator<<(u, (mtiLongT) v); }
  friend sc_unsigned operator << (const sc_unsigned&  u, unsigned int        v) 
    { return operator<<(u, (mtiUlongT) v); }

  const sc_unsigned& operator <<= (const sc_signed&    v); 
  const sc_unsigned& operator <<= (const sc_unsigned&  v); 
  const sc_unsigned& operator <<= (int64               v); 
  const sc_unsigned& operator <<= (uint64              v); 
  const sc_unsigned& operator <<= (mtiLongT v); 
  const sc_unsigned& operator <<= (mtiUlongT v); 
  const sc_unsigned& operator <<= (int                 v) 
    { return operator<<=((mtiLongT) v); }
  const sc_unsigned& operator <<= (unsigned int        v) 
    { return operator<<=((mtiUlongT) v); }

  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_uint_base& v);
  friend sc_unsigned operator << (const sc_unsigned&  u, const sc_int_base&  v);
  const sc_unsigned& operator <<= (const sc_int_base&  v);
  const sc_unsigned& operator <<= (const sc_uint_base& v);

  // RIGHT SHIFT operators:
   
  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_signed&    v); 
  friend   sc_signed operator >> (const sc_signed&    u, const sc_unsigned&  v); 

  friend sc_unsigned operator >> (const sc_unsigned&  u, const sc_unsigned&  v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, int64               v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, uint64              v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, mtiLongT                v); 
  friend sc_unsigned operator >> (const sc_unsigned&  u, mtiUlongT       v);
  friend sc_unsigned operator >> (const sc_unsigned&  u, int                 v) 
    { return operator>>(u, (mtiLongT) v); }
  friend sc_unsigned operator >> (const sc_unsigned&  u, unsigned int        v) 
    { return operator>>(u, (mtiUlongT) v); }

  const sc_unsigned& operator >>= (const sc_signed&    v); 
  const sc_unsigned& operator >>= (const sc_unsigned&  v); 
  const sc_unsigned& operator >>= (int64               v); 
  const sc_unsigned& operator >>= (uint64              v); 
  const sc_unsigned& operator >>= (mtiLongT v); 
  const sc_unsigned& operator >>= (mtiUlongT v); 
  const sc_unsigned& operator >>= (int                 v) 
    { return operator>>=((mtiLongT) v); }
  const sc_unsigned& operator >>= (unsigned int        v) 
    { return operator>>=((mtiUlongT) v); }

  friend sc_unsigned operator >> ( const sc_unsigned& , const sc_uint_base& );
  friend sc_unsigned operator >> ( const sc_unsigned&, const sc_int_base& );
  const sc_unsigned& operator >>= (const sc_int_base&  v);
  const sc_unsigned& operator >>= (const sc_uint_base& v);

  // Unary arithmetic operators
  friend sc_unsigned operator + (const sc_unsigned& u);
  friend   sc_signed operator - (const sc_unsigned& u);

  // LOGICAL OPERATORS:

  // Logical EQUAL operators:
   
  friend bool operator == (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator == (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator == (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator == (const sc_unsigned&  u, int64               v); 
  friend bool operator == (const sc_unsigned&  u, uint64              v); 
  friend bool operator == (const sc_unsigned&  u, mtiLongT                v); 
  friend bool operator == (const sc_unsigned&  u, mtiUlongT       v);
  friend bool operator == (const sc_unsigned&  u, int                 v) 
    { return operator==(u, (mtiLongT) v); }
  friend bool operator == (const sc_unsigned&  u, unsigned int        v) 
    { return operator==(u, (mtiUlongT) v); }

  friend bool operator == (int64               u, const sc_unsigned&  v); 
  friend bool operator == (uint64              u, const sc_unsigned&  v); 
  friend bool operator == (mtiLongT u, const sc_unsigned&  v); 
  friend bool operator == (mtiUlongT u, const sc_unsigned&  v);
  friend bool operator == (int                 u, const sc_unsigned&  v)  
    { return operator==((mtiLongT) u,  v); } 
  friend bool operator == (unsigned int        u, const sc_unsigned&  v)  
    { return operator==((mtiUlongT) u,  v); } 

  friend bool operator == (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator == (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator == (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator == (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical NOT_EQUAL operators:
   
  friend bool operator != (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator != (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator != (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator != (const sc_unsigned&  u, int64               v); 
  friend bool operator != (const sc_unsigned&  u, uint64              v); 
  friend bool operator != (const sc_unsigned&  u, mtiLongT                v); 
  friend bool operator != (const sc_unsigned&  u, mtiUlongT       v);
  friend bool operator != (const sc_unsigned&  u, int                 v) 
    { return operator!=(u, (mtiLongT) v); }
  friend bool operator != (const sc_unsigned&  u, unsigned int        v) 
    { return operator!=(u, (mtiUlongT) v); }

  friend bool operator != (int64               u, const sc_unsigned&  v); 
  friend bool operator != (uint64              u, const sc_unsigned&  v); 
  friend bool operator != (mtiLongT u, const sc_unsigned&  v); 
  friend bool operator != (mtiUlongT u, const sc_unsigned&  v);
  friend bool operator != (int                 u, const sc_unsigned&  v)  
    { return operator!=((mtiLongT) u,  v); } 
  friend bool operator != (unsigned int        u, const sc_unsigned&  v)  
    { return operator!=((mtiUlongT) u,  v); } 

  friend bool operator != (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator != (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator != (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator != (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical LESS_THAN operators:
   
  friend bool operator < (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator < (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator < (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator < (const sc_unsigned&  u, int64               v); 
  friend bool operator < (const sc_unsigned&  u, uint64              v); 
  friend bool operator < (const sc_unsigned&  u, mtiLongT                v); 
  friend bool operator < (const sc_unsigned&  u, mtiUlongT       v);
  friend bool operator < (const sc_unsigned&  u, int                 v) 
    { return operator<(u, (mtiLongT) v); }
  friend bool operator < (const sc_unsigned&  u, unsigned int        v) 
    { return operator<(u, (mtiUlongT) v); }

  friend bool operator < (int64               u, const sc_unsigned&  v); 
  friend bool operator < (uint64              u, const sc_unsigned&  v); 
  friend bool operator < (mtiLongT u, const sc_unsigned&  v); 
  friend bool operator < (mtiUlongT u, const sc_unsigned&  v);
  friend bool operator < (int                 u, const sc_unsigned&  v)  
    { return operator<((mtiLongT) u,  v); } 
  friend bool operator < (unsigned int        u, const sc_unsigned&  v)  
    { return operator<((mtiUlongT) u,  v); } 

  friend bool operator < (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator < (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator < (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator < (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical LESS_THAN_AND_EQUAL operators:
   
  friend bool operator <= (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator <= (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator <= (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator <= (const sc_unsigned&  u, int64               v); 
  friend bool operator <= (const sc_unsigned&  u, uint64              v); 
  friend bool operator <= (const sc_unsigned&  u, mtiLongT                v); 
  friend bool operator <= (const sc_unsigned&  u, mtiUlongT       v);
  friend bool operator <= (const sc_unsigned&  u, int                 v) 
    { return operator<=(u, (mtiLongT) v); }
  friend bool operator <= (const sc_unsigned&  u, unsigned int        v) 
    { return operator<=(u, (mtiUlongT) v); }

  friend bool operator <= (int64               u, const sc_unsigned&  v); 
  friend bool operator <= (uint64              u, const sc_unsigned&  v); 
  friend bool operator <= (mtiLongT u, const sc_unsigned&  v); 
  friend bool operator <= (mtiUlongT u, const sc_unsigned&  v);
  friend bool operator <= (int                 u, const sc_unsigned&  v)  
    { return operator<=((mtiLongT) u,  v); } 
  friend bool operator <= (unsigned int        u, const sc_unsigned&  v)  
    { return operator<=((mtiUlongT) u,  v); } 

  friend bool operator <= (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator <= (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator <= (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator <= (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical GREATER_THAN operators:
   
  friend bool operator > (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator > (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator > (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator > (const sc_unsigned&  u, int64               v); 
  friend bool operator > (const sc_unsigned&  u, uint64              v); 
  friend bool operator > (const sc_unsigned&  u, mtiLongT                v); 
  friend bool operator > (const sc_unsigned&  u, mtiUlongT       v);
  friend bool operator > (const sc_unsigned&  u, int                 v) 
    { return operator>(u, (mtiLongT) v); }
  friend bool operator > (const sc_unsigned&  u, unsigned int        v) 
    { return operator>(u, (mtiUlongT) v); }

  friend bool operator > (int64               u, const sc_unsigned&  v); 
  friend bool operator > (uint64              u, const sc_unsigned&  v); 
  friend bool operator > (mtiLongT u, const sc_unsigned&  v); 
  friend bool operator > (mtiUlongT u, const sc_unsigned&  v);
  friend bool operator > (int                 u, const sc_unsigned&  v)  
    { return operator>((mtiLongT) u,  v); } 
  friend bool operator > (unsigned int        u, const sc_unsigned&  v)  
    { return operator>((mtiUlongT) u,  v); } 

  friend bool operator > (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator > (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator > (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator > (const sc_int_base&  u, const sc_unsigned&  v);

  // Logical GREATER_THAN_AND_EQUAL operators:
   
  friend bool operator >= (const sc_unsigned&  u, const sc_signed&    v); 
  friend bool operator >= (const sc_signed&    u, const sc_unsigned&  v); 

  friend bool operator >= (const sc_unsigned&  u, const sc_unsigned&  v);
  friend bool operator >= (const sc_unsigned&  u, int64               v); 
  friend bool operator >= (const sc_unsigned&  u, uint64              v); 
  friend bool operator >= (const sc_unsigned&  u, mtiLongT                v); 
  friend bool operator >= (const sc_unsigned&  u, mtiUlongT       v);
  friend bool operator >= (const sc_unsigned&  u, int                 v) 
    { return operator>=(u, (mtiLongT) v); }
  friend bool operator >= (const sc_unsigned&  u, unsigned int        v) 
    { return operator>=(u, (mtiUlongT) v); }

  friend bool operator >= (int64               u, const sc_unsigned&  v); 
  friend bool operator >= (uint64              u, const sc_unsigned&  v); 
  friend bool operator >= (mtiLongT u, const sc_unsigned&  v); 
  friend bool operator >= (mtiUlongT u, const sc_unsigned&  v);
  friend bool operator >= (int                 u, const sc_unsigned&  v)  
    { return operator>=((mtiLongT) u,  v); } 
  friend bool operator >= (unsigned int        u, const sc_unsigned&  v)  
    { return operator>=((mtiUlongT) u,  v); } 

  friend bool operator >= (const sc_unsigned&  u, const sc_uint_base& v);
  friend bool operator >= (const sc_unsigned&  u, const sc_int_base&  v);
  friend bool operator >= (const sc_uint_base& u, const sc_unsigned&  v);
  friend bool operator >= (const sc_int_base&  u, const sc_unsigned&  v);

  // Bitwise NOT operator (unary).
  friend sc_unsigned operator ~ (const sc_unsigned& u); 

  // Helper functions.
  friend int compare_unsigned(small_type us, 
                              int unb,
                              int und, 
                              const sc_digit *ud, 
                              small_type vs, 
                              int vnb,
                              int vnd,
                              const sc_digit *vd,
                              small_type if_u_signed = 0,
                              small_type if_v_signed = 0);
  
  friend sc_unsigned add_unsigned_friend(small_type us, 
                                         int unb,
                                         int und, 
                                         const sc_digit *ud, 
                                         small_type vs, 
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);
  
  friend sc_unsigned sub_unsigned_friend(small_type us, 
                                         int unb,
                                         int und, 
                                         const sc_digit *ud, 
                                         small_type vs, 
                                         int vnb,
                                         int vnd, 
                                         const sc_digit *vd);
  
  friend sc_unsigned mul_unsigned_friend(small_type s,
                                         int unb,
                                         int und, 
                                         const sc_digit *ud, 
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);
  
  friend sc_unsigned div_unsigned_friend(small_type s,
                                         int unb,
                                         int und, 
                                         const sc_digit *ud, 
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);
  
  friend sc_unsigned mod_unsigned_friend(small_type us,
                                         int unb,
                                         int und, 
                                         const sc_digit *ud, 
                                         int vnb,
                                         int vnd,
                                         const sc_digit *vd);
  
  friend sc_unsigned and_unsigned_friend(small_type us, 
                                         int unb, 
                                         int und, 
                                         const sc_digit *ud, 
                                         small_type vs,
                                         int vnb, 
                                         int vnd,
                                         const sc_digit *vd);
  
  friend sc_unsigned or_unsigned_friend(small_type us, 
                                        int unb, 
                                        int und, 
                                        const sc_digit *ud, 
                                        small_type vs,
                                        int vnb, 
                                        int vnd,
                                        const sc_digit *vd);
  
  friend sc_unsigned xor_unsigned_friend(small_type us, 
                                         int unb, 
                                         int und, 
                                         const sc_digit *ud, 
                                         small_type vs,
                                         int vnb, 
                                         int vnd,
                                         const sc_digit *vd);
  
public:
  static sc_core::sc_vpool<sc_unsigned> m_pool;

private:

  small_type  sgn;         // Shortened as s.

  // BEGIN MODELSIM ADDITIONS
  unsigned char is_using_pool;
  // END MODELSIM ADDITIONS

  int nbits;       // Shortened as nb.
  int ndigits;     // Shortened as nd.

#ifdef SC_MAX_NBITS
  sc_digit digit[DIV_CEIL(SC_MAX_NBITS)];   // Shortened as d.
#else
  sc_digit *digit;                       // Shortened as d.
#endif

  // Private constructors:

  // Create a copy of v with sign s.
  sc_unsigned(const sc_unsigned& v, small_type s);
  sc_unsigned(const sc_signed&   v, small_type s);

  // Create an unsigned number with the given attributes.
  sc_unsigned(small_type s, int nb, int nd, 
              sc_digit *d, bool alloc = true);

  // Create an unsigned number using the bits u[l..r].
  sc_unsigned(const sc_signed* u, int l, int r);
  sc_unsigned(const sc_unsigned* u, int l, int r);

  // Private member functions. The called functions are inline functions.

  small_type default_sign() const
    { return SC_POS; }

  int num_bits(int nb) const { return nb + 1; }

  bool check_if_outside(int bit_num) const;

  void copy_digits(int nb, int nd, const sc_digit *d)
    { copy_digits_unsigned(sgn, nbits, ndigits, digit, nb, nd, d); }
  
  void makezero()
    { sgn = make_zero(ndigits, digit); }

  // Conversion functions between 2's complement (2C) and
  // sign-magnitude (SM):
  void convert_2C_to_SM()
    { sgn = convert_unsigned_2C_to_SM(nbits, ndigits, digit); }

  void convert_SM_to_2C_to_SM()
    { sgn = convert_unsigned_SM_to_2C_to_SM(sgn, nbits, ndigits, digit); }

  void convert_SM_to_2C()
    { convert_unsigned_SM_to_2C(sgn, ndigits, digit); }

  // BEGIN MODELSIM ADDITIONS

protected:
  void initialize();

  void resize(int nb);

public:

  virtual void mti_get_value_size(
	int* has_construction_param,
	int* elem_num_bits,
	int* elem_num_integer_bits);

  virtual void* mti_get_value();

  virtual void mti_set_value(void* value);

  virtual void mti_get_dpi_value(void* value) const;

  virtual void mti_set_dpi_value(void* value);

  virtual mtiTypeIdT mti_get_value_type();

private:

  unsigned char* m_mti_value;

  // END MODELSIM ADDITIONS

};


inline
::std::ostream&
operator << ( ::std::ostream&, const sc_unsigned& );

inline
::std::istream&
operator >> ( ::std::istream&, sc_unsigned& );


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref_r
//
//  Proxy class for sc_unsigned bit selection (r-value only).
// ----------------------------------------------------------------------------


inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_unsigned_bitref_r& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_bitref
//
//  Proxy class for sc_unsigned bit selection (r-value and l-value).
// ----------------------------------------------------------------------------


inline
::std::istream&
operator >> ( ::std::istream& is, sc_unsigned_bitref& a )
{
    a.scan( is );
    return is;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref_r
//
//  Proxy class for sc_unsigned part selection (r-value only).
// ----------------------------------------------------------------------------

// reduce methods

inline bool sc_unsigned_subref_r::and_reduce() const 
{
   const sc_unsigned* target_p = m_obj_p;
   for ( int i = m_right; i <= m_left; i++ )
	if ( !target_p->test(i) ) return false;
   return true;
}

inline bool sc_unsigned_subref_r::nand_reduce() const
{ 
    return !and_reduce(); 
}

inline bool sc_unsigned_subref_r::or_reduce() const 
{
   const sc_unsigned* target_p = m_obj_p;
   for ( int i = m_right; i <= m_left; i++ )
	if ( target_p->test(i) ) return true;
   return false;
}

inline bool sc_unsigned_subref_r::nor_reduce() const
{ 
    return !or_reduce(); 
}

inline bool sc_unsigned_subref_r::xor_reduce() const 
{
   int                odd;
   const sc_unsigned* target_p = m_obj_p;
   odd = 0;
   for ( int i = m_right; i <= m_left; i++ )
	if ( target_p->test(i) ) odd = ~odd;
   return odd ? true : false;
}

inline bool sc_unsigned_subref_r::xnor_reduce() const
{ 
    return !xor_reduce(); 
}


inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_unsigned_subref_r& a )
{
    a.print( os );
    return os;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned_subref
//
//  Proxy class for sc_unsigned part selection (r-value and l-value).
// ----------------------------------------------------------------------------

// assignment operators

// START MODELSIM MODIFICATIONS

//
// Put this function definition outside the sc_signed_subref
// class definition for HP aCC compiler.
//
template<class T>
inline const sc_unsigned_subref& 
sc_unsigned_subref::operator = ( const sc_generic_base<T>& a )
{
	sc_unsigned temp( length() );
	a->to_sc_unsigned(temp); 
	return *this = temp;
}

// END MODELSIM MODIFICATIONS

inline
const sc_unsigned_subref&
sc_unsigned_subref::operator = ( const char* a )
{
    sc_unsigned aa( length() );
    return ( *this = aa = a );
}


inline
::std::istream&
operator >> ( ::std::istream& is, sc_unsigned_subref& a )
{
    a.scan( is );
    return is;
}



// ----------------------------------------------------------------------------
//  CLASS : sc_unsigned
//
//  Arbitrary precision signed number.
// ----------------------------------------------------------------------------

template<class T>
sc_unsigned::sc_unsigned( const sc_generic_base<T>& v )
{
	initialize();
	int nb = v->length();
	sgn = default_sign();
	if( nb > 0 ) {
		nbits = num_bits( nb );
	} else {
		char msg[BUFSIZ];
		std::sprintf( msg,
			"sc_unsigned( sc_generic_base<T> ) : nb = %d is not valid", nb);
		SC_REPORT_ERROR( sc_core::SC_ID_INIT_FAILED_, msg );
	}
	ndigits = DIV_CEIL(nbits);
#   ifdef SC_MAX_NBITS
		test_bound(nb);
#    else
		digit = new sc_digit[ndigits];
#    endif
	makezero();
	v->to_sc_unsigned(*this);

	// BEGIN MODELSIM ADDITION
	m_mti_value = NULL;
	// END MODELSIM ADDITION
}


inline
::std::ostream&
operator << ( ::std::ostream& os, const sc_unsigned& a )
{
    a.print( os );
    return os;
}

inline
::std::istream&
operator >> ( ::std::istream& is, sc_unsigned& a )
{
    a.scan( is );
    return is;
}


} // namespace sc_dt


#endif