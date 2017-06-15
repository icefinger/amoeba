// -*- mode: c++; -*-
/* amoeba_version.h version header file
 *
 *
 */

#ifndef __amoeba__amoeba_version_h
#define __amoeba__amoeba_version_h 1

//  AMOEBA_VERSION % 100        is the patch level
//  AMOEBA_VERSION / 100 % 1000 is the minor version
//  AMOEBA_VERSION / 100000     is the major version

#define AMOEBA_VERSION 100

//  AMOEBA_LIB_VERSION must be defined to be the same as AMOEBA_VERSION
//  but as a *string* in the form "x_y[_z]" where x is the major version
//  number, y is the minor version number, and z is the patch level if not 0.

#define AMOEBA_LIB_VERSION "1.0"

#endif // __amoeba__amoeba_version_h

// end of amoeba_version.h
