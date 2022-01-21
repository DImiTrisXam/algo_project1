from keras.callbacks import EarlyStopping
# from sklearn.metrics import mean_absolute_error
from sklearn.metrics import mean_squared_error
from sklearn.preprocessing import MinMaxScaler
from keras.layers import *
from keras.layers import Dropout
from keras.layers import LSTM
from keras.layers import Dense
from keras.models import Sequential
import numpy as np
import pandas as pd
import keras
import matplotlib.pyplot as plt
import math


def create_dataset(dataset, look_back=1):
    """convert an array of values into a dataset matrix"""
    dataX, dataY = [], []

    for i in range(look_back, len(dataset)):
        dataX.append(dataset[i-look_back:i, 0])
        dataY.append(dataset[i, 0])

    return np.array(dataX), np.array(dataY)


def split_dataset_LSTM(dataset, row, percent, scaler, look_back=1):
    """split dataset to train and test sets for LSTM"""
    train_size = int(len(dataset.columns) * percent)
    # print(train_size)

    training_set = dataset.iloc[row, 1:train_size].values.reshape(-1, 1)
    test_set = dataset.iloc[row, train_size-look_back:].values.reshape(-1, 1)

    # Feature Scaling
    training_set_scaled = scaler.fit_transform(training_set)
    test_set_scaled = scaler.transform(test_set)

    # Creating a data structure with 60 time-steps and 1 output
    X_train, y_train = create_dataset(training_set_scaled, look_back)
    X_test, y_test = create_dataset(test_set_scaled, look_back)
    # reshape input to be [samples, time steps, features]
    X_train = np.reshape(X_train, (X_train.shape[0], X_train.shape[1], 1))
    X_test = np.reshape(X_test, (X_test.shape[0], X_test.shape[1], 1))

    return X_train, y_train, X_test, train_size


def train_model_LSTM(X_train, y_train):
    """train LSTM model"""

    model = Sequential()
    # Adding the first LSTM layer and some Dropout regularisation
    model.add(LSTM(units=50, return_sequences=True,
              input_shape=(X_train.shape[1], 1)))
    model.add(Dropout(0.2))
    # Adding a second LSTM layer and some Dropout regularisation
    model.add(LSTM(units=50, return_sequences=True))
    model.add(Dropout(0.2))
    # Adding a third LSTM layer and some Dropout regularisation
    model.add(LSTM(units=50, return_sequences=True))
    model.add(Dropout(0.2))
    # Adding a fourth LSTM layer and some Dropout regularisation
    model.add(LSTM(units=50))
    model.add(Dropout(0.2))
    # Adding the output layer
    model.add(Dense(units=1))

    # Compiling the RNN
    model.compile(optimizer='adam', loss='mean_squared_error')

    # Fitting the RNN to the Training set
    model.fit(X_train, y_train, epochs=10, batch_size=32)

    return model


def train_model_LSTM_increment(dataframe, N, percent, scaler, look_back=1):
    """train LSTM model incrementallly"""

    X_train, y_train, X_test, train_size = split_dataset_LSTM(
        dataframe, 0, percent, scaler, look_back)
    model = train_model_LSTM(X_train, y_train)

    for i in range(1, N):
        X_train, y_train, X_test, train_size = split_dataset_LSTM(
            dataframe, i, percent, scaler, look_back)

        # Fitting the RNN to the Training set
        model.fit(X_train, y_train, epochs=10, batch_size=32)

    return model


def train_model_LSTM_hyperparams(X_train, y_train, layers=4, units_=50, epochs_=10, batch_size_=32):
    """train LSTM model with different hyperparameters"""

    model = Sequential()
    # Adding the first LSTM layer and some Dropout regularisation
    model.add(LSTM(units=units_, return_sequences=True,
              input_shape=(X_train.shape[1], 1)))
    model.add(Dropout(0.2))

    for i in range(layers-2):
        # Adding another LSTM layer and some Dropout regularisation
        model.add(LSTM(units=units_, return_sequences=True))
        model.add(Dropout(0.2))

    # Adding last LSTM layer and some Dropout regularisation
    model.add(LSTM(units=units_))
    model.add(Dropout(0.2))
    # Adding the output layer
    model.add(Dense(units=1))

    # Compiling the RNN
    model.compile(optimizer='adam', loss='mean_squared_error')

    # Fitting the RNN to the Training set
    history = model.fit(X_train, y_train, epochs=epochs_,
                        batch_size=batch_size_, validation_split=0.1)

    return model, history


def printHyperparams(file, X_train, y_train, layers=4, units_=50, epochs_=10, batch_size_=32):
    """print hyperparameters to output/file"""

    model, history = train_model_LSTM_hyperparams(
        X_train, y_train, layers, units_, epochs_, batch_size_)

    # write data
    line = "{}\t{}\t{}\t{}\t{:.4f}\t{:.4f}\n"
    file.write(line.format(layers, units_, epochs_, batch_size_,
                           history.history['loss'][-1], history.history['val_loss'][-1]))

    return model
