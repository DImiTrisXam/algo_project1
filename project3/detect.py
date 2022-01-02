import argparse

parser = argparse.ArgumentParser(description='Detect anomalies in time series')

parser.add_argument("-d", dest="dataset", help="dataset file", metavar="FILE")
parser.add_argument("-n", dest="numOfTimeSeries", metavar='N',
                    type=int, help='number of time series selected')
parser.add_argument("-mae", dest="mae", metavar='m',
                    type=float, help='error value as double')

args = parser.parse_args()

print(args.dataset)
print(args.numOfTimeSeries)
print(args.mae)
