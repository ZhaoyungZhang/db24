/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-23 16:40:32
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-23 16:41:18
 * @FilePath: /miniob/src/observer/sql/executor/drop_table_executor.h
 */
#pragma once

#include "common/rc.h"

class SQLStageEvent;

/**
 * @brief 删除表执行器
 * @ingroup Executor
 */
class DropTableExecutor {
public:
    DropTableExecutor()          = default;
    RC execute(SQLStageEvent *sql_event);
    virtual ~DropTableExecutor() = default;
};