#include "basic_csv_functions.h"

// Helper function to convert string to numeric type
template <typename T>
T convertStringToType(const std::string& str) {
    T value;
    std::istringstream iss(str);
    iss >> value;
    return value;
}

int get_column_index_by_name(rapidcsv::Document& doc, const std::string& columnName) {
    const auto& columnNames = doc.GetColumnNames();
    auto it = std::find(columnNames.begin(), columnNames.end(), columnName);

    if (it != columnNames.end()) {
        return static_cast<int>(std::distance(columnNames.begin(), it));
    }

    return -1;  // Column not found
}

int count_rows_with_specific_values(rapidcsv::Document& doc, const std::vector<std::string>& columnNames, const std::vector<std::string>& targetValues) {
    int rowCount = doc.GetRowCount();
    int columnCount = static_cast<int>(columnNames.size());

    if (columnCount != static_cast<int>(targetValues.size())) {
        std::cerr << "Number of column names does not match the number of target values." << std::endl;
        return -1;
    }

    int count = 0;

    for (int i = 0; i < rowCount; ++i) {
        bool match = true;

        for (int j = 0; j < columnCount; ++j) {
            int columnIndex = get_column_index_by_name(doc, columnNames[j]);

            if (columnIndex == -1) {
                std::cerr << "Column not found: " << columnNames[j] << std::endl;
                return -1;
            }

            try {
                std::string cellValue = doc.GetCell<std::string>(columnIndex, i);
                std::string targetValue = targetValues[j];

                // Convert string values to the appropriate type (e.g., int or float) if needed
                // Check if the cell value matches the target value
                if (cellValue != targetValue) {
                    match = false;
                    break;
                }
            }
            catch (const std::out_of_range& e) {
                std::cerr << "Out of range exception: " << e.what() << std::endl;
                return -1;
            }
            catch (const std::exception& e) {
                std::cerr << "Error occurred: " << e.what() << std::endl;
                return -1;
            }
        }

        if (match) {
            count++;
        }
    }

    return count;
}

int find_row_number(const rapidcsv::Document& doc, const std::string& column1, const std::string& value1, const std::string& column2, const std::string& value2) {
    // Iterate through the rows
    for (int row = 0; row < doc.GetRowCount(); ++row) {
        // Check if the values in the specified columns match the desired values
        if (doc.GetCell<std::string>(column1, row) == value1 &&
            doc.GetCell<std::string>(column2, row) == value2) {
            // Return the row number when a match is found
            return row;
        }
    }

    // Return -1 if no match is found
    return -1;
}