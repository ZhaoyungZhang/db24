/*
 * @Author: ZhaoyangZhang
 * @Date: 2024-07-24 09:46:21
 * @LastEditors: Do not edit
 * @LastEditTime: 2024-07-24 17:40:40
 * @FilePath: /miniob/src/observer/sql/parser/date.h
 */
#pragma once

#include <string>

// DATE FORMAT : 2024-07-24
const int kYearBegin = 1970;
const int kYearEnd = 2038;
const int kYearCount = kYearEnd - kYearBegin + 1;
const int kMonthCount = 12;
const int kSecondsInDay = 24 * 60 * 60;


class Date {
public:
    int value;
    Date() = default;
    Date(int val) : value(val) {}
    Date(const std::string &s);
    auto operator<=>(const Date &d) const = default;
    
    static std::string to_string(const Date &date);
    static int compare_date(const Date *a, const Date *b);
};

const Date INVALID_DATE(-1);
// inline bool is_leap_year(int year){
//     return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0; 
// }

// inline RC string_to_date(const char *str, int32_t &date) {
//     int year = 0;
//     int month = 0;
//     int day = 0;
//     int ret = sscanf(str, "%d-%d-%d", &year, &month, &day);
//     if (ret != 3) {
//         return RC::INVALID_ARGUMENT;
//     }

//     // 验证
//     if (year < 1900 || year > 9999 ||
//          (month < 1 || month > 12) ||  
//          (day < 1 || day > 31) ) {
//         return RC::INVALID_ARGUMENT;
//     }

//     int max_days_in_month[] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
//     const int max_days = max_days_in_month[month - 1];
//     if (day > max_days) {
//         return RC::INVALID_ARGUMENT;
//     }
//     if (month == 2 && day == 29 && !is_leap_year(year)){
//         return RC::INVALID_ARGUMENT;
//     }
//     date = year * 1000 + month * 100 + day;
//     return RC::SUCCESS;
// }

// inline std::string date_to_string(int32_t date) {
//     int year = date / 10000;
//     int month = (date % 10000) / 100;
//     int day = date % 100;

//     char buffer[11];
//     snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);

//     return std::string(buffer);
// }
