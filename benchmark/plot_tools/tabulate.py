import json
import sys
import pandas as pd

def main():
    usage = ["mst", "mst_ogbl", "connec", "all"]

    if len(sys.argv) != 2 or (sys.argv[1] not in usage):
        exit(f"Usage {sys.argv[0]} <{"|".join(usage)}>")
    
    to_process = []
    if sys.argv[1] == "all":
        to_process = ["mst", "mst_ogbl", "connec"]
    else:
        to_process = [sys.argv[1]]

    for cat in to_process:
        splay_top_tree_df = pd.read_json(f"results/splay_top_tree/{cat}.jsonl", lines=True)
        splay_top_tree_df = splay_top_tree_df.drop(columns=['name'])
        splay_top_tree_df['time_ns'] = splay_top_tree_df['time_ns'] / 1000

        sttc_df = pd.read_json(f"results/splay_top_tree/{cat}.jsonl", lines=True)
        sttc_df = sttc_df.drop(columns=['name'])
        sttc_df['time_ns'] = sttc_df['time_ns'] / 1000

        sttrs_df = pd.read_json(f"results/splay_top_tree/{cat}.jsonl", lines=True)
        sttrs_df = sttrs_df.drop(columns=['name'])
        sttrs_df['time_ns'] = sttrs_df['time_ns'] / 1000

        df = pd.concat([splay_top_tree_df, sttc_df['time_ns'], sttrs_df['time_ns']], axis=1)
        with open(f"plots/table_{cat}.tex", 'w') as tf:

            tf.write(df.to_latex(header=["Edges", "Vertices", "Splay ($\\mu s$)", "STT-C ($\\mu s$)", "STT-RS ($\\mu s$)"], index=False, escape=False))





if __name__ == "__main__":
    main()