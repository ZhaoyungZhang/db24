/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 10:42:58
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-25 14:07:07
 * @FilePath: /miniob/src/observer/sql/stmt/insert_stmt.cpp
 */
/* Copyright (c) 2021OceanBase and/or its affiliates. All rights reserved.
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

#include "sql/stmt/insert_stmt.h"
#include "common/log/log.h"
#include "storage/db/db.h"
#include "storage/table/table.h"
#include "insert_stmt.h"

InsertStmt::InsertStmt(Table *table, std::vector<std::vector<Value>> records)
    : table_(table), records_(records), record_amount_(static_cast<int>(records.size())) {}

RC InsertStmt::create(Db *db, const InsertSqlNode &inserts, Stmt *&stmt)
{
  const char *table_name = inserts.relation_name.c_str();
  RC rc = RC::SUCCESS;
  if (nullptr == db || nullptr == table_name || inserts.values.empty()) {
    LOG_WARN("invalid argument. db=%p, table_name=%p, value_num=%d",
        db, table_name, static_cast<int>(inserts.values.size()));
    return (rc = RC::INVALID_ARGUMENT, rc);
  }
  
  // check whether the table exists
  Table *table = db->find_table(table_name);
  if (nullptr == table) {
    LOG_WARN("no such table. db=%s, table_name=%s", db->name(), table_name);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  // check the fields number
  const std::vector<std::vector<Value>> &records = inserts.values;
  for (auto &record : records) {
    rc = check_one_record(table, record);
    if (rc != RC::SUCCESS) {
      return rc;
    }
  }

  // everything alright
  stmt = new InsertStmt(table, records);
  return RC::SUCCESS;
}

RC InsertStmt::check_one_record(Table *table, const std::vector<Value> &record) { 
  // 检查每一行记录是否符合表的模式
  RC rc = RC::SUCCESS;
  const TableMeta &table_meta = table->table_meta();
  const char* table_name = table->name();
  const int field_num = table_meta.field_num();
  const int value_num = static_cast<int>(record.size());

  // check field number
  if (field_num != value_num) {
    LOG_WARN("schema mismatch. value num=%d, field num in schema=%d", value_num, field_num);
    return RC::SCHEMA_FIELD_MISSING;
  }

  // 处理每个属性
  for (int i = 0; i < value_num; i++) {
    const FieldMeta *field_meta = table_meta.field(i + table_meta.sys_field_num());
    const AttrType field_type = field_meta->type();
    if (!Value::convert(field_type, const_cast<Value &>(record[i]))) {
      LOG_WARN("field type mismatch. table=%s, field=%s, field type=%d, value_type=%d",
          table_name, field_meta->name(), field_type, record[i].attr_type());
      return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    }
  }
  return rc;
}