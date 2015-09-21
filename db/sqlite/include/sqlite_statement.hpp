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

#ifndef SQLITE_STATEMENT_HPP
#define SQLITE_STATEMENT_HPP

#include "database/statement_impl.hpp"

#include "object/object_atomizer.hpp"
#include "object/primary_key.hpp"

#include <string>
#include <vector>
#include <memory>

struct sqlite3_stmt;

namespace oos {

class varchar_base;

namespace sqlite {

class sqlite_database;

class sqlite_statement : public oos::detail::statement_impl
{
public:
  sqlite_statement(sqlite_database &db, const std::string stmt);
  virtual ~sqlite_statement();

  virtual void clear();
  virtual result execute();
  virtual void reset();

protected:
  virtual void write(const char *id, char x);
  virtual void write(const char *id, short x);
  virtual void write(const char *id, int x);
  virtual void write(const char *id, long x);
  virtual void write(const char *id, unsigned char x);
  virtual void write(const char *id, unsigned short x);
  virtual void write(const char *id, unsigned int x);
  virtual void write(const char *id, unsigned long x);
  virtual void write(const char *id, float x);
  virtual void write(const char *id, double x);
  virtual void write(const char *id, bool x);
  virtual void write(const char *id, const char *x, int s);
  virtual void write(const char *id, const varchar_base &x);
  virtual void write(const char *id, const std::string &x);
  virtual void write(const char *id, const oos::date &x);
  virtual void write(const char *id, const oos::time &x);
  virtual void write(const char *id, const object_base_ptr &x);
  virtual void write(const char *id, const object_container &x);
  virtual void write(const char *id, const primary_key_base &x);

private:
  sqlite_database &db_;
  sqlite3_stmt *stmt_;

  std::vector<std::shared_ptr<std::string> > host_strings_;
};

}

}

#endif /* SQLITE_STATEMENT_HPP */
