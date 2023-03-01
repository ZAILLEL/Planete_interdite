# -*- coding: utf-8 -*-
"""
Created on Wed Mar 1 14:51:14 2023
@author: diopa
"""

# simple example demonstrating how to control a Tello using your keyboard.
# For a more fully featured example see manual-control-pygame.py
#
# Use W, A, S, D for moving, E, Q for rotating and R, F for going up and down.
# When starting the script the Tello will takeoff, pressing ESC makes it land
#  and the script exit.

from djitellopy import Tello
import cv2
import numpy as np
import math

# Create Tello Object
tello = Tello()

# Connect to Tello
tello.connect()

# Fonction pour détecter les cercles dans une image
def detect_circles(image):
    # Convertir l'image en niveaux de gris
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    
    # Réduire le bruit de l'image
    gray = cv2.GaussianBlur(gray, (5, 5), 0)
    
    # Détecter les cercles dans l'image
    circles = cv2.HoughCircles(gray, cv2.HOUGH_GRADIENT, 1, 20, param1=50, param2=30, minRadius=0, maxRadius=0)
    
    if circles is not None:
        circles = np.round(circles[0, :]).astype("int")
    
    return circles

print(f"Battery Life Percentage: {tello.get_battery()}")

# Start the video Stream
tello.streamon()

# Get the frame reader
frame_reader = tello.get_frame_read()

# Load the Haar Cascade Classifier for Face Detection
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

while True:
    # In reality you want to display frames in a separate thread. Otherwise
    # they will freeze while the drone moves.

    # Read a video frame from Tello
    img = frame_reader.frame

    # Detect Faces in the Video Frame
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, 1.3, 5)

    # Draw a Rectangle Around Each Detected Face
    for (x, y, w, h) in faces:
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # Have OpenCV display the Video Frame
    cv2.imshow("Tello View", img)

    # If ESC is pressed then stop
    key = cv2.waitKey(1) & 0xff
    if key == 27: # ESC
        break

cv2.destroyWindow('Tello View')
cv2.destroyAllWindows()
tello.streamoff()
