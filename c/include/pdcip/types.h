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
  PDCIP_INT_ARRAY,
  PDCIP_FLOAT,
  PDCIP_FLOAT_ARRAY,
  PDCIP_DOUBLE,
  PDCIP_DOUBLE_ARRAY,
  PDCIP_CHAR,
  PDCIP_CHAR_ARRAY,
  PDCIP_STRING,
  PDCIP_SIZE_T,
  PDCIP_SIZE_T_ARRAY,
  PDCIP_VOID_PTR,
  PDCIP_VOID_PTR_ARRAY,
  NUM_TYPES  // convenience/sentinel member
};
typedef enum pdcip_type pdcip_type;

#endif  /* PDCIP_TYPES_H_ */
