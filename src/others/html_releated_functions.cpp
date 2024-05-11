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

void htmlGenerator::create_detailed_validity_report(rapidcsv::Document doc, const std::string html_file_name, const std::string test_folder_path)
{
    // todo use section name instead of section order in generated html
    // due to some unidentified reason replacing via std::string was not not working so making use of wxString

    std::stringstream __html_stream;
    __html_stream << detailed_analysis_of_validity_boiler_plate;

    const char __path_separator = '\\';
    for (unsigned short int row = 0; row < doc.GetRowCount(); row++)
    {
        auto question_data = doc.GetRow<std::string>(row);
        std::string __question_table_row = "";

        if (question_data[8].compare("0") == 0)
            __question_table_row = validity_table_row_boiler_plates[doc.GetCell<std::string>(4, row) + std::string("i")];
        else if (question_data[8].compare("1") == 0)
            __question_table_row = validity_table_row_boiler_plates[doc.GetCell<std::string>(4, row) + std::string("c")];
        else
            __question_table_row = validity_table_row_boiler_plates[doc.GetCell<std::string>(4, row)];

        wxString __question_table_row_wx = wxString(__question_table_row);
        __question_table_row_wx.Replace(wxT("$(section_name)"), question_data[1]);
        __question_table_row_wx.Replace(wxT("$(question_number)"), question_data[2]);
        __question_table_row_wx.Replace(wxT("$(question_image_path)"), test_folder_path + __path_separator + question_data[0]);
        __question_table_row_wx.Replace(wxT("$(question_image_width)"), std::to_string(1000));
        __question_table_row_wx.Replace(wxT("$(alt_question_image)"), question_data[0] + " not found!");
        __question_table_row_wx.Replace(wxT("$(assumed_answer)"), question_data[6]);
        __question_table_row_wx.Replace(wxT("$(correct_answer)"), question_data[7]);
        __question_table_row_wx.Replace(wxT("$(time_taken)"), std::to_string((int)(std::stof(question_data[5]) / 60)) + " min " + std::to_string((std::stoi(question_data[5]) % 60)) + " s");
        __question_table_row_wx.Replace(wxT("$(time_taken_sec)"), std::to_string((std::stoi(question_data[5]))));

        __html_stream << __question_table_row_wx.ToStdString();
    }

    __html_stream << "</tbody></table></div>";
    this->write_to_file(__html_stream.str(), test_folder_path + __path_separator + html_file_name);
}

