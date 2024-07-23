/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 10:42:58
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-23 23:19:36
 * @FilePath: /miniob/src/observer/sql/stmt/update_stmt.h
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
// Created by Wangyunlai on 2022/5/22.
//

#pragma once

#include "common/rc.h"
#include "sql/expr/expression.h"
#include "sql/stmt/stmt.h"
#include "sql/stmt/filter_stmt.h"
#include "storage/field/field.h"

class Table;

/**
 * @brief 更新单元，表示要更新的字段和值
 */
/**
 * @brief 更新单元，表示要更新的字段和值
 */
class UpdateUnit {
public:
  UpdateUnit(Field field, Value value) 
    : field_(std::move(field)), value_(std::move(value)) {}

  // Getter and Setter for field
  const Field &field() const { return field_; }
  void set_field(const Field &field) { field_ = field; }

  // Getter and Setter for value
  const Value &value() const { return value_; }
  void set_value(const Value &value) { value_ = value; }

private:
  Field field_;
  Value value_;   // 可能要换成表达式
};


/**
 * @brief 更新语句
 * @ingroup Statement
 */
class UpdateStmt : public Stmt
{
public:
  UpdateStmt() = default;
  // UpdateStmt(Table *table, Value *values, int value_amount);
  UpdateStmt(Table *table, std::vector<UpdateUnit> units, FilterStmt *filter);

public:
  static RC create(Db *db, const UpdateSqlNode &update_sql, Stmt *&stmt);
  StmtType type() const override { return StmtType::UPDATE; }

public:
  Table *table() const { return table_; }
  const std::vector<UpdateUnit> &units() const { return units_; }
  FilterStmt *filter() const { return filter_; }

  // Value *values() const { return values_; }
  // int    value_amount() const { return value_amount_; }

private:
  Table *table_        = nullptr;   // 要更新的表
  std::vector<UpdateUnit> units_;   // 要更新的单元
  FilterStmt *filter_  = nullptr;   // 过滤条件
  // Value *values_       = nullptr;   // 要更新的值
  // int    value_amount_ = 0;         // 要更新的值的数量
};
