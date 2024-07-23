#pragma once

#include "sql/operator/logical_operator.h"
#include "sql/stmt/update_stmt.h"
#include "storage/field/field.h"


class UpdateLogicalOperator : public LogicalOperator {
public:

private:
    Table *table_;
    
};