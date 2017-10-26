# Importando bibliotecas necessárias
import time
import os
import random
import RPi.GPIO as GPIO
from twython import TwythonStreamer

#Hashtag a ser pesquisada
TERMS = '#Arduino'
TERMS = '#RaspberryPi'

#Pino da GPIO conectado ao LED
#LED = 12

music = None

# Crie o APP para obter as chaves e tokens da API
#https://apps.twitter.com
#Autenticacao na aplicacao Twitter
APP_KEY = 'sua_chave'
APP_SECRET = 'seu_app_secret'
OAUTH_TOKEN = 'seu_token'
OAUTH_TOKEN_SECRET = 'seu_token_secret'

# Reproduz som usando o mplayer
bb8_sounds = ['mplayer /home/pi/Documentos/GitHub/hashtags.mp3', 'mplayer /home/pi/Documentos/GitHub/hashtags2.mp3']

# randonizar os 2 sons
def sound():
	randomSound = random.choice (bb8_sounds)
	music = os.popen(randomSound)

class BlinkyStreamer(TwythonStreamer):
        def on_success(self, data):
                if 'text' in data:
                        print data['text'].encode('utf-8')
                        print
                        for x in range(0,1):
                                #GPIO.output(LED, 1)
                                #time.sleep(0.5)
                                #GPIO.output(LED, 0)
                                time.sleep(0.5)
                                sound()

#Definicoes da GPIO
#GPIO.setmode(GPIO.BOARD)
#GPIO.setup(LED, GPIO.OUT)
#GPIO.output(LED, GPIO.LOW)

#Loop principal
try:
        stream = BlinkyStreamer(APP_KEY, APP_SECRET, OAUTH_TOKEN, OAUTH_TOKEN_SECRET)
        stream.statuses.filter(track=TERMS)
except KeyboardInterrupt:
        time.sleep(0.1)
        #GPIO.cleanup()
