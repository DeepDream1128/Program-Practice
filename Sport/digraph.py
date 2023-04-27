from graphviz import Digraph

# 创建一个新的图
dot = Digraph(comment='Sports Scoring System')

# 添加节点
dot.node('A', '输入学校数量、男子团体项目数量、女子团体项目数量')
dot.node('B', '输入学校名称')
dot.node('C', '输入项目名称')
dot.node('D', '输入项目成绩')
dot.node('E', '按学校编号排序输出')
dot.node('F', '按学校总分排序输出')
dot.node('G', '按男女团体总分排序输出')
dot.node('H', '按学校编号查询学校某个项目的情况')
dot.node('I', '按项目编号查询取得前三或前五名的学校')

# 添加边
dot.edge('A', 'B')
dot.edge('B', 'C')
dot.edge('C', 'D')
dot.edge('D', 'C')
dot.edge('D', 'E')
dot.edge('D', 'F')
dot.edge('D', 'G')
dot.edge('H', 'B')
dot.edge('H', 'D')
dot.edge('I', 'C')
dot.edge('I', 'D')

# 渲染图并保存为pdf文件
dot.format = 'jpg'
dot.render('sports_scoring_system', view=True)
