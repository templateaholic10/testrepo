# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

import pandas
import numpy as np

numberings = ['01', '02', '03', '04', '05', '06']

for numbering in numberings:
    df = pandas.read_csv("square_initialtest" + numbering + ".csv")
    df['KL_01'] = 0
    df['KL_02'] = 0
    df['KL_03'] = 0
    df['KL_12'] = 0
    df['KL_13'] = 0
    df['KL_23'] = 0

    # Jensen Shannon Divegenceの計算
    for i in df.index:
        mu1 = np.matrix([[df.loc[i, 'mu^0_0']], [df.loc[i, 'mu^0_1']]])
        mu2 = np.matrix([[df.loc[i, 'mu^1_0']], [df.loc[i, 'mu^1_1']]])
        sigma1 = np.matrix([[df.loc[i, 'sigma^0_00'], df.loc[i, 'sigma^0_01']], [
                           df.loc[i, 'sigma^0_10'], df.loc[i, 'sigma^0_11']]])
        sigma2 = np.matrix([[df.loc[i, 'sigma^1_00'], df.loc[i, 'sigma^1_01']], [
                           df.loc[i, 'sigma^1_10'], df.loc[i, 'sigma^1_11']]])
        df.loc[i, 'KL_01'] = ((sigma2.I * sigma1).trace() + (sigma1.I * sigma2).trace() + (
            mu1 - mu2).T * (sigma1.I + sigma2.I) * (mu1 - mu2)) / 4 - 1
    for i in df.index:
        mu1 = np.matrix([[df.loc[i, 'mu^0_0']], [df.loc[i, 'mu^0_1']]])
        mu2 = np.matrix([[df.loc[i, 'mu^2_0']], [df.loc[i, 'mu^2_1']]])
        sigma1 = np.matrix([[df.loc[i, 'sigma^0_00'], df.loc[i, 'sigma^0_01']], [
                           df.loc[i, 'sigma^0_10'], df.loc[i, 'sigma^0_11']]])
        sigma2 = np.matrix([[df.loc[i, 'sigma^2_00'], df.loc[i, 'sigma^2_01']], [
                           df.loc[i, 'sigma^2_10'], df.loc[i, 'sigma^2_11']]])
        df.loc[i, 'KL_02'] = ((sigma2.I * sigma1).trace() + (sigma1.I * sigma2).trace() + (
            mu1 - mu2).T * (sigma1.I + sigma2.I) * (mu1 - mu2)) / 4 - 1
    for i in df.index:
        mu1 = np.matrix([[df.loc[i, 'mu^0_0']], [df.loc[i, 'mu^0_1']]])
        mu2 = np.matrix([[df.loc[i, 'mu^3_0']], [df.loc[i, 'mu^3_1']]])
        sigma1 = np.matrix([[df.loc[i, 'sigma^0_00'], df.loc[i, 'sigma^0_01']], [
                           df.loc[i, 'sigma^0_10'], df.loc[i, 'sigma^0_11']]])
        sigma2 = np.matrix([[df.loc[i, 'sigma^3_00'], df.loc[i, 'sigma^3_01']], [
                           df.loc[i, 'sigma^3_10'], df.loc[i, 'sigma^3_11']]])
        df.loc[i, 'KL_03'] = ((sigma2.I * sigma1).trace() + (sigma1.I * sigma2).trace() + (
            mu1 - mu2).T * (sigma1.I + sigma2.I) * (mu1 - mu2)) / 4 - 1
    for i in df.index:
        mu1 = np.matrix([[df.loc[i, 'mu^1_0']], [df.loc[i, 'mu^1_1']]])
        mu2 = np.matrix([[df.loc[i, 'mu^2_0']], [df.loc[i, 'mu^2_1']]])
        sigma1 = np.matrix([[df.loc[i, 'sigma^1_00'], df.loc[i, 'sigma^1_01']], [
                           df.loc[i, 'sigma^1_10'], df.loc[i, 'sigma^1_11']]])
        sigma2 = np.matrix([[df.loc[i, 'sigma^2_00'], df.loc[i, 'sigma^2_01']], [
                           df.loc[i, 'sigma^2_10'], df.loc[i, 'sigma^2_11']]])
        df.loc[i, 'KL_12'] = ((sigma2.I * sigma1).trace() + (sigma1.I * sigma2).trace() + (
            mu1 - mu2).T * (sigma1.I + sigma2.I) * (mu1 - mu2)) / 4 - 1
    for i in df.index:
        mu1 = np.matrix([[df.loc[i, 'mu^1_0']], [df.loc[i, 'mu^1_1']]])
        mu2 = np.matrix([[df.loc[i, 'mu^3_0']], [df.loc[i, 'mu^3_1']]])
        sigma1 = np.matrix([[df.loc[i, 'sigma^1_00'], df.loc[i, 'sigma^1_01']], [
                           df.loc[i, 'sigma^1_10'], df.loc[i, 'sigma^1_11']]])
        sigma2 = np.matrix([[df.loc[i, 'sigma^3_00'], df.loc[i, 'sigma^3_01']], [
                           df.loc[i, 'sigma^3_10'], df.loc[i, 'sigma^3_11']]])
        df.loc[i, 'KL_13'] = ((sigma2.I * sigma1).trace() + (sigma1.I * sigma2).trace() + (
            mu1 - mu2).T * (sigma1.I + sigma2.I) * (mu1 - mu2)) / 4 - 1
    for i in df.index:
        mu1 = np.matrix([[df.loc[i, 'mu^2_0']], [df.loc[i, 'mu^2_1']]])
        mu2 = np.matrix([[df.loc[i, 'mu^3_0']], [df.loc[i, 'mu^3_1']]])
        sigma1 = np.matrix([[df.loc[i, 'sigma^2_00'], df.loc[i, 'sigma^2_01']], [
                           df.loc[i, 'sigma^2_10'], df.loc[i, 'sigma^2_11']]])
        sigma2 = np.matrix([[df.loc[i, 'sigma^3_00'], df.loc[i, 'sigma^3_01']], [
                           df.loc[i, 'sigma^3_10'], df.loc[i, 'sigma^3_11']]])
        df.loc[i, 'KL_23'] = ((sigma2.I * sigma1).trace() + (sigma1.I * sigma2).trace() + (
            mu1 - mu2).T * (sigma1.I + sigma2.I) * (mu1 - mu2)) / 4 - 1

    # JSdの平均と最大値の計算
    df['KLmean'] = (df.iloc[:, 59] + df.iloc[:, 60] + df.iloc[:,
                                                              61] + df.iloc[:, 62] + df.iloc[:, 63] + df.iloc[:, 64]) / 6
    for i in df.index:
        df.loc[i, 'KLmax'] = max(df.iloc[i, 59], df.iloc[i, 60], df.iloc[
                             i, 61], df.iloc[i, 62], df.iloc[i, 63], df.iloc[i, 64])

    # まとめる
    subdf = df.iloc[:, [59, 60, 61, 62, 63, 64, 65, 66, 28, 57, 58]]

    subdf.to_csv('for_linear' + numbering + '.csv', index=False)
