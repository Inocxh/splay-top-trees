import json
import sys
import pandas as pd

def main():
    if len(sys.argv) != 2:
        exit(f"Usage {sys.argv[0]} <mst|mst_ogbl|connec>")
    name = sys.argv[1]
      
    df = pd.read_json(f"results/{name}.jsonl", lines=True)
    df = df.drop(columns=['name'])
    df['time_ns'] = df['time_ns'] / 1000
    df = df.rename(columns={"num_vertices": "n", "num_edges": "m", "time_ns": "time (\mu s)"})
    with open(f"plots/table_{name}.tex", 'w') as tf:
        tf.write(df.to_latex())





if __name__ == "__main__":
    main()