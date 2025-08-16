#!/usr/bin/env python3
import argparse
import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

pd.options.plotting.backend = "plotly"

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Plot data for LRDemo")
    parser.add_argument("dataFiles", nargs="+", help="Data files name")
    parser.add_argument(
        "-g",
        "--showgrad",
        action="store_true",
    )
    args = parser.parse_args()

    # read data
    datas = []
    for filename in args.dataFiles:
        data = pd.read_csv(filename)
        if not args.showgrad:
            data = data.drop(["f_wg", "f_bg"], axis=1)
        datas.append(data)

    # plot
    fig = make_subplots(
        rows=1,
        cols=len(datas),
        subplot_titles=[f"Plot {f}" for f in args.dataFiles],
    )
    # fig = data.plot(kind="line", title="LR data")

    for i, df in enumerate(datas):
        for col in df.columns:
            fig.add_trace(
                go.Scatter(x=df.index, y=df[col], name=f"{col}{i}"), row=1, col=i + 1
            )

    fig.update_layout(title_text="LR Plot")
    fig.show()
