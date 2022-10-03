import numpy as np
import matplotlib.pyplot as plt
from matplotlib import rcParams

from pylab import mpl
mpl.rcParams['font.sans-serif'] = ['STZhongsong']   # 指定默认字体：解决 plot 不能显示中文问题
mpl.rcParams['axes.unicode_minus'] = False          # 解决保存图像是负号 '-'显示为方块的问题
# plt.rcParams['axes.unicode_minus'] = False          # 用来正常显示负号

config = {
    "font.family": 'Times New Roman',   # 英文标题
    # "font.family": 'serif',  # 中文标题
    "font.size": 18,
    "mathtext.fontset": 'stix',
    "font.serif": ['SimSun'],
}
rcParams.update(config)

# 从获胜车辆的角度验证真实性
def TruthfulnessVerification_Winner():
    # 获胜车辆 id = 6823 的信息 : b[6823] = 733.68, p[6823] = 614.32,
    # u[6823] = b[6823] - p[6823] = 733.68 - 614.32 = 199.36
    Winner_CarBid = (400, 450, 500, 550, 600, 614.32, 614.32, 650, 700, 750, 800)   # 获胜车辆 id = 6823 投标（X 轴）
    Winner_CarUtility = (0, 0, 0, 0, 0, 0, 199.36, 199.36, 199.36, 199.36,199.36)   # 获胜车辆 id = 6823 效用 （Y轴）

    plt.title('ECS = 8, Number of Cars = 10000(S)')
    # x = (np.linspace(500, 1200, 8))
    # y = (np.linspace(-100, 300, 5))

    plt.xticks(np.linspace(400, 800, 9))
    plt.yticks(np.linspace(-100, 300, 5))
    plt.ylim(-120, 320)

    plt.xlabel("Bid of Winning Car 6823\n(a)")
    plt.ylabel("Utility of Winning Car 6823")

    # 如果要画彩色图 ，颜色为红色，否则为黑色
    plt.plot(Winner_CarBid, Winner_CarUtility, color='red', linewidth=1.0, linestyle='--')
    plt.scatter(Winner_CarBid, Winner_CarUtility, color='red', linewidths=1.0, marker='*')

    plt.tight_layout()

    plt.savefig('./pdf/exp4_TV_Winner.pdf', dpi=600)
    # plt.savefig('./png/exp4_TV_Winner(in Chinese).png', dpi=600)
    # plt.savefig('./jpeg/图20(a)  OTRAP的真实性验证（胜者）.jpeg')
    plt.show()

# 从失败车辆的角度验证真实性
def TruthfulnessVerification_Loser():
    # 失败车辆 id = 7703 的信息 : b[7703] = 658.20，p[7703] = 0，u[7703] = 0.
    # 当失败车辆的投标升高到一定程度时，他会获胜，此时，p'[7703] = 786.09,
    # 但真实效用为负 u'[7703] = b[7703] - p'[7703] = -127.89.
    Loser_CarBid = (600, 650, 700, 750, 786.09, 786.09, 800, 850, 900, 950)  # 失败车辆 id = 7703 投标（X 轴）
    Loser_CarUtility = (0, 0, 0, 0, 0, -127.89, -127.89, -127.89, -127.89, -127.89)  # 失败车辆 id = 7703 效用 （Y轴）

    plt.title('ECS = 8, Number of Cars = 10000(S)')
    # x = (np.linspace(200, 1200, 7))
    # y = (np.linspace(-800, 200, 6))

    plt.xticks(np.linspace(550, 1000, 10))
    plt.yticks(np.linspace(-150, 50, 5))
    plt.ylim(-190, 60)

    plt.xlabel("Bid of Losing Car 7703\n(b)")
    plt.ylabel("Utility of Losing Car 7703")

    # 如果要画彩色图 ，颜色为绿色，否则为黑色
    plt.plot(Loser_CarBid, Loser_CarUtility, color='green', linewidth=1.0, linestyle='--')
    plt.scatter(Loser_CarBid, Loser_CarUtility, color='green', linewidths=1.0, marker='*')

    plt.tight_layout()

    plt.savefig('./pdf/exp4_TV_Loser.pdf', dpi=600)
    # plt.savefig('./png/exp4_TV_Loser(in Chinese).png', dpi=600)
    # plt.savefig('./jpeg/图20(b)  OTRAP的真实性验证（败者）.jpeg')
    plt.show()

if __name__ == '__main__':
    TruthfulnessVerification_Winner()
    TruthfulnessVerification_Loser()
