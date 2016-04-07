/*
 * This file is part of OpenObjectStore OOS.
 *
 * OpenObjectStore OOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * OpenObjectStore OOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with OpenObjectStore OOS. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SQL_HPP
#define SQL_HPP

#ifdef _MSC_VER
  #ifdef oos_EXPORTS
    #define OOS_API __declspec(dllexport)
    #define EXPIMP_TEMPLATE
  #else
    #define OOS_API __declspec(dllimport)
    #define EXPIMP_TEMPLATE extern
  #endif
  #pragma warning(disable: 4251)
#else
  #define OOS_API
#endif

#include "sql/types.hpp"
#include "sql/token.hpp"

#include <string>
#include <map>
#include <list>
#include <vector>
#include <stdexcept>
#include <memory>

namespace oos {

/// @cond OOS_DEV

class OOS_API sql
{
public:

public:
  typedef std::list<std::shared_ptr<detail::token>> token_list_t;
  
public:
  ~sql();

  void append(const std::shared_ptr<detail::token> tokptr);
  void append(detail::token *tok);

  void reset();

  std::string compile(basic_dialect &dialect) const;

  static unsigned int type_size(data_type_t type);
  template < class T >
  static unsigned int data_type()
  {
    return type_traits<T>::data_type();
  }

private:
  token_list_t token_list_;
};

/// @endcond

}

#endif /* SQL_HPP */
