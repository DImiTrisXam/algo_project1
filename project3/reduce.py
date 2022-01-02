import argparse

parser = argparse.ArgumentParser(description='Reduce dimension time series')

parser.add_argument("-d", dest="dataset", help="dataset file", metavar="DFILE")
parser.add_argument("-q", dest="queryset",
                    help="queryset file", metavar="QFILE")
parser.add_argument("-od", dest="outputDataset",
                    help="output dataset file", metavar="ODFILE")
parser.add_argument("-oq", dest="outputQueryset",
                    help="output queryset file", metavar="OQFILE")

args = parser.parse_args()

print(args.dataset)
print(args.queryset)
print(args.outputDataset)
print(args.outputQueryset)
