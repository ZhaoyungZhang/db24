/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 16:47:14
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-23 16:55:58
 * @FilePath: /miniob/src/observer/sql/stmt/drop_table_stmt.cpp
 */
#include "sql/stmt/drop_table_stmt.h"
#include "common/log/log.h"
#include "drop_table_stmt.h"
#include "event/sql_debug.h"

RC DropTableStmt::create(Db *db, const DropTableSqlNode &drop_table, Stmt *&stmt) { 
    RC rc = RC::SUCCESS;
    stmt = new DropTableStmt(drop_table.relation_name);
    sql_debug("drop table statement: table name %s", drop_table.relation_name);
    return rc; 
}