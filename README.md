# DeepSC
Vulnerability detection with GNN

## Code Parse

Put your source code to code-slicer/raw_code (*only supports C)

'''
   ../multi-slicer.sh
'''

each of your source code will then be parsed into node.csv & edge.csv, and automatically stored into data/parsed/[codeNo._Vul].c
if the source code is a vulnerable one, then name Vul 1, else 0. (e.g. 173_1.c)

## Data Preprocessing

data_processing directory

* Separate edge types into DFG-edge and CFG-edge

* Generate ggnn_json_data
    * Just a simple json file with only file names

* Prepare Word2Vec model
    * No pre-trained model, so you need to generate yourself
    * Use data_processing/word2vec_train.py

* extract_slices.ipynb
    * Tokenize line-sliced codes to adapt node/edge info
    * will generate data/[DatasetName]_data_with_slices.json

* create_ggnn_data.py
    * embed more information for graph
        * control flow graph
        * data flow graph
        * full graph
        * ..
    * generate [DatasetName]-full_graph.json
        * vulnerability(0,1)
        * graph
        * node feature vector
        * file name/path
        * code

## Prepare input to Gated GNN

* full_data_prep_script.ipynb
    * create [DatasetName]-line-ggnn.json
        * vulnerability(0,1)
        * graph
        * node feature vector
        * original/symbolic tokens


## Run GGNN (Devign Implementation)

* Copy implementation of Devign
    * need json dataset that contains:
        * vulnerability(0,1)
        * graph
        * node feature vector

    * need 3 json dataset, for train-valid-test

 
