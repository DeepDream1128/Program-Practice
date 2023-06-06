#include <iostream>
#include <vector>
#include <stack>
#pragma GCC optimize(2)  // 优化编译速度
// 定义迷宫的相关参数
int numRows = 6;                    // 迷宫的行数
int numCols = 5;                    // 迷宫的列数
std::vector<std::vector<int>> maze; // 二维向量表示的迷宫，0表示通道，1表示墙

// 定义8种可能的行动：上、下、左、右、左上、右上、左下、右下
int actions[8][2] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                     {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
int action_cnt = 4; // 行动的种类数

// 判断是否是合法的移动
bool isValidMove(int row, int col) {
  if (row >= 0 && row < numRows && col >= 0 && col < numCols &&
      maze[row][col] == 0) {
    return true;
  }
  return false;
}

// 使用深度优先搜索解决迷宫问题
bool dfs(int currentRow, int currentCol, std::vector<std::vector<bool>>& visited,
         std::vector<std::pair<int, int>>& path) {
  // 到达终点，返回true
  if (currentRow == numRows - 1 && currentCol == numCols - 1) {
    return true;
  }

  visited[currentRow][currentCol] = true; // 将当前位置标记为已访问

  // 遍历所有可能的动作
  for (int i = 0; i < action_cnt; i++) {
    int newRow = currentRow + actions[i][0]; // 计算执行动作后的新位置
    int newCol = currentCol + actions[i][1];

    // 如果新位置是合法的且未被访问过
    if (isValidMove(newRow, newCol) && !visited[newRow][newCol]) {
      // 将新位置加入到路径中
      path.push_back(std::make_pair(newRow, newCol));

      // 递归调用DFS搜索
      if (dfs(newRow, newCol, visited, path)) {
        return true; // 如果找到一条路径，返回true
      }

      // 未找到路径，回溯
      path.pop_back();
    }
  }

  return false; // 未找到路径，返回false
}

// 解决迷宫问题的主函数
void solveMaze() {
  // 创建一个二维布尔数组来记录访问状态
  std::vector<std::vector<bool>> visited(numRows, std::vector<bool>(numCols, false));

  std::vector<std::pair<int, int>> path; // 存储路径
  path.push_back(std::make_pair(0, 0));  // 将起点加入到路径中

  if (dfs(0, 0, visited, path)) {
    // 输出找到的路径
    for (const auto& p : path) {
      std::cout << "(" << p.first << ", " << p.second << ") ";
    }
    std::cout << std::endl;
  } else {
    std::cout << "There is no valid path!" << std::endl;
  }
}

int main() {
  // 从文件读取迷宫数据
  freopen("/media/zby/SSD数据盘/Program-Practice/Maze/build/maze.txt", "r", stdin);
  std::cin >> numRows >> numCols; // 输入迷宫的行数和列数
  maze.resize(numRows, std::vector<int>(numCols)); // 调整迷宫大小
  for (int i = 0; i < numRows; i++) {              // 输入迷宫数据
    for (int j = 0; j < numCols; j++) {
      std::cin >> maze[i][j];
    }
  }

  solveMaze(); // 解决迷宫问题

  return 0; // 程序结束
}
