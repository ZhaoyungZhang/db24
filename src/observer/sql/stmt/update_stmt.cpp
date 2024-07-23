/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 10:42:58
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-23 23:07:21
 * @FilePath: /miniob/src/observer/sql/stmt/update_stmt.cpp
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

#include "sql/stmt/update_stmt.h"
#include "sql/expr/expression.h"
#include "sql/parser/parse_defs.h"
#include "storage/db/db.h"

#include "common/log/log.h"
#include "common/rc.h"
#include "common/lang/string.h"

UpdateStmt::UpdateStmt(Table *table, std::vector<UpdateUnit> units, FilterStmt *filter)
    : table_(table), units_(std::move(units)), filter_(filter)
{}



/**
 * @brief 创建 UpdateStmt 对象
 * @todo 支持多个字段的更新
 */
RC UpdateStmt::create(Db *db, const UpdateSqlNode &update, Stmt *&stmt) {
  // 1. 查找要更新的表
  Table *table = db->find_table(update.relation_name.c_str());
  if (table == nullptr) {
    LOG_WARN("No such table. db=%s, table_name=%s", db->name(), update.relation_name.c_str());
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  RC rc = RC::SUCCESS;
  // table map for filter expr
  std::unordered_map<std::string, Table *> table_lookup;
  table_lookup.emplace(table->name(), table);
  
  // 2. 查找要更新的filed
  const FieldMeta *field_metadata = table->table_meta().field(update.attribute_name.c_str()); 
  if(field_metadata == nullptr){
    LOG_WARN("No such field. db=%s, table_name=%s, field_name=%s", db->name(), table->name(), update.attribute_name.c_str());
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }

  // 3. 检查值的有效性 (不能非空列插入null)
  // if(update.value)

  // 4. 创建待更新的单元
  std::vector<UpdateUnit> update_units;
  update_units.emplace_back(Field(table, field_metadata), update.value);

  // 5. 创建filter stmt
  FilterStmt *filter_stmt = nullptr;
  if(!update.conditions.empty()){
    rc = FilterStmt::create(db, table, &table_lookup, update.conditions.data(), update.conditions.size(), filter_stmt); 
    if(rc != RC::SUCCESS){
      LOG_WARN("Failed to create filter stmt. rc=%s", strrc(rc));
      return rc;
    }
  }

  // 6. 创建UpdateStmt对象
  stmt = new UpdateStmt(table, std::move(update_units), filter_stmt);

  return rc;
}
