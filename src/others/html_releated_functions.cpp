#include "html_releated_functions.h"

void htmlGenerator::create_test_summary(rapidcsv::Document doc, std::string html_file_name, const test_info test_starting_data)
{
    std::stringstream __html_stream;

    __html_stream << summary_boiler_plate;

    for (unsigned short int section_order = 1; section_order <= test_starting_data.number_of_sections; section_order++)
    {
        std::string __section_info = this->summary_replaceable_text_for_section;

        __section_info.replace(__section_info.find("$(section_name)"), sizeof("$(section_name)") - 1, test_starting_data.sections[section_order - 1].section_name);
        __section_info.replace(__section_info.find("$(answered)"), sizeof("$(answered)") - 1, std::to_string(count_rows_with_specific_values(doc, {"section_order", "question_status"}, {std::to_string(section_order), "a"})));
        __section_info.replace(__section_info.find("$(answered_scrutinize)"), sizeof("$(answered_scrutinize)") - 1, std::to_string(count_rows_with_specific_values(doc, {"section_order", "question_status"}, {std::to_string(section_order), "ar"})));
        __section_info.replace(__section_info.find("$(not_answered)"), sizeof("$(not_answered)") - 1, std::to_string(count_rows_with_specific_values(doc, {"section_order", "question_status"}, {std::to_string(section_order), "n"})));
        __section_info.replace(__section_info.find("$(not_answered_scrutinize)"), sizeof("$(not_answered_scrutinize)") - 1, std::to_string(count_rows_with_specific_values(doc, {"section_order", "question_status"}, {std::to_string(section_order), "nr"})));
        __section_info.replace(__section_info.find("$(not_visited)"), sizeof("$(not_visited)") - 1, std::to_string(count_rows_with_specific_values(doc, {"section_order", "question_status"}, {std::to_string(section_order), "nv"})));
        __section_info.replace(__section_info.find("$(total_questions)"), sizeof("$(total_questions)") - 1, std::to_string(count_rows_with_specific_values(doc, {"section_order"}, {std::to_string(section_order)})));

        __html_stream << __section_info;
    }

    std::string __test_info = this->summary_replaceable_text_for_total;
    __test_info.replace(__test_info.find("$(answered)"), sizeof("$(answered)") - 1, std::to_string(count_rows_with_specific_values(doc, {"question_status"}, {"a"})));
    __test_info.replace(__test_info.find("$(answered_scrutinize)"), sizeof("$(answered_scrutinize)") - 1, std::to_string(count_rows_with_specific_values(doc, {"question_status"}, {"ar"})));
    __test_info.replace(__test_info.find("$(not_answered)"), sizeof("$(not_answered)") - 1, std::to_string(count_rows_with_specific_values(doc, {"question_status"}, {"n"})));
    __test_info.replace(__test_info.find("$(not_answered_scrutinize)"), sizeof("$(not_answered_scrutinize)") - 1, std::to_string(count_rows_with_specific_values(doc, {"question_status"}, {"nr"})));
    __test_info.replace(__test_info.find("$(not_visited)"), sizeof("$(not_visited)") - 1, std::to_string(count_rows_with_specific_values(doc, {"question_status"}, {"nv"})));
    __test_info.replace(__test_info.find("$(total_questions)"), sizeof("$(total_questions)") - 1, std::to_string(doc.GetRowCount()));

    __html_stream << __test_info;
     this->write_to_file(__html_stream.str(), html_file_name);
}

void htmlGenerator::create_detailed_test_analysis(rapidcsv::Document doc, std::string html_file_name, const test_info test_starting_data)
{
    std::stringstream __html_stream;
    __html_stream << detailed_analysis_boiler_plate;

    unsigned short int question_row_number;
    const char __path_separator = '\\';

    for (unsigned short int section_order = 1; section_order <= test_starting_data.number_of_sections; section_order++)
    {
        for (unsigned short int question_number = 1; question_number <= test_starting_data.sections[section_order - 1].number_of_questions; question_number++)
        {
             question_row_number = find_row_number(doc, "section_order", std::to_string(section_order), "question_number", std::to_string(question_number));
            std::string __question_table_row = table_row_boiler_plates[doc.GetCell<std::string>(4, question_row_number)];
            std::string question_image_file_path = test_starting_data.student_test_result_file;
            question_image_file_path = question_image_file_path.erase(question_image_file_path.find_last_of(__path_separator) + 1) + __path_separator + doc.GetCell<std::string>(0, question_row_number);

            __question_table_row.replace(__question_table_row.find("$(section_name)"), sizeof("$(section_name)") - 1, test_starting_data.sections[section_order - 1].section_name);
            __question_table_row.replace(__question_table_row.find("$(question_number)"), sizeof("$(question_number)") - 1, std::to_string(std::stoi(doc.GetCell<std::string>(2, question_row_number))));
            __question_table_row.replace(__question_table_row.find("$(question_image_path)"), sizeof("$(question_image_path)") - 1, question_image_file_path);
            __question_table_row.replace(__question_table_row.find("$(question_image_width)"), sizeof("$(question_image_width)") - 1, std::to_string(1150));
            //__question_table_row.replace(__question_table_row.find("$(question_image_height)"), sizeof("$(question_image_height)") - 1, std::to_string(800));
            __question_table_row.replace(__question_table_row.find("$(assumed_answer)"), sizeof("$(assumed_answer)") - 1, doc.GetCell<std::string>(6, question_row_number));
            __question_table_row.replace(__question_table_row.find("$(time_taken)"), sizeof("$(time_taken)") - 1, std::to_string(std::stoi(doc.GetCell<std::string>(5, question_row_number)) / 60) + " min " + std::to_string(std::stoi(doc.GetCell<std::string>(5, question_row_number)) % 60) + " s");
            
            __html_stream << __question_table_row;

        }
    }

    __html_stream << "</tbody></table></div>";
    this->write_to_file(__html_stream.str(), html_file_name);
}

bool inline htmlGenerator::write_to_file(std::string file_content, std::string file_name)
{
    std::ofstream output_file(file_name);
    if (output_file.is_open())
    {
        output_file << file_content;
        output_file.close();
        return true;
    }
    else
        return false;
}
