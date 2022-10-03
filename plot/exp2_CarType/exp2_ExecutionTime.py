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

n_groups = 5

def plot_ExecutionTime():

    DTVC_OVMAP = (1999.909, 2187.497, 2524.632, 3024.234, 5065.920)  # 淡黄色
    DTVC_OTRAP = (2194.571, 2339.289, 2747.723, 3285.816, 5680.014)  # 淡蓝色
    # DTVC_OPT_TRAP = (151.573, 4826.109, 0, 0)  # 淡绿色

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, np.array(DTVC_OVMAP)/1000, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)      # OPT_TRAP 金色
    rects2 = plt.bar(index + 1 * bar_width, np.array(DTVC_OTRAP)/1000, bar_width, color='royalblue', label='DTVC_OTRAP', alpha=0.7)  # OTRAP 皇家蓝
    # rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP', alpha=0.7)  # OPT_TRAP 石灰绿

    plt.title('ECS = 8, Number of Cars = 10000(S)', fontsize=18)
    x = ('0%', '5%', '10%', '20%', '50%')
    plt.xticks(index + bar_width, x, family='Times New Roman', fontsize=18)
    plt.yticks(np.linspace(1, 6, 6), family='Times New Roman', fontsize=18)
    plt.ylim((1, 6))
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel(r'Execution Time($1\times10^3$ s)')
    plt.xlabel('Proportion of Large Requirements')
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'})

    # 生成 .eps 文件时应该注释 .show()
    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp2_ET.pdf', dpi=600)
    # plt.savefig('./jpeg/图8  不同车辆任务请求规模下的执行时间.jpeg')
    # plt.savefig('./eps/图8  不同车辆任务请求规模下的执行时间.eps', dpi=600)
    plt.show()

if __name__ == '__main__':
    plot_ExecutionTime()