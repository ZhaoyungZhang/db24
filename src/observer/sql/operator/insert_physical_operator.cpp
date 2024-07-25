/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 10:42:58
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-25 13:11:43
 * @FilePath: /miniob/src/observer/sql/operator/insert_physical_operator.cpp
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
// Created by WangYunlai on 2021/6/9.
//

#include "sql/operator/insert_physical_operator.h"
#include "sql/stmt/insert_stmt.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include "common/log/log.h"
#include "common/rc.h"
#include "insert_physical_operator.h"

using namespace std;

InsertPhysicalOperator::InsertPhysicalOperator(Table *table, vector<std::vector<Value> > &&values)
    : table_(table), values_(std::move(values))
{}

RC InsertPhysicalOperator::open(Trx *trx)
{
  vector<Record> values;
  RC     rc = mutli_insert(trx, values);
  if (rc != RC::SUCCESS) {
    RC new_rc = rollback(trx, values); // 如果插入失败，回滚已插入的记录
    if (new_rc != RC::SUCCESS) {
      LOG_WARN("rollback failed. rc=%s", strrc(new_rc));
      return RC::INTERNAL; // 如果回滚失败，返回内部错误状态码
    }
  }
  return rc; 
}

RC InsertPhysicalOperator::next() { return RC::RECORD_EOF; }

RC InsertPhysicalOperator::close() { return RC::SUCCESS; }

RC InsertPhysicalOperator::mutli_insert(Trx *trx, std::vector<Record> &values) {
    // 遍历所有插入的值 构造record 调用insert record插入
    values.reserve(values_.size());
    RC rc = RC::SUCCESS;
    for (auto &value : values_) {
        Record record;
        rc = table_->make_record(static_cast<int>(value.size()), value.data(), record);
        if (rc != RC::SUCCESS) {
          LOG_WARN("failed to make record. rc=%s", strrc(rc));
          return rc;
        }
        rc = trx->insert_record(table_, record);
        if (rc != RC::SUCCESS) {
          LOG_WARN("failed to insert record by transaction. rc=%s", strrc(rc));
          return rc;
        }
        values.push_back(record);
    }
    return rc;
}

RC InsertPhysicalOperator::rollback(Trx *trx, vector<Record> &values) {
  RC rc = RC::SUCCESS;
  for(auto it = values.rbegin(); it != values.rend(); it++) {
    rc = trx->delete_record(table_, *it);
    if (rc != RC::SUCCESS) {
      LOG_WARN("failed to delete record by transaction. rc=%s", strrc(rc));
      return RC::INTERNAL;
    }
  }
  return rc;
}

