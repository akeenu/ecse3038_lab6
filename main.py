from flask import Flask, request, jsonify
from flask_pymongo import PyMongo
from marshmallow import Schema, fields, ValidationError
from bson.json_util import dumps
from json import loads

profile_db = {
    "success": True,
    "data": {
        "last_updated": "2/3/2021, 8:48:51 PM",
        "username": "Akeenu Allen",
        "role": "Electronics Engineer",
        "color": "Burgundy"
    }
}

app = Flask(__name__)
app.config["MONGO_URI"] = "mongodb+srv://supm:qwerty123@cluster0.ejobu.mongodb.net/lab3?retryWrites=true&w=majority"
mongo = PyMongo(app)

ClientData = {
    "tank_id": "",
    "percentage_full": 20
}

class levels(Schema):
    tankID = fields.String
    Percent_full = fields.Integer()


@app.route ("/data", methods = ["POST"])
def add_Tank ():
    W_level = (request.json["water_level"])
    P_full = percent(W_level, 10, 200, 0, 100)
    ClientData["tank_id"] = request.json["tank_id"]
    ClientData["percentage_full"] = P_full
    try:
        nwTank = levels().load(ClientData)
        tank_ID = mongo.db.tanks.insert_one(nwTank).inserted_id
        tank = mongo.db.tanks.find_one(tank_ID)
        return loads(dumps(tank))
    except ValidationError as ve:
         return ve.messages, 400



if __name__ =="__main__" :
    app.run (port=3000, debug = True )
