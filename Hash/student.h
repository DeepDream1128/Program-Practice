#include <bits/stdc++.h>

struct Student {
    std::string name;
    std::string pinyin;
    int votes;
    std::string className;
    std::string major;
    int grade;
    std::string outstandingDeeds;
};

int hashFunction(const std::string& pinyin);
void readStudentData(const std::string& filename);
void insertStudent(const Student& student);
void voteForStudent(const std::string& pinyin);
void displayStudentInfo(const std::string& pinyin);
void displayVoteCount();
void displayRanking();
