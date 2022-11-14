import argparse
import json
import sqlite3
import plotly.graph_objects as go
import numpy as np

parser = argparse.ArgumentParser(description='Draw charts via plotly library.')
parser.add_argument('-c', '--config', help='path to the dome_core configuration file', required=True)
parser.add_argument('-t', '--tables', nargs='*', help='tables to read', required=True)
parser.add_argument('-p', '--period', type=int, help='period in hours to read data', required=True)
args = parser.parse_args()

core_config_file = open(args.config)
core_config = json.load(core_config_file)
db_path = core_config['database']['path']

db_conn = sqlite3.connect(db_path)
db_cur = db_conn.cursor()
res = db_cur.execute(f"select strftime('%m.%d|%H:%M',timestamp,'localtime'), value from '{args.tables[0]}' \
where timestamp >= (select DATETIME('now', '-{args.period * 60 * 60} second'))")
print(res.fetchall())

np.random.seed(1)

N = 100
x = np.random.rand(N)
y = np.random.rand(N)
colors = np.random.rand(N)
sz = np.random.rand(N) * 30

fig = go.Figure()
fig.add_trace(go.Scatter(
    x=x,
    y=y,
    mode="markers",
    marker=go.scatter.Marker(
        size=sz,
        color=colors,
        opacity=0.6,
        colorscale="Viridis"
    )
))
fig.write_image("figure.png", engine="kaleido")

# deps:
#   plotly
#   pandas
