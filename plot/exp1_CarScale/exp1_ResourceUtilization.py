import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import FuncFormatter
from matplotlib import rcParams

config = {
    "font.family": 'Times New Roman',   # 英文标题
    # "font.family": 'serif',  # 中文标题
    "font.size": 18,
    "mathtext.fontset": 'stix',
    "font.serif": ['SimSun'],
}
rcParams.update(config)

n_groups = 4

# CPU 资源利用率
def plot_UR_CPU():

    DTVC_OVMAP = (0.13, 0.66, 6.47, 11.91)  # 金色
    DTVC_OTRAP = (0.13, 0.66, 6.48, 12.05)  # 皇家蓝
    DTVC_OPT_TRAP = (0.13, 0.66, 0, 0)  # 石灰绿

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP',alpha=0.7)
    rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP',alpha=0.7)

    def to_percent(temp, position):
        return "%1.0f" % (temp) + "%"
    plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    plt.title('ECS = 8, Proportion of Large Requirements = 0%', fontsize=16)
    x = ('100', '500', '5000', '10000')
    plt.xticks(index + bar_width, x, family='Times New Roman')
    plt.yticks(family='Times New Roman')
    plt.ylim(0, 20)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('CPU Utilization')
    plt.xlabel('Number of Cars\n(a)CPU')
    plt.legend(bbox_to_anchor=(0.35, 1), prop={'size': 10, 'family': 'Times New Roman'})
    # plt.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp1_UR_CPU.pdf', dpi=600)
    plt.show()

# GPU 资源利用率
def plot_UR_GPU():

    DTVC_OVMAP = (0.14, 0.72, 7.17, 13.41)  # 金色
    DTVC_OTRAP = (0.14, 0.72, 7.18, 13.55)  # 皇家蓝
    DTVC_OPT_TRAP = (0.14, 0.72, 0, 0)  # 石灰绿

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP',alpha=0.7)
    rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP',alpha=0.7)

    def to_percent(temp, position):
        return "%1.0f" % (temp) + "%"
    plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    plt.title('ECS = 8, Proportion of Large Requirements = 0%', fontsize=16)
    x = ('100', '500', '5000', '10000')
    plt.xticks(index + bar_width, x, family='Times New Roman')
    plt.yticks(family='Times New Roman')
    plt.ylim(0, 20)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('GPU Utilization')
    plt.xlabel('Number of Cars\n(b)GPU')
    plt.legend(bbox_to_anchor=(0.35, 1), prop={'size': 10, 'family': 'Times New Roman'})
    # plt.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp1_UR_GPU.pdf', dpi=600)
    plt.show()

# Memory 资源利用率
def plot_UR_Memory():

    DTVC_OVMAP = (0.17, 0.87, 8.58, 16.00)  # 金色
    DTVC_OTRAP = (0.17, 0.87, 8.59, 16.08)  # 皇家蓝
    DTVC_OPT_TRAP = (0.17, 0.87, 0, 0)  # 石灰绿

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP',alpha=0.7)
    rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP',alpha=0.7)

    def to_percent(temp, position):
        return "%1.0f" % (temp) + "%"
    plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    plt.title('ECS = 8, Proportion of Large Requirements = 0%', fontsize=16)
    x = ('100', '500', '5000', '10000')
    plt.xticks(index + bar_width, x, family='Times New Roman')
    plt.yticks(family='Times New Roman')
    plt.ylim(0, 20)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('Memory Utilization')
    plt.xlabel('Number of Cars\n(c)Memory')
    # plt.legend(bbox_to_anchor=(0.31, 1), prop={'size': 10, 'family': 'Times New Roman'})
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp1_UR_Memory.pdf', dpi=600)
    plt.show()

# Disk 资源利用率
def plot_UR_Disk():

    DTVC_OVMAP = (0.37, 1.85, 18.26, 33.94)  # 金色
    DTVC_OTRAP = (0.37, 1.85, 18.27, 34.07)  # 皇家蓝
    DTVC_OPT_TRAP = (0.37, 1.85, 0, 0)  # 石灰绿

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP',alpha=0.7)
    rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP',alpha=0.7)

    def to_percent(temp, position):
        return "%1.0f" % (temp) + "%"
    plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    plt.title('ECS = 8, Proportion of Large Requirements = 0%', fontsize=16)
    x = ('100', '500', '5000', '10000')
    plt.xticks(index + bar_width, x, family='Times New Roman')
    plt.yticks(family='Times New Roman')
    plt.ylim(0, 40)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('Disk Utilization')
    plt.xlabel('Number of Cars\n(d)Disk')
    # plt.legend(bbox_to_anchor=(0.31, 1), prop={'size': 10, 'family': 'Times New Roman'})
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp1_UR_Disk.pdf', dpi=600)
    plt.show()

if __name__ == '__main__':
    plot_UR_CPU()
    plot_UR_GPU()
    plot_UR_Memory()
    plot_UR_Disk()