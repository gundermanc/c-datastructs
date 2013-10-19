/**
 * Lookup3 HashTable Imports for Gunderman Data Structures
 * (C) 2013 Christian Gunderman
 *
 * NOTE: lookup3 algorithm is the work of Bob Jenkins.
 * Used according to his Public Domain statement.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>
 *
 * Contact Email: gundermanc@gmail.com
 */
 */

#ifndef LOOKUP3__H__
#define LOOKUP3__H__

#include <stdint.h>
uint32_t hashlittle( const void *key, size_t length, uint32_t initval);

#endif LOOKUP3__H__
