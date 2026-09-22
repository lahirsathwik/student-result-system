#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include "httplib.h"

using namespace std;

struct Student {
    string roll;
    string name;
    int c;
    int cpp;
    int python;
    int dbms;
    int ai;
};

vector<Student> students = {
    {"25B11AIC10", "Sathwik", 85, 78, 90, 82, 88},
    {"25B11AIC11", "Rahul", 75, 80, 85, 78, 82},
    {"25B11AIC12", "Anjali", 90, 88, 92, 85, 91}
};
void saveStudents() {

    ofstream file("students.txt");

    for (const auto& student : students) {

        file << student.roll << "|"
             << student.name << "|"
             << student.c << "|"
             << student.cpp << "|"
             << student.python << "|"
             << student.dbms << "|"
             << student.ai << "\n";
    }

   file.close();
}
void loadStudents() {

    ifstream file("students.txt");

    if (!file.is_open()) {
        return;
    }

    students.clear();

    string line;

    while (getline(file, line)) {

        Student student;

        size_t pos = 0;
        vector<string> data;

        while ((pos = line.find("|")) != string::npos) {

            data.push_back(line.substr(0, pos));
            line.erase(0, pos + 1);
        }

        data.push_back(line);

        if (data.size() == 7) {

            student.roll = data[0];
            student.name = data[1];
            student.c = stoi(data[2]);
            student.cpp = stoi(data[3]);
            student.python = stoi(data[4]);
            student.dbms = stoi(data[5]);
            student.ai = stoi(data[6]);

            students.push_back(student);
        }
    }

    file.close();
}

string getGrade(double percentage) {
    if (percentage >= 90) return "A+";
    if (percentage >= 80) return "A";
    if (percentage >= 70) return "B";
    if (percentage >= 60) return "C";
    if (percentage >= 50) return "D";
    return "F";
}

