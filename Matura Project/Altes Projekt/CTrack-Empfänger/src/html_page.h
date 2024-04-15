const char webpageCode[] PROGMEM =
R"=====(
<!--
    HTML_HELPER 
    const char html_page[] = TO_STRING( 
<!-- -->

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <meta http-equiv="X-UA-Compatible" content="ie=edge">
    <title>CTrack V2</title>
    <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" integrity="sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY=" crossorigin="" />
    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js" integrity="sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo=" crossorigin=""></script>
  </head>
  <body>
    <div id="map" style="width: 600px; height: 600px;"></div>
	<script>
        var obj;
        var map = L.map('map').setView([47.809, 13.055], 13);
        var marker = L.marker([0, 0]).addTo(map);

        L.tileLayer('https://tile.openstreetmap.org/{z}/{x}/{y}.png', {
            maxZoom: 19,
            attribution: '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>'
        }).addTo(map);

        var gateway = `ws://${window.location.hostname}/ws`;
        var websocket;

        window.addEventListener('load', onLoad);

        function onLoad(event) {
            init();
        }

        function init() {
            console.log('Trying to open a WebSocket connection...');
            websocket = new WebSocket(gateway);
            websocket.onmessage = onMessage;
            websocket.onopen = onOpen;
        };

        function onOpen(event) {
            console.log('Connection opened');
        }

        function onMessage(event) {
            console.log(`Received a notification from ${event.origin}`);
            //console.log(event);
            obj = JSON.parse(event.data);
            obj = JSON.parse(obj.LoRaData_received);
            
            lat = obj.latitude / 100;
            long =  obj.longitude / 100;

            if (lat == "" || long == "") {
                console.log("No GPS Data");
                return;
            } 

            marker.setLatLng([lat,long]).update()
        }

    </script>
  </body>
</html>

<!-- ); // end of TO_STRING <!-- -->

)=====";