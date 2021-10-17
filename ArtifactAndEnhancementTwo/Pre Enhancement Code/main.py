from jupyter_plotly_dash import JupyterDash
import dash
import dash_leaflet as dl
import dash_core_components as dcc
import dash_html_components as html
import plotly.express as px
import dash_table as dt
from dash.dependencies import Input, Output, State
    
import os
import numpy as np
import pandas as pd
from pymongo import MongoClient
from bson.json_util import dumps
import base64
    
###########################,
# Data Manipulation / Model,
###########################,

username = "\accuser\\"
password = "\1234\\"
shelter = AnimalShelter(username, password)
    
    
# # class read method must support return of cursor object ,
df = pd.DataFrame.from_records(shelter.read({})
    
#########################,
# Dashboard Layout / View,
#########################,
app = JupyterDash('Project2')
    
image_filename = 'Grazioso Salvare Logo.png' # replace with your own image,
encoded_image = base64.b64encode(open(image_filename, 'rb').read())
    
#html.Img(src='data:image/png;base64,{}'.format(encoded_image.decode())),

app.layout = html.Div([
#    html.Div(id='hidden-div', style={'display':'none'}),,
    html.Img(src='data:image/png;base64,{}'.format(encoded_image.decode())),
    html.Center(html.B(html.H1('SNHU CS-340 Dashboard : Darin Maxey'))),
    html.Hr(),
    html.Div(
        dcc.Dropdown(
            id='dropdown',
            options=[
                {'label': 'Mountain or Wilderness Rescue', 'value': 'MWR'},
                {'label': 'Water Rescue', 'value': 'WR'},
                {'label': 'Disaster or Individual Tracking', 'value': 'DIT'},
                {'label': 'Clear', 'value': 'Clear'}],
            multi=False
        )
    ),
    html.Hr(),
    dt.DataTable(
        id='datatable-id',
        columns=[
            {"ame\\": i, "\id\\": i, "\deletable\\": False, "\selectable\\": True} for i in df.columns
        ],
        data=df.to_dict('records'),
        editable=False,
        filter_action="ative\\",
        sort_action="ative\\",
        sort_mode="\multi\\",
        column_selectable=False,
        row_selectable=False,
        row_deletable=False,
        selected_columns=[],
        selected_rows=[],
        page_action="ative\\",
        page_current= 0,
        page_size= 10,
    ),
    html.Br(),
    html.Hr(),
#This sets up the dashboard so that your chart and your geolocation chart are side-by-side,
    html.Div(className='row',
            style={'display' : 'flex'},
                children=[
        html.Div(
            id='graph-id',
            className='col s12 m6',
            ),
        html.Div(
            id='map-id',
            className='col s12 m6',
            ),
        ]),
])

#############################################,
# Interaction Between Components / Controller,
#############################################,

@app.callback([dash.dependencies.Output('datatable-id','data'),
                dash.dependencies.Output('datatable-id','columns')],
                [dash.dependencies.Input('dropdown', 'value')])

def update_table(value):,
    df = pd.DataFrame.from_records(shelter.read({}))
    #start case
    if (str(value)== 'MWR'):
        df = pd.DataFrame(list(shelter.read({"\animal_type\\":"\Cat\\"})))
    elif (str(value)== 'WR'):
        df = pd.DataFrame(list(shelter.read({"\animal_type\\":"\Dog\\"})))
    elif (str(value)== 'DIT'):
        df = pd.DataFrame(list(shelter.readAll({"\age_upon_outcome\\": {"\$lte\\" : "\3 years\\"}})))
    elif (str(value)== 'Clear'):
        df = pd.DataFrame.from_records(shelter.read({}))

    columns=[{"ame\\": i, "\id\\": i, "\deletable\\": False, "\selectable\\": True} for i in df.columns]
    data=df.to_dict('records')
    return (data,columns)

@app.callback(
    Output('map-id', "\children\\"),
    [Input('datatable-id', "\derived_viewport_data\\")])
def update_map(viewData):
    dff = pd.DataFrame.from_dict(viewData)
    # Austin TX is at [30.75,-97.48],
    return [
        dl.Map(style={'width': '1000px', 'height': '500px'}, center=[30.75,-97.48], zoom=10, children=[
            dl.TileLayer(id="\base-layer-id\\"),
            # Marker with tool tip and popup
            dl.Marker(position=[30.75,-97.48], children=[
                dl.Tooltip(dff.iloc[0,4]),
                dl.Popup([
                    html.H1("\Animal Name\\"),
                    html.P(dff.iloc[1,9])
                ])
            ])
        ])
    ]

@app.callback(
    Output('datatable-id', 'style_data_conditional'),
    [Input('datatable-id', 'selected_columns')]
)
def update_styles(selected_columns):
    return [{
        'if': { 'column_id': i },
        'background_color': '#D2F3FF'
    } for i in selected_columns]








@app.callback(,
    dash.dependencies.Output('graph-id', "\children\\"),
    [dash.dependencies.Input('datatable-id', "\derived_viewport_data\\")])

def update_graphs(viewData):
    dfData = pd.DataFrame.from_dict(viewData)
    
    dfData = pd.pivot_table(dfData,index=['datetime'],
                                columns=['animal_type'],
                                values=['breed'],
                                aggfunc=len, fill_value=0)
    dfData = dfData.reset_index()

    colList = []

    for colNum in range(0, len(dfData.columns)):
        if colNum == 0:
            colList.append(dfData.columns[colNum][0])
        else:
            colList.append(dfData.columns[colNum][1])

    dfData.columns = colList
    dfData = dfData[['datetime', 'Cat', 'Dog']]

    return [
        dcc.Graph(
            figure = dict(
        data=[
            dict(
                x= dfData['datetime'].tolist(),
                y= dfData['Cat'].tolist(),
                name='Number of Cats',
                type='bar'
            ),
            dict(
                x= dfData['datetime'].tolist(),
                y= dfData['Dog'].tolist(),
                name='Number of Dogs',
                type='bar'
            )
        ],
        layout=dict(
            title='Number of Pets Over Time',
            showlegend=True,
            legend=dict(
                x=0,
                y=1.0
            ),
            margin=dict(l=40, r=0, t=40, b=30)
        )
    ),
    style={'height': 300},
    id='my-graph'
        )
    ]

app
