from bson.json_util import JSONOptions
from flask import Flask, render_template, redirect, url_for, request, jsonify
from flask.templating import render_template
import pymongo
import json
from bson import ObjectId

client = pymongo.MongoClient()

db = client['nhl']
collection = db['data']

lookupCollection = db['Lookup Data']

app = Flask(__name__)

@app.route('/')
def home():
    players = collection.find()
    return render_template("dashboard.html", players=players)

@app.route('/addPlayer', methods=['POST'])
def addPlayer():
    rosteredPlayer = collection.find_one({'name': request.form['playerName']})
    players = collection.find()
    lookupPlayers = lookupCollection.find()
    found = False
    for lookupPlayer in lookupPlayers:
        if lookupPlayer.get('Name') == request.form['playerName']:
            found = True
            if rosteredPlayer is None:
                collection.insert_one({'name':request.form['playerName'], 
                                       'fantasyTeamName': str(players[0].get('fantasyTeamName')),
                                       'number' : lookupPlayer.get('Number'),
                                       'position' : lookupPlayer.get('Position'),
                                       'team' : lookupPlayer.get('Team'),
                                       'goals': lookupPlayer.get('Goals'),
                                       'assists' : lookupPlayer.get('Assists'),
                                       'points' : lookupPlayer.get('Points')
                                      })
                return redirect(url_for('home'))
            else:
                return '<p>That player already exists</p>'
            break
        
    if not found:
        return '<p>That player does not exist</p>'

@app.route('/deletePlayer', methods=['POST'])
def deletePlayer():
    players = collection.find()
    print(type(players[0].get('_id')))
    collection.delete_one({'_id':ObjectId(str(request.form['deletePlayer']))})
    return redirect(url_for('home'))

@app.route('/updateTeamName', methods=['POST'])
def updateTeamName():
    players = collection.find()
    for player in players:
        collection.update_one({'_id':ObjectId(str(player.get('_id')))}, {'$set' : { 'fantasyTeamName' : request.form['teamName'] }})
    return redirect(url_for('home'))

app.run(debug=True)

