#include <bits/stdc++.h> // 引入常用库

// 定义强化学习的一些参数
const double learningRate = 0.9;     // 学习率
const double discountFactor = 0.9;   // 折扣因子
const int numEpisodes = 5000;        // 总的训练轮数
double epsilon = 0.9;                // ε-贪婪策略中的参数
const double epsilonDecay = 0.99;    // ε的衰减系数
const int maxStepsPerEpisode = 3000; // 每轮训练的最大步数

// 定义迷宫的相关参数
int numRows = 6;                    // 迷宫的行数
int numCols = 5;                    // 迷宫的列数
std::vector<std::vector<int>> maze; // 二维向量表示的迷宫，0表示通道，1表示墙

// 定义起点和终点
int startRow = 0;         // 起点的行坐标
int startCol = 0;         // 起点的列坐标
int endRow = numRows - 1; // 终点的行坐标
int endCol = numCols - 1; // 终点的列坐标

// 定义8种可能的行动：上、下、左、右、左上、右上、左下、右下
int actions[8][2] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                     {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
int action_cnt = 8; // 行动的种类数

// 判断是否是合法的移动
bool isValidMove(int row, int col) {
  if (row >= 0 && row < numRows && col >= 0 && col < numCols &&
      maze[row][col] == 0) {
    return true;
  }
  return false;
}

// 根据当前状态和Q值表选择行动
int chooseAction(int row, int col,
                 std::vector<std::vector<std::vector<double>>> &qValues,
                 int previousAction) {
  // epsilon-greedy策略选择动作

  double random =
      static_cast<double>(rand()) / RAND_MAX; // 生成一个[0, 1)之间的随机数
  if (random < epsilon) {                  // 以ε的概率随机选择动作
    std::vector<int> validActions;         // 存储所有有效的动作
    for (int i = 0; i < action_cnt; i++) { // 遍历所有可能的动作
      int newRow = row + actions[i][0];    // 计算执行动作后的新位置
      int newCol = col + actions[i][1];
      if (isValidMove(newRow, newCol)) { // 如果该动作是有效的
        validActions.push_back(i); // 将该动作加入到有效动作列表中
      }
    }

    if (validActions.empty()) { // 如果没有有效的动作
      return -1;                // 返回-1表示无有效动作
    }

    return validActions[rand() %
                        validActions.size()]; // 在所有有效动作中随机选择一个
  }

  // 以1-ε的概率选择最优的动作
  int bestAction = -1;                   // 存储最优动作
  double maxValue = -1e9;                // 存储最大的Q值
  for (int i = 0; i < action_cnt; i++) { // 遍历所有可能的动作
    int newRow = row + actions[i][0];    // 计算执行动作后的新位置
    int newCol = col + actions[i][1];
    // 如果该动作是有效的并且Q值大于当前最大的Q值，那么更新最优动作和最大Q值
    if (isValidMove(newRow, newCol) && qValues[row][col][i] > maxValue &&
        (previousAction == -1 || // 对于第一步，任何动作都可以是最优动作
         (actions[i][0] !=
              -actions[previousAction][0] || // 不选取与上一步相反的动作
          actions[i][1] != -actions[previousAction][1]))) {
      maxValue = qValues[row][col][i];
      bestAction = i;
    }
  }
  return bestAction; // 返回最优动作
}

// 根据公式更新Q值表
void updateQValues(int currentRow, int currentCol, int newRow, int newCol,
                   int action,
                   std::vector<std::vector<std::vector<double>>> &qValues) {
  double maxQValue = -1e9;               // 存储最大的Q值
  for (int i = 0; i < action_cnt; i++) { // 遍历所有可能的动作
    // 如果Q值大于当前最大的Q值，那么更新最大Q值
    if (qValues[newRow][newCol][i] > maxQValue) {
      maxQValue = qValues[newRow][newCol][i];
    }
  }

  // 定义奖励值，如果到达目标位置则奖励为100，否则按离终点的距离给予惩罚
  double distance=sqrt(pow(newRow - endRow, 2) + pow(newCol - endCol, 2));
  double reward = (distance == 0) ? 100 : -distance;
  // 根据Q学习算法的公式更新Q值
  qValues[currentRow][currentCol][action] +=
      learningRate * (reward + discountFactor * maxQValue -
                      qValues[currentRow][currentCol][action]);
}

// Q学习的主要函数
void qLearning() {
  srand(time(0)); // 设置随机种子
  // 初始化Q值表，所有的Q值都设置为0
  std::vector<std::vector<std::vector<double>>> qValues(
      numRows, std::vector<std::vector<double>>(
                   numCols, std::vector<double>(action_cnt, 0)));

  // 进行numEpisodes轮训练
  for (int episode = 0; episode < numEpisodes; episode++) {
    int currentRow = startRow; // 当前位置的行坐标
    int currentCol = startCol; // 当前位置的列坐标

    int step = 0;            // 计步器
    int previousAction = -1; // 上一步的动作
    // 当未达到目标且未超过每轮最大步数时，继续训练
    while ((currentRow != endRow || currentCol != endCol) &&
           step < maxStepsPerEpisode) {
      // 选择一个动作
      int action = chooseAction(currentRow, currentCol, qValues, -1);

      if (action == -1) { // 如果没有有效的动作
        break;            // 结束本轮训练
      }

      // 执行选定的动作，更新当前位置
      int newRow = currentRow + actions[action][0];
      int newCol = currentCol + actions[action][1];

      if (isValidMove(newRow, newCol)) { // 如果移动后的位置有效
        // 更新Q值表
        updateQValues(currentRow, currentCol, newRow, newCol, action, qValues);
        currentRow = newRow; // 更新当前位置
        currentCol = newCol;
        previousAction = action; // 更新上一步的动作
      }
      step++; // 计步器增加
    }
    // ε按衰减系数衰减
    epsilon *= epsilonDecay;
  }

  double originalEpsilon = epsilon; // 存储原始的ε值
  epsilon = 0; // ε设为0，即在路径规划时只采用贪婪策略

  // 显示找到的路径
  std::vector<std::pair<int, int>> path; // 存储路径
  int currentRow = startRow;             // 当前位置的行坐标
  int currentCol = startCol;             // 当前位置的列坐标
  path.push_back(std::make_pair(currentRow, currentCol)); // 将起点加入到路径中
  int previousAction1 = -1;                               // 上一步的动作

  while (currentRow != endRow ||
         currentCol != endCol) { // 当未到达终点时，继续行走
    // 选择最优动作
    int bestAction =
        chooseAction(currentRow, currentCol, qValues, previousAction1);

    if (bestAction == -1) { // 如果没有有效的动作
      std::cout << "There is no valid path!" << std::endl; // 输出提示信息
      return;                                              // 结束函数执行
    }

    // 执行选定的动作，更新当前位置
    int newRow = currentRow + actions[bestAction][0];
    int newCol = currentCol + actions[bestAction][1];

    if (isValidMove(newRow, newCol)) { // 如果移动后的位置有效
      currentRow = newRow;             // 更新当前位置
      currentCol = newCol;
      path.push_back(
          std::make_pair(currentRow, currentCol)); // 将新位置加入到路径中
      previousAction1 = bestAction;                // 更新上一步的动作
    }
  }

  // 输出找到的路径
  for (const auto &p : path) {
    std::cout << "(" << p.first << ", " << p.second << ") ";
  }
  std::cout << std::endl;

  // 恢复原始的ε值
  epsilon = originalEpsilon;
}

// 主函数
int main() {
  // 从文件读取迷宫数据
  freopen("/media/zby/SSD数据盘/Program-Practice/Maze/build/maze.txt", "r",
          stdin);
  std::cin >> numRows >> numCols; // 输入迷宫的行数和列数
  maze.resize(numRows, std::vector<int>(numCols)); // 调整迷宫大小
  for (int i = 0; i < numRows; i++) {              // 输入迷宫数据
    for (int j = 0; j < numCols; j++) {
      std::cin >> maze[i][j];
    }
  }

  qLearning(); // 执行Q学习算法

  return 0; // 程序结束
}
