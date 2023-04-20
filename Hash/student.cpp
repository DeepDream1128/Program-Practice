#include "student.h"

std::map<std::string, Student> hashTable;

int hashFunction(const std::string &pinyin) {
  int sum = 0;
  for (char c : pinyin) {
    sum += c;
  }
  return sum % (hashTable.size()?hashTable.size():1);
}

void insertStudent(const Student &student) {
  int hashIndex = hashFunction(student.pinyin);
  while (hashTable.find(student.pinyin) != hashTable.end()) {
    hashIndex++;
  }
  hashTable.insert({student.pinyin, student});
}

void voteForStudent(const std::string &pinyin) {
  auto it = hashTable.find(pinyin);
  if (it != hashTable.end()) {
    it->second.votes++;
  } else {
    // 插入新学生
    Student student;
    student.pinyin = pinyin;
    student.votes = 1;
    insertStudent(student);

  }
}

void displayStudentInfo(const std::string &pinyin) {
  auto it = hashTable.find(pinyin);
  if (it != hashTable.end()) {
    Student &student = it->second;
    std::cout << "姓名: " << student.name << std::endl;
    std::cout << "班级: " << student.className << std::endl;
    std::cout << "专业: " << student.major << std::endl;
    std::cout << "年级: " << student.grade << std::endl;
    std::cout << "突出事迹: " << student.outstandingDeeds << std::endl;
  } else {
    std::cout << "没有找到该学生的信息" << std::endl;
  }
}
void displayVoteCount() {
  for (const auto &entry : hashTable) {
    const Student &student = entry.second;
    std::cout << "姓名: " << student.name << " 票数: " << student.votes
              << std::endl;
  }
}

void displayRanking() {
  std::vector<Student> students;
  for (const auto &entry : hashTable) {
    students.push_back(entry.second);
  }

  std::sort(
      students.begin(), students.end(),
      [](const Student &a, const Student &b) { return a.votes > b.votes; });

  std::cout << "排行榜前10名: " << std::endl;
  for (size_t i = 0; i < 10 && i < students.size(); i++) {
    std::cout << "姓名: " << students[i].name << " 票数: " << students[i].votes
              << std::endl;
  }
}
