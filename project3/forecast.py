import argparse

parser = argparse.ArgumentParser(description='Forecast time series')

parser.add_argument("-d", dest="dataset", help="dataset file", metavar="FILE")
parser.add_argument("-n", dest="numOfTimeSeries", metavar='N',
                    type=int, help='number of time series selected')

args = parser.parse_args()

print(args.dataset)
print(args.numOfTimeSeries)
