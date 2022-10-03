import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter
from matplotlib import ticker
from matplotlib import rcParams

# font = {'family': 'Times New Roman', 'color': 'black', 'weight': 'normal', 'size': 18}

# 设置全局字体的命令，需引入 rcParams 模块
config = {
    "font.family": 'Times New Roman',   # 英文标题
    # "font.family": 'serif',   # 中文标题
    "font.size": 18,
    "mathtext.fontset": 'stix',
    "font.serif": ['SimSun'],
}
rcParams.update(config)

n_groups = 5

def plot_ServedCar():
    # 服务车辆数

    Num_DTVC_OVMAP = (9227, 9173, 9055, 8867, 8218)  # 金色
    Num_DTVC_OTRAP = (9261, 9191, 9072, 8858, 8150)  # 皇家蓝
    # Num_DTVC_OPT_TRAP = (100, 500, 0, 0)  # 石灰绿

    # 服务车辆比例
    # Ratio_OPT_TRAP = np.array((99.00, 98.70, 0, 0, 0)) / 100  # 石灰绿
    # Ratio_OVMAP_mod = np.array((89.00, 92.00, 83.41, 71.83, 54.05)) / 100  # 金色
    # Ratio_OTRAP = np.array((89.00, 92.00, 83.28, 71.91, 54.03)) / 100  # 皇家蓝

    fig = plt.figure()
    ax = fig.add_subplot(111)
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, Num_DTVC_OVMAP, bar_width, color='gold', label='DTVC-OVMAP', alpha=0.7)  # OVMAP 金色
    rects2 = plt.bar(index + 1 * bar_width, Num_DTVC_OTRAP, bar_width, color='royalblue', label='DTVC-OTRAP', alpha=0.7)  # OTRAP 皇家蓝
    # rects3 = plt.bar(index + 2 * bar_width, Num_DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC-OPT-TRAP', alpha=0.7)  # OPT_TRAP 石灰绿

    plt.title('ECS = 8, Number of Cars = 10000(S)')
    x = ('0%', '5%', '10%', '20%', '50%')

    plt.xticks(index + bar_width, x, family='Times New Roman', fontsize=18)
    plt.yticks(np.linspace(7500, 9500, 5), family='Times New Roman', fontsize=18)
    plt.ylim(7500, 9800)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    ax.set_xlabel('Proportion of Large Requirements')
    ax.set_ylabel('Number of Served Cars')

    # 图像右边增加一条坐标轴，表示百分比
    # ax_cp = ax.twinx()
    # ax_OVMAP_mod = ax_cp.twinx()
    # ax_OTRAP = ax_cp.twinx()
    # ax_cp.plot(index + 0 * bar_width, Ratio_OVMAP_mod, color='gold', linestyle='--', marker='o', linewidth=1)
    # ax_cp.plot(index + 1 * bar_width, Ratio_OTRAP, color='royalblue', linestyle='--', marker='o', linewidth=1)
    # ax_cp.plot(index + 2 * bar_width, Ratio_OPT_TRAP, color='limegreen', linestyle='--', marker='o', linewidth=1)
    # ax_cp.set_ylabel("服务的车辆任务请求比例", color='k', fontsize=16)
    # ax_cp.yaxis.set_major_formatter(ticker.PercentFormatter(xmax=1, decimals=0))

    # def to_percent(temp, position):
    #     return "%1.0f" % (temp) + "%"
    # plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    # ax.legend(fontsize=12, bbox_to_anchor=(0.36, 0.8))
    # ax.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')
    plt.legend(bbox_to_anchor=(0.31, 1), prop={'size': 10, 'family': 'Times New Roman'})
    plt.tight_layout()

    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp2_SC.pdf', dpi=600)
    # plt.savefig('./jpeg/图9  不同车辆任务请求规模下的服务请求数.jpeg')
    plt.show()

if __name__ == '__main__':
    plot_ServedCar()