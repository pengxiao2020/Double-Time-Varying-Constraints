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

n_groups = 5

# CPU 资源利用率
def plot_UR_CPU():

    DTVC_OVMAP = (11.91, 12.75, 13.42, 14.83, 18.52)  # 金色
    DTVC_OTRAP = (12.05, 12.97, 13.73, 15.30, 19.18)  # 皇家蓝
    # DTVC_OPT_TRAP = (0.13, 0.66, 0, 0)  # 石灰绿

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP',alpha=0.7)
    # rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP',alpha=0.7)

    def to_percent(temp, position):
        return "%1.0f" % (temp) + "%"
    plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    plt.title('ECS = 8, Number of Cars = 10000(S)')
    x = ('0%', '5%', '10%', '20%', '50%')

    plt.xticks(index + bar_width, x, family='Times New Roman')
    plt.yticks(family='Times New Roman')
    plt.ylim(10, 20)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('CPU Utilization')
    plt.xlabel('Proportion of Large Requirements\n(a)CPU')
    # plt.legend(bbox_to_anchor=(0.35, 1), prop={'size': 10, 'family': 'Times New Roman'})
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp2_UR_CPU.pdf', dpi=600)
    plt.show()

# GPU 资源利用率
def plot_UR_GPU():

    DTVC_OVMAP = (13.41, 14.12, 14.74, 15.98, 19.33)  # 金色
    DTVC_OTRAP = (13.55, 14.32, 15.03, 16.41, 19.93)  # 皇家蓝
    # DTVC_OPT_TRAP = (0.14, 0.72, 0, 0)  # 石灰绿

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP',alpha=0.7)
    # rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP',alpha=0.7)

    def to_percent(temp, position):
        return "%1.0f" % (temp) + "%"
    plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    plt.title('ECS = 8, Number of Cars = 10000(S)')
    x = ('0%', '5%', '10%', '20%', '50%')

    plt.xticks(index + bar_width, x, family='Times New Roman')
    plt.yticks(family='Times New Roman')
    plt.ylim(10, 21)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('GPU Utilization')
    plt.xlabel('Proportion of Large Requirements\n(b)GPU')
    # plt.legend(bbox_to_anchor=(0.35, 1), prop={'size': 10, 'family': 'Times New Roman'})
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp2_UR_GPU.pdf', dpi=600)
    plt.show()

# Memory 资源利用率
def plot_UR_Memory():

    DTVC_OVMAP = (16.00, 16.63, 17.06, 18.09, 20.60)  # 金色
    DTVC_OTRAP = (16.08, 16.76, 17.28, 18.39, 21.05)  # 皇家蓝
    # DTVC_OPT_TRAP = (0.17, 0.87, 0, 0)  # 石灰绿

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP',alpha=0.7)
    # rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP',alpha=0.7)

    def to_percent(temp, position):
        return "%1.0f" % (temp) + "%"
    plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    plt.title('ECS = 8, Number of Cars = 10000(S)')
    x = ('0%', '5%', '10%', '20%', '50%')

    plt.xticks(index + bar_width, x, family='Times New Roman')
    plt.yticks(family='Times New Roman')
    plt.ylim(14, 22)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('Memory Utilization')
    plt.xlabel('Proportion of Large Requirements\n(c)Memory')
    # plt.legend(bbox_to_anchor=(0.31, 1), prop={'size': 10, 'family': 'Times New Roman'})
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp2_UR_Memory.pdf', dpi=600)
    plt.show()

# Disk 资源利用率
def plot_UR_Disk():

    DTVC_OVMAP = (33.94, 35.22, 36.19, 38.28, 43.45)  # 金色
    DTVC_OTRAP = (34.07, 35.44, 36.59, 38.87, 44.26)  # 皇家蓝
    # DTVC_OPT_TRAP = (0.37, 1.85, 0, 0)  # 石灰绿

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    rects1 = plt.bar(index + 0 * bar_width, DTVC_OVMAP, bar_width, color='gold', label='DTVC_OVMAP', alpha=0.7)
    rects2 = plt.bar(index + 1 * bar_width, DTVC_OTRAP, bar_width, color='royalblue', label='DTVC_OTRAP',alpha=0.7)
    # rects3 = plt.bar(index + 2 * bar_width, DTVC_OPT_TRAP, bar_width, color='limegreen', label='DTVC_OPT_TRAP',alpha=0.7)

    def to_percent(temp, position):
        return "%1.0f" % (temp) + "%"
    plt.gca().yaxis.set_major_formatter(FuncFormatter(to_percent))

    plt.title('ECS = 8, Number of Cars = 10000(S)')
    x = ('0%', '5%', '10%', '20%', '50%')

    plt.xticks(index + bar_width, x, family='Times New Roman')
    plt.yticks(family='Times New Roman')
    plt.ylim(30, 46)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel('Disk Utilization')
    plt.xlabel('Proportion of Large Requirements\n(d)Disk')
    # plt.legend(bbox_to_anchor=(0.31, 1), prop={'size': 10, 'family': 'Times New Roman'})
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'}, loc='best')

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp2_UR_Disk.pdf', dpi=600)
    plt.show()

if __name__ == '__main__':
    plot_UR_CPU()
    plot_UR_GPU()
    plot_UR_Memory()
    plot_UR_Disk()