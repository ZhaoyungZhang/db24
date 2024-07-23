/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 10:42:58
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-23 21:45:10
 * @FilePath: /miniob/src/observer/sql/operator/physical_operator.h
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
// Created by WangYunlai on 2022/6/7.
//

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "common/rc.h"
#include "sql/expr/tuple.h"

class Record;
class TupleCellSpec;
class Trx;

/**
 * @brief 物理算子
 * @defgroup PhysicalOperator
 * @details 物理算子描述执行计划将如何执行，比如从表中怎么获取数据，如何做投影，怎么做连接等
 */

/**
 * @brief 物理算子类型
 * @ingroup PhysicalOperator
 */
enum class PhysicalOperatorType
{
  TABLE_SCAN,       // 表扫描: 逐行读取整个表。
  INDEX_SCAN,       // 索引扫描
  NESTED_LOOP_JOIN, // 嵌套循环连接
  EXPLAIN,          // 描述执行计划
  PREDICATE,        // 过滤: 应用过滤条件 (如 WHERE 子句)
  PROJECT,          // 投影: 选择特定的列（如 SELECT 子句）
  CALC,             // 计算: 执行计算或表达式求值
  STRING_LIST,      // 字符串列表: 处理字符串列表操作
  DELETE,           // 删除: 删除表中的记录
  INSERT,           // 插入: 插入元组
  SCALAR_GROUP_BY,  // 标量分组
  HASH_GROUP_BY,    // 哈希分组
  AGGREGATE,        // 聚合: 对数据进行聚合操作
  TABLE_SCAN_VEC,   // 表扫描(向量化)
  PREDICATE_VEC,    // 过滤(向量化)
  PROJECT_VEC,      // 投影(向量化)
  GROUP_BY_VEC,     // 分组(向量化)
  AGGREGATE_VEC,    // 聚合(向量化)
  EXPR_VEC,         // 表达式(向量化)
};

/**
 * @brief 与LogicalOperator对应，物理算子描述执行计划将如何执行
 * @ingroup PhysicalOperator
 */
class PhysicalOperator
{
public:
  PhysicalOperator() = default;

  virtual ~PhysicalOperator() = default;

  /**
   * 这两个函数是为了打印时使用的，比如在explain中
   */
  virtual std::string name() const;
  virtual std::string param() const;

  virtual PhysicalOperatorType type() const = 0;

  virtual RC open(Trx *trx) = 0;
  virtual RC next() { return RC::UNIMPLENMENT; }
  virtual RC next(Chunk &chunk) { return RC::UNIMPLENMENT; }
  virtual RC close() = 0;

  virtual Tuple *current_tuple() { return nullptr; }

  virtual RC tuple_schema(TupleSchema &schema) const { return RC::UNIMPLENMENT; }

  void add_child(std::unique_ptr<PhysicalOperator> oper) { children_.emplace_back(std::move(oper)); }

  std::vector<std::unique_ptr<PhysicalOperator>> &children() { return children_; }

protected:
  std::vector<std::unique_ptr<PhysicalOperator>> children_;
};
