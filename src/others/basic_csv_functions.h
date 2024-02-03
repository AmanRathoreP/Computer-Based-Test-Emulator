#pragma once
#include <vector>
#include <rapidcsv.h>

int get_column_index_by_name(rapidcsv::Document& doc, const std::string& columnName);
int count_rows_with_specific_values(rapidcsv::Document& doc, const std::vector<std::string>& columnNames, const std::vector<std::string>& targetValues);
