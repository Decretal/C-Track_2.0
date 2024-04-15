import 'package:flutter/material.dart';
import 'package:flutter_map/flutter_map.dart';  //Flutter Map
import 'package:latlong2/latlong.dart';         //LatLong Data
import 'package:flutter_svg/flutter_svg.dart';  //using svg images
import 'package:http/http.dart' as http;        //Server Verb
import 'dart:convert';                          //JSON
import 'dart:async';

import 'package:ctrack/navbar.dart';            //Drop Down 

void main() {
  runApp(const MyHomePage());
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});
  @override
  // ignore: library_private_types_in_public_api
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {

//================================= Server Verbindung, LatLong Umwandeln ===================================

  LatLng? markerPosition;
  Timer? timer;

  @override
  void initState() {
    super.initState();
    fetchMarkerPosition(); 
    timer = Timer.periodic(const Duration(seconds: 20), (timer) {
      fetchMarkerPosition();
    });
  }

  @override
  void dispose() {
    timer?.cancel(); // Timer stoppen, wenn das Widget zerstört wird
    super.dispose();
  }

  Future<void> fetchMarkerPosition() async {
    String url = 'http://192.168.136.152:8000/api/trackers/1/positions';   //Server Url
    String token = 'Bearer 1|wwGyHyxhlLxlRHhAuIIGDmGqjZ6saHcQdzjYF9cif7715c39'; //Account

    try {
      var response = await http.get(
        Uri.parse(url),
        headers: {'Authorization': token},
      );

      if (response.statusCode == 200) {
        // Parse response body
        Map<String, dynamic> data = jsonDecode(response.body);
        double latitude = data['latitude'];
        double longitude = data['longitude'];
        setState(() {
          markerPosition = LatLng(latitude, longitude); // Update marker position
        });
      } else {
        // Request failed with an error status code
        // ignore: avoid_print
        print('Request failed with status: ${response.statusCode}');
      }
    } catch (e) {
      // An error occurred
     // ignore: avoid_print
     print('Error: $e');
    }
  }

//==========================================================================================================

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,  //Debug Banner ausblenden

      home: Scaffold(
        extendBodyBehindAppBar: true, //Karte hinter Leiste anzeigen
        drawer: const NavBar(), //Drop Down Menu einfügen

//==================================== Leiste ==============================================================

        appBar: AppBar(
          title: const Text(
            'C-Track 2.0',  //Text
            textDirection: TextDirection.ltr, //Wichtig: sonst "No Directional Widget found"

            style: TextStyle(
              fontSize: 30,
              fontWeight: FontWeight.bold,  //dick machen
              fontStyle: FontStyle.italic,  //kursiv

            ),
          ),

          shape: const RoundedRectangleBorder(  //Leiste abrunden
            borderRadius: BorderRadius.only(
              bottomRight: Radius.circular(25),
              bottomLeft: Radius.circular(25),

            ),
          ),

          centerTitle: true,  //zentrieren
          toolbarHeight: 40,  //Größe der Leiste
          backgroundColor: const Color.fromARGB(255, 89, 225, 59),  //Farbe ändern
        ),

//==========================================================================================================

        body: markerPosition != null ? content() : const Center(child: CircularProgressIndicator()), // Lade Indikator anzeigen & Karte einfügen

      ),
    );
  }


//====================================== Flutter Map =======================================================

  Widget content() {
    return FlutterMap(
      options: const MapOptions(
        // ignore: deprecated_member_use
        initialCenter: LatLng(47.820317, 13.043026),  //Startwert eingeben (Salzburg) 
        // ignore: deprecated_member_use
        zoom: 11, //Anfangszoom
        interactionOptions: InteractionOptions(flags: ~InteractiveFlag.doubleTapZoom),
      ),

      children: [
        openStreetMapTileLayer,
        MarkerLayer(markers: [
          Marker(
            point: markerPosition!, // Markerps. von Server benutzen
            width: 50,
            height: 50,
            alignment: Alignment.centerLeft,
            child: SvgPicture.asset(  //Kuh svg als Icon benutzen
              'assets/cow.svg',
              semanticsLabel: 'Cow',

            ),
          ),
        ]),
      ],
    );
  }

  TileLayer get openStreetMapTileLayer => TileLayer(    //OSM einfügen
    urlTemplate: 'https://tile.openstreetmap.org/{z}/{x}/{y}.png',
    userAgentPackageName: 'dev.fleaflet.flutter_map.example',
  );
}

//==========================================================================================================