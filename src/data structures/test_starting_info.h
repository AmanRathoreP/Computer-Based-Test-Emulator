#pragma once
#ifndef STRUCT_HEADER
#define STRUCT_HEADER
#endif

#include <json/json.h>

struct test_section_info {
    unsigned short int number_of_questions;
    std::string section_name;
    unsigned short int priority;

    void from_json(const Json::Value& json_data) {
        number_of_questions = json_data.get("number of questions", 0).asInt();
        section_name = json_data.get("section name", "unidentified section").asString();
        priority = json_data.get("section order", 0).asInt();
    }
};

struct test_info {
    unsigned int duration; // in seconds
    unsigned int threshold_time; // in seconds
    std::string test_name;
    std::string test_description;
    std::string student_test_result_file;

    unsigned short int number_of_sections;
    test_section_info* sections;
    void from_json(const Json::Value& json_data) {
        number_of_sections = json_data.get("total number of sections", 0).asUInt();
        sections = new test_section_info[number_of_sections];
        duration = json_data.get("duration", 0).asUInt();
        threshold_time = json_data.get("threshold time", 0).asUInt();
        test_name = json_data.get("test name", "no name provided").asString();
        test_description = json_data.get("test description", "no test description provided").asString();

        // Deserialize the array of sections
        const Json::Value& __sections = json_data["sections"];
        for (unsigned short int i = 0; i < number_of_sections; ++i) {
            sections[i].from_json(__sections[i]);
        }
    }
};