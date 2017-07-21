/**
 * Copyright (C) 2017 Ossama Othman
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <MaRC/Vector.h>


int main()
{
  using test_vector_type = MaRC::Vector<int, 3>;
  test_vector_type const v1{  0,  1,  5 };
  test_vector_type const v2{  1,  2,  3 };
  test_vector_type const s {  1,  3,  8 };
  test_vector_type const d { -1, -1,  2 };
  
  return v1 + v2 == s && v1 - v2 == d ? 0 : -1;
}
