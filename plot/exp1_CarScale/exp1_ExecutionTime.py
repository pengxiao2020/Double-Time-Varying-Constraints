import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams

# font = {'family': 'Times New Roman', 'color': 'black', 'weight': 'normal', 'size': 18}

config = {
    "font.family": 'Times New Roman',   # 英文标题
    # "font.family": 'serif',   # 中文标题
    "font.size": 18,
    "mathtext.fontset": 'stix',
    "font.serif": ['SimSun'],
}
rcParams.update(config)

def plot_ExecutionTime():
    n_groups = 4

    DTVC_OVMAP = (1.347, 7.373, 222.095, 1999.909)  # 淡黄色
    DTVC_OTRAP = (1.490, 8.449, 264.804, 2194.571)  # 淡蓝色
    DTVC_OPT_TRAP = (151.573, 4826.109, 0, 0)  # 淡绿色

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)      # OPT_TRAP 金色
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP', alpha=0.7)  # OTRAP 皇家蓝
    rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP', alpha=0.7)  # OPT_TRAP 石灰绿

    plt.title('ECS = 8, Proportion of Large Requirements = 0%', fontsize=18)
    x = ('100', '500', '5000', '10000')
    plt.xticks(index + bar_width, x, family='Times New Roman', fontsize=18)
    plt.yticks(family='Times New Roman', fontsize=18)
    plt.ylim(10 ** (-3), 10 ** 5)
    plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('Execution Time(s)')
    plt.xlabel('Number of Cars')
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'})

    # 生成 .eps 文件时应该注释 .show()
    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp1_ET.pdf', dpi=600)
    # plt.savefig('./jpeg/图8  不同车辆任务请求规模下的执行时间.jpeg')
    # plt.savefig('./eps/图8  不同车辆任务请求规模下的执行时间.eps', dpi=600)
    plt.show()

if __name__ == '__main__':
    plot_ExecutionTime()