void htmlGenerator::create_summary_validity_report(rapidcsv::Document doc, const std::string html_file_name, const std::string test_folder_path, const float marks_for_correct_answer, const float marks_for_wrong_answer)
{
    // todo use section name instead of section order in generated html
    // due to some unidentified reason replacing via std::string was not not working so making use of wxString

    std::stringstream __html_stream;
    __html_stream << validity_summary_boiler_plate;

    const char __path_separator = '\\';
    auto section_orders_vec_str = doc.GetColumn<std::string>("section_order");
    std::vector<unsigned short int> section_orders_vec;
    section_orders_vec.reserve(section_orders_vec_str.size());
    for (const auto &str : section_orders_vec_str)
        section_orders_vec.push_back(std::stoi(str));
    auto max_section_order = std::max_element(section_orders_vec.begin(), section_orders_vec.end());
    float total_marks = 0;
    float total_AC = 0;
    float total_ARC = 0;
    float total_AI = 0;
    float total_ARI = 0;
    float total_NA = 0;
    float total_NV = 0;
    float total_TS = 0;
    for (unsigned short int section_order = 1; section_order <= *max_section_order; section_order++)
    {
        auto __section_stats_table_row = wxString(validity_summary_replaceable_text_for_section);

        auto section_AC = count_rows_with_specific_values(doc, {"validity", "question_status", "section_order"}, {"1", "a", std::to_string(section_order)}) * marks_for_correct_answer;
        auto section_ARC = count_rows_with_specific_values(doc, {"validity", "question_status", "section_order"}, {"1", "ar", std::to_string(section_order)}) * marks_for_correct_answer;
        auto section_AI = count_rows_with_specific_values(doc, {"validity", "question_status", "section_order"}, {"0", "a", std::to_string(section_order)}) * marks_for_wrong_answer;
        auto section_ARI = count_rows_with_specific_values(doc, {"validity", "question_status", "section_order"}, {"0", "ar", std::to_string(section_order)}) * marks_for_wrong_answer;
        auto section_NA = (count_rows_with_specific_values(doc, {"question_status", "section_order"}, {"n", std::to_string(section_order)}) + count_rows_with_specific_values(doc, {"question_status", "section_order"}, {"nr", std::to_string(section_order)})) * 0.0;
        auto section_NV = count_rows_with_specific_values(doc, {"question_status", "section_order"}, {"nv", std::to_string(section_order)}) * 0.0;
        auto section_TS = section_AC + section_ARC + section_AI + section_ARI + section_NA + section_NV;
        total_AC += section_AC;
        total_ARC += section_ARC;
        total_AI += section_AI;
        total_ARI += section_ARI;
        total_NA += section_NA;
        total_NV += section_NV;
        total_TS += section_TS;

        __section_stats_table_row.Replace(wxT("$(section_name)"), std::to_string(section_order));
        __section_stats_table_row.Replace(wxT("$(section_AC)"), wxString::Format("%.2f", section_AC));
        __section_stats_table_row.Replace(wxT("$(section_ARC)"), wxString::Format("%.2f", section_ARC));
        __section_stats_table_row.Replace(wxT("$(section_AI)"), wxString::Format("%.2f", section_AI));
        __section_stats_table_row.Replace(wxT("$(section_ARI)"), wxString::Format("%.2f", section_ARI));
        __section_stats_table_row.Replace(wxT("$(section_NA)"), wxString::Format("%.2f", section_NA));
        __section_stats_table_row.Replace(wxT("$(section_NV)"), wxString::Format("%.2f", section_NV));
        __section_stats_table_row.Replace(wxT("$(TS)"), wxString::Format("%.2f", section_TS));

        total_marks += section_TS;
        __html_stream << __section_stats_table_row.ToStdString();
    }
    wxString sub_total_html = wxString(validity_summary_replaceable_text_for_sub_total);
    wxString total_html = wxString(validity_summary_replaceable_text_for_total);

    total_html.Replace(wxT("$(AC)"), wxString::Format("%.2f", total_AC));
    total_html.Replace(wxT("$(ARC)"), wxString::Format("%.2f", total_ARC));
    total_html.Replace(wxT("$(AI)"), wxString::Format("%.2f", total_AI));
    total_html.Replace(wxT("$(ARI)"), wxString::Format("%.2f", total_ARI));
    total_html.Replace(wxT("$(NA)"), wxString::Format("%.2f", total_NA));
    total_html.Replace(wxT("$(NV)"), wxString::Format("%.2f", total_NV));
    total_html.Replace(wxT("$(TS)"), wxString::Format("%.2f", total_TS));
    total_html.Replace(wxT("$(TM)"), wxString::Format("%.2f", total_marks));

    sub_total_html.Replace(wxT("$(CC)"), wxString::Format("%.2f", total_AC + total_ARC));
    sub_total_html.Replace(wxT("$(II)"), wxString::Format("%.2f", total_AI + total_ARI));
    sub_total_html.Replace(wxT("$(NN)"), wxString::Format("%.2f", total_NA + total_NV));
    sub_total_html.Replace(wxT("$(TM)"), wxString::Format("%.2f", total_marks));

    __html_stream << total_html.ToStdString();
    __html_stream << sub_total_html.ToStdString();

    this->write_to_file(__html_stream.str(), test_folder_path + __path_separator + html_file_name);
}
