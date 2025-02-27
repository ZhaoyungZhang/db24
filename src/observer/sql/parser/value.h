/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 10:42:58
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-27 12:09:05
 * @FilePath: /miniob/src/observer/sql/parser/value.h
 */
/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Wangyunlai 2023/6/27
//

#pragma once

#include "common/date.h"
#include <compare>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>


/**
 * @brief 属性的类型
 *
 */
enum class AttrType
{
  UNDEFINED,
  CHARS,     ///< 字符串类型
  INTS,      ///< 整数类型(4字节)
  FLOATS,    ///< 浮点数类型(4字节)
  DATES,     ///< 日期类型 字符串
  BOOLEANS,  ///< boolean类型，当前不是由parser解析出来的，是程序内部使用的
};

const char *attr_type_to_string(AttrType type);
AttrType    attr_type_from_string(const char *s);

/**
 * @brief 属性的值
 *
 */
class Value final
{
public:
  Value() = default;

  Value(AttrType attr_type, char *data, int length = 4) : attr_type_(attr_type) { this->set_data(data, length); }

  explicit Value(int val);
  explicit Value(float val);
  explicit Value(bool val);
  explicit Value(Date date);
  explicit Value(const char *s, int len = 0);

  Value(const Value &other)            = default;
  Value &operator=(const Value &other) = default;

  void set_type(AttrType type) { this->attr_type_ = type; }
  void set_data(char *data, int length);
  void set_data(const char *data, int length) { this->set_data(const_cast<char *>(data), length); }
  void set_int(int val);
  void set_float(float val);
  void set_boolean(bool val);
  void set_date(Date date);
  void set_string(const char *s, int len = 0);
  void set_value(const Value &value);

  std::string to_string() const;

  int compare(const Value &other) const;

  static bool convert(AttrType new_type, Value &value); // convert Value type to new_type

  static bool check_value(const Value &value);

  const char *data() const;
  int         length() const { return length_; }

  AttrType attr_type() const { return attr_type_; }

public:
  /**
   * 获取对应的值
   * 如果当前的类型与期望获取的类型不符，就会执行转换操作
   */
  int         get_int() const;
  float       get_float() const;
  std::string get_string() const;
  Date        get_date() const;
  bool        get_boolean() const;
  // bool        is_null() const; 
  

private:
  AttrType attr_type_ = AttrType::UNDEFINED;
  int      length_    = 0;

  union
  {
    int   int_value_;
    float float_value_;
    Date date_value_;
    bool  bool_value_;
  } num_value_;
  std::string str_value_;
};

AttrType AttrTypeCompare(AttrType a, AttrType b);