# -*- coding: utf-8 -*-
"""
@file: train_svm.py
@project: Smart Anti-Tremor Glove - Edge-AI Pipeline
@authors: Roshan Kumar Mishra & Bijendra Yadav (SRMIST)
Description: Script to train a 24-dimensional RBF-kernel SVM for tremor 
             discrimination and export it to a bare-metal header via micromlgen.
"""

import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from micromlgen import port

# 1. Setup mock structure for your 24-dimensional feature array windows
print("Generating feature extraction validation arrays...")
np.random.seed(42)
mock_tremor = np.random.normal(loc=[1.5, 0.8, -2.0, 3.5, 5.5]*4 + [1.0, 2.0, 3.0, 4.0], scale=0.1, size=(100, 24))
mock_normal = np.random.normal(loc=[0.2, 0.1, -0.3, 0.4, 0.7]*4 + [0.1, 0.2, 0.3, 0.4], scale=0.05, size=(100, 24))

X = np.vstack((mock_tremor, mock_normal))
y = np.hstack((np.ones(100), np.zeros(100))) # 1 = Tremor, 0 = Normal

# 2. Stratified Train-Test Split (80/20 matching project specifications)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, stratify=y, random_state=42)

# 3. Fit scaling arrays
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)

# 4. Instantiate and fit your exact RBF-kernel SVM configuration
print("Training system-level SVM Model parameters...")
model = SVC(kernel='rbf', C=1.0, gamma=0.01)
model.fit(X_train_scaled, y_train)

train_acc = model.score(X_train_scaled, y_train) * 100
test_acc = model.score(X_test_scaled, y_test) * 100

print(f"[*] Target Accuracy Achieved - Training: {train_acc:.2f}%, Testing: {test_acc:.2f}%")
print(f"[*] Train-Test Operational Variance Gap: {abs(train_acc - test_acc):.2f}%")

# 5. Export structural metrics to high-performance C++ header array for Teensy 4.1 deployment
print("Exporting bare-metal model header matrix via micromlgen...")
with open("svm_model.h", "w") as f:
    f.write("// Transpiled Embedded Matrix Model Array Configuration\n")
    f.write(port(model))

print("[SUCCESS]: 'svm_model.h' written to workspace root.")
print(f"Means array parameters: {list(scaler.mean_)}")
print(f"Scales array parameters: {list(scaler.scale_)}")
