// import React from 'react';
import logo from './logo.svg';
import './App.css';

// import React, { Component } from 'react';
import React, { Component,useEffect, useState } from "react";

import Webcam from "react-webcam";
import 'semantic-ui-css/semantic.min.css'
import { Dropdown } from 'semantic-ui-react'
import socketIOClient from "socket.io-client";
import { Map, TileLayer, Marker, Popup } from 'react-leaflet'
import L from 'leaflet'



import icn from "./map-marker-icon.png"
const ENDPOINT = "http://127.0.0.1:4001";




const LeafletMap = Map;

var myIcon = L.icon({
  iconUrl: {icn},
  iconSize: [35, 46],
  iconAnchor: [17, 46]
});

const customMarker = new L.icon({
  // iconUrl: '/images/icons/location-pointer.svg',
  iconSize: [35, 46],
  iconAnchor: [17, 46]
});
// const { Map: LeafletMap, TileLayer, Marker, Popup } = ReactLeaflet

var LeafIcon = L.Icon.extend({
  options: {
      shadowUrl: 'leaf-shadow.png',
      iconSize:     [38, 95],
      shadowSize:   [50, 64],
      iconAnchor:   [22, 94],
      shadowAnchor: [4, 62],
      popupAnchor:  [-3, -76]
  }
});

// var greenIcon = new LeafIcon({iconUrl: 'leaf-green.png'});



function SimpleExample() {

  // const [response, setResponse] = useState({lat: 0, long:0});

  const [pos, setPos] = useState({
        lat: 41, 
        long: 28
      });





  useEffect(() => {
    const socket = socketIOClient(ENDPOINT);
    socket.on("FromAPI", data => {
      // setResponse(data);
      console.log("DATA", data);
      if(data.lat != 0 && data.lat != pos.lat){
        setPos({lat: data.lat, long: data.long});
      }
    });

  }, []);

  const [viewport, setViewport] = useState({
    center: [pos.lat, pos.long],
    zoom: 5,
    });

 function viewPortChanged(vPort){
    setViewport(vPort);
 }



    
    return (
      
      <LeafletMap  viewport={viewport} onViewportChanged={viewPortChanged}>
        <TileLayer
          attribution='&copy; <a href="http://osm.org/copyright">OpenStreetMap</a> contributors'
          url='https://{s}.tile.osm.org/{z}/{x}/{y}.png'
        />
        <Marker position={[pos.lat, pos.long]}>
          <Popup>
            A pretty CSS3 popup. <br/> Easily customizable.
          </Popup>
        </Marker>
      </LeafletMap>
    );
  
}


const WebcamCapture = () => {
  const [deviceId, setDeviceId] = React.useState({});
  const [devices, setDevices] = React.useState([]);
  const [options, setOptions] = React.useState([]);
  const [selected, setSelected] = React.useState(0);

  const handleDevices = React.useCallback(
    mediaDevices =>{
        setDevices(mediaDevices.filter(({ kind }) => kind === "videoinput"));
        setOptions(mediaDevices.filter(({ kind }) => kind === "videoinput").map((device, key) => {
          return {
            key: device.deviceId,
            text : device.label || `Device ${key + 1}`,
            value : device.deviceId
          };
        }));
    },
    [setDevices, setOptions]
  );

  React.useEffect(
    () => {
      navigator.mediaDevices.enumerateDevices().then(handleDevices);
    },
    [handleDevices]
  );

  return (
    <>
      {/* {devices.map((device, key) => (
          <div>
            <Webcam audio={false} videoConstraints={{ deviceId: device.deviceId }} />
            {device.label || `Device ${key + 1}`}
          </div>

        ))} */}
        <div>
            <Webcam audio={false} videoConstraints={{ deviceId: selected }} />
            {/* {selected.label || `Device ${0}`} */}
        </div>
        {console.log("asd", options)}
        <Dropdown
    placeholder='Select Friend'
    fluid
    selection
    options={options}
    onChange={(error, data) => setSelected(data.value)}
  />
    </>
  );
};

// render(map, document.getElementById('map-container'))



function App() {
  return (
    <div>
    <SimpleExample />
    <WebcamCapture />

    </div>
  );
}

export default App;
