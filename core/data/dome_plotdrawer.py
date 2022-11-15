import argparse
import json
import sqlite3
import plotly.graph_objects as go
import os

parser = argparse.ArgumentParser(description='Draw charts via plotly library.')
parser.add_argument('-c', '--config', help='path to the dome_core configuration file', required=True)
parser.add_argument('-t', '--tables', nargs='*', help='tables to read', required=True)
parser.add_argument('-y', '--y-legends', nargs='*', help='Y axis legend for values', required=True)
parser.add_argument('-p', '--period', type=int, help='period in seconds to read data', required=True)
parser.add_argument('-l', '--titles', nargs='*', help='X and Y axis titles', required=True)
parser.add_argument('-n', '--name', help='Chart name', required=True)
parser.add_argument('-f', '--file', help='File to save', required=True)
args = parser.parse_args()

core_config_file = open(args.config)
core_config = json.load(core_config_file)
db_path = core_config['database']['path']

db_conn = sqlite3.connect(db_path)
db_cur = db_conn.cursor()

fig = go.Figure()
index = 0
for table in args.tables:
    res = db_cur.execute(f"select strftime('%m.%d|%H:%M',timestamp,'localtime'), value from '{table}' \
    where timestamp >= (select DATETIME('now', '-{args.period} second'))")
    data = res.fetchall()

    x_data = []
    y_data = []
    for value in data:
        x_data.append(value[0])
        y_data.append(value[1])

    char_data = dict(
        Data = x_data,
        Value = y_data
    )

    fig.add_trace(go.Scatter(x=x_data, y=y_data,
                        mode='lines',
                        name=args.y_legends[index]))
    index = index + 1
fig.update_layout(title=args.name,
                   xaxis_title=args.titles[0],
                   yaxis_title=args.titles[1],
                   showlegend=True)
fig.write_image(args.file, engine="kaleido")

print(args.file, end='')

# deps:
#   plotly
#   pandas
