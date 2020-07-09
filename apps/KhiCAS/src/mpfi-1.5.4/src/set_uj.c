/* mpfi_set_uj -- set a MPFI interval from a huge machine unsigned integer

Copyright 2017-2018 Free Software Foundation, Inc.
Contributed by the AriC project, INRIA.

This file is part of the GNU MPFR Library.

The GNU MPFI Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 3 of the License, or (at your
option) any later version.

The GNU MPFI Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the GNU MPFI Library; see the file COPYING.LESSER.  If not, see
http://www.gnu.org/licenses/ or write to the Free Software Foundation, Inc.,
51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA. */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "mpfi-intmax.h"
#include "mpfi-impl.h"

#ifdef _MPFR_H_HAVE_INTMAX_T

int
mpfi_set_uj (mpfi_t a, uintmax_t b)
{
int inexact_left, inexact_right, inexact=0;

  inexact_left = mpfr_set_uj_2exp (&(a->left), b, 0, MPFI_RNDD);
  inexact_right = mpfr_set_uj_2exp (&(a->right), b, 0, MPFI_RNDU);

  if (b == 0) {
    /* fix signed zero so as to return [+0, -0] */
    mpfr_setsign (&(a->right), &(a->right), 1, MPFI_RNDD);
  }

  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  return inexact;
}


/* Combined initialization and assignment      */

int
mpfi_init_set_uj (mpfi_ptr a, const uintmax_t b)
{
  int inexact_left, inexact_right, inexact = 0;

  inexact_left = mpfr_init_set_uj (&(a->left), b, MPFI_RNDD);
  inexact_right = mpfr_init_set_uj (&(a->right), b, MPFI_RNDU);

  if (b == 0) {
    /* fix signed zero so as to return [+0, -0] */
    mpfr_setsign (&(a->left), &(a->left), 0, MPFI_RNDU);
    mpfr_setsign (&(a->right), &(a->right), 1, MPFI_RNDD);
  }

  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  return inexact;
}

int
mpfi_set_uj_2exp (mpfi_t x, uintmax_t b, intmax_t e)
{
int inexact_left, inexact_right, inexact=0;

  inexact_left = mpfr_set_uj_2exp (&(a->left), b, e, MPFI_RNDD);
  inexact_right = mpfr_set_uj_2exp (&(a->right), b, e, MPFI_RNDU);

  if (b == 0) {
    /* fix signed zero so as to return [+0, -0] */
    mpfr_setsign (&(a->right), &(a->right), 1, MPFI_RNDD);
  }
    
  if (inexact_left)
    inexact += 1;
  if (inexact_right)
    inexact += 2;

  return inexact;
}

#endif
