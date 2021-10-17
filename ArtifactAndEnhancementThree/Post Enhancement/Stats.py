import requests
import pandas as pd
from pandas import DataFrame
import os
import pickle
import json
from datetime import timedelta


homeAwayList = []
teamList = []
teamAbbreviationList = []
fullNameList = []
shootsCatchesList = []
rosterStatusList = []
jerseyNumberList = []
positionList = []
timeOnIceList = []
assistsList = []
goalsList = []
shotsList = []
hitsList = []
ppGoalsList = []
ppAssistsList = []
penaltyMinutesList = []
faceOffWinsList = []
faceOffTakenList = []
takeawaysList = []
giveawaysList = []
shortHandedGoalsList = []
shortHandedAssistsList = []
blockedList = []
plusMinusList = []
evenTimeOnIceList = []
ppTimeOnIceList = []
shortHandedTimeOnIceList = []
gameIdList = []
startTimeList = []



#Set up the api call variables
gameData = []
years = ['2021']
seasonType = '02' #02 refers to regular season
maxGameID = 2000 #max number of game events in the year

#loop over the counter and format the api call
for year in years:
    print(year, 'new')
    gameNumId = 0
    for i in range(0, maxGameID):
        try:
            r = requests.get(url='http://statsapi.web.nhl.com/api/v1/game/' + str(year) + seasonType + str(i).zfill(4) + '/feed/live')
            data = r.json()
            gameData.append(data)
        except:
            break

    with open (r'/Users/djmaxey/Desktop//' + str(year) + 'FullDataSet.pkl', 'wb') as f:
        pickle.dump(gameData, f, pickle.HIGHEST_PROTOCOL)

    df = pd.read_pickle(r'/Users/djmaxey/Desktop//' + str(year) + 'FullDataSet.pkl')
    for gameNum in range(1, len(df)):
        for key, value in df[gameNum].items():
            if key == 'liveData':
                dict = value
                for key5, value5 in sorted(dict.items(), reverse=True):
                    if key5 == 'linescore':
                        dict11 = value5
                        for key7, value7 in dict11.items():
                            if key7 == 'periods':
                                if len(value7) > 0:
                                    startTime = value7[0].get('startTime')
                                else:
                                    startTime = '1/1/1999'
                            if key7 == 'teams':
                                dict12 = value7
                    if key5 == 'boxscore':
                        dict2 = value5
                        gameDict = {'id' : '', 'home': '', 'away':''}
                        i = 0
                        for key8, value8 in dict12.items():
                            i += 1
                            if i > 1:
                                break
                            dict13 = value8

                            for key13, value13 in dict13.items():
                                if key13 == 'team':
                                    teamDict = value13
                            for keyTeam, valueTeam in teamDict.items():
                                if keyTeam == 'id':
                                    gameID = valueTeam
                                    if ~(gameID in gameDict.values()):
                                        gameDict['id'] = gameID
                            for key6, value6 in dict2.items():
                                if key6 == 'teams':
                                    dict3 = value6
                                    for key3, value3 in dict3.items():
                                        dict4 = value3
                                        for key2, value2 in dict4.items():
                                            if key2 == 'team':
                                                teamAbbreviation = value2.get('abbreviation')
                                                if ~(teamAbbreviation in gameDict.values()):
                                                    if gameDict.get('away') == '':
                                                        gameDict['away'] = teamAbbreviation
                                                        homeAway = 'away'
                                                        gameNumId += 1
                                                    else:
                                                        gameDict['home'] = teamAbbreviation
                                                        homeAway = 'home'
                                            if key2 == 'players':
                                                dict5 = value2
                                                for key1, value1 in dict5.items():
                                                    dict6 = value1
                                                    for key, value in dict6.items():
                                                        if key == 'person':
                                                            fullName = value.get('fullName')

                                                            shootsCatches = value.get('shootsCatches')
                                                            rosterStatus = value.get('rosterStatus')

                                                        elif key == 'jerseyNumber':
                                                            jerseyNumber = value
                                                        elif key == 'position':
                                                            position = value.get('code')
                                                        elif key == 'stats':
                                                            dict7 = value
                                                            for key4, value4 in dict7.items():
                                                                if key4 == 'skaterStats':
                                                                    timeOnIce = value4.get('timeOnIce')
                                                                    assists = value4.get('assists')
                                                                    goals = value4.get('goals')
                                                                    shots = value4.get('shots')
                                                                    hits = value4.get('hits')
                                                                    ppGoals = value4.get('powerPlayGoals')
                                                                    ppAssists = value4.get('powerPlayAssists')
                                                                    penaltyMinutes = value4.get('penaltyMinutes')
                                                                    faceOffWins = value4.get('faceOffWins')
                                                                    faceOffTaken = value4.get('faceoffTaken')
                                                                    takeaways = value4.get('takeaways')
                                                                    giveaways = value4.get('giveaways')
                                                                    shortHandedGoals = value4.get('shortHandedGoals')
                                                                    shortHandedAssists = value4.get('shortHandedAssists')
                                                                    blocked = value4.get('blocked')
                                                                    plusMinus = value4.get('plusMinus')
                                                                    evenTimeOnIce = value4.get('evenTimeOnIce')
                                                                    ppTimeOnIce = value4.get('powerPlayTimeOnIce')
                                                                    shortHandedTimeOnIce = value4.get('shortHandedTimeOnIce')

                                                                    timeOnIceList.append(timeOnIce)
                                                                    assistsList.append(assists)
                                                                    goalsList.append(goals)
                                                                    shotsList.append(shots)
                                                                    hitsList.append(hits)
                                                                    ppGoalsList.append(ppGoals)
                                                                    ppAssistsList.append(ppAssists)
                                                                    penaltyMinutesList.append(penaltyMinutes)
                                                                    faceOffWinsList.append(faceOffWins)
                                                                    faceOffTakenList.append(faceOffTaken)
                                                                    takeawaysList.append(takeaways)
                                                                    giveawaysList.append(giveaways)
                                                                    shortHandedGoalsList.append(shortHandedGoals)
                                                                    shortHandedAssistsList.append(shortHandedAssists)
                                                                    blockedList.append(blocked)
                                                                    plusMinusList.append(plusMinus)
                                                                    evenTimeOnIceList.append(evenTimeOnIce)
                                                                    ppTimeOnIceList.append(ppTimeOnIce)
                                                                    shortHandedTimeOnIceList.append(shortHandedTimeOnIce)

                                                                    homeAwayList.append(homeAway)

                                                                    startDate = pd.to_datetime(startTime.split('T')[0]).date()
                                                                    hour = int(startTime.split('T')[1].split(':')[0])
                                                                    if hour < 9:
                                                                        startDate = startDate - timedelta(1)
                                                                    startTimeList.append(startDate)
                                                                    teamAbbreviationList.append(teamAbbreviation)
                                                                    positionList.append(position)
                                                                    jerseyNumberList.append(jerseyNumber)
                                                                    shootsCatchesList.append(shootsCatches)
                                                                    rosterStatusList.append(rosterStatus)
                                                                    fullNameList.append(fullName)
                                                                    gameIdList.append(gameNumId)


    df = pd.DataFrame(list(zip(gameIdList, startTimeList, homeAwayList, teamAbbreviationList, fullNameList,
        shootsCatchesList, rosterStatusList, jerseyNumberList,
        positionList, timeOnIceList, assistsList,
        shotsList, goalsList, hitsList, ppGoalsList,
        ppAssistsList, penaltyMinutesList, faceOffWinsList,
        faceOffTakenList, takeawaysList, giveawaysList,
        shortHandedGoalsList, shortHandedAssistsList, blockedList, plusMinusList,
        evenTimeOnIceList, ppTimeOnIceList, shortHandedTimeOnIceList)),
                      columns = ['ID', 'StartTime', 'HomeAway', 'Team', 'Name',
                                 'Shot', 'Status', 'Number', 'Position', 'TOI', 'A', 'Shots', 'G', 'Hits',
                                 'PPG', 'PPA', 'PIM', 'FOW', 'FO Taken', 'Takeaways', 'Giveaways', 'SHG',
                                 'SHA', 'Blocks', 'PlusMinus', 'Even TOI', 'PP TOI', 'SH TOI'])

    df = df.drop_duplicates()
    df.to_csv(r'/Users/djmaxey/Desktop/nhldata_' + str(year) + '.csv', index=False)
    print("done")
