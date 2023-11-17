from crypt import methods
import json
from flask import Flask, jsonify, request, render_template

app = Flask(__name__)

import serial
serial_stm = serial.Serial('/dev/ttyAMA0')
serial_stm.reset_input_buffer()
serial_stm.reset_output_buffer()

welcome = "Welcome to 3ESE API! (RODIER & PICHERY)"
temperature = []
pressure = []

@app.route('/')
def hello_world():
    return 'Hello, World!\n'

@app.errorhandler(404)
def page_not_found(error):
    return render_template("page_not_found.html"), 404
############################
#   BEGIN api/welcome

# Retrieve the entire sentence
@app.route('/api/welcome/', methods=['GET'])
def retrieve_sentence():
    return jsonify({"message": welcome})

# Retrieve a specific letter at position x
@app.route('/api/welcome/<int:index>', methods=['GET'])
def retrieve_letter(index):
    if 0 <= index < len(welcome):
        return jsonify({"letter": welcome[index]})
    else:
        return jsonify({"error": "Index out of range"}), 400  # Return a 400 Bad Request status

# Create or change the sentence
@app.route('/api/welcome/', methods=['POST'])
def change_sentence():
    global welcome
    data = request.get_json()
    if 'message' in data:
        welcome = data['message']
        return jsonify({"message": welcome})
    else:
        return jsonify({"error": "Invalid data"}), 400  # Return a 400 Bad Request status

# Insert a new word at position x
@app.route('/api/welcome/<int:index>', methods=['PUT'])
def insert_word(index):
    global welcome
    data = request.get_json()
    if 'word' in data:
        word_to_insert = data['word']
        if 0 <= index <= len(welcome):
            welcome = welcome[:index] + word_to_insert + welcome[index:]
            return jsonify({"message": welcome})
        else:
            return jsonify({"error": "Index out of range"}), 400  # Return a 400 Bad Request status
    else:
        return jsonify({"error": "Invalid data"}), 400  # Return a 400 Bad Request status

# Change the letter at position x
@app.route('/api/welcome/<int:index>', methods=['PATCH'])
def change_letter(index):
    global welcome
    data = request.get_json()
    if 'letter' in data:
        new_letter = data['letter']
        if 0 <= index < len(welcome):
            welcome = welcome[:index] + new_letter + welcome[index + 1:]
            return jsonify({"message": welcome})
        else:
            return jsonify({"error": "Index out of range"}), 400  # Return a 400 Bad Request status
    else:
        return jsonify({"error": "Invalid data"}), 400  # Return a 400 Bad Request status

# Delete a letter at position x
@app.route('/api/welcome/<int:index>', methods=['DELETE'])
def delete_letter(index):
    global welcome
    if 0 <= index < len(welcome):
        welcome = welcome[:index] + welcome[index + 1:]
        return jsonify({"message": welcome})
    else:
        return jsonify({"error": "Index out of range"}), 400  # Return a 400 Bad Request status

# Delete the entire sentence
@app.route('/api/welcome/', methods=['DELETE'])
def delete_sentence():
    global welcome 
    welcome = ""
    return jsonify({"message": welcome})

#   ENDING api/welcome  
############################
#   BEGIN api/temp

# Retrieve new temperature
@app.route('/api/temp/', methods=['POST'])
def retrieve_new_temp():
    serial_stm.write(b'GET_T\r')
    global temperature
    new_temp = serial_stm.readline().decode()
    temperature.append(new_temp)
    print(new_temp)
    print(temperature)
    return jsonify({"Temp": temperature[len(temperature)]})

# Retrieve a specific temperature at position x
@app.route('/api/temp/<int:index>', methods=['GET'])
def retrieve_temp(index):
    global temperature
    if 0 <= index < len(temperature):
        return jsonify({"Temp": temperature[index]})
    else:
        return jsonify({"error": "Index out of range"}), 400  # Return a 400 Bad Request status

# Retrieve all previous temperatures
@app.route('/api/temp/', methods=['GET'])
def retrieve_all_temp():
    global temperature
    #Dsadaad

# Delete a temperature at position x
@app.route('/api/temp/<int:index>', methods=['DELETE'])
def delete_temp(index):
    global temperature
    if 0 <= index < len(temperature):
        return jsonify({"Delete": temperature.pop(index)})
    else:
        return jsonify({"error": "Index out of range"}), 400  # Return a 400 Bad Request status

# END api/temp
############################
# BEGIN api/pres

# Retrieve new pressure
@app.route('/api/pres/', methods=['POST'])
def retrieve_new_pres():
    serial_stm.write('GET_P')
    global pressure
    new_pres = serial_stm.readline().decode()
    pressure.append(new_pres)
    return jsonify({"Temp": pressure[len(pressure)]})

# Retrieve a specific pressure at position x
@app.route('/api/pres/<int:index>', methods=['GET'])
def retrieve_pres(index):
    global pressure
    if 0 <= index < len(pressure):
        return jsonify({"Temp": pressure[index]})
    else:
        return jsonify({"error": "Index out of range"}), 400  # Return a 400 Bad Request status

# Retrieve all previous pressures
@app.route('/api/pres/', methods=['GET'])
def retrieve_all_pres():
    global temperature
    #Dsadaad

# Delete a pressures at position x
@app.route('/api/pres/<int:index>', methods=['DELETE'])
def delete_pres(index):
    global pressure
    if 0 <= index < len(pressure):
        return jsonify({"Delete": pressure.pop(index)})
    else:
        return jsonify({"error": "Index out of range"}), 400  # Return a 400 Bad Request status

# END api/pres
############################
# BEGIN api/scale
#
#@app.route('/api/scale/', methods=['GET'])
#def return_scale():
#    global scale
#
#@app.route('/api/scale/<int:index>', methods=['POST'])
#def return_scale():
#    global scale
#
# END api/scale
############################

@app.route('/api/angle/', methods=['GET'])
def get_angle():
    serial_stm.write('GET_A')
    angle = serial_stm.readline().decode()
    return jsonify({"Angle": angle})
