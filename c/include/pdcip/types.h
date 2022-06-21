/**
 * @file types.h
 * @author Derek Huang
 * @brief C header for type enums
 * @copyright MIT License
 */

#ifndef PDCIP_TYPES_H_
#define PDCIP_TYPES_H_

/**
 * Type enumeration so we can embed type information.
 */
enum pdcip_type {
  PDCIP_INT,
  PDCIP_INT_PTR,
  PDCIP_FLOAT,
  PDCIP_FLOAT_PTR,
  PDCIP_DOUBLE,
  PDCIP_DOUBLE_PTR,
  PDCIP_CHAR,
  PDCIP_CHAR_PTR,
  PDCIP_STRING,
  PDCIP_SIZE_T,
  PDCIP_SIZE_T_PTR
};
typedef enum pdcip_type pdcip_type;

#endif  /* PDCIP_TYPES_H_ */
