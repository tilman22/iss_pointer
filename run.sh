#!/bin/bash
while true; do
    json=$(wget -q -O - "http://api.open-notify.org/iss-now.json")
    lat=$(echo $json | jq -r '.iss_position.latitude')
    lon=$(echo $json | jq -r '.iss_position.longitude')
    echo "lat = $lat, lon = $lon"
    ./iss_pointer $lat $lon
    sleep 1
done

