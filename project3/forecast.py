from sklearn.preprocessing import MinMaxScaler
import numpy as np
import pandas as pd
import keras
import matplotlib.pyplot as plt
import math
import argparse
from functions import split_dataset_LSTM, train_model_LSTM_hyperparams

parser = argparse.ArgumentParser(description='Forecast time series')

parser.add_argument("-d", dest="dataset", help="dataset file", metavar="FILE")
parser.add_argument("-n", dest="numOfTimeSeries", metavar='N',
                    type=int, help='number of time series selected')

args = parser.parse_args()

if args.dataset == None:
    print("-d parameter is missing. Exiting")
    exit()

if args.numOfTimeSeries == None:
    print("-n parameter is missing. Exiting")
    exit()

# number of time series from dataset
N = args.numOfTimeSeries
dataset = args.dataset

df = pd.read_csv(dataset, header=None, sep='\t')
print("Number of rows and columns:", df.shape)
# df.head(5)

time_array = np.array([x for x in range(len(df.columns))])
df2 = pd.DataFrame(time_array)

training_percent = 0.8
look_back = 60
sc = MinMaxScaler(feature_range=(0, 1))

# load model trained with whole dataset
model_N = keras.models.load_model("./models/forecast_model")

# pick some examples of stocks
stocks = [0, 10, 21, 99, 350]

# compare results of the two models for each example
for i in stocks:
    # Creating a data structure with 'look_back' time-steps and 1 output
    X_train, y_train, X_test, train_size = split_dataset_LSTM(
        df, i, training_percent, sc, look_back)

    model, history = train_model_LSTM_hyperparams(
        X_train, y_train, 6, 512, 10, 128)

    # Getting the predicted stock price (trained with 1 timeseries)
    predicted_stock_price = model.predict(X_test)
    predicted_stock_price = sc.inverse_transform(predicted_stock_price)

    # Getting the predicted stock price (trained with whole dataset)
    predicted_stock_price_N = model_N.predict(X_test)
    predicted_stock_price_N = sc.inverse_transform(predicted_stock_price_N)

    fig, (ax1, ax2) = plt.subplots(1, 2, sharey=True, figsize=(14, 7))
    fig.suptitle(
        'Model trained with 1 timeseries (left) - Model trained with whole dataset (right)')
    ax1.plot(df2.iloc[train_size:].values, df.iloc[0,
                                                   train_size:].values, color='red', label='Real Stock Price')
    ax1.plot(df2.iloc[train_size:].values, predicted_stock_price,
             color='blue', label='Predicted Stock Price')
    ax1.set_title('Stock Price Prediction')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Stock Price')
    ax1.legend()

    ax2.plot(df2.iloc[train_size:].values, df.iloc[0,
                                                   train_size:].values, color='red', label='Real Stock Price')
    ax2.plot(df2.iloc[train_size:].values, predicted_stock_price_N,
             color='blue', label='Predicted Stock Price')
    ax2.set_title('Stock Price Prediction')
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Stock Price')
    ax2.legend()
