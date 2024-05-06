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

    Json::Value to_json() const
    {
        Json::Value json_data;
        json_data["number of questions"] = number_of_questions;
        json_data["section name"] = section_name;
        json_data["section order"] = priority;
        return json_data;
    }
};

struct test_info {
    unsigned int duration; // in seconds
    unsigned int threshold_time; // in seconds
    unsigned int warning_time;   // in seconds
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
        warning_time = json_data.get("warning time", 0).asUInt();
        test_name = json_data.get("test name", "no name provided").asString();
        test_description = json_data.get("test description", "no test description provided").asString();

        // Deserialize the array of sections
        const Json::Value& __sections = json_data["sections"];
        for (unsigned short int i = 0; i < number_of_sections; ++i) {
            sections[i].from_json(__sections[i]);
        }
    }

    Json::Value to_json() const
    {
        Json::Value json_data;
        json_data["duration"] = duration;
        json_data["threshold time"] = threshold_time;
        json_data["warning time"] = warning_time;
        json_data["test name"] = test_name;
        json_data["test description"] = test_description;
        json_data["total number of sections"] = number_of_sections;

        // Serialize the array of sections
        Json::Value sections_array(Json::arrayValue);
        for (unsigned short int i = 0; i < number_of_sections; ++i)
        {
            sections_array.append(sections[i].to_json());
        }
        json_data["sections"] = sections_array;

        return json_data;
    }
};

struct data_to_save_in_result {
    unsigned short int section_order = 0;
    unsigned short int question_number = 0;
    std::string question_status = "NaN"; //not visited
    float time_spent = 0.0f; //in seconds
    std::string answer = "NaN";
};
