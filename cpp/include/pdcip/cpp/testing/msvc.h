/**
 * @file msvc.h
 * @author Derek Huang
 * @brief C++ header defining some test helpers when using MSVC compiler
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_TESTING_MSVC_H_
#define PDCIP_CPP_TESTING_MSVC_H_

#ifdef _MSC_VER

/**
 * MSVC warnings emitted with /W4 or above specified.
 *
 * Usage: `#pragma warning (disable: GTEST_FIXTURE_WARNINGS)`
 */
#define GTEST_FIXTURE_WARNINGS 4625 4626 5026 5027

#endif  // _MSC_VER

#endif  // PDCIP_CPP_TESTING_MSVC_H_