int main() {
 loadStudents();
    httplib::Server server;
    // Admin Login
server.Post("/api/login", [](const httplib::Request& req,
                             httplib::Response& res) {

    string username = "";
    string password = "";

    if (req.has_param("username")) {
        username = req.get_param_value("username");
    }

    if (req.has_param("password")) {
        password = req.get_param_value("password");
    }

    if (username == "admin" && password == "1234") {

        res.set_content(
            "{\"message\":\"Login successful\"}",
            "application/json"
        );

    } else {

        res.status = 401;

        res.set_content(
            "{\"message\":\"Invalid username or password\"}",
            "application/json"
        );
    }
});
// Add Student
server.Post("/api/add", [](const httplib::Request& req,
                           httplib::Response& res) {

    if (!req.has_param("roll") ||
        !req.has_param("name") ||
        !req.has_param("c") ||
        !req.has_param("cpp") ||
        !req.has_param("python") ||
        !req.has_param("dbms") ||
        !req.has_param("ai")) {

        res.status = 400;
        res.set_content(
            "{\"message\":\"Please fill all fields\"}",
            "application/json"
        );
        return;
    }

    Student student;

    student.roll = req.get_param_value("roll");
    student.name = req.get_param_value("name");
    if (student.roll.empty() || student.name.empty()) {

    res.status = 400;

    res.set_content(
        "{\"message\":\"Roll number and student name are required\"}",
        "application/json"
    );

    return;
}

    student.c = stoi(req.get_param_value("c"));
    student.cpp = stoi(req.get_param_value("cpp"));
    student.python = stoi(req.get_param_value("python"));
    student.dbms = stoi(req.get_param_value("dbms"));
    student.ai = stoi(req.get_param_value("ai"));
    if (student.c < 0 || student.c > 100 ||
    student.cpp < 0 || student.cpp > 100 ||
    student.python < 0 || student.python > 100 ||
    student.dbms < 0 || student.dbms > 100 ||
    student.ai < 0 || student.ai > 100) {

    res.status = 400;

    res.set_content(
        "{\"message\":\"Marks must be between 0 and 100\"}",
        "application/json"
    );

    return;
}
for (const auto& existing : students) {

    if (existing.roll == student.roll) {

        res.status = 409;

        res.set_content(
            "{\"message\":\"Roll number already exists\"}",
            "application/json"
        );

        return;
    }
}

students.push_back(student);

saveStudents();

    res.set_content(
        "{\"message\":\"Student added successfully\"}",
        "application/json"
    );
});
// View all students
server.Get("/api/students", [](const httplib::Request& req,
                               httplib::Response& res) {

    string json = "[";

    for (size_t i = 0; i < students.size(); i++) {

        const auto& student = students[i];

        int total =
            student.c +
            student.cpp +
            student.python +
            student.dbms +
            student.ai;

        double percentage = total / 5.0;

        string grade = getGrade(percentage);

        string result =
            (grade == "F") ? "FAIL" : "PASS";

        json += "{";
        json += "\"roll\":\"" + student.roll + "\",";
        json += "\"name\":\"" + student.name + "\",";
        json += "\"c\":" + to_string(student.c) + ",";
        json += "\"cpp\":" + to_string(student.cpp) + ",";
        json += "\"python\":" + to_string(student.python) + ",";
        json += "\"dbms\":" + to_string(student.dbms) + ",";
        json += "\"ai\":" + to_string(student.ai) + ",";
        json += "\"total\":" + to_string(total) + ",";
        json += "\"percentage\":" + to_string(percentage) + ",";
        json += "\"grade\":\"" + grade + "\",";
        json += "\"result\":\"" + result + "\"";
        json += "}";

        if (i < students.size() - 1) {
            json += ",";
        }
    }

    json += "]";

    res.set_content(json, "application/json");
});
// Update student result
server.Post("/api/update", [](const httplib::Request& req,
                              httplib::Response& res) {

    if (!req.has_param("roll") ||
        !req.has_param("name") ||
        !req.has_param("c") ||
        !req.has_param("cpp") ||
        !req.has_param("python") ||
        !req.has_param("dbms") ||
        !req.has_param("ai")) {

        res.status = 400;

        res.set_content(
            "{\"message\":\"Please fill all fields\"}",
            "application/json"
        );

        return;
    }

    string roll = req.get_param_value("roll");

    for (auto& student : students) {

        if (student.roll == roll) {

            student.name =
                req.get_param_value("name");

            student.c =
                stoi(req.get_param_value("c"));

            student.cpp =
                stoi(req.get_param_value("cpp"));

            student.python =
                stoi(req.get_param_value("python"));

            student.dbms =
                stoi(req.get_param_value("dbms"));

            student.ai =
                stoi(req.get_param_value("ai"));

            saveStudents();

            res.set_content(
                "{\"message\":\"Student result updated successfully\"}",
                "application/json"
            );

            return;
        }
    }

    res.status = 404;

    res.set_content(
        "{\"message\":\"Student not found\"}",
        "application/json"
    );
});
// Delete student
server.Post("/api/delete", [](const httplib::Request& req,
                              httplib::Response& res) {

    if (!req.has_param("roll")) {

        res.status = 400;

        res.set_content(
            "{\"message\":\"Roll number is required\"}",
            "application/json"
        );

        return;
    }

    string roll = req.get_param_value("roll");

    for (auto it = students.begin(); it != students.end(); ++it) {

        if (it->roll == roll) {

            students.erase(it);

            saveStudents();

            res.set_content(
                "{\"message\":\"Student deleted successfully\"}",
                "application/json"
            );

            return;
        }
    }

    res.status = 404;

    res.set_content(
        "{\"message\":\"Student not found\"}",
        "application/json"
    );
});
    // Search student result
    server.Get("/api/student", [](const httplib::Request& req,
                                  httplib::Response& res) {

        if (!req.has_param("roll")) {
            res.status = 400;
            res.set_content(
                "{\"message\":\"Roll number is required\"}",
                "application/json"
            );
            return;
        }

        string roll = req.get_param_value("roll");

        for (const auto& student : students) {

            if (student.roll == roll) {

                int total =
                    student.c +
                    student.cpp +
                    student.python +
                    student.dbms +
                    student.ai;

                double percentage = total / 5.0;

                string grade = getGrade(percentage);

                string result =
                    (grade == "F") ? "FAIL" : "PASS";

                string json =
                    "{"
                    "\"name\":\"" + student.name + "\","
                    "\"roll\":\"" + student.roll + "\","
                    "\"c\":" + to_string(student.c) + ","
                    "\"cpp\":" + to_string(student.cpp) + ","
                    "\"python\":" + to_string(student.python) + ","
                    "\"dbms\":" + to_string(student.dbms) + ","
                    "\"ai\":" + to_string(student.ai) + ","
                    "\"total\":" + to_string(total) + ","
                    "\"percentage\":" + to_string(percentage) + ","
                    "\"grade\":\"" + grade + "\","
                    "\"result\":\"" + result + "\""
                    "}";

                res.set_content(json, "application/json");
                return;
            }
        }

        res.status = 404;

        res.set_content(
            "{\"message\":\"Student not found\"}",
            "application/json"
        );
    });
    server.set_mount_point("/", "../");

cout << "Student Result System Backend Started!" << endl;

int port = 10000;

if (getenv("PORT")) {
    port = stoi(getenv("PORT"));
}
cout << "Server running on port " << port << endl;

server.listen("0.0.0.0", port);

    return 0;
}