/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 16:40:42
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-23 17:43:05
 * @FilePath: /miniob/src/observer/sql/executor/drop_table_executor.cpp
 */
#include "sql/executor/drop_table_executor.h"

#include "common/log/log.h"
#include "event/session_event.h"
#include "event/sql_event.h"
#include "session/session.h"
#include "sql/stmt/drop_table_stmt.h"
#include "storage/db/db.h"

RC DropTableExecutor::execute(SQLStageEvent *sql_event){
    // 获取session and stmt
    Session *session = sql_event->session_event()->session();
    Stmt *stmt = sql_event->stmt();

    ASSERT(stmt->type() == StmtType::DROP_TABLE,
    "drop table executor can not run this command: %d",
    static_cast<int>(stmt->type()));

    DropTableStmt *drop_table_stmt = static_cast<DropTableStmt *>(stmt);

    const char *table_name = drop_table_stmt->table_name().c_str();
    // Trx *trx = session->current_trx();
    // Here we use the session's current db to drop the table
    RC rc = session->get_current_db()->drop_table(table_name);
    return rc;
}
