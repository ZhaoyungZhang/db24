/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 16:43:24
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-23 17:23:06
 * @FilePath: /miniob/src/observer/sql/stmt/drop_table_stmt.h
 */
#pragma once

#include <string>
#include <vector>

#include "sql/stmt/stmt.h"

class Db;

/**
 * @brief 删除表语句
 * @ingroup Statement
 * 
 */

class DropTableStmt : public Stmt {
public:
    DropTableStmt(const std::string &table_name) : table_name_(table_name) {}

    virtual ~DropTableStmt() = default;

    StmtType type() const override { return StmtType::DROP_TABLE; }

    const std::string &table_name() const { return table_name_; }

    // "create" drop table statement
    static RC create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt);

private:
    std::string table_name_;
};