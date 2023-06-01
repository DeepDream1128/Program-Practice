#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <string>

using namespace std;

//定义学校结构体，包含学校的信息和比赛得分
struct School {
    int id;  // 学校ID
    string name;  // 学校名字
    int total_score = 0;  // 学校总得分
    int male_score = 0;  // 男子团体总得分
    int female_score = 0;  // 女子团体总得分
    map<int, int> event_scores;  // 按项目编号存储每个项目的得分
};

//定义比赛项目结构体，包含项目的信息和每个学校在该项目中的得分
struct Event {
    int id;  // 项目ID
    string name;  // 项目名字
    string gender;  // 项目性别分类，m表示男子项目，w表示女子项目
    int num_rankings;  // 用户指定的名次取法，例如前三名或前五名
    vector<pair<int, int>> school_scores;  // 每个学校在该项目中的得分，
                                           //pair中第一个元素为学校ID，
                                           //第二个元素为该学校在该项目中的得分
};

//比较函数，用于按照学校ID排序
bool compare_by_id(const School& a, const School& b) {
    return a.id < b.id;
}

//比较函数，用于按照学校总得分排序
bool compare_by_total_score(const School& a, const School& b) {
    return a.total_score > b.total_score;
}

//比较函数，用于按照男女团体总得分排序
bool compare_by_male_female_score(const School& a, const School& b) {
    if (a.male_score != b.male_score) {
        return a.male_score > b.male_score;
    }
    return a.female_score > b.female_score;
}


int main() {
    int n, m, w;
    cout << "请输入学校数量、男子团体项目数量、女子团体项目数量：" << endl;
    cin >> n >> m >> w;
    cout << "请输入学校名称：" << endl;
    vector<School> schools(n);
    for (int i = 0; i < n; ++i) {
        schools[i].id = i + 1;
        cin >> schools[i].name;
    }
    cout << "请输入项目名称, 参与性别及名次取法：" << endl; // 让用户指定每个项目的名次取法
    vector<Event> events(m + w);
    for (int i = 0; i < m + w; ++i) {
        events[i].id = i + 1;
        // 获取用户指定的名次取法
        cin >> events[i].name >> events[i].gender 
            >>events[i].num_rankings; 
    }

    for (auto& event : events) {
        for (int i = 0; i < event.num_rankings; ++i) { // 根据用户指定的名次取法计算学校得分
            int school_id, score;
            cout<< "请输入项目 " << event.name << " 的第 " << i + 1 << " 名学校编号和得分：" << endl;
            cin >> school_id >> score;
            event.school_scores.push_back({school_id, score});

            School& school = schools[school_id - 1];
            school.total_score += score;
            if (event.gender=="m") {
                school.male_score += score;
            } else {
                school.female_score += score;
                cout<< school.female_score<<endl;
            }
            school.event_scores[event.id] = score;
        }
    }
    // 输出学校编号排序
    sort(schools.begin(), schools.end(), compare_by_id);
    cout << "按学校编号排序：" << endl;
    for (const auto& school : schools) {
        cout << school.name << " (编号: " << school.id << ")" << endl;
    }
    cout << endl;

    // 输出学校总分排序
    sort(schools.begin(), schools.end(), compare_by_total_score);
    cout << "按学校总分排序：" << endl;
    for (const auto& school : schools) {
        cout << school.name << " (总分: " << school.total_score << ")" << endl;
    }
    cout << endl;

    // 输出男女团体总分排序
    sort(schools.begin(), schools.end(), compare_by_male_female_score);
    cout << "按男女团体总分排序：" << endl;
    for (const auto& school : schools) {
        cout << school.name << " (男子团队分数: " << school.male_score << ", 女子团队分数: " << school.female_score << ")" << endl;
    }
    cout << endl;

    // 查询学校某个项目的情况
    int query_school_id, query_event_id;
    cout << "请输入查询的学校编号和项目编号：" << endl;
    cin >> query_school_id >> query_event_id;
    const School& query_school = schools[query_school_id - 1];
    auto it = query_school.event_scores.find(query_event_id);
    if (it != query_school.event_scores.end()) {
        cout << query_school.name << " 在项目 " << events[query_event_id - 1].name << " 中的得分为: " << it->second << endl;
    } else {
        cout << query_school.name << " 在项目 " << events[query_event_id - 1].name << " 中没有得分" << endl;
    }
    cout << endl;

    // 按项目编号查询取得前三或前五名的学校
    cout << "请输入要查询的项目编号：" << endl;
    int query_event_id2;
    cin >> query_event_id2;
    const Event& query_event = events[query_event_id2 - 1];

    cout << "在项目 " << query_event.name << " 中取得前" << query_event.num_rankings << "名的学校有：" << endl;
    for (const auto& school_score : query_event.school_scores) {
        cout << schools[school_score.first - 1].name << " (得分: " << school_score.second << ")" << endl;
    }

    return 0;
}
