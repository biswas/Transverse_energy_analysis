import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
from matplotlib import ticker

df = pd.read_csv('~/rhip/analysisCodes/fitResults5_python.csv')

cols = ['CollEn','mass', 'centrality', 'beta', 'temp', 'dETdEtaTotal']
x = [i for i, _ in enumerate(cols)]
colors = ['#000080', '#006400', '#ff0000', '#98fb98', '#ff1493', '#b8860b']
df['particles'] = df['particle'].astype('category')
#for i in range(len(df['particles'])):
#	colors = df['particles'].cat.categories[i]
colors ={df['particles'].cat.categories[i]: colors[i] for i, _ in enumerate(df['particles'].cat.categories)}
fig, axes = plt.subplots(1, len(x)-1, sharey=False, figsize=(15,5))
min_max_range = {}
for col in cols:
	min_max_range[col] = [df[col].min(), df[col].max(), np.ptp(df[col])]
	df[col] = np.true_divide(df[col] - df[col].min(), np.ptp(df[col]))    
for i, ax in enumerate(axes):
    for idx in df.index:
       	particles_category = df.loc[idx, 'particles']
        ax.plot(x, df.loc[idx, cols], colors[particles_category])
    ax.set_xlim([x[i], x[i+1]])
def set_ticks_for_axis(dim, ax, ticks):
    min_val, max_val, val_range = min_max_range[cols[dim]]
    step = val_range / float(ticks-1)
    tick_labels = [round(min_val + step * i, 2) for i in range(ticks)]
    norm_min = df[cols[dim]].min()
    norm_range = np.ptp(df[cols[dim]])
    norm_step = norm_range / float(ticks-1)
    ticks = [round(norm_min + norm_step * i, 2) for i in range(ticks)]
    ax.yaxis.set_ticks(ticks)
    ax.set_yticklabels(tick_labels)
    
for dim, ax in enumerate(axes):
    ax.xaxis.set_major_locator(ticker.FixedLocator([dim]))
    set_ticks_for_axis(dim, ax, ticks=6)
    ax.set_xticklabels([cols[dim]])
    
ax = plt.twinx(axes[-1])
dim = len(axes)
ax.xaxis.set_major_locator(ticker.FixedLocator([x[-2], x[-1]]))
set_ticks_for_axis(dim, ax, ticks=6)
ax.set_xticklabels([cols[-2], cols[-1]])

plt.subplots_adjust(wspace=0)

plt.legend(
    [plt.Line2D((0,1),(0,0), color=colors[cat]) for cat in df['particles'].cat.categories],
    df['particles'].cat.categories,
    bbox_to_anchor=(1.2, 1), loc=2, borderaxespad=0.)

plt.title("Parameters and Transverse Energy Results for Particles")

plt.show()
