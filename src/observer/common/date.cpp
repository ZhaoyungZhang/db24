/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-24 10:11:58
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-25 14:56:39
 * @FilePath: /miniob/src/observer/common/date.cpp
 */
#include "common/date.h"
#include "common/lang/comparator.h"
#include <cstdio>
#include <ctime>
#include <string>
#include <strings.h>

bool valid_tm(const std::tm &tm) {
  auto cpy = tm;
  const auto as_time_t = std::mktime(std::addressof(cpy));
  cpy = *std::localtime(std::addressof(as_time_t));
  return tm.tm_mday == cpy.tm_mday && // valid day
         tm.tm_mon == cpy.tm_mon &&   // valid month
         tm.tm_year == cpy.tm_year && // valid year
         tm.tm_wday == cpy.tm_wday;   // valid day of week
}

Date::Date(const std::string &s) {
  std::tm tm;
  bzero(&tm, sizeof(tm));
  char *tail = strptime(s.c_str(), "%F", &tm);
  if (tail != s.c_str() + s.size() || !valid_tm(tm)) {
    value = -1;
  } else {
    value = std::mktime(&tm) / kSecondsInDay;
  }
}

std::string Date::to_string(const Date &date) {
  time_t t = date.value;
  t *= kSecondsInDay;
  struct tm tm;
  gmtime_r(&t, &tm);
  char buf[32];  // 扩大缓冲区大小以确保足够大
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
  return std::string(buf);
}

int Date::compare_date(const Date *a, const Date *b) { return common::compare_int((void *)a, (void *)b); }