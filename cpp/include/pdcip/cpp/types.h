/**
 * @file types.h
 * @author Derek Huang
 * @brief Extra convenience types
 * @copyright MIT License
 */

#ifndef PDCIP_CPP_TYPES_H_
#define PDCIP_CPP_TYPES_H_

#include <memory>
#include <vector>

namespace pdcip {

template <class T>
using vector_ptr_t = std::shared_ptr<std::vector<T>>;

using double_vector = std::vector<double>;
using double_vector_ptr = vector_ptr_t<double>;

}  // namespace pdcip

#endif  // PDCIP_CPP_TYPES_H_
