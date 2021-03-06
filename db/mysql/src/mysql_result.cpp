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

#include "tools/varchar.hpp"
#include "tools/date.hpp"
#include "tools/time.hpp"
#include "tools/string.hpp"

#include "tools/identifier.hpp"

#include "mysql_result.hpp"
#include "mysql_exception.hpp"

#include <cstring>

namespace oos {

namespace mysql {

mysql_result::mysql_result(MYSQL *c)
  : affected_rows_((size_type)mysql_affected_rows(c))
  , rows_(0)
  , fields_(0)
  , res_(0)
{
  res_ = mysql_store_result(c);
  if (res_ == 0 && mysql_errno(c) > 0) {
    throw_error(-1, c, "mysql", "");
  } else if (res_) {
    rows_ = (size_type)mysql_num_rows(res_);
    fields_ = mysql_num_fields(res_);
  }
}
mysql_result::~mysql_result()
{
  if (res_) {
    mysql_free_result(res_);
  }
}

const char* mysql_result::column(size_type c) const
{
  if (row_) {
    return row_[c];
  } else {
    return 0;
  }
}

bool mysql_result::fetch()
{
  row_ = mysql_fetch_row(res_);
  if (!row_) {
    rows_ = 0;
    return false;
  }    
  return rows_-- > 0;
}

bool mysql_result::prepare_fetch()
{
  if (!fetch()) {
    return false;
  }

  result_index_ = 0;
  return true;
}

bool mysql_result::finalize_fetch()
{
  return true;
}

mysql_result::size_type mysql_result::affected_rows() const
{
  return affected_rows_;
}

mysql_result::size_type mysql_result::result_rows() const
{
  return rows_;
}

mysql_result::size_type mysql_result::fields() const
{
  return fields_;
}

int mysql_result::transform_index(int index) const
{
  return index;
}

void mysql_result::serialize(const char */*id*/, char &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) > 1) {
    x = val[0];
  }
}

void mysql_result::serialize(const char */*id*/, short &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (short)strtol(val, &end, 10);
}

void mysql_result::serialize(const char */*id*/, int &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (int)strtol(val, &end, 10);
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, long &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtol(val, &end, 10);
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, unsigned char &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned char)strtoul(val, &end, 10);
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, unsigned short &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned short)strtoul(val, &end, 10);
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, unsigned int &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (unsigned int)strtoul(val, &end, 10);
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, unsigned long &x)
{
  char *val = row_[result_index_++];
  if (!val || strlen(val) == 0) {
    return;
  }
  char *end = nullptr;
  x = strtoul(val, &end, 10);
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, bool &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtoul(val, &end, 10) > 0;
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, float &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = (float)strtod(val, &end);
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, double &x)
{
  char *val = row_[result_index_++];
  if (strlen(val) == 0) {
    return;
  }
  char *end;
  x = strtod(val, &end);
  // Todo: check error
}

void mysql_result::serialize(const char */*id*/, char *x, size_t s)
{
  char *val = row_[result_index_++];
  size_t len = strlen(val);
  if (len > (size_t)s) {
    strncpy(x, val, (size_t)s);
    x[s-1] = '\n';
  } else {
    strcpy(x, val);
  }

}

void mysql_result::serialize(const char */*id*/, varchar_base &x)
{
  char *val = row_[result_index_++];
  x.assign(val);
}

void mysql_result::serialize(const char */*id*/, std::string &x)
{
  char *val = row_[result_index_++];
  x.assign(val);
}

void mysql_result::serialize(const char *, oos::date &x)
{
  char *val = row_[result_index_++];
  size_t len = strlen(val);
  if (val == nullptr || len == 0) {
    return;
  }
  x.set(val, oos::date_format::ISO8601);
}

void mysql_result::serialize(const char *id, oos::time &x)
{
#if MYSQL_VERSION_ID < 50604
  // before mysql version 5.6.4 datetime
  // doesn't support fractional seconds
  // so we use a datetime string here
  std::string val;
  serialize(id, val);
  x = oos::time::parse(val, "%F %T");
#else
  std::string val;
  serialize(id, val);
  x = oos::time::parse(val, "%F %T.%f");
#endif
}

void mysql_result::serialize(const char *id, oos::basic_identifier &x)
{
  x.serialize(id, *this);
}

void mysql_result::serialize(const char *id, oos::identifiable_holder &x, cascade_type)
{
  read_foreign_object(id, x);
}

}

}
