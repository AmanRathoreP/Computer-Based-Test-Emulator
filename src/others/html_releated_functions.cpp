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
