# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""
# Importer les bibliothèques nécessaires
import cv2
import numpy as np
import time
import math
import socket
from djitellopy import tello


# Paramètres pour se connecter au drone
tello_address = ('192.168.10.1', 8889)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(('', 9001))

# Fonction pour envoyer des commandes au drone
def send(message):
    try:
        sock.sendto(message.encode(), tello_address)
        print("Envoi de la commande: " + message)
    except Exception as e:
        print("Erreur d'envoi de la commande: " + message + " : " + str(e))

# Fonction pour récupérer une image de la caméra du drone
def get_frame():
    cap = cv2.VideoCapture('udp://@0.0.0.0:11111')
    ret, frame = cap.read()
    cap.release()
    return frame

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

# Connexion au drone
send("command")
time.sleep(1)

# Décollage du drone
send("takeoff")
time.sleep(5)
  # Récupérer l'image de la caméra
frame_reader = tello.get_frame_read()
# Boucle principale
while True:
  
    # Read a video frame from Tello
    img = frame_reader.frame

    
    # Détecter les cercles dans l'image
    circles = detect_circles(img)
    
    # Si des cercles ont été détectés, calculer leur distance par rapport au drone
    if circles is not None:
        for (x, y, r) in circles:
            distance = 100 / math.tan(math.radians(r*2))
            print("Distance: " + str(distance) + " cm")
            
            # Si la distance est inférieure à 50 cm, faire avancer le drone
            if distance < 50:
                send("forward 20")
    
    # Attendre avant la prochaine itération
    time.sleep(0.1)

       
