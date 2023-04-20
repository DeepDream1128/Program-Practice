#include "student.h"

int main() {
  // 初始化评选系统，添加测试用例等

  readStudentData("../student.txt");
  
  while (true) {
    std::cout << "请选择操作：\n"
                 "1. 提名优秀学生并投票\n"
                 "2. 查看提名学生的基本信息\n"
                 "3. 显示各提名学生的票数\n"
                 "4. 显示排行榜\n"
                 "5. 退出\n";
    int option;
    std::cin >> option;
    switch (option) {
    case 1: {
      std::string pinyin;
      std::cout << "请输入学生的拼音：";
      std::cin >> pinyin;
      voteForStudent(pinyin);
      break;
    }
    case 2: {
      std::string pinyin;
      std::cout << "请输入学生的拼音：";
      std::cin >> pinyin;
      displayStudentInfo(pinyin);
      break;
    }
    case 3:
      std::cout << "各提名学生的票数如下：\n";
      displayVoteCount();
      break;
    case 4:
      std::cout << "排行榜如下：\n";
      displayRanking();
      break;
    case 5:
      std::cout << "感谢使用！\n";
      return 0;
    default:
      std::cout << "无效的选项，请重新输入！\n";
    }
  }

  return 0;
}
