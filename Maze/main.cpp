#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
const int numRows = 6;
const int numCols = 5;
const double learningRate = 0.9;
const double discountFactor = 0.9;
const int numEpisodes = 500;
double epsilon = 0.9;
const double epsilonDecay = 0.99;
const int maxStepsPerEpisode = 300;

int maze[numRows][numCols] = {{0, 0, 1, 0, 0}, 
                              {1, 0, 1, 1, 0},
                              {0, 0, 0, 1, 0}, 
                              {0, 1, 0, 0, 1},
                              {0, 0, 1, 0, 1}, 
                              {1, 0, 0, 0, 0}};

int startRow = 0;
int startCol = 0;
int endRow = 5;
int endCol = 4;

// int actions[8][2] = {
//     {-1, 0}, {1, 0},  {0, -1}, {0, 1}, {-1, -1},
//     {-1, 1}, {1, -1}, {1, 1}}; // Up, Down, Left, Right, UpLeft, UpRight,
//                                // DownLeft, DownRight
int actions[4][2]= {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right
int action_cnt = 4;

bool isValidMove(int row, int col) {
  if (row >= 0 && row < numRows && col >= 0 && col < numCols &&
      maze[row][col] == 0) {
    return true;
  }
  return false;
}

int chooseAction(int row, int col,
                 std::vector<std::vector<std::vector<double>>> &qValues,
                 int previousAction) {
  double random = static_cast<double>(rand()) / RAND_MAX;
  if (random < epsilon) {
    std::vector<int> validActions;
    for (int i = 0; i < action_cnt; i++) {
      int newRow = row + actions[i][0];
      int newCol = col + actions[i][1];
      if (isValidMove(newRow, newCol)) {
        validActions.push_back(i);
      }
    }

    if (validActions.empty()) {
      return -1;
    }

    return validActions[rand() % validActions.size()];
  }
  int bestAction = -1;
  double maxValue = -1e9;
  for (int i = 0; i < action_cnt; i++) {
    int newRow = row + actions[i][0];
    int newCol = col + actions[i][1];
    if (isValidMove(newRow, newCol) && qValues[row][col][i] > maxValue &&
        (previousAction == -1 ||
         (actions[i][0] != -actions[previousAction][0] ||
          actions[i][1] !=
              -actions[previousAction][1]))) { // Add this condition
      maxValue = qValues[row][col][i];
      bestAction = i;
    }
  }
  return bestAction;
}

void updateQValues(int currentRow, int currentCol, int newRow, int newCol,
                   int action,
                   std::vector<std::vector<std::vector<double>>> &qValues) {
  double maxQValue = -1e9;
  for (int i = 0; i < action_cnt; i++) {
    if (qValues[newRow][newCol][i] > maxQValue) {
      maxQValue = qValues[newRow][newCol][i];
    }
  }

  double reward = (newRow == endRow && newCol == endCol) ? 100 : -1;
  qValues[currentRow][currentCol][action] +=
      learningRate * (reward + discountFactor * maxQValue -
                      qValues[currentRow][currentCol][action]);
}

void qLearning() {
  srand(time(0));
  std::vector<std::vector<std::vector<double>>> qValues(
      numRows,
      std::vector<std::vector<double>>(numCols, std::vector<double>(action_cnt, 0)));

  for (int episode = 0; episode < numEpisodes; episode++) {
    int currentRow = startRow;
    int currentCol = startCol;
    std::cout << "Episode: " << episode << std::endl;

    int step = 0;            // Add a step counter
    int previousAction = -1; // Add this line
    while ((currentRow != endRow || currentCol != endCol) &&
           step < maxStepsPerEpisode) {
      int action = chooseAction(currentRow, currentCol, qValues, -1);

      if (action == -1) { // No valid actions available
        break;
      }

      int newRow = currentRow + actions[action][0];
      int newCol = currentCol + actions[action][1];

      if (isValidMove(newRow, newCol)) {
        updateQValues(currentRow, currentCol, newRow, newCol, action, qValues);
        currentRow = newRow;
        currentCol = newCol;
        previousAction = action; // Update previousAction
      }
      step++; // Increment the step counter
    }
    epsilon *= epsilonDecay;
  }
  double originalEpsilon = epsilon;
  epsilon = 0;
  // Display the path found
  std::vector<std::pair<int, int>> path;
  int currentRow = startRow;
  int currentCol = startCol;
  path.push_back(std::make_pair(currentRow, currentCol));
  int previousAction1 = -1; // Add this line
  while (currentRow != endRow || currentCol != endCol) {
    int bestAction =
        chooseAction(currentRow, currentCol, qValues, previousAction1);

    if (bestAction == -1) { // No valid actions available
      std::cout << "ERROR!" << std::endl;
      return; // Exit the function
    }

    int newRow = currentRow + actions[bestAction][0];
    int newCol = currentCol + actions[bestAction][1];

    if (isValidMove(newRow, newCol)) {
      currentRow = newRow;
      currentCol = newCol;
      path.push_back(std::make_pair(currentRow, currentCol));
      previousAction1 = bestAction; // Update previousAction
    }
  }
  for (const auto &p : path) {
    std::cout << "(" << p.first << ", " << p.second << ") ";
  }
  std::cout << std::endl;

  // Restore epsilon
  epsilon = originalEpsilon;
}

int main() {
  qLearning();
  return 0;
}