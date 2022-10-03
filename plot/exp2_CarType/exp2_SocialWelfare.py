import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams

# import matplotlib
# print(matplotlib.matplotlib_fname())

# from pylab import mpl
# mpl.rcParams['font.sans-serif'] = ['STZhongsong']   # 指定默认字体：解决 plot 不能显示中文问题
# mpl.rcParams['axes.unicode_minus'] = False          # 解决保存图像是负号 '-'显示为方块的问题


config = {
    "font.family": 'Times New Roman',   # 英文标题
    # "font.family": 'serif',  # 中文标题
    "font.size": 18,
    "mathtext.fontset": 'stix',
    "font.serif": ['SimSun'],
}
rcParams.update(config)

n_groups = 5  # 柱状图组数

def plot_SocialWelfare():

    DTVC_OVMAP = (14255220, 16472120, 18614190, 22735270, 35205350)      # 淡黄色
    DTVC_OTRAP = (14384050, 16856450, 19386040, 24019140, 37310600)      # 淡蓝色
    # DTVC_OPT_TRAP = (145969, 745251, 0, 0)  # 淡绿色

    fig, ax = plt.subplots()
    index = np.arange(n_groups)
    bar_width = 0.2

    opacity = 0.6

    # 如果使用彩色：OVMAP*：color='gold', OTRAP: color='royalblue', OPT_TRAP:  color='limegreen'
    # 如果使用黑白：
    # OVMAP*：color='lightgray', edgecolor='k', hatch='xxx',
    # OTRAP:  color='gray', edgecolor='k', hatch='++',
    # OPT_TRAP: color='white', edgecolor='k',
    rects1 = plt.bar(index + 0 * bar_width, np.array(DTVC_OVMAP)/1000000, bar_width, color='gold', label='DTVC-OVMAP', alpha=0.7)     # OPT_TRAP 金色
    rects2 = plt.bar(index + 1 * bar_width, np.array(DTVC_OTRAP)/1000000, bar_width, color='royalblue', label='DTVC-OTRAP', alpha=0.7)     # OTRAP 皇家蓝
    # rects3 = plt.bar(index + 2 * bar_width, np.array(DTVC_OPT_TRAP)/1000000, bar_width, color='limegreen', label='DTVC-OPT-TRAP', alpha=0.7)   # OPT_TRAP 石灰绿

    plt.title('ECS = 8, Number of Cars = 10000(S)')
    x = ('0%', '5%', '10%', '20%', '50%')
    plt.xticks(index + bar_width, x, family='Times New Roman')
    # plt.yticks([0, 1, 2, 3, 4, 5, 6, 7, 8], family='Times New Roman', fontsize=18)
    plt.yticks(np.linspace(14, 38, 7), family='Times New Roman')
    plt.ylim(14, 40)
    # plt.yscale('log')
    # plt.xlim(-0.2, 4.0)
    plt.ylabel(r'Social Welfare($1\times10^6$)')
    plt.xlabel('Proportion of Large Requirements')
    plt.legend(prop={'size': 10, 'family': 'Times New Roman'})

    plt.tight_layout()
    # plt.grid(ls='--')
    # plt.savefig('1scale-time.png', dpi=600)
    # plt.savefig('1scale-time.eps', dpi=600)
    plt.savefig('./pdf/exp2_SW.pdf', dpi=600)
    #plt.savefig('./jpeg/图7  不同车辆任务请求规模下的社会福利.jpeg')
    plt.show()

if __name__ == '__main__':
    plot_SocialWelfare